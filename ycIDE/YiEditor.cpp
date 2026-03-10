#include "framework.h"
#include "YiEditor.h"
#include "ycIDE.h"
#include "Theme.h"
#include "ResourceExplorer.h"
#include "Resource.h"
#include "Utils.h"
#include "Keyword.h"
#include "LibraryParser.h"
#include "PinyinHelper.h"
#include "OutputPanel.h"
#include "SyntaxChecker.h"  // 添加语法检查器
#include "VisualDesigner.h"  // 可视化设计器（用于获取控件名称和属性）

// 外部变量声明 - 可视化设计器实例
extern VisualDesigner* g_pVisualDesigner;
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <set>
#include <fstream>
#include <gdiplus.h>
#include <imm.h>
#include <windowsx.h>  // GET_X_LPARAM, GET_Y_LPARAM
#include <filesystem>
#pragma comment(lib, "imm32.lib")

using namespace Gdiplus;

// 流程线绘制调试日志(已禁用以提升性能)
static void LogFlowLine(const wchar_t* format, ...) {
    // 空操作 - 避免每次WM_PAINT时对每一行进行文件I/O
}

extern WCHAR szYiEditorClass[];
extern bool g_LeftPanelVisible;  // 左侧AI面板是否可见
extern OutputPanel* g_pOutputPanel;  // 输出面板实例

// 全局语法检查器实例
static SyntaxChecker g_SyntaxChecker;

// 类型补全相关常量
static const int TYPE_COMPLETION_MAX_VISIBLE = 8;
static const int TYPE_COMPLETION_ITEM_HEIGHT = 24;

// 打勾图片
static HBITMAP g_hCheckIcon = nullptr;

// 计算行的嵌套深度
// 返回值：0=不在流程块内，1=第一层嵌套，2=第二层嵌套，以此类推
// 同时返回标记字符的总长度（用于跳过标记获取实际内容）
static int GetNestingDepth(const std::wstring& line, size_t* markerLength = nullptr) {
    if (line.empty()) {
        if (markerLength) *markerLength = 0;
        return 0;
    }
    
    // 检查是否以标记字符开头（\u200C、\u200D 或 \u200B）
    if (line[0] != L'\u200C' && line[0] != L'\u200D' && line[0] != L'\u200B') {
        if (markerLength) *markerLength = 0;
        return 0;
    }
    
    int depth = 0;
    size_t pos = 0;
    
    // 遍历所有标记字符计算深度
    // 格式: [分支标记][(\u200B[分支标记])*]
    // 例如: \u200C = 深度1, \u200C\u200B\u200C = 深度2, \u200C\u200B\u200C\u200B\u200C = 深度3
    while (pos < line.length()) {
        wchar_t ch = line[pos];
        if (ch == L'\u200C' || ch == L'\u200D') {
            depth++;
            pos++;
        } else if (ch == L'\u200B') {
            // \u200B 是深度分隔符，跳过但不增加深度
            pos++;
        } else {
            // 遇到非标记字符，停止
            break;
        }
    }
    
    if (markerLength) *markerLength = pos;
    return depth;
}

// 构建指定嵌套深度的标记前缀
// branchType: L'\u200C' 表示条件达成分支，L'\u200D' 表示否则分支
// 格式: 深度1=\u200C, 深度2=\u200C\u200B\u200C, 深度3=\u200C\u200B\u200C\u200B\u200C
static std::wstring BuildNestingMarker(int depth, wchar_t branchType = L'\u200C') {
    if (depth <= 0) return L"";
    
    std::wstring marker;
    marker += branchType;  // 第一层标记
    for (int i = 1; i < depth; i++) {
        marker += L'\u200B';  // 深度分隔符
        marker += branchType;  // 每层的分支类型标记
    }
    return marker;
}

// ===== 性能优化：流程块索引预计算 =====
// 在内容变化时一次性扫描所有流程控制命令的配对关系
// 实时查找紧跟的下一个流程控制命令（只检查块末后的几行，O(1)）
// searchFrom: 从哪一行开始查找
// depth: 当前命令的嵌套深度
// 返回: 下一个流程控制命令的行号，-1表示没有
static int FindNextFlowCmd(EditorDocument* doc, int searchFrom, int depth) {
    if (searchFrom < 0 || searchFrom >= (int)doc->lines.size()) return -1;
    for (int nj = searchFrom; nj < (int)doc->lines.size(); nj++) {
        const std::wstring& nLine = doc->lines[nj];
        if (nLine.empty()) return -1;  // 空行视为间隔，不连接
        if (nLine.length() > 0 && nLine[0] == L'\u2060') continue;  // 参数行属于上方命令，跳过
        int nDepth = GetNestingDepth(nLine);
        if (nDepth != depth) break;
        // 检查是否是流程控制命令
        std::wstring nt = nLine;
        size_t ns = 0;
        while (ns < nt.length() && (nt[ns] == L'\u200C' || nt[ns] == L'\u200D' || nt[ns] == L'\u200B')) ns++;
        if (ns < nt.length()) nt = nt.substr(ns);
        // 排除循环首/尾命令
        bool isLoop = (nt.find(L".判断循环") == 0 || nt.find(L"判断循环") == 0 ||
                       nt.find(L".循环判断") == 0 || nt.find(L"循环判断") == 0 ||
                       nt.find(L".计次循环") == 0 || nt.find(L"计次循环") == 0 ||
                       nt.find(L".变量循环") == 0 || nt.find(L"变量循环") == 0);
        if (isLoop) break;
        // 匹配常见流程控制命令
        if (nt.find(L".判断(") == 0 || nt.find(L".判断 (") == 0 ||
            nt.find(L"判断(") == 0 || nt.find(L"判断 (") == 0 ||
            nt.find(L".如果(") == 0 || nt.find(L".如果 (") == 0 ||
            nt.find(L"如果(") == 0 || nt.find(L"如果 (") == 0 ||
            nt.find(L".如果真") == 0 || nt.find(L"如果真") == 0 ||
            IsFlowControlLine(nLine)) {
            return nj;
        }
        break;
    }
    return -1;
}

// 避免每次WM_PAINT时O(n²)搜索
static void BuildFlowBlockIndex(EditorDocument* doc) {
    doc->flowBlockIndex.clear();
    
    for (size_t i = 0; i < doc->lines.size(); i++) {
        const std::wstring& line = doc->lines[i];
        if (line.empty()) continue;
        
        // 跳过参数行
        if (line[0] == L'\u2060') continue;
        
        // 检查是否是流程控制行
        if (!IsFlowControlLine(line)) continue;
        
        // 获取嵌套深度和trimmed内容
        size_t markerLen = 0;
        int depth = GetNestingDepth(line, &markerLen);
        std::wstring trimmedLine = (markerLen < line.length()) ? line.substr(markerLen) : line;
        
        EditorDocument::FlowBlockInfo info = { -1, -1, -1, -1, -1 };
        
        // 判断命令类型（带点号和不带点号都匹配）
        bool isIfElse = (trimmedLine.find(L".如果(") == 0 || trimmedLine.find(L".如果 (") == 0 ||
                         trimmedLine.find(L".判断(") == 0 || trimmedLine.find(L".判断 (") == 0 ||
                         trimmedLine.find(L"如果(") == 0 || trimmedLine.find(L"如果 (") == 0 ||
                         trimmedLine.find(L"判断(") == 0 || trimmedLine.find(L"判断 (") == 0);
        bool isLoopTail = (trimmedLine.find(L".判断循环尾") == 0 || trimmedLine.find(L".循环判断尾") == 0 ||
                           trimmedLine.find(L".计次循环尾") == 0 || trimmedLine.find(L".变量循环尾") == 0 ||
                           trimmedLine.find(L"判断循环尾") == 0 || trimmedLine.find(L"循环判断尾") == 0 ||
                           trimmedLine.find(L"计次循环尾") == 0 || trimmedLine.find(L"变量循环尾") == 0);
        bool isLoopHead = (trimmedLine.find(L".判断循环首") == 0 || trimmedLine.find(L".循环判断首") == 0 ||
                           trimmedLine.find(L".计次循环首") == 0 || trimmedLine.find(L".变量循环首") == 0 ||
                           trimmedLine.find(L"判断循环首") == 0 || trimmedLine.find(L"循环判断首") == 0 ||
                           trimmedLine.find(L"计次循环首") == 0 || trimmedLine.find(L"变量循环首") == 0);
        
        if (isIfElse) {
            // 查找否则分支和流程块结束
            int elseLineIdx = -1;
            int lastIndentedLine = -1;
            bool foundTrueBranch = false;
            
            for (size_t j = i + 1; j < doc->lines.size(); j++) {
                const std::wstring& checkLine = doc->lines[j];
                if (checkLine.empty()) continue;
                if (checkLine.length() > 0 && checkLine[0] == L'\u2060') continue;
                
                int checkDepth = GetNestingDepth(checkLine);
                
                if (elseLineIdx == -1) {
                    if (checkDepth == depth + 1) {
                        size_t pos = 0;
                        wchar_t lastBranchType = L'\0';
                        while (pos < checkLine.length()) {
                            wchar_t ch = checkLine[pos];
                            if (ch == L'\u200C' || ch == L'\u200D') { lastBranchType = ch; pos++; }
                            else if (ch == L'\u200B') { pos++; }
                            else break;
                        }
                        
                        if (lastBranchType == L'\u200D') {
                            elseLineIdx = (int)j;
                            lastIndentedLine = (int)j;
                        } else if (lastBranchType == L'\u200C') {
                            foundTrueBranch = true;
                        } else if (foundTrueBranch) {
                            elseLineIdx = (int)j;
                            lastIndentedLine = (int)j;
                        } else {
                            foundTrueBranch = true;
                        }
                    } else if (checkDepth > depth + 1) {
                        continue;
                    } else {
                        break;
                    }
                } else {
                    if (checkDepth == depth + 1) {
                        size_t pos2 = 0;
                        wchar_t lastBT = L'\0';
                        while (pos2 < checkLine.length()) {
                            wchar_t ch = checkLine[pos2];
                            if (ch == L'\u200C' || ch == L'\u200D') { lastBT = ch; pos2++; }
                            else if (ch == L'\u200B') { pos2++; }
                            else break;
                        }
                        if (lastBT == L'\u200D') {
                            lastIndentedLine = (int)j;
                        } else {
                            break;
                        }
                    } else if (checkDepth > depth + 1) {
                        continue;
                    } else {
                        break;
                    }
                }
            }
            
            info.elseLineIdx = elseLineIdx;
            info.lastIndentedLine = lastIndentedLine;
            
            // 如果没有if-else结构，计算block结束位置
            if (elseLineIdx < 0) {
                int endLine = (int)i;
                for (size_t j = i + 1; j < doc->lines.size(); j++) {
                    const std::wstring& checkLine = doc->lines[j];
                    if (checkLine.length() > 0 && checkLine[0] == L'\u2060') { endLine = (int)j; continue; }
                    int cd = GetNestingDepth(checkLine);
                    if (cd > depth) { endLine = (int)j; }
                    else if (checkLine.empty()) { continue; }
                    else { break; }
                }
                info.blockEndLine = endLine;
            }
            // nextFlowCmdLine 不在这里计算，由渲染代码实时计算（避免缓存过期问题）
        } else if (isLoopTail) {
            // 向上查找对应的循环首
            std::wstring targetHead, targetHeadNoDot;
            if (trimmedLine.find(L"判断循环尾") != std::wstring::npos) { targetHead = L".判断循环首"; targetHeadNoDot = L"判断循环首"; }
            else if (trimmedLine.find(L"循环判断尾") != std::wstring::npos) { targetHead = L".循环判断首"; targetHeadNoDot = L"循环判断首"; }
            else if (trimmedLine.find(L"计次循环尾") != std::wstring::npos) { targetHead = L".计次循环首"; targetHeadNoDot = L"计次循环首"; }
            else { targetHead = L".变量循环首"; targetHeadNoDot = L"变量循环首"; }
            
            for (int j = (int)i - 1; j >= 0; j--) {
                const std::wstring& checkLine = doc->lines[j];
                if (checkLine.length() > 0 && checkLine[0] == L'\u2060') continue;
                if (checkLine.empty()) continue;
                int checkD = GetNestingDepth(checkLine);
                if (checkD < depth) break;
                if (checkD > depth) continue;
                size_t cStart = 0;
                while (cStart < checkLine.length() && (checkLine[cStart] == L'\u200C' || checkLine[cStart] == L'\u200D' || checkLine[cStart] == L'\u200B')) cStart++;
                std::wstring ct = (cStart < checkLine.length()) ? checkLine.substr(cStart) : checkLine;
                if (ct.find(targetHead) == 0 || ct.find(targetHeadNoDot) == 0) {
                    info.loopHeadLine = j;
                    break;
                }
            }
        } else if (isLoopHead) {
            // 循环首：不需要额外索引
        } else {
            // 其他流程控制（如果真等）：计算block结束位置
            int endLine = (int)i;
            for (size_t j = i + 1; j < doc->lines.size(); j++) {
                const std::wstring& checkLine = doc->lines[j];
                if (checkLine.length() > 0 && checkLine[0] == L'\u2060') { endLine = (int)j; continue; }
                int cd = GetNestingDepth(checkLine);
                if (cd > depth) { endLine = (int)j; }
                else if (checkLine.empty()) { continue; }
                else { break; }
            }
            info.blockEndLine = endLine;
            // nextFlowCmdLine 不在这里计算，由渲染代码实时计算
        }
        
        doc->flowBlockIndex[(int)i] = info;
    }
    
    doc->flowBlockDirty = false;
}

// ===== 性能优化：文本宽度缓存 =====
// 避免每帧大量调用 GetTextExtentPoint32W
static int CachedGetTextWidth(HDC hdc, EditorDocument* doc, const std::wstring& text) {
    if (text.empty()) return 0;
    
    auto it = doc->textWidthCache.find(text);
    if (it != doc->textWidthCache.end()) {
        return it->second;
    }
    
    SIZE sz;
    GetTextExtentPoint32W(hdc, text.c_str(), (int)text.length(), &sz);
    
    // 缓存上限防止内存膨胀
    if (doc->textWidthCache.size() < 50000) {
        doc->textWidthCache[text] = sz.cx;
    }
    return sz.cx;
}

// 获取命令的参数列表
static std::vector<LibraryParameter> GetCommandParameters(const std::wstring& cmdName) {
    std::vector<LibraryParameter> params;
    
    // 从支持库中查找命令
    const LibraryCommand* cmd = LibraryParser::GetInstance().FindCommand(cmdName);
    if (cmd) {
        return cmd->parameters;
    }
    
    return params;
}

// 从行中提取命令名称
static std::wstring ExtractCommandName(const std::wstring& line) {
    if (line.empty()) return L"";
    
    // 跳过特殊标记字符（包括嵌套深度分隔符\u200B）
    size_t start = 0;
    while (start < line.length() && (line[start] == L'\u200C' || line[start] == L'\u200D' || line[start] == L'\u200B' || line[start] == L' ' || line[start] == L'\t')) {
        start++;
    }
    
    // 跳过点号（流程控制命令）
    if (start < line.length() && line[start] == L'.') {
        start++;
    }
    
    // 查找命令名称（到左括号或行尾）
    size_t end = line.find(L'(', start);
    if (end == std::wstring::npos) {
        end = line.length();
    }
    
    // 提取命令名称并去除前后空格
    std::wstring cmdName = line.substr(start, end - start);
    
    // 去除末尾空格
    while (!cmdName.empty() && (cmdName.back() == L' ' || cmdName.back() == L'\t')) {
        cmdName.pop_back();
    }
    
    return cmdName;
}

// 更新输出面板的命令提示信息
static void UpdateCommandHint(const std::wstring& line, int lineNumber) {
    if (!g_pOutputPanel) return;
    
    // 检查是否是参数行（以特殊标记开头）
    if (!line.empty() && line[0] == L'\u2060') {
        // 参数行，不更新提示
        return;
    }
    
    // 检查是否是表格行（包含Tab）
    if (line.find(L'\t') != std::wstring::npos) {
        // 表格行（子程序定义、参数定义、变量定义等），清除提示
        g_pOutputPanel->ClearHint();
        return;
    }
    
    // 提取命令名称
    std::wstring cmdName = ExtractCommandName(line);
    if (cmdName.empty()) {
        g_pOutputPanel->ClearHint();
        return;
    }
    
    // 从支持库中查找命令信息
    const LibraryCommand* cmd = LibraryParser::GetInstance().FindCommand(cmdName);
    if (cmd) {
        CommandHint hint;
        hint.commandName = cmd->chineseName;
        
        // 构建调用格式：〈返回值类型〉 命令名 （参数类型 参数名, ...） - 支持库名->类别
        std::wstring callFormat = L"调用格式： 〈";
        callFormat += cmd->returnType;  // 直接使用支持库中的类型（已经正确格式化）
        callFormat += L"〉 " + cmd->chineseName + L" （";
        
        for (size_t i = 0; i < cmd->parameters.size(); i++) {
            if (i > 0) callFormat += L"，";
            callFormat += cmd->parameters[i].type + L" " + cmd->parameters[i].name;
        }
        callFormat += L"） - " + cmd->library + L"->" + cmd->category;
        
        hint.syntax = callFormat;
        
        // 英文名称
        if (!cmd->englishName.empty()) {
            hint.syntax += L"\r\n英文名称：" + cmd->englishName;
        }
        
        // 命令说明
        hint.description = cmd->description;
        
        // 添加参数详细信息
        for (size_t i = 0; i < cmd->parameters.size(); i++) {
            const auto& param = cmd->parameters[i];
            // 使用 typeWithEnglish（如：逻辑型（bool）），如果为空则使用 type
            std::wstring typeDisplay = param.typeWithEnglish.empty() ? param.type : param.typeWithEnglish;
            std::wstring paramInfo = L"参数<";
            paramInfo += std::to_wstring(i + 1);
            paramInfo += L">的名称为\"";
            paramInfo += param.name;
            paramInfo += L"\"，类型为\"";
            paramInfo += typeDisplay;
            paramInfo += L"\"。";
            if (!param.description.empty()) {
                paramInfo += param.description;
            }
            hint.parameters.push_back(paramInfo);
        }
        
        // 操作系统需求
        hint.returnValue = L"Windows";
        
        g_pOutputPanel->SetHint(hint);
    } else {
        // 没有找到命令信息，显示基本信息
        CommandHint hint;
        hint.commandName = cmdName;
        hint.description = L"未找到命令帮助信息";
        g_pOutputPanel->SetHint(hint);
    }
}

// 检查行是否是隐藏命令（.否则、.如果结束等）
// IsHiddenCommand函数已删除 - 不再需要隐藏命令

// 加载打勾PNG图片
static HBITMAP LoadCheckIcon() {
    if (g_hCheckIcon) return g_hCheckIcon;
    
    // 获取exe所在目录
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    std::wstring path(exePath);
    size_t lastSlash = path.find_last_of(L"\\");
    if (lastSlash != std::wstring::npos) {
        path = path.substr(0, lastSlash + 1);
    }
    path += L"img\\打勾.png";
    
    Bitmap* bitmap = new Bitmap(path.c_str());
    if (bitmap->GetLastStatus() != Ok) {
        delete bitmap;
        return nullptr;
    }
    bitmap->GetHBITMAP(Color(0, 0, 0, 0), &g_hCheckIcon);
    delete bitmap;
    return g_hCheckIcon;
}

// 获取光标位置的当前单词
static std::wstring GetCurrentWord(const std::wstring& line, int cursorCol, int& wordStart) {
    wordStart = cursorCol;
    int wordEnd = cursorCol;
    
    // 向前找单词开始
    while (wordStart > 0 && (iswalnum(line[wordStart - 1]) || line[wordStart - 1] == L'_')) {
        wordStart--;
    }
    
    // 向后找单词结束
    while (wordEnd < (int)line.length() && (iswalnum(line[wordEnd]) || line[wordEnd] == L'_')) {
        wordEnd++;
    }
    
    if (wordStart < wordEnd) {
        return line.substr(wordStart, wordEnd - wordStart);
    }
    return L"";
}

// 获取点号前的单词（用于属性补全）
static std::wstring GetWordBeforeDot(const std::wstring& line, int dotPos) {
    if (dotPos <= 0) return L"";
    
    int wordEnd = dotPos;
    int wordStart = dotPos;
    
    // 向前找单词开始
    while (wordStart > 0 && (iswalnum(line[wordStart - 1]) || line[wordStart - 1] == L'_')) {
        wordStart--;
    }
    
    if (wordStart < wordEnd) {
        return line.substr(wordStart, wordEnd - wordStart);
    }
    return L"";
}

// 获取可视化设计器中的控件补全项
static std::vector<CompletionItem> GetDesignerControlCompletions(const std::wstring& input) {
    std::vector<CompletionItem> completions;
    
    if (!g_pVisualDesigner || input.empty()) return completions;
    
    // 辅助函数：转小写（支持所有Unicode字符）
    auto toLowerUnicode = [](const std::wstring& str) -> std::wstring {
        std::wstring result = str;
        if (!result.empty()) {
            CharLowerBuffW(const_cast<wchar_t*>(result.c_str()), (DWORD)result.length());
        }
        return result;
    };
    
    std::wstring lowerInput = toLowerUnicode(input);
    
    // 辅助函数：判断字符是否为ASCII字母
    auto isAsciiAlpha = [](wchar_t ch) -> bool {
        return (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z');
    };
    
    // 辅助函数：中英混合智能匹配
    auto mixedMatch = [&toLowerUnicode, &isAsciiAlpha](const std::wstring& inputStr, const std::wstring& target) -> bool {
        std::wstring lowerInputStr = toLowerUnicode(inputStr);
        std::wstring lowerTarget = toLowerUnicode(target);
        
        size_t inputPos = 0;
        size_t targetPos = 0;
        
        while (inputPos < lowerInputStr.length() && targetPos < lowerTarget.length()) {
            wchar_t inputChar = lowerInputStr[inputPos];
            wchar_t targetChar = lowerTarget[targetPos];
            
            if (inputChar == targetChar) {
                inputPos++;
                targetPos++;
            } else if (isAsciiAlpha(inputChar)) {
                std::wstring targetCharStr(1, target[targetPos]);
                std::wstring targetPinyin = PinyinHelper::GetStringPinyin(targetCharStr);
                std::wstring targetInitial = PinyinHelper::GetStringInitials(targetCharStr);
                std::wstring lowerPinyin = toLowerUnicode(targetPinyin);
                std::wstring lowerInitial = toLowerUnicode(targetInitial);
                
                if (lowerPinyin.length() > 0 && inputPos + lowerPinyin.length() <= lowerInputStr.length()) {
                    std::wstring inputSubstr = lowerInputStr.substr(inputPos, lowerPinyin.length());
                    if (inputSubstr == lowerPinyin) {
                        inputPos += lowerPinyin.length();
                        targetPos++;
                    }
                    else if (!lowerInitial.empty() && lowerInitial[0] == inputChar) {
                        inputPos++;
                        targetPos++;
                    } else {
                        return false;
                    }
                }
                else if (!lowerInitial.empty() && lowerInitial[0] == inputChar) {
                    inputPos++;
                    targetPos++;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }
        
        return inputPos == lowerInputStr.length();
    };
    
    // 遍历可视化设计器中的所有控件
    const FormInfo& formInfo = g_pVisualDesigner->GetFormInfo();
    for (const auto& ctrl : formInfo.controls) {
        const std::wstring& ctrlName = ctrl->name;
        if (ctrlName.empty()) continue;
        
        int score = 0;
        std::wstring lowerCtrlName = toLowerUnicode(ctrlName);
        
        // 1. 中文匹配
        size_t pos = lowerCtrlName.find(lowerInput);
        if (pos != std::wstring::npos) {
            if (pos == 0) {
                if (ctrlName == input) score = 1000;  // 完全匹配
                else score = 500;  // 前缀匹配
            } else {
                score = 100;  // 包含匹配
            }
            score -= (int)(ctrlName.length() - input.length());
        }
        
        // 2. 拼音匹配
        if (score == 0) {
            std::wstring pinyin = PinyinHelper::GetStringPinyin(ctrlName);
            std::wstring initials = PinyinHelper::GetStringInitials(ctrlName);
            std::wstring lowerPinyin = toLowerUnicode(pinyin);
            std::wstring lowerInitials = toLowerUnicode(initials);
            
            // 全拼匹配
            if (!lowerPinyin.empty() && lowerPinyin.length() >= lowerInput.length() 
                && lowerPinyin.substr(0, lowerInput.length()) == lowerInput) {
                if (lowerPinyin == lowerInput) score = 800;
                else score = 400;
                score -= (int)(lowerPinyin.length() - lowerInput.length());
            }
            // 首字母匹配
            else if (!lowerInitials.empty() && lowerInitials.length() >= lowerInput.length()
                     && lowerInitials.substr(0, lowerInput.length()) == lowerInput) {
                if (lowerInitials == lowerInput) score = 600;
                else score = 300;
                score -= (int)(lowerInitials.length() - lowerInput.length());
            }
        }
        
        // 3. 中英混合智能匹配
        if (score == 0 && mixedMatch(input, ctrlName)) {
            score = 550;
        }
        
        if (score > 0) {
            CompletionItem item;
            item.displayText = ctrlName + L" (" + ctrl->type + L")";
            item.insertText = ctrlName;
            item.description = L"窗体控件 - " + ctrl->type;
            item.type = KW_OTHER;
            item.isLibraryCommand = false;
            item.score = score;
            completions.push_back(item);
        }
    }
    
    // 按分数排序
    std::sort(completions.begin(), completions.end(), 
        [](const CompletionItem& a, const CompletionItem& b) {
            return a.score > b.score;
        });
    
    return completions;
}

// 获取控件类型的属性补全项
static std::vector<CompletionItem> GetControlPropertyCompletions(const std::wstring& controlName) {
    std::vector<CompletionItem> completions;
    
    if (!g_pVisualDesigner || controlName.empty()) return completions;
    
    // 查找控件
    const FormInfo& formInfo = g_pVisualDesigner->GetFormInfo();
    std::wstring controlType;
    for (const auto& ctrl : formInfo.controls) {
        if (ctrl->name == controlName) {
            controlType = ctrl->type;
            break;
        }
    }
    
    if (controlType.empty()) return completions;
    
    // 从支持库获取该控件类型的属性列表
    const WindowUnitInfo* unitInfo = LibraryParser::GetInstance().FindWindowUnit(controlType);
    if (!unitInfo) return completions;
    
    // 添加属性到补全列表
    for (const auto& prop : unitInfo->properties) {
        if (prop.isHidden) continue;  // 跳过隐藏属性
        
        CompletionItem item;
        item.displayText = prop.name;
        if (!prop.englishName.empty()) {
            item.displayText += L" (" + prop.englishName + L")";
        }
        item.insertText = prop.name;
        item.description = prop.description;
        item.type = KW_OTHER;
        item.isLibraryCommand = false;
        item.score = 1000;  // 属性优先显示
        completions.push_back(item);
    }
    
    return completions;
}

// 检查一行是否是赋值语句（包含 ＝ 或 = 但不在括号内）
// 返回 true 时，通过 lhs 和 rhs 返回左值和右值
static bool IsAssignmentLine(const std::wstring& line, std::wstring* lhs = nullptr, std::wstring* rhs = nullptr) {
    if (line.empty()) return false;
    // 跳过标记字符
    size_t start = 0;
    while (start < line.length() && (line[start] == L'\u200C' || line[start] == L'\u200D' || line[start] == L'\u200B')) {
        start++;
    }
    // 跳过点号前缀（流程控制命令不算赋值）  
    if (start < line.length() && line[start] == L'.') return false;
    // 跳过参数行和表格行
    if (start < line.length() && line[start] == L'\u2060') return false;
    if (line.find(L'\t') != std::wstring::npos) return false;
    
    // 查找 ＝ 或 = （不在括号内）
    int parenDepth = 0;
    for (size_t i = start; i < line.length(); i++) {
        if (line[i] == L'(' || line[i] == L'（') parenDepth++;
        else if (line[i] == L')' || line[i] == L'）') parenDepth--;
        else if (parenDepth == 0) {
            bool isFullWidthEq = (line[i] == L'\uFF1D'); // ＝
            bool isHalfWidthEq = (line[i] == L'=');
            if (isFullWidthEq || isHalfWidthEq) {
                if (lhs || rhs) {
                    std::wstring left = line.substr(start, i - start);
                    // 去除前后空格
                    while (!left.empty() && (left.back() == L' ' || left.back() == L'\t')) left.pop_back();
                    while (!left.empty() && (left.front() == L' ' || left.front() == L'\t')) left.erase(0, 1);
                    
                    std::wstring right = (i + 1 < line.length()) ? line.substr(i + 1) : L"";
                    while (!right.empty() && (right.front() == L' ' || right.front() == L'\t')) right.erase(0, 1);
                    while (!right.empty() && (right.back() == L' ' || right.back() == L'\t')) right.pop_back();
                    
                    if (lhs) *lhs = left;
                    if (rhs) *rhs = right;
                }
                return true;
            }
        }
    }
    return false;
}

// 检查赋值语句的左值是否引用了一个有效的可视化设计器组件属性（组件名.属性名）
static bool IsValidAssignmentTarget(const std::wstring& line) {
    std::wstring lhs;
    if (!IsAssignmentLine(line, &lhs, nullptr)) return false;
    
    // lhs 必须是 组件名.属性名 的形式
    size_t dotPos = lhs.find(L'.');
    if (dotPos == std::wstring::npos || dotPos == 0) return false;
    
    std::wstring controlName = lhs.substr(0, dotPos);
    
    // 检查组件是否存在于可视化设计器中
    if (!g_pVisualDesigner) return false;
    const FormInfo& formInfo = g_pVisualDesigner->GetFormInfo();
    for (const auto& ctrl : formInfo.controls) {
        if (ctrl->name == controlName) {
            return true;
        }
    }
    return false;
}

// 运算表达式信息
struct ExprOperatorInfo {
    wchar_t op;             // 半角运算符: + - * /
    wchar_t fullWidthOp;    // 全角运算符: ＋ － × ÷
    std::wstring fullWidthOpStr; // 多字符全角运算符（如≥ ≤）
    std::wstring param1Name; // 第一个参数名
    std::wstring param2Name; // 第二个参数名
    std::wstring leftOperand;  // 左操作数
    std::wstring rightOperand; // 右操作数
    int opLength;            // 运算符字符长度（用于多字符运算符如>=）
};

// 获取运算符的参数名称
static ExprOperatorInfo GetOperatorInfo(wchar_t op, wchar_t nextOp = 0) {
    ExprOperatorInfo info = {};
    info.op = op;
    info.opLength = 1;
    
    // 先检查多字符运算符
    if ((op == L'>' || op == L'\uFF1E') && (nextOp == L'=' || nextOp == L'\uFF1D')) {
        info.fullWidthOp = L'\u2265';  // ≥
        info.fullWidthOpStr = L"\u2265";
        info.param1Name = L"被比较值";
        info.param2Name = L"比较值";
        info.opLength = 2;
        return info;
    }
    if ((op == L'<' || op == L'\uFF1C') && (nextOp == L'=' || nextOp == L'\uFF1D')) {
        info.fullWidthOp = L'\u2264';  // ≤
        info.fullWidthOpStr = L"\u2264";
        info.param1Name = L"被比较值";
        info.param2Name = L"比较值";
        info.opLength = 2;
        return info;
    }
    if ((op == L'!' || op == L'\uFF01') && (nextOp == L'=' || nextOp == L'\uFF1D')) {
        info.fullWidthOp = L'\u2260';  // ≠
        info.fullWidthOpStr = L"\u2260";
        info.param1Name = L"被比较值";
        info.param2Name = L"比较值";
        info.opLength = 2;
        return info;
    }
    if ((op == L'=' || op == L'\uFF1D') && (nextOp == L'=' || nextOp == L'\uFF1D')) {
        info.fullWidthOp = L'\uFF1D';  // ＝
        info.fullWidthOpStr = L"\uFF1D";
        info.param1Name = L"被比较值";
        info.param2Name = L"比较值";
        info.opLength = 2;
        return info;
    }
    
    // 单字符运算符
    switch (op) {
        case L'+': case L'\uFF0B':
            info.fullWidthOp = L'\uFF0B';
            info.fullWidthOpStr = L"\uFF0B";
            info.param1Name = L"被加数或文本或字节集";
            info.param2Name = L"加数或文本或字节集";
            break;
        case L'-': case L'\uFF0D':
            info.fullWidthOp = L'\uFF0D';
            info.fullWidthOpStr = L"\uFF0D";
            info.param1Name = L"被减数";
            info.param2Name = L"减数";
            break;
        case L'*': case L'\u00D7':
            info.fullWidthOp = L'\u00D7';
            info.fullWidthOpStr = L"\u00D7";
            info.param1Name = L"被乘数";
            info.param2Name = L"乘数";
            break;
        case L'/': case L'\u00F7':
            info.fullWidthOp = L'\u00F7';
            info.fullWidthOpStr = L"\u00F7";
            info.param1Name = L"被除数";
            info.param2Name = L"除数";
            break;
        case L'>': case L'\uFF1E':
            info.fullWidthOp = L'\uFF1E';
            info.fullWidthOpStr = L"\uFF1E";
            info.param1Name = L"被比较值";
            info.param2Name = L"比较值";
            break;
        case L'<': case L'\uFF1C':
            info.fullWidthOp = L'\uFF1C';
            info.fullWidthOpStr = L"\uFF1C";
            info.param1Name = L"被比较值";
            info.param2Name = L"比较值";
            break;
        case L'\u2265':  // ≥
            info.fullWidthOp = L'\u2265';
            info.fullWidthOpStr = L"\u2265";
            info.param1Name = L"被比较值";
            info.param2Name = L"比较值";
            break;
        case L'\u2264':  // ≤
            info.fullWidthOp = L'\u2264';
            info.fullWidthOpStr = L"\u2264";
            info.param1Name = L"被比较值";
            info.param2Name = L"比较值";
            break;
        case L'\u2260':  // ≠
            info.fullWidthOp = L'\u2260';
            info.fullWidthOpStr = L"\u2260";
            info.param1Name = L"被比较值";
            info.param2Name = L"比较值";
            break;
        default:
            info.fullWidthOp = op;
            info.fullWidthOpStr = std::wstring(1, op);
            info.param1Name = L"参数1";
            info.param2Name = L"参数2";
            break;
    }
    return info;
}

// 解析表达式中最外层的运算符（不在括号内），返回是否找到
// 按优先级从低到高查找：先找比较运算符，再找加减，再找乘除
static bool ParseExpressionOperator(const std::wstring& expr, ExprOperatorInfo& info) {
    int parenDepth = 0;
    bool inString = false;
    
    // 最先查找比较运算符（最低优先级）
    int lastCmpPos = -1;
    wchar_t lastCmpOp = 0;
    int lastCmpLen = 1;  // 运算符字符长度
    for (size_t i = 0; i < expr.length(); i++) {
        if (expr[i] == L'"') { inString = !inString; continue; }
        if (inString) continue;
        if (expr[i] == L'(' || expr[i] == L'\uFF08') parenDepth++;
        else if (expr[i] == L')' || expr[i] == L'\uFF09') parenDepth--;
        else if (parenDepth == 0) {
            wchar_t nextCh = (i + 1 < expr.length()) ? expr[i + 1] : 0;
            // 检查多字符比较运算符: >=, <=, !=, ==
            if ((expr[i] == L'>' || expr[i] == L'\uFF1E') && (nextCh == L'=' || nextCh == L'\uFF1D')) {
                lastCmpPos = (int)i; lastCmpOp = expr[i]; lastCmpLen = 2;
            } else if ((expr[i] == L'<' || expr[i] == L'\uFF1C') && (nextCh == L'=' || nextCh == L'\uFF1D')) {
                lastCmpPos = (int)i; lastCmpOp = expr[i]; lastCmpLen = 2;
            } else if ((expr[i] == L'!' || expr[i] == L'\uFF01') && (nextCh == L'=' || nextCh == L'\uFF1D')) {
                lastCmpPos = (int)i; lastCmpOp = expr[i]; lastCmpLen = 2;
            } else if ((expr[i] == L'=' || expr[i] == L'\uFF1D') && (nextCh == L'=' || nextCh == L'\uFF1D')) {
                lastCmpPos = (int)i; lastCmpOp = expr[i]; lastCmpLen = 2;
            }
            // 单字符全角比较运算符: ≥ ≤ ≠
            else if (expr[i] == L'\u2265' || expr[i] == L'\u2264' || expr[i] == L'\u2260') {
                lastCmpPos = (int)i; lastCmpOp = expr[i]; lastCmpLen = 1;
            }
            // 单字符比较运算符: > <（不带=）
            else if (expr[i] == L'>' || expr[i] == L'\uFF1E' || expr[i] == L'<' || expr[i] == L'\uFF1C') {
                // 确保下一个字符不是=（已在上面处理）
                lastCmpPos = (int)i; lastCmpOp = expr[i]; lastCmpLen = 1;
            }
        }
    }
    
    if (lastCmpPos >= 0) {
        wchar_t nextCh = (lastCmpPos + 1 < (int)expr.length()) ? expr[lastCmpPos + 1] : 0;
        info = GetOperatorInfo(lastCmpOp, nextCh);
        info.opLength = lastCmpLen;
        std::wstring left = expr.substr(0, lastCmpPos);
        std::wstring right = (lastCmpPos + lastCmpLen < (int)expr.length()) ? expr.substr(lastCmpPos + lastCmpLen) : L"";
        while (!left.empty() && (left.back() == L' ' || left.back() == L'\t')) left.pop_back();
        while (!left.empty() && (left.front() == L' ' || left.front() == L'\t')) left.erase(0, 1);
        while (!right.empty() && (right.front() == L' ' || right.front() == L'\t')) right.erase(0, 1);
        while (!right.empty() && (right.back() == L' ' || right.back() == L'\t')) right.pop_back();
        info.leftOperand = left;
        info.rightOperand = right;
        return true;
    }
    
    // 再查找加减（低优先级，从右往左找确保左结合）
    int lastAddSubPos = -1;
    wchar_t lastAddSubOp = 0;
    for (size_t i = 0; i < expr.length(); i++) {
        if (expr[i] == L'"') { inString = !inString; continue; }
        if (inString) continue;
        if (expr[i] == L'(' || expr[i] == L'\uFF08') parenDepth++;
        else if (expr[i] == L')' || expr[i] == L'\uFF09') parenDepth--;
        else if (parenDepth == 0) {
            // 跳过第一个字符位置（可能是负号）
            if (i == 0) continue;
            if (expr[i] == L'+' || expr[i] == L'\uFF0B' || expr[i] == L'-' || expr[i] == L'\uFF0D') {
                // 检查前一个非空格字符是否也是运算符（排除负号情况，如 3 + -5）
                int prevIdx = (int)i - 1;
                while (prevIdx >= 0 && expr[prevIdx] == L' ') prevIdx--;
                if (prevIdx >= 0) {
                    wchar_t prev = expr[prevIdx];
                    if (prev != L'+' && prev != L'-' && prev != L'*' && prev != L'/' &&
                        prev != L'\uFF0B' && prev != L'\uFF0D' && prev != L'\u00D7' && prev != L'\u00F7') {
                        lastAddSubPos = (int)i;
                        lastAddSubOp = expr[i];
                    }
                }
            }
        }
    }
    
    if (lastAddSubPos > 0) {
        info = GetOperatorInfo(lastAddSubOp);
        info.opLength = 1;
        std::wstring left = expr.substr(0, lastAddSubPos);
        std::wstring right = (lastAddSubPos + 1 < (int)expr.length()) ? expr.substr(lastAddSubPos + 1) : L"";
        // 去除前后空格
        while (!left.empty() && (left.back() == L' ' || left.back() == L'\t')) left.pop_back();
        while (!left.empty() && (left.front() == L' ' || left.front() == L'\t')) left.erase(0, 1);
        while (!right.empty() && (right.front() == L' ' || right.front() == L'\t')) right.erase(0, 1);
        while (!right.empty() && (right.back() == L' ' || right.back() == L'\t')) right.pop_back();
        info.leftOperand = left;
        info.rightOperand = right;
        return true;
    }
    
    // 再查找乘除（高优先级）
    parenDepth = 0;
    inString = false;
    int lastMulDivPos = -1;
    wchar_t lastMulDivOp = 0;
    for (size_t i = 0; i < expr.length(); i++) {
        if (expr[i] == L'"') { inString = !inString; continue; }
        if (inString) continue;
        if (expr[i] == L'(' || expr[i] == L'\uFF08') parenDepth++;
        else if (expr[i] == L')' || expr[i] == L'\uFF09') parenDepth--;
        else if (parenDepth == 0) {
            if (expr[i] == L'*' || expr[i] == L'\u00D7' || expr[i] == L'/' || expr[i] == L'\u00F7') {
                lastMulDivPos = (int)i;
                lastMulDivOp = expr[i];
            }
        }
    }
    
    if (lastMulDivPos > 0) {
        info = GetOperatorInfo(lastMulDivOp);
        info.opLength = 1;
        std::wstring left = expr.substr(0, lastMulDivPos);
        std::wstring right = (lastMulDivPos + 1 < (int)expr.length()) ? expr.substr(lastMulDivPos + 1) : L"";
        while (!left.empty() && (left.back() == L' ' || left.back() == L'\t')) left.pop_back();
        while (!left.empty() && (left.front() == L' ' || left.front() == L'\t')) left.erase(0, 1);
        while (!right.empty() && (right.front() == L' ' || right.front() == L'\t')) right.erase(0, 1);
        while (!right.empty() && (right.back() == L' ' || right.back() == L'\t')) right.pop_back();
        info.leftOperand = left;
        info.rightOperand = right;
        return true;
    }
    
    return false;
}

// 格式化表达式中的运算符为全角（用于显示）
static std::wstring FormatExprOperators(const std::wstring& expr) {
    std::wstring result;
    bool inString = false;
    int parenDepth = 0;
    for (size_t i = 0; i < expr.length(); i++) {
        if (expr[i] == L'"') { inString = !inString; result += expr[i]; continue; }
        if (inString) { result += expr[i]; continue; }
        if (expr[i] == L'(' || expr[i] == L'\uFF08') { parenDepth++; result += expr[i]; continue; }
        if (expr[i] == L')' || expr[i] == L'\uFF09') { parenDepth--; result += expr[i]; continue; }
        if (parenDepth == 0) {
            // 先检查多字符运算符
            wchar_t nextCh = (i + 1 < expr.length()) ? expr[i + 1] : 0;
            if ((expr[i] == L'>' && nextCh == L'=') || (expr[i] == L'\uFF1E' && (nextCh == L'=' || nextCh == L'\uFF1D'))) {
                result += L" \u2265 "; i++; continue;  // ≥
            }
            if ((expr[i] == L'<' && nextCh == L'=') || (expr[i] == L'\uFF1C' && (nextCh == L'=' || nextCh == L'\uFF1D'))) {
                result += L" \u2264 "; i++; continue;  // ≤
            }
            if ((expr[i] == L'!' && nextCh == L'=') || (expr[i] == L'\uFF01' && (nextCh == L'=' || nextCh == L'\uFF1D'))) {
                result += L" \u2260 "; i++; continue;  // ≠
            }
            if ((expr[i] == L'=' && nextCh == L'=') || (expr[i] == L'\uFF1D' && nextCh == L'\uFF1D')) {
                result += L" \uFF1D " ; i++; continue;  // ＝
            }
            switch (expr[i]) {
                case L'>': result += L" \uFF1E "; continue;
                case L'<': result += L" \uFF1C "; continue;
                // 已是全角的比较运算符直接保留（不重复添加空格）
                case L'+': result += L" \uFF0B "; continue;
                case L'-':
                    // 如果是首字符或前面是运算符/空格/左括号，则是负号不格式化
                    if (i == 0 || (i > 0 && (expr[i-1] == L'+' || expr[i-1] == L'-' || expr[i-1] == L'*' || expr[i-1] == L'/' || expr[i-1] == L'(' || expr[i-1] == L' '))) {
                        result += expr[i]; continue;
                    }
                    result += L" \uFF0D "; continue;
                case L'*': result += L" \u00D7 "; continue;
                case L'/': result += L" \u00F7 "; continue;
                default: break;
            }
        }
        result += expr[i];
    }
    return result;
}

// 将全角运算符还原为半角（用于同步参数行回命令行时保持编译兼容性）
static std::wstring RestoreHalfWidthOperators(const std::wstring& expr) {
    std::wstring result;
    for (size_t i = 0; i < expr.length(); i++) {
        wchar_t ch = expr[i];
        const wchar_t* replacement = nullptr;
        switch (ch) {
            case L'\uFF0B': replacement = L"+"; break;  // ＋ → +
            case L'\uFF0D': replacement = L"-"; break;  // ー → -
            case L'\u00D7': replacement = L"*"; break;  // × → *
            case L'\u00F7': replacement = L"/"; break;  // ÷ → /
            case L'\uFF1E': replacement = L">"; break;  // ＞ → >
            case L'\uFF1C': replacement = L"<"; break;  // ＜ → <
            case L'\u2265': replacement = L">="; break; // ≥ → >=
            case L'\u2264': replacement = L"<="; break; // ≤ → <=
            case L'\u2260': replacement = L"!="; break; // ≠ → !=
            case L'\uFF1D': replacement = L"="; break;  // ＝ → =
            default: break;
        }
        if (replacement) {
            // 移除运算符前的空格
            if (!result.empty() && result.back() == L' ') result.pop_back();
            result += replacement;
            // 跳过运算符后的空格
            if (i + 1 < expr.length() && expr[i + 1] == L' ') i++;
        } else {
            result += ch;
        }
    }
    return result;
}

// 格式化赋值语句：var=value 或 var =value 等 → var ＝ value（运算符也全角化）
static void FormatAssignmentLine(std::wstring& line) {
    // 跳过标记字符
    size_t start = 0;
    while (start < line.length() && (line[start] == L'\u200C' || line[start] == L'\u200D' || line[start] == L'\u200B')) {
        start++;
    }
    std::wstring prefix = line.substr(0, start);
    
    std::wstring lhs, rhs;
    if (IsAssignmentLine(line, &lhs, &rhs)) {
        // 格式化右值中的运算符为全角
        rhs = FormatExprOperators(rhs);
        line = prefix + lhs + L" \uFF1D " + rhs;
    }
}

// 检查命令是否有参数（从支持库命令或关键词中获取）
static bool HasCommandParameters(const std::wstring& cmdName) {
    // 先检查支持库命令
    const LibraryCommand* libCmd = LibraryParser::GetInstance().FindCommand(cmdName);
    if (libCmd && !libCmd->parameters.empty()) {
        return true;
    }
    
    // 再检查关键词
    const Keyword* kw = KeywordManager::GetInstance().GetKeyword(cmdName);
    if (kw && kw->needsBrackets) {
        return true;
    }
    
    return false;
}

// 应用自动完成
static void ApplyCompletion(HWND hWnd, EditorData* data, EditorDocument* doc) {
    if (!data->showCompletion || data->completionItems.empty()) return;
    if (data->selectedCompletionIndex < 0 || data->selectedCompletionIndex >= (int)data->completionItems.size()) return;
    
    const CompletionItem& item = data->completionItems[data->selectedCompletionIndex];
    
    // 找到当前单词的起始位置并替换
    std::wstring& line = doc->lines[doc->cursorLine];
    int wordStart;
    GetCurrentWord(line, doc->cursorCol, wordStart);
    
    // 删除当前单词
    line.erase(wordStart, doc->cursorCol - wordStart);
    
    // 只插入中文关键词，不加括号
    // 从 displayText 中提取纯关键词（去掉括号中的别名部分）
    std::wstring keyword = item.displayText;
    size_t spacePos = keyword.find(L' ');
    if (spacePos != std::wstring::npos) {
        keyword = keyword.substr(0, spacePos);  // 只取空格前的部分
    }
    line.insert(wordStart, keyword);
    
    // 移动光标到关键词末尾
    doc->cursorCol = wordStart + (int)keyword.length();
    
    data->showCompletion = false;
    doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
    InvalidateRect(hWnd, NULL, TRUE);
}

// 获取所有数据类型
static std::vector<std::wstring> GetAllDataTypes() {
    // 从支持库获取所有数据类型
    return LibraryParser::GetInstance().GetAllDataTypeNames();
}

// 检查当前是否在类型列编辑（子程序返回类型、参数类型、变量类型）
static bool IsEditingTypeColumn(EditorDocument* doc) {
    if (!doc || doc->cursorLine < 0 || doc->cursorLine >= (int)doc->lines.size()) {
        return false;
    }
    
    const std::wstring& line = doc->lines[doc->cursorLine];
    
    // 检查是否是表格行（包含Tab）
    if (line.find(L'\t') == std::wstring::npos) {
        return false;
    }
    
    // 检查是否是表头行（不允许编辑）
    if (line.find(L"子程序名") == 0 || line.find(L"参数名") == 0 || 
        line.find(L"变量名") == 0 || line.find(L"程序集名") == 0) {
        return false;
    }
    
    // 解析行获取单元格
    std::vector<std::wstring> cells;
    size_t start = 0, pos = 0;
    while ((pos = line.find(L'\t', start)) != std::wstring::npos) {
        cells.push_back(line.substr(start, pos - start));
        start = pos + 1;
    }
    cells.push_back(line.substr(start));
    
    // 计算光标所在单元格索引
    int charCount = 0;
    int cellIndex = 0;
    for (size_t i = 0; i < cells.size(); i++) {
        int cellLen = (int)cells[i].length();
        if (doc->cursorCol <= charCount + cellLen) {
            cellIndex = (int)i;
            break;
        }
        charCount += cellLen + 1;  // +1 for tab
        cellIndex = (int)i + 1;
    }
    
    // 向上查找表头确定表类型
    for (int searchLine = doc->cursorLine - 1; searchLine >= 0; searchLine--) {
        const std::wstring& headerLine = doc->lines[searchLine];
        if (headerLine.empty()) continue;
        
        // 子程序表：类型在第1列（返回值类型）
        if (headerLine.find(L"子程序名") == 0) {
            return cellIndex == 1;
        }
        // 参数表：类型在第1列
        if (headerLine.find(L"参数名") == 0) {
            return cellIndex == 1;
        }
        // 局部变量表：类型在第1列
        if (headerLine.find(L"变量名") == 0 && headerLine.find(L"静态") != std::wstring::npos) {
            return cellIndex == 1;
        }
        // 程序集变量表：类型在第1列
        if (headerLine.find(L"变量名") == 0 && headerLine.find(L"静态") == std::wstring::npos) {
            return cellIndex == 1;
        }
        // 程序集表头：跳出
        if (headerLine.find(L"程序集名") == 0) {
            break;
        }
    }
    
    return false;
}

// 检查当前是否在表格行（包含Tab的行，通常是子程序表、参数表、变量表等）
static bool IsInTableRow(EditorDocument* doc) {
    if (!doc || doc->cursorLine < 0 || doc->cursorLine >= (int)doc->lines.size()) {
        return false;
    }
    
    const std::wstring& line = doc->lines[doc->cursorLine];
    
    // 参数行不是表格行
    if (line.length() > 0 && line[0] == L'\u2060') {
        return false;
    }
    
    // 缩进行（流程控制内的代码）不是表格行
    if (line.length() > 0 && line[0] == L' ') {
        return false;
    }
    
    // 检查是否包含Tab（表格行特征）
    return line.find(L'\t') != std::wstring::npos;
}

// 获取当前单元格文本（用于类型补全）
static std::wstring GetCurrentCellText(EditorDocument* doc, int& cellStartCol) {
    cellStartCol = 0;
    if (!doc || doc->cursorLine < 0 || doc->cursorLine >= (int)doc->lines.size()) {
        return L"";
    }
    
    const std::wstring& line = doc->lines[doc->cursorLine];
    
    // 解析行获取单元格
    std::vector<std::wstring> cells;
    std::vector<int> cellStarts;
    size_t start = 0, pos = 0;
    while ((pos = line.find(L'\t', start)) != std::wstring::npos) {
        cellStarts.push_back((int)start);
        cells.push_back(line.substr(start, pos - start));
        start = pos + 1;
    }
    cellStarts.push_back((int)start);
    cells.push_back(line.substr(start));
    
    // 计算光标所在单元格索引
    int charCount = 0;
    for (size_t i = 0; i < cells.size(); i++) {
        int cellLen = (int)cells[i].length();
        if (doc->cursorCol <= charCount + cellLen) {
            cellStartCol = cellStarts[i];
            return cells[i];
        }
        charCount += cellLen + 1;  // +1 for tab
    }
    
    if (!cells.empty()) {
        cellStartCol = cellStarts.back();
        return cells.back();
    }
    
    return L"";
}

// 隐藏类型补全窗口
static void HideTypeCompletion(EditorData* data) {
    if (data->showTypeCompletion) {
        data->showTypeCompletion = false;
        data->typeCompletionItems.clear();
        data->typeCompletionSelectedIndex = 0;
        data->typeCompletionScrollOffset = 0;
    }
}

// 更新类型补全列表
static void UpdateTypeCompletion(HWND hWnd, EditorData* data, EditorDocument* doc) {
    if (!IsEditingTypeColumn(doc)) {
        HideTypeCompletion(data);
        return;
    }
    
    // 获取当前单元格文本
    int cellStartCol;
    std::wstring input = GetCurrentCellText(doc, cellStartCol);
    
    // 辅助函数：转小写（支持所有Unicode字符）
    auto toLowerUnicode = [](const std::wstring& str) -> std::wstring {
        std::wstring result = str;
        CharLowerBuffW(&result[0], (DWORD)result.length());
        return result;
    };
    
    // 辅助函数：判断字符是否为ASCII字母
    auto isAsciiAlpha = [](wchar_t ch) -> bool {
        return (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z');
    };
    
    // 辅助函数：中英混合智能匹配
    auto mixedMatch = [&](const std::wstring& inputStr, const std::wstring& target) -> bool {
        std::wstring lowerInput = toLowerUnicode(inputStr);
        std::wstring lowerTarget = toLowerUnicode(target);
        
        size_t inputPos = 0;
        size_t targetPos = 0;
        
        while (inputPos < lowerInput.length() && targetPos < lowerTarget.length()) {
            wchar_t inputChar = lowerInput[inputPos];
            wchar_t targetChar = lowerTarget[targetPos];
            
            if (inputChar == targetChar) {
                inputPos++;
                targetPos++;
            } else if (isAsciiAlpha(inputChar)) {
                std::wstring targetCharStr(1, target[targetPos]);
                std::wstring targetPinyin = PinyinHelper::GetStringPinyin(targetCharStr);
                std::wstring targetInitial = PinyinHelper::GetStringInitials(targetCharStr);
                std::wstring lowerPinyin = toLowerUnicode(targetPinyin);
                std::wstring lowerInitial = toLowerUnicode(targetInitial);
                
                if (lowerPinyin.length() > 0 && inputPos + lowerPinyin.length() <= lowerInput.length()) {
                    std::wstring inputSubstr = lowerInput.substr(inputPos, lowerPinyin.length());
                    if (inputSubstr == lowerPinyin) {
                        inputPos += lowerPinyin.length();
                        targetPos++;
                    }
                    else if (!lowerInitial.empty() && lowerInitial[0] == inputChar) {
                        inputPos++;
                        targetPos++;
                    } else {
                        return false;
                    }
                }
                else if (!lowerInitial.empty() && lowerInitial[0] == inputChar) {
                    inputPos++;
                    targetPos++;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }
        
        return inputPos == lowerInput.length();
    };
    
    // 获取所有数据类型
    std::vector<std::wstring> allTypes = GetAllDataTypes();
    
    // 过滤匹配的类型
    data->typeCompletionItems.clear();
    
    // 用于存储匹配项和分数
    std::vector<std::pair<std::wstring, int>> scoredItems;
    
    std::wstring lowerInput = toLowerUnicode(input);
    
    for (const auto& type : allTypes) {
        int score = 0;
        std::wstring lowerType = toLowerUnicode(type);
        
        // 1. 直接字符串前缀匹配
        if (lowerType.find(lowerInput) == 0) {
            score = 1000;
        }
        // 2. 纯拼音匹配
        else {
            std::wstring fullPinyin = PinyinHelper::GetStringPinyin(type);
            std::wstring initials = PinyinHelper::GetStringInitials(type);
            std::wstring lowerFullPinyin = toLowerUnicode(fullPinyin);
            std::wstring lowerInitials = toLowerUnicode(initials);
            
            if (lowerInitials.find(lowerInput) == 0) {
                score = 800;
            } else if (lowerFullPinyin.find(lowerInput) == 0) {
                score = 700;
            }
            // 3. 中英混合智能匹配
            else if (mixedMatch(input, type)) {
                score = 600;
            }
            // 4. 包含匹配
            else if (lowerType.find(lowerInput) != std::wstring::npos) {
                score = 500;
            } else if (lowerFullPinyin.find(lowerInput) != std::wstring::npos) {
                score = 300;
            } else if (lowerInitials.find(lowerInput) != std::wstring::npos) {
                score = 400;
            }
        }
        
        if (score > 0) {
            scoredItems.push_back({type, score});
        }
    }
    
    // 如果输入为空，显示所有类型
    if (input.empty()) {
        for (const auto& type : allTypes) {
            scoredItems.push_back({type, 100});
        }
    }
    
    // 按分数排序
    std::sort(scoredItems.begin(), scoredItems.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });
    
    // 提取排序后的类型名
    for (const auto& item : scoredItems) {
        data->typeCompletionItems.push_back(item.first);
    }
    
    // 检查是否完全匹配：如果输入内容完全等于某个数据类型，且只有一个匹配项，则不显示补全
    bool isExactMatch = false;
    if (data->typeCompletionItems.size() == 1 && data->typeCompletionItems[0] == input) {
        isExactMatch = true;
    }
    
    // 如果有匹配项且不是完全匹配，显示补全窗口
    if (!data->typeCompletionItems.empty() && !isExactMatch) {
        data->showTypeCompletion = true;
        data->typeCompletionSelectedIndex = 0;
        data->typeCompletionScrollOffset = 0;
        // 补全窗口位置将在绘制时计算
    } else {
        HideTypeCompletion(data);
    }
}

// 应用类型补全
static void ApplyTypeCompletion(HWND hWnd, EditorData* data, EditorDocument* doc) {
    if (!data->showTypeCompletion || data->typeCompletionItems.empty()) return;
    if (data->typeCompletionSelectedIndex < 0 || 
        data->typeCompletionSelectedIndex >= (int)data->typeCompletionItems.size()) return;
    
    // 获取选中的类型
    std::wstring selectedType = data->typeCompletionItems[data->typeCompletionSelectedIndex];
    
    // 获取当前单元格信息
    int cellStartCol;
    std::wstring currentCell = GetCurrentCellText(doc, cellStartCol);
    
    // 替换单元格内容
    std::wstring& line = doc->lines[doc->cursorLine];
    
    // 删除当前单元格内容
    int cellEndCol = cellStartCol + (int)currentCell.length();
    line.erase(cellStartCol, cellEndCol - cellStartCol);
    
    // 插入选中的类型
    line.insert(cellStartCol, selectedType);
    
    // 移动光标到类型末尾
    doc->cursorCol = cellStartCol + (int)selectedType.length();
    
    // 隐藏补全窗口
    HideTypeCompletion(data);
    
    doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
    InvalidateRect(hWnd, NULL, TRUE);
}

// 检查当前行是否是流程控制块内唯一的内容行（不能被删除）
// 检查上方是否有同深度的流程控制命令，并且下方没有其他同深度的内容行
static bool IsOnlyContentLineInFlowBlock(EditorDocument* doc, int lineIndex) {
    if (lineIndex <= 0 || lineIndex >= (int)doc->lines.size()) return false;
    
    const std::wstring& line = doc->lines[lineIndex];
    int depth = GetNestingDepth(line);
    if (depth <= 0) return false;  // 不是嵌套内容行
    
    // 去掉标记后看是否有实际代码内容（排除嵌套的流程控制命令）
    size_t markerLen = 0;
    GetNestingDepth(line, &markerLen);
    std::wstring content = line.substr(markerLen);
    // 如果内容行是流程控制命令（如嵌套的.如果真），则不受保护
    if (!content.empty() && content[0] == L'.') return false;
    
    // 向上查找同深度-1的流程控制命令（父命令）
    bool foundParentCmd = false;
    for (int k = lineIndex - 1; k >= 0; k--) {
        const std::wstring& prevLine = doc->lines[k];
        // 跳过参数行
        if (!prevLine.empty() && prevLine[0] == L'\u2060') continue;
        
        int prevDepth = GetNestingDepth(prevLine);
        if (prevDepth < depth) {
            // 检查是否是流程控制命令
            if (IsFlowControlLine(prevLine)) {
                foundParentCmd = true;
            }
            break;
        }
    }
    if (!foundParentCmd) return false;
    
    // 检查是否是同深度的唯一内容行（不含嵌套的流程控制命令）
    int sameDepthContentCount = 0;
    // 向上扫描
    for (int k = lineIndex - 1; k >= 0; k--) {
        const std::wstring& checkLine = doc->lines[k];
        if (!checkLine.empty() && checkLine[0] == L'\u2060') continue;
        int checkDepth = GetNestingDepth(checkLine);
        if (checkDepth < depth) break;  // 到达上层，停止
        if (checkDepth == depth) {
            size_t ml = 0;
            GetNestingDepth(checkLine, &ml);
            std::wstring c = checkLine.substr(ml);
            if (c.empty() || c[0] != L'.') sameDepthContentCount++;  // 非流程控制命令的同深度行
        }
    }
    // 向下扫描
    for (int k = lineIndex + 1; k < (int)doc->lines.size(); k++) {
        const std::wstring& checkLine = doc->lines[k];
        if (!checkLine.empty() && checkLine[0] == L'\u2060') continue;
        int checkDepth = GetNestingDepth(checkLine);
        if (checkDepth < depth) break;  // 到达同层或上层，停止
        if (checkDepth == depth) {
            size_t ml = 0;
            GetNestingDepth(checkLine, &ml);
            std::wstring c = checkLine.substr(ml);
            if (c.empty() || c[0] != L'.') sameDepthContentCount++;
        }
    }
    
    // 加上当前行自己
    sameDepthContentCount++;
    
    return sameDepthContentCount <= 1;
}

// 检查当前行是否是流程控制区块后的保护空行
// 如果是，返回流程控制内最后一行的索引；否则返回-1
static int IsProtectedEmptyLineAfterFlowControl(EditorDocument* doc, int lineIndex) {
    if (lineIndex <= 0 || lineIndex >= (int)doc->lines.size()) return -1;
    
    // 当前行必须是普通空行（不是缩进行）
    if (!doc->lines[lineIndex].empty()) return -1;
    
    // 向上查找流程控制结构
    int prevLine = lineIndex - 1;
    int lastIndentLine = -1;  // 最后一个缩进行的索引
    
    // 向上查找所有缩进行，记录最后一个（最接近保护空行的）
    while (prevLine >= 0) {
        const std::wstring& line = doc->lines[prevLine];
        
        // 如果是缩进行（流程控制内的行）
        if (line.length() > 0 && line[0] == L' ') {
            // 记录这个缩进行，这是最接近保护空行的缩进行
            if (lastIndentLine == -1) {
                lastIndentLine = prevLine;
            }
            prevLine--;
            continue;
        }
        // 如果是.如果真行，继续向上
        else if (IsFlowControlLine(line)) {
            prevLine--;
            continue;
        }
        // 其他情况，停止查找
        else {
            break;
        }
    }
    
    // 返回找到的最后一个缩进行（最接近保护空行的那个）
    return lastIndentLine;
}

// 同步参数行的值到命令行括号内
// 当参数行的值被修改时，更新命令行（如 .如果() ）括号内的内容
static void SyncParamLineToCommandLine(EditorDocument* doc, int paramLineIndex) {
    if (paramLineIndex <= 0 || paramLineIndex >= (int)doc->lines.size()) return;
    
    const std::wstring& paramLine = doc->lines[paramLineIndex];
    if (paramLine.empty() || paramLine[0] != L'\u2060') return;
    
    // 如果是嵌套参数行（\u2060\u2060前缀），不需要同步到命令行
    if (paramLine.length() > 1 && paramLine[1] == L'\u2060') return;
    
    // 向上查找命令行（跳过其他参数行）
    int cmdLineIndex = paramLineIndex - 1;
    int paramIndex = 0;  // 当前参数行是第几个参数（从0开始）
    
    while (cmdLineIndex >= 0) {
        const std::wstring& checkLine = doc->lines[cmdLineIndex];
        if (checkLine.length() > 0 && checkLine[0] == L'\u2060') {
            // 跳过嵌套参数行（\u2060\u2060），只计数一级参数行
            if (!(checkLine.length() > 1 && checkLine[1] == L'\u2060')) {
                paramIndex++;
            }
            cmdLineIndex--;
        } else {
            // 找到了命令行
            break;
        }
    }
    
    if (cmdLineIndex < 0) return;
    
    std::wstring& cmdLine = doc->lines[cmdLineIndex];
    
    // 检查命令行是否有括号
    size_t leftBracket = cmdLine.find(L'(');
    size_t rightBracket = cmdLine.find(L')');
    if (leftBracket == std::wstring::npos || rightBracket == std::wstring::npos || leftBracket >= rightBracket) return;
    
    // 收集所有一级参数行的值（跳过嵌套参数行）
    std::vector<std::wstring> paramValues;
    for (int i = cmdLineIndex + 1; i < (int)doc->lines.size(); i++) {
        const std::wstring& line = doc->lines[i];
        if (line.length() > 0 && line[0] == L'\u2060') {
            // 跳过嵌套参数行（\u2060\u2060前缀）
            if (line.length() > 1 && line[1] == L'\u2060') continue;
            // 提取参数值（格式：\u2060参数名:参数值）
            size_t colonPos = line.find(L':');
            if (colonPos != std::wstring::npos) {
                paramValues.push_back(line.substr(colonPos + 1));
            } else {
                paramValues.push_back(L"");
            }
        } else {
            break;
        }
    }
    
    // 构建新的括号内容
    std::wstring newBracketContent;
    for (size_t i = 0; i < paramValues.size(); i++) {
        if (i > 0) newBracketContent += L", ";
        newBracketContent += paramValues[i];
    }
    
    // 更新命令行括号内的内容
    cmdLine = cmdLine.substr(0, leftBracket + 1) + newBracketContent + cmdLine.substr(rightBracket);
}

// 同步赋值参数行的值到赋值命令行
// 当赋值参数行（被赋值的变量 或 赋予的值）被修改时，更新赋值行
static void SyncAssignParamLineToAssignLine(EditorDocument* doc, int paramLineIndex) {
    if (paramLineIndex <= 0 || paramLineIndex >= (int)doc->lines.size()) return;
    
    const std::wstring& paramLine = doc->lines[paramLineIndex];
    if (paramLine.empty() || paramLine[0] != L'\u2060') return;
    
    // 如果是嵌套参数行（\u2060\u2060前缀），不需要同步到赋值行
    if (paramLine.length() > 1 && paramLine[1] == L'\u2060') return;
    
    // 向上查找赋值命令行（跳过其他参数行）
    int cmdLineIndex = paramLineIndex - 1;
    while (cmdLineIndex >= 0 && !doc->lines[cmdLineIndex].empty() && doc->lines[cmdLineIndex][0] == L'\u2060') {
        cmdLineIndex--;
    }
    if (cmdLineIndex < 0) return;
    
    std::wstring& cmdLine = doc->lines[cmdLineIndex];
    if (!IsAssignmentLine(cmdLine)) return;
    
    // 收集两行一级参数的值（跳过嵌套参数行）
    std::wstring newLhs, newRhs;
    int paramCount = 0;
    for (int j = cmdLineIndex + 1; j < (int)doc->lines.size(); j++) {
        const std::wstring& pl = doc->lines[j];
        if (pl.empty() || pl[0] != L'\u2060') break;
        // 跳过嵌套参数行
        if (pl.length() > 1 && pl[1] == L'\u2060') continue;
        size_t colonPos = pl.find(L':');
        std::wstring val = (colonPos != std::wstring::npos) ? pl.substr(colonPos + 1) : L"";
        if (paramCount == 0) newLhs = val;
        else if (paramCount == 1) newRhs = val;
        paramCount++;
        if (paramCount >= 2) break;
    }
    
    // 重建赋值行，保留标记前缀
    size_t start = 0;
    while (start < cmdLine.length() && (cmdLine[start] == L'\u200C' || cmdLine[start] == L'\u200D' || cmdLine[start] == L'\u200B')) {
        start++;
    }
    std::wstring prefix = cmdLine.substr(0, start);
    cmdLine = prefix + newLhs + L"=" + newRhs;
    FormatAssignmentLine(cmdLine);
}

// 执行语法检查（在文本变化后调用）
static void PerformSyntaxCheck(EditorDocument* doc) {
    if (!doc || !doc->syntaxCheckEnabled) return;
    
    // 初始化语法检查器（如果还没有初始化）
    static bool initialized = false;
    if (!initialized) {
        g_SyntaxChecker.SetKeywordManager(&KeywordManager::GetInstance());
        g_SyntaxChecker.SetLibraryParser(&LibraryParser::GetInstance());
        initialized = true;
    }
    
    // 获取整个文档的文本
    std::wstring fullText;
    for (size_t i = 0; i < doc->lines.size(); i++) {
        fullText += doc->lines[i];
        if (i < doc->lines.size() - 1) {
            fullText += L"\n";
        }
    }
    
    // 执行语法检查
    auto result = g_SyntaxChecker.Check(fullText);
    
    // 更新错误列表
    doc->syntaxErrors = result.errors;
    
    // 调试输出：显示语法错误（如果有）
    if (!result.errors.empty()) {
        std::wstring debugMsg = L"[语法检查] 发现 " + std::to_wstring(result.errors.size()) + L" 个语法错误:\n";
        for (size_t i = 0; i < result.errors.size() && i < 5; i++) {  // 最多显示5个错误
            const auto& err = result.errors[i];
            debugMsg += L"  行" + std::to_wstring(err.line) + L":" + std::to_wstring(err.column) 
                       + L" - " + err.message + L"\n";
        }
        OutputDebugStringW(debugMsg.c_str());
    }
}

// 检查并格式化行上的关键词（离开行时调用）
static void CheckAndFormatKeywords(EditorDocument* doc, int lineIndex) {
    if (lineIndex < 0 || lineIndex >= (int)doc->lines.size()) return;
    
    std::wstring& line = doc->lines[lineIndex];
    if (line.empty()) return;
    
    // 获取行首的单词（去掉前导空格和嵌套标记）
    size_t start = 0;
    
    // 跳过嵌套标记字符（\u200C、\u200D、\u200B）
    while (start < line.length() && (line[start] == L'\u200C' || line[start] == L'\u200D' || line[start] == L'\u200B')) {
        start++;
    }
    
    // 记录标记结束位置（用于后续插入点号）
    size_t markerEnd = start;
    
    while (start < line.length() && (line[start] == L' ' || line[start] == L'\t')) {
        start++;
    }
    
    // 查找第一个单词
    size_t end = start;
    while (end < line.length() && line[end] != L' ' && line[end] != L'\t' && 
           line[end] != L'(' && line[end] != L')' && line[end] != L'（' && line[end] != L'）') {
        end++;
    }
    
    if (end > start) {
        std::wstring word = line.substr(start, end - start);
        
        // 先检查是否已有括号（中文或英文都算）
        bool originalHasBracket = (line.find(L'(', end) != std::wstring::npos || 
                                   line.find(L'（', end) != std::wstring::npos);
        
        // 格式化中文符号为英文符号
        for (size_t j = 0; j < line.length(); j++) {
            if (line[j] == L'（') line[j] = L'(';
            else if (line[j] == L'）') line[j] = L')';
            else if (line[j] == L'，') line[j] = L',';
        }
        
        // 先从内置关键词查找
        const Keyword* kw = KeywordManager::GetInstance().GetKeyword(word);
        
        // 如果没找到，从支持库命令查找
        const LibraryCommand* libCmd = nullptr;
        if (!kw) {
            libCmd = LibraryParser::GetInstance().FindCommand(word);
        }
        
        // 检查是否是流程控制命令
        bool isFlowControl = false;
        bool needsFlowLine = false;
        bool needsBrackets = false;
        bool hasParameters = false;
        
        if (kw && kw->type == KW_CONTROL) {
            isFlowControl = true;
            needsFlowLine = kw->needsFlowLine;
            needsBrackets = kw->needsBrackets;
        } else if (libCmd) {
            // 所有支持库命令都检查是否有参数
            if (libCmd->needsFlowLine) {
                isFlowControl = true;
                needsFlowLine = libCmd->needsFlowLine;
                needsBrackets = libCmd->needsBrackets;
            }
            // 检查是否有参数
            if (!libCmd->parameters.empty()) {
                hasParameters = true;
                needsBrackets = true;  // 有参数的命令都需要括号
            }
        }
        
        // 处理流程控制命令或有参数的普通命令
        if (isFlowControl || hasParameters) {
            // 检查关键词前面（标记之后）是否已有点号
            bool hasDot = (start > markerEnd && line[start - 1] == L'.');
            
            // 如果需要流程线但没有点号，自动在标记后面添加点号
            if (needsFlowLine && !hasDot) {
                line.insert(start, L".");
                end++;  // 调整end位置，因为插入了一个字符
            }
            
            // 如果需要括号但原来没有括号（中英文都没有），自动添加（带空格）
            if (!originalHasBracket && needsBrackets) {
                line.insert(end, L" ()");
            } else if (originalHasBracket) {
                // 已有括号，确保命令和括号之间有一个空格
                size_t bracketPos = line.find(L'(', end);
                if (bracketPos != std::wstring::npos) {
                    // 统计括号前有多少个空格
                    size_t beforeBracket = bracketPos;
                    size_t spaceCount = 0;
                    while (beforeBracket > end && (line[beforeBracket - 1] == L' ' || line[beforeBracket - 1] == L'\t')) {
                        beforeBracket--;
                        spaceCount++;
                    }
                    
                    if (spaceCount == 0) {
                        // 没有空格，添加一个
                        line.insert(bracketPos, L" ");
                    } else if (spaceCount > 1) {
                        // 多于一个空格，删除多余的
                        line.erase(beforeBracket, spaceCount - 1);
                    }
                }
            }
            
            // 如果需要流程线，在后面添加相应的行
            if (needsFlowLine) {
                // 检查是否是.如果()或.判断()命令（带括号，有否则分支）
                // 判断当前是否有括号（原来有或者刚添加的）
                bool hasBracket = originalHasBracket || needsBrackets;
                bool hasElseBranch = ((word == L"如果" || word == L"判断") && needsBrackets && hasBracket);
                
                if (hasElseBranch) {
                    // .如果()：创建带否则分支的结构
                    bool needsStructure = true;
                    if (lineIndex + 1 < (int)doc->lines.size()) {
                        // 检查是否已经有结构
                        const std::wstring& line1 = doc->lines[lineIndex + 1];
                        const std::wstring& line2 = lineIndex + 2 < (int)doc->lines.size() ? doc->lines[lineIndex + 2] : L"";
                        
                        // 获取当前行的嵌套深度
                        int currentDepth = GetNestingDepth(line);
                        
                        // 辅助lambda：检查行是否以\u200C或\u200D结尾（标记结束符）
                        auto getLineEndMarker = [](const std::wstring& checkLine) -> wchar_t {
                            if (checkLine.empty()) return 0;
                            // 遍历跳过所有标记字符，返回最后一个标记字符
                            size_t pos = 0;
                            wchar_t lastMarker = 0;
                            while (pos < checkLine.length() && 
                                   (checkLine[pos] == L'\u200C' || checkLine[pos] == L'\u200D' || checkLine[pos] == L'\u200B')) {
                                if (checkLine[pos] == L'\u200C' || checkLine[pos] == L'\u200D') {
                                    lastMarker = checkLine[pos];
                                }
                                pos++;
                            }
                            return lastMarker;
                        };
                        
                        // 检查：第一行应该是条件达成分支（以\u200C结尾），第二行应该是否则分支（以\u200D结尾）
                        bool hasLine1 = (line1.length() > 0 && getLineEndMarker(line1) == L'\u200C');
                        bool hasLine2 = (line2.length() > 0 && getLineEndMarker(line2) == L'\u200D');
                        
                        if (hasLine1 && hasLine2) {
                            needsStructure = false;
                        }
                    }
                    if (needsStructure) {
                        // 计算当前行的嵌套深度
                        int nestingDepth = GetNestingDepth(line);
                        
                        // 获取当前行的标记前缀
                        size_t markerLen = 0;
                        while (markerLen < line.length() && 
                               (line[markerLen] == L'\u200C' || line[markerLen] == L'\u200D' || line[markerLen] == L'\u200B')) {
                            markerLen++;
                        }
                        std::wstring currentMarker = (markerLen > 0) ? line.substr(0, markerLen) : L"";
                        
                        // 构建内层的嵌套标记
                        std::wstring innerMarkerC, innerMarkerD;
                        if (nestingDepth == 0) {
                            innerMarkerC = L"\u200C";  // 条件达成分支
                            innerMarkerD = L"\u200D";  // 否则分支
                        } else {
                            // 嵌套情况下，条件达成用\u200C结尾，否则用\u200D结尾
                            innerMarkerC = currentMarker + L"\u200B\u200C";
                            innerMarkerD = currentMarker + L"\u200B\u200D";
                        }
                        
                        // 插入内层行（条件达成和否则分支）
                        doc->lines.insert(doc->lines.begin() + lineIndex + 1, innerMarkerC);
                        doc->lines.insert(doc->lines.begin() + lineIndex + 2, innerMarkerD);
                        
                        // 插入结束行：深度0时插入空行，深度>0时插入上一层缩进行
                        if (nestingDepth > 0) {
                            doc->lines.insert(doc->lines.begin() + lineIndex + 3, currentMarker);
                        } else {
                            doc->lines.insert(doc->lines.begin() + lineIndex + 3, L"");
                        }
                    }
                } else if (word == L"判断循环首") {
                    // 判断循环首命令：创建缩进行 + 判断循环尾命令行
                    bool needsStructure = true;
                    if (lineIndex + 1 < (int)doc->lines.size()) {
                        int currentDepth = GetNestingDepth(line);
                        int nextLineDepth = GetNestingDepth(doc->lines[lineIndex + 1]);
                        // 检查结束行是否包含判断循环尾
                        bool hasCorrectEnd = false;
                        if (lineIndex + 2 < (int)doc->lines.size()) {
                            const std::wstring& endLine = doc->lines[lineIndex + 2];
                            hasCorrectEnd = (endLine.find(L".判断循环尾") != std::wstring::npos);
                        }
                        if (nextLineDepth == currentDepth + 1 && hasCorrectEnd) {
                            needsStructure = false;
                        }
                    }
                    if (needsStructure) {
                        int nestingDepth = GetNestingDepth(line);
                        
                        size_t markerLen = 0;
                        while (markerLen < line.length() && 
                               (line[markerLen] == L'\u200C' || line[markerLen] == L'\u200D' || line[markerLen] == L'\u200B')) {
                            markerLen++;
                        }
                        std::wstring currentMarker = (markerLen > 0) ? line.substr(0, markerLen) : L"";
                        
                        std::wstring innerMarker;
                        if (nestingDepth == 0) {
                            innerMarker = L"\u200C";
                        } else {
                            innerMarker = currentMarker + L"\u200B\u200C";
                        }
                        
                        // 插入内层缩进行
                        doc->lines.insert(doc->lines.begin() + lineIndex + 1, innerMarker);
                        
                        // 插入判断循环尾命令行（与判断循环首同层）
                        std::wstring tailLine = currentMarker + L".判断循环尾 ()";
                        doc->lines.insert(doc->lines.begin() + lineIndex + 2, tailLine);
                        
                        // 插入结束行
                        if (nestingDepth > 0) {
                            doc->lines.insert(doc->lines.begin() + lineIndex + 3, currentMarker);
                        } else {
                            doc->lines.insert(doc->lines.begin() + lineIndex + 3, L"");
                        }
                    }
                } else if (word == L"循环判断首") {
                    // 循环判断首命令：创建缩进行 + 循环判断尾命令行
                    bool needsStructure = true;
                    if (lineIndex + 1 < (int)doc->lines.size()) {
                        int currentDepth = GetNestingDepth(line);
                        int nextLineDepth = GetNestingDepth(doc->lines[lineIndex + 1]);
                        bool hasCorrectEnd = false;
                        if (lineIndex + 2 < (int)doc->lines.size()) {
                            const std::wstring& endLine = doc->lines[lineIndex + 2];
                            hasCorrectEnd = (endLine.find(L".循环判断尾") != std::wstring::npos);
                        }
                        if (nextLineDepth == currentDepth + 1 && hasCorrectEnd) {
                            needsStructure = false;
                        }
                    }
                    if (needsStructure) {
                        int nestingDepth = GetNestingDepth(line);
                        
                        size_t markerLen = 0;
                        while (markerLen < line.length() && 
                               (line[markerLen] == L'\u200C' || line[markerLen] == L'\u200D' || line[markerLen] == L'\u200B')) {
                            markerLen++;
                        }
                        std::wstring currentMarker = (markerLen > 0) ? line.substr(0, markerLen) : L"";
                        
                        std::wstring innerMarker;
                        if (nestingDepth == 0) {
                            innerMarker = L"\u200C";
                        } else {
                            innerMarker = currentMarker + L"\u200B\u200C";
                        }
                        
                        // 插入内层缩进行
                        doc->lines.insert(doc->lines.begin() + lineIndex + 1, innerMarker);
                        
                        // 插入循环判断尾命令行（与循环判断首同层）
                        std::wstring tailLine = currentMarker + L".循环判断尾 ()";
                        doc->lines.insert(doc->lines.begin() + lineIndex + 2, tailLine);
                        
                        // 插入结束行
                        if (nestingDepth > 0) {
                            doc->lines.insert(doc->lines.begin() + lineIndex + 3, currentMarker);
                        } else {
                            doc->lines.insert(doc->lines.begin() + lineIndex + 3, L"");
                        }
                    }
                } else if (word == L"如果真") {
                    // 如果真命令：创建递减深度的缩进行
                    bool needsStructure = true;
                    if (lineIndex + 1 < (int)doc->lines.size()) {
                        // 检查是否已经有结构（精确匹配深度）
                        int currentDepth = GetNestingDepth(line);
                        int nextLineDepth = GetNestingDepth(doc->lines[lineIndex + 1]);
                        bool hasCorrectEnd = false;
                        if (lineIndex + 2 < (int)doc->lines.size()) {
                            if (currentDepth == 0) {
                                hasCorrectEnd = doc->lines[lineIndex + 2].empty();
                            } else {
                                hasCorrectEnd = (GetNestingDepth(doc->lines[lineIndex + 2]) == currentDepth);
                            }
                        }
                        if (nextLineDepth == currentDepth + 1 && hasCorrectEnd) {
                            needsStructure = false;
                        }
                    }
                    if (needsStructure) {
                        // 计算当前行的嵌套深度
                        int nestingDepth = GetNestingDepth(line);
                        
                        // 获取当前行的标记前缀
                        size_t markerLen = 0;
                        while (markerLen < line.length() && 
                               (line[markerLen] == L'\u200C' || line[markerLen] == L'\u200D' || line[markerLen] == L'\u200B')) {
                            markerLen++;
                        }
                        std::wstring currentMarker = (markerLen > 0) ? line.substr(0, markerLen) : L"";
                        
                        // 构建内层的嵌套标记（比当前深一层）
                        std::wstring innerMarker;
                        if (nestingDepth == 0) {
                            innerMarker = L"\u200C";
                        } else {
                            innerMarker = currentMarker + L"\u200B\u200C";
                        }
                        
                        // 插入内层缩进行
                        doc->lines.insert(doc->lines.begin() + lineIndex + 1, innerMarker);
                        
                        // 插入结束行：深度0时插入空行，深度>0时插入上一层缩进行
                        if (nestingDepth > 0) {
                            doc->lines.insert(doc->lines.begin() + lineIndex + 2, currentMarker);
                        } else {
                            doc->lines.insert(doc->lines.begin() + lineIndex + 2, L"");
                        }
                    }
                } else if (word == L"计次循环首") {
                    // 计次循环首命令：创建缩进行 + 计次循环尾命令行
                    bool needsStructure = true;
                    if (lineIndex + 1 < (int)doc->lines.size()) {
                        int currentDepth = GetNestingDepth(line);
                        int nextLineDepth = GetNestingDepth(doc->lines[lineIndex + 1]);
                        bool hasCorrectEnd = false;
                        if (lineIndex + 2 < (int)doc->lines.size()) {
                            const std::wstring& endLine = doc->lines[lineIndex + 2];
                            hasCorrectEnd = (endLine.find(L".计次循环尾") != std::wstring::npos);
                        }
                        if (nextLineDepth == currentDepth + 1 && hasCorrectEnd) {
                            needsStructure = false;
                        }
                    }
                    if (needsStructure) {
                        int nestingDepth = GetNestingDepth(line);
                        
                        size_t markerLen = 0;
                        while (markerLen < line.length() && 
                               (line[markerLen] == L'\u200C' || line[markerLen] == L'\u200D' || line[markerLen] == L'\u200B')) {
                            markerLen++;
                        }
                        std::wstring currentMarker = (markerLen > 0) ? line.substr(0, markerLen) : L"";
                        
                        std::wstring innerMarker;
                        if (nestingDepth == 0) {
                            innerMarker = L"\u200C";
                        } else {
                            innerMarker = currentMarker + L"\u200B\u200C";
                        }
                        
                        doc->lines.insert(doc->lines.begin() + lineIndex + 1, innerMarker);
                        
                        std::wstring tailLine = currentMarker + L".计次循环尾 ()";
                        doc->lines.insert(doc->lines.begin() + lineIndex + 2, tailLine);
                        
                        if (nestingDepth > 0) {
                            doc->lines.insert(doc->lines.begin() + lineIndex + 3, currentMarker);
                        } else {
                            doc->lines.insert(doc->lines.begin() + lineIndex + 3, L"");
                        }
                    }
                } else if (word == L"变量循环首") {
                    // 变量循环首命令：创建缩进行 + 变量循环尾命令行
                    bool needsStructure = true;
                    if (lineIndex + 1 < (int)doc->lines.size()) {
                        int currentDepth = GetNestingDepth(line);
                        int nextLineDepth = GetNestingDepth(doc->lines[lineIndex + 1]);
                        bool hasCorrectEnd = false;
                        if (lineIndex + 2 < (int)doc->lines.size()) {
                            const std::wstring& endLine = doc->lines[lineIndex + 2];
                            hasCorrectEnd = (endLine.find(L".变量循环尾") != std::wstring::npos);
                        }
                        if (nextLineDepth == currentDepth + 1 && hasCorrectEnd) {
                            needsStructure = false;
                        }
                    }
                    if (needsStructure) {
                        int nestingDepth = GetNestingDepth(line);
                        
                        size_t markerLen = 0;
                        while (markerLen < line.length() && 
                               (line[markerLen] == L'\u200C' || line[markerLen] == L'\u200D' || line[markerLen] == L'\u200B')) {
                            markerLen++;
                        }
                        std::wstring currentMarker = (markerLen > 0) ? line.substr(0, markerLen) : L"";
                        
                        std::wstring innerMarker;
                        if (nestingDepth == 0) {
                            innerMarker = L"\u200C";
                        } else {
                            innerMarker = currentMarker + L"\u200B\u200C";
                        }
                        
                        doc->lines.insert(doc->lines.begin() + lineIndex + 1, innerMarker);
                        
                        std::wstring tailLine = currentMarker + L".变量循环尾 ()";
                        doc->lines.insert(doc->lines.begin() + lineIndex + 2, tailLine);
                        
                        if (nestingDepth > 0) {
                            doc->lines.insert(doc->lines.begin() + lineIndex + 3, currentMarker);
                        } else {
                            doc->lines.insert(doc->lines.begin() + lineIndex + 3, L"");
                        }
                    }
                } else if (word == L"判断循环尾" || word == L"循环判断尾" || word == L"计次循环尾" || word == L"变量循环尾") {
                    // 循环尾命令：不创建嵌套结构，只作为循环的结束标记
                    // 流程线由绘制代码负责（从循环尾向上到循环首）
                } else {
                    // 其他流程控制命令
                    bool needsStructure = true;
                    if (lineIndex + 1 < (int)doc->lines.size()) {
                        // 检查是否已经有结构（精确匹配深度）
                        int currentDepth = GetNestingDepth(line);
                        int nextLineDepth = GetNestingDepth(doc->lines[lineIndex + 1]);
                        bool hasCorrectEnd = false;
                        if (lineIndex + 2 < (int)doc->lines.size()) {
                            if (currentDepth == 0) {
                                hasCorrectEnd = doc->lines[lineIndex + 2].empty();
                            } else {
                                hasCorrectEnd = (GetNestingDepth(doc->lines[lineIndex + 2]) == currentDepth);
                            }
                        }
                        if (nextLineDepth == currentDepth + 1 && hasCorrectEnd) {
                            needsStructure = false;
                        }
                    }
                    if (needsStructure) {
                        // 计算当前行的嵌套深度
                        int nestingDepth = GetNestingDepth(line);
                        
                        // 获取当前行的标记前缀
                        size_t markerLen = 0;
                        while (markerLen < line.length() && 
                               (line[markerLen] == L'\u200C' || line[markerLen] == L'\u200D' || line[markerLen] == L'\u200B')) {
                            markerLen++;
                        }
                        std::wstring currentMarker = (markerLen > 0) ? line.substr(0, markerLen) : L"";
                        
                        // 构建内层的嵌套标记（比当前深一层）
                        std::wstring innerMarker;
                        if (nestingDepth == 0) {
                            innerMarker = L"\u200C";
                        } else {
                            innerMarker = currentMarker + L"\u200B\u200C";
                        }
                        
                        // 插入内层缩进行
                        doc->lines.insert(doc->lines.begin() + lineIndex + 1, innerMarker);
                        
                        // 插入结束行：深度0时插入空行，深度>0时插入上一层缩进行
                        if (nestingDepth > 0) {
                            doc->lines.insert(doc->lines.begin() + lineIndex + 2, currentMarker);
                        } else {
                            doc->lines.insert(doc->lines.begin() + lineIndex + 2, L"");
                        }
                    }
                }
            }
        } else {
            // 不是流程控制命令也不是有参数的支持库命令
            // 但如果行中有括号，也需要格式化命令和括号之间的空格
            size_t bracketPos = line.find(L'(');
            if (bracketPos != std::wstring::npos && bracketPos > start) {
                // 统计括号前有多少个空格
                size_t beforeBracket = bracketPos;
                size_t spaceCount = 0;
                while (beforeBracket > start && (line[beforeBracket - 1] == L' ' || line[beforeBracket - 1] == L'\t')) {
                    beforeBracket--;
                    spaceCount++;
                }
                
                // 检查括号前是否是命令名（不是运算符等）
                if (beforeBracket > start) {
                    wchar_t ch = line[beforeBracket - 1];
                    bool isCommand = (ch >= 0x4E00 && ch <= 0x9FFF) ||  // 中文字符
                                     (ch >= L'a' && ch <= L'z') ||      // 小写字母
                                     (ch >= L'A' && ch <= L'Z') ||      // 大写字母
                                     (ch >= L'0' && ch <= L'9') ||      // 数字
                                     ch == L'_';                         // 下划线
                    
                    if (isCommand) {
                        if (spaceCount == 0) {
                            // 没有空格，添加一个
                            line.insert(bracketPos, L" ");
                        } else if (spaceCount > 1) {
                            // 多于一个空格，删除多余的
                            line.erase(beforeBracket, spaceCount - 1);
                        }
                    }
                }
            }
            
            // 检查是否是赋值语句，格式化为 var ＝ value
            if (IsAssignmentLine(line)) {
                int oldLen = (int)line.length();
                FormatAssignmentLine(line);
                // 格式化后行长度变化，需要调整光标位置
                if (lineIndex == doc->cursorLine) {
                    int delta = (int)line.length() - oldLen;
                    doc->cursorCol += delta;
                    if (doc->cursorCol < 0) doc->cursorCol = 0;
                    if (doc->cursorCol > (int)line.length()) doc->cursorCol = (int)line.length();
                }
            }
        }
    }
}

// 格式化文档中所有行（统一中英文符号，命令和括号之间添加空格，去除普通行缩进）
void FormatAllCommandLines(EditorDocument* doc) {
    if (!doc) return;
    
    for (int i = 0; i < (int)doc->lines.size(); i++) {
        std::wstring& line = doc->lines[i];
        if (line.empty()) continue;
        
        // 跳过表格行（包含制表符的行，如表头、数据行）
        if (line.find(L'\t') != std::wstring::npos) {
            continue;
        }
        
        // 跳过表头行
        if (line.find(L"程序集名") == 0 || line.find(L"子程序名") == 0 ||
            line.find(L"参数名") == 0 || line.find(L"变量名") == 0 ||
            line.find(L"常量名") == 0 || line.find(L"版本") == 0) {
            continue;
        }
        
        // 跳过参数展开行（以特殊标记开头）
        if (!line.empty() && line[0] == L'\u2060') {
            continue;
        }
        
        // 跳过流程控制内的行（以特殊字符标记开头）
        if (!line.empty() && (line[0] == L'\u200C' || line[0] == L'\u200D')) {
            // 流程控制内的行，只去除标记后的空格和TAB
            size_t contentStart = 1;
            while (contentStart < line.length() && (line[contentStart] == L' ' || line[contentStart] == L'\t')) {
                contentStart++;
            }
            if (contentStart > 1) {
                line = line[0] + line.substr(contentStart);
            }
            continue;
        }
        
        // 普通代码行：去除开头的空格和TAB缩进
        size_t contentStart = 0;
        while (contentStart < line.length() && (line[contentStart] == L' ' || line[contentStart] == L'\t')) {
            contentStart++;
        }
        if (contentStart > 0) {
            line = line.substr(contentStart);
        }
        
        // 如果行变空了，跳过后续处理
        if (line.empty()) continue;
        
        // 统一格式化：中文符号转英文符号
        for (size_t j = 0; j < line.length(); j++) {
            // 中文左括号 → 英文左括号
            if (line[j] == L'（') {
                line[j] = L'(';
            }
            // 中文右括号 → 英文右括号
            else if (line[j] == L'）') {
                line[j] = L')';
            }
            // 中文逗号 → 英文逗号
            else if (line[j] == L'，') {
                line[j] = L',';
            }
        }
        
        // 格式化命令：确保命令和括号之间有一个空格
        // 例如：返回() → 返回 ()，返回  () → 返回 ()
        size_t bracketPos = line.find(L'(');
        if (bracketPos != std::wstring::npos && bracketPos > 0) {
            // 找到括号前的位置
            size_t beforeBracket = bracketPos;
            
            // 统计括号前有多少个空格
            size_t spaceCount = 0;
            while (beforeBracket > 0 && (line[beforeBracket - 1] == L' ' || line[beforeBracket - 1] == L'\t')) {
                beforeBracket--;
                spaceCount++;
            }
            
            // 检查括号前是否是命令名（不是运算符等）
            // 如果括号前是中文字符或字母数字，说明可能是命令
            if (beforeBracket > 0) {
                wchar_t ch = line[beforeBracket - 1];
                bool isCommand = (ch >= 0x4E00 && ch <= 0x9FFF) ||  // 中文字符
                                 (ch >= L'a' && ch <= L'z') ||      // 小写字母
                                 (ch >= L'A' && ch <= L'Z') ||      // 大写字母
                                 (ch >= L'0' && ch <= L'9') ||      // 数字
                                 ch == L'_';                         // 下划线
                
                if (isCommand) {
                    if (spaceCount == 0) {
                        // 没有空格，添加一个空格
                        line.insert(bracketPos, L" ");
                    } else if (spaceCount > 1) {
                        // 多于一个空格，删除多余的，保留一个
                        line.erase(beforeBracket, spaceCount - 1);
                    }
                    // 正好一个空格，不需要修改
                }
            }
        }
        
        // 检查是否是赋值语句，格式化为 var ＝ value
        if (IsAssignmentLine(line)) {
            FormatAssignmentLine(line);
        }
    }
}

EditorData* g_EditorData = nullptr;

EditorDocument::EditorDocument() : cursorLine(0), cursorCol(0), scrollY(0), scrollX(0), modified(false),
                                   hasSelection(false), selStartLine(0), selStartCol(0), selEndLine(0), selEndCol(0), isSelecting(false), hFileLock(INVALID_HANDLE_VALUE),
                                   showDiff(false), hasUserInteraction(false), fileType(FILE_TYPE_EYC), syntaxCheckEnabled(true),
                                   tableLayoutDirty(true), cachedLineCount(0), cachedMaxLineWidth(0), maxLineWidthDirty(true),
                                   flowBlockDirty(true), cachedFontSize(0), cachedCharWidth(0), cachedChineseCharWidth(0) {
    fileName = L"未命名";
}

EditorDocument::~EditorDocument() {
    // 关闭文件锁
    if (hFileLock != INVALID_HANDLE_VALUE) {
        CloseHandle(hFileLock);
        hFileLock = INVALID_HANDLE_VALUE;
    }
}

void EditorDocument::CreateSnapshot(const std::wstring& desc, bool isNamed) {
    Snapshot snapshot;
    snapshot.lines = this->lines;
    snapshot.timestamp = std::time(nullptr);
    snapshot.description = desc;
    
    // 生成唯一ID
    wchar_t idBuf[64];
    swprintf_s(idBuf, L"%lld_%d", snapshot.timestamp, rand());
    snapshot.id = idBuf;
    
    if (isNamed) {
        wchar_t timeBuf[64];
        tm localTime;
        localtime_s(&localTime, &snapshot.timestamp);
        swprintf_s(timeBuf, L"%02d:%02d:%02d", localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
        snapshot.name = std::wstring(timeBuf) + L" " + desc;
        snapshots.push_back(snapshot);
        
        // 通知资源管理器更新
        extern void UpdateTimeline(EditorDocument* doc);
        UpdateTimeline(this);
    } else {
        undoStack.push_back(snapshot);
        // 限制栈大小
        if (undoStack.size() > 100) {
            undoStack.erase(undoStack.begin());
        }
        // 清空重做栈
        redoStack.clear();
    }
}

void EditorDocument::RestoreSnapshot(const Snapshot& snapshot) {
    this->lines = snapshot.lines;
    this->modified = true;
    MarkContentDirty();
    // 尝试恢复光标位置
    if (this->cursorLine >= (int)this->lines.size()) {
        this->cursorLine = (int)this->lines.size() - 1;
        if (this->cursorLine < 0) this->cursorLine = 0;
    }
    this->cursorCol = 0; // 简单重置列
}

void EditorDocument::Undo() {
    if (undoStack.empty()) return;
    
    // 保存当前状态到重做栈
    Snapshot current;
    current.lines = this->lines;
    redoStack.push_back(current);
    
    Snapshot snapshot = undoStack.back();
    undoStack.pop_back();
    RestoreSnapshot(snapshot);
}

void EditorDocument::Redo() {
    if (redoStack.empty()) return;
    
    // 保存当前状态到撤销栈
    Snapshot current;
    current.lines = this->lines;
    undoStack.push_back(current);
    
    Snapshot snapshot = redoStack.back();
    redoStack.pop_back();
    RestoreSnapshot(snapshot);
}

EditorData::EditorData() : activeDocIndex(-1), fontSize(12), rowHeight(26), tabHeight(0),  // 标签栏已禁用，使用外部TabBar
    scrollbarWidth(12), isDraggingVScroll(false), isDraggingHScroll(false),
    dragStartY(0), dragStartX(0), dragStartScrollY(0), dragStartScrollX(0),
    vScrollHover(false), hScrollHover(false), hoverTabIndex(-1),
    mouseDownX(0), mouseDownY(0), isDraggingSelection(false), isLineNumberSelection(false),
    showCompletion(false), completionX(0), completionY(0), selectedCompletionIndex(0),
    completionScrollOffset(0), completionMaxVisible(8),
    isDraggingCompletionScroll(false), completionDragStartY(0), completionDragStartOffset(0),
    hRightArrowCursor(NULL), showWelcomePage(true),
    showTypeCompletion(false), typeCompletionSelectedIndex(0), typeCompletionScrollOffset(0),
    currentCellIndex(-1), skipNextSpaceForType(false), skipNextChar(false) {
    // 启动时显示欢迎页，不创建默认文档
    
    // 初始化滚动条矩形
    vScrollbarRect = {0, 0, 0, 0};
    vScrollThumbRect = {0, 0, 0, 0};
    hScrollbarRect = {0, 0, 0, 0};
    hScrollThumbRect = {0, 0, 0, 0};
    
    // 初始化类型补全相关
    typeCompletionRect = {0, 0, 0, 0};
    currentCellRect = {0, 0, 0, 0};
    
    // 创建右箭头光标（镜像的箭头，用于行号区域）
    // 使用系统光标创建镜像版本
    HCURSOR hArrow = LoadCursor(NULL, IDC_ARROW);
    ICONINFO iconInfo;
    if (GetIconInfo(hArrow, &iconInfo)) {
        // 创建镜像位图
        BITMAP bm;
        GetObject(iconInfo.hbmMask, sizeof(BITMAP), &bm);
        
        HDC hdc = GetDC(NULL);
        HDC hdcMem = CreateCompatibleDC(hdc);
        HDC hdcMem2 = CreateCompatibleDC(hdc);
        
        HBITMAP hbmMask = CreateCompatibleBitmap(hdc, bm.bmWidth, bm.bmHeight);
        HBITMAP hbmColor = iconInfo.hbmColor ? CreateCompatibleBitmap(hdc, bm.bmWidth, bm.bmHeight) : NULL;
        
        // 镜像掩码位图
        SelectObject(hdcMem, iconInfo.hbmMask);
        SelectObject(hdcMem2, hbmMask);
        StretchBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight,
                   hdcMem, bm.bmWidth - 1, 0, -bm.bmWidth, bm.bmHeight, SRCCOPY);
        
        // 如果有颜色位图，也镜像它
        if (iconInfo.hbmColor && hbmColor) {
            SelectObject(hdcMem, iconInfo.hbmColor);
            SelectObject(hdcMem2, hbmColor);
            StretchBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight,
                       hdcMem, bm.bmWidth - 1, 0, -bm.bmWidth, bm.bmHeight, SRCCOPY);
        }
        
        DeleteDC(hdcMem);
        DeleteDC(hdcMem2);
        ReleaseDC(NULL, hdc);
        
        // 创建新的光标（热点位置也要镜像）
        ICONINFO newIconInfo = iconInfo;
        newIconInfo.hbmMask = hbmMask;
        newIconInfo.hbmColor = hbmColor;
        newIconInfo.xHotspot = bm.bmWidth - iconInfo.xHotspot - 1;
        
        hRightArrowCursor = CreateIconIndirect(&newIconInfo);
        
        // 清理资源
        DeleteObject(hbmMask);
        if (hbmColor) DeleteObject(hbmColor);
        DeleteObject(iconInfo.hbmMask);
        if (iconInfo.hbmColor) DeleteObject(iconInfo.hbmColor);
    }
}

EditorData::~EditorData() {
    // 销毁自定义光标
    if (hRightArrowCursor) {
        DestroyCursor(hRightArrowCursor);
        hRightArrowCursor = NULL;
    }
    
    // 清理文档
    for (auto doc : documents) {
        delete doc;
    }
    documents.clear();
}

EditorDocument* EditorData::GetActiveDoc() {
    if (activeDocIndex >= 0 && activeDocIndex < (int)documents.size()) {
        return documents[activeDocIndex];
    }
    return nullptr;
}

void EditorData::AddDocument(const std::wstring& path) {
    // 添加文档时关闭欢迎页
    showWelcomePage = false;
    
    EditorDocument* doc = new EditorDocument();
    doc->filePath = path;
    if (path.empty()) {
        doc->fileName = L"未命名";
        // 新建文档初始化默认内容
        doc->lines.clear();
        doc->lines.push_back(L"版本\t2");
        doc->lines.push_back(L"程序集名\t保留\t保留\t备注");
        doc->lines.push_back(L"程序集1\t\t\t");
        doc->lines.push_back(L"");
    } else {
        size_t lastSlash = path.find_last_of(L"\\/");
        if (lastSlash != std::wstring::npos) {
            doc->fileName = path.substr(lastSlash + 1);
        } else {
            doc->fileName = path;
        }
    }
    documents.push_back(doc);
    activeDocIndex = (int)documents.size() - 1;
}

void EditorData::CloseDocument(int index) {
    if (index >= 0 && index < (int)documents.size()) {
        delete documents[index];
        documents.erase(documents.begin() + index);
        
        if (activeDocIndex >= (int)documents.size()) {
            activeDocIndex = (int)documents.size() - 1;
        }
        if (documents.empty()) {
            // 没有文档时显示欢迎页
            showWelcomePage = true;
            activeDocIndex = -1;
        }
    }
}

void EditorData::SwitchToDocument(int index) {
    if (index >= 0 && index < (int)documents.size()) {
        activeDocIndex = index;
    }
}

int EditorData::FindDocument(const std::wstring& filePath) {
    for (int i = 0; i < (int)documents.size(); i++) {
        if (documents[i]->filePath == filePath) {
            return i;
        }
    }
    return -1;
}

std::wstring GetSelectedText(EditorDocument* doc) {
    if (!doc->hasSelection) return L"";

    int startL, startC, endL, endC;

    // 规范化选择区域
    if (doc->selStartLine < doc->selEndLine) {
        startL = doc->selStartLine; startC = doc->selStartCol;
        endL = doc->selEndLine; endC = doc->selEndCol;
    } else if (doc->selStartLine > doc->selEndLine) {
        startL = doc->selEndLine; startC = doc->selEndCol;
        endL = doc->selStartLine; endC = doc->selStartCol;
    } else {
        startL = doc->selStartLine;
        endL = doc->selEndLine;
        if (doc->selStartCol < doc->selEndCol) {
            startC = doc->selStartCol; endC = doc->selEndCol;
        } else {
            startC = doc->selEndCol; endC = doc->selStartCol;
        }
    }

    if (startL >= (int)doc->lines.size()) return L"";
    if (endL >= (int)doc->lines.size()) endL = (int)doc->lines.size() - 1;

    std::wstring result;
    if (startL == endL) {
        const std::wstring& line = doc->lines[startL];
        // 跳过参数展开行（以\u2060开头）
        if (line.length() > 0 && line[0] == L'\u2060') {
            return L"";
        }
        int len = (int)line.length();
        int s = (startC > len) ? len : startC;
        int e = (endC > len) ? len : endC;
        if (s < e) {
            result = line.substr(s, e - s);
        }
    } else {
        // 第一行
        const std::wstring& startLine = doc->lines[startL];
        // 跳过参数展开行
        if (!(startLine.length() > 0 && startLine[0] == L'\u2060')) {
            if (startC < (int)startLine.length()) {
                result += startLine.substr(startC);
            }
            result += L"\r\n";
        }

        // 中间行
        for (int i = startL + 1; i < endL; i++) {
            const std::wstring& midLine = doc->lines[i];
            // 跳过参数展开行（以\u2060开头）
            if (midLine.length() > 0 && midLine[0] == L'\u2060') {
                continue;
            }
            result += midLine;
            result += L"\r\n";
        }

        // 最后一行
        const std::wstring& endLine = doc->lines[endL];
        // 跳过参数展开行
        if (!(endLine.length() > 0 && endLine[0] == L'\u2060')) {
            if (endC > 0) {
                result += endLine.substr(0, (std::min)(endC, (int)endLine.length()));
            }
        }
    }
    return result;
}

// 检查一行是否是表头行
static bool IsTableHeader(const std::wstring& line) {
    return line.find(L"子程序名") == 0 || 
           line.find(L"参数名") == 0 || 
           line.find(L"变量名") == 0;
}

// 检查一行是否是程序集表头
static bool IsAssemblyHeader(const std::wstring& line) {
    return line.find(L"程序集名") == 0;
}

// 检查当前行是否在程序集区域但不在任何表内（这种位置不允许输入普通代码）
static bool IsInAssemblyButNotInTable(EditorDocument* doc, int lineIndex) {
    if (lineIndex >= (int)doc->lines.size()) return false;
    
    // 向上查找，看是否在程序集区域内
    bool inAssembly = false;
    int assemblyVarEndLine = -1;  // 程序集变量表结束行
    
    for (int i = lineIndex; i >= 0; i--) {
        const std::wstring& line = doc->lines[i];
        
        // 遇到子程序名表头，说明已经不在程序集区域了
        if (line.find(L"子程序名") == 0) {
            return false;
        }
        
        // 遇到程序集名表头，说明在程序集区域内
        if (line.find(L"程序集名") == 0) {
            inAssembly = true;
            break;
        }
    }
    
    if (!inAssembly) return false;
    
    // 在程序集区域内，需要找到程序集变量表的结束位置
    for (int i = 0; i < (int)doc->lines.size(); i++) {
        const std::wstring& line = doc->lines[i];
        
        if (line.find(L"程序集名") == 0) {
            // 找到程序集表头后，继续找变量名表头
            for (int j = i + 1; j < (int)doc->lines.size(); j++) {
                const std::wstring& varLine = doc->lines[j];
                
                // 找到程序集的变量名表头
                if (varLine.find(L"变量名") == 0) {
                    // 继续向下找，直到遇到空行、表头或子程序
                    for (int k = j + 1; k < (int)doc->lines.size(); k++) {
                        const std::wstring& nextLine = doc->lines[k];
                        
                        if (nextLine.empty() || 
                            nextLine.find(L"子程序名") == 0 ||
                            nextLine.find(L"程序集名") == 0) {
                            assemblyVarEndLine = k - 1;
                            break;
                        }
                    }
                    if (assemblyVarEndLine == -1) {
                        assemblyVarEndLine = (int)doc->lines.size() - 1;
                    }
                    break;
                }
                
                // 如果程序集后面没有变量表，那么程序集数据行就是结束位置
                if (varLine.find(L"子程序名") == 0) {
                    assemblyVarEndLine = j - 1;
                    break;
                }
            }
            break;
        }
    }
    
    // 如果当前行在程序集变量表结束后、下一个子程序表前，且不是表行
    if (lineIndex > assemblyVarEndLine) {
        const std::wstring& currentLine = doc->lines[lineIndex];
        
        // 如果当前行是子程序表头或更后面，允许
        for (int i = lineIndex; i < (int)doc->lines.size(); i++) {
            if (doc->lines[i].find(L"子程序名") == 0) {
                // 如果当前行就是子程序名表头或在其之后，允许
                if (i == lineIndex) return false;
                // 如果在子程序名表头之前，不允许
                return true;
            }
        }
        
        // 没有找到子程序表，说明在程序集末尾，不允许普通代码
        if (currentLine.find(L'\t') == std::wstring::npos) {  // 不是表格行
            return true;
        }
    }
    
    return false;
}

// 检查 lineIndex 行是否是其所在子程序中唯一的普通代码行
// 普通代码行 = 不含制表符且不以\u2060开头的行（可以为空行）
static bool IsOnlyNormalLineInSubroutine(EditorDocument* doc, int lineIndex) {
    if (!doc || lineIndex < 0 || lineIndex >= (int)doc->lines.size()) return false;
    
    const std::wstring& line = doc->lines[lineIndex];
    // 当前行必须是普通行
    if (line.find(L'\t') != std::wstring::npos) return false;
    if (line.length() > 0 && line[0] == L'\u2060') return false;
    
    // 向上查找，确认在某个子程序内
    bool foundSubHeader = false;
    for (int i = lineIndex - 1; i >= 0; i--) {
        const std::wstring& l = doc->lines[i];
        if (l.find(L"子程序名") == 0) { foundSubHeader = true; break; }
        if (l.find(L"程序集名") == 0) return false;
        if (l.find(L'\t') != std::wstring::npos) continue;  // 表格行
        if (l.length() > 0 && l[0] == L'\u2060') continue;  // 参数行
        // 遇到另一个普通行，说明不是唯一的
        return false;
    }
    if (!foundSubHeader) return false;
    
    // 向下查找同一子程序内是否还有其他普通代码行
    for (int i = lineIndex + 1; i < (int)doc->lines.size(); i++) {
        const std::wstring& l = doc->lines[i];
        if (l.find(L"子程序名") == 0 || l.find(L"程序集名") == 0) break;
        if (l.find(L'\t') != std::wstring::npos) continue;
        if (l.length() > 0 && l[0] == L'\u2060') continue;
        return false;  // 还有其他普通行
    }
    return true;
}

// 清理孤立的表头（表头后面没有数据行时删除表头）
static void CleanupOrphanedHeaders(EditorDocument* doc) {
    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < (int)doc->lines.size(); i++) {
            const std::wstring& line = doc->lines[i];
            
            // 跳过程序集名表头（但程序集变量表可以删除）
            if (IsAssemblyHeader(line)) {
                continue;
            }
            
            // 检查所有类型的表头：子程序名、参数名、变量名（包括程序集变量）
            if (IsTableHeader(line)) {
                // 检查下一行是否是数据行
                bool hasData = false;
                if (i + 1 < (int)doc->lines.size()) {
                    const std::wstring& nextLine = doc->lines[i + 1];
                    // 如果下一行不是空行、不是另一个表头、不是程序集表头，则有数据
                    if (!nextLine.empty() && 
                        !IsTableHeader(nextLine) && 
                        !IsAssemblyHeader(nextLine) &&
                        nextLine.find(L"版本\t") != 0) {
                        hasData = true;
                    }
                }
                
                // 如果没有数据行，删除这个表头
                // 对于程序集变量表（变量名表头但无"静态"字段），也可以删除
                if (!hasData) {
                    doc->lines.erase(doc->lines.begin() + i);
                    // 调整光标位置
                    if (doc->cursorLine > i) {
                        doc->cursorLine--;
                    } else if (doc->cursorLine == i && doc->cursorLine > 0) {
                        doc->cursorLine--;
                        doc->cursorCol = 0;
                    }
                    changed = true;
                    break;  // 重新开始扫描
                }
            }
        }
    }
}

void DeleteSelection(EditorDocument* doc) {
    if (!doc->hasSelection) return;

    int startL, startC, endL, endC;

    // 规范化选择区域
    if (doc->selStartLine < doc->selEndLine) {
        startL = doc->selStartLine; startC = doc->selStartCol;
        endL = doc->selEndLine; endC = doc->selEndCol;
    } else if (doc->selStartLine > doc->selEndLine) {
        startL = doc->selEndLine; startC = doc->selEndCol;
        endL = doc->selStartLine; endC = doc->selStartCol;
    } else {
        startL = doc->selStartLine;
        endL = doc->selEndLine;
        if (doc->selStartCol < doc->selEndCol) {
            startC = doc->selStartCol; endC = doc->selEndCol;
        } else {
            startC = doc->selEndCol; endC = doc->selStartCol;
        }
    }

    if (startL >= (int)doc->lines.size()) return;
    if (endL >= (int)doc->lines.size()) endL = (int)doc->lines.size() - 1;

    // 找到程序集区域的范围（程序集名表头、数据行、变量名表头、变量数据行）
    int assemblyHeaderLine = -1;
    int assemblyDataLine = -1;
    int assemblyVarHeaderLine = -1;
    int assemblyVarEndLine = -1;
    
    for (int i = 0; i < (int)doc->lines.size(); i++) {
        if (doc->lines[i].find(L"程序集名") == 0) {
            assemblyHeaderLine = i;
            if (i + 1 < (int)doc->lines.size()) {
                assemblyDataLine = i + 1;
            }
        }
        if (assemblyHeaderLine != -1 && doc->lines[i].find(L"变量名") == 0 && i > assemblyHeaderLine) {
            // 检查是否是程序集变量（4列，无"静态"）
            if (doc->lines[i].find(L"静态") == std::wstring::npos) {
                assemblyVarHeaderLine = i;
                // 找变量结束位置
                for (int j = i + 1; j < (int)doc->lines.size(); j++) {
                    if (doc->lines[j].find(L"子程序名") == 0 || doc->lines[j].empty()) {
                        assemblyVarEndLine = j - 1;
                        break;
                    }
                    assemblyVarEndLine = j;
                }
                break;
            }
        }
    }

    // 检查选择是否涉及程序集保护区域
    bool involvesAssemblyHeader = (assemblyHeaderLine != -1 && startL <= assemblyHeaderLine && endL >= assemblyHeaderLine);
    bool involvesAssemblyData = (assemblyDataLine != -1 && startL <= assemblyDataLine && endL >= assemblyDataLine);
    bool involvesAssemblyVarHeader = (assemblyVarHeaderLine != -1 && startL <= assemblyVarHeaderLine && endL >= assemblyVarHeaderLine);
    
    // 如果涉及程序集表头或数据行，特殊处理
    if (involvesAssemblyHeader || involvesAssemblyData) {
        // 策略：保留程序集结构，删除其他所有选中内容
        
        // 构建新的文档内容
        std::vector<std::wstring> newLines;
        
        // 1. 保留版本行（如果存在且不在选择范围内）
        int versionLine = -1;
        if (!doc->lines.empty() && doc->lines[0].find(L"版本\t") == 0) {
            versionLine = 0;
            newLines.push_back(doc->lines[0]);
        }
        
        // 2. 添加程序集表头和空数据行
        if (assemblyHeaderLine != -1 && assemblyHeaderLine < (int)doc->lines.size()) {
            newLines.push_back(doc->lines[assemblyHeaderLine]);  // 程序集名
            newLines.push_back(L"\t\t\t");  // 空数据行
        }
        
        // 3. 添加程序集变量表头（如果存在）
        if (assemblyVarHeaderLine != -1 && assemblyVarHeaderLine < (int)doc->lines.size()) {
            newLines.push_back(doc->lines[assemblyVarHeaderLine]);  // 变量名表头
        }
        
        // 4. 添加选择范围之后未被选中的内容
        if (endL + 1 < (int)doc->lines.size()) {
            // 如果endL行还有剩余部分（部分选择）
            if (endC < (int)doc->lines[endL].length()) {
                newLines.push_back(doc->lines[endL].substr(endC));
            }
            // 添加endL之后的所有行
            for (int i = endL + 1; i < (int)doc->lines.size(); i++) {
                newLines.push_back(doc->lines[i]);
            }
        }
        
        // 替换文档内容
        doc->lines = newLines;
        
        // 设置光标位置到程序集数据行
        doc->cursorLine = (versionLine != -1) ? 2 : 1;  // 版本行+程序集表头+数据行
        doc->cursorCol = 0;
        doc->hasSelection = false;
        doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
        CleanupOrphanedHeaders(doc);
        return;
    }

    // 普通删除逻辑
    if (startL == endL) {
        std::wstring& line = doc->lines[startL];
        if (startC > (int)line.length()) startC = (int)line.length();
        if (endC > (int)line.length()) endC = (int)line.length();
        
        if (startC < endC) {
            line.erase(startC, endC - startC);
        }
    } else {
        std::wstring& startLine = doc->lines[startL];
        std::wstring& endLine = doc->lines[endL];
        
        if (startC > (int)startLine.length()) startC = (int)startLine.length();
        if (endC > (int)endLine.length()) endC = (int)endLine.length();
        
        // Keep start part
        startLine = startLine.substr(0, startC);
        
        // Append end part
        startLine += endLine.substr(endC);
        
        // Remove intermediate lines
        doc->lines.erase(doc->lines.begin() + startL + 1, doc->lines.begin() + endL + 1);
    }

    doc->cursorLine = startL;
    doc->cursorCol = startC;
    doc->hasSelection = false;
    doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
    
    // 清理孤立的表头
    CleanupOrphanedHeaders(doc);
    
    // 确保每个子程序至少有一个普通代码行
    for (int i = 0; i < (int)doc->lines.size(); i++) {
        if (doc->lines[i].find(L"子程序名") == 0 && i + 1 < (int)doc->lines.size()) {
            // 找到子程序表格区域的结束位置
            int tableEnd = i + 1;  // 至少跳过数据行
            for (int j = i + 1; j < (int)doc->lines.size(); j++) {
                const std::wstring& l = doc->lines[j];
                if (l.find(L'\t') != std::wstring::npos) {
                    tableEnd = j + 1;
                    continue;
                }
                if (l.length() > 0 && l[0] == L'\u2060') {
                    tableEnd = j + 1;
                    continue;
                }
                break;
            }
            // 检查 tableEnd 后面是否有普通代码行（在下一个子程序之前）
            bool hasNormal = false;
            for (int j = tableEnd; j < (int)doc->lines.size(); j++) {
                const std::wstring& l = doc->lines[j];
                if (l.find(L"子程序名") == 0 || l.find(L"程序集名") == 0) break;
                if (l.find(L'\t') == std::wstring::npos && !(l.length() > 0 && l[0] == L'\u2060')) {
                    hasNormal = true;
                    break;
                }
            }
            if (!hasNormal) {
                doc->lines.insert(doc->lines.begin() + tableEnd, L"");
            }
        }
    }
}

void InsertText(EditorDocument* doc, const std::wstring& text) {
    if (doc->hasSelection) {
        DeleteSelection(doc);
    }

    // Split text into lines
    std::vector<std::wstring> newLines;
    std::wstring currentLine;
    for (wchar_t c : text) {
        if (c == L'\r') continue;
        if (c == L'\n') {
            newLines.push_back(currentLine);
            currentLine.clear();
        } else {
            currentLine += c;
        }
    }
    newLines.push_back(currentLine);

    if (newLines.empty()) return;

    // Insert at cursor
    if (doc->cursorLine >= (int)doc->lines.size()) {
        doc->lines.push_back(L"");
        doc->cursorLine = (int)doc->lines.size() - 1;
        doc->cursorCol = 0;
    }

    std::wstring& line = doc->lines[doc->cursorLine];
    if (doc->cursorCol > (int)line.length()) doc->cursorCol = (int)line.length();

    std::wstring suffix = line.substr(doc->cursorCol);
    line = line.substr(0, doc->cursorCol) + newLines[0];

    if (newLines.size() == 1) {
        doc->cursorCol += (int)newLines[0].length();
        line += suffix;
    } else {
        for (size_t i = 1; i < newLines.size(); i++) {
            doc->lines.insert(doc->lines.begin() + doc->cursorLine + i, newLines[i]);
        }
        doc->lines[doc->cursorLine + newLines.size() - 1] += suffix;
        doc->cursorLine += (int)newLines.size() - 1;
        doc->cursorCol = (int)newLines.back().length();
    }
    doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
}

// 菜单命令ID
enum {
    IDM_NEW_SUB = 2001,
    IDM_COPY,
    IDM_PASTE,
    IDM_CUT,
    IDM_SELECT_ALL,
    IDM_COMMENT,
    IDM_UNCOMMENT
};

void SelectAll(EditorDocument* doc) {
    if (doc->lines.empty()) return;
    doc->selStartLine = 0;
    doc->selStartCol = 0;
    doc->selEndLine = (int)doc->lines.size() - 1;
    doc->selEndCol = (int)doc->lines.back().length();
    doc->hasSelection = true;
    doc->isSelecting = false;
}

// 获取唯一的子程序名称
std::wstring GetUniqueSubroutineName(EditorDocument* doc, const std::wstring& baseName = L"子程序") {
    std::set<std::wstring> existingNames;
    for (size_t i = 0; i < doc->lines.size(); i++) {
        if (doc->lines[i].find(L"子程序名") == 0) {
            if (i + 1 < doc->lines.size()) {
                std::wstring dataLine = doc->lines[i + 1];
                size_t tabPos = dataLine.find(L'\t');
                if (tabPos != std::wstring::npos) {
                    existingNames.insert(dataLine.substr(0, tabPos));
                } else {
                    existingNames.insert(dataLine);
                }
            }
        }
    }

    // 如果baseName不重复，直接返回
    if (existingNames.find(baseName) == existingNames.end()) {
        return baseName;
    }

    // 尝试 baseName + 数字
    int index = 1;
    while (true) {
        std::wstring newName = baseName + std::to_wstring(index);
        if (existingNames.find(newName) == existingNames.end()) {
            return newName;
        }
        index++;
    }
}

void InsertNewSubroutine(EditorDocument* doc) {
    doc->CreateSnapshot(L"Insert Subroutine");
    // 寻找插入位置：当前子程序之后，或者下一个子程序/程序集之前
    int insertLine = (int)doc->lines.size();
    
    // 从当前行向下查找下一个子程序
    for (int i = doc->cursorLine + 1; i < (int)doc->lines.size(); i++) {
        const std::wstring& line = doc->lines[i];
        // 检查是否是子程序名
        // 注意：不检查程序集名，因为程序集必须在最上方，新子程序永远插在程序集之后
        if (line.find(L"子程序名") == 0) {
            insertLine = i;
            break;
        }
    }
    
    // 向上回溯找到上一个非空行，以确定上下文
    int lastContentLine = insertLine - 1;
    while (lastContentLine >= 0 && doc->lines[lastContentLine].empty()) {
        lastContentLine--;
    }
    
    bool isAssemblyContext = false;
    if (lastContentLine >= 0) {
        // 向上查找最近的表头
        for (int i = lastContentLine; i >= 0; i--) {
            if (doc->lines[i].find(L"程序集名") == 0) {
                isAssemblyContext = true;
                break;
            }
            if (doc->lines[i].find(L"子程序名") == 0) {
                isAssemblyContext = false;
                break;
            }
        }
    } else {
        // 文件为空或只有空行（理论上不应该，因为有默认程序集）
        isAssemblyContext = true;
    }
    
    // 清除插入点之前的空行，确保紧凑或控制间隔
    if (insertLine > lastContentLine + 1) {
        doc->lines.erase(doc->lines.begin() + lastContentLine + 1, doc->lines.begin() + insertLine);
        insertLine = lastContentLine + 1;
    }
    
    // 移动光标到插入位置
    doc->cursorLine = insertLine;
    doc->cursorCol = 0;

    std::wstring newSubName = GetUniqueSubroutineName(doc, L"子程序");

    std::wstring subTemplate;
    if (isAssemblyContext) {
        // 程序集下：紧跟，无空行
        subTemplate = L"子程序名\t返回值类型\t公开\t备注\n" +
                      newSubName + L"\t\t\t\n";
    } else {
        // 子程序之间：必须有空行
        subTemplate = L"\n子程序名\t返回值类型\t公开\t备注\n" +
                      newSubName + L"\t\t\t\n";
    }
    
    // 如果插入点后面还有内容（即插入在中间），需要额外加一个换行符作为分隔
    if (insertLine < (int)doc->lines.size()) {
        subTemplate += L"\n";
    }
    
    InsertText(doc, subTemplate);
}

void CommentLine(EditorDocument* doc) {
    int startL = doc->cursorLine;
    int endL = doc->cursorLine;
    
    if (doc->hasSelection) {
        startL = std::min(doc->selStartLine, doc->selEndLine);
        endL = std::max(doc->selStartLine, doc->selEndLine);
    }
    
    for (int i = startL; i <= endL; i++) {
        if (i < (int)doc->lines.size()) {
            std::wstring& line = doc->lines[i];
            if (line.empty() || line[0] != L'\'') {
                line = L"'" + line;
                doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
            }
        }
    }
}

void UncommentLine(EditorDocument* doc) {
    int startL = doc->cursorLine;
    int endL = doc->cursorLine;
    
    if (doc->hasSelection) {
        startL = std::min(doc->selStartLine, doc->selEndLine);
        endL = std::max(doc->selStartLine, doc->selEndLine);
    }
    
    for (int i = startL; i <= endL; i++) {
        if (i < (int)doc->lines.size()) {
            std::wstring& line = doc->lines[i];
            if (line.length() > 0 && line[0] == L'\'') {
                line = line.substr(1);
                doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
            }
        }
    }
}

void UpdateScrollBars(HWND hWnd);

void CopyToClipboard(HWND hWnd, EditorDocument* doc) {
    std::wstring text = GetSelectedText(doc);
    if (text.empty()) return;
    
    // 将内部格式转换为EPL格式（包含将√转换为参考、可空等文本）
    std::vector<std::wstring> lines = SplitString(text, L"\r\n");
    if (lines.empty()) lines = SplitString(text, L"\n");
    std::wstring eplText = ConvertInternalToEPL(lines);

    // 确保以 .版本 2 开头
    if (eplText.find(L".版本 2") != 0) {
        eplText = L".版本 2\r\n" + eplText;
    }

    if (OpenClipboard(hWnd)) {
        EmptyClipboard();
        HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, (eplText.length() + 1) * sizeof(wchar_t));
        if (hGlob) {
            memcpy(GlobalLock(hGlob), eplText.c_str(), (eplText.length() + 1) * sizeof(wchar_t));
            GlobalUnlock(hGlob);
            SetClipboardData(CF_UNICODETEXT, hGlob);
        }
        CloseClipboard();
    }
}

#include <set>

// 获取唯一的子程序名称
std::wstring GetUniqueSubroutineName(EditorDocument* doc, const std::wstring& baseName);

void PasteFromClipboard(HWND hWnd, EditorDocument* doc) {
    if (OpenClipboard(hWnd)) {
        HANDLE hData = GetClipboardData(CF_UNICODETEXT);
        if (hData) {
            wchar_t* pszText = (wchar_t*)GlobalLock(hData);
            if (pszText) {
                std::wstring text(pszText);
                
                // 检测是否包含易语言代码特征
                bool looksLikeEPL = false;
                if (text.find(L".版本") != std::wstring::npos || 
                    text.find(L".程序集") != std::wstring::npos || 
                    text.find(L".子程序") != std::wstring::npos ||
                    text.find(L".参数") != std::wstring::npos ||
                    text.find(L".局部变量") != std::wstring::npos ||
                    text.find(L".全局变量") != std::wstring::npos ||
                    text.find(L".如果") != std::wstring::npos ||
                    text.find(L".如果真") != std::wstring::npos ||
                    text.find(L".否则") != std::wstring::npos ||
                    text.find(L".判断") != std::wstring::npos ||
                    text.find(L".循环") != std::wstring::npos ||
                    // 支持无点号前缀的格式
                    (text.find(L"子程序") != std::wstring::npos && text.find(L"子程序名") == std::wstring::npos) ||
                    (text.find(L"参数") != std::wstring::npos && text.find(L"参数名") == std::wstring::npos) ||
                    (text.find(L"局部变量") != std::wstring::npos && text.find(L"变量名") == std::wstring::npos)) {
                    looksLikeEPL = true;
                }

                if (looksLikeEPL) {
                    std::wstring converted = ConvertEPLToInternal(text);
                    
                    // 记录插入前的光标位置
                    int insertStartLine = doc->cursorLine;
                    
                    // 解析转换后的文本
                    std::vector<std::wstring> lines = SplitString(converted, L"\n");
                    std::vector<std::wstring> assemblyVars;
                    std::vector<std::wstring> otherLines;
                    std::wstring assemblyNameLine;
                    bool hasAssemblyHeader = false;
                    bool inAssemblyVars = false;

                    // 预先扫描文档中已有的子程序名
                    std::set<std::wstring> existingNames;
                    for (size_t i = 0; i < doc->lines.size(); i++) {
                        if (doc->lines[i].find(L"子程序名") == 0) {
                            if (i + 1 < doc->lines.size()) {
                                std::wstring dataLine = doc->lines[i + 1];
                                size_t tabPos = dataLine.find(L'\t');
                                if (tabPos != std::wstring::npos) {
                                    existingNames.insert(dataLine.substr(0, tabPos));
                                } else {
                                    existingNames.insert(dataLine);
                                }
                            }
                        }
                    }

                    for (size_t i = 0; i < lines.size(); i++) {
                        std::wstring& line = lines[i];
                        if (!line.empty() && line.back() == L'\r') line.pop_back();
                        
                        if (line.empty()) {
                            if (!inAssemblyVars) otherLines.push_back(line);
                            continue;
                        }

                        if (line.find(L"程序集名") == 0) {
                            hasAssemblyHeader = true;
                            inAssemblyVars = false;
                            if (i + 1 < lines.size()) {
                                assemblyNameLine = lines[i+1];
                                if (!assemblyNameLine.empty() && assemblyNameLine.back() == L'\r') assemblyNameLine.pop_back();
                                i++;
                            }
                            continue;
                        }

                        if (line.find(L"变量名") == 0) {
                            // 区分程序集变量(4列)和局部变量(5列，含静态)
                            if (line.find(L"静态") == std::wstring::npos) {
                                inAssemblyVars = true;
                                continue; // 跳过表头
                            } else {
                                inAssemblyVars = false;
                                otherLines.push_back(line);
                                continue;
                            }
                        }

                        // 版本行应该被忽略（会被隐藏）
                        if (line.find(L"版本") == 0) {
                            continue;
                        }

                        if (line.find(L"子程序名") == 0) {
                            inAssemblyVars = false;
                            // 确保子程序之间有空行
                            if (!otherLines.empty() && !otherLines.back().empty()) {
                                otherLines.push_back(L"");
                            }
                            otherLines.push_back(line);
                            
                            // 处理下一行（子程序数据行），检查重名
                            if (i + 1 < lines.size()) {
                                std::wstring& dataLine = lines[i+1];
                                if (!dataLine.empty() && dataLine.back() == L'\r') dataLine.pop_back();
                                
                                // 解析名称
                                std::wstring name;
                                std::wstring rest;
                                size_t tabPos = dataLine.find(L'\t');
                                if (tabPos != std::wstring::npos) {
                                    name = dataLine.substr(0, tabPos);
                                    rest = dataLine.substr(tabPos);
                                } else {
                                    name = dataLine;
                                }
                                
                                // 检查重名并重命名
                                if (existingNames.find(name) != existingNames.end()) {
                                    // 生成新名称
                                    std::wstring baseName = name;
                                    // 如果是以数字结尾，尝试分离
                                    size_t lastNonDigit = baseName.find_last_not_of(L"0123456789");
                                    if (lastNonDigit != std::wstring::npos && lastNonDigit < baseName.length() - 1) {
                                        baseName = baseName.substr(0, lastNonDigit + 1);
                                    }
                                    
                                    int index = 1;
                                    while (true) {
                                        std::wstring newName = baseName + std::to_wstring(index);
                                        if (existingNames.find(newName) == existingNames.end()) {
                                            name = newName;
                                            break;
                                        }
                                        index++;
                                    }
                                }
                                
                                // 记录新名称
                                existingNames.insert(name);
                                
                                // 更新行内容
                                dataLine = name + rest;
                                otherLines.push_back(dataLine);
                                i++; // 跳过已处理的数据行
                            }
                            continue;
                        }

                        if (inAssemblyVars) {
                            assemblyVars.push_back(line);
                        } else {
                            otherLines.push_back(line);
                        }
                    }

                    // 检查文档是否已有程序集表格
                    int docAssemblyIndex = -1;
                    int docAssemblyVarsEnd = -1;
                    for (int i = 0; i < (int)doc->lines.size(); i++) {
                        if (doc->lines[i].find(L"程序集名") == 0) {
                            docAssemblyIndex = i;
                            // 寻找变量结束位置
                            int j = i + 2; // 跳过表头和名称
                            if (j < (int)doc->lines.size() && doc->lines[j].find(L"变量名") == 0) {
                                j++;
                                while (j < (int)doc->lines.size()) {
                                    if (doc->lines[j].find(L"子程序名") == 0 || doc->lines[j].find(L"程序集名") == 0) break;
                                    j++;
                                }
                            }
                            docAssemblyVarsEnd = j;
                            break;
                        }
                    }

                    if (docAssemblyIndex != -1) {
                        // 合并变量到现有程序集
                        if (!assemblyVars.empty()) {
                            bool hasHeader = false;
                            if (docAssemblyIndex + 2 < (int)doc->lines.size() && doc->lines[docAssemblyIndex + 2].find(L"变量名") == 0) {
                                hasHeader = true;
                            }

                            int insertPos = docAssemblyVarsEnd;
                            if (!hasHeader) {
                                doc->lines.insert(doc->lines.begin() + insertPos, L"变量名\t类型\t数组\t备注");
                                insertPos++;
                            }
                            
                            doc->lines.insert(doc->lines.begin() + insertPos, assemblyVars.begin(), assemblyVars.end());
                            
                            if (doc->cursorLine >= insertPos) {
                                doc->cursorLine += (int)assemblyVars.size() + (hasHeader ? 0 : 1);
                            }
                        }
                        
                        // 插入其他内容
                        if (!otherLines.empty()) {
                            std::wstring otherText;
                            for (const auto& l : otherLines) otherText += l + L"\n";
                            if (!otherText.empty() && otherText.back() == L'\n') otherText.pop_back();
                            InsertText(doc, otherText);
                            
                            // 对插入的行调用CheckAndFormatKeywords
                            int endLine = doc->cursorLine;
                            int startLine = insertStartLine;
                            for (int i = startLine; i <= endLine && i < (int)doc->lines.size(); i++) {
                                CheckAndFormatKeywords(doc, i);
                            }
                        } else {
                            doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                        }
                    } else {
                        // 文档无程序集，如果有程序集信息则置顶
                        if (hasAssemblyHeader || !assemblyVars.empty()) {
                            std::wstring asmText = L"程序集名\t保留\t保留\t备注\n";
                            asmText += (assemblyNameLine.empty() ? L"\t\t\t" : assemblyNameLine) + L"\n";
                            if (!assemblyVars.empty()) {
                                asmText += L"变量名\t类型\t数组\t备注\n";
                                for (const auto& l : assemblyVars) asmText += l + L"\n";
                            }
                            
                            std::vector<std::wstring> asmLines = SplitString(asmText, L"\n");
                            if (!asmLines.empty() && asmLines.back().empty()) asmLines.pop_back();
                            
                            doc->lines.insert(doc->lines.begin(), asmLines.begin(), asmLines.end());
                            doc->cursorLine += (int)asmLines.size();
                            insertStartLine += (int)asmLines.size();
                        }
                        
                        if (!otherLines.empty()) {
                            std::wstring otherText;
                            for (const auto& l : otherLines) otherText += l + L"\n";
                            if (!otherText.empty() && otherText.back() == L'\n') otherText.pop_back();
                            InsertText(doc, otherText);
                            
                            // 对插入的行调用CheckAndFormatKeywords
                            int endLine = doc->cursorLine;
                            int startLine = insertStartLine;
                            for (int i = startLine; i <= endLine && i < (int)doc->lines.size(); i++) {
                                CheckAndFormatKeywords(doc, i);
                            }
                        } else {
                            doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                        }
                    }

                } else {
                    // 非EPL格式，直接插入
                    // 先去除每行开头的空格和TAB缩进
                    std::vector<std::wstring> pasteLines = SplitString(text, L"\n");
                    std::wstring cleanText;
                    for (size_t li = 0; li < pasteLines.size(); li++) {
                        std::wstring& pasteLine = pasteLines[li];
                        // 去除回车符
                        if (!pasteLine.empty() && pasteLine.back() == L'\r') {
                            pasteLine.pop_back();
                        }
                        // 去除开头的空格和TAB
                        size_t contentStart = 0;
                        while (contentStart < pasteLine.length() && (pasteLine[contentStart] == L' ' || pasteLine[contentStart] == L'\t')) {
                            contentStart++;
                        }
                        if (contentStart > 0) {
                            pasteLine = pasteLine.substr(contentStart);
                        }
                        cleanText += pasteLine;
                        if (li < pasteLines.size() - 1) {
                            cleanText += L"\n";
                        }
                    }
                    
                    int insertStartLine = doc->cursorLine;
                    InsertText(doc, cleanText);
                    
                    // 对插入的行调用CheckAndFormatKeywords
                    int endLine = doc->cursorLine;
                    for (int i = insertStartLine; i <= endLine && i < (int)doc->lines.size(); i++) {
                        CheckAndFormatKeywords(doc, i);
                    }
                }

                GlobalUnlock(hData);
                UpdateScrollBars(hWnd);
            }
        }
        CloseClipboard();
    }
}

void CutToClipboard(HWND hWnd, EditorDocument* doc) {
    CopyToClipboard(hWnd, doc);
    DeleteSelection(doc);
    UpdateScrollBars(hWnd);
}

void UpdateScrollBars(HWND hWnd) {
    EditorData* data = (EditorData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    if (!data) return;
    EditorDocument* doc = data->GetActiveDoc();
    if (!doc) return;

    RECT rect;
    GetClientRect(hWnd, &rect);
    int clientHeight = rect.bottom - data->tabHeight;
    int clientWidth = rect.right - 50;
    
    int scrollbarWidth = data->scrollbarWidth;

    // 计算垂直滚动条
    int totalLines = (int)doc->lines.size();
    if (!doc->lines.empty() && doc->lines[0].find(L"版本\t") == 0) {
        totalLines--;
    }
    int totalHeight = totalLines * data->rowHeight + 100;
    
    // 垂直滚动条区域（右侧）
    data->vScrollbarRect.left = rect.right - scrollbarWidth;
    data->vScrollbarRect.top = data->tabHeight;
    data->vScrollbarRect.right = rect.right;
    data->vScrollbarRect.bottom = rect.bottom - scrollbarWidth;  // 减去水平滚动条高度
    
    // 计算垂直滚动条滑块
    if (totalHeight > clientHeight) {
        int trackHeight = data->vScrollbarRect.bottom - data->vScrollbarRect.top;
        int thumbHeight = std::max(20, (int)((float)clientHeight / totalHeight * trackHeight));
        int maxScroll = totalHeight - clientHeight;
        int thumbTop = data->vScrollbarRect.top + (int)((float)doc->scrollY / maxScroll * (trackHeight - thumbHeight));
        
        data->vScrollThumbRect.left = data->vScrollbarRect.left;
        data->vScrollThumbRect.top = thumbTop;
        data->vScrollThumbRect.right = data->vScrollbarRect.right;
        data->vScrollThumbRect.bottom = thumbTop + thumbHeight;
    } else {
        data->vScrollThumbRect = {0, 0, 0, 0};
    }

    // 计算水平滚动条（使用缓存避免每次都遍历所有行）
    int maxWidth = 0;
    if (doc->maxLineWidthDirty) {
        HDC hdc = GetDC(hWnd);
        HFONT hFont = CreateFont(data->fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                               GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                               DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Consolas");
        HGDIOBJ hOldFont = SelectObject(hdc, hFont);
        
        for (const auto& line : doc->lines) {
            SIZE size;
            GetTextExtentPoint32W(hdc, line.c_str(), (int)line.length(), &size);
            if (size.cx > maxWidth) maxWidth = size.cx;
        }
        
        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);
        ReleaseDC(hWnd, hdc);
        
        maxWidth += 100;
        doc->cachedMaxLineWidth = maxWidth;
        doc->maxLineWidthDirty = false;
    } else {
        maxWidth = doc->cachedMaxLineWidth;
    }
    
    // 水平滚动条区域（底部）
    data->hScrollbarRect.left = 50;
    data->hScrollbarRect.top = rect.bottom - scrollbarWidth;
    data->hScrollbarRect.right = rect.right - scrollbarWidth;  // 减去垂直滚动条宽度
    data->hScrollbarRect.bottom = rect.bottom;
    
    // 计算水平滚动条滑块
    if (maxWidth > clientWidth) {
        int trackWidth = data->hScrollbarRect.right - data->hScrollbarRect.left;
        int thumbWidth = std::max(20, (int)((float)clientWidth / maxWidth * trackWidth));
        int maxScroll = maxWidth - clientWidth;
        int thumbLeft = data->hScrollbarRect.left + (int)((float)doc->scrollX / maxScroll * (trackWidth - thumbWidth));
        
        data->hScrollThumbRect.left = thumbLeft;
        data->hScrollThumbRect.top = data->hScrollbarRect.top;
        data->hScrollThumbRect.right = thumbLeft + thumbWidth;
        data->hScrollThumbRect.bottom = data->hScrollbarRect.bottom;
    } else {
        data->hScrollThumbRect = {0, 0, 0, 0};
    }
}

ATOM RegisterYiEditorClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc    = YiEditorWndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = nullptr;
    wcex.hCursor        = NULL;  // 不设置默认光标，由WM_SETCURSOR处理
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = szYiEditorClass;
    wcex.hIconSm        = nullptr;

    return RegisterClassExW(&wcex);
}

// YiEditor 窗口过程
LRESULT CALLBACK YiEditorWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_NCHITTEST:
        {
            // 当AI窗口隐藏时，让左边缘8px的鼠标消息穿透到主窗口，以便调整窗口大小
            if (!g_LeftPanelVisible) {
                POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
                ScreenToClient(hWnd, &pt);
                if (pt.x < 8) {
                    return HTTRANSPARENT;
                }
            }
            return HTCLIENT;
        }
        
    case WM_CREATE:
        // 初始化编辑器数据
        g_EditorData = new EditorData();
        g_EditorData->hWnd = hWnd;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)g_EditorData);
        
        // 初始化关键词管理器
        KeywordManager::GetInstance().Initialize();
        
        // 加载支持库命令
        {
            wchar_t exePath[MAX_PATH];
            GetModuleFileName(NULL, exePath, MAX_PATH);
            std::wstring exeDir(exePath);
            size_t lastSlash = exeDir.find_last_of(L"\\");
            if (lastSlash != std::wstring::npos) {
                exeDir = exeDir.substr(0, lastSlash);
            }
            
            // 从 exe 运行目录的 lib 文件夹加载核心支持库
            std::wstring libFilePath = exeDir + L"\\lib\\krnln.fne";
            
            // 调试：输出路径和加载结果
            CreateDirectoryW(L"logs", NULL);
            std::wofstream debugFile(L"logs\\yieditor_init_debug.txt");
            debugFile.imbue(std::locale(""));
            debugFile << L"exe路径: " << exePath << std::endl;
            debugFile << L"exe目录: " << exeDir << std::endl;
            debugFile << L"FNE路径: " << libFilePath << std::endl;
            debugFile << L"文件是否存在: " << (GetFileAttributesW(libFilePath.c_str()) != INVALID_FILE_ATTRIBUTES ? L"是" : L"否") << std::endl;
            
            bool loadResult = LibraryParser::GetInstance().LoadFneLibrary(libFilePath);
            debugFile << L"加载结果: " << (loadResult ? L"成功" : L"失败") << std::endl;
            if (!loadResult) {
                debugFile << L"错误信息: " << LibraryParser::GetInstance().GetLastError() << std::endl;
            }
            debugFile.close();
        }
        
        // 欢迎页模式，不创建文本光标
        // SetFocus(hWnd);  // 移除，避免触发WM_SETFOCUS创建光标
        return 0;
        
    case WM_LBUTTONDOWN:
        {
            EditorData* data = (EditorData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (!data) return 0;
            
            // 获取鼠标位置
            int mouseX = LOWORD(lParam);
            int mouseY = HIWORD(lParam);
            
            // 如果正在显示欢迎页，检测点击了哪个链接
            if (data->showWelcomePage) {
                RECT rect;
                GetClientRect(hWnd, &rect);
                int centerY = rect.bottom / 2 - 80;
                int linkY = centerY + 120;
                
                // 计算链接的点击区域（居中显示，宽度300像素）
                int linkLeft = rect.right / 2 - 150;
                int linkRight = rect.right / 2 + 150;
                
                // 检测点击了哪个链接
                if (mouseX >= linkLeft && mouseX <= linkRight) {
                    extern HWND hMainWnd;
                    if (mouseY >= linkY && mouseY < linkY + 25) {
                        // 点击了"新建文件"
                        data->AddDocument(L"");
                        InvalidateRect(hWnd, NULL, TRUE);
                        return 0;
                    } else if (mouseY >= linkY + 30 && mouseY < linkY + 55) {
                        // 点击了"打开文件" - 发送消息给主窗口
                        PostMessage(hMainWnd, WM_COMMAND, IDM_OPEN, 0);
                        return 0;
                    } else if (mouseY >= linkY + 60 && mouseY < linkY + 85) {
                        // 点击了"打开文件夹" - 发送消息给主窗口
                        PostMessage(hMainWnd, WM_COMMAND, IDM_OPEN_FOLDER, 0);
                        return 0;
                    }
                }
                return 0;
            }
            
            POINT pt = {mouseX, mouseY};
            
            // 检查是否点击了类型补全窗口
            if (data->showTypeCompletion && PtInRect(&data->typeCompletionRect, pt)) {
                EditorDocument* doc = data->GetActiveDoc();
                if (doc) {
                    // 检查是否点击了滚动条区域
                    bool needScrollbar = (int)data->typeCompletionItems.size() > TYPE_COMPLETION_MAX_VISIBLE;
                    int scrollbarWidth = needScrollbar ? 12 : 0;
                    int scrollbarLeft = data->typeCompletionRect.right - scrollbarWidth;
                    
                    if (needScrollbar && mouseX >= scrollbarLeft) {
                        // 点击了滚动条区域 - 简单处理，跳转到该位置
                        int relativeY = mouseY - data->typeCompletionRect.top;
                        int popupHeight = data->typeCompletionRect.bottom - data->typeCompletionRect.top;
                        int maxScroll = std::max(0, (int)data->typeCompletionItems.size() - TYPE_COMPLETION_MAX_VISIBLE);
                        
                        if (maxScroll > 0) {
                            float scrollRatio = (float)relativeY / popupHeight;
                            data->typeCompletionScrollOffset = (int)(scrollRatio * maxScroll);
                            if (data->typeCompletionScrollOffset < 0) data->typeCompletionScrollOffset = 0;
                            if (data->typeCompletionScrollOffset > maxScroll) data->typeCompletionScrollOffset = maxScroll;
                            InvalidateRect(hWnd, NULL, TRUE);
                        }
                        return 0;
                    }
                    
                    // 计算点击了哪一项
                    int itemY = mouseY - data->typeCompletionRect.top - 2;
                    int clickedIndex = data->typeCompletionScrollOffset + (itemY / TYPE_COMPLETION_ITEM_HEIGHT);
                    
                    if (clickedIndex >= 0 && clickedIndex < (int)data->typeCompletionItems.size()) {
                        data->typeCompletionSelectedIndex = clickedIndex;
                        ApplyTypeCompletion(hWnd, data, doc);
                    }
                }
                return 0;
            }
            
            // 点击其他区域时隐藏类型补全窗口
            if (data->showTypeCompletion) {
                HideTypeCompletion(data);
                InvalidateRect(hWnd, NULL, TRUE);
            }
            
            // 检查是否点击了自动完成弹窗
            if (data->showCompletion && PtInRect(&data->completionRect, pt)) {
                EditorDocument* doc = data->GetActiveDoc();
                if (doc) {
                    // 检查是否点击了滚动条区域
                    bool needScrollbar = data->completionItems.size() > (size_t)data->completionMaxVisible;
                    int scrollbarWidth = needScrollbar ? 12 : 0;
                    int scrollbarLeft = data->completionRect.right - scrollbarWidth;
                    
                    if (needScrollbar && mouseX >= scrollbarLeft) {
                        // 点击了滚动条区域
                        int relativeY = mouseY - data->completionRect.top;
                        int popupHeight = data->completionRect.bottom - data->completionRect.top;
                        int maxScroll = std::max(0, (int)data->completionItems.size() - data->completionMaxVisible);
                        
                        if (maxScroll > 0) {
                            // 计算滚动块的位置和大小
                            float thumbRatio = (float)data->completionMaxVisible / data->completionItems.size();
                            int thumbHeight = std::max(20, (int)(popupHeight * thumbRatio));
                            float scrollRatio = (float)data->completionScrollOffset / maxScroll;
                            int thumbY = (int)((popupHeight - thumbHeight) * scrollRatio);
                            
                            // 检查是否点击了滚动块
                            if (relativeY >= thumbY && relativeY < thumbY + thumbHeight) {
                                // 点击了滚动块，开始拖动
                                data->isDraggingCompletionScroll = true;
                                data->completionDragStartY = mouseY;
                                data->completionDragStartOffset = data->completionScrollOffset;
                                SetCapture(hWnd);
                            } else {
                                // 点击了滚动轨道，跳转到该位置
                                float scrollRatio = (float)relativeY / popupHeight;
                                data->completionScrollOffset = (int)(scrollRatio * maxScroll);
                                if (data->completionScrollOffset < 0) data->completionScrollOffset = 0;
                                if (data->completionScrollOffset > maxScroll) data->completionScrollOffset = maxScroll;
                                
                                // 同时更新选中项，使其在可见范围内
                                if (data->selectedCompletionIndex < data->completionScrollOffset) {
                                    data->selectedCompletionIndex = data->completionScrollOffset;
                                } else if (data->selectedCompletionIndex >= data->completionScrollOffset + data->completionMaxVisible) {
                                    data->selectedCompletionIndex = data->completionScrollOffset + data->completionMaxVisible - 1;
                                }
                                
                                InvalidateRect(hWnd, NULL, TRUE);
                            }
                        }
                        return 0;
                    }
                    
                    // 计算点击了哪一项（排除滚动条区域）
                    int itemY = mouseY - data->completionRect.top;
                    int clickedIndex = data->completionScrollOffset + (itemY / data->completionItemHeight);
                    
                    if (clickedIndex >= 0 && clickedIndex < (int)data->completionItems.size()) {
                        data->selectedCompletionIndex = clickedIndex;
                        ApplyCompletion(hWnd, data, doc);
                    }
                }
                return 0;
            }
            
            // 点击其他区域时隐藏自动完成弹窗
            if (data->showCompletion) {
                data->showCompletion = false;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            
            // 检查是否点击了流程控制命令的展开/折叠符号（位于行号右边）
            EditorDocument* doc = data->GetActiveDoc();
            if (doc && mouseX >= 56 && mouseX <= 70) {  // 展开符号的X范围（与绘制位置一致：58到68）
                int rowHeight = data->rowHeight;
                int startY = 5 + data->tabHeight - doc->scrollY;  // 与绘制代码一致
                
                bool hasVersion = (!doc->lines.empty() && doc->lines[0].find(L"版本\t") == 0);
                size_t startLine = hasVersion ? 1 : 0;
                
                int currentY = startY;
                bool lastWasAssemblySection = false;
                int lastTableType = 0;  // 0:无, 1:程序集, 2:子程序, 3:参数, 4:变量
                bool insideAssembly = false;
                bool inParamTable = false;
                bool inClassVarTable = false;
                
                for (size_t i = startLine; i < doc->lines.size(); i++) {
                    const std::wstring& line = doc->lines[i];
                    
                    // 检查当前行是什么类型的表头（与绘制代码一致）
                    bool isSubProgramHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"子程序名") == 0);
                    bool isParamHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"参数名") == 0);
                    bool isVarHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"变量名") == 0);
                    
                    // 在程序集区域结束后、子程序表开始前添加间距
                    if (isSubProgramHeader && lastWasAssemblySection) {
                        currentY += rowHeight / 2;
                        lastWasAssemblySection = false;
                    }
                    
                    // 在参数表和变量表之间添加间距
                    if (isVarHeader && lastTableType == 3) {
                        currentY += rowHeight / 2;
                    }
                    
                    // 在子程序表（无参数时）和变量表之间添加间距
                    if (isVarHeader && lastTableType == 2) {
                        currentY += rowHeight / 2;
                    }
                    
                    // 空行处理
                    if (line.empty()) {
                        inParamTable = false;
                        inClassVarTable = false;
                    }
                    
                    if (mouseY >= currentY && mouseY < currentY + rowHeight) {
                        // 检查这一行是否是有参数的命令（流程控制或普通命令）
                        bool isCommandLine = IsFlowControlLine(line);
                        if (!isCommandLine && line.find(L'(') != std::wstring::npos) {
                            // 可能是普通命令，检查是否有参数
                            isCommandLine = true;
                        }
                        
                        if (isCommandLine) {
                            // 使用 ExtractCommandName 提取命令名（会正确处理空格和点号）
                            std::wstring cmdName = ExtractCommandName(line);
                            
                            // 获取命令参数
                            std::vector<LibraryParameter> params = GetCommandParameters(cmdName);
                            
                            if (!params.empty()) {
                                // 切换展开/折叠状态
                                while (doc->parametersExpanded.size() <= i) {
                                    doc->parametersExpanded.push_back(false);
                                }
                                doc->parametersExpanded[i] = !doc->parametersExpanded[i];
                                
                                if (doc->parametersExpanded[i]) {
                                    // 展开：在命令行下方插入参数行
                                    // 首先提取括号内的参数值
                                    std::vector<std::wstring> argValues;
                                    size_t bracketStart = line.find(L'(');
                                    size_t bracketEnd = line.rfind(L')');
                                    if (bracketStart != std::wstring::npos && bracketEnd != std::wstring::npos && bracketEnd > bracketStart) {
                                        std::wstring argsStr = line.substr(bracketStart + 1, bracketEnd - bracketStart - 1);
                                        // 按逗号分隔参数，但要注意括号嵌套
                                        int parenDepth = 0;
                                        size_t argStart = 0;
                                        for (size_t j = 0; j <= argsStr.length(); j++) {
                                            if (j == argsStr.length() || (argsStr[j] == L',' && parenDepth == 0)) {
                                                std::wstring arg = argsStr.substr(argStart, j - argStart);
                                                // 去除前后空格
                                                while (!arg.empty() && (arg.front() == L' ' || arg.front() == L'\t')) arg.erase(0, 1);
                                                while (!arg.empty() && (arg.back() == L' ' || arg.back() == L'\t')) arg.pop_back();
                                                argValues.push_back(arg);
                                                argStart = j + 1;
                                            } else if (argsStr[j] == L'(') {
                                                parenDepth++;
                                            } else if (argsStr[j] == L')') {
                                                parenDepth--;
                                            }
                                        }
                                    }
                                    
                                    for (size_t p = 0; p < params.size(); p++) {
                                        // 参数行格式：\u2060 + 参数名 + : + 参数值（存储半角，渲染时全角显示）
                                        std::wstring paramValue = (p < argValues.size()) ? argValues[p] : L"";
                                        std::wstring paramLine = L"\u2060" + params[p].name + L":" + paramValue;
                                        doc->lines.insert(doc->lines.begin() + i + p + 1, paramLine);
                                        doc->parametersExpanded.insert(doc->parametersExpanded.begin() + i + p + 1, false);
                                    }
                                } else {
                                    // 折叠：删除所有后续参数行（包括嵌套的子参数行）
                                    while (i + 1 < doc->lines.size() && doc->lines[i + 1].length() > 0 && doc->lines[i + 1][0] == L'\u2060') {
                                        doc->lines.erase(doc->lines.begin() + i + 1);
                                        if (i + 1 < doc->parametersExpanded.size()) {
                                            doc->parametersExpanded.erase(doc->parametersExpanded.begin() + i + 1);
                                        }
                                    }
                                }
                                
                                doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                                InvalidateRect(hWnd, NULL, TRUE);
                                return 0;
                            }
                        }
                        
                        // 检查是否是赋值语句（对所有赋值语句生效）
                        if (!isCommandLine && IsAssignmentLine(line)) {
                            while (doc->parametersExpanded.size() <= i) {
                                doc->parametersExpanded.push_back(false);
                            }
                            doc->parametersExpanded[i] = !doc->parametersExpanded[i];
                            
                            if (doc->parametersExpanded[i]) {
                                // 展开：插入两行（被赋值的变量、用于赋予的值）
                                std::wstring lhs, rhs;
                                IsAssignmentLine(line, &lhs, &rhs);
                                
                                std::wstring line1 = L"\u2060\u88AB\u8D4B\u503C\u7684\u53D8\u91CF\u6216\u53D8\u91CF\u6570\u7EC4:" + lhs;
                                std::wstring line2 = L"\u2060\u7528\u4F5C\u8D4B\u4E88\u7684\u503C\u6216\u8D44\u6E90:" + rhs;
                                doc->lines.insert(doc->lines.begin() + i + 1, line1);
                                doc->parametersExpanded.insert(doc->parametersExpanded.begin() + i + 1, false);
                                doc->lines.insert(doc->lines.begin() + i + 2, line2);
                                doc->parametersExpanded.insert(doc->parametersExpanded.begin() + i + 2, false);
                            } else {
                                // 折叠：删除所有后续参数行（包括嵌套的子参数行）
                                while (i + 1 < doc->lines.size() && !doc->lines[i + 1].empty() && doc->lines[i + 1][0] == L'\u2060') {
                                    doc->lines.erase(doc->lines.begin() + i + 1);
                                    if (i + 1 < doc->parametersExpanded.size()) {
                                        doc->parametersExpanded.erase(doc->parametersExpanded.begin() + i + 1);
                                    }
                                }
                            }
                            
                            doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                            InvalidateRect(hWnd, NULL, TRUE);
                            return 0;
                        }
                        
                        // 检查是否是参数行且值包含运算表达式（支持嵌套展开）
                        if (line.length() > 0 && line[0] == L'\u2060') {
                            // 提取参数值
                            size_t colonPos = line.find(L':');
                            if (colonPos != std::wstring::npos) {
                                std::wstring paramValue = line.substr(colonPos + 1);
                                // 去除前后空格
                                while (!paramValue.empty() && (paramValue.front() == L' ' || paramValue.front() == L'\t')) paramValue.erase(0, 1);
                                while (!paramValue.empty() && (paramValue.back() == L' ' || paramValue.back() == L'\t')) paramValue.pop_back();
                                
                                ExprOperatorInfo opInfo;
                                if (ParseExpressionOperator(paramValue, opInfo)) {
                                    while (doc->parametersExpanded.size() <= i) {
                                        doc->parametersExpanded.push_back(false);
                                    }
                                    doc->parametersExpanded[i] = !doc->parametersExpanded[i];
                                    
                                    if (doc->parametersExpanded[i]) {
                                        // 展开：插入两行运算参数行（用双\u2060标记嵌套层级）
                                        std::wstring subLine1 = L"\u2060\u2060" + opInfo.param1Name + L":" + opInfo.leftOperand;
                                        std::wstring subLine2 = L"\u2060\u2060" + opInfo.param2Name + L":" + opInfo.rightOperand;
                                        doc->lines.insert(doc->lines.begin() + i + 1, subLine1);
                                        doc->parametersExpanded.insert(doc->parametersExpanded.begin() + i + 1, false);
                                        doc->lines.insert(doc->lines.begin() + i + 2, subLine2);
                                        doc->parametersExpanded.insert(doc->parametersExpanded.begin() + i + 2, false);
                                    } else {
                                        // 折叠：删除所有后续嵌套参数行
                                        while (i + 1 < doc->lines.size() && !doc->lines[i + 1].empty() && doc->lines[i + 1][0] == L'\u2060') {
                                            doc->lines.erase(doc->lines.begin() + i + 1);
                                            if (i + 1 < doc->parametersExpanded.size()) {
                                                doc->parametersExpanded.erase(doc->parametersExpanded.begin() + i + 1);
                                            }
                                        }
                                    }
                                    
                                    doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                                    InvalidateRect(hWnd, NULL, TRUE);
                                    return 0;
                                }
                            }
                        }
                        break;
                    }
                    
                    // 更新表格类型状态（与绘制代码一致）
                    if (line.find(L"\t") != std::wstring::npos) {
                        std::vector<std::wstring> checkCells;
                        size_t start = 0, pos = 0;
                        while ((pos = line.find(L'\t', start)) != std::wstring::npos) {
                            checkCells.push_back(line.substr(start, pos - start));
                            start = pos + 1;
                        }
                        checkCells.push_back(line.substr(start));
                        
                        if (checkCells.size() > 0) {
                            if (checkCells[0] == L"程序集名") {
                                lastWasAssemblySection = true;
                                lastTableType = 1;
                                insideAssembly = true;
                            } else if (checkCells[0] == L"变量名" && insideAssembly) {
                                lastWasAssemblySection = true;
                                lastTableType = 1;
                                inClassVarTable = true;
                            } else if (checkCells[0] == L"子程序名") {
                                lastWasAssemblySection = false;
                                lastTableType = 2;
                                insideAssembly = false;
                            } else if (checkCells[0] == L"参数名") {
                                lastTableType = 3;
                                inParamTable = true;
                            } else if (checkCells[0] == L"变量名" && !insideAssembly) {
                                lastTableType = 4;
                            } else if (inParamTable) {
                                lastTableType = 3;
                            }
                        }
                        if (inClassVarTable) {
                            lastWasAssemblySection = true;
                        }
                    } else {
                        if (!inClassVarTable && !insideAssembly) {
                            lastWasAssemblySection = false;
                        }
                    }
                    
                    currentY += rowHeight;
                }
            }
            
            // 检查是否点击了滚动条滑块
            if (PtInRect(&data->vScrollThumbRect, pt)) {
                EditorDocument* doc = data->GetActiveDoc();
                if (doc) {
                    data->isDraggingVScroll = true;
                    data->dragStartY = mouseY;
                    data->dragStartScrollY = doc->scrollY;
                    SetCapture(hWnd);
                }
                return 0;
            }
            if (PtInRect(&data->hScrollThumbRect, pt)) {
                EditorDocument* doc = data->GetActiveDoc();
                if (doc) {
                    data->isDraggingHScroll = true;
                    data->dragStartX = mouseX;
                    data->dragStartScrollX = doc->scrollX;
                    SetCapture(hWnd);
                }
                return 0;
            }
            
            // 检查是否点击了滚动条轨道（滑块之外的区域）
            if (PtInRect(&data->vScrollbarRect, pt) && !PtInRect(&data->vScrollThumbRect, pt)) {
                // 点击垂直滚动条轨道，跳转到该位置
                EditorDocument* doc = data->GetActiveDoc();
                if (doc) {
                    int trackHeight = data->vScrollbarRect.bottom - data->vScrollbarRect.top;
                    int thumbHeight = data->vScrollThumbRect.bottom - data->vScrollThumbRect.top;
                    int clickY = mouseY - data->vScrollbarRect.top;
                    RECT rect;
                    GetClientRect(hWnd, &rect);
                    int clientHeight = rect.bottom - data->tabHeight;
                    int totalLines = (int)doc->lines.size();
                    if (!doc->lines.empty() && doc->lines[0].find(L"版本\t") == 0) totalLines--;
                    int totalHeight = totalLines * data->rowHeight + 100;
                    int maxScroll = totalHeight - clientHeight;
                    if (maxScroll > 0) {
                        doc->scrollY = (int)((float)clickY / trackHeight * maxScroll);
                        if (doc->scrollY < 0) doc->scrollY = 0;
                        if (doc->scrollY > maxScroll) doc->scrollY = maxScroll;
                        UpdateScrollBars(hWnd);
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                }
                return 0;
            }
            if (PtInRect(&data->hScrollbarRect, pt) && !PtInRect(&data->hScrollThumbRect, pt)) {
                // 点击水平滚动条轨道
                EditorDocument* doc = data->GetActiveDoc();
                if (doc) {
                    int trackWidth = data->hScrollbarRect.right - data->hScrollbarRect.left;
                    int clickX = mouseX - data->hScrollbarRect.left;
                    // 类似垂直滚动条的处理
                    RECT rect;
                    GetClientRect(hWnd, &rect);
                    int clientWidth = rect.right - 50;
                    // 计算maxWidth（简化版本）
                    int maxWidth = 2000;  // 暂时使用固定值
                    int maxScroll = maxWidth - clientWidth;
                    if (maxScroll > 0) {
                        doc->scrollX = (int)((float)clickX / trackWidth * maxScroll);
                        if (doc->scrollX < 0) doc->scrollX = 0;
                        if (doc->scrollX > maxScroll) doc->scrollX = maxScroll;
                        UpdateScrollBars(hWnd);
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                }
                return 0;
            }

            // doc 已在上面定义，不需要重复定义
            if (!doc) return 0;
            
            // 标记用户已经有过交互（但不立即创建光标，等SetFocus触发WM_SETFOCUS时创建）
            if (!doc->hasUserInteraction) {
                doc->hasUserInteraction = true;
            }

            // 检查是否点击在行号区域（X < 75）
            if (mouseX < 75 && mouseY >= data->tabHeight) {
                // 计算点击的行号
                int adjustedY = mouseY - data->tabHeight + doc->scrollY;
                int rowHeight = data->rowHeight;
                int startY = 5;
                
                bool hasVersion = (!doc->lines.empty() && doc->lines[0].find(L"版本\t") == 0);
                size_t startLine = hasVersion ? 1 : 0;
                
                int currentY = startY;
                int clickedLine = -1;
                bool insideAssembly = false;
                bool lastWasAssemblySection = false;
                int lastTableType = 0;
                bool inClassVarTable = false;
                
                // 逐行计算Y坐标，找到点击的行
                for (size_t i = startLine; i < doc->lines.size(); i++) {
                    const std::wstring& line = doc->lines[i];
                    
                    bool isSubProgramHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"子程序名") == 0);
                    bool isParamHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"参数名") == 0);
                    bool isVarHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"变量名") == 0);
                    
                    if (isSubProgramHeader && lastWasAssemblySection) {
                        currentY += rowHeight / 2;
                        lastWasAssemblySection = false;
                    }
                    
                    if (isVarHeader && lastTableType == 3) {
                        currentY += rowHeight / 2;
                    }
                    
                    if (isVarHeader && lastTableType == 2) {
                        currentY += rowHeight / 2;
                    }
                    
                    if (line.empty()) {
                        inClassVarTable = false;
                    }
                    
                    if (adjustedY >= currentY && adjustedY < currentY + rowHeight) {
                        clickedLine = (int)i;
                        break;
                    }
                    
                    if (line.find(L"\t") != std::wstring::npos) {
                        std::vector<std::wstring> checkCells;
                        size_t start = 0, pos = 0;
                        while ((pos = line.find(L'\t', start)) != std::wstring::npos) {
                            checkCells.push_back(line.substr(start, pos - start));
                            start = pos + 1;
                        }
                        checkCells.push_back(line.substr(start));
                        
                        if (checkCells.size() > 0) {
                            if (checkCells[0] == L"程序集名") {
                                lastWasAssemblySection = true;
                                lastTableType = 1;
                                insideAssembly = true;
                            } else if (checkCells[0] == L"变量名" && insideAssembly) {
                                lastWasAssemblySection = true;
                                lastTableType = 1;
                                inClassVarTable = true;
                            } else if (checkCells[0] == L"子程序名") {
                                lastWasAssemblySection = false;
                                lastTableType = 2;
                                insideAssembly = false;
                            } else if (checkCells[0] == L"参数名") {
                                lastTableType = 3;
                            } else if (checkCells[0] == L"变量名" && !insideAssembly) {
                                lastTableType = 4;
                            }
                        }
                        
                        if (inClassVarTable) {
                            lastWasAssemblySection = true;
                        }
                    } else {
                        if (!inClassVarTable && !insideAssembly) {
                            lastWasAssemblySection = false;
                        }
                    }
                    
                    currentY += rowHeight;
                }
                
                // 如果找到了有效行，选中整行
                if (clickedLine >= 0 && clickedLine < (int)doc->lines.size()) {
                    doc->selStartLine = clickedLine;
                    doc->selStartCol = 0;
                    doc->selEndLine = clickedLine;
                    doc->selEndCol = (int)doc->lines[clickedLine].length();
                    doc->hasSelection = true;
                    doc->isSelecting = true;  // 允许拖动继续选择
                    doc->cursorLine = clickedLine;
                    doc->cursorCol = 0;
                    
                    // 标记这是行号区域的选择，且已经是拖动选择
                    data->isLineNumberSelection = true;
                    data->isDraggingSelection = true;
                    
                    // 捕获鼠标以便拖动
                    SetCapture(hWnd);
                    SetFocus(hWnd);
                    InvalidateRect(hWnd, NULL, TRUE);
                }
                
                return 0;
            }

            // 内部标签页点击处理已移除，使用外部TabBar组件

            if (!doc) return 0;

            // 调整编辑器内容的 mouseY
            mouseY -= data->tabHeight;
            mouseY += doc->scrollY;
            
            // 计算点击的行和列 - 使用EditorData中的配置
            int rowHeight = data->rowHeight;
            int fontSize = data->fontSize;
            int startX = 75 - doc->scrollX;  // 与WM_PAINT保持一致
            int startY = 5;
            
            // 计算行号 - 需要考虑表格间距
            int clickedLine = -1;
            bool clickedInBlankArea = false;  // 标记是否点击了空白区域
            bool hasVersion = (!doc->lines.empty() && doc->lines[0].find(L"版本\t") == 0);
            size_t startLine = hasVersion ? 1 : 0;
            
            // 逐行计算Y坐标，找到鼠标所在行（完全匹配绘制逻辑）
            int currentY = startY;
            bool insideAssembly = false;
            bool lastWasAssemblySection = false;
            int lastTableType = 0; // 0:无, 1:程序集, 2:子程序, 3:参数, 4:变量
            bool inClassVarTable = false;
            
            for (size_t i = startLine; i < doc->lines.size(); i++) {
                const std::wstring& line = doc->lines[i];
                
                // 检查当前行是什么类型的表头（完全匹配绘制逻辑）
                bool isSubProgramHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"子程序名") == 0);
                bool isParamHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"参数名") == 0);
                bool isVarHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"变量名") == 0);
                
                // 在程序集区域结束后、子程序表开始前添加间距
                if (isSubProgramHeader && lastWasAssemblySection) {
                    currentY += rowHeight / 2;
                    lastWasAssemblySection = false;
                }
                
                // 在参数表和变量表之间添加间距
                if (isVarHeader && lastTableType == 3) {
                    currentY += rowHeight / 2;
                }
                
                // 在子程序表（无参数时）和变量表之间添加间距
                if (isVarHeader && lastTableType == 2) {
                    currentY += rowHeight / 2;
                }
                
                // 空行标记表格结束
                if (line.empty()) {
                    inClassVarTable = false;
                }
                
                // 检查鼠标是否在当前行范围内
                if (mouseY >= currentY && mouseY < currentY + rowHeight) {
                    clickedLine = (int)i;
                    break;
                }
                
                // 更新状态标记（匹配绘制逻辑）
                if (line.find(L"\t") != std::wstring::npos) {
                    std::vector<std::wstring> checkCells;
                    size_t start = 0, pos = 0;
                    while ((pos = line.find(L'\t', start)) != std::wstring::npos) {
                        checkCells.push_back(line.substr(start, pos - start));
                        start = pos + 1;
                    }
                    checkCells.push_back(line.substr(start));
                    
                    if (checkCells.size() > 0) {
                        if (checkCells[0] == L"程序集名") {
                            lastWasAssemblySection = true;
                            lastTableType = 1;
                            insideAssembly = true;
                        } else if (checkCells[0] == L"变量名" && insideAssembly) {
                            lastWasAssemblySection = true;
                            lastTableType = 1;
                            inClassVarTable = true;
                        } else if (checkCells[0] == L"子程序名") {
                            lastWasAssemblySection = false;
                            lastTableType = 2;
                            insideAssembly = false;
                        } else if (checkCells[0] == L"参数名") {
                            lastTableType = 3;
                        } else if (checkCells[0] == L"变量名" && !insideAssembly) {
                            lastTableType = 4;
                        }
                    }
                    
                    if (inClassVarTable) {
                        lastWasAssemblySection = true;
                    }
                } else {
                    if (!inClassVarTable && !insideAssembly) {
                        lastWasAssemblySection = false;
                    }
                }
                
                currentY += rowHeight;
            }
            
            // 如果没找到匹配行，说明点击在所有行下方的空白区域
            if (clickedLine < 0) {
                clickedInBlankArea = true;
                // 将光标定位到最后一行
                if (doc->lines.empty()) {
                    clickedLine = 0;
                } else {
                    clickedLine = (int)doc->lines.size() - 1;
                }
            }

            // 程序集表头和版本行不可点击，其他表头也不可点击
            if (clickedLine >= 0 && clickedLine < (int)doc->lines.size()) {
                const std::wstring& l = doc->lines[clickedLine];
                if (l.find(L"程序集名") == 0 || 
                    l.find(L"版本\t") == 0 ||
                    l.find(L"子程序名") == 0 ||
                    l.find(L"参数名") == 0 ||
                    l.find(L"变量名") == 0) {
                    return 0;
                }
            }
            
            // 精确计算列号（使用字体度量）
            HDC hdc = GetDC(hWnd);
            HFONT hFont = CreateFont(fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                   GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                   DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Consolas");
            HGDIOBJ hOldFont = SelectObject(hdc, hFont);
            
            // 计算tableMaxWidths（与WM_PAINT中的计算逻辑保持一致）
            std::vector<std::vector<int>> tableMaxWidths;
            std::vector<int> tableTypes;
            int preCalcTableIndex = -1;
            bool preCalcInsideAssembly = false;
            
            for (size_t i = startLine; i < doc->lines.size(); i++) {
                const std::wstring& calcLine = doc->lines[i];
                if (calcLine.empty()) {
                    preCalcInsideAssembly = false;
                    continue;
                }
                
                if (calcLine.find(L"\t") != std::wstring::npos) {
                    std::vector<std::wstring> calcCells;
                    size_t cStart = 0, cPos = 0;
                    while ((cPos = calcLine.find(L'\t', cStart)) != std::wstring::npos) {
                        calcCells.push_back(calcLine.substr(cStart, cPos - cStart));
                        cStart = cPos + 1;
                    }
                    calcCells.push_back(calcLine.substr(cStart));
                    
                    bool isCalcHeader = (calcCells.size() > 0 && 
                                       (calcCells[0] == L"程序集名" || calcCells[0] == L"子程序名" || 
                                        calcCells[0] == L"参数名" || calcCells[0] == L"变量名"));
                    bool isCalcParam = (calcCells.size() > 0 && calcCells[0] == L"参数名");
                    bool isCalcAssemblyVar = (calcCells.size() > 0 && calcCells[0] == L"变量名" && preCalcInsideAssembly);
                    
                    if (isCalcHeader && !isCalcParam && !isCalcAssemblyVar) {
                        preCalcTableIndex++;
                        tableMaxWidths.push_back(std::vector<int>());
                        tableTypes.push_back(0);
                        
                        if (calcCells[0] == L"子程序名") {
                            tableMaxWidths[preCalcTableIndex] = {150, 100, 30, 320};
                        } else if (calcCells[0] == L"变量名") {
                            tableMaxWidths[preCalcTableIndex] = {100, 80, 30, 30, 200};
                        } else if (calcCells[0] == L"程序集名") {
                            tableMaxWidths[preCalcTableIndex] = {150, 100, 100, 200};
                            preCalcInsideAssembly = true;
                        }
                    }
                    
                    if (preCalcTableIndex >= 0 && preCalcTableIndex < (int)tableMaxWidths.size()) {
                        for (size_t j = 0; j < calcCells.size(); j++) {
                            if (j >= tableMaxWidths[preCalcTableIndex].size()) {
                                tableMaxWidths[preCalcTableIndex].push_back(100);
                            }
                            if (!calcCells[j].empty()) {
                                SIZE textSize;
                                GetTextExtentPoint32W(hdc, calcCells[j].c_str(), (int)calcCells[j].length(), &textSize);
                                int minWidth = textSize.cx + 10;
                                if (minWidth > tableMaxWidths[preCalcTableIndex][j]) {
                                    tableMaxWidths[preCalcTableIndex][j] = minWidth;
                                }
                            }
                        }
                    }
                }
            }
            
            int clickedCol = 0;
            const std::wstring& line = doc->lines[clickedLine];
            
            // 检查是否为表格行
            if (line.find(L"\t") != std::wstring::npos) {
                int relativeX = mouseX - startX;
                // 表格行：根据鼠标X坐标判断点击在哪个单元格
                std::vector<std::wstring> cells;
                size_t start = 0, pos = 0;
                while ((pos = line.find(L'\t', start)) != std::wstring::npos) {
                    cells.push_back(line.substr(start, pos - start));
                    start = pos + 1;
                }
                cells.push_back(line.substr(start));
                
                // 计算当前行所属的表格索引（与绘制逻辑一致）
                int clickTableIndex = -1;
                bool clickInsideAssembly = false;
                bool clickInParamTable = false;
                
                for (size_t i = startLine; i <= (size_t)clickedLine; i++) {
                    const std::wstring& checkLine = doc->lines[i];
                    if (checkLine.empty()) {
                        continue;
                    }
                    
                    if (checkLine.find(L"\t") != std::wstring::npos) {
                        std::vector<std::wstring> checkCells;
                        size_t cStart = 0, cPos = 0;
                        while ((cPos = checkLine.find(L'\t', cStart)) != std::wstring::npos) {
                            checkCells.push_back(checkLine.substr(cStart, cPos - cStart));
                            cStart = cPos + 1;
                        }
                        checkCells.push_back(checkLine.substr(cStart));
                        
                        bool isHeader = (checkCells.size() > 0 && 
                                       (checkCells[0] == L"程序集名" || checkCells[0] == L"子程序名" || 
                                        checkCells[0] == L"参数名" || checkCells[0] == L"变量名"));
                        bool isParamHeader = (checkCells.size() > 0 && checkCells[0] == L"参数名");
                        bool isAssemblyVarHeader = (checkCells.size() > 0 && checkCells[0] == L"变量名" && clickInsideAssembly);
                        
                        if (isHeader && !isParamHeader && !isAssemblyVarHeader) {
                            clickTableIndex++;
                            if (checkCells[0] == L"程序集名") {
                                clickInsideAssembly = true;
                            } else {
                                clickInsideAssembly = false;
                            }
                            clickInParamTable = false;
                        } else if (isParamHeader) {
                            clickInParamTable = true;
                        }
                    }
                }
                
                // 获取列宽（使用预先计算的tableMaxWidths）
                std::vector<int> colWidths;
                bool isHeader = (cells.size() > 0 && 
                               (cells[0] == L"程序集名" || cells[0] == L"子程序名" || 
                                cells[0] == L"参数名" || cells[0] == L"变量名"));
                bool isParamTable = (cells.size() > 0 && cells[0] == L"参数名") || clickInParamTable;
                
                // 使用预先计算好的列宽
                if (clickTableIndex >= 0 && clickTableIndex < (int)tableMaxWidths.size()) {
                    colWidths = tableMaxWidths[clickTableIndex];
                } else {
                    // 如果没有预计算数据，使用默认值
                    for (size_t i = 0; i < cells.size(); i++) {
                        colWidths.push_back(100);
                    }
                }
                
                // 根据鼠标X坐标判断点击在哪个单元格
                int cellX = 0;
                int cellIndex = 0;
                bool foundCell = false;
                
                for (size_t i = 0; i < cells.size() && i < colWidths.size(); i++) {
                    int currentColWidth = colWidths[i];
                    
                    // 参数表的列3需要分割为可空+数组+备注
                    if (isParamTable && i == 3 && cells.size() >= 6) {
                        int nullableWidth = 30;  // 可空列
                        int arrayWidth = 30;     // 数组列
                        int remarkWidth = currentColWidth - nullableWidth - arrayWidth;  // 备注列
                        
                        // 检查是否点击在可空列
                        if (relativeX >= cellX && relativeX < cellX + nullableWidth) {
                            cellIndex = 3;  // 可空列（第4个单元格）
                            foundCell = true;
                            break;
                        } else if (relativeX >= cellX + nullableWidth && relativeX < cellX + nullableWidth + arrayWidth) {
                            cellIndex = 4;  // 数组列（第5个单元格）
                            cellX += nullableWidth;
                            foundCell = true;
                            break;
                        } else if (relativeX >= cellX + nullableWidth + arrayWidth && relativeX < cellX + currentColWidth) {
                            cellIndex = 5;  // 备注列（第6个单元格）
                            cellX += nullableWidth + arrayWidth;
                            foundCell = true;
                            break;
                        }
                        cellX += currentColWidth;
                        continue;
                    }
                    
                    if (relativeX >= cellX && relativeX < cellX + currentColWidth) {
                        cellIndex = (int)i;
                        foundCell = true;
                        break;
                    }
                    cellX += currentColWidth;
                }
                
                // 参数表数据行点击复选框列（参考、可空、数组）时切换值
                if (isParamTable && !isHeader && cells.size() >= 6 && (cellIndex == 2 || cellIndex == 3 || cellIndex == 4)) {
                    std::wstring& currentLine = doc->lines[clickedLine];
                    std::vector<std::wstring> editCells;
                    size_t eStart = 0, ePos = 0;
                    while ((ePos = currentLine.find(L'\t', eStart)) != std::wstring::npos) {
                        editCells.push_back(currentLine.substr(eStart, ePos - eStart));
                        eStart = ePos + 1;
                    }
                    editCells.push_back(currentLine.substr(eStart));
                    
                    if (cellIndex < (int)editCells.size()) {
                        // 切换值：有值则清空，无值则设为"√"
                        if (editCells[cellIndex].empty()) {
                            editCells[cellIndex] = L"√";
                        } else {
                            editCells[cellIndex] = L"";
                        }
                        
                        // 重建行内容
                        std::wstring newLine;
                        for (size_t ec = 0; ec < editCells.size(); ec++) {
                            if (ec > 0) newLine += L"\t";
                            newLine += editCells[ec];
                        }
                        doc->lines[clickedLine] = newLine;
                        doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                    
                    SelectObject(hdc, hOldFont);
                    DeleteObject(hFont);
                    ReleaseDC(hWnd, hdc);
                    return 0;
                }
                
                // 子程序表数据行点击公开列（第2列）时切换值
                bool isSubProgramData = false;
                if (!isHeader && cells.size() >= 4 && cellIndex == 2) {
                    // 向上查找表头判断是否为子程序表
                    for (int searchLine = clickedLine - 1; searchLine >= 0; searchLine--) {
                        const std::wstring& headerLine = doc->lines[searchLine];
                        if (headerLine.empty()) continue;
                        if (headerLine.find(L"子程序名") == 0) {
                            isSubProgramData = true;
                            break;
                        }
                        if (headerLine.find(L"参数名") == 0 || headerLine.find(L"变量名") == 0 || headerLine.find(L"程序集名") == 0) {
                            break;
                        }
                    }
                }
                
                if (isSubProgramData && cellIndex == 2) {
                    std::wstring& currentLine = doc->lines[clickedLine];
                    std::vector<std::wstring> editCells;
                    size_t eStart = 0, ePos = 0;
                    while ((ePos = currentLine.find(L'\t', eStart)) != std::wstring::npos) {
                        editCells.push_back(currentLine.substr(eStart, ePos - eStart));
                        eStart = ePos + 1;
                    }
                    editCells.push_back(currentLine.substr(eStart));
                    
                    if (cellIndex < (int)editCells.size()) {
                        if (editCells[cellIndex].empty()) {
                            editCells[cellIndex] = L"√";
                        } else {
                            editCells[cellIndex] = L"";
                        }
                        
                        std::wstring newLine;
                        for (size_t ec = 0; ec < editCells.size(); ec++) {
                            if (ec > 0) newLine += L"\t";
                            newLine += editCells[ec];
                        }
                        doc->lines[clickedLine] = newLine;
                        doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                    
                    SelectObject(hdc, hOldFont);
                    DeleteObject(hFont);
                    ReleaseDC(hWnd, hdc);
                    return 0;
                }
                
                // 局部变量表数据行点击静态/数组列（第2、3列）时切换值
                bool isLocalVarData = false;
                if (!isHeader && cells.size() >= 5 && (cellIndex == 2 || cellIndex == 3)) {
                    for (int searchLine = clickedLine - 1; searchLine >= 0; searchLine--) {
                        const std::wstring& headerLine = doc->lines[searchLine];
                        if (headerLine.empty()) continue;
                        if (headerLine.find(L"变量名") == 0 && headerLine.find(L"静态") != std::wstring::npos) {
                            isLocalVarData = true;
                            break;
                        }
                        if (headerLine.find(L"子程序名") == 0 || headerLine.find(L"程序集名") == 0 || headerLine.find(L"参数名") == 0) {
                            break;
                        }
                    }
                }
                
                if (isLocalVarData && (cellIndex == 2 || cellIndex == 3)) {
                    std::wstring& currentLine = doc->lines[clickedLine];
                    std::vector<std::wstring> editCells;
                    size_t eStart = 0, ePos = 0;
                    while ((ePos = currentLine.find(L'\t', eStart)) != std::wstring::npos) {
                        editCells.push_back(currentLine.substr(eStart, ePos - eStart));
                        eStart = ePos + 1;
                    }
                    editCells.push_back(currentLine.substr(eStart));
                    
                    if (cellIndex < (int)editCells.size()) {
                        if (editCells[cellIndex].empty()) {
                            editCells[cellIndex] = L"√";
                        } else {
                            editCells[cellIndex] = L"";
                        }
                        
                        std::wstring newLine;
                        for (size_t ec = 0; ec < editCells.size(); ec++) {
                            if (ec > 0) newLine += L"\t";
                            newLine += editCells[ec];
                        }
                        doc->lines[clickedLine] = newLine;
                        doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                    
                    SelectObject(hdc, hOldFont);
                    DeleteObject(hFont);
                    ReleaseDC(hWnd, hdc);
                    return 0;
                }
                
                // 如果没有找到单元格（点击在最后一列右侧），使用最后一列
                if (!foundCell && cells.size() > 0) {
                    cellIndex = (int)cells.size() - 1;
                    // 重新计算cellX为最后一列的起始位置
                    cellX = 0;
                    
                    // 对于参数表，需要特殊处理列3的拆分
                    if (isParamTable && cells.size() >= 6) {
                        // 累加列0, 1, 2的宽度
                        for (int i = 0; i < 3 && i < (int)colWidths.size(); i++) {
                            cellX += colWidths[i];
                        }
                        // 如果cellIndex是5（备注列），需要累加可空(30)和数组(30)的宽度
                        if (cellIndex == 5) {
                            cellX += 30 + 30;  // 可空列 + 数组列
                        } else if (cellIndex == 4) {
                            cellX += 30;  // 可空列
                        }
                        // cellIndex == 3时，cellX已经正确（就是列0+1+2的总宽度）
                    } else {
                        // 非参数表，正常累加
                        for (int i = 0; i < cellIndex && i < (int)colWidths.size(); i++) {
                            cellX += colWidths[i];
                        }
                    }
                }
                
                // 计算在单元格内的位置
                int inCellX = relativeX - cellX;
                if (inCellX < 0) inCellX = 0;
                
                // 计算光标在单元格内的字符位置
                int inCellCol = 0;
                if (cellIndex < (int)cells.size()) {
                    std::wstring cellText = cells[cellIndex];
                    SIZE textSize;
                    bool found = false;
                    for (size_t i = 0; i < cellText.length(); i++) {
                        std::wstring substr = cellText.substr(0, i + 1);
                        GetTextExtentPoint32W(hdc, substr.c_str(), (int)substr.length(), &textSize);
                        if (textSize.cx + 3 > inCellX) {  // 3是单元格内边距
                            SIZE prevSize;
                            if (i > 0) {
                                std::wstring prevSubstr = cellText.substr(0, i);
                                GetTextExtentPoint32W(hdc, prevSubstr.c_str(), (int)prevSubstr.length(), &prevSize);
                                if (inCellX - (prevSize.cx + 3) < (textSize.cx + 3) - inCellX) {
                                    inCellCol = (int)i;
                                } else {
                                    inCellCol = (int)i + 1;
                                }
                            } else {
                                inCellCol = ((textSize.cx + 3) / 2 > inCellX) ? 0 : 1;
                            }
                            found = true;
                            break;
                        }
                    }
                    // 如果点击位置超过文本宽度，光标应该在文本末尾
                    if (!found) {
                        inCellCol = (int)cellText.length();
                    }
                }
                
                // 计算在整行中的字符位置
                clickedCol = 0;
                for (int i = 0; i < cellIndex && i < (int)cells.size(); i++) {
                    // 参数表的可空(3)、数组(4)、备注(5)列对应实际数据的列3、4、5
                    // 不需要特殊处理，因为cells已经包含了这些列
                    clickedCol += (int)cells[i].length() + 1;  // +1 为制表符
                }
                clickedCol += inCellCol;
            } else {
                // 非表格行：使用原有逻辑，但需要考虑缩进
                
                // 检查是否是条件达成分支或否则分支行（使用特殊字符标记）
                bool isIndented = (line.length() >= 1 && (line[0] == L'\u200C' || line[0] == L'\u200D'));
                
                if (isIndented) {
                    // 流程控制内的缩进行，文本从 startX + 20 开始
                    int indentChars = 1;  // 跳过标记字符
                    
                    // 如果点击在流程线区域（startX 到 startX+20），光标移动到缩进后
                    if (mouseX < startX + 20) {
                        clickedCol = indentChars;
                    } else if (indentChars < (int)line.length()) {
                        // 点击在文本区域，计算精确位置
                        int textStartX = startX + 20;
                        int relativeX = mouseX - textStartX;
                        std::wstring textPart = line.substr(indentChars);
                        
                        if (relativeX > 0) {
                            SIZE textSize;
                            clickedCol = (int)indentChars;
                            for (size_t i = 0; i < textPart.length(); i++) {
                                std::wstring substr = textPart.substr(0, i + 1);
                                GetTextExtentPoint32W(hdc, substr.c_str(), (int)substr.length(), &textSize);
                                if (textSize.cx > relativeX) {
                                    // 检查是靠近左边还是右边
                                    SIZE prevSize;
                                    if (i > 0) {
                                        std::wstring prevSubstr = textPart.substr(0, i);
                                        GetTextExtentPoint32W(hdc, prevSubstr.c_str(), (int)prevSubstr.length(), &prevSize);
                                        if (relativeX - prevSize.cx < textSize.cx - relativeX) {
                                            clickedCol = (int)indentChars + (int)i;
                                        } else {
                                            clickedCol = (int)indentChars + (int)i + 1;
                                        }
                                    } else {
                                        clickedCol = (textSize.cx / 2 > relativeX) ? (int)indentChars : (int)indentChars + 1;
                                    }
                                    break;
                                }
                                clickedCol = (int)indentChars + (int)i + 1;
                            }
                        } else {
                            // 点击在文本起始位置左侧
                            clickedCol = indentChars;
                        }
                    } else {
                        // 只有缩进没有文本
                        clickedCol = indentChars;
                    }
                } else if (line.empty()) {
                    // 空行
                    clickedCol = 0;
                } else {
                    int textStartX = startX;
                    size_t indentChars = 0;  // 缩进字符数量
                    
                    // 检查是否是参数行（使用\u2060标记）
                    bool isParamLine = (line.length() > 0 && line[0] == L'\u2060');
                    
                    // 计算实际文本的起始位置
                    if (IsFlowControlLine(line)) {
                        textStartX = startX + 20;
                    } else if (isParamLine) {
                        // 参数行：格式为 \u2060参数名:参数值（嵌套行有多个\u2060前缀）
                        // 计算嵌套层级
                        int pNestLevel = 0;
                        while (pNestLevel < (int)line.length() && line[pNestLevel] == L'\u2060') pNestLevel++;
                        
                        // 查找冒号位置，确定参数名和参数值的分界
                        size_t colonPos = line.find(L':');
                        if (colonPos == std::wstring::npos) {
                            colonPos = line.length();  // 如果没有冒号，假设在末尾
                        }
                        
                        // 参数行：计算动态的显示位置（与渲染逻辑一致）
                        int paramTextX = startX + 20;
                        // 查找命令行（可能需要向上查找多行，跳过其他参数行）
                        int cmdLineIndex = clickedLine - 1;
                        while (cmdLineIndex >= 0 && doc->lines[cmdLineIndex].length() > 0 && doc->lines[cmdLineIndex][0] == L'\u2060') {
                            cmdLineIndex--;
                        }
                        if (cmdLineIndex >= 0) {
                            const std::wstring& cmdLine = doc->lines[cmdLineIndex];
                            size_t leftBracket = cmdLine.find(L'(');
                            if (leftBracket != std::wstring::npos) {
                                // 计算括号X坐标
                                std::wstring cmdText = cmdLine;
                                // 去掉嵌套标记字符
                                size_t cmdMLen = 0;
                                int cmdNDepth = GetNestingDepth(cmdText, &cmdMLen);
                                int cmdBaseX = startX;
                                if (cmdNDepth > 0) {
                                    cmdText = cmdText.substr(cmdMLen);
                                    leftBracket = cmdText.find(L'(');
                                    if (leftBracket == std::wstring::npos) leftBracket = 0;
                                    cmdBaseX = startX + cmdNDepth * 20;
                                }
                                size_t dotPos = cmdText.find(L'.');
                                if (dotPos != std::wstring::npos) {
                                    cmdText.erase(dotPos, 1);
                                    if (leftBracket > 0) leftBracket--;
                                }
                                SIZE textSize;
                                GetTextExtentPoint32W(hdc, cmdText.c_str(), (int)leftBracket, &textSize);
                                int bracketX = cmdBaseX + 20 + textSize.cx;
                                
                                // 计算※符号位置
                                SIZE prefixSize;
                                GetTextExtentPoint32W(hdc, L"      ", 6, &prefixSize);
                                int minArrowX = bracketX + 20;
                                int defaultArrowX = cmdBaseX + 20 + prefixSize.cx;
                                int arrowX = (std::max)(minArrowX, defaultArrowX);
                                
                                paramTextX = arrowX - prefixSize.cx;
                            }
                        }
                        
                        // 嵌套参数行：增加额外偏移量（与渲染一致）
                        if (pNestLevel >= 2) {
                            SIZE nestOffset;
                            GetTextExtentPoint32W(hdc, L"中文中文", 4, &nestOffset);
                            paramTextX += (pNestLevel - 1) * nestOffset.cx;
                        }
                        
                        // 绘制时整个displayText从paramTextX开始，所以光标计算也要从这里开始
                        // 但光标逻辑上的位置应该是在 line 中的位置（colonPos + 1）
                        // 需要计算 "      ※参数名：" 的显示宽度来确定参数值的起始X坐标
                        std::wstring paramName = line.substr(pNestLevel, colonPos - pNestLevel);  // 跳过所有前导\u2060标记，到冒号之前
                        std::wstring displayPrefix = L"      ※" + paramName + L"：";
                        SIZE prefixDisplaySize;
                        GetTextExtentPoint32W(hdc, displayPrefix.c_str(), (int)displayPrefix.length(), &prefixDisplaySize);
                        
                        // 参数值的实际绘制起始位置
                        textStartX = paramTextX + prefixDisplaySize.cx;
                        
                        // 光标只能在冒号之后（colonPos + 1），不能编辑参数名部分
                        indentChars = colonPos + 1;  // 跳过 "\u2060参数名:"，光标从冒号后开始
                    } else {
                        // 计算缩进（与光标绘制逻辑保持一致，处理制表符）
                        int indent = 0;
                        while (indentChars < line.length() && line[indentChars] == L'\t') {
                            indent += 20;
                            indentChars++;
                        }
                        textStartX = startX + indent;
                    }
                    
                    int relativeX = mouseX - textStartX;  // 相对于实际文本起始位置
                    
                    if (relativeX > 0 && line.length() > indentChars) {
                        // 只对实际文本部分（去除缩进）计算位置
                        std::wstring textPart = line.substr(indentChars);
                        SIZE textSize;
                        bool found = false;
                        for (size_t i = 0; i < textPart.length(); i++) {
                            std::wstring substr = textPart.substr(0, i + 1);
                            GetTextExtentPoint32W(hdc, substr.c_str(), (int)substr.length(), &textSize);
                            if (textSize.cx > relativeX) {
                                // 计算当前字符的左边界和右边界
                                SIZE prevSize = {0, 0};
                                if (i > 0) {
                                    std::wstring prevSubstr = textPart.substr(0, i);
                                    GetTextExtentPoint32W(hdc, prevSubstr.c_str(), (int)prevSubstr.length(), &prevSize);
                                }
                                // 计算点击位置距离左边界和右边界的距离
                                int distToLeft = relativeX - prevSize.cx;
                                int distToRight = textSize.cx - relativeX;
                                // 选择距离更近的边界
                                if (distToLeft < distToRight) {
                                    clickedCol = (int)indentChars + (int)i;
                                } else {
                                    clickedCol = (int)indentChars + (int)i + 1;
                                }
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            // 点击在文本末尾右侧
                            clickedCol = (int)indentChars + (int)textPart.length();
                        }
                    } else if (relativeX <= 0) {
                        // 点击在文本起始位置左侧，光标放在缩进后的位置
                        clickedCol = (int)indentChars;
                    } else {
                        // 点击在空行或末尾
                        clickedCol = (int)line.length();
                    }
                }
            }
            
            if (clickedCol > (int)line.length()) {
                clickedCol = (int)line.length();
            }
            
            // 如果点击的是空白区域，将光标设置到该行末尾
            if (clickedInBlankArea) {
                clickedCol = (int)line.length();
            }
            
            SelectObject(hdc, hOldFont);
            DeleteObject(hFont);
            ReleaseDC(hWnd, hdc);
            
            // 离开当前行时，检查并格式化关键词
            int oldLine = doc->cursorLine;
            
            doc->cursorLine = clickedLine;
            doc->cursorCol = clickedCol;
            
            // 如果切换到了不同的行，格式化旧行
            if (oldLine != clickedLine) {
                CheckAndFormatKeywords(doc, oldLine);
                InvalidateRect(hWnd, NULL, FALSE);
            }
            
            // 清除旧的选中状态，重新开始
            doc->isSelecting = true;
            doc->hasSelection = false;
            doc->selStartLine = clickedLine;
            doc->selStartCol = clickedCol;
            doc->selEndLine = clickedLine;
            doc->selEndCol = clickedCol;
            
            // 记录鼠标按下位置，用于拖动检测
            data->mouseDownX = LOWORD(lParam);
            data->mouseDownY = HIWORD(lParam);
            data->isDraggingSelection = false;
            data->isLineNumberSelection = false;  // 清除行号选择模式
            
            // 捕获鼠标，以便在窗口外部也能接收鼠标移动消息
            SetCapture(hWnd);
            
            // 获得焦点
            SetFocus(hWnd);
            
            // 如果是第一次交互，确保创建光标（因为窗口可能已经有焦点，SetFocus不会触发WM_SETFOCUS）
            if (doc->hasUserInteraction && !data->showWelcomePage) {
                int caretHeight = data->fontSize + 4;
                DestroyCaret();  // 先销毁可能存在的光标
                CreateCaret(hWnd, NULL, 1, caretHeight);
                ShowCaret(hWnd);
            }
            
            // 更新命令提示信息
            if (clickedLine >= 0 && clickedLine < (int)doc->lines.size()) {
                UpdateCommandHint(doc->lines[clickedLine], clickedLine);
            }
            
            InvalidateRect(hWnd, NULL, TRUE);
        }
        return 0;
        
    case WM_MOUSEMOVE:
        {
            EditorData* data = (EditorData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (!data) return 0;
            
            // 获取鼠标位置
            int mouseX = LOWORD(lParam);
            int mouseY = HIWORD(lParam);
            
            // 如果正在显示欢迎页，处理鼠标光标
            if (data->showWelcomePage) {
                RECT rect;
                GetClientRect(hWnd, &rect);
                int centerY = rect.bottom / 2 - 80;
                int linkY = centerY + 120;
                
                // 检测是否在三个文字区域（只根据Y坐标判断，与点击逻辑一致）
                if ((mouseY >= linkY && mouseY < linkY + 25) ||
                    (mouseY >= linkY + 30 && mouseY < linkY + 55) ||
                    (mouseY >= linkY + 60 && mouseY < linkY + 85)) {
                    SetCursor(LoadCursor(NULL, IDC_HAND));
                } else {
                    SetCursor(LoadCursor(NULL, IDC_ARROW));
                }
                return 0;
            }
            
            // 处理滚动条拖动
            if (data->isDraggingVScroll) {
                EditorDocument* doc = data->GetActiveDoc();
                if (doc) {
                    int deltaY = mouseY - data->dragStartY;
                    int trackHeight = data->vScrollbarRect.bottom - data->vScrollbarRect.top;
                    int thumbHeight = data->vScrollThumbRect.bottom - data->vScrollThumbRect.top;
                    
                    RECT rect;
                    GetClientRect(hWnd, &rect);
                    int clientHeight = rect.bottom - data->tabHeight;
                    int totalLines = (int)doc->lines.size();
                    if (!doc->lines.empty() && doc->lines[0].find(L"版本\t") == 0) totalLines--;
                    int totalHeight = totalLines * data->rowHeight + 100;
                    int maxScroll = totalHeight - clientHeight;
                    
                    if (maxScroll > 0 && trackHeight > thumbHeight) {
                        float scrollRatio = (float)deltaY / (trackHeight - thumbHeight);
                        doc->scrollY = data->dragStartScrollY + (int)(scrollRatio * maxScroll);
                        if (doc->scrollY < 0) doc->scrollY = 0;
                        if (doc->scrollY > maxScroll) doc->scrollY = maxScroll;
                        UpdateScrollBars(hWnd);
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                }
                return 0;
            }
            
            // 处理补全窗口滚动条拖动
            if (data->isDraggingCompletionScroll) {
                int deltaY = mouseY - data->completionDragStartY;
                int popupHeight = data->completionRect.bottom - data->completionRect.top;
                int maxScroll = std::max(0, (int)data->completionItems.size() - data->completionMaxVisible);
                
                if (maxScroll > 0) {
                    float thumbRatio = (float)data->completionMaxVisible / data->completionItems.size();
                    int thumbHeight = std::max(20, (int)(popupHeight * thumbRatio));
                    int trackHeight = popupHeight - thumbHeight;
                    
                    if (trackHeight > 0) {
                        float scrollRatio = (float)deltaY / trackHeight;
                        data->completionScrollOffset = data->completionDragStartOffset + (int)(scrollRatio * maxScroll);
                        if (data->completionScrollOffset < 0) data->completionScrollOffset = 0;
                        if (data->completionScrollOffset > maxScroll) data->completionScrollOffset = maxScroll;
                        
                        // 同时更新选中项
                        if (data->selectedCompletionIndex < data->completionScrollOffset) {
                            data->selectedCompletionIndex = data->completionScrollOffset;
                        } else if (data->selectedCompletionIndex >= data->completionScrollOffset + data->completionMaxVisible) {
                            data->selectedCompletionIndex = data->completionScrollOffset + data->completionMaxVisible - 1;
                        }
                        
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                }
                return 0;
            }
            
            if (data->isDraggingHScroll) {
                EditorDocument* doc = data->GetActiveDoc();
                if (doc) {
                    int deltaX = mouseX - data->dragStartX;
                    int trackWidth = data->hScrollbarRect.right - data->hScrollbarRect.left;
                    int thumbWidth = data->hScrollThumbRect.right - data->hScrollThumbRect.left;
                    
                    // 简化处理
                    RECT rect;
                    GetClientRect(hWnd, &rect);
                    int clientWidth = rect.right - 50;
                    int maxWidth = 2000;
                    int maxScroll = maxWidth - clientWidth;
                    
                    if (maxScroll > 0 && trackWidth > thumbWidth) {
                        float scrollRatio = (float)deltaX / (trackWidth - thumbWidth);
                        doc->scrollX = data->dragStartScrollX + (int)(scrollRatio * maxScroll);
                        if (doc->scrollX < 0) doc->scrollX = 0;
                        if (doc->scrollX > maxScroll) doc->scrollX = maxScroll;
                        UpdateScrollBars(hWnd);
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                }
                return 0;
            }
            
            // 检测滚动条悬停
            POINT pt = {mouseX, mouseY};
            bool wasVScrollHover = data->vScrollHover;
            bool wasHScrollHover = data->hScrollHover;
            
            data->vScrollHover = PtInRect(&data->vScrollThumbRect, pt);
            data->hScrollHover = PtInRect(&data->hScrollThumbRect, pt);
            
            if (wasVScrollHover != data->vScrollHover || wasHScrollHover != data->hScrollHover) {
                InvalidateRect(hWnd, NULL, FALSE);
            }
            
            // 内部标签栏悬停处理已移除，使用外部TabBar组件
            
            EditorDocument* doc = data->GetActiveDoc();
            if (!doc || !doc->isSelecting) return 0;
            
            // 如果是行号选择模式，使用特殊的整行选择逻辑
            if (data->isLineNumberSelection) {
                // 计算鼠标当前所在的行
                int adjustedY = mouseY - data->tabHeight + doc->scrollY;
                int rowHeight = data->rowHeight;
                int startY = 5;
                
                bool hasVersion = (!doc->lines.empty() && doc->lines[0].find(L"版本\t") == 0);
                size_t startLine = hasVersion ? 1 : 0;
                
                int currentY = startY;
                int currentLine = -1;
                bool insideAssembly = false;
                bool lastWasAssemblySection = false;
                int lastTableType = 0;
                bool inClassVarTable = false;
                
                // 逐行计算Y坐标，找到鼠标所在行
                for (size_t i = startLine; i < doc->lines.size(); i++) {
                    const std::wstring& line = doc->lines[i];
                    
                    bool isSubProgramHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"子程序名") == 0);
                    bool isParamHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"参数名") == 0);
                    bool isVarHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"变量名") == 0);
                    
                    if (isSubProgramHeader && lastWasAssemblySection) {
                        currentY += rowHeight / 2;
                        lastWasAssemblySection = false;
                    }
                    
                    if (isVarHeader && lastTableType == 3) {
                        currentY += rowHeight / 2;
                    }
                    
                    if (isVarHeader && lastTableType == 2) {
                        currentY += rowHeight / 2;
                    }
                    
                    if (line.empty()) {
                        inClassVarTable = false;
                    }
                    
                    if (adjustedY >= currentY && adjustedY < currentY + rowHeight) {
                        currentLine = (int)i;
                        break;
                    }
                    
                    if (line.find(L"\t") != std::wstring::npos) {
                        std::vector<std::wstring> checkCells;
                        size_t start = 0, pos = 0;
                        while ((pos = line.find(L'\t', start)) != std::wstring::npos) {
                            checkCells.push_back(line.substr(start, pos - start));
                            start = pos + 1;
                        }
                        checkCells.push_back(line.substr(start));
                        
                        if (checkCells.size() > 0) {
                            if (checkCells[0] == L"程序集名") {
                                lastWasAssemblySection = true;
                                lastTableType = 1;
                                insideAssembly = true;
                            } else if (checkCells[0] == L"变量名" && insideAssembly) {
                                lastWasAssemblySection = true;
                                lastTableType = 1;
                                inClassVarTable = true;
                            } else if (checkCells[0] == L"子程序名") {
                                lastWasAssemblySection = false;
                                lastTableType = 2;
                                insideAssembly = false;
                            } else if (checkCells[0] == L"参数名") {
                                lastTableType = 3;
                            } else if (checkCells[0] == L"变量名" && !insideAssembly) {
                                lastTableType = 4;
                            }
                        }
                        
                        if (inClassVarTable) {
                            lastWasAssemblySection = true;
                        }
                    } else {
                        if (!inClassVarTable && !insideAssembly) {
                            lastWasAssemblySection = false;
                        }
                    }
                    
                    currentY += rowHeight;
                }
                
                // 更新选中范围（整行模式）
                if (currentLine >= 0 && currentLine < (int)doc->lines.size()) {
                    // 确定选中的起始和结束行
                    int startLineNum = doc->selStartLine;
                    int endLineNum = currentLine;
                    
                    if (startLineNum <= endLineNum) {
                        // 向下拖动
                        doc->selStartCol = 0;
                        doc->selEndLine = endLineNum;
                        doc->selEndCol = (int)doc->lines[endLineNum].length();
                    } else {
                        // 向上拖动
                        doc->selEndLine = endLineNum;
                        doc->selEndCol = 0;
                        doc->selStartCol = (int)doc->lines[startLineNum].length();
                    }
                    
                    doc->hasSelection = true;
                    InvalidateRect(hWnd, NULL, FALSE);
                }
                
                return 0;
            }
            
            // 检测是否开始拖动选择（鼠标移动超过阈值）
            if (!data->isDraggingSelection) {
                int deltaX = mouseX - data->mouseDownX;
                int deltaY = mouseY - data->mouseDownY;
                int distance = (int)sqrt(deltaX * deltaX + deltaY * deltaY);
                
                // 如果移动距离小于3像素，不认为是拖动
                if (distance < 3) {
                    return 0;
                }
                
                // 开始拖动选择
                data->isDraggingSelection = true;
            }
            
            mouseY -= data->tabHeight;
            mouseY += doc->scrollY;
            
            // 计算当前位置 - 使用与WM_LBUTTONDOWN相同的精确逻辑
            int rowHeight = data->rowHeight;
            int fontSize = data->fontSize;
            int startX = 75 - doc->scrollX;  // 与WM_PAINT保持一致
            int startY = 5;
            
            // 计算行号 - 需要考虑表格间距
            int currentLine = -1;
            bool hasVersion = (!doc->lines.empty() && doc->lines[0].find(L"版本\t") == 0);
            size_t startLine = hasVersion ? 1 : 0;
            
            // 逐行计算Y坐标，找到鼠标所在行（完全匹配绘制逻辑）
            int currentY = startY;
            bool insideAssembly = false;
            bool lastWasAssemblySection = false;
            int lastTableType = 0;
            bool inClassVarTable = false;
            
            for (size_t i = startLine; i < doc->lines.size(); i++) {
                const std::wstring& line = doc->lines[i];
                
                bool isSubProgramHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"子程序名") == 0);
                bool isParamHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"参数名") == 0);
                bool isVarHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"变量名") == 0);
                
                if (isSubProgramHeader && lastWasAssemblySection) {
                    currentY += rowHeight / 2;
                    lastWasAssemblySection = false;
                }
                
                if (isVarHeader && lastTableType == 3) {
                    currentY += rowHeight / 2;
                }
                
                if (isVarHeader && lastTableType == 2) {
                    currentY += rowHeight / 2;
                }
                
                if (line.empty()) {
                    inClassVarTable = false;
                }
                
                if (mouseY >= currentY && mouseY < currentY + rowHeight) {
                    currentLine = (int)i;
                    break;
                }
                
                if (line.find(L"\t") != std::wstring::npos) {
                    std::vector<std::wstring> checkCells;
                    size_t start = 0, pos = 0;
                    while ((pos = line.find(L'\t', start)) != std::wstring::npos) {
                        checkCells.push_back(line.substr(start, pos - start));
                        start = pos + 1;
                    }
                    checkCells.push_back(line.substr(start));
                    
                    if (checkCells.size() > 0) {
                        if (checkCells[0] == L"程序集名") {
                            lastWasAssemblySection = true;
                            lastTableType = 1;
                            insideAssembly = true;
                        } else if (checkCells[0] == L"变量名" && insideAssembly) {
                            lastWasAssemblySection = true;
                            lastTableType = 1;
                            inClassVarTable = true;
                        } else if (checkCells[0] == L"子程序名") {
                            lastWasAssemblySection = false;
                            lastTableType = 2;
                            insideAssembly = false;
                        } else if (checkCells[0] == L"参数名") {
                            lastTableType = 3;
                        } else if (checkCells[0] == L"变量名" && !insideAssembly) {
                            lastTableType = 4;
                        }
                    }
                    
                    if (inClassVarTable) {
                        lastWasAssemblySection = true;
                    }
                } else {
                    if (!inClassVarTable && !insideAssembly) {
                        lastWasAssemblySection = false;
                    }
                }
                
                currentY += rowHeight;
            }
            
            if (currentLine < 0) {
                currentLine = (int)startLine;
            }
            
            // 精确计算列位置
            HDC hdc = GetDC(hWnd);
            HFONT hFont = CreateFont(fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                   GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                   DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Consolas");
            HGDIOBJ hOldFont = SelectObject(hdc, hFont);
            
            const std::wstring& line = doc->lines[currentLine];
            int currentCol = 0;
            
            // 计算实际文本的起始位置（考虑缩进和特殊格式）
            int textStartX = startX;
            size_t indentChars = 0;
            
            if (IsFlowControlLine(line)) {
                textStartX = startX + 20;
            } else if (line.find(L"\t") == std::wstring::npos) {
                // 非表格行，计算缩进
                int indent = 0;
                while (indentChars < line.length() && (line[indentChars] == L' ' || line[indentChars] == L'\t')) {
                    if (line[indentChars] == L'\t') indent += 20;
                    else indent += 4;
                    indentChars++;
                }
                textStartX = startX + indent;
            }
            
            int relativeX = mouseX - textStartX;  // 相对于实际文本起始位置
            
            if (relativeX > 0 && line.length() > indentChars) {
                std::wstring textPart = line.substr(indentChars);
                SIZE textSize;
                for (size_t i = 0; i < textPart.length(); i++) {
                    std::wstring substr = textPart.substr(0, i + 1);
                    GetTextExtentPoint32W(hdc, substr.c_str(), (int)substr.length(), &textSize);
                    if (textSize.cx > relativeX) {
                        // 检查是靠近左边还是右边
                        SIZE prevSize;
                        if (i > 0) {
                            std::wstring prevSubstr = textPart.substr(0, i);
                            GetTextExtentPoint32W(hdc, prevSubstr.c_str(), (int)prevSubstr.length(), &prevSize);
                            if (relativeX - prevSize.cx < textSize.cx - relativeX) {
                                currentCol = (int)indentChars + (int)i;
                            } else {
                                currentCol = (int)indentChars + (int)i + 1;
                            }
                        } else {
                            currentCol = (textSize.cx / 2 > relativeX) ? (int)indentChars : (int)indentChars + 1;
                        }
                        break;
                    }
                    currentCol = (int)indentChars + (int)i + 1;
                }
            } else if (relativeX <= 0) {
                currentCol = (int)indentChars;
            } else {
                currentCol = (int)line.length();
            }
            
            if (currentCol > (int)line.length()) {
                currentCol = (int)line.length();
            }
            
            SelectObject(hdc, hOldFont);
            DeleteObject(hFont);
            ReleaseDC(hWnd, hdc);
            
            // 更新选中结束位置
            doc->selEndLine = currentLine;
            doc->selEndCol = currentCol;
            
            // 如果起始和结束位置不同，则有选中
            if (doc->selStartLine != doc->selEndLine || doc->selStartCol != doc->selEndCol) {
                doc->hasSelection = true;
            }
            
            InvalidateRect(hWnd, NULL, FALSE);  // 不擦除背景，减少闪烁
        }
        return 0;
        
    case WM_LBUTTONUP:
        {
            EditorData* data = (EditorData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (!data) return 0;
            
            // 停止滚动条拖动
            if (data->isDraggingVScroll || data->isDraggingHScroll || data->isDraggingCompletionScroll) {
                data->isDraggingVScroll = false;
                data->isDraggingHScroll = false;
                data->isDraggingCompletionScroll = false;
                ReleaseCapture();
                return 0;
            }
            
            EditorDocument* doc = data->GetActiveDoc();
            if (!doc) return 0;
            
            // 结束选中
            doc->isSelecting = false;
            
            // 如果没有真正拖动，清除选择状态
            if (!data->isDraggingSelection) {
                doc->hasSelection = false;
            }
            
            // 清除拖动状态
            data->isDraggingSelection = false;
            data->isLineNumberSelection = false;  // 清除行号选择模式
            
            // 释放鼠标捕获
            ReleaseCapture();
            
            InvalidateRect(hWnd, NULL, TRUE);
        }
        return 0;

    case WM_RBUTTONUP:
        {
            EditorData* data = (EditorData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (!data) return 0;
            
            EditorDocument* doc = data->GetActiveDoc();
            if (!doc) return 0;

            // 如果没有选中内容，右键点击移动光标
            if (!doc->hasSelection) {
                int mouseX = LOWORD(lParam);
                int mouseY = HIWORD(lParam);
                
                // 调整编辑器内容的 mouseY
                mouseY -= data->tabHeight;
                mouseY += doc->scrollY;
                
                // 计算点击的行和列 - 使用EditorData中的配置
                int rowHeight = data->rowHeight;
                int fontSize = data->fontSize;
                int startX = 75 - doc->scrollX;  // 与WM_PAINT保持一致
                int startY = 5;
                
                // 计算行号 - 需要考虑表格间距
                int clickedLine = -1;
                bool hasVersion = (!doc->lines.empty() && doc->lines[0].find(L"版本\t") == 0);
                size_t startLine = hasVersion ? 1 : 0;
                
                // 逐行计算Y坐标，找到鼠标所在行（完全匹配绘制逻辑）
                int currentY = startY;
                bool insideAssembly = false;
                bool lastWasAssemblySection = false;
                int lastTableType = 0; // 0:无, 1:程序集, 2:子程序, 3:参数, 4:变量
                bool inClassVarTable = false;
                
                for (size_t i = startLine; i < doc->lines.size(); i++) {
                    const std::wstring& line = doc->lines[i];
                    
                    // 检查当前行是什么类型的表头（完全匹配绘制逻辑）
                    bool isSubProgramHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"子程序名") == 0);
                    bool isParamHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"参数名") == 0);
                    bool isVarHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"变量名") == 0);
                    
                    // 在程序集区域结束后、子程序表开始前添加间距
                    if (isSubProgramHeader && lastWasAssemblySection) {
                        currentY += rowHeight / 2;
                        lastWasAssemblySection = false;
                    }
                    
                    // 在参数表和变量表之间添加间距
                    if (isVarHeader && lastTableType == 3) {
                        currentY += rowHeight / 2;
                    }
                    
                    // 在子程序表（无参数时）和变量表之间添加间距
                    if (isVarHeader && lastTableType == 2) {
                        currentY += rowHeight / 2;
                    }
                    
                    // 空行标记表格结束
                    if (line.empty()) {
                        inClassVarTable = false;
                    }
                    
                    // 检查鼠标是否在当前行范围内
                    if (mouseY >= currentY && mouseY < currentY + rowHeight) {
                        clickedLine = (int)i;
                        break;
                    }
                    
                    // 更新状态标记（匹配绘制逻辑）
                    if (line.find(L"\t") != std::wstring::npos) {
                        std::vector<std::wstring> checkCells;
                        size_t start = 0, pos = 0;
                        while ((pos = line.find(L'\t', start)) != std::wstring::npos) {
                            checkCells.push_back(line.substr(start, pos - start));
                            start = pos + 1;
                        }
                        checkCells.push_back(line.substr(start));
                        
                        if (checkCells.size() > 0) {
                            if (checkCells[0] == L"程序集名") {
                                lastWasAssemblySection = true;
                                lastTableType = 1;
                                insideAssembly = true;
                            } else if (checkCells[0] == L"变量名" && insideAssembly) {
                                lastWasAssemblySection = true;
                                lastTableType = 1;
                                inClassVarTable = true;
                            } else if (checkCells[0] == L"子程序名") {
                                lastWasAssemblySection = false;
                                lastTableType = 2;
                                insideAssembly = false;
                            } else if (checkCells[0] == L"参数名") {
                                lastTableType = 3;
                            } else if (checkCells[0] == L"变量名" && !insideAssembly) {
                                lastTableType = 4;
                            }
                        }
                        
                        if (inClassVarTable) {
                            lastWasAssemblySection = true;
                        }
                    } else {
                        if (!inClassVarTable && !insideAssembly) {
                            lastWasAssemblySection = false;
                        }
                    }
                    
                    currentY += rowHeight;
                }
                
                // 如果没找到匹配行，使用边界值
                if (clickedLine < 0) {
                    clickedLine = (int)startLine;
                }
                
                // 精确计算列号（使用字体度量）
                HDC hdc = GetDC(hWnd);
                HFONT hFont = CreateFont(fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                       GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                       DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Consolas");
                HGDIOBJ hOldFont = SelectObject(hdc, hFont);
                
                // 计算tableMaxWidths（与WM_PAINT中的计算逻辑保持一致）
                std::vector<std::vector<int>> tableMaxWidths;
                std::vector<int> tableTypes;
                int preCalcTableIndex = -1;
                bool preCalcInsideAssembly = false;
                
                for (size_t i = startLine; i < doc->lines.size(); i++) {
                    const std::wstring& calcLine = doc->lines[i];
                    if (calcLine.empty()) {
                        preCalcInsideAssembly = false;
                        continue;
                    }
                    
                    if (calcLine.find(L"\t") != std::wstring::npos) {
                        std::vector<std::wstring> calcCells;
                        size_t cStart = 0, cPos = 0;
                        while ((cPos = calcLine.find(L'\t', cStart)) != std::wstring::npos) {
                            calcCells.push_back(calcLine.substr(cStart, cPos - cStart));
                            cStart = cPos + 1;
                        }
                        calcCells.push_back(calcLine.substr(cStart));
                        
                        bool isCalcHeader = (calcCells.size() > 0 && 
                                           (calcCells[0] == L"程序集名" || calcCells[0] == L"子程序名" || 
                                            calcCells[0] == L"参数名" || calcCells[0] == L"变量名"));
                        bool isCalcParam = (calcCells.size() > 0 && calcCells[0] == L"参数名");
                        bool isCalcAssemblyVar = (calcCells.size() > 0 && calcCells[0] == L"变量名" && preCalcInsideAssembly);
                        
                        if (isCalcHeader && !isCalcParam && !isCalcAssemblyVar) {
                            preCalcTableIndex++;
                            tableMaxWidths.push_back(std::vector<int>());
                            tableTypes.push_back(0);
                            
                            if (calcCells[0] == L"子程序名") {
                                tableMaxWidths[preCalcTableIndex] = {150, 100, 30, 320};
                            } else if (calcCells[0] == L"变量名") {
                                tableMaxWidths[preCalcTableIndex] = {100, 80, 30, 30, 200};
                            } else if (calcCells[0] == L"程序集名") {
                                tableMaxWidths[preCalcTableIndex] = {150, 100, 100, 200};
                                preCalcInsideAssembly = true;
                            }
                        }
                        
                        if (preCalcTableIndex >= 0 && preCalcTableIndex < (int)tableMaxWidths.size()) {
                            for (size_t j = 0; j < calcCells.size(); j++) {
                                if (j >= tableMaxWidths[preCalcTableIndex].size()) {
                                    tableMaxWidths[preCalcTableIndex].push_back(100);
                                }
                                if (!calcCells[j].empty()) {
                                    SIZE textSize;
                                    GetTextExtentPoint32W(hdc, calcCells[j].c_str(), (int)calcCells[j].length(), &textSize);
                                    int minWidth = textSize.cx + 10;
                                    if (minWidth > tableMaxWidths[preCalcTableIndex][j]) {
                                        tableMaxWidths[preCalcTableIndex][j] = minWidth;
                                    }
                                }
                            }
                        }
                    }
                }
                
                int clickedCol = 0;
                const std::wstring& line = doc->lines[clickedLine];
                
                // 检查是否为表格行
                if (line.find(L"\t") != std::wstring::npos) {
                    int relativeX = mouseX - startX;
                    // 表格行：根据鼠标X坐标判断点击在哪个单元格
                    std::vector<std::wstring> cells;
                    size_t start = 0, pos = 0;
                    while ((pos = line.find(L'\t', start)) != std::wstring::npos) {
                        cells.push_back(line.substr(start, pos - start));
                        start = pos + 1;
                    }
                    cells.push_back(line.substr(start));
                    
                    // 计算当前行所属的表格索引（与绘制逻辑一致）
                    int clickTableIndex = -1;
                    bool clickInsideAssembly = false;
                    bool clickInParamTable = false;
                    
                    for (size_t i = startLine; i <= (size_t)clickedLine; i++) {
                        const std::wstring& checkLine = doc->lines[i];
                        if (checkLine.empty()) {
                            continue;
                        }
                        
                        if (checkLine.find(L"\t") != std::wstring::npos) {
                            std::vector<std::wstring> checkCells;
                            size_t cStart = 0, cPos = 0;
                            while ((cPos = checkLine.find(L'\t', cStart)) != std::wstring::npos) {
                                checkCells.push_back(checkLine.substr(cStart, cPos - cStart));
                                cStart = cPos + 1;
                            }
                            checkCells.push_back(checkLine.substr(cStart));
                            
                            bool isHeader = (checkCells.size() > 0 && 
                                           (checkCells[0] == L"程序集名" || checkCells[0] == L"子程序名" || 
                                            checkCells[0] == L"参数名" || checkCells[0] == L"变量名"));
                            bool isParamHeader = (checkCells.size() > 0 && checkCells[0] == L"参数名");
                            bool isAssemblyVarHeader = (checkCells.size() > 0 && checkCells[0] == L"变量名" && clickInsideAssembly);
                            
                            if (isHeader && !isParamHeader && !isAssemblyVarHeader) {
                                clickTableIndex++;
                                if (checkCells[0] == L"程序集名") {
                                    clickInsideAssembly = true;
                                } else {
                                    clickInsideAssembly = false;
                                }
                                clickInParamTable = false;
                            } else if (isParamHeader) {
                                clickInParamTable = true;
                            }
                        }
                    }
                    
                    // 获取列宽（使用预先计算的tableMaxWidths）
                    std::vector<int> colWidths;
                    bool isHeader = (cells.size() > 0 && 
                                   (cells[0] == L"程序集名" || cells[0] == L"子程序名" || 
                                    cells[0] == L"参数名" || cells[0] == L"变量名"));
                    bool isParamTable = (cells.size() > 0 && cells[0] == L"参数名") || clickInParamTable;
                    
                    // 使用预先计算好的列宽
                    if (clickTableIndex >= 0 && clickTableIndex < (int)tableMaxWidths.size()) {
                        colWidths = tableMaxWidths[clickTableIndex];
                    } else {
                        // 如果没有预计算数据，使用默认值
                        for (size_t i = 0; i < cells.size(); i++) {
                            colWidths.push_back(100);
                        }
                    }
                    
                    // 根据鼠标X坐标判断点击在哪个单元格
                    int cellX = 0;
                    int cellIndex = 0;
                    bool foundCell = false;
                    
                    for (size_t i = 0; i < cells.size() && i < colWidths.size(); i++) {
                        int currentColWidth = colWidths[i];
                        
                        // 参数表的列3需要分割为可空+数组+备注
                        if (isParamTable && i == 3 && cells.size() >= 6) {
                            int nullableWidth = 30;  // 可空列
                            int arrayWidth = 30;     // 数组列
                            int remarkWidth = currentColWidth - nullableWidth - arrayWidth;  // 备注列
                            
                            // 检查是否点击在可空列
                            if (relativeX >= cellX && relativeX < cellX + nullableWidth) {
                                cellIndex = 3;  // 可空列（第4个单元格）
                                foundCell = true;
                                break;
                            } else if (relativeX >= cellX + nullableWidth && relativeX < cellX + nullableWidth + arrayWidth) {
                                cellIndex = 4;  // 数组列（第5个单元格）
                                cellX += nullableWidth;
                                foundCell = true;
                                break;
                            } else if (relativeX >= cellX + nullableWidth + arrayWidth && relativeX < cellX + currentColWidth) {
                                cellIndex = 5;  // 备注列（第6个单元格）
                                cellX += nullableWidth + arrayWidth;
                                foundCell = true;
                                break;
                            }
                            cellX += currentColWidth;
                            continue;
                        }
                        
                        if (relativeX >= cellX && relativeX < cellX + currentColWidth) {
                            cellIndex = (int)i;
                            foundCell = true;
                            break;
                        }
                        cellX += currentColWidth;
                    }
                    
                    // 如果没有找到单元格（点击在最后一列右侧），使用最后一列
                    if (!foundCell && cells.size() > 0) {
                        cellIndex = (int)cells.size() - 1;
                        // 重新计算cellX为最后一列的起始位置
                        cellX = 0;
                        
                        // 对于参数表，需要特殊处理列3的拆分
                        if (isParamTable && cells.size() >= 6) {
                            // 累加列0, 1, 2的宽度
                            for (int i = 0; i < 3 && i < (int)colWidths.size(); i++) {
                                cellX += colWidths[i];
                            }
                            // 如果cellIndex是5（备注列），需要累加可空(30)和数组(30)的宽度
                            if (cellIndex == 5) {
                                cellX += 30 + 30;  // 可空列 + 数组列
                            } else if (cellIndex == 4) {
                                cellX += 30;  // 可空列
                            }
                            // cellIndex == 3时，cellX已经正确（就是列0+1+2的总宽度）
                        } else {
                            // 非参数表，正常累加
                            for (int i = 0; i < cellIndex && i < (int)colWidths.size(); i++) {
                                cellX += colWidths[i];
                            }
                        }
                    }
                    
                    // 计算在单元格内的位置
                    int inCellX = relativeX - cellX;
                    if (inCellX < 0) inCellX = 0;
                    
                    // 计算光标在单元格内的字符位置
                    int inCellCol = 0;
                    if (cellIndex < (int)cells.size()) {
                        std::wstring cellText = cells[cellIndex];
                        SIZE textSize;
                        bool found = false;
                        for (size_t i = 0; i < cellText.length(); i++) {
                            std::wstring substr = cellText.substr(0, i + 1);
                            GetTextExtentPoint32W(hdc, substr.c_str(), (int)substr.length(), &textSize);
                            if (textSize.cx + 3 > inCellX) {  // 3是单元格内边距
                                SIZE prevSize;
                                if (i > 0) {
                                    std::wstring prevSubstr = cellText.substr(0, i);
                                    GetTextExtentPoint32W(hdc, prevSubstr.c_str(), (int)prevSubstr.length(), &prevSize);
                                    if (inCellX - (prevSize.cx + 3) < (textSize.cx + 3) - inCellX) {
                                        inCellCol = (int)i;
                                    } else {
                                        inCellCol = (int)i + 1;
                                    }
                                } else {
                                    inCellCol = ((textSize.cx + 3) / 2 > inCellX) ? 0 : 1;
                                }
                                found = true;
                                break;
                            }
                        }
                        // 如果点击位置超过文本宽度，光标应该在文本末尾
                        if (!found) {
                            inCellCol = (int)cellText.length();
                        }
                    }
                    
                    // 计算在整行中的字符位置
                    clickedCol = 0;
                    for (int i = 0; i < cellIndex && i < (int)cells.size(); i++) {
                        // 参数表的可空(3)、数组(4)、备注(5)列对应实际数据的列3、4、5
                        // 不需要特殊处理，因为cells已经包含了这些列
                        clickedCol += (int)cells[i].length() + 1;  // +1 为制表符
                    }
                    clickedCol += inCellCol;
                } else {
                    // 非表格行：使用原有逻辑，但需要考虑缩进
                    int textStartX = startX;
                    size_t indentChars = 0;  // 缩进字符数量
                    
                    // 计算实际文本的起始位置
                    if (IsFlowControlLine(line)) {
                        textStartX = startX + 20;
                    } else {
                        // 计算缩进
                        int indent = 0;
                        while (indentChars < line.length() && (line[indentChars] == L' ' || line[indentChars] == L'\t')) {
                            if (line[indentChars] == L'\t') indent += 20;
                            else indent += 4;
                            indentChars++;
                        }
                        textStartX = startX + indent;
                    }
                    
                    int relativeX = mouseX - textStartX;  // 相对于实际文本起始位置
                    
                    if (relativeX > 0 && line.length() > indentChars) {
                        // 只对实际文本部分（去除缩进）计算位置
                        std::wstring textPart = line.substr(indentChars);
                        SIZE textSize;
                        for (size_t i = 0; i < textPart.length(); i++) {
                            std::wstring substr = textPart.substr(0, i + 1);
                            GetTextExtentPoint32W(hdc, substr.c_str(), (int)substr.length(), &textSize);
                            if (textSize.cx > relativeX) {
                                // 检查是靠近左边还是右边
                                SIZE prevSize;
                                if (i > 0) {
                                    std::wstring prevSubstr = textPart.substr(0, i);
                                    GetTextExtentPoint32W(hdc, prevSubstr.c_str(), (int)prevSubstr.length(), &prevSize);
                                    if (relativeX - prevSize.cx < textSize.cx - relativeX) {
                                        clickedCol = (int)indentChars + (int)i;
                                    } else {
                                        clickedCol = (int)indentChars + (int)i + 1;
                                    }
                                } else {
                                    clickedCol = (textSize.cx / 2 > relativeX) ? (int)indentChars : (int)indentChars + 1;
                                }
                                break;
                            }
                            clickedCol = (int)indentChars + (int)i + 1;
                        }
                    } else if (relativeX <= 0) {
                        // 点击在文本起始位置左侧，光标放在缩进后的位置
                        clickedCol = (int)indentChars;
                    } else {
                        // 点击在文本末尾右侧
                        clickedCol = (int)line.length();
                    }
                }
                
                if (clickedCol > (int)line.length()) {
                    clickedCol = (int)line.length();
                }
                
                SelectObject(hdc, hOldFont);
                DeleteObject(hFont);
                ReleaseDC(hWnd, hdc);
                
                // 更新光标位置
                doc->cursorLine = clickedLine;
                doc->cursorCol = clickedCol;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            
            POINT pt;
            GetCursorPos(&pt);
            
            HMENU hMenu = CreatePopupMenu();
            AppendMenuW(hMenu, MF_STRING, IDM_NEW_SUB, L"新子程序");
            AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
            AppendMenuW(hMenu, MF_STRING, IDM_COPY, L"复制");
            AppendMenuW(hMenu, MF_STRING, IDM_PASTE, L"粘贴");
            AppendMenuW(hMenu, MF_STRING, IDM_CUT, L"剪切");
            AppendMenuW(hMenu, MF_STRING, IDM_SELECT_ALL, L"全选");
            AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
            AppendMenuW(hMenu, MF_STRING, IDM_COMMENT, L"屏蔽整行");
            AppendMenuW(hMenu, MF_STRING, IDM_UNCOMMENT, L"解除屏蔽");
            
            TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
            DestroyMenu(hMenu);
        }
        return 0;

    case WM_COMMAND:
        {
            EditorData* data = (EditorData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (!data) break;
            EditorDocument* doc = data->GetActiveDoc();
            if (!doc) break;
            
            switch (LOWORD(wParam)) {
            case IDM_NEW_SUB:
                InsertNewSubroutine(doc);
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case IDM_COPY:
                CopyToClipboard(hWnd, doc);
                break;
            case IDM_PASTE:
                doc->CreateSnapshot(L"Paste");
                PasteFromClipboard(hWnd, doc);
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case IDM_CUT:
                doc->CreateSnapshot(L"Cut");
                CutToClipboard(hWnd, doc);
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case IDM_SELECT_ALL:
                SelectAll(doc);
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case IDM_COMMENT:
                doc->CreateSnapshot(L"Comment");
                CommentLine(doc);
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case IDM_UNCOMMENT:
                doc->CreateSnapshot(L"Uncomment");
                UncommentLine(doc);
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            }
            UpdateScrollBars(hWnd);
        }
        break;
        
    case WM_CHAR:
        {
            EditorData* data = (EditorData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (!data) return 0;
            
            // 如果正在显示欢迎页，任何输入创建新文档
            if (data->showWelcomePage) {
                data->AddDocument(L"");
                InvalidateRect(hWnd, NULL, TRUE);
                // 继续处理字符输入
            }
            
            EditorDocument* doc = data->GetActiveDoc();
            if (!doc) return 0;
            
            wchar_t ch = (wchar_t)wParam;
            
            // 处理TAB键
            if (ch == L'\t') {
                if (doc->cursorLine < (int)doc->lines.size()) {
                    const std::wstring& line = doc->lines[doc->cursorLine];
                    
                    // 检查是否是表格行（包含制表符）
                    bool isTableLine = (line.find(L'\t') != std::wstring::npos);
                    
                    if (isTableLine) {
                        // 表格行：移动到下一个单元格
                        size_t tabPos = line.find(L'\t', doc->cursorCol);
                        if (tabPos != std::wstring::npos) {
                            doc->cursorCol = (int)tabPos + 1;
                        } else {
                            // 没有更多单元格，移到下一行
                            if (doc->cursorLine + 1 < (int)doc->lines.size()) {
                                doc->cursorLine++;
                                doc->cursorCol = 0;
                            }
                        }
                    } else {
                        // 普通行：直接移到下一行
                        if (doc->cursorLine + 1 < (int)doc->lines.size()) {
                            doc->cursorLine++;
                            doc->cursorCol = 0;
                        }
                    }
                    InvalidateRect(hWnd, NULL, TRUE);
                }
                return 0;
            }
            
            if (ch >= 32) {  // 可打印字符
                if (doc->cursorLine < (int)doc->lines.size()) {
                    // 检查是否是表头行，如果是则禁止编辑
                    const std::wstring& checkLine = doc->lines[doc->cursorLine];
                    if (checkLine.find(L"程序集名") == 0 || 
                        checkLine.find(L"版本\t") == 0 ||
                        checkLine.find(L"子程序名") == 0 ||
                        checkLine.find(L"参数名") == 0 ||
                        checkLine.find(L"变量名") == 0) {
                        return 0;
                    }
                    
                    // 检查是否在程序集区域但不在任何表内（不允许输入普通代码）
                    if (IsInAssemblyButNotInTable(doc, doc->cursorLine)) {
                        return 0;
                    }
                    
                    // 检查是否是参数行，如果光标在参数名区域则不允许输入
                    if (checkLine.length() > 0 && checkLine[0] == L'\u2060') {
                        size_t colonPos = checkLine.find(L':');
                        if (colonPos == std::wstring::npos) {
                            colonPos = checkLine.length();
                        }
                        int minCursorCol = (int)colonPos + 1;  // 光标最小位置在冒号之后
                        if (doc->cursorCol < minCursorCol) {
                            // 将光标移动到冒号之后，然后输入
                            doc->cursorCol = minCursorCol;
                        }
                    }

                    // 自动将中文符号转换为英文符号
                    if (ch == L'，') ch = L',';
                    else if (ch == L'。') ch = L'.';
                    else if (ch == L'；') ch = L';';
                    else if (ch == L'：') ch = L':';
                    else if (ch == L'？') ch = L'?';
                    else if (ch == L'！') ch = L'!';
                    else if (ch == L'（') ch = L'(';
                    else if (ch == L'）') ch = L')';
                    else if (ch == L'【') ch = L'[';
                    else if (ch == L'】') ch = L']';
                    else if (ch == L'「') ch = L'[';
                    else if (ch == L'」') ch = L']';
                    else if (ch == 0x201C) ch = L'"';  // 中文左双引号 "
                    else if (ch == 0x201D) ch = L'"';  // 中文右双引号 "
                    else if (ch == 0x2018) ch = L'\''; // 中文左单引号 '
                    else if (ch == 0x2019) ch = L'\''; // 中文右单引号 '
                    
                    doc->CreateSnapshot(L"Typing");;
                    
                    // 如果有选区，检查是否跨越多个单元格（包含制表符）
                    if (doc->hasSelection) {
                        int startL, startC, endL, endC;
                        
                        // 规范化选择区域
                        if (doc->selStartLine < doc->selEndLine) {
                            startL = doc->selStartLine; startC = doc->selStartCol;
                            endL = doc->selEndLine; endC = doc->selEndCol;
                        } else if (doc->selStartLine > doc->selEndLine) {
                            startL = doc->selEndLine; startC = doc->selEndCol;
                            endL = doc->selStartLine; endC = doc->selStartCol;
                        } else {
                            startL = doc->selStartLine;
                            endL = doc->selEndLine;
                            if (doc->selStartCol < doc->selEndCol) {
                                startC = doc->selStartCol; endC = doc->selEndCol;
                            } else {
                                startC = doc->selEndCol; endC = doc->selStartCol;
                            }
                        }
                        
                        // 检查起始行是否是表格行
                        bool isTableLine = false;
                        if (startL < (int)doc->lines.size()) {
                            const std::wstring& line = doc->lines[startL];
                            isTableLine = (line.find(L'\t') != std::wstring::npos);
                        }
                        
                        // 只有表格行才需要检查跨单元格选择
                        if (isTableLine) {
                            // 检查选中内容是否包含制表符或跨行
                            bool hasTabOrMultiLine = false;
                            if (startL != endL) {
                                hasTabOrMultiLine = true;  // 跨行
                            } else if (startL < (int)doc->lines.size()) {
                                // 同一行，检查是否包含制表符
                                const std::wstring& line = doc->lines[startL];
                                for (int i = startC; i < endC && i < (int)line.length(); i++) {
                                    if (line[i] == L'\t') {
                                        hasTabOrMultiLine = true;
                                        break;
                                    }
                                }
                            }
                            
                            if (hasTabOrMultiLine) {
                                // 跨单元格选择，只清空第一个单元格到制表符之前的内容
                                if (startL < (int)doc->lines.size()) {
                                    std::wstring& line = doc->lines[startL];
                                    int tabPos = startC;
                                    while (tabPos < (int)line.length() && line[tabPos] != L'\t') {
                                        tabPos++;
                                    }
                                    // 清空从startC到制表符之前的内容
                                    if (startC < tabPos) {
                                        line.erase(startC, tabPos - startC);
                                    }
                                    doc->cursorLine = startL;
                                    doc->cursorCol = startC;
                                    doc->hasSelection = false;
                                }
                            } else {
                                // 单元格内选择，正常删除
                                DeleteSelection(doc);
                            }
                        } else {
                            // 普通代码行，直接删除所有选中内容
                            DeleteSelection(doc);
                        }
                    }
                    
                    std::wstring& line = doc->lines[doc->cursorLine];
                    if (doc->cursorCol <= (int)line.length()) {
                        // 检查是否需要跳过空格（类型补全空格上屏后）
                        if (ch == L' ' && data->skipNextSpaceForType) {
                            data->skipNextSpaceForType = false;
                            return 0;
                        }
                        // 检查是否需要跳过字符（自动补全上屏后）
                        if (ch == L' ' && data->skipNextChar) {
                            data->skipNextChar = false;
                            return 0;
                        }
                        
                        line.insert(doc->cursorCol, 1, ch);
                        doc->cursorCol++;
                        doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                        
                        // 如果是参数行，同步更新命令行括号内的参数值
                        if (line.length() > 0 && line[0] == L'\u2060') {
                            SyncParamLineToCommandLine(doc, doc->cursorLine);
                            SyncAssignParamLineToAssignLine(doc, doc->cursorLine);
                        }
                        
                        // 补全逻辑：
                        // 1. 表格行中只有类型列需要类型补全，其他列不需要任何补全
                        // 2. 只有普通代码行才需要命令补全
                        // 3. 输入点号后显示控件属性补全
                        
                        if (IsInTableRow(doc)) {
                            // 在表格行中
                            if (IsEditingTypeColumn(doc)) {
                                // 在类型列，触发类型补全
                                UpdateTypeCompletion(hWnd, data, doc);
                                data->showCompletion = false;  // 确保命令补全窗口关闭
                            } else {
                                // 表格的其他列，不需要任何补全
                                HideTypeCompletion(data);
                                data->showCompletion = false;
                            }
                        } else if (ch == L'.') {
                            // 输入点号，检查是否需要属性补全
                            HideTypeCompletion(data);
                            
                            // 获取点号前的单词（控件名称）
                            std::wstring controlName = GetWordBeforeDot(line, doc->cursorCol - 1);
                            if (!controlName.empty()) {
                                // 获取该控件的属性补全项
                                std::vector<CompletionItem> propCompletions = GetControlPropertyCompletions(controlName);
                                if (!propCompletions.empty()) {
                                    data->showCompletion = true;
                                    data->completionItems = propCompletions;
                                    data->selectedCompletionIndex = 0;
                                    data->completionScrollOffset = 0;
                                    data->currentWord = L"";  // 点号后还没有输入内容
                                    data->completionMaxVisible = 8;
                                    data->completionItemHeight = 24;
                                } else {
                                    data->showCompletion = false;
                                }
                            } else {
                                data->showCompletion = false;
                            }
                        } else if (ch != L' ') {
                            // 非表格行（普通代码行），触发命令补全
                            HideTypeCompletion(data);
                            int wordStart;
                            std::wstring currentWord = GetCurrentWord(line, doc->cursorCol, wordStart);
                            
                            // 检查当前单词前是否有点号（正在输入属性名）
                            bool isPropertyInput = false;
                            if (wordStart > 0 && line[wordStart - 1] == L'.') {
                                std::wstring controlName = GetWordBeforeDot(line, wordStart - 1);
                                if (!controlName.empty()) {
                                    // 获取该控件的属性补全项并筛选
                                    std::vector<CompletionItem> propCompletions = GetControlPropertyCompletions(controlName);
                                    if (!propCompletions.empty() && !currentWord.empty()) {
                                        // 筛选匹配当前输入的属性
                                        std::vector<CompletionItem> filteredProps;
                                        std::wstring lowerWord = currentWord;
                                        std::transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::towlower);
                                        
                                        for (auto& prop : propCompletions) {
                                            std::wstring lowerPropName = prop.insertText;
                                            std::transform(lowerPropName.begin(), lowerPropName.end(), lowerPropName.begin(), ::towlower);
                                            
                                            // 中文匹配
                                            if (lowerPropName.find(lowerWord) == 0) {
                                                filteredProps.push_back(prop);
                                            }
                                            // 拼音匹配
                                            else {
                                                std::wstring pinyin = PinyinHelper::GetStringPinyin(prop.insertText);
                                                std::wstring initials = PinyinHelper::GetStringInitials(prop.insertText);
                                                std::transform(pinyin.begin(), pinyin.end(), pinyin.begin(), ::towlower);
                                                std::transform(initials.begin(), initials.end(), initials.begin(), ::towlower);
                                                
                                                if ((!pinyin.empty() && pinyin.find(lowerWord) == 0) ||
                                                    (!initials.empty() && initials.find(lowerWord) == 0)) {
                                                    filteredProps.push_back(prop);
                                                }
                                            }
                                        }
                                        
                                        if (!filteredProps.empty()) {
                                            data->showCompletion = true;
                                            data->completionItems = filteredProps;
                                            data->selectedCompletionIndex = 0;
                                            data->completionScrollOffset = 0;
                                            data->currentWord = currentWord;
                                            data->completionMaxVisible = 8;
                                            data->completionItemHeight = 24;
                                            isPropertyInput = true;
                                        } else {
                                            data->showCompletion = false;
                                            isPropertyInput = true;  // 仍然标记为属性输入（虽然没有匹配）
                                        }
                                    } else if (!propCompletions.empty() && currentWord.empty()) {
                                        // 点号后还没有输入，显示所有属性
                                        data->showCompletion = true;
                                        data->completionItems = propCompletions;
                                        data->selectedCompletionIndex = 0;
                                        data->completionScrollOffset = 0;
                                        data->currentWord = L"";
                                        data->completionMaxVisible = 8;
                                        data->completionItemHeight = 24;
                                        isPropertyInput = true;
                                    }
                                }
                            }
                            
                            // 如果不是属性输入，则进行普通命令补全（包含控件名称）
                            if (!isPropertyInput) {
                                if (!currentWord.empty() && currentWord.length() >= 1) {
                                    // 获取关键词补全
                                    std::vector<CompletionItem> completions = KeywordManager::GetInstance().GetCompletions(currentWord);
                                    
                                    // 获取可视化设计器控件名称补全
                                    std::vector<CompletionItem> ctrlCompletions = GetDesignerControlCompletions(currentWord);
                                    
                                    // 合并两个列表，控件名称在前
                                    for (const auto& ctrl : ctrlCompletions) {
                                        completions.insert(completions.begin(), ctrl);
                                    }
                                    
                                    if (!completions.empty()) {
                                        data->showCompletion = true;
                                        data->completionItems = completions;
                                        data->selectedCompletionIndex = 0;
                                        data->completionScrollOffset = 0;
                                        data->currentWord = currentWord;
                                        data->completionMaxVisible = 8;
                                        data->completionItemHeight = 24;
                                    } else {
                                        data->showCompletion = false;
                                    }
                                } else {
                                    data->showCompletion = false;
                                }
                            }
                        } else {
                            // 空格输入时，隐藏所有补全
                            HideTypeCompletion(data);
                            data->showCompletion = false;
                        }
                        
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                }
            } else if (ch == VK_RETURN) {  // 回车
                if (data->skipNextChar) {
                    data->skipNextChar = false;
                    return 0;
                }
                bool handled = false;

                // 检查是否是表头行，如果是则禁止回车
                if (doc->cursorLine < (int)doc->lines.size()) {
                    const std::wstring& checkLine = doc->lines[doc->cursorLine];
                    if (checkLine.find(L"程序集名") == 0 || 
                        checkLine.find(L"版本\t") == 0 ||
                        checkLine.find(L"子程序名") == 0 ||
                        checkLine.find(L"参数名") == 0 ||
                        checkLine.find(L"变量名") == 0) {
                        return 0;
                    }
                    
                    // 检查是否在程序集区域但不在任何表内（不允许创建新行）
                    if (IsInAssemblyButNotInTable(doc, doc->cursorLine)) {
                        return 0;
                    }
                }
                
                doc->CreateSnapshot(L"Enter");

                // 查找版本行和程序集表头行
                int versionLine = -1;
                int assemblyHeaderLine = -1;
                
                for (int i = 0; i < (int)doc->lines.size(); i++) {
                    if (versionLine == -1 && doc->lines[i].find(L"版本\t") == 0) {
                        versionLine = i;
                    }
                    if (assemblyHeaderLine == -1 && doc->lines[i].find(L"程序集名") == 0) {
                        assemblyHeaderLine = i;
                    }
                    if (versionLine != -1 && assemblyHeaderLine != -1) break;
                    // 只在前几行查找，避免误判
                    if (i > 5) break;
                }

                // 如果没找到，尝试使用默认位置
                if (assemblyHeaderLine == -1) {
                    bool hasVersion = (!doc->lines.empty() && doc->lines[0].find(L"版本\t") == 0);
                    assemblyHeaderLine = hasVersion ? 1 : 0;
                }

                // 禁止在版本行和程序集表头行回车
                if (doc->cursorLine == versionLine || doc->cursorLine == assemblyHeaderLine) {
                    return 0; 
                }

                int assemblyDataLine = assemblyHeaderLine + 1;

                if (doc->cursorLine == assemblyDataLine) {
                    // 程序集数据行回车：自动创建或跳转到程序集变量表
                    bool hasVarTable = false;
                    int nextLine = doc->cursorLine + 1;
                    
                    // 检查下一行是否已经是变量表
                    if (nextLine < (int)doc->lines.size()) {
                        // 跳过可能的空行
                        while (nextLine < (int)doc->lines.size() && doc->lines[nextLine].empty()) {
                            nextLine++;
                        }
                        if (nextLine < (int)doc->lines.size() && doc->lines[nextLine].find(L"变量名") == 0) {
                            hasVarTable = true;
                        }
                    }
                    
                    if (!hasVarTable) {
                        // 创建新的程序集变量表
                        // 插入位置在数据行之后
                        int insertPos = assemblyDataLine + 1;
                        doc->lines.insert(doc->lines.begin() + insertPos, L"变量名\t类型\t数组\t备注");
                        doc->lines.insert(doc->lines.begin() + insertPos + 1, L"\t整数型\t\t");
                        doc->cursorLine = insertPos + 1;
                        doc->cursorCol = 0;
                    } else {
                        // 跳转到现有变量表的数据行
                        // nextLine 是表头，跳转到 nextLine + 1
                        if (nextLine + 1 < (int)doc->lines.size()) {
                            doc->cursorLine = nextLine + 1;
                            doc->cursorCol = 0;
                        } else {
                            // 如果表头是最后一行，添加数据行
                            doc->lines.insert(doc->lines.begin() + nextLine + 1, L"\t整数型\t\t");
                            doc->cursorLine = nextLine + 1;
                            doc->cursorCol = 0;
                        }
                    }
                    doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                    InvalidateRect(hWnd, NULL, TRUE);
                    return 0;
                }
                
                if (doc->cursorLine >= 0 && doc->cursorLine < (int)doc->lines.size()) {
                    std::wstring line = doc->lines[doc->cursorLine];
                    
                    // 检查是否是参数行，在参数行回车添加新参数行
                    if (line.length() > 0 && line[0] == L'\u2060') {
                        // 在参数行回车，插入新的参数行
                        // 找到命令行以获取下一个参数名
                        int cmdLineIndex = doc->cursorLine - 1;
                        while (cmdLineIndex >= 0 && doc->lines[cmdLineIndex].length() > 0 && doc->lines[cmdLineIndex][0] == L'\u2060') {
                            cmdLineIndex--;
                        }
                        
                        if (cmdLineIndex >= 0) {
                            // 使用 ExtractCommandName 提取命令名
                            const std::wstring& cmdLine = doc->lines[cmdLineIndex];
                            std::wstring cmdName = ExtractCommandName(cmdLine);
                            
                            std::vector<LibraryParameter> params = GetCommandParameters(cmdName);
                            
                            // 计算当前是第几个参数行
                            int paramIndex = 0;
                            for (int i = cmdLineIndex + 1; i <= doc->cursorLine; i++) {
                                if (doc->lines[i].length() > 0 && doc->lines[i][0] == L'\u2060') {
                                    if (i < doc->cursorLine) paramIndex++;
                                }
                            }
                            
                            // 如果还有更多参数，插入下一个参数行
                            if (paramIndex + 1 < (int)params.size()) {
                                std::wstring newParamLine = L"\u2060" + params[paramIndex + 1].name + L":";
                                doc->lines.insert(doc->lines.begin() + doc->cursorLine + 1, newParamLine);
                                while (doc->parametersExpanded.size() <= doc->cursorLine + 1) {
                                    doc->parametersExpanded.push_back(false);
                                }
                                doc->parametersExpanded.insert(doc->parametersExpanded.begin() + doc->cursorLine + 1, false);
                                doc->cursorLine++;
                                size_t colonPos = newParamLine.find(L':');
                                doc->cursorCol = (colonPos != std::wstring::npos) ? (int)colonPos + 1 : (int)newParamLine.length();
                                doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                                InvalidateRect(hWnd, NULL, TRUE);
                                return 0;
                            } else {
                                // 没有更多参数，在最后一个参数行后插入带标记的内容行
                                // 查找最后一个连续参数行
                                int lastParam = doc->cursorLine;
                                while (lastParam + 1 < (int)doc->lines.size() && 
                                       doc->lines[lastParam + 1].length() > 0 && doc->lines[lastParam + 1][0] == L'\u2060') {
                                    lastParam++;
                                }
                                // 获取命令行的嵌套标记前缀，内容行应比命令行深一层
                                size_t pCmdMarkerLen = 0;
                                int pCmdDepth = GetNestingDepth(cmdLine, &pCmdMarkerLen);
                                std::wstring newLine;
                                int newCol = 0;
                                if (pCmdDepth > 0) {
                                    newLine = cmdLine.substr(0, pCmdMarkerLen) + L"\u200B\u200C";
                                    newCol = (int)newLine.length();
                                } else {
                                    newLine = L"\u200C";
                                    newCol = 1;
                                }
                                doc->lines.insert(doc->lines.begin() + lastParam + 1, newLine);
                                while (doc->parametersExpanded.size() <= lastParam + 1) {
                                    doc->parametersExpanded.push_back(false);
                                }
                                doc->parametersExpanded.insert(doc->parametersExpanded.begin() + lastParam + 1, false);
                                doc->cursorLine = lastParam + 1;
                                doc->cursorCol = newCol;
                                doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                                InvalidateRect(hWnd, NULL, TRUE);
                                return 0;
                            }
                        }
                        // 如果没有找到命令行，继续到下面的普通处理
                    }
                    
                    // 检查当前行是否有括号（可能是命令行）
                    size_t leftBracket = line.find(L'(');
                    size_t rightBracket = line.find(L')');
                    
                    if (leftBracket != std::wstring::npos && rightBracket != std::wstring::npos) {
                        // 有括号的行（命令行）
                        // 只允许在右括号后面回车，其他位置都禁止
                        if (doc->cursorCol <= (int)rightBracket) {
                            // 光标在右括号或之前的位置，禁止回车
                            return 0;
                        } else {
                            // 光标在括号后，检查是否有展开的参数行
                            bool hasParamLines = false;
                            int lastParamLine = doc->cursorLine;
                            
                            for (int i = doc->cursorLine + 1; i < (int)doc->lines.size(); i++) {
                                if (doc->lines[i].length() > 0 && doc->lines[i][0] == L'\u2060') {
                                    hasParamLines = true;
                                    lastParamLine = i;
                                } else {
                                    break;
                                }
                            }
                            
                            if (hasParamLines) {
                                // 跳过参数行，在最后一个参数行后面插入新行
                                // 获取命令行的嵌套标记前缀，内容行应比命令行深一层
                                size_t cmdMarkerLen = 0;
                                int cmdDepth = GetNestingDepth(line, &cmdMarkerLen);
                                std::wstring newContentLine;
                                int newCol = 0;
                                if (cmdDepth > 0) {
                                    newContentLine = line.substr(0, cmdMarkerLen) + L"\u200B\u200C";
                                    newCol = (int)newContentLine.length();
                                } else {
                                    newContentLine = L"\u200C";
                                    newCol = 1;
                                }
                                doc->lines.insert(doc->lines.begin() + lastParamLine + 1, newContentLine);
                                while (doc->parametersExpanded.size() <= lastParamLine + 1) {
                                    doc->parametersExpanded.push_back(false);
                                }
                                doc->parametersExpanded.insert(doc->parametersExpanded.begin() + lastParamLine + 1, false);
                                doc->cursorLine = lastParamLine + 1;
                                doc->cursorCol = newCol;
                                doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                                InvalidateRect(hWnd, NULL, TRUE);
                                return 0;
                            }
                            // 如果没有参数行，继续正常处理（下面的代码会处理）
                        }
                    }
                    
                    // 检查是否输入了 .参数 或 .变量
                    std::wstring trimLine = line;
                    // 去除首尾空格
                    trimLine.erase(0, trimLine.find_first_not_of(L" \t"));
                    trimLine.erase(trimLine.find_last_not_of(L" \t") + 1);
                    
                    if (trimLine == L".参数" || trimLine == L".变量") {
                        // 查找当前上下文
                        int subStart = -1;
                        int assemblyStart = -1;
                        
                        for (int i = doc->cursorLine; i >= 0; i--) {
                            if (doc->lines[i].find(L"子程序名") == 0) {
                                subStart = i;
                                break;
                            }
                            if (doc->lines[i].find(L"程序集名") == 0) {
                                assemblyStart = i;
                                if (subStart == -1) break; // 找到了程序集且没在子程序内
                            }
                        }
                        
                        if (trimLine == L".参数") {
                            if (subStart != -1) {
                                // 在子程序内：添加到参数表
                                int insertPos = subStart + 2; // 默认在子程序数据行之后
                                bool hasParamTable = false;
                                
                                // 检查是否已有参数表
                                if (insertPos < (int)doc->lines.size() && doc->lines[insertPos].find(L"参数名") == 0) {
                                    hasParamTable = true;
                                    // 找到参数表末尾
                                    insertPos++;
                                    while (insertPos < (int)doc->lines.size()) {
                                        std::wstring& l = doc->lines[insertPos];
                                        if (l.empty() || l.find(L"变量名") == 0 || l.find(L"子程序名") == 0) break;
                                        insertPos++;
                                    }
                                }
                                
                                // 清除当前行
                                doc->lines.erase(doc->lines.begin() + doc->cursorLine);
                                if (doc->cursorLine < insertPos) insertPos--; // 调整插入位置
                                
                                if (!hasParamTable) {
                                    // 插入新参数表
                                    // 确保紧挨着子程序（删除中间的空行）
                                    while (insertPos > subStart + 2 && doc->lines[insertPos-1].empty()) {
                                        doc->lines.erase(doc->lines.begin() + insertPos - 1);
                                        insertPos--;
                                    }
                                    
                                    doc->lines.insert(doc->lines.begin() + insertPos, L"参数名\t类型\t参考\t可空\t数组\t备注");
                                    doc->lines.insert(doc->lines.begin() + insertPos + 1, L"\t整数型\t\t\t\t");
                                    doc->cursorLine = insertPos + 1;
                                } else {
                                    // 追加到现有参数表
                                    doc->lines.insert(doc->lines.begin() + insertPos, L"\t整数型\t\t\t\t");
                                    doc->cursorLine = insertPos;
                                }
                                doc->cursorCol = 0;
                                doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                                InvalidateRect(hWnd, NULL, TRUE);
                                handled = true;
                            }
                        } else if (trimLine == L".变量") {
                            if (subStart != -1) {
                                // 在子程序内：添加到局部变量表
                                int insertPos = subStart + 2;
                                // 跳过参数表
                                if (insertPos < (int)doc->lines.size() && doc->lines[insertPos].find(L"参数名") == 0) {
                                    insertPos++;
                                    while (insertPos < (int)doc->lines.size()) {
                                        std::wstring& l = doc->lines[insertPos];
                                        if (l.empty() || l.find(L"变量名") == 0 || l.find(L"子程序名") == 0) break;
                                        insertPos++;
                                    }
                                }
                                
                                bool hasVarTable = false;
                                if (insertPos < (int)doc->lines.size() && doc->lines[insertPos].find(L"变量名") == 0) {
                                    hasVarTable = true;
                                    insertPos++;
                                    while (insertPos < (int)doc->lines.size()) {
                                        std::wstring& l = doc->lines[insertPos];
                                        if (l.empty() || l.find(L"子程序名") == 0) break;
                                        insertPos++;
                                    }
                                }
                                
                                // 清除当前行
                                doc->lines.erase(doc->lines.begin() + doc->cursorLine);
                                if (doc->cursorLine < insertPos) insertPos--;
                                
                                if (!hasVarTable) {
                                    // 插入新变量表
                                    // 确保紧挨着上方内容（删除中间的空行）
                                    while (insertPos > subStart + 2 && doc->lines[insertPos-1].empty()) {
                                        doc->lines.erase(doc->lines.begin() + insertPos - 1);
                                        insertPos--;
                                    }
                                    
                                    doc->lines.insert(doc->lines.begin() + insertPos, L"变量名\t类型\t静态\t数组\t备注");
                                    doc->lines.insert(doc->lines.begin() + insertPos + 1, L"\t整数型\t\t\t");
                                    doc->cursorLine = insertPos + 1;
                                } else {
                                    // 追加到现有变量表
                                    doc->lines.insert(doc->lines.begin() + insertPos, L"\t整数型\t\t\t");
                                    doc->cursorLine = insertPos;
                                }
                                doc->cursorCol = 0;
                                doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                                InvalidateRect(hWnd, NULL, TRUE);
                                handled = true;
                            } else if (assemblyStart != -1) {
                                // 在程序集区域（且不在子程序内）：添加到程序集变量表
                                int insertPos = assemblyStart + 2; // 程序集数据行之后
                                bool hasVarTable = false;
                                
                                if (insertPos < (int)doc->lines.size() && doc->lines[insertPos].find(L"变量名") == 0) {
                                    hasVarTable = true;
                                    insertPos++;
                                    while (insertPos < (int)doc->lines.size()) {
                                        std::wstring& l = doc->lines[insertPos];
                                        if (l.empty() || l.find(L"子程序名") == 0) break;
                                        insertPos++;
                                    }
                                }
                                
                                // 清除当前行
                                doc->lines.erase(doc->lines.begin() + doc->cursorLine);
                                if (doc->cursorLine < insertPos) insertPos--;
                                
                                if (!hasVarTable) {
                                    doc->lines.insert(doc->lines.begin() + insertPos, L"变量名\t类型\t数组\t备注");
                                    doc->lines.insert(doc->lines.begin() + insertPos + 1, L"\t整数型\t\t");
                                    doc->cursorLine = insertPos + 1;
                                } else {
                                    doc->lines.insert(doc->lines.begin() + insertPos, L"\t整数型\t\t");
                                    doc->cursorLine = insertPos;
                                }
                                doc->cursorCol = 0;
                                doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                                InvalidateRect(hWnd, NULL, TRUE);
                                handled = true;
                            }
                        }
                    }
                    
                    // 判断是否在表格行
                    if (!handled && line.find(L"\t") != std::wstring::npos) {
                        // 向上查找表头
                        std::wstring headerType = L"";
                        int headerLine = -1;
                        
                        for (int i = doc->cursorLine; i >= 0; i--) {
                            if (doc->lines[i].find(L"子程序名") == 0) {
                                headerType = L"子程序名";
                                headerLine = i;
                                break;
                            }
                            if (doc->lines[i].find(L"参数名") == 0) {
                                headerType = L"参数名";
                                headerLine = i;
                                break;
                            }
                            if (doc->lines[i].find(L"变量名") == 0) {
                                headerType = L"变量名";
                                headerLine = i;
                                break;
                            }
                            if (doc->lines[i].empty()) break; // 表格结束
                        }
                        
                        if (headerType == L"子程序名") {
                            // 在子程序表中
                            if (doc->cursorLine == headerLine) {
                                // 在表头：移动到下一行（数据行）
                                if (doc->cursorLine + 1 < (int)doc->lines.size()) {
                                    doc->cursorLine++;
                                    doc->cursorCol = 0;
                                }
                                handled = true;
                            } else if (doc->cursorLine == headerLine + 1) {
                                // 在子程序数据行：插入/跳转到参数表
                                bool hasParamTable = false;
                                int paramHeaderLine = -1;
                                
                                // 检查下一行是否是参数表
                                if (doc->cursorLine + 1 < (int)doc->lines.size()) {
                                    if (doc->lines[doc->cursorLine + 1].find(L"参数名") == 0) {
                                        hasParamTable = true;
                                        paramHeaderLine = doc->cursorLine + 1;
                                    }
                                }
                                
                                if (!hasParamTable) {
                                    // 插入参数表
                                    doc->lines.insert(doc->lines.begin() + doc->cursorLine + 1, L"参数名\t类型\t参考\t可空\t数组\t备注");
                                    doc->lines.insert(doc->lines.begin() + doc->cursorLine + 2, L"\t整数型\t\t\t\t");
                                    doc->cursorLine += 2;
                                    doc->cursorCol = 0;
                                } else {
                                    // 参数表已存在，在参数表末尾插入新行
                                    int insertPos = paramHeaderLine + 1;
                                    while (insertPos < (int)doc->lines.size()) {
                                        std::wstring& l = doc->lines[insertPos];
                                        if (l.empty() || l.find(L"变量名") == 0 || l.find(L"子程序名") == 0) break;
                                        insertPos++;
                                    }
                                    doc->lines.insert(doc->lines.begin() + insertPos, L"\t整数型\t\t\t\t");
                                    doc->cursorLine = insertPos;
                                    doc->cursorCol = 0;
                                }
                                doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                                InvalidateRect(hWnd, NULL, TRUE);
                                handled = true;
                            }
                        } else if (headerType == L"参数名") {
                            // 在参数表中（表头或数据行）：插入新参数行
                            doc->lines.insert(doc->lines.begin() + doc->cursorLine + 1, L"\t整数型\t\t\t\t");
                            doc->cursorLine++;
                            doc->cursorCol = 0;
                            doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                            InvalidateRect(hWnd, NULL, TRUE);
                            handled = true;
                        } else if (headerType == L"变量名") {
                            // 在变量表中（表头或数据行）：插入新变量行
                            // 检查是否是程序集变量表还是局部变量表
                            bool isAssemblyVar = false;
                            for (int i = headerLine; i >= 0; i--) {
                                if (doc->lines[i].find(L"子程序名") == 0) {
                                    // 在子程序内，是局部变量表
                                    isAssemblyVar = false;
                                    break;
                                }
                                if (doc->lines[i].find(L"程序集名") == 0) {
                                    // 在程序集内，是程序集变量表
                                    isAssemblyVar = true;
                                    break;
                                }
                            }
                            
                            if (isAssemblyVar) {
                                // 程序集变量表（3列：变量名、类型、数组、备注）
                                doc->lines.insert(doc->lines.begin() + doc->cursorLine + 1, L"\t整数型\t\t");
                            } else {
                                // 局部变量表（4列：变量名、类型、静态、数组、备注）
                                doc->lines.insert(doc->lines.begin() + doc->cursorLine + 1, L"\t整数型\t\t\t");
                            }
                            doc->cursorLine++;
                            doc->cursorCol = 0;
                            doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                            InvalidateRect(hWnd, NULL, TRUE);
                            handled = true;
                        }
                    }
                }

                if (!handled && doc->cursorLine < (int)doc->lines.size()) {
                    int oldLine = doc->cursorLine;
                    
                    // 离开当前行前检查并格式化关键词
                    // 注意：此函数可能会修改doc->lines导致重新分配内存，所以不能在此之前持有line的引用
                    CheckAndFormatKeywords(doc, oldLine);
                    
                    // 重新获取当前行引用
                    std::wstring& line = doc->lines[doc->cursorLine];
                    
                    // 使用 GetNestingDepth 获取嵌套深度和标记长度
                    size_t markerLen = 0;
                    int nestingDepth = GetNestingDepth(line, &markerLen);
                    bool isNestedFlowControl = false;
                    
                    if (nestingDepth > 0) {
                        // 检查标记后面是否是流程控制命令
                        std::wstring contentAfterMarker = line.substr(markerLen);
                        if (!contentAfterMarker.empty() && contentAfterMarker[0] == L'.') {
                            isNestedFlowControl = IsFlowControlLine(contentAfterMarker);
                        }
                    }
                    
                    // 检查当前行是否是顶层流程控制命令
                    bool isTopLevelFlowControl = IsFlowControlLine(line);
                    
                    if (isNestedFlowControl) {
                        // 嵌套的流程控制命令行按回车
                        // 继承当前行的标记前缀，增加一层深度
                        std::wstring currentMarker = line.substr(0, markerLen);
                        std::wstring expectedNextMarker = currentMarker + L"\u200B\u200C";
                        
                        // 检查下一行是否已经是正确的缩进行（可能由CheckAndFormatKeywords创建）
                        bool needCreateNewLine = true;
                        if (doc->cursorLine + 1 < (int)doc->lines.size()) {
                            const std::wstring& nextLine = doc->lines[doc->cursorLine + 1];
                            // 检查下一行是否以期望的标记开头
                            if (nextLine.length() >= expectedNextMarker.length() &&
                                nextLine.substr(0, expectedNextMarker.length()) == expectedNextMarker) {
                                // 检查下一行是否是流程控制命令（不是纯内容行）
                                std::wstring nextContent = nextLine.substr(expectedNextMarker.length());
                                bool nextIsFlowCmd = !nextContent.empty() && nextContent[0] == L'.' && IsFlowControlLine(nextContent);
                                if (nextIsFlowCmd) {
                                    // 下一行是更深层的流程控制命令，在它上方插入一行内容行
                                    // needCreateNewLine 保持 true
                                } else {
                                    // 下一行是纯内容行，跳转即可
                                    needCreateNewLine = false;
                                    doc->cursorLine++;
                                    doc->cursorCol = (int)expectedNextMarker.length();
                                }
                            }
                        }
                        
                        if (needCreateNewLine) {
                            // 创建新的缩进行：当前标记 + \u200B + \u200C
                            std::wstring newLine = expectedNextMarker;
                            
                            // 计算新的光标位置（跳过所有标记字符）
                            int newCursorCol = (int)newLine.length();
                            
                            doc->lines.insert(doc->lines.begin() + doc->cursorLine + 1, newLine);
                            while (doc->parametersExpanded.size() <= doc->cursorLine + 1) {
                                doc->parametersExpanded.push_back(false);
                            }
                            doc->parametersExpanded.insert(doc->parametersExpanded.begin() + doc->cursorLine + 1, false);
                            doc->cursorLine++;
                            doc->cursorCol = newCursorCol;
                        }
                        
                        doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                        InvalidateRect(hWnd, NULL, TRUE);
                        return 0;
                    } else if (isTopLevelFlowControl) {
                        // 在顶层流程控制命令行按回车
                        // 检查下一行是否已经是缩进行
                        bool needCreateNewLine = true;
                        if (doc->cursorLine + 1 < (int)doc->lines.size()) {
                            std::wstring& nextLine = doc->lines[doc->cursorLine + 1];
                            if (nextLine.length() > 0 && (nextLine[0] == L'\u200C' || nextLine[0] == L'\u200D')) {
                                // 检查下一行是否是流程控制命令（不是纯内容行）
                                size_t nextMarkerLen = 0;
                                GetNestingDepth(nextLine, &nextMarkerLen);
                                std::wstring nextContent = nextLine.substr(nextMarkerLen);
                                bool nextIsFlowCmd = !nextContent.empty() && nextContent[0] == L'.' && IsFlowControlLine(nextContent);
                                if (nextIsFlowCmd) {
                                    // 下一行是嵌套流程控制命令，在它上方插入内容行
                                    // needCreateNewLine 保持 true
                                } else {
                                    // 下一行是纯内容行，跳转即可
                                    needCreateNewLine = false;
                                    doc->cursorLine++;
                                    size_t skipLen = 0;
                                    GetNestingDepth(nextLine, &skipLen);
                                    doc->cursorCol = (int)skipLen;
                                }
                            }
                        }
                        
                        if (needCreateNewLine) {
                            // 需要创建新的缩进行
                            doc->lines.insert(doc->lines.begin() + doc->cursorLine + 1, L"\u200C");
                            while (doc->parametersExpanded.size() <= doc->cursorLine + 1) {
                                doc->parametersExpanded.push_back(false);
                            }
                            doc->parametersExpanded.insert(doc->parametersExpanded.begin() + doc->cursorLine + 1, false);
                            doc->cursorLine++;
                            doc->cursorCol = 1;
                        }
                        
                        doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                        InvalidateRect(hWnd, NULL, TRUE);
                        return 0;
                    } else {
                        // 普通行，正常处理
                        std::wstring originalNewLine = line.substr(doc->cursorCol);
                        line = line.substr(0, doc->cursorCol);
                        
                        int newCursorCol = 0;
                        std::wstring newLine;
                        
                        // 使用 GetNestingDepth 获取嵌套深度和标记长度
                        size_t markerLen = 0;
                        int nestingDepth = GetNestingDepth(line, &markerLen);
                        
                        if (nestingDepth > 0) {
                            // 在流程块内，新行保持相同的嵌套深度
                            // 直接继承当前行的标记前缀
                            std::wstring currentMarker = line.substr(0, markerLen);
                            
                            // 去掉 originalNewLine 中可能存在的标记字符
                            size_t origMarkerLen = 0;
                            GetNestingDepth(originalNewLine, &origMarkerLen);
                            std::wstring cleanContent = originalNewLine.substr(origMarkerLen);
                            
                            newLine = currentMarker + cleanContent;
                            newCursorCol = (int)currentMarker.length();
                        } else {
                            newLine = originalNewLine;
                        }
                        
                        doc->lines.insert(doc->lines.begin() + doc->cursorLine + 1, newLine);
                        // 同步 parametersExpanded 数组
                        while (doc->parametersExpanded.size() <= doc->cursorLine + 1) {
                            doc->parametersExpanded.push_back(false);
                        }
                        if (doc->parametersExpanded.size() > doc->cursorLine + 1) {
                            doc->parametersExpanded.insert(doc->parametersExpanded.begin() + doc->cursorLine + 1, false);
                        }
                        doc->cursorLine++;
                        doc->cursorCol = newCursorCol;
                    }
                    
                    doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                    InvalidateRect(hWnd, NULL, TRUE);
                }
            } else if (ch == VK_BACK) {  // 退格
                doc->CreateSnapshot(L"Backspace");
                if (doc->hasSelection) {
                    // 直接删除选中内容
                    DeleteSelection(doc);
                    InvalidateRect(hWnd, NULL, TRUE);
                } else if (doc->cursorCol > 0) {
                    std::wstring& line = doc->lines[doc->cursorLine];
                    
                    // 检查是否是流程控制内容行，不能删除标记前缀字符
                    if (line.length() > 0 && (line[0] == L'\u200C' || line[0] == L'\u200D' || line[0] == L'\u200B')) {
                        size_t markerLen = 0;
                        GetNestingDepth(line, &markerLen);
                        if (doc->cursorCol <= (int)markerLen) {
                            return 0;
                        }
                    }
                    
                    // 检查是否是参数行，如果是则不能删除参数名部分
                    if (line.length() > 0 && line[0] == L'\u2060') {
                        size_t colonPos = line.find(L':');
                        if (colonPos == std::wstring::npos) {
                            colonPos = line.length();
                        }
                        int minCursorCol = (int)colonPos + 1;  // 光标最小位置在冒号之后
                        if (doc->cursorCol <= minCursorCol) {
                            // 不允许删除参数名部分，直接返回
                            return 0;
                        }
                    }
                    
                    // 表格行中不允许删除制表符（单元格分隔符）
                    if (line.find(L'\t') != std::wstring::npos && line[doc->cursorCol - 1] == L'\t') {
                        return 0;
                    }
                    
                    wchar_t deletedChar = line[doc->cursorCol - 1];
                    line.erase(doc->cursorCol - 1, 1);
                    doc->cursorCol--;
                    
                    // 如果删除了括号，检查是否是流程命令需要自动补全
                    if (deletedChar == L'(' || deletedChar == L')') {
                        // 检查行是否是流程控制命令
                        if (IsFlowControlLine(line)) {
                            // 去掉点号检查命令
                            std::wstring checkLine = line;
                            size_t dotPos = checkLine.find(L'.');
                            if (dotPos != std::wstring::npos) {
                                checkLine = checkLine.substr(dotPos + 1);
                            }
                            
                            // 提取命令名（去掉括号和空格）
                            size_t nameEnd = 0;
                            while (nameEnd < checkLine.length() && 
                                   checkLine[nameEnd] != L'(' && 
                                   checkLine[nameEnd] != L' ') {
                                nameEnd++;
                            }
                            
                            if (nameEnd > 0) {
                                std::wstring cmdName = checkLine.substr(0, nameEnd);
                                const Keyword* kw = KeywordManager::GetInstance().GetKeyword(cmdName);
                                
                                // 如果是需要括号的流程命令且当前没有括号，自动补全
                                if (kw && kw->needsBrackets && line.find(L'(') == std::wstring::npos) {
                                    line.insert(doc->cursorCol, L"()");
                                    // 光标位置不变，在括号前面
                                }
                            }
                        }
                    }
                    
                    doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                    // 检查是否清空了数据行，需要清理表头
                    CleanupOrphanedHeaders(doc);
                    
                    // 如果是参数行，同步更新命令行括号内的参数值
                    if (line.length() > 0 && line[0] == L'\u2060') {
                        SyncParamLineToCommandLine(doc, doc->cursorLine);
                        SyncAssignParamLineToAssignLine(doc, doc->cursorLine);
                    }
                    
                    // 删除后更新补全提示
                    if (IsInTableRow(doc)) {
                        // 在表格行中
                        if (IsEditingTypeColumn(doc)) {
                            // 在类型列，触发类型补全
                            UpdateTypeCompletion(hWnd, data, doc);
                            data->showCompletion = false;
                        } else {
                            // 表格的其他列，不需要任何补全
                            HideTypeCompletion(data);
                            data->showCompletion = false;
                        }
                    } else if (data->showCompletion) {
                        // 非表格行，更新命令补全（包含控件名称和属性补全）
                        HideTypeCompletion(data);
                        int wordStart;
                        std::wstring currentWord = GetCurrentWord(line, doc->cursorCol, wordStart);
                        
                        // 检查当前单词前是否有点号（正在输入属性名）
                        bool isPropertyInput = false;
                        if (wordStart > 0 && line[wordStart - 1] == L'.') {
                            std::wstring controlName = GetWordBeforeDot(line, wordStart - 1);
                            if (!controlName.empty()) {
                                // 获取该控件的属性补全项并筛选
                                std::vector<CompletionItem> propCompletions = GetControlPropertyCompletions(controlName);
                                if (!propCompletions.empty() && !currentWord.empty()) {
                                    // 筛选匹配当前输入的属性
                                    std::vector<CompletionItem> filteredProps;
                                    std::wstring lowerWord = currentWord;
                                    std::transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::towlower);
                                    
                                    for (auto& prop : propCompletions) {
                                        std::wstring lowerPropName = prop.insertText;
                                        std::transform(lowerPropName.begin(), lowerPropName.end(), lowerPropName.begin(), ::towlower);
                                        
                                        // 中文匹配
                                        if (lowerPropName.find(lowerWord) == 0) {
                                            filteredProps.push_back(prop);
                                        }
                                        // 拼音匹配
                                        else {
                                            std::wstring pinyin = PinyinHelper::GetStringPinyin(prop.insertText);
                                            std::wstring initials = PinyinHelper::GetStringInitials(prop.insertText);
                                            std::transform(pinyin.begin(), pinyin.end(), pinyin.begin(), ::towlower);
                                            std::transform(initials.begin(), initials.end(), initials.begin(), ::towlower);
                                            
                                            if ((!pinyin.empty() && pinyin.find(lowerWord) == 0) ||
                                                (!initials.empty() && initials.find(lowerWord) == 0)) {
                                                filteredProps.push_back(prop);
                                            }
                                        }
                                    }
                                    
                                    if (!filteredProps.empty()) {
                                        data->completionItems = filteredProps;
                                        data->selectedCompletionIndex = 0;
                                        data->completionScrollOffset = 0;
                                        data->currentWord = currentWord;
                                        data->completionMaxVisible = 8;
                                        data->completionItemHeight = 24;
                                        isPropertyInput = true;
                                    } else {
                                        data->showCompletion = false;
                                        isPropertyInput = true;
                                    }
                                } else if (!propCompletions.empty() && currentWord.empty()) {
                                    // 点号后还没有输入，显示所有属性
                                    data->completionItems = propCompletions;
                                    data->selectedCompletionIndex = 0;
                                    data->completionScrollOffset = 0;
                                    data->currentWord = L"";
                                    data->completionMaxVisible = 8;
                                    data->completionItemHeight = 24;
                                    isPropertyInput = true;
                                }
                            }
                        }
                        
                        // 如果不是属性输入，则进行普通命令补全（包含控件名称）
                        if (!isPropertyInput) {
                            if (!currentWord.empty() && currentWord.length() >= 1) {
                                // 获取关键词补全
                                std::vector<CompletionItem> completions = KeywordManager::GetInstance().GetCompletions(currentWord);
                                
                                // 获取可视化设计器控件名称补全
                                std::vector<CompletionItem> ctrlCompletions = GetDesignerControlCompletions(currentWord);
                                
                                // 合并两个列表，控件名称在前
                                for (const auto& ctrl : ctrlCompletions) {
                                    completions.insert(completions.begin(), ctrl);
                                }
                                
                                if (!completions.empty()) {
                                    data->completionItems = completions;
                                    data->selectedCompletionIndex = 0;
                                    data->completionScrollOffset = 0;
                                    data->currentWord = currentWord;
                                    data->completionMaxVisible = 8;
                                    data->completionItemHeight = 24;
                                } else {
                                    data->showCompletion = false;
                                }
                            } else {
                                data->showCompletion = false;
                            }
                        }
                    }
                    
                    InvalidateRect(hWnd, NULL, TRUE);
                } else if (doc->cursorLine > 0) {
                    // 保护前3行不被合并删除
                    if (doc->cursorLine <= 2) return 0;

                    // 检查当前行是否是流程控制区块后的保护空行(且光标在行首)
                    if (doc->cursorCol == 0) {
                        int flowControlLastLine = IsProtectedEmptyLineAfterFlowControl(doc, doc->cursorLine);
                        if (flowControlLastLine >= 0) {
                            // 当前行是流程控制区块后的保护空行，不能删除保护行
                            // 转而处理流程控制内的最后一行
                            std::wstring& targetLine = doc->lines[flowControlLastLine];
                            
                            // 检查目标行是否只有空格标记（空缩进行）
                            bool isEmptyIndentLine = true;
                            for (size_t i = 1; i < targetLine.length(); i++) {
                                if (targetLine[i] != L' ') {
                                    isEmptyIndentLine = false;
                                    break;
                                }
                            }
                            
                            if (isEmptyIndentLine) {
                                // 空缩进行，删除整行
                                doc->lines.erase(doc->lines.begin() + flowControlLastLine);
                                // 光标保持在当前行开头（当前行索引会自动-1）
                                doc->cursorLine--;
                            } else {
                                // 有内容的行，光标移到那行末尾，删除最后一个字符
                                doc->cursorLine = flowControlLastLine;
                                doc->cursorCol = (int)targetLine.length();
                                if (targetLine.length() > 1) {
                                    targetLine.erase(targetLine.length() - 1, 1);
                                    doc->cursorCol--;
                                }
                            }
                            
                            doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                            InvalidateRect(hWnd, NULL, TRUE);
                            return 0;
                        }
                    }

                    // 禁止删除子程序之间的空行
                    // 情况1：当前行是子程序名 -> 禁止向上合并
                    if (doc->lines[doc->cursorLine].find(L"子程序名") == 0) {
                        return 0;
                    }
                    
                    // 情况2：当前行是空行，下一行是子程序名 -> 禁止向上合并（删除空行）
                    if (doc->lines[doc->cursorLine].empty()) {
                        if (doc->cursorLine + 1 < (int)doc->lines.size() && doc->lines[doc->cursorLine + 1].find(L"子程序名") == 0) {
                            return 0;
                        }
                    }

                    // 检查是否在表格内，如果是且上一行不为空，则禁止合并
                    bool isTableLine = false;
                    for (int i = doc->cursorLine; i >= 0; i--) {
                        const std::wstring& l = doc->lines[i];
                        if (l.empty()) break;
                        if (l.find(L"子程序名") == 0 || l.find(L"参数名") == 0 || 
                            l.find(L"变量名") == 0 || l.find(L"程序集名") == 0) {
                            isTableLine = true;
                            break;
                        }
                    }
                    
                    if (isTableLine && !doc->lines[doc->cursorLine - 1].empty()) {
                        return 0;
                    }

                    // 保护子程序中最后一个普通代码行不被删除
                    if (IsOnlyNormalLineInSubroutine(doc, doc->cursorLine)) {
                        return 0;
                    }

                    // 保护流程控制块内唯一的内容行不被删除
                    if (IsOnlyContentLineInFlowBlock(doc, doc->cursorLine)) {
                        return 0;
                    }

                    // 合并到上一行
                    std::wstring currentLine = doc->lines[doc->cursorLine];
                    doc->lines.erase(doc->lines.begin() + doc->cursorLine);
                    doc->cursorLine--;
                    doc->cursorCol = (int)doc->lines[doc->cursorLine].length();
                    doc->lines[doc->cursorLine] += currentLine;
                    doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                    // 检查是否需要清理表头
                    CleanupOrphanedHeaders(doc);
                    InvalidateRect(hWnd, NULL, TRUE);
                }
            } else if (ch == VK_TAB) {  // Tab键
                if (doc->cursorLine < (int)doc->lines.size()) {
                    std::wstring& line = doc->lines[doc->cursorLine];
                    
                    // 检查是否是表格行（包含制表符）
                    bool isTableLine = (line.find(L'\t') != std::wstring::npos);
                    // 检查是否在流程控制内（以空格开头）
                    bool isIndented = (line.length() > 0 && line[0] == L' ');
                    
                    if (isTableLine) {
                        // 表格行：移动光标到下一个单元格（下一个制表符之后）
                        bool shiftPressed = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
                        
                        if (shiftPressed) {
                            // Shift+TAB: 移动到前一个单元格
                            // 找到光标前面最近的制表符
                            size_t prevTab = line.rfind(L'\t', doc->cursorCol > 0 ? doc->cursorCol - 1 : 0);
                            if (prevTab != std::wstring::npos && (int)prevTab < doc->cursorCol) {
                                // 找到了前一个制表符，再找它前面的制表符来确定单元格起始位置
                                size_t prevPrevTab = line.rfind(L'\t', prevTab > 0 ? prevTab - 1 : 0);
                                if (prevPrevTab != std::wstring::npos && prevPrevTab < prevTab) {
                                    doc->cursorCol = (int)prevPrevTab + 1;  // 移动到前前一个制表符之后
                                } else {
                                    doc->cursorCol = 0;  // 移动到行首
                                }
                            } else {
                                doc->cursorCol = 0;  // 移动到行首
                            }
                        } else {
                            // TAB: 移动到下一个单元格
                            size_t nextTab = line.find(L'\t', doc->cursorCol);
                            if (nextTab != std::wstring::npos) {
                                doc->cursorCol = (int)nextTab + 1;  // 移动到制表符之后
                            } else {
                                // 没有下一个制表符，移动到行尾
                                doc->cursorCol = (int)line.length();
                            }
                        }
                        InvalidateRect(hWnd, NULL, TRUE);
                    } else if (isIndented) {
                        // 流程控制内：插入单个空格（保持缩进标记）
                        line.insert(doc->cursorCol, L" ");
                        doc->cursorCol++;
                        doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                        InvalidateRect(hWnd, NULL, TRUE);
                    } else {
                        // 普通代码行：插入4个空格
                        line.insert(doc->cursorCol, L"    ");
                        doc->cursorCol += 4;
                        doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                }
            }
            UpdateScrollBars(hWnd);
        }
        return 0;
        
    case WM_KEYDOWN:
        {
            EditorData* data = (EditorData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (!data) return 0;
            
            // 如果正在显示欢迎页，某些按键创建新文档
            if (data->showWelcomePage) {
                // 只有非功能键才创建新文档
                if (wParam != VK_SHIFT && wParam != VK_CONTROL && wParam != VK_MENU &&
                    wParam != VK_LWIN && wParam != VK_RWIN && wParam != VK_ESCAPE) {
                    data->AddDocument(L"");
                    InvalidateRect(hWnd, NULL, TRUE);
                }
                return 0;
            }
            
            EditorDocument* doc = data->GetActiveDoc();
            if (!doc) return 0;
            
            // 如果类型补全窗口正在显示，优先处理
            if (data->showTypeCompletion && !data->typeCompletionItems.empty()) {
                switch (wParam) {
                case VK_UP:
                    if (data->typeCompletionSelectedIndex > 0) {
                        data->typeCompletionSelectedIndex--;
                        // 确保选中项可见
                        if (data->typeCompletionSelectedIndex < data->typeCompletionScrollOffset) {
                            data->typeCompletionScrollOffset = data->typeCompletionSelectedIndex;
                        }
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                    return 0;
                case VK_DOWN:
                    if (data->typeCompletionSelectedIndex < (int)data->typeCompletionItems.size() - 1) {
                        data->typeCompletionSelectedIndex++;
                        // 确保选中项可见
                        if (data->typeCompletionSelectedIndex >= data->typeCompletionScrollOffset + TYPE_COMPLETION_MAX_VISIBLE) {
                            data->typeCompletionScrollOffset = data->typeCompletionSelectedIndex - TYPE_COMPLETION_MAX_VISIBLE + 1;
                        }
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                    return 0;
                case VK_TAB:
                case VK_RETURN:
                    ApplyTypeCompletion(hWnd, data, doc);
                    return 0;
                case VK_SPACE:
                    ApplyTypeCompletion(hWnd, data, doc);
                    data->skipNextSpaceForType = true;  // 标记跳过下一个空格
                    return 0;
                case VK_ESCAPE:
                    HideTypeCompletion(data);
                    InvalidateRect(hWnd, NULL, TRUE);
                    return 0;
                }
            }
            
            // 如果自动完成弹窗正在显示，处理特殊键
            if (data->showCompletion && !data->completionItems.empty()) {
                switch (wParam) {
                case VK_UP:
                    if (data->selectedCompletionIndex > 0) {
                        data->selectedCompletionIndex--;
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                    return 0;
                case VK_DOWN:
                    if (data->selectedCompletionIndex < (int)data->completionItems.size() - 1) {
                        data->selectedCompletionIndex++;
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                    return 0;
                case VK_TAB:
                case VK_RETURN:
                    ApplyCompletion(hWnd, data, doc);
                    data->skipNextChar = true;
                    return 0;
                case VK_SPACE:
                    ApplyCompletion(hWnd, data, doc);
                    data->skipNextChar = true;
                    return 0;
                case VK_ESCAPE:
                    data->showCompletion = false;
                    InvalidateRect(hWnd, NULL, TRUE);
                    return 0;
                }
            }
            
            switch (wParam) {
            case 'Z':
                if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
                    if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                        doc->Redo();
                    } else {
                        doc->Undo();
                    }
                    InvalidateRect(hWnd, NULL, TRUE);
                    return 0;
                }
                break;
            case 'Y':
                if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
                    doc->Redo();
                    InvalidateRect(hWnd, NULL, TRUE);
                    return 0;
                }
                break;
            case 'A':
                if (GetKeyState(VK_CONTROL) < 0) {
                    SelectAll(doc);
                    InvalidateRect(hWnd, NULL, TRUE);
                    return 0;
                }
                break;
            case 'C':
                if (GetKeyState(VK_CONTROL) < 0) {
                    CopyToClipboard(hWnd, doc);
                    return 0;
                }
                break;
            case 'V':
                if (GetKeyState(VK_CONTROL) < 0) {
                    doc->CreateSnapshot(L"Paste");
                    PasteFromClipboard(hWnd, doc);
                    InvalidateRect(hWnd, NULL, TRUE);
                    return 0;
                }
                break;
            case 'L':
                if (GetKeyState(VK_CONTROL) < 0) {
                    doc->CreateSnapshot(L"Insert Var Table");
                    // 查找当前子程序范围
                    int subStart = -1;
                    // 向上查找子程序头
                    for (int i = doc->cursorLine; i >= 0; i--) {
                        if (doc->lines[i].find(L"子程序名") == 0) {
                            subStart = i;
                            break;
                        }
                        if (doc->lines[i].find(L"程序集名") == 0) break;
                    }
                    
                    if (subStart != -1) {
                        // 检查是否已经存在变量表
                        bool foundVarTable = false;
                        int insertPos = subStart + 1;
                        
                        // 向下扫描直到下一个子程序或程序集或文件结束
                        for (int i = subStart + 1; i < (int)doc->lines.size(); i++) {
                            std::wstring& l = doc->lines[i];
                            if (l.find(L"子程序名") == 0 || l.find(L"程序集名") == 0) break;
                            if (l.find(L"变量名") == 0) {
                                foundVarTable = true;
                                break;
                            }
                        }
                        
                        if (!foundVarTable) {
                            // 确定插入位置：在子程序表和参数表之后
                            insertPos = subStart + 1;
                            while (insertPos < (int)doc->lines.size()) {
                                std::wstring& l = doc->lines[insertPos];
                                if (l.empty()) break;
                                if (l.find(L"子程序名") == 0 || l.find(L"程序集名") == 0) break;
                                
                                // 如果遇到参数表，跳过它
                                if (l.find(L"参数名") == 0) {
                                    insertPos++;
                                    // 跳过参数行
                                    while (insertPos < (int)doc->lines.size()) {
                                        std::wstring& pl = doc->lines[insertPos];
                                        if (pl.empty()) break;
                                        if (pl.find(L"变量名") == 0) break; // 不应该发生，因为前面检查过了
                                        // 简单的启发式：如果行不包含制表符，可能是代码，停止
                                        if (pl.find(L"\t") == std::wstring::npos) break;
                                        insertPos++;
                                    }
                                    break;
                                }
                                
                                // 如果是子程序表行（紧跟在子程序名后）
                                if (insertPos == subStart + 1) {
                                    insertPos++;
                                    continue;
                                }
                                
                                // 如果遇到非表格行（代码），停止
                                if (l.find(L"\t") == std::wstring::npos) break;
                                
                                insertPos++;
                            }
                            
                            // 插入变量表
                            doc->lines.insert(doc->lines.begin() + insertPos, L"变量名\t类型\t静态\t数组\t备注");
                            doc->lines.insert(doc->lines.begin() + insertPos + 1, L"\t整数型\t\t\t");
                            doc->cursorLine = insertPos + 1;
                            doc->cursorCol = 0;
                            doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                            InvalidateRect(hWnd, NULL, TRUE);
                        }
                    }
                    return 0;
                }
                break;
            case 'X':
                if (GetKeyState(VK_CONTROL) < 0) {
                    CutToClipboard(hWnd, doc);
                    InvalidateRect(hWnd, NULL, TRUE);
                    return 0;
                }
                break;
            case VK_LEFT:  // 左箭头
                if (doc->cursorCol > 0) {
                    doc->cursorCol--;
                    // 检查是否是参数行，如果光标移动到了参数名区域则停在冒号后
                    if (doc->cursorLine < (int)doc->lines.size()) {
                        const std::wstring& line = doc->lines[doc->cursorLine];
                        if (line.length() > 0 && line[0] == L'\u2060') {
                            size_t colonPos = line.find(L':');
                            if (colonPos != std::wstring::npos) {
                                int minCursorCol = (int)colonPos + 1;
                                if (doc->cursorCol < minCursorCol) {
                                    doc->cursorCol = minCursorCol;
                                }
                            }
                        }
                    }
                } else if (doc->cursorLine > 0) {
                    doc->cursorLine--;
                    doc->cursorCol = (int)doc->lines[doc->cursorLine].length();
                }
                InvalidateRect(hWnd, NULL, TRUE);
                break;
                
            case VK_RIGHT:  // 右箭头
                if (doc->cursorLine < (int)doc->lines.size()) {
                    if (doc->cursorCol < (int)doc->lines[doc->cursorLine].length()) {
                        doc->cursorCol++;
                    } else if (doc->cursorLine < (int)doc->lines.size() - 1) {
                        doc->cursorLine++;
                        doc->cursorCol = 0;
                        // 如果新行是参数行，将光标移动到冒号后
                        const std::wstring& newLine = doc->lines[doc->cursorLine];
                        if (newLine.length() > 0 && newLine[0] == L'\u2060') {
                            size_t colonPos = newLine.find(L':');
                            if (colonPos != std::wstring::npos) {
                                doc->cursorCol = (int)colonPos + 1;
                            }
                        }
                    }
                }
                InvalidateRect(hWnd, NULL, TRUE);
                break;
                
            case VK_UP:  // 上箭头
                if (doc->cursorLine > 0) {
                    int oldLine = doc->cursorLine;
                    doc->cursorLine--;
                    
                    if (doc->cursorCol > (int)doc->lines[doc->cursorLine].length()) {
                        doc->cursorCol = (int)doc->lines[doc->cursorLine].length();
                    }
                    // 如果新行是参数行，确保光标在冒号后
                    const std::wstring& newLine = doc->lines[doc->cursorLine];
                    if (newLine.length() > 0 && newLine[0] == L'\u2060') {
                        size_t colonPos = newLine.find(L':');
                        if (colonPos != std::wstring::npos) {
                            int minCursorCol = (int)colonPos + 1;
                            if (doc->cursorCol < minCursorCol) {
                                doc->cursorCol = minCursorCol;
                            }
                        }
                    }
                    // 离开旧行时检查并格式化
                    CheckAndFormatKeywords(doc, oldLine);
                }
                InvalidateRect(hWnd, NULL, TRUE);
                break;
                
            case VK_DOWN:  // 下箭头
                if (doc->cursorLine < (int)doc->lines.size() - 1) {
                    int oldLine = doc->cursorLine;
                    doc->cursorLine++;
                    
                    if (doc->cursorCol > (int)doc->lines[doc->cursorLine].length()) {
                        doc->cursorCol = (int)doc->lines[doc->cursorLine].length();
                    }
                    // 如果新行是参数行，确保光标在冒号后
                    const std::wstring& newLine = doc->lines[doc->cursorLine];
                    if (newLine.length() > 0 && newLine[0] == L'\u2060') {
                        size_t colonPos = newLine.find(L':');
                        if (colonPos != std::wstring::npos) {
                            int minCursorCol = (int)colonPos + 1;
                            if (doc->cursorCol < minCursorCol) {
                                doc->cursorCol = minCursorCol;
                            }
                        }
                    }
                    // 离开旧行时检查并格式化
                    CheckAndFormatKeywords(doc, oldLine);
                }
                InvalidateRect(hWnd, NULL, TRUE);
                break;
                
            case VK_DELETE:  // Delete键
                doc->CreateSnapshot(L"Delete");
                if (doc->hasSelection) {
                    // 直接删除选中内容
                    DeleteSelection(doc);
                    InvalidateRect(hWnd, NULL, TRUE);
                } else if (doc->cursorLine < (int)doc->lines.size()) {
                    std::wstring& line = doc->lines[doc->cursorLine];
                    
                    // 检查是否是参数行，如果是则不能删除参数名部分
                    if (line.length() > 0 && line[0] == L'\u2060') {
                        size_t colonPos = line.find(L':');
                        if (colonPos == std::wstring::npos) {
                            colonPos = line.length();
                        }
                        int minCursorCol = (int)colonPos + 1;  // 光标最小位置在冒号之后
                        if (doc->cursorCol < minCursorCol) {
                            // 不允许在参数名区域删除，直接返回
                            return 0;
                        }
                    }
                    
                    // 保护流程控制内容行的标记前缀不被删除
                    if (line.length() > 0 && (line[0] == L'\u200C' || line[0] == L'\u200D' || line[0] == L'\u200B')) {
                        size_t markerLen = 0;
                        GetNestingDepth(line, &markerLen);
                        if (doc->cursorCol < (int)markerLen) {
                            return 0;
                        }
                    }
                    
                    if (doc->cursorCol < (int)line.length()) {
                        // 表格行中不允许删除制表符（单元格分隔符）
                        if (line.find(L'\t') != std::wstring::npos && line[doc->cursorCol] == L'\t') {
                            return 0;
                        }
                        
                        wchar_t deletedChar = line[doc->cursorCol];
                        line.erase(doc->cursorCol, 1);
                        
                        // 如果删除了括号，检查是否是流程命令需要自动补全
                        if (deletedChar == L'(' || deletedChar == L')') {
                            // 检查行是否是流程控制命令
                            if (IsFlowControlLine(line)) {
                                // 去掉点号检查命令
                                std::wstring checkLine = line;
                                size_t dotPos = checkLine.find(L'.');
                                if (dotPos != std::wstring::npos) {
                                    checkLine = checkLine.substr(dotPos + 1);
                                }
                                
                                // 提取命令名（去掉括号和空格）
                                size_t nameEnd = 0;
                                while (nameEnd < checkLine.length() && 
                                       checkLine[nameEnd] != L'(' && 
                                       checkLine[nameEnd] != L' ') {
                                    nameEnd++;
                                }
                                
                                if (nameEnd > 0) {
                                    std::wstring cmdName = checkLine.substr(0, nameEnd);
                                    const Keyword* kw = KeywordManager::GetInstance().GetKeyword(cmdName);
                                    
                                    // 如果是需要括号的流程命令且当前没有括号，自动补全
                                    if (kw && kw->needsBrackets && line.find(L'(') == std::wstring::npos) {
                                        line.insert(doc->cursorCol, L"()");
                                        // 光标位置不变，在括号前面
                                    }
                                }
                            }
                        }
                        
                        doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                        // 检查是否清空了数据行，需要清理表头
                        CleanupOrphanedHeaders(doc);
                        
                        // 如果是参数行，同步更新命令行括号内的参数值
                        if (line.length() > 0 && line[0] == L'\u2060') {
                            SyncParamLineToCommandLine(doc, doc->cursorLine);
                            SyncAssignParamLineToAssignLine(doc, doc->cursorLine);
                        }
                        
                        // 删除后更新补全提示
                        if (IsInTableRow(doc)) {
                            // 在表格行中
                            if (IsEditingTypeColumn(doc)) {
                                // 在类型列，触发类型补全
                                UpdateTypeCompletion(hWnd, data, doc);
                                data->showCompletion = false;
                            } else {
                                // 表格的其他列，不需要任何补全
                                HideTypeCompletion(data);
                                data->showCompletion = false;
                            }
                        } else {
                            // 非表格行，隐藏类型补全
                            HideTypeCompletion(data);
                        }
                        
                        InvalidateRect(hWnd, NULL, TRUE);
                    } else if (doc->cursorLine < (int)doc->lines.size() - 1) {
                        // 保护前2行不被合并（即不能删除第1行和第2行）
                        // 如果在第0行末尾按Delete，会合并第1行 -> 禁止
                        // 如果在第1行末尾按Delete，会合并第2行 -> 禁止
                        if (doc->cursorLine < 2) return 0;

                        // 检查是否是流程控制区块后的保护空行
                        int flowControlLastLine = IsProtectedEmptyLineAfterFlowControl(doc, doc->cursorLine + 1);
                        if (flowControlLastLine >= 0) {
                            // 当前行末尾，下一行是流程控制区块后的保护空行
                            // 不删除保护空行，而是处理流程控制内的内容
                            std::wstring& targetLine = doc->lines[flowControlLastLine];
                            
                            // 检查目标行是否只有空格标记（空缩进行）
                            bool isEmptyIndentLine = true;
                            for (size_t i = 1; i < targetLine.length(); i++) {
                                if (targetLine[i] != L' ') {
                                    isEmptyIndentLine = false;
                                    break;
                                }
                            }
                            
                            if (isEmptyIndentLine) {
                                // 删除整个空缩进行
                                doc->lines.erase(doc->lines.begin() + flowControlLastLine);
                                // 光标位置不变
                            } else {
                                // 删除最后一个字符
                                if (targetLine.length() > 1) {
                                    targetLine.erase(targetLine.length() - 1, 1);
                                }
                                // 光标移动到那一行的末尾
                                doc->cursorLine = flowControlLastLine;
                                doc->cursorCol = (int)targetLine.length();
                            }
                            
                            doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                            InvalidateRect(hWnd, NULL, TRUE);
                            return 0;
                        }

                        // 禁止删除子程序之间的空行（防止合并）
                        // 情况1：当前行是空行，下一行是子程序名 -> 禁止删除当前行（向上合并）
                        if (doc->lines[doc->cursorLine].empty() && doc->lines[doc->cursorLine + 1].find(L"子程序名") == 0) {
                            return 0;
                        }
                        
                        // 情况2：当前行非空，下一行是空行，下下行是子程序名 -> 禁止删除下一行
                        if (doc->lines[doc->cursorLine + 1].empty()) {
                            if (doc->cursorLine + 2 < (int)doc->lines.size() && doc->lines[doc->cursorLine + 2].find(L"子程序名") == 0) {
                                return 0;
                            }
                        }
                        
                        // 情况3：直接合并到子程序名行
                        if (doc->lines[doc->cursorLine + 1].find(L"子程序名") == 0) {
                            return 0;
                        }

                        // 保护子程序中最后一个普通代码行不被删除
                        if (IsOnlyNormalLineInSubroutine(doc, doc->cursorLine + 1)) {
                            return 0;
                        }

                        // 保护流程控制块内唯一的内容行不被删除
                        if (IsOnlyContentLineInFlowBlock(doc, doc->cursorLine + 1)) {
                            return 0;
                        }

                        // 合并下一行
                        doc->lines[doc->cursorLine] += doc->lines[doc->cursorLine + 1];
                        doc->lines.erase(doc->lines.begin() + doc->cursorLine + 1);
                        doc->modified = true;
                doc->MarkContentDirty();
                doc->MarkContentDirty();
                        // 检查是否需要清理表头
                        CleanupOrphanedHeaders(doc);
                    }
                    InvalidateRect(hWnd, NULL, TRUE);
                }
                break;
                
            case VK_HOME:  // Home键
                {
                    doc->cursorCol = 0;
                    // 如果是参数行，确保光标在冒号后
                    if (doc->cursorLine < (int)doc->lines.size()) {
                        const std::wstring& line = doc->lines[doc->cursorLine];
                        if (line.length() > 0 && line[0] == L'\u2060') {
                            size_t colonPos = line.find(L':');
                            if (colonPos != std::wstring::npos) {
                                doc->cursorCol = (int)colonPos + 1;
                            }
                        }
                    }
                }
                InvalidateRect(hWnd, NULL, TRUE);
                break;
                
            case VK_END:  // End键
                if (doc->cursorLine < (int)doc->lines.size()) {
                    doc->cursorCol = (int)doc->lines[doc->cursorLine].length();
                }
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            }
            UpdateScrollBars(hWnd);
        }
        return 0;
        
    case WM_ERASEBKGND:
        // 阻止默认的背景擦除，由WM_PAINT中的双缓冲统一处理
        return 1;
        
    case WM_SETFOCUS:
        {
            EditorData* data = (EditorData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (data && !data->showWelcomePage) {
                EditorDocument* doc = data->GetActiveDoc();
                // 只有在用户有过交互后才显示光标
                if (doc && doc->hasUserInteraction) {
                    // 根据字体大小计算光标高度
                    int caretHeight = data->fontSize + 4;  // 字体大小 + 4像素边距
                    CreateCaret(hWnd, NULL, 1, caretHeight);
                    ShowCaret(hWnd);
                }
            }
        }
        return 0;
        
    case WM_KILLFOCUS:
        // 隐藏并销毁光标
        HideCaret(hWnd);
        DestroyCaret();
        return 0;
        
    case WM_SIZE:
        // 拖拽输出面板时跳过昂贵的滚动条重算（搜索最大行宽需遍历所有行）
        {
            extern bool g_IsDraggingOutputSplitter;
            if (!g_IsDraggingOutputSplitter) {
                UpdateScrollBars(hWnd);
            }
        }
        break;

    case WM_MOUSEWHEEL:
        {
            EditorData* data = (EditorData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (!data) return 0;
            
            int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            int lines = delta / WHEEL_DELTA;
            
            // 获取鼠标位置
            POINT pt;
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            ScreenToClient(hWnd, &pt);
            
            // 如果类型补全窗口打开，检查鼠标是否在补全窗口内
            if (data->showTypeCompletion && !data->typeCompletionItems.empty()) {
                if (PtInRect(&data->typeCompletionRect, pt)) {
                    // 在类型补全窗口内滚动，滚动补全列表
                    int scrollLines = lines > 0 ? -1 : 1;  // 向上滚动减少偏移
                    int newOffset = data->typeCompletionScrollOffset + scrollLines;
                    int maxOffset = std::max(0, (int)data->typeCompletionItems.size() - TYPE_COMPLETION_MAX_VISIBLE);
                    
                    newOffset = std::max(0, std::min(newOffset, maxOffset));
                    
                    if (newOffset != data->typeCompletionScrollOffset) {
                        data->typeCompletionScrollOffset = newOffset;
                        
                        // 确保选中项在可见范围内
                        if (data->typeCompletionSelectedIndex < data->typeCompletionScrollOffset) {
                            data->typeCompletionSelectedIndex = data->typeCompletionScrollOffset;
                        } else if (data->typeCompletionSelectedIndex >= data->typeCompletionScrollOffset + TYPE_COMPLETION_MAX_VISIBLE) {
                            data->typeCompletionSelectedIndex = data->typeCompletionScrollOffset + TYPE_COMPLETION_MAX_VISIBLE - 1;
                        }
                        
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                    return 0;
                } else {
                    // 鼠标不在类型补全窗口内，隐藏补全窗口
                    HideTypeCompletion(data);
                }
            }
            
            // 如果自动补全窗口打开，优先处理补全列表滚动
            if (data->showCompletion && !data->completionItems.empty()) {
                // 改为通过上下键的方式移动选中项，这样能同时处理滚动
                if (lines > 0) {
                    // 向上滚动（选中上一项）
                    for (int i = 0; i < lines; i++) {
                        if (data->selectedCompletionIndex > 0) {
                            data->selectedCompletionIndex--;
                        }
                    }
                } else if (lines < 0) {
                    // 向下滚动（选中下一项）
                    for (int i = 0; i < -lines; i++) {
                        if (data->selectedCompletionIndex < (int)data->completionItems.size() - 1) {
                            data->selectedCompletionIndex++;
                        }
                    }
                }
                
                // 确保选中项可见（自动调整scrollOffset）
                if (data->selectedCompletionIndex < data->completionScrollOffset) {
                    data->completionScrollOffset = data->selectedCompletionIndex;
                }
                if (data->selectedCompletionIndex >= data->completionScrollOffset + data->completionMaxVisible) {
                    data->completionScrollOffset = data->selectedCompletionIndex - data->completionMaxVisible + 1;
                }
                
                InvalidateRect(hWnd, NULL, TRUE);
                return 0;
            }
            
            EditorDocument* doc = data->GetActiveDoc();
            if (!doc) return 0;

            int scrollAmount = -lines * 3 * data->rowHeight;

            int newPos = doc->scrollY + scrollAmount;
            
            // 计算最大滚动值
            RECT rect;
            GetClientRect(hWnd, &rect);
            int clientHeight = rect.bottom - data->tabHeight;
            int totalLines = (int)doc->lines.size();
            if (!doc->lines.empty() && doc->lines[0].find(L"版本\t") == 0) {
                totalLines--;
            }
            int totalHeight = totalLines * data->rowHeight + 100;
            int maxScroll = totalHeight - clientHeight;

            if (newPos < 0) newPos = 0;
            if (newPos > maxScroll) newPos = maxScroll;
            if (maxScroll < 0) newPos = 0;

            if (newPos != doc->scrollY) {
                doc->scrollY = newPos;
                UpdateScrollBars(hWnd);
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        return 0;

    case WM_PAINT:
        {
            EditorData* data = (EditorData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (!data) return DefWindowProc(hWnd, message, wParam, lParam);
            EditorDocument* doc = data->GetActiveDoc();
            
            PAINTSTRUCT ps;
            HDC hdcWindow = BeginPaint(hWnd, &ps);
            
            // 声明光标位置变量供自动完成弹窗使用
            int caretX = 0;
            int caretY = 0;
            RECT rect;
            GetClientRect(hWnd, &rect);
            
            // 创建双缓冲
            HDC hdc = CreateCompatibleDC(hdcWindow);
            HBITMAP hBitmap = CreateCompatibleBitmap(hdcWindow, rect.right, rect.bottom);
            HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdc, hBitmap);
            
            // 字体定义 - 使用EditorData中的字体大小
            HFONT hFont = CreateFont(data->fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 
                                   GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
                                   DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Consolas");
            HFONT hBoldFont = CreateFont(data->fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                        GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Consolas");
            
            HGDIOBJ hOldFont = SelectObject(hdc, hFont);
            SetBkMode(hdc, TRANSPARENT);
            
            // 绘制背景（使用代码编辑器专用背景色）
            HBRUSH hBgBrush = CreateSolidBrush(g_CurrentTheme.editorBg);
            FillRect(hdc, &rect, hBgBrush);
            DeleteObject(hBgBrush);

            // 如果显示欢迎页
            if (data->showWelcomePage) {
                // 欢迎页不需要文本光标，确保销毁
                HideCaret(hWnd);
                DestroyCaret();
                
                // 绘制欢迎页
                HFONT hWelcomeFont = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                    DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
                HFONT hSubFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                    DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
                HFONT hLinkFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                    DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
                
                int centerX = rect.right / 2;
                int centerY = rect.bottom / 2 - 80;
                
                // 绘制标题
                SelectObject(hdc, hWelcomeFont);
                SetTextColor(hdc, g_CurrentTheme.text);
                RECT titleRect = {0, centerY, rect.right, centerY + 50};
                DrawTextW(hdc, L"ycIDE", -1, &titleRect, DT_CENTER | DT_SINGLELINE);
                
                // 绘制副标题
                SelectObject(hdc, hSubFont);
                SetTextColor(hdc, g_CurrentTheme.textDim);
                RECT subRect = {0, centerY + 50, rect.right, centerY + 80};
                DrawTextW(hdc, L"易承语言集成开发环境", -1, &subRect, DT_CENTER | DT_SINGLELINE);
                
                // 绘制快捷操作提示
                SelectObject(hdc, hLinkFont);
                SetTextColor(hdc, g_CurrentTheme.syntaxKeyword);
                
                int linkY = centerY + 120;
                RECT linkRect1 = {0, linkY, rect.right, linkY + 25};
                DrawTextW(hdc, L"新建文件 (Ctrl+N)", -1, &linkRect1, DT_CENTER | DT_SINGLELINE);
                
                RECT linkRect2 = {0, linkY + 30, rect.right, linkY + 55};
                DrawTextW(hdc, L"打开文件 (Ctrl+O)", -1, &linkRect2, DT_CENTER | DT_SINGLELINE);
                
                RECT linkRect3 = {0, linkY + 60, rect.right, linkY + 85};
                DrawTextW(hdc, L"打开文件夹", -1, &linkRect3, DT_CENTER | DT_SINGLELINE);
                
                // 绘制版本信息
                SetTextColor(hdc, g_CurrentTheme.textDim);
                RECT versionRect = {0, rect.bottom - 40, rect.right, rect.bottom - 20};
                DrawTextW(hdc, L"Version 1.0.0", -1, &versionRect, DT_CENTER | DT_SINGLELINE);
                
                DeleteObject(hWelcomeFont);
                DeleteObject(hSubFont);
                DeleteObject(hLinkFont);
                
                // 欢迎页绘制完成，跳过正常编辑器绘制
            } else {
            // 正常编辑器绘制开始（在else块中）
            
            // 注意：内部标签栏已移除，使用外部TabBar组件
            
            // 绘制顶部边框线
            HPEN hTopBorderPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.grid);
            HPEN hOldTopBorderPen = (HPEN)SelectObject(hdc, hTopBorderPen);
            MoveToEx(hdc, 0, 0, NULL);
            LineTo(hdc, rect.right, 0);
            SelectObject(hdc, hOldTopBorderPen);
            DeleteObject(hTopBorderPen);

            if (!doc) {
                BitBlt(hdcWindow, 0, 0, rect.right, rect.bottom, hdc, 0, 0, SRCCOPY);
                SelectObject(hdc, hOldFont);
                DeleteObject(hFont);
                DeleteObject(hBoldFont);
                SelectObject(hdc, hOldBitmap);
                DeleteObject(hBitmap);
                DeleteDC(hdc);
                EndPaint(hWnd, &ps);
                return 0;
            }
            
            // 绘制左侧行号区背景
            RECT rcLineNum = rect;
            rcLineNum.top = data->tabHeight;
            rcLineNum.right = 70;  // 加宽行号区域以容纳+/-符号
            HBRUSH hLineNumBrush = CreateSolidBrush(g_CurrentTheme.margin);
            FillRect(hdc, &rcLineNum, hLineNumBrush);
            DeleteObject(hLineNumBrush);
            
            // 绘制行号分割线
            HPEN hBorderPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.grid);
            HGDIOBJ hOldPen = SelectObject(hdc, hBorderPen);
            MoveToEx(hdc, 70, data->tabHeight, NULL);  // 同步更新分割线位置
            LineTo(hdc, 70, rect.bottom);
            SelectObject(hdc, hOldPen);
            DeleteObject(hBorderPen);
            
            // 设置裁剪区域以排除标签栏
            HRGN hRgn = CreateRectRgn(0, data->tabHeight, rect.right, rect.bottom);
            SelectClipRgn(hdc, hRgn);
            DeleteObject(hRgn);

            // 绘制参数 - 使用EditorData中的配置
            int rowHeight = data->rowHeight;
            int fontSize = data->fontSize;
            int startX = 75 - doc->scrollX;  // 更新起始X坐标以匹配新的行号区域
            int currentY = 5 + data->tabHeight - doc->scrollY;
            int lineNum = 1;
            
            // 颜色定义 - 使用主题变量
            COLORREF clrText = g_CurrentTheme.text;
            COLORREF clrKeyword = g_CurrentTheme.syntaxKeyword;  // 流程块文本颜色
            COLORREF clrComment = g_CurrentTheme.syntaxComment;
            COLORREF clrLineNum = g_CurrentTheme.lineNumText;  // 行号颜色
            COLORREF clrGridLine = g_CurrentTheme.tableGridLine;  // 表格框线颜色
            COLORREF clrSubHeaderBg = g_CurrentTheme.tableSubHeaderBg;  // 子程序表头背景色
            COLORREF clrVarHeaderBg = g_CurrentTheme.tableVarHeaderBg;  // 变量表头背景色
            
            // Lambda 函数：绘制行号（带流程控制符号）
            auto DrawLineNum = [&](int y, bool skipNumber = false, int currentLineIndex = -1) {
                if (skipNumber) {
                    lineNum++;
                    return;
                }
                WCHAR szNum[10];
                wsprintf(szNum, L"%d", lineNum++);
                SetTextColor(hdc, clrLineNum);
                SelectObject(hdc, hFont);
                
                SIZE textSize;
                GetTextExtentPoint32W(hdc, szNum, lstrlenW(szNum), &textSize);
                int textX = 52 - textSize.cx;  // 行号右对齐在符号左侧（符号在x=56处）
                int textY = y + (rowHeight - fontSize) / 2;  // 使用fontSize计算垂直居中
                
                TextOutW(hdc, textX, textY, szNum, lstrlenW(szNum));
                
                // 如果是有参数的命令行，在行号右侧（x=62）绘制展开/折叠符号
                if (currentLineIndex >= 0 && currentLineIndex < (int)doc->lines.size()) {
                    const std::wstring& line = doc->lines[currentLineIndex];
                    bool isCommandLine = IsFlowControlLine(line);
                    
                    // 检查是否有英文或中文括号
                    if (!isCommandLine && (line.find(L'(') != std::wstring::npos || line.find(L'（') != std::wstring::npos)) {
                        isCommandLine = true;
                    }
                    
                    // 如果没有括号，检查是否是支持库命令且有参数
                    if (!isCommandLine) {
                        // 提取命令名（跳过嵌套标记字符）
                        size_t cmdStart = 0;
                        while (cmdStart < line.length() && (line[cmdStart] == L' ' || line[cmdStart] == L'\t' || line[cmdStart] == L'\u200C' || line[cmdStart] == L'\u200D' || line[cmdStart] == L'\u200B')) cmdStart++;
                        if (cmdStart < line.length() && line[cmdStart] == L'.') cmdStart++;
                        while (cmdStart < line.length() && (line[cmdStart] == L' ' || line[cmdStart] == L'\t')) cmdStart++;
                        
                        size_t cmdEnd = cmdStart;
                        while (cmdEnd < line.length() && line[cmdEnd] != L' ' && line[cmdEnd] != L'\t' && 
                               line[cmdEnd] != L'(' && line[cmdEnd] != L'（') {
                            cmdEnd++;
                        }
                        
                        if (cmdEnd > cmdStart) {
                            std::wstring cmdName = line.substr(cmdStart, cmdEnd - cmdStart);
                            const LibraryCommand* libCmd = LibraryParser::GetInstance().FindCommand(cmdName);
                            if (libCmd && !libCmd->parameters.empty()) {
                                isCommandLine = true;
                            }
                        }
                    }
                    
                    if (isCommandLine) {
                        // 使用 ExtractCommandName 提取命令名（会正确处理空格和点号）
                        std::wstring cmdName = ExtractCommandName(line);
                        
                        // 获取命令参数
                        std::vector<LibraryParameter> params = GetCommandParameters(cmdName);
                        
                        // 只有当命令有参数时才绘制展开/折叠符号
                        if (!params.empty()) {
                            while (doc->parametersExpanded.size() <= currentLineIndex) {
                                doc->parametersExpanded.push_back(false);
                            }
                            bool isExpanded = doc->parametersExpanded[currentLineIndex];
                            
                            // 绘制与DLL编辑器一致的方框样式的展开/折叠符号
                            int foldBtnSize = 10;  // 方框大小
                            int foldBtnX = 58;     // 方框X位置
                            int foldBtnY = y + (rowHeight - foldBtnSize) / 2;  // 垂直居中
                            
                            // 绘制方框边框
                            HPEN hFoldPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.textDim);
                            HPEN hOldFoldPen = (HPEN)SelectObject(hdc, hFoldPen);
                            HBRUSH hOldFoldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
                            Rectangle(hdc, foldBtnX, foldBtnY, foldBtnX + foldBtnSize, foldBtnY + foldBtnSize);
                            
                            // 绘制横线（-）
                            MoveToEx(hdc, foldBtnX + 2, foldBtnY + foldBtnSize / 2, NULL);
                            LineTo(hdc, foldBtnX + foldBtnSize - 2, foldBtnY + foldBtnSize / 2);
                            
                            // 如果是折叠状态（未展开），绘制竖线形成（+）
                            if (!isExpanded) {
                                MoveToEx(hdc, foldBtnX + foldBtnSize / 2, foldBtnY + 2, NULL);
                                LineTo(hdc, foldBtnX + foldBtnSize / 2, foldBtnY + foldBtnSize - 2);
                            }
                            
                            SelectObject(hdc, hOldFoldBrush);
                            SelectObject(hdc, hOldFoldPen);
                            DeleteObject(hFoldPen);
                        }
                    }
                    
                    // 赋值语句也绘制展开/折叠符号（对所有赋值语句生效）
                    if (!isCommandLine && IsAssignmentLine(line)) {
                        while (doc->parametersExpanded.size() <= (size_t)currentLineIndex) {
                            doc->parametersExpanded.push_back(false);
                        }
                        bool isExpanded = doc->parametersExpanded[currentLineIndex];
                        
                        int foldBtnSize = 10;
                        int foldBtnX = 58;
                        int foldBtnY = y + (rowHeight - foldBtnSize) / 2;
                        
                        HPEN hFoldPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.textDim);
                        HPEN hOldFoldPen = (HPEN)SelectObject(hdc, hFoldPen);
                        HBRUSH hOldFoldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
                        Rectangle(hdc, foldBtnX, foldBtnY, foldBtnX + foldBtnSize, foldBtnY + foldBtnSize);
                        
                        MoveToEx(hdc, foldBtnX + 2, foldBtnY + foldBtnSize / 2, NULL);
                        LineTo(hdc, foldBtnX + foldBtnSize - 2, foldBtnY + foldBtnSize / 2);
                        
                        if (!isExpanded) {
                            MoveToEx(hdc, foldBtnX + foldBtnSize / 2, foldBtnY + 2, NULL);
                            LineTo(hdc, foldBtnX + foldBtnSize / 2, foldBtnY + foldBtnSize - 2);
                        }
                        
                        SelectObject(hdc, hOldFoldBrush);
                        SelectObject(hdc, hOldFoldPen);
                        DeleteObject(hFoldPen);
                    }
                }
            };
            
            // Lambda 函数：绘制网格线
            auto DrawGridLine = [&](int x1, int y1, int x2, int y2) {
                HPEN hPen = CreatePen(PS_SOLID, 1, clrGridLine);
                HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
                MoveToEx(hdc, x1, y1, NULL);
                LineTo(hdc, x2, y2);
                SelectObject(hdc, hOldPen);
                DeleteObject(hPen);
            };
            
            // Lambda 函数：绘制表格单元格 (默认参数使用主题颜色)
            auto DrawTableCell = [&](int x, int y, int w, int h, const wchar_t* text, COLORREF color, bool isHeader = false, COLORREF headerBgColor = g_CurrentTheme.tableHeaderBg, bool centerAlign = false, bool isCheckbox = false, COLORREF diffBgColor = g_CurrentTheme.bg, bool hasDiffBg = false) {
                // 如果有diff背景色，先绘制
                if (hasDiffBg) {
                    RECT diffRc = { x, y, x + w, y + h };
                    HBRUSH hDiffBrush = CreateSolidBrush(diffBgColor);
                    FillRect(hdc, &diffRc, hDiffBrush);
                    DeleteObject(hDiffBrush);
                } else if (isHeader) {
                    RECT rc = { x, y, x + w, y + h };
                    HBRUSH hHeaderBrush = CreateSolidBrush(headerBgColor);
                    FillRect(hdc, &rc, hHeaderBrush);
                    DeleteObject(hHeaderBrush);
                }
                
                DrawGridLine(x, y, x + w, y);
                DrawGridLine(x + w, y, x + w, y + h);
                DrawGridLine(x, y + h, x + w, y + h);
                DrawGridLine(x, y, x, y + h);
                
                SetTextColor(hdc, color);
                if (isHeader) {
                    SelectObject(hdc, hBoldFont);
                } else {
                    SelectObject(hdc, hFont);
                }
                
                int textY = y + (h - fontSize) / 2;  // 使用fontSize计算垂直居中
                
                // 如果是复选框列且有值，绘制打勾图片
                if (isCheckbox && text && wcslen(text) > 0) {
                    HBITMAP hCheck = LoadCheckIcon();
                    if (hCheck) {
                        // 获取图片尺寸
                        BITMAP bm;
                        GetObject(hCheck, sizeof(bm), &bm);
                        
                        // 计算居中位置
                        int iconSize = 16;  // 目标图标大小
                        int iconX = x + (w - iconSize) / 2;
                        int iconY = y + (h - iconSize) / 2;
                        
                        // 创建内存DC绘制图片
                        HDC hdcMem = CreateCompatibleDC(hdc);
                        HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, hCheck);
                        
                        // 使用透明绘制
                        BLENDFUNCTION bf;
                        bf.BlendOp = AC_SRC_OVER;
                        bf.BlendFlags = 0;
                        bf.SourceConstantAlpha = 255;
                        bf.AlphaFormat = AC_SRC_ALPHA;
                        AlphaBlend(hdc, iconX, iconY, iconSize, iconSize, 
                                   hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, bf);
                        
                        SelectObject(hdcMem, hOldBmp);
                        DeleteDC(hdcMem);
                    } else {
                        // 如果图片加载失败，绘制文字
                        if (centerAlign) {
                            SIZE textSize;
                            GetTextExtentPoint32W(hdc, text, lstrlenW(text), &textSize);
                            int textX = x + (w - textSize.cx) / 2;
                            TextOutW(hdc, textX, textY, text, lstrlenW(text));
                        } else {
                            TextOutW(hdc, x + 3, textY, text, lstrlenW(text));
                        }
                    }
                } else if (centerAlign) {
                    // 水平居中对齐
                    SIZE textSize;
                    GetTextExtentPoint32W(hdc, text, lstrlenW(text), &textSize);
                    int textX = x + (w - textSize.cx) / 2;
                    TextOutW(hdc, textX, textY, text, lstrlenW(text));
                } else {
                    // 默认左对齐
                    TextOutW(hdc, x + 3, textY, text, lstrlenW(text));
                }
            };
            
            // 流程控制线的彩虹色数组（按嵌套深度循环使用）
            static const COLORREF g_RainbowColors[] = {
                RGB(230, 80, 80),    // 红
                RGB(230, 160, 50),   // 橙
                RGB(220, 200, 50),   // 黄
                RGB(80, 190, 80),    // 绿
                RGB(60, 180, 220),   // 青
                RGB(80, 120, 230),   // 蓝
                RGB(170, 100, 220),  // 紫
            };
            static const int g_RainbowColorCount = sizeof(g_RainbowColors) / sizeof(g_RainbowColors[0]);
            
            auto GetFlowLineColor = [](int depth) -> COLORREF {
                return g_RainbowColors[depth % g_RainbowColorCount];
            };

            // Lambda 函数：绘制实心向下箭头（可指定颜色，默认使用textDim）
            auto DrawDownArrow = [&](int x, int y, COLORREF color = 0) {
                if (color == 0) color = g_CurrentTheme.textDim;
                HBRUSH hArrowBrush = CreateSolidBrush(color);
                HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hArrowBrush);
                HPEN hNoPen = (HPEN)GetStockObject(NULL_PEN);
                HPEN hOldPen = (HPEN)SelectObject(hdc, hNoPen);
                
                POINT points[3];
                points[0] = { x, y + 1 };      // Tip
                points[1] = { x - 3, y - 2 };  // Left
                points[2] = { x + 3, y - 2 };  // Right
                
                Polygon(hdc, points, 3);
                
                SelectObject(hdc, hOldPen);
                SelectObject(hdc, hOldBrush);
                DeleteObject(hArrowBrush);
            };

            // Lambda 函数：绘制实心向右箭头（可指定颜色，默认使用textDim）
            auto DrawRightArrow = [&](int x, int y, COLORREF color = 0) {
                if (color == 0) color = g_CurrentTheme.textDim;
                HBRUSH hArrowBrush = CreateSolidBrush(color);
                HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hArrowBrush);
                HPEN hNoPen = (HPEN)GetStockObject(NULL_PEN);
                HPEN hOldPen = (HPEN)SelectObject(hdc, hNoPen);
                
                POINT points[3];
                points[0] = { x, y };          // Tip
                points[1] = { x - 6, y - 3 };  // Top
                points[2] = { x - 6, y + 3 };  // Bottom
                
                Polygon(hdc, points, 3);
                
                SelectObject(hdc, hOldPen);
                SelectObject(hdc, hOldBrush);
                DeleteObject(hArrowBrush);
            };

            // Lambda 函数：绘制实心向上箭头（可指定颜色，默认使用textDim）
            auto DrawUpArrow = [&](int x, int y, COLORREF color = 0) {
                if (color == 0) color = g_CurrentTheme.textDim;
                HBRUSH hArrowBrush = CreateSolidBrush(color);
                HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hArrowBrush);
                HPEN hNoPen = (HPEN)GetStockObject(NULL_PEN);
                HPEN hOldPen = (HPEN)SelectObject(hdc, hNoPen);
                
                POINT points[3];
                points[0] = { x, y - 1 };      // Tip (pointing up)
                points[1] = { x - 3, y + 2 };  // Left
                points[2] = { x + 3, y + 2 };  // Right
                
                Polygon(hdc, points, 3);
                
                SelectObject(hdc, hOldPen);
                SelectObject(hdc, hOldBrush);
                DeleteObject(hArrowBrush);
            };
            
            // 绘制每一行（遍历data->lines）
            std::vector<int> currentTableColWidths;  // 当前表格的列宽
            
            // 第一遍遍历：计算每列的最大宽度（使用缓存避免每次重绘都重算）
            std::vector<std::vector<int>> tableMaxWidths;
            std::vector<int> tableTypes;
            
            // 检查是否有版本行
            bool hasVersion = (!doc->lines.empty() && doc->lines[0].find(L"版本\t") == 0);
            size_t startLine = hasVersion ? 1 : 0;
            
            if (!doc->tableLayoutDirty && (int)doc->lines.size() == doc->cachedLineCount) {
                // 使用缓存
                tableMaxWidths = doc->cachedTableMaxWidths;
                tableTypes = doc->cachedTableTypes;
            } else {
            // 需要重新计算
            int currentTableIndex = -1;
            bool insideAssembly = false;
            bool inAssemblyVarSection = false;
            
            for (size_t i = startLine; i < doc->lines.size(); i++) {
                const std::wstring& line = doc->lines[i];
                
                if (line.empty()) {
                    // 空行不重置currentTableIndex，只是跳过
                    insideAssembly = false; // 遇到空行，退出程序集块
                    inAssemblyVarSection = false; // 退出程序集变量区域
                    continue;
                }
                
                if (line.find(L"\t") != std::wstring::npos) {
                    // 表格行 - 解析单元格
                    std::vector<std::wstring> cells;
                    size_t start = 0, pos = 0;
                    while ((pos = line.find(L'\t', start)) != std::wstring::npos) {
                        cells.push_back(line.substr(start, pos - start));
                        start = pos + 1;
                    }
                    cells.push_back(line.substr(start));
                    
                    // 判断是否为表头
                    bool isHeader = (cells.size() > 0 && 
                                   (cells[0] == L"程序集名" || 
                                    cells[0] == L"子程序名" || 
                                    cells[0] == L"参数名" || 
                                    cells[0] == L"变量名" ||
                                    cells[0] == L"版本"));
                    
                    // 判断是否为参数表（属于子程序表的一部分）
                    bool isParamTable = (cells.size() > 0 && cells[0] == L"参数名");
                    
                    // 判断是否为程序集变量表（在程序集块内）
                    bool isAssemblyVarTable = (cells.size() > 0 && cells[0] == L"变量名" && insideAssembly);
                    
                    if (isHeader && !isParamTable) {
                        if (isAssemblyVarTable) {
                            // 程序集变量表：不创建新表格，复用程序集表格
                            // 确保有足够的列（4列）
                            if (currentTableIndex >= 0 && currentTableIndex < (int)tableMaxWidths.size()) {
                                while (tableMaxWidths[currentTableIndex].size() < 4) {
                                    tableMaxWidths[currentTableIndex].push_back(100);
                                }
                            }
                            inAssemblyVarSection = true; // 进入程序集变量区域
                        } else {
                            // 每个表格都创建新索引（包括子程序表）
                            currentTableIndex++;
                            tableMaxWidths.push_back(std::vector<int>());
                            tableTypes.push_back(0);
                            inAssemblyVarSection = false; // 退出程序集变量区域
                            
                            // 初始化列宽
                            if (cells.size() > 0 && cells[0] == L"子程序名") {
                                // 子程序表4列：名称、类型、公开、备注
                                // 参数表使用时：列0,1,2相同，列3拆分为可空(30)+数组(30)+备注
                                tableMaxWidths[currentTableIndex] = {150, 100, 30, 320};
                                insideAssembly = false;
                            } else if (cells.size() > 0 && cells[0] == L"变量名") {
                                // 变量表5列：名称、类型、静态（复选框）、数组（复选框）、备注
                                tableMaxWidths[currentTableIndex] = {100, 80, 30, 30, 200};
                                insideAssembly = false;
                            } else if (cells.size() > 0 && cells[0] == L"程序集名") {
                                // 程序集表4列：名称、保留、保留、备注
                                tableMaxWidths[currentTableIndex] = {150, 100, 100, 200};
                                tableTypes[currentTableIndex] = 1; // Assembly
                                insideAssembly = true; // 进入程序集块
                            } else {
                                for (size_t j = 0; j < cells.size(); j++) {
                                    tableMaxWidths[currentTableIndex].push_back(100);
                                }
                                insideAssembly = false;
                            }
                        }
                    }
                    
                    if (isParamTable) {
                        // 参数表：如果没有当前表格，创建一个；否则使用当前表格
                        if (currentTableIndex < 0) {
                            currentTableIndex++;
                            tableMaxWidths.push_back(std::vector<int>());
                            tableTypes.push_back(0);
                            // 参数表使用子程序表的4列结构
                            tableMaxWidths[currentTableIndex] = {100, 80, 40, 260};
                        }
                        // 参数表与子程序表共享列宽，不重新初始化
                    }
                    
                    // 计算每个单元格需要的宽度并更新最大值
                    if (currentTableIndex >= 0 && currentTableIndex < (int)tableMaxWidths.size()) {
                        // 如果当前表格列数不足，扩充列数（处理参数表和数据行）
                        if (tableMaxWidths[currentTableIndex].size() < cells.size()) {
                            while (tableMaxWidths[currentTableIndex].size() < cells.size()) {
                                tableMaxWidths[currentTableIndex].push_back(100);
                            }
                        }
                        
                        // 判断当前数据行所属的表格类型
                        bool isAssembly = (currentTableIndex < (int)tableTypes.size() && tableTypes[currentTableIndex] == 1);
                        bool isAssemblyVarData = inAssemblyVarSection;
                        
                        // 判断是否为参数表数据行：需要向上查找最近的表头
                        bool isParamRow = false;
                        if (!isHeader && cells.size() >= 6) {
                            // 向上查找最近的表头
                            for (int searchLine = (int)i - 1; searchLine >= (int)startLine; searchLine--) {
                                const std::wstring& searchLineTxt = doc->lines[searchLine];
                                if (searchLineTxt.empty()) continue;
                                if (searchLineTxt.find(L"\t") != std::wstring::npos) {
                                    std::vector<std::wstring> searchCells;
                                    size_t start = 0, pos = 0;
                                    while ((pos = searchLineTxt.find(L'\t', start)) != std::wstring::npos) {
                                        searchCells.push_back(searchLineTxt.substr(start, pos - start));
                                        start = pos + 1;
                                    }
                                    searchCells.push_back(searchLineTxt.substr(start));
                                    
                                    if (searchCells.size() > 0) {
                                        if (searchCells[0] == L"参数名") {
                                            isParamRow = true;
                                            break;
                                        } else if (searchCells[0] == L"子程序名" || searchCells[0] == L"变量名") {
                                            break;
                                        }
                                    }
                                }
                            }
                        }

                        if (isParamRow && cells.size() >= 6) {
                            // 参数表：第0列（名称）、第1列（类型）正常计算
                            for (size_t j = 0; j < 2 && j < cells.size(); j++) {
                                if (!cells[j].empty()) {
                                    SIZE textSize;
                                    GetTextExtentPoint32W(hdc, cells[j].c_str(), (int)cells[j].length(), &textSize);
                                    int minWidth = textSize.cx + 10;
                                    if (minWidth > tableMaxWidths[currentTableIndex][j]) {
                                        tableMaxWidths[currentTableIndex][j] = minWidth;
                                    }
                                }
                            }
                            // 第2列（参考）固定30像素，不计算
                            // 第3列（备注）包含可空(30)+数组(30)+备注文本
                            
                            int remarkWidth = 0;
                            if (cells.size() > 5 && !cells[5].empty()) {
                                SIZE textSize;
                                GetTextExtentPoint32W(hdc, cells[5].c_str(), (int)cells[5].length(), &textSize);
                                remarkWidth = textSize.cx + 10;
                            }
                            
                            // 参数表列3总宽度 = 可空(30) + 数组(30) + 备注文本
                            int totalWidth = 30 + 30 + remarkWidth;
                            
                            // 更新列3（对应子程序表的备注列）
                            if (totalWidth > tableMaxWidths[currentTableIndex][3]) {
                                tableMaxWidths[currentTableIndex][3] = totalWidth;
                            }
                        } else {
                            // 普通表格行（包括程序集变量数据行和子程序数据行）
                            for (size_t j = 0; j < cells.size() && j < tableMaxWidths[currentTableIndex].size(); j++) {
                                // 判断是否为程序集变量数据行
                                bool isAssemblyVarData = inAssemblyVarSection;
                                
                                // 判断是否为子程序数据行或局部变量数据行（需要向上查找表头）
                                bool isSubProgramRow = false;
                                bool isLocalVarRow = false;
                                if (!isHeader && !isAssemblyVarData) {
                                    for (int searchLine = (int)i - 1; searchLine >= (int)startLine; searchLine--) {
                                        const std::wstring& searchLineTxt = doc->lines[searchLine];
                                        if (searchLineTxt.empty()) continue;
                                        if (searchLineTxt.find(L"\t") != std::wstring::npos) {
                                            std::vector<std::wstring> searchCells;
                                            size_t start = 0, pos = 0;
                                            while ((pos = searchLineTxt.find(L'\t', start)) != std::wstring::npos) {
                                                searchCells.push_back(searchLineTxt.substr(start, pos - start));
                                                start = pos + 1;
                                            }
                                            searchCells.push_back(searchLineTxt.substr(start));
                                            
                                            if (searchCells.size() > 0) {
                                                if (searchCells[0] == L"子程序名") {
                                                    isSubProgramRow = true;
                                                    break;
                                                } else if (searchCells[0] == L"参数名") {
                                                    // 参数表下面是参数数据，继续向上找子程序名
                                                    continue;
                                                } else if (searchCells[0] == L"变量名") {
                                                    // 判断是程序集变量表还是局部变量表
                                                    // 如果有"静态"列，则是局部变量表
                                                    if (searchLineTxt.find(L"静态") != std::wstring::npos) {
                                                        isLocalVarRow = true;
                                                        break;
                                                    } else {
                                                        // 程序集变量表，继续向上找
                                                        continue;
                                                    }
                                                } else if (searchCells[0] == L"程序集名") {
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                                
                                // 对于程序集变量数据行，使用4列；对于局部变量行，跳过复选框列（第2、第3列）；子程序行不跳过
                                if (isAssemblyVarData) {
                                    // 程序集变量：只有4列，不跳过任何列
                                    if (j < 4 && !cells[j].empty()) {
                                        SIZE textSize;
                                        GetTextExtentPoint32W(hdc, cells[j].c_str(), (int)cells[j].length(), &textSize);
                                        int minWidth = textSize.cx + 10;
                                        if (minWidth > tableMaxWidths[currentTableIndex][j]) {
                                            tableMaxWidths[currentTableIndex][j] = minWidth;
                                        }
                                    }
                                } else if (isLocalVarRow) {
                                    // 局部变量行：跳过复选框列（第2、第3列）
                                    if (j == 2 || j == 3) continue;
                                    
                                    if (!cells[j].empty()) {
                                        SIZE textSize;
                                        GetTextExtentPoint32W(hdc, cells[j].c_str(), (int)cells[j].length(), &textSize);
                                        int minWidth = textSize.cx + 10;
                                        if (minWidth > tableMaxWidths[currentTableIndex][j]) {
                                            tableMaxWidths[currentTableIndex][j] = minWidth;
                                        }
                                    }
                                } else if (isSubProgramRow) {
                                    // 子程序数据行：计算列0,1,2,3，其中列2（公开）固定30不计算
                                    if (j == 2) continue;  // 公开列固定30px
                                    
                                    if (!cells[j].empty()) {
                                        SIZE textSize;
                                        GetTextExtentPoint32W(hdc, cells[j].c_str(), (int)cells[j].length(), &textSize);
                                        int minWidth = textSize.cx + 10;
                                        if (minWidth > tableMaxWidths[currentTableIndex][j]) {
                                            tableMaxWidths[currentTableIndex][j] = minWidth;
                                        }
                                    }
                                } else {
                                    // 其他数据行：不跳过任何列
                                    if (!cells[j].empty()) {
                                        SIZE textSize;
                                        GetTextExtentPoint32W(hdc, cells[j].c_str(), (int)cells[j].length(), &textSize);
                                        int minWidth = textSize.cx + 10;
                                        if (minWidth > tableMaxWidths[currentTableIndex][j]) {
                                            tableMaxWidths[currentTableIndex][j] = minWidth;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            // 保存缓存
            doc->cachedTableMaxWidths = tableMaxWidths;
            doc->cachedTableTypes = tableTypes;
            doc->tableLayoutDirty = false;
            doc->cachedLineCount = (int)doc->lines.size();
            } // end if (需要重新计算)
            
            // 性能优化：重建流程块索引缓存
            if (doc->flowBlockDirty) {
                BuildFlowBlockIndex(doc);
            }
            
            // 性能优化：字体变化时清空文本宽度缓存
            if (doc->cachedFontSize != data->fontSize) {
                doc->textWidthCache.clear();
                doc->cachedFontSize = data->fontSize;
                // 预计算常用字符宽度
                SIZE chSz;
                GetTextExtentPoint32W(hdc, L"中", 1, &chSz);
                doc->cachedChineseCharWidth = chSz.cx;
                SIZE enSz;
                GetTextExtentPoint32W(hdc, L"M", 1, &enSz);
                doc->cachedCharWidth = enSz.cx;
            }
            
            // 第二遍遍历：实际绘制
            int currentTableIndex = -1;
            bool inParamTable = false;  // 标记当前是否在参数表区域内
            bool inClassVarTable = false; // 标记当前是否在程序集变量表区域内
            bool insideAssembly = false; // 标记是否在程序集块内
            bool lastWasAssemblySection = false; // 标记上一行是否属于程序集区域
            int lastTableType = 0; // 0:无, 1:程序集, 2:子程序, 3:参数, 4:变量
            
            for (size_t i = startLine; i < doc->lines.size() && currentY < rect.bottom; i++) {
                const std::wstring& line = doc->lines[i];
                
                // 检查当前行是什么类型的表头
                bool isSubProgramHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"子程序名") == 0);
                bool isParamHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"参数名") == 0);
                bool isVarHeader = (line.find(L"\t") != std::wstring::npos && line.find(L"变量名") == 0);
                
                // 在程序集区域结束后、子程序表开始前添加间距
                if (isSubProgramHeader && lastWasAssemblySection) {
                    currentY += rowHeight / 2;
                    lastWasAssemblySection = false;
                }
                
                // 在参数表和变量表之间添加间距
                if (isVarHeader && lastTableType == 3) {
                    currentY += rowHeight / 2;
                }
                
                // 在子程序表（无参数时）和变量表之间添加间距
                if (isVarHeader && lastTableType == 2) {
                    currentY += rowHeight / 2;
                }
                
                // 空行标记表格结束
                if (line.empty()) {
                    currentTableColWidths.clear();
                    inParamTable = false;  // 退出参数表区域
                    inClassVarTable = false; // 退出程序集变量表区域
                    // 注意：不重置 insideAssembly，因为程序集变量表可能在程序集数据行之后
                    // insideAssembly 只在遇到新的表头时才更新
                }
                
                // 性能优化：行在可见区域之上时，跳过所有绘制，只做状态跟踪
                if (currentY + rowHeight < data->tabHeight) {
                    if (!line.empty() && line.find(L"\t") != std::wstring::npos) {
                        // 表格行：快速状态跟踪
                        std::wstring firstCell;
                        size_t tabPos = line.find(L'\t');
                        if (tabPos != std::wstring::npos) {
                            firstCell = line.substr(0, tabPos);
                        }
                        bool isHeader = (firstCell == L"程序集名" || firstCell == L"子程序名" || 
                                       firstCell == L"参数名" || firstCell == L"变量名");
                        bool isParamTableHdr = (firstCell == L"参数名");
                        bool isAssemblyVarTable = (firstCell == L"变量名" && insideAssembly);
                        
                        if (isHeader && !isParamTableHdr) {
                            if (isAssemblyVarTable) {
                                inClassVarTable = true;
                            } else {
                                currentTableIndex++;
                                inClassVarTable = false;
                                insideAssembly = (firstCell == L"程序集名");
                            }
                            inParamTable = false;
                        }
                        if (isParamTableHdr) inParamTable = true;
                        
                        // 更新 lastWasAssemblySection / lastTableType
                        if (firstCell == L"程序集名") {
                            lastWasAssemblySection = true; lastTableType = 1;
                        } else if (firstCell == L"变量名" && insideAssembly) {
                            lastWasAssemblySection = true; lastTableType = 1;
                        } else if (firstCell == L"子程序名") {
                            lastWasAssemblySection = false; lastTableType = 2;
                        } else if (firstCell == L"参数名") {
                            lastTableType = 3;
                        } else if (firstCell == L"变量名" && !insideAssembly) {
                            lastTableType = 4;
                        } else if (inParamTable) {
                            lastTableType = 3;
                        }
                        if (inClassVarTable) lastWasAssemblySection = true;
                    } else if (!line.empty()) {
                        if (!inClassVarTable && !insideAssembly) lastWasAssemblySection = false;
                    }
                    lineNum++;
                    currentY += rowHeight;
                    continue;
                }
                
                // 绘制行号（删除行不显示行号）
                bool isDeletedLine = (doc->showDiff && i < doc->diffTypes.size() && doc->diffTypes[i] == DIFF_DELETED);
                DrawLineNum(currentY, isDeletedLine, static_cast<int>(i));
                
                // 绘制选中背景
                if (doc->hasSelection) {
                    int selStart = doc->selStartLine;
                    int selEnd = doc->selEndLine;
                    int selColStart = doc->selStartCol;
                    int selColEnd = doc->selEndCol;
                    
                    // 确保 selStart <= selEnd
                    if (selStart > selEnd || (selStart == selEnd && selColStart > selColEnd)) {
                        std::swap(selStart, selEnd);
                        std::swap(selColStart, selColEnd);
                    }
                    
                    // 如果当前行在选中范围内
                    if ((int)i >= selStart && (int)i <= selEnd) {
                        int selX = startX;
                        int selWidth = 0;
                        
                        // 计算实际文本的起始位置（考虑缩进和特殊格式）
                        int textStartX = startX;
                        bool isTableLine = (line.find(L"\t") != std::wstring::npos);
                        
                        // 计算嵌套深度和标记长度
                        size_t selMarkerLen = 0;
                        int selNestDepth = GetNestingDepth(line, &selMarkerLen);
                        bool selIsFlowControl = IsFlowControlLine(line);
                        
                        if (selIsFlowControl) {
                            // 流程控制语句，文本从 startX + 20 + 嵌套偏移 开始
                            textStartX = startX + 20 + selNestDepth * 20;
                        } else if (selNestDepth > 0) {
                            // 嵌套内容行，文本从 startX + 嵌套偏移 开始
                            textStartX = startX + selNestDepth * 20;
                        } else if (!isTableLine) {
                            // 非表格行，需要计算缩进
                            int indent = 0;
                            size_t pos = 0;
                            while (pos < line.length() && (line[pos] == L' ' || line[pos] == L'\t')) {
                                if (line[pos] == L'\t') indent += 20;
                                else indent += 4;
                                pos++;
                            }
                            textStartX = startX + indent;
                        }
                        
                        if (isTableLine) {
                            // 表格行：根据单元格宽度计算选中区域
                            std::vector<std::wstring> cells;
                            size_t start = 0, pos = 0;
                            while ((pos = line.find(L'\t', start)) != std::wstring::npos) {
                                cells.push_back(line.substr(start, pos - start));
                                start = pos + 1;
                            }
                            cells.push_back(line.substr(start));
                            
                            bool isHeader = (cells.size() > 0 && 
                                           (cells[0] == L"程序集名" || cells[0] == L"子程序名" || 
                                            cells[0] == L"参数名" || cells[0] == L"变量名"));
                            bool isParamTable = (cells.size() > 0 && cells[0] == L"参数名");
                            bool isAssemblyVarTable = (cells.size() > 0 && cells[0] == L"变量名" && insideAssembly);
                            
                            int useTableIndex = currentTableIndex;
                            if (isHeader && !isParamTable && !isAssemblyVarTable) useTableIndex++;
                            
                            std::vector<int> colWidths;
                            if (useTableIndex >= 0 && useTableIndex < (int)tableMaxWidths.size()) {
                                colWidths = tableMaxWidths[useTableIndex];
                            } else {
                                for (size_t j = 0; j < cells.size(); j++) colWidths.push_back(100);
                            }
                            
                            bool isParamRow = isParamTable || (inParamTable && !line.empty());
                            
                            int currentX = startX;
                            int charCount = 0;
                            int selStartX = -1;
                            int selEndX = -1;
                            
                            // 确定当前行的选中范围
                            int lineSelStart = 0;
                            int lineSelEnd = (int)line.length();
                            
                            if (selStart == selEnd) {
                                lineSelStart = selColStart;
                                lineSelEnd = selColEnd;
                            } else if ((int)i == selStart) {
                                lineSelStart = selColStart;
                            } else if ((int)i == selEnd) {
                                lineSelEnd = selColEnd;
                            }
                            
                            for (size_t j = 0; j < cells.size(); j++) {
                                int visualW = 0;
                                if (isParamRow && cells.size() >= 6) {
                                    if (j == 0) visualW = colWidths[0];
                                    else if (j == 1) visualW = colWidths[1];
                                    else if (j == 2) visualW = colWidths[2];
                                    else if (j == 3) visualW = 30;  // 可空列固定30像素
                                    else if (j == 4) visualW = 30;  // 数组列固定30像素
                                    else if (j == 5) visualW = colWidths[3] - 60;  // 备注列 = 子程序表备注列宽度 - 可空(30) - 数组(30)
                                } else {
                                    visualW = (j < colWidths.size()) ? colWidths[j] : 100;
                                }
                                
                                int cellLen = (int)cells[j].length();
                                int cellEndChar = charCount + cellLen;
                                
                                // 计算选中起始位置X
                                if (selStartX == -1 && lineSelStart <= cellEndChar) {
                                    if (lineSelStart <= charCount) {
                                        selStartX = currentX;
                                    } else {
                                        int offset = lineSelStart - charCount;
                                        std::wstring sub = cells[j].substr(0, offset);
                                        SIZE sz;
                                        GetTextExtentPoint32W(hdc, sub.c_str(), (int)sub.length(), &sz);
                                        selStartX = currentX + 3 + sz.cx;
                                    }
                                }
                                
                                // 计算选中结束位置X
                                if (selEndX == -1 && lineSelEnd <= cellEndChar) {
                                    if (lineSelEnd <= charCount) {
                                        selEndX = currentX;
                                    } else {
                                        int offset = lineSelEnd - charCount;
                                        std::wstring sub = cells[j].substr(0, offset);
                                        SIZE sz;
                                        GetTextExtentPoint32W(hdc, sub.c_str(), (int)sub.length(), &sz);
                                        
                                        // 对于参数表的备注列（第6列），如果选中到了列末尾，延伸到整个列宽
                                        if (isParamRow && j == 5 && offset >= cellLen) {
                                            selEndX = currentX + visualW;
                                        } else {
                                            selEndX = currentX + 3 + sz.cx;
                                        }
                                    }
                                }
                                
                                currentX += visualW;
                                charCount += cellLen + 1; // +1 为制表符
                            }
                            
                            // 强制补充所有缺失列的宽度，确保选中背景能覆盖到最后一列
                            // 对于参数表
                            if (isParamRow && colWidths.size() > 3) {
                                int expectedCells = 6;  // 参数表应该有6个单元格
                                if ((int)cells.size() < expectedCells) {
                                    for (int j = (int)cells.size(); j < expectedCells; j++) {
                                        int visualW = 0;
                                        if (j == 3) visualW = 30;  // 可空列
                                        else if (j == 4) visualW = 30;  // 数组列
                                        else if (j == 5) visualW = colWidths[3] - 60;  // 备注列
                                        currentX += visualW;
                                        charCount += 1;  // 每个缺失列仍算一个制表符
                                    }
                                }
                            } else if (!isHeader && colWidths.size() > 0) {
                                // 非参数表的数据行，补充到colWidths的完整列数
                                int expectedCells = (int)colWidths.size();
                                if ((int)cells.size() < expectedCells) {
                                    for (int j = (int)cells.size(); j < expectedCells; j++) {
                                        int visualW = (j < colWidths.size()) ? colWidths[j] : 100;
                                        currentX += visualW;
                                        charCount += 1;  // 每个缺失列仍算一个制表符
                                    }
                                }
                            }
                            
                            if (selStartX == -1) selStartX = currentX;
                            
                            // 如果选中到行尾，强制使用完整列宽（包括补充的列）
                            if (selEndX == -1 || lineSelEnd >= (int)line.length()) {
                                selEndX = currentX;
                            }
                            
                            selX = selStartX;
                            selWidth = selEndX - selStartX;
                            if (selWidth < 5 && lineSelStart != lineSelEnd) selWidth = 5;
                            
                        } else {
                            // 非表格行：使用原有逻辑
                            // 使用GDI精确计算文本宽度
                            SIZE textSize;
                            
                            // 计算缩进字符数量（包括嵌套标记字符）
                            size_t indentChars = 0;
                            if (selIsFlowControl) {
                                indentChars = selMarkerLen + 1;  // 标记字符 + 点号
                            } else if (selNestDepth > 0) {
                                indentChars = selMarkerLen;  // 仅标记字符
                            } else {
                                while (indentChars < line.length() && (line[indentChars] == L' ' || line[indentChars] == L'\t')) {
                                    indentChars++;
                                }
                            }
                            
                            if (selStart == selEnd) {
                            // 单行选中
                            int adjustedSelStart = (std::max)(0, selColStart - (int)indentChars);
                            int adjustedSelEnd = (std::max)(0, selColEnd - (int)indentChars);
                            
                            if (selColStart < (int)line.length() && selColStart >= (int)indentChars) {
                                std::wstring beforeSel = line.substr(indentChars, adjustedSelStart);
                                GetTextExtentPoint32W(hdc, beforeSel.c_str(), (int)beforeSel.length(), &textSize);
                                selX = textStartX + textSize.cx;
                            } else {
                                selX = textStartX;
                            }
                            
                            if (selColEnd > (int)indentChars && selColEnd <= (int)line.length()) {
                                std::wstring selText = line.substr(indentChars + adjustedSelStart, adjustedSelEnd - adjustedSelStart);
                                GetTextExtentPoint32W(hdc, selText.c_str(), (int)selText.length(), &textSize);
                                selWidth = textSize.cx;
                            }
                            
                            // 如果选中空行或选中到行尾，至少显示5像素宽度
                            if (selWidth == 0) {
                                selWidth = 5;
                            }
                        } else if ((int)i == selStart) {
                            // 选中起始行
                            int adjustedSelStart = (std::max)(0, selColStart - (int)indentChars);
                            
                            if (selColStart < (int)line.length() && selColStart >= (int)indentChars) {
                                std::wstring beforeSel = line.substr(indentChars, adjustedSelStart);
                                GetTextExtentPoint32W(hdc, beforeSel.c_str(), (int)beforeSel.length(), &textSize);
                                selX = textStartX + textSize.cx;
                                
                                std::wstring selText = line.substr(selColStart);
                                GetTextExtentPoint32W(hdc, selText.c_str(), (int)selText.length(), &textSize);
                                selWidth = textSize.cx + 5;  // 加额外的5像素
                            } else {
                                // 从行尾开始选中
                                if ((int)line.length() > (int)indentChars) {
                                    std::wstring textPart = line.substr(indentChars);
                                    GetTextExtentPoint32W(hdc, textPart.c_str(), (int)textPart.length(), &textSize);
                                    selX = textStartX + textSize.cx;
                                } else {
                                    selX = textStartX;
                                }
                                selWidth = 5;
                            }
                        } else if ((int)i == selEnd) {
                            // 选中结束行
                            selX = textStartX;
                            int adjustedSelEnd = (std::max)(0, selColEnd - (int)indentChars);
                            
                            if (selColEnd > (int)indentChars && selColEnd <= (int)line.length()) {
                                std::wstring selText = line.substr(indentChars, adjustedSelEnd);
                                GetTextExtentPoint32W(hdc, selText.c_str(), (int)selText.length(), &textSize);
                                selWidth = textSize.cx;
                            } else if (selColEnd == 0 || selColEnd <= (int)indentChars) {
                                // 选中到行首或缩进内
                                selWidth = 5;
                            }
                        } else {
                            // 中间行，全部选中
                            selX = textStartX;
                            if ((int)line.length() > (int)indentChars) {
                                std::wstring textPart = line.substr(indentChars);
                                GetTextExtentPoint32W(hdc, textPart.c_str(), (int)textPart.length(), &textSize);
                                selWidth = textSize.cx + 5;  // 加额外的5像素
                            } else {
                                // 空行也显示5像素宽度
                                selWidth = 5;
                            }
                        }
                        }
                        
                        if (selWidth > 0) {
                            RECT selRect = {selX, currentY, selX + selWidth, currentY + rowHeight};
                            HBRUSH selBrush = CreateSolidBrush(g_CurrentTheme.editorSelection);  // 选中背景色
                            FillRect(hdc, &selRect, selBrush);
                            DeleteObject(selBrush);
                        }
                    }
                }
                
                // 分析行内容并绘制
                if (line.find(L"\t") != std::wstring::npos) {
                    // 表格行 - 绘制diff符号（+/-）
                    // diff背景色已在表格单元格中处理，这里不需要绘制+/-符号
                    
                    // 表格行 - 解析制表符分隔的内容
                    std::vector<std::wstring> cells;
                    size_t start = 0, pos = 0;
                    while ((pos = line.find(L'\t', start)) != std::wstring::npos) {
                        cells.push_back(line.substr(start, pos - start));
                        start = pos + 1;
                    }
                    cells.push_back(line.substr(start));
                    
                    // 判断是否为表头
                    bool isHeader = (cells.size() > 0 && 
                                   (cells[0] == L"程序集名" || cells[0] == L"子程序名" || 
                                    cells[0] == L"参数名" || cells[0] == L"变量名"));
                    
                    // 判断是否为参数表（属于子程序表的一部分）
                    bool isParamTable = (cells.size() > 0 && cells[0] == L"参数名");
                    
                    // 判断是否为程序集变量表（在程序集块内）
                    bool isAssemblyVarTable = (cells.size() > 0 && cells[0] == L"变量名" && insideAssembly);
                    
                    // 如果是表头但不是参数表，递增currentTableIndex
                    if (isHeader && !isParamTable) {
                        if (isAssemblyVarTable) {
                            // 程序集变量表：不递增索引，复用程序集表格
                            inClassVarTable = true;
                        } else {
                            // 每个表格都递增索引
                            currentTableIndex++;
                            
                            inClassVarTable = false;
                            // 更新insideAssembly状态
                            if (cells[0] == L"子程序名") {
                                insideAssembly = false;
                            } else if (cells[0] == L"变量名") {
                                insideAssembly = false;
                            } else if (cells[0] == L"程序集名") {
                                insideAssembly = true;
                            } else {
                                insideAssembly = false;
                            }
                        }
                        inParamTable = false;  // 退出参数表区域
                    }
                    
                    // 如果是参数表头，进入参数表区域
                    if (isParamTable) {
                        inParamTable = true;
                    }
                    
                    // 确定表头背景色
                    COLORREF headerBg = g_CurrentTheme.tableHeaderBg;
                    if (cells.size() > 0) {
                        if (cells[0] == L"子程序名") {
                            headerBg = clrSubHeaderBg;
                        } else if (cells[0] == L"程序集名") {
                            headerBg = g_CurrentTheme.tableSubHeaderBg;
                        } else if (cells[0] == L"变量名") {
                            headerBg = clrVarHeaderBg;
                        } else if (cells[0] == L"参数名") {
                            headerBg = g_CurrentTheme.tableVarHeaderBg;  // 参数表头背景色
                        }
                    }
                    
                    // 使用预先计算好的列宽
                    std::vector<int> colWidths;
                    bool isAssembly = false;
                    if (currentTableIndex >= 0 && currentTableIndex < (int)tableMaxWidths.size()) {
                        colWidths = tableMaxWidths[currentTableIndex];
                        if (currentTableIndex < (int)tableTypes.size() && tableTypes[currentTableIndex] == 1) {
                            isAssembly = true;
                        }
                    } else {
                        // 如果没有预计算数据，使用默认值
                        for (size_t j = 0; j < cells.size(); j++) {
                            colWidths.push_back(100);
                        }
                    }
                    
                    // 绘制表格单元格
                    int cellX = startX;
                    int cursorCellX = startX;  // 记录光标所在单元格的X位置
                    int cursorInCellCol = 0;  // 光标在当前单元格内的位置
                    
                    // 检查当前行是否有diff标记
                    bool hasDiffBg = false;
                    COLORREF diffBgColor = g_CurrentTheme.bg;
                    if (doc->showDiff && i < doc->diffTypes.size()) {
                        DiffLineType diffType = doc->diffTypes[i];
                        if (diffType == DIFF_ADDED || diffType == DIFF_DELETED) {
                            hasDiffBg = true;
                            diffBgColor = (diffType == DIFF_ADDED) ? g_CurrentTheme.diffAdded : g_CurrentTheme.diffRemoved;
                        }
                    }
                    
                    // 如果在参数表区域内且有超过5列，使用分割绘制
                    if (inParamTable && cells.size() >= 6 && colWidths.size() >= 4) {
                        // 参数表：使用子程序表的4列宽度，将第3列拆分为可空+数组+备注
                        COLORREF checkColor = isHeader ? clrText : g_CurrentTheme.syntaxCheckbox;  // 深红色
                        COLORREF typeColor = isHeader ? clrText : g_CurrentTheme.syntaxType;   // 类型列颜色
                        COLORREF remarkColor = isHeader ? clrText : g_CurrentTheme.syntaxRemark; // 备注列绿色
                        
                        // 绘制第0列：参数名
                        DrawTableCell(cellX, currentY, colWidths[0], rowHeight, 
                                    cells[0].c_str(), clrText, isHeader, headerBg, false, false, diffBgColor, hasDiffBg);
                        cellX += colWidths[0];
                        
                        // 绘制第1列：类型（蓝色）
                        DrawTableCell(cellX, currentY, colWidths[1], rowHeight, 
                                    cells[1].c_str(), typeColor, isHeader, headerBg, false, false, diffBgColor, hasDiffBg);
                        cellX += colWidths[1];
                        
                        // 绘制第2列：参考（复选框，居中，深红色）
                        DrawTableCell(cellX, currentY, colWidths[2], rowHeight, 
                                    cells.size() > 2 ? cells[2].c_str() : L"", checkColor, isHeader, headerBg, !isHeader, !isHeader, diffBgColor, hasDiffBg);
                        cellX += colWidths[2];
                        
                        // 第3列拆分成3部分：可空(30) + 数组(30) + 备注(剩余)
                        int totalCol3Width = colWidths[3];  // 对应子程序表的备注列宽度
                        int nullableWidth = 30;  // 可空列固定30像素
                        int arrayWidth = 30;     // 数组列固定30像素
                        int remarkWidth = totalCol3Width - nullableWidth - arrayWidth;  // 备注列占剩余空间
                        if (remarkWidth < 50) remarkWidth = 50;  // 确保备注列至少50像素
                        
                        // 绘制可空列（参数表第4列）- 复选框，居中，深红色
                        DrawTableCell(cellX, currentY, nullableWidth, rowHeight, 
                                    cells.size() > 3 ? cells[3].c_str() : L"", checkColor, isHeader, headerBg, !isHeader, !isHeader, diffBgColor, hasDiffBg);
                        cellX += nullableWidth;
                        
                        // 绘制数组列（参数表第5列）- 复选框，居中，深红色
                        DrawTableCell(cellX, currentY, arrayWidth, rowHeight, 
                                    cells.size() > 4 ? cells[4].c_str() : L"", checkColor, isHeader, headerBg, !isHeader, !isHeader, diffBgColor, hasDiffBg);
                        cellX += arrayWidth;
                        
                        // 绘制备注列（参数表第6列，绿色）
                        DrawTableCell(cellX, currentY, remarkWidth, rowHeight, 
                                    cells.size() > 5 ? cells[5].c_str() : L"", remarkColor, isHeader, headerBg, false, false, diffBgColor, hasDiffBg);
                        cellX += remarkWidth;
                    } else {
                        // 普通表格行：正常绘制，但第2、第3列（复选框列）居中并使用深红色
                        COLORREF checkColor = g_CurrentTheme.syntaxCheckbox;  // 深红色
                        COLORREF typeColor = isHeader ? clrText : g_CurrentTheme.syntaxType;   // 类型列颜色
                        COLORREF remarkColor = isHeader ? clrText : g_CurrentTheme.syntaxRemark; // 备注列绿色
                        
                        // 判断是否为子程序表（4列）
                        bool isSubProgramTable = false;
                        if (cells.size() == 4 && !inClassVarTable && !isAssembly) {
                            // 可能是子程序表，向上查找确认
                            for (int searchLine = (int)i - 1; searchLine >= (int)startLine; searchLine--) {
                                const std::wstring& headerLine = doc->lines[searchLine];
                                if (headerLine.empty()) continue;
                                if (headerLine.find(L"子程序名") == 0) {
                                    isSubProgramTable = true;
                                    break;
                                }
                                if (headerLine.find(L"参数名") == 0 || headerLine.find(L"变量名") == 0 || headerLine.find(L"程序集名") == 0) {
                                    break;
                                }
                            }
                        }
                        
                        for (size_t j = 0; j < cells.size(); j++) {
                            if (j >= colWidths.size()) break;
                            
                            bool isCheckboxCol = !isAssembly && (j == 2 || j == 3);  // 第2、第3列是复选框列（非程序集表）
                            bool isTypeCol = !isAssembly && (j == 1);  // 第1列是类型列（非程序集表）
                            bool isRemarkCol = false;
                            
                            if (inClassVarTable) {
                                // 程序集变量表 (4列: Name, Type, Array, Note)
                                isTypeCol = (j == 1);
                                isRemarkCol = (j == 3);
                                isCheckboxCol = false;
                            } else if (isAssembly) {
                                // 程序集表特殊处理
                                if (j == 3) isRemarkCol = true; // 第3列是备注
                            } else if (isSubProgramTable) {
                                // 子程序表：只有第2列是复选框，第3列是备注
                                isCheckboxCol = (j == 2);
                                isRemarkCol = (j == 3);
                            } else {
                                // 其他表
                                isRemarkCol = (j == 4);  // 第4列是备注列
                            }
                            
                            COLORREF cellColor = clrText;
                            if (isCheckboxCol && !isHeader) {
                                cellColor = checkColor;  // 复选框列深红色
                            } else if (isTypeCol && !isHeader) {
                                cellColor = typeColor;  // 类型列蓝色
                            } else if (isRemarkCol && !isHeader) {
                                cellColor = remarkColor;  // 备注列绿色
                            }
                            
                            bool centerAlign = isCheckboxCol && !isHeader;
                            
                            DrawTableCell(cellX, currentY, colWidths[j], rowHeight, 
                                        cells[j].c_str(), cellColor, isHeader, headerBg, centerAlign, isCheckboxCol && !isHeader, diffBgColor, hasDiffBg);
                            cellX += colWidths[j];
                        }
                    }
                } else if (IsFlowControlLine(line)) {
                    // 流程控制语句 - 绘制虚线和横线
                    LogFlowLine(L">>> 行 %d 进入 IsFlowControlLine 分支", (int)i);
                    
                    // 先计算嵌套深度
                    int ifNestingDepth = GetNestingDepth(line);
                    int depthOffset = ifNestingDepth * 20;  // 每层嵌套向右偏移20像素
                    
                    // 跳过空格、制表符和嵌套标记字符，获取纯净的命令文本
                    std::wstring trimmedLine = line;
                    size_t trimStart = 0;
                    while (trimStart < trimmedLine.length() && 
                           (trimmedLine[trimStart] == L' ' || trimmedLine[trimStart] == L'\t' ||
                            trimmedLine[trimStart] == L'\u200C' || trimmedLine[trimStart] == L'\u200D' || 
                            trimmedLine[trimStart] == L'\u200B')) {
                        trimStart++;
                    }
                    if (trimStart < trimmedLine.length()) {
                        trimmedLine = trimmedLine.substr(trimStart);
                    }
                    
                    SetTextColor(hdc, clrKeyword);
                    // 隐藏点号，只显示命令本身
                    std::wstring displayText = trimmedLine;  // 使用已去除标记的文本
                    size_t dotPos = displayText.find(L'.');
                    if (dotPos != std::wstring::npos) {
                        displayText.erase(dotPos, 1);  // 删除点号
                    }
                    // 格式化括号内的运算符为全角显示
                    size_t bracketStart = displayText.find(L'(');
                    size_t bracketEnd = displayText.rfind(L')');
                    if (bracketStart != std::wstring::npos && bracketEnd != std::wstring::npos && bracketEnd > bracketStart) {
                        std::wstring beforeBracket = displayText.substr(0, bracketStart + 1);
                        std::wstring bracketContent = displayText.substr(bracketStart + 1, bracketEnd - bracketStart - 1);
                        std::wstring afterBracket = displayText.substr(bracketEnd);
                        bracketContent = FormatExprOperators(bracketContent);
                        displayText = beforeBracket + bracketContent + afterBracket;
                    }
                    // 根据嵌套深度偏移绘制位置
                    TextOutW(hdc, startX + 20 + depthOffset, currentY + (rowHeight - fontSize) / 2, displayText.c_str(), (int)displayText.length());
                    
                    // 检查是否包含"如果("或"如果 ("或"判断("或"判断 ("
                    bool hasElseBranch = (trimmedLine.find(L".如果(") == 0 || trimmedLine.find(L".如果 (") == 0 ||
                                          trimmedLine.find(L".判断(") == 0 || trimmedLine.find(L".判断 (") == 0);
                    
                    // 调试日志：输出行信息
                    LogFlowLine(L"========================================");
                    LogFlowLine(L"行 %d: 原始行='%s'", (int)i, line.c_str());
                    LogFlowLine(L"行 %d: trimmedLine='%s', trimStart=%d", (int)i, trimmedLine.c_str(), (int)trimStart);
                    LogFlowLine(L"行 %d: hasElseBranch=%d", (int)i, hasElseBranch ? 1 : 0);
                    
                    // 输出行的字符编码（用于调试）
                    std::wstring charCodes;
                    for (size_t k = 0; k < line.length() && k < 20; k++) {
                        wchar_t buf[16];
                        swprintf_s(buf, L"U+%04X ", (int)line[k]);
                        charCodes += buf;
                    }
                    LogFlowLine(L"行 %d: 字符编码: %s", (int)i, charCodes.c_str());
                    LogFlowLine(L"行 %d: ifNestingDepth=%d, depthOffset=%d", (int)i, ifNestingDepth, depthOffset);
                    
                    // 绘制横向虚线（使用彩虹色）
                    int lineX = startX + 10 + depthOffset;
                    int textCenterOffset = (rowHeight - fontSize) / 2 + fontSize / 2;  // 文本垂直中心位置
                    COLORREF flowColor = GetFlowLineColor(ifNestingDepth);
                    HPEN hDashPen = CreatePen(PS_DOT, 1, flowColor);
                    HPEN hOldDashPen = (HPEN)SelectObject(hdc, hDashPen);
                    MoveToEx(hdc, lineX, currentY + textCenterOffset, NULL);
                    LineTo(hdc, startX + 18 + depthOffset, currentY + textCenterOffset);
                    
                    if (hasElseBranch) {
                        // .如果() 绘制if-else结构
                        int elseLineIdx = -1;  // 否则分支的行号
                        int lastIndentedLine = -1; // 否则分支的最后一个缩进行
                        int cachedBlockEnd = -1;    // 预计算的流程块结束行
                        
                        // 使用流程块索引缓存获取预计算结果（O(1)替代O(n²)搜索）
                        bool flowIndexHit = false;
                        {
                            auto flowIt = doc->flowBlockIndex.find((int)i);
                            if (flowIt != doc->flowBlockIndex.end()) {
                                elseLineIdx = flowIt->second.elseLineIdx;
                                lastIndentedLine = flowIt->second.lastIndentedLine;
                                cachedBlockEnd = flowIt->second.blockEndLine;
                                flowIndexHit = true;
                            }
                        }

                        // 以下为原始搜索代码（当索引命中时跳过）
                        // 辅助函数：获取行的实际深度
                        auto getLineDepth = [](const std::wstring& checkLine) -> int {
                            if (checkLine.empty()) return 0;
                            if (checkLine[0] != L'\u200C' && checkLine[0] != L'\u200D' && checkLine[0] != L'\u200B') return 0;
                            
                            size_t pos = 0;
                            int depth = 0;
                            
                            // 遍历所有标记字符计算深度
                            // 格式: [分支标记][(\u200B[分支标记])*]
                            // 例如: \u200C = 深度1, \u200C\u200B\u200C = 深度2, \u200C\u200B\u200C\u200B\u200C = 深度3
                            while (pos < checkLine.length()) {
                                wchar_t ch = checkLine[pos];
                                if (ch == L'\u200C' || ch == L'\u200D') {
                                    depth++;
                                    pos++;
                                } else if (ch == L'\u200B') {
                                    // \u200B 是深度分隔符，跳过但不增加深度
                                    pos++;
                                } else {
                                    // 遇到非标记字符，停止
                                    break;
                                }
                            }
                            
                            return depth;
                        };
                        
                        // 辅助函数：检查行是否是否则分支（以\u200D结尾或开头）
                        // 返回值：0=不是，1=是（新格式，以\u200D结尾），2=可能是（旧格式，需要按顺序判断），-1=深度更深应跳过
                        auto checkElseBranchLine = [&getLineDepth](const std::wstring& checkLine, int expectedDepth) -> int {
                            if (checkLine.empty()) return 0;
                            
                            int lineDepth = getLineDepth(checkLine);
                            
                            // 如果行的深度大于期望深度+1，说明是更深层的嵌套，应该跳过
                            if (lineDepth > expectedDepth + 1) {
                                return -1;  // 跳过
                            }
                            
                            // 如果行的深度小于期望深度+1，说明已经离开了当前块
                            if (lineDepth < expectedDepth + 1 && lineDepth > 0) {
                                return 0;  // 不是
                            }
                            
                            // 深度0：直接以\u200D开头且深度为1
                            if (expectedDepth == 0) {
                                if (checkLine[0] == L'\u200D' && lineDepth == 1) return 1;
                                return 0;
                            }
                            
                            // 深度>0：检查是否是期望深度+1的否则分支
                            if (lineDepth == expectedDepth + 1) {
                                // 找到标记的最后一个分支类型字符
                                // 格式: \u200C[\u200B\u200C]*[\u200B]\u200D 表示否则分支
                                size_t pos = 0;
                                wchar_t lastBranchType = L'\0';
                                while (pos < checkLine.length()) {
                                    wchar_t ch = checkLine[pos];
                                    if (ch == L'\u200C' || ch == L'\u200D') {
                                        lastBranchType = ch;
                                        pos++;
                                    } else if (ch == L'\u200B') {
                                        pos++;
                                    } else {
                                        break;
                                    }
                                }
                                
                                // 新格式：最后一个分支标记是\u200D
                                if (lastBranchType == L'\u200D') {
                                    return 1;
                                }
                                // 旧格式：没有分支类型后缀或其他情况
                                if (lastBranchType == L'\0') {
                                    return 2;
                                }
                            }
                            return 0;
                        };
                        
                        // 辅助函数：检查行是否是条件达成分支
                        auto checkTrueBranchLine = [&getLineDepth](const std::wstring& checkLine, int expectedDepth) -> int {
                            if (checkLine.empty()) return 0;
                            
                            int lineDepth = getLineDepth(checkLine);
                            
                            // 如果行的深度大于期望深度+1，说明是更深层的嵌套，应该跳过
                            if (lineDepth > expectedDepth + 1) {
                                return -1;  // 跳过
                            }
                            
                            // 深度0：直接以\u200C开头且深度为1
                            if (expectedDepth == 0) {
                                if (checkLine[0] == L'\u200C' && lineDepth == 1) return 1;
                                return 0;
                            }
                            
                            // 深度>0
                            if (lineDepth == expectedDepth + 1) {
                                // 找到标记的最后一个分支类型字符
                                size_t pos = 0;
                                wchar_t lastBranchType = L'\0';
                                while (pos < checkLine.length()) {
                                    wchar_t ch = checkLine[pos];
                                    if (ch == L'\u200C' || ch == L'\u200D') {
                                        lastBranchType = ch;
                                        pos++;
                                    } else if (ch == L'\u200B') {
                                        pos++;
                                    } else {
                                        break;
                                    }
                                }
                                
                                // 新格式：最后一个分支标记是\u200C
                                if (lastBranchType == L'\u200C') {
                                    return 1;
                                }
                                // 旧格式：没有分支类型后缀
                                if (lastBranchType == L'\0') {
                                    return 2;
                                }
                            }
                            return 0;
                        };
                        
                        // 从.如果()下一行开始查找
                        bool foundTrueBranch = false;  // 用于旧格式判断
                        LogFlowLine(L"行 %d: 开始查找否则分支, ifNestingDepth=%d", (int)i, ifNestingDepth);
                        
                        for (size_t j = i + 1; !flowIndexHit && j < doc->lines.size(); j++) {
                            const std::wstring& checkLine = doc->lines[j];
                            
                            // 输出检查行的字符编码
                            std::wstring checkCharCodes;
                            for (size_t k = 0; k < checkLine.length() && k < 10; k++) {
                                wchar_t buf[16];
                                swprintf_s(buf, L"U+%04X ", (int)checkLine[k]);
                                checkCharCodes += buf;
                            }
                            
                            // 跳过参数行（使用\u2060标记）
                            if (checkLine.length() > 0 && checkLine[0] == L'\u2060') {
                                LogFlowLine(L"  检查行 %d: 参数行，跳过", (int)j);
                                continue;  // 参数行，跳过
                            }
                            
                            if (elseLineIdx == -1) {
                                // 还没找到否则分支，查找否则分支标记
                                int elseResult = checkElseBranchLine(checkLine, ifNestingDepth);
                                int trueResult = checkTrueBranchLine(checkLine, ifNestingDepth);
                                
                                LogFlowLine(L"  检查行 %d: elseResult=%d, trueResult=%d, foundTrueBranch=%d, chars=%s", 
                                    (int)j, elseResult, trueResult, foundTrueBranch ? 1 : 0, checkCharCodes.c_str());
                                
                                // 如果返回-1，表示深层嵌套，需要跳过
                                if (elseResult == -1 || trueResult == -1) {
                                    LogFlowLine(L"  检查行 %d: 深层嵌套行，跳过", (int)j);
                                    continue;
                                }
                                
                                if (elseResult == 1) {
                                    // 新格式，确定是否则分支
                                    elseLineIdx = (int)j;
                                    lastIndentedLine = (int)j;
                                    LogFlowLine(L"  检查行 %d: 找到否则分支（新格式）!", (int)j);
                                } else if (elseResult == 2) {
                                    // 旧格式，需要按顺序判断：第一个是条件达成，第二个是否则
                                    if (foundTrueBranch) {
                                        // 已经找到条件达成分支，这个就是否则分支
                                        elseLineIdx = (int)j;
                                        lastIndentedLine = (int)j;
                                        LogFlowLine(L"  检查行 %d: 找到否则分支（旧格式）!", (int)j);
                                    } else {
                                        // 这是条件达成分支
                                        foundTrueBranch = true;
                                    }
                                } else if (trueResult > 0) {
                                    // 条件达成分支
                                    foundTrueBranch = true;
                                    LogFlowLine(L"  检查行 %d: 条件达成分支", (int)j);
                                } else if (!checkLine.empty() && checkLine[0] != L'\u200C' && checkLine[0] != L'\u200D' && checkLine[0] != L'\u200B') {
                                    // 遇到非标记行（普通代码），继续查找
                                    LogFlowLine(L"  检查行 %d: 非标记行，继续", (int)j);
                                    continue;
                                } else {
                                    LogFlowLine(L"  检查行 %d: 未匹配任何条件", (int)j);
                                }
                            } else {
                                // 已找到否则分支，继续查找所有否则分支行
                                if (checkLine.empty()) {
                                    continue;
                                }

                                // 检查是否是否则分支标记的行（否则分支的延续）
                                int elseResult = checkElseBranchLine(checkLine, ifNestingDepth);
                                if (elseResult == -1) {
                                    // 深层嵌套行，跳过
                                    continue;
                                } else if (elseResult > 0) {
                                    lastIndentedLine = (int)j;  // 更新最后一个否则分支行
                                } else {
                                    // 遇到非否则分支行，结束
                                    LogFlowLine(L"  检查行 %d: 遇到非否则分支行，结束查找", (int)j);
                                    break;
                                }
                            }
                        }
                        
                        LogFlowLine(L"行 %d: 查找结束, elseLineIdx=%d, lastIndentedLine=%d", (int)i, elseLineIdx, lastIndentedLine);
                        
                        if (elseLineIdx > 0 && lastIndentedLine >= elseLineIdx) {
                            LogFlowLine(L"行 %d: 绘制否则分支流程线!", (int)i);

                            // 计算关键位置的Y坐标
                            // .如果()命令行的Y坐标（当前行）
                            int ifY = currentY + textCenterOffset;
                            
                            // 检查下一行是否是参数行（展开状态）
                            int trueBranchStartLine = static_cast<int>(i) + 1;  // 条件达成分支的起始行
                            int skipLines = 0;  // 需要跳过的行数
                            
                            // 跳过所有参数行
                            for (size_t j = i + 1; j < doc->lines.size(); j++) {
                                const std::wstring& checkLine = doc->lines[j];
                                if (checkLine.length() > 0 && checkLine[0] == L'\u2060') {
                                    // 参数行，需要跳过
                                    skipLines++;
                                } else {
                                    // 遇到非参数行，停止
                                    break;
                                }
                            }
                            
                            trueBranchStartLine = static_cast<int>(i) + 1 + skipLines;
                            
                            // 条件达成分支的最后一行（否则分支的前一行）的Y坐标
                            int trueBranchLastLine = elseLineIdx - 1;
                            int trueBranchY = currentY + (trueBranchLastLine - (int)i) * rowHeight + textCenterOffset;
                            
                            // 否则分支第一行（双空格行）的Y坐标
                            int elseY = currentY + (elseLineIdx - (int)i) * rowHeight + textCenterOffset;
                            
                            // 定义两条虚线的X坐标（交错绘制，不合并）
                            int leftX = lineX - 10;   // 左边虚线（从.如果到否则分支的向右箭头）
                            int rightX = lineX + 10;  // 右边虚线（从条件达成到流程结束的向下箭头）
                            
                            // 绘制第一条虚线：从.如果()行到否则分支行的向右箭头
                            // 补全第一行横向虚线空缺 (连接竖线到原横线起点)
                            MoveToEx(hdc, leftX, ifY, NULL);
                            LineTo(hdc, lineX, ifY);

                            // 竖线部分：从.如果()到否则分支
                            MoveToEx(hdc, leftX, ifY, NULL);
                            LineTo(hdc, leftX, elseY);
                            // 横线部分：指向否则分支代码起始位置
                            MoveToEx(hdc, leftX, elseY, NULL);
                            LineTo(hdc, startX + 18 + depthOffset, elseY);
                            // 向右箭头
                            DrawRightArrow(startX + 18 + depthOffset, elseY, flowColor);
                            
                            // 绘制第二条虚线：从条件达成分支到否则分支下方的向下箭头
                            // 先向左绘制横线连接到竖线
                            MoveToEx(hdc, rightX, trueBranchY, NULL);
                            LineTo(hdc, rightX - 13, trueBranchY);
                            // 再向下绘制到否则分支行的底部位置（否则分支结束位置）
                            // 使用 lastIndentedLine 计算底部位置
                            int elseBranchBottomY = currentY + (lastIndentedLine - (int)i) * rowHeight + rowHeight - 5;
                            MoveToEx(hdc, rightX - 13, trueBranchY, NULL);
                            LineTo(hdc, rightX - 13, elseBranchBottomY);
                            
                            // 实时查找下一个流程命令（避免缓存过期问题）
                            int nextFlowCmdLine1 = FindNextFlowCmd(doc, lastIndentedLine + 1, ifNestingDepth);
                            
                            if (nextFlowCmdLine1 >= 0) {
                                // 连接到紧跟的下一个判断/如果命令（往上偏移，与命令自身横线保持间距）
                                int nextCmdY1 = currentY + (nextFlowCmdLine1 - (int)i) * rowHeight + textCenterOffset - 8;
                                LineTo(hdc, rightX - 13, nextCmdY1);
                                MoveToEx(hdc, rightX - 13, nextCmdY1, NULL);
                                LineTo(hdc, startX + 18 + depthOffset, nextCmdY1);
                                DrawRightArrow(startX + 18 + depthOffset, nextCmdY1, flowColor);
                            } else {
                                // 向下箭头（在否则分支行的底部）
                                DrawDownArrow(rightX - 13, elseBranchBottomY, flowColor);
                            }
                        } else {
                            // 没找到完整的if-else结构，绘制单条向下的线
                            LogFlowLine(L"行 %d: 没有找到否则分支，绘制单条线", (int)i);
                            int endY = currentY + rowHeight;
                            int nextFlowCmdLine2 = FindNextFlowCmd(doc, cachedBlockEnd >= 0 ? cachedBlockEnd + 1 : (int)i + 1, ifNestingDepth);
                            
                            // 使用流程块索引缓存计算endY（避免O(n)搜索）
                            if (cachedBlockEnd > (int)i) {
                                if (cachedBlockEnd < (int)doc->lines.size() && 
                                    doc->lines[cachedBlockEnd].length() > 0 && 
                                    doc->lines[cachedBlockEnd][0] == L'\u2060') {
                                    endY = currentY + (cachedBlockEnd - (int)i + 2) * rowHeight;
                                } else {
                                    endY = currentY + (cachedBlockEnd - (int)i + 1) * rowHeight;
                                }
                            }
                            
                            MoveToEx(hdc, lineX, currentY + textCenterOffset, NULL);
                            if (nextFlowCmdLine2 >= 0) {
                                // 连接到紧跟的下一个判断/如果命令（往上偏移，与命令自身横线保持间距）
                                int nextCmdY2 = currentY + (nextFlowCmdLine2 - (int)i) * rowHeight + textCenterOffset - 8;
                                LineTo(hdc, lineX, nextCmdY2);
                                MoveToEx(hdc, lineX, nextCmdY2, NULL);
                                LineTo(hdc, startX + 18 + depthOffset, nextCmdY2);
                                DrawRightArrow(startX + 18 + depthOffset, nextCmdY2, flowColor);
                            } else {
                                LineTo(hdc, lineX, endY);
                                DrawDownArrow(lineX, endY, flowColor);
                            }
                        }
                    } else {
                        // 检查是否是循环首命令（判断循环首/循环判断首），不绘制流程线（由循环尾负责绘制）
                        bool isLoopHead = (trimmedLine.find(L".判断循环首") == 0 || trimmedLine.find(L".循环判断首") == 0 ||
                                            trimmedLine.find(L".计次循环首") == 0 || trimmedLine.find(L".变量循环首") == 0);
                        // 检查是否是循环尾命令，需要绘制向上的流程线
                        bool isLoopTail = (trimmedLine.find(L".判断循环尾") == 0 || trimmedLine.find(L".循环判断尾") == 0 ||
                                           trimmedLine.find(L".计次循环尾") == 0 || trimmedLine.find(L".变量循环尾") == 0);
                        
                        if (isLoopHead) {
                            // 循环首命令：不绘制流程线，由对应的循环尾命令绘制向上的流程线
                        } else if (isLoopTail) {
                            // 循环尾命令：使用流程块索引获取对应的循环首行号
                            int headLineIdx = -1;
                            {
                                auto flowIt = doc->flowBlockIndex.find((int)i);
                                if (flowIt != doc->flowBlockIndex.end()) {
                                    headLineIdx = flowIt->second.loopHeadLine;
                                }
                            }
                            
                            if (headLineIdx >= 0) {
                                // 找到循环首，绘制从循环尾到循环首的向上流程线
                                int headY = currentY - ((int)i - headLineIdx) * rowHeight + textCenterOffset;
                                int tailY = currentY + textCenterOffset;
                                
                                // 竖线：从循环尾向上到循环首
                                MoveToEx(hdc, lineX, tailY, NULL);
                                LineTo(hdc, lineX, headY);
                                
                                // 横线：从竖线顶端向右到"判"字左边
                                int arrowEndX = startX + 18 + depthOffset;
                                MoveToEx(hdc, lineX, headY, NULL);
                                LineTo(hdc, arrowEndX, headY);
                                
                                // 向右箭头（指向"判"字左侧）
                                DrawRightArrow(arrowEndX, headY, flowColor);
                            }
                        } else {
                            // .如果真 或其他流程控制，绘制单条线
                            // 使用流程块索引缓存获取结束行和下一个流程命令
                            int endY = currentY + rowHeight;  // 默认至少延伸一行
                            int nextFlowCmdLine3 = -1;
                            
                            {
                                auto flowIt = doc->flowBlockIndex.find((int)i);
                                if (flowIt != doc->flowBlockIndex.end()) {
                                    int blockEnd = flowIt->second.blockEndLine;
                                    nextFlowCmdLine3 = FindNextFlowCmd(doc, blockEnd > (int)i ? blockEnd + 1 : (int)i + 1, ifNestingDepth);
                                    if (blockEnd > (int)i) {
                                        if (blockEnd < (int)doc->lines.size() && 
                                            doc->lines[blockEnd].length() > 0 && 
                                            doc->lines[blockEnd][0] == L'\u2060') {
                                            endY = currentY + (blockEnd - (int)i + 2) * rowHeight;
                                        } else {
                                            endY = currentY + (blockEnd - (int)i + 1) * rowHeight;
                                        }
                                    }
                                }
                            }
                            
                            MoveToEx(hdc, lineX, currentY + textCenterOffset, NULL);
                            if (nextFlowCmdLine3 >= 0) {
                                // 连接到紧跟的下一个判断/如果命令（往上偏移，与命令自身横线保持间距）
                                int nextCmdY3 = currentY + (nextFlowCmdLine3 - (int)i) * rowHeight + textCenterOffset - 8;
                                LineTo(hdc, lineX, nextCmdY3);
                                MoveToEx(hdc, lineX, nextCmdY3, NULL);
                                LineTo(hdc, startX + 18 + depthOffset, nextCmdY3);
                                DrawRightArrow(startX + 18 + depthOffset, nextCmdY3, flowColor);
                            } else {
                                LineTo(hdc, lineX, endY);
                                // 绘制向下箭头
                                DrawDownArrow(lineX, endY, flowColor);
                            }
                        }
                    }
                    
                    SelectObject(hdc, hOldDashPen);
                    DeleteObject(hDashPen);
                } else if (line.length() > 0 && line[0] == L'\u2060') {
                    // 参数行（命令展开的参数输入行）
                    
                    // 计算嵌套层级（前导\u2060的数量，1=一级参数，2=二级嵌套参数）
                    int nestingLevel = 0;
                    while (nestingLevel < (int)line.length() && line[nestingLevel] == L'\u2060') {
                        nestingLevel++;
                    }
                    
                    int paramTextX = startX + 20;  // 默认位置
                    
                    // 绘制从上一行命令括号到参数行的虚线
                    if (i > 0) {
                        // 找到上一行命令行（可能需要向上查找多行，跳过其他参数行）
                        int cmdLineIndex = (int)i - 1;
                        while (cmdLineIndex >= 0 && doc->lines[cmdLineIndex].length() > 0 && doc->lines[cmdLineIndex][0] == L'\u2060') {
                            cmdLineIndex--;
                        }
                        
                        // 对于嵌套参数行，找到父参数行（嵌套层级少一的最近行）
                        int parentParamIndex = -1;
                        if (nestingLevel >= 2) {
                            for (int k = (int)i - 1; k > cmdLineIndex; k--) {
                                const std::wstring& kLine = doc->lines[k];
                                int kLevel = 0;
                                while (kLevel < (int)kLine.length() && kLine[kLevel] == L'\u2060') kLevel++;
                                if (kLevel < nestingLevel) {
                                    parentParamIndex = k;
                                    break;
                                }
                            }
                        }
                        
                        if (cmdLineIndex >= 0) {
                            const std::wstring& prevLine = doc->lines[cmdLineIndex];
                            // 找到括号位置
                            size_t leftBracket = prevLine.find(L'(');
                            bool isAssignment = (leftBracket == std::wstring::npos) && IsAssignmentLine(prevLine);
                            if (leftBracket != std::wstring::npos || isAssignment) {
                                int textCenterOffset = (rowHeight - fontSize) / 2 + fontSize / 2;
                                int prevLineY = currentY - (int)(i - cmdLineIndex) * rowHeight;
                                int prevLineBottom = prevLineY + rowHeight;  // 上一行的底部
                                int currY = currentY + textCenterOffset;  // 当前行的中心
                                
                                // 计算括号的X坐标
                                std::wstring cmdText = prevLine;
                                
                                // 检查是否是流程控制内的行（使用特殊字符标记），获取嵌套深度
                                size_t cmdMarkerLen = 0;
                                int cmdNestDepth = GetNestingDepth(cmdText, &cmdMarkerLen);
                                bool isIndentedCmd = (cmdNestDepth > 0);
                                int cmdStartX = isIndentedCmd ? (startX + 20 + (cmdNestDepth - 1) * 20) : startX;
                                
                                // 如果是流程控制内的行，去掉所有标记字符
                                if (isIndentedCmd) {
                                    cmdText = cmdText.substr(cmdMarkerLen);
                                    if (!isAssignment) leftBracket = cmdText.find(L'(');
                                }
                                
                                // 计算命令行缩进
                                int cmdIndent = 0;
                                size_t pos = 0;
                                while (pos < cmdText.length() && (cmdText[pos] == L' ' || cmdText[pos] == L'\t')) {
                                    if (cmdText[pos] == L'\t') cmdIndent += 20;
                                    else cmdIndent += 4;
                                    pos++;
                                }
                                
                                // 检查是否是流程控制命令（以点号开头，去掉点号后的位置）
                                size_t dotPos = cmdText.find(L'.');
                                if (dotPos != std::wstring::npos) {
                                    cmdText.erase(dotPos, 1);
                                    if (leftBracket != std::wstring::npos && leftBracket > 0) leftBracket--;  // 调整括号位置
                                }
                                
                                // 计算两个中文字符的宽度（用于参数文本位置）
                                SIZE twoChineseCharSize;
                                GetTextExtentPoint32W(hdc, L"中文", 2, &twoChineseCharSize);
                                
                                // 参数文本位置：命令文本起始位置 + 两个中文字符宽度
                                paramTextX = cmdStartX + cmdIndent + twoChineseCharSize.cx;
                                
                                // 计算四个中文字符的宽度（用于流程线位置）
                                SIZE fourChineseCharSize;
                                GetTextExtentPoint32W(hdc, L"中文中文", 4, &fourChineseCharSize);
                                
                                // 流程线X位置：命令文本起始位置 + 四个中文字符宽度
                                int flowLineX = cmdStartX + cmdIndent + fourChineseCharSize.cx;
                                
                                // 计算横线终点（参数文本实际开始位置，前面有6个空格）
                                SIZE prefixSize;
                                GetTextExtentPoint32W(hdc, L"      ", 6, &prefixSize);
                                int arrowX = paramTextX + prefixSize.cx;
                                
                                // 嵌套参数行：增加额外右移偏移量
                                if (nestingLevel >= 2) {
                                    SIZE nestOffset;
                                    GetTextExtentPoint32W(hdc, L"中文中文", 4, &nestOffset);
                                    int extraOffset = (nestingLevel - 1) * nestOffset.cx;
                                    paramTextX += extraOffset;
                                    flowLineX += extraOffset;
                                    arrowX = paramTextX + prefixSize.cx;
                                    
                                    // 流程线连接到父参数行而非命令行
                                    if (parentParamIndex >= 0) {
                                        prevLineY = currentY - (int)(i - parentParamIndex) * rowHeight;
                                        prevLineBottom = prevLineY + rowHeight;
                                    }
                                }
                                
                                // 绘制虚线：从参数文本位置向下，然后向右到参数内容位置
                                HPEN hDashPen = CreatePen(PS_DOT, 1, g_CurrentTheme.textDim);
                                HPEN hOldPen = (HPEN)SelectObject(hdc, hDashPen);
                                
                                // 垂直线：从上一行底部到当前行中心
                                MoveToEx(hdc, flowLineX, prevLineBottom, NULL);
                                LineTo(hdc, flowLineX, currY);
                                
                                // 水平线：从垂直线到参数文本前
                                MoveToEx(hdc, flowLineX, currY, NULL);
                                LineTo(hdc, arrowX, currY);
                                
                                SelectObject(hdc, hOldPen);
                                DeleteObject(hDashPen);
                                
                                // 绘制向上的实心三角形箭头（嵌套参数行：第一个子参数行绘制；一级参数行：第一个参数行绘制）
                                bool drawArrow = false;
                                if (nestingLevel >= 2 && parentParamIndex >= 0) {
                                    drawArrow = ((int)i == parentParamIndex + 1);
                                } else {
                                    drawArrow = ((int)i == cmdLineIndex + 1);
                                }
                                if (drawArrow) {
                                    POINT upArrow[3];
                                    upArrow[0] = {flowLineX, prevLineBottom - 5};      // 顶点（向上）
                                    upArrow[1] = {flowLineX - 5, prevLineBottom};      // 左下角
                                    upArrow[2] = {flowLineX + 5, prevLineBottom};      // 右下角
                                    HBRUSH hArrowBrush = CreateSolidBrush(g_CurrentTheme.textDim);
                                    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hArrowBrush);
                                    Polygon(hdc, upArrow, 3);
                                    SelectObject(hdc, hOldBrush);
                                    DeleteObject(hArrowBrush);
                                }
                            }
                        }
                    }
                    
                    SetTextColor(hdc, g_CurrentTheme.syntaxVariable);  // 浅蓝色
                    // 提取参数名和参数值（格式：\u2060参数名:参数值，嵌套行有多个\u2060前缀）
                    std::wstring lineContent = line.substr(nestingLevel);  // 跳过所有前导\u2060标记
                    size_t colonPos = lineContent.find(L':');
                    std::wstring paramName, paramValue;
                    if (colonPos != std::wstring::npos) {
                        paramName = lineContent.substr(0, colonPos);
                        paramValue = lineContent.substr(colonPos + 1);
                    } else {
                        paramName = lineContent;
                        paramValue = L"";
                    }
                    
                    // 检查参数值是否包含运算表达式，如果是则绘制展开/折叠按钮
                    {
                        std::wstring trimmedValue = paramValue;
                        while (!trimmedValue.empty() && (trimmedValue.front() == L' ' || trimmedValue.front() == L'\t')) trimmedValue.erase(0, 1);
                        while (!trimmedValue.empty() && (trimmedValue.back() == L' ' || trimmedValue.back() == L'\t')) trimmedValue.pop_back();
                        ExprOperatorInfo checkOpInfo;
                        if (ParseExpressionOperator(trimmedValue, checkOpInfo)) {
                            while (doc->parametersExpanded.size() <= i) {
                                doc->parametersExpanded.push_back(false);
                            }
                            bool isExpanded = doc->parametersExpanded[i];
                            
                            int foldBtnSize = 10;
                            int foldBtnX = 58;
                            int foldBtnY = currentY + (rowHeight - foldBtnSize) / 2;
                            
                            HPEN hFoldPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.textDim);
                            HPEN hOldFoldPen = (HPEN)SelectObject(hdc, hFoldPen);
                            HBRUSH hOldFoldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
                            Rectangle(hdc, foldBtnX, foldBtnY, foldBtnX + foldBtnSize, foldBtnY + foldBtnSize);
                            
                            MoveToEx(hdc, foldBtnX + 2, foldBtnY + foldBtnSize / 2, NULL);
                            LineTo(hdc, foldBtnX + foldBtnSize - 2, foldBtnY + foldBtnSize / 2);
                            
                            if (!isExpanded) {
                                MoveToEx(hdc, foldBtnX + foldBtnSize / 2, foldBtnY + 2, NULL);
                                LineTo(hdc, foldBtnX + foldBtnSize / 2, foldBtnY + foldBtnSize - 2);
                            }
                            
                            SelectObject(hdc, hOldFoldBrush);
                            SelectObject(hdc, hOldFoldPen);
                            DeleteObject(hFoldPen);
                        }
                    }
                    
                    // 格式化参数值中的运算符为全角显示
                    std::wstring displayValue = FormatExprOperators(paramValue);
                    std::wstring displayText = L"      ※" + paramName + L"：" + displayValue;
                    TextOutW(hdc, paramTextX, currentY + (rowHeight - fontSize) / 2, displayText.c_str(), (int)displayText.length());
                } else if (line.length() > 0 && line[0] == L'\'') {
                    // 注释行
                    SetTextColor(hdc, clrComment);
                    TextOutW(hdc, startX, currentY + (rowHeight - fontSize) / 2, line.c_str(), (int)line.length());
                } else {
                    // 普通代码行（包括支持库命令、返回语句等）
                    LogFlowLine(L">>> 行 %d 进入普通代码行分支", (int)i);
                    
                    // 绘制diff背景（如果启用了diff视图）- 只显示背景色，不显示+/-符号
                    if (doc->showDiff && i < doc->diffTypes.size()) {
                        DiffLineType diffType = doc->diffTypes[i];
                        if (diffType == DIFF_ADDED || diffType == DIFF_DELETED) {
                            // 绘制背景色（删除行：深红色，新增行：深绿色），只绘制代码区域（从startX开始）
                            COLORREF bgColor = (diffType == DIFF_ADDED) ? g_CurrentTheme.diffAdded : g_CurrentTheme.diffRemoved;
                            HBRUSH hDiffBrush = CreateSolidBrush(bgColor);
                            RECT diffRect = {startX, currentY, rect.right, currentY + rowHeight};  // 从startX开始，不覆盖行号区域
                            FillRect(hdc, &diffRect, hDiffBrush);
                            DeleteObject(hDiffBrush);
                        }
                    }
                    
                    int indent = 0;
                    size_t pos = 0;
                    
                    // 使用原始行内容
                    std::wstring displayLine = line;
                    
                    // 使用 GetNestingDepth 获取嵌套深度和标记长度
                    size_t markerLen = 0;
                    int nestingDepth = GetNestingDepth(displayLine, &markerLen);
                    
                    if (nestingDepth > 0) {
                        // 流程控制内的行
                        // 跳过标记字符
                        pos = markerLen;
                        
                        std::wstring contentPart = displayLine.substr(pos);
                        
                        // 计算基础缩进：每层嵌套增加20像素
                        int baseIndent = nestingDepth * 20;
                        
                        // 检查是否是嵌套的流程控制命令（以点号开头）
                        bool isNestedFlowControl = false;
                        if (!contentPart.empty() && contentPart[0] == L'.') {
                            // 检查是否是流程控制命令
                            std::wstring tempLine = contentPart;  // 临时行用于检查
                            isNestedFlowControl = IsFlowControlLine(tempLine);
                        }
                        
                        if (isNestedFlowControl) {
                            // 嵌套的流程控制命令，在缩进位置绘制完整流程线并跳过点号
                            SetTextColor(hdc, clrKeyword);
                            
                            // 跳过点号，文本绘制在更深的缩进位置
                            int textIndent = baseIndent + 20;  // 嵌套流程命令再缩进20
                            std::wstring displayText = contentPart.substr(1);
                            // 格式化括号内的运算符为全角显示
                            size_t nBracketStart = displayText.find(L'(');
                            size_t nBracketEnd = displayText.rfind(L')');
                            if (nBracketStart != std::wstring::npos && nBracketEnd != std::wstring::npos && nBracketEnd > nBracketStart) {
                                std::wstring beforeBkt = displayText.substr(0, nBracketStart + 1);
                                std::wstring bktContent = displayText.substr(nBracketStart + 1, nBracketEnd - nBracketStart - 1);
                                std::wstring afterBkt = displayText.substr(nBracketEnd);
                                bktContent = FormatExprOperators(bktContent);
                                displayText = beforeBkt + bktContent + afterBkt;
                            }
                            TextOutW(hdc, startX + textIndent, currentY + (rowHeight - fontSize) / 2, displayText.c_str(), (int)displayText.length());
                            
                            // 绘制横向虚线（在缩进位置，使用彩虹色）
                            int lineX = startX + baseIndent + 10;  // 基于baseIndent计算虚线起点
                            int textCenterOffset = (rowHeight - fontSize) / 2 + fontSize / 2;
                            COLORREF nestedFlowColor = GetFlowLineColor(nestingDepth);
                            HPEN hDashPen = CreatePen(PS_DOT, 1, nestedFlowColor);
                            HPEN hOldDashPen = (HPEN)SelectObject(hdc, hDashPen);
                            MoveToEx(hdc, lineX, currentY + textCenterOffset, NULL);
                            LineTo(hdc, startX + baseIndent + 18, currentY + textCenterOffset);
                            
                            // 查找嵌套流程控制内的代码行
                            // 嵌套流程块的范围：从当前行到下一个非嵌套代码行或流程控制命令
                            int endY = currentY + rowHeight;  // 默认至少延伸一行
                            
                            // 当前嵌套流程控制的深度（会在下一行创建更深一层的嵌套）
                            int currentFlowDepth = nestingDepth + 1;
                            
                            for (size_t j = i + 1; j < doc->lines.size(); j++) {
                                const std::wstring& checkLine = doc->lines[j];
                                
                                // 遇到参数行（\u2060），跳过但更新endY
                                if (checkLine.length() > 0 && checkLine[0] == L'\u2060') {
                                    endY = currentY + (int)(j - i + 1) * rowHeight;
                                    continue;
                                }
                                
                                // 获取检查行的嵌套深度
                                size_t checkMarkerLen = 0;
                                int checkDepth = GetNestingDepth(checkLine, &checkMarkerLen);
                                
                                if (checkDepth >= currentFlowDepth) {
                                    // 嵌套深度等于或大于当前流程控制的内部深度，属于当前嵌套流程块
                                    endY = currentY + (int)(j - i + 1) * rowHeight;
                                } else if (checkDepth > 0 && checkDepth < currentFlowDepth) {
                                    // 嵌套深度小于当前流程控制的内部深度
                                    // 检查是否是同级或更高级的流程控制命令
                                    std::wstring innerContent = checkLine.substr(checkMarkerLen);
                                    if (!innerContent.empty() && innerContent[0] == L'.') {
                                        // 遇到另一个流程控制命令，嵌套流程块结束
                                        break;
                                    }
                                    // 普通浅层嵌套行，嵌套流程块结束
                                    break;
                                } else if (checkLine.empty()) {
                                    // 空行，继续查找
                                    continue;
                                } else {
                                    // 遇到非缩进行，结束查找
                                    break;
                                }
                            }
                            
                            // 绘制竖向虚线到流程块结束
                            MoveToEx(hdc, lineX, currentY + textCenterOffset, NULL);
                            LineTo(hdc, lineX, endY);
                            
                            // 绘制向下箭头（使用彩虹色）
                            POINT downArrow[3];
                            downArrow[0] = {lineX, endY};           // 顶点（向下）
                            downArrow[1] = {lineX - 4, endY - 6};   // 左上角
                            downArrow[2] = {lineX + 4, endY - 6};   // 右上角
                            HBRUSH hArrowBrush = CreateSolidBrush(nestedFlowColor);
                            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hArrowBrush);
                            HPEN hNoPen = (HPEN)GetStockObject(NULL_PEN);
                            SelectObject(hdc, hNoPen);
                            Polygon(hdc, downArrow, 3);
                            SelectObject(hdc, hOldBrush);
                            DeleteObject(hArrowBrush);
                            
                            SelectObject(hdc, hOldDashPen);
                            DeleteObject(hDashPen);
                        } else {
                            // 普通代码行（非嵌套流程控制）
                            // 检查是否有行内注释
                            size_t commentPos = contentPart.find(L'\'');
                            
                            if (commentPos != std::wstring::npos && commentPos > 0) {
                                // 有行内注释，分两部分绘制
                                std::wstring codePart = contentPart.substr(0, commentPos);
                                std::wstring commentPartStr = contentPart.substr(commentPos);
                                
                                // 绘制代码部分
                                size_t cmdEnd = 0;
                                while (cmdEnd < codePart.length() && 
                                       codePart[cmdEnd] != L' ' && codePart[cmdEnd] != L'\t' && 
                                       codePart[cmdEnd] != L'(' && codePart[cmdEnd] != L')') {
                                    cmdEnd++;
                                }
                                std::wstring cmdName = codePart.substr(0, cmdEnd);
                                const LibraryCommand* cmd = LibraryParser::GetInstance().FindCommand(cmdName);
                                
                                if (cmd) {
                                    SetTextColor(hdc, clrKeyword);
                                } else {
                                    SetTextColor(hdc, clrText);
                                }
                                TextOutW(hdc, startX + baseIndent, currentY + (rowHeight - fontSize) / 2, codePart.c_str(), (int)codePart.length());
                                
                                // 计算代码部分的宽度
                                SIZE codeSize;
                                GetTextExtentPoint32W(hdc, codePart.c_str(), (int)codePart.length(), &codeSize);
                                
                                // 绘制注释部分（绿色）
                                SetTextColor(hdc, clrComment);
                                TextOutW(hdc, startX + baseIndent + codeSize.cx, currentY + (rowHeight - fontSize) / 2, commentPartStr.c_str(), (int)commentPartStr.length());
                            } else if (commentPos == 0) {
                                // 整行注释
                                SetTextColor(hdc, clrComment);
                                TextOutW(hdc, startX + baseIndent, currentY + (rowHeight - fontSize) / 2, contentPart.c_str(), (int)contentPart.length());
                            } else {
                                // 普通代码行（无注释）
                                size_t cmdEnd = 0;
                                while (cmdEnd < contentPart.length() && 
                                       contentPart[cmdEnd] != L' ' && contentPart[cmdEnd] != L'\t' && 
                                       contentPart[cmdEnd] != L'(' && contentPart[cmdEnd] != L')') {
                                    cmdEnd++;
                                }
                                std::wstring cmdName = contentPart.substr(0, cmdEnd);
                                const LibraryCommand* cmd = LibraryParser::GetInstance().FindCommand(cmdName);
                                
                                if (cmd) {
                                    SetTextColor(hdc, clrKeyword);
                                } else {
                                    SetTextColor(hdc, clrText);
                                }
                                TextOutW(hdc, startX + baseIndent, currentY + (rowHeight - fontSize) / 2, contentPart.c_str(), (int)contentPart.length());
                            }
                        }
                    } else {
                        // 普通代码行，按正常缩进绘制
                        while (pos < displayLine.length() && (displayLine[pos] == L' ' || displayLine[pos] == L'\t')) {
                            if (displayLine[pos] == L'\t') indent += 20;
                            else indent += 4;
                            pos++;
                        }
                        
                        // 检查是否是流程控制命令行（以点号开头且需要流程线）
                        bool isFlowControlLine = false;
                        if (pos < displayLine.length() && displayLine[pos] == L'.') {
                            // 可能是流程控制命令，使用 IsFlowControlLine 函数检查
                            isFlowControlLine = IsFlowControlLine(displayLine);
                            if (isFlowControlLine) {
                                // 跳过点号，不显示它
                                pos++;
                            }
                        }
                        
                        // 获取要绘制的内容部分
                        std::wstring contentPart = displayLine.substr(pos);
                        
                        // 检查是否有行内注释
                        size_t commentPos = contentPart.find(L'\'');
                        
                        if (commentPos != std::wstring::npos && commentPos > 0) {
                            // 有行内注释，分两部分绘制
                            std::wstring codePart = contentPart.substr(0, commentPos);
                            std::wstring commentPartStr = contentPart.substr(commentPos);
                            
                            // 绘制代码部分
                            size_t cmdEnd = 0;
                            while (cmdEnd < codePart.length() && 
                                   codePart[cmdEnd] != L' ' && codePart[cmdEnd] != L'\t' && 
                                   codePart[cmdEnd] != L'(' && codePart[cmdEnd] != L')') {
                                cmdEnd++;
                            }
                            std::wstring cmdName = codePart.substr(0, cmdEnd);
                            const LibraryCommand* cmd = LibraryParser::GetInstance().FindCommand(cmdName);
                            
                            if (cmd) {
                                SetTextColor(hdc, clrKeyword);
                            } else {
                                SetTextColor(hdc, clrText);
                            }
                            TextOutW(hdc, startX + indent, currentY + (rowHeight - fontSize) / 2, codePart.c_str(), (int)codePart.length());
                            
                            // 计算代码部分的宽度
                            SIZE codeSize;
                            GetTextExtentPoint32W(hdc, codePart.c_str(), (int)codePart.length(), &codeSize);
                            
                            // 绘制注释部分（绿色）
                            SetTextColor(hdc, clrComment);
                            TextOutW(hdc, startX + indent + codeSize.cx, currentY + (rowHeight - fontSize) / 2, commentPartStr.c_str(), (int)commentPartStr.length());
                        } else if (commentPos == 0) {
                            // 整行注释（在缩进之后）
                            SetTextColor(hdc, clrComment);
                            TextOutW(hdc, startX + indent, currentY + (rowHeight - fontSize) / 2, contentPart.c_str(), (int)contentPart.length());
                        } else {
                            // 普通代码行（无注释）
                            size_t cmdEnd = 0;
                            while (cmdEnd < contentPart.length() && 
                                   contentPart[cmdEnd] != L' ' && contentPart[cmdEnd] != L'\t' && 
                                   contentPart[cmdEnd] != L'(' && contentPart[cmdEnd] != L')') {
                                cmdEnd++;
                            }
                            std::wstring cmdName = contentPart.substr(0, cmdEnd);
                            const LibraryCommand* cmd = LibraryParser::GetInstance().FindCommand(cmdName);
                            
                            if (cmd) {
                                SetTextColor(hdc, clrKeyword);
                            } else {
                                SetTextColor(hdc, clrText);
                            }
                            TextOutW(hdc, startX + indent, currentY + (rowHeight - fontSize) / 2, contentPart.c_str(), (int)contentPart.length());
                        }
                    }
                }
                
                // 绘制光标
                if ((int)i == doc->cursorLine) {
                    caretX = startX;
                    caretY = currentY + (rowHeight - fontSize) / 2;
                    
                    if (line.find(L"\t") != std::wstring::npos) {
                        // 表格行：需要计算光标在哪个单元格内
                        std::vector<std::wstring> cursorCells;
                        size_t start = 0, pos = 0;
                        while ((pos = line.find(L'\t', start)) != std::wstring::npos) {
                            cursorCells.push_back(line.substr(start, pos - start));
                            start = pos + 1;
                        }
                        cursorCells.push_back(line.substr(start));
                        
                        // 计算光标在哪个单元格
                        int charCount = 0;
                        int cellIndex = 0;
                        for (size_t j = 0; j < cursorCells.size(); j++) {
                            int cellEnd = charCount + (int)cursorCells[j].length();
                            if (doc->cursorCol <= cellEnd || j == cursorCells.size() - 1) {
                                cellIndex = (int)j;
                                break;
                            }
                            charCount = cellEnd + 1;  // +1 为制表符
                        }
                        
                        // 使用与绘制相同的列宽
                        std::vector<int> cursorColWidths;
                        
                        // 重新计算光标所在行的表格索引
                        int cursorTableIndex = -1;
                        bool cursorInsideAssembly = false;
                        // 检查是否有版本行
                        size_t searchStart = (!doc->lines.empty() && doc->lines[0].find(L"版本\t") == 0) ? 1 : 0;
                        
                        for (size_t searchLine = searchStart; searchLine <= i; searchLine++) {
                            const std::wstring& searchLineTxt = doc->lines[searchLine];
                            if (searchLineTxt.empty()) {
                                cursorInsideAssembly = false;
                                continue;
                            }
                            
                            if (searchLineTxt.find(L"\t") != std::wstring::npos) {
                                std::vector<std::wstring> searchCells;
                                size_t start = 0, pos = 0;
                                while ((pos = searchLineTxt.find(L'\t', start)) != std::wstring::npos) {
                                    searchCells.push_back(searchLineTxt.substr(start, pos - start));
                                    start = pos + 1;
                                }
                                searchCells.push_back(searchLineTxt.substr(start));
                                
                                bool isHeader = (searchCells.size() > 0 && 
                                               (searchCells[0] == L"程序集名" || searchCells[0] == L"子程序名" || 
                                                searchCells[0] == L"参数名" || searchCells[0] == L"变量名"));
                                bool isParamHeader = (searchCells.size() > 0 && searchCells[0] == L"参数名");
                                bool isAssemblyVarHeader = (searchCells.size() > 0 && searchCells[0] == L"变量名" && cursorInsideAssembly);
                                
                                // 如果是表头但不是参数表头且不是程序集变量表头，递增索引
                                if (isHeader && !isParamHeader && !isAssemblyVarHeader) {
                                    // 每个表格都递增索引
                                    cursorTableIndex++;
                                    
                                    // 更新程序集状态
                                    if (searchCells[0] == L"程序集名") {
                                        cursorInsideAssembly = true;
                                    } else if (searchCells[0] == L"子程序名" || searchCells[0] == L"变量名") {
                                        cursorInsideAssembly = false;
                                    }
                                }
                            }
                        }
                        
                        // 获取列宽
                        if (cursorTableIndex >= 0 && cursorTableIndex < (int)tableMaxWidths.size()) {
                            cursorColWidths = tableMaxWidths[cursorTableIndex];
                        } else {
                            for (size_t j = 0; j < cursorCells.size(); j++) {
                                cursorColWidths.push_back(100);
                            }
                        }
                        
                        // 计算光标X位置
                        int cursorCellX = startX;
                        
                        // 判断光标所在行是否为参数表（需要分割最后一列）
                        bool inParamTableCursor = false;
                        // 向上查找最近的表头判断是否为参数表
                        for (int searchLine = (int)i - 1; searchLine >= 0; searchLine--) {
                            const std::wstring& headerLine = doc->lines[searchLine];
                            if (headerLine.empty()) continue;  // 跳过空行继续查找
                            if (headerLine.find(L"参数名") != std::wstring::npos) {
                                inParamTableCursor = true;
                                break;
                            }
                            if (headerLine.find(L"子程序名") != std::wstring::npos || 
                                headerLine.find(L"变量名") != std::wstring::npos) {
                                break;
                            }
                        }
                        
                        for (int j = 0; j < cellIndex && j < (int)cursorColWidths.size(); j++) {
                            // 参数表的第3列（索引3）是组合列（可空+数组+备注）
                            if (inParamTableCursor) {
                                if (j == 3) {
                                    if (cellIndex == 4) {
                                        // 光标在数组列，累加可空列宽度(30)
                                        cursorCellX += 30;
                                    } else if (cellIndex == 5) {
                                        // 光标在备注列，累加可空列(30)+数组列(30)
                                        cursorCellX += 30 + 30;
                                    } else if (cellIndex > 5) {
                                        // 光标在更后面的列（如果有），累加完整宽度
                                        cursorCellX += cursorColWidths[j];
                                    }
                                    // 如果cellIndex == 3 (可空列)，不累加任何宽度
                                    continue;
                                }
                                // 参数表的列4和列5（数组和备注）实际上包含在列3中，不应重复累加
                                if (j == 4 || j == 5) {
                                    continue;
                                }
                            }
                            cursorCellX += cursorColWidths[j];
                        }
                        
                        // 计算光标在单元格内的位置
                        int inCellPos = doc->cursorCol - charCount;
                        if (cellIndex < (int)cursorCells.size()) {
                            std::wstring cellText = cursorCells[cellIndex];
                            // 限制光标位置不超过单元格文本长度
                            inCellPos = (std::min)(inCellPos, (int)cellText.length());
                            std::wstring beforeCursor = cellText.substr(0, inCellPos);
                            SIZE textSize;
                            GetTextExtentPoint32W(hdc, beforeCursor.c_str(), (int)beforeCursor.length(), &textSize);
                            caretX = cursorCellX + 3 + textSize.cx;  // 3是单元格内边距
                        }
                    } else {
                        // 非表格行：使用原有逻辑，但需考虑特殊格式
                        // 根据行类型调整起始位置
                        bool isParamLine = (line.length() > 0 && line[0] == L'\u2060');
                        
                        if (isParamLine) {
                            // 参数行：光标只能在冒号后面（参数值区域）
                            // 格式：\u2060参数名:参数值，显示为 "      ※参数名：参数值"
                            size_t colonPos = line.find(L':');
                            if (colonPos == std::wstring::npos) {
                                colonPos = line.length();
                            }
                            int minCursorCol = (int)colonPos + 1;  // 光标最小位置在冒号之后
                            
                            // 确保光标不在前缀区域
                            if (doc->cursorCol < minCursorCol) {
                                doc->cursorCol = minCursorCol;
                            }
                            
                            // 计算参数行的实际显示位置（与渲染逻辑完全一致）
                            int paramTextX = startX + 20;
                            // 查找命令行（可能需要向上查找多行，跳过其他参数行）
                            int cmdLineIndex = (int)i - 1;
                            while (cmdLineIndex >= 0 && doc->lines[cmdLineIndex].length() > 0 && doc->lines[cmdLineIndex][0] == L'\u2060') {
                                cmdLineIndex--;
                            }
                            if (cmdLineIndex >= 0) {
                                const std::wstring& cmdLine = doc->lines[cmdLineIndex];
                                size_t leftBracket = cmdLine.find(L'(');
                                if (leftBracket != std::wstring::npos) {
                                    // 计算括号X坐标（与绘制逻辑完全一致）
                                    std::wstring cmdText = cmdLine;
                                    
                                    // 检查是否是流程控制内的行（使用特殊字符标记），获取嵌套深度
                                    size_t cmdMarkerLen2 = 0;
                                    int cmdNestDepth2 = GetNestingDepth(cmdText, &cmdMarkerLen2);
                                    bool isIndentedCmd = (cmdNestDepth2 > 0);
                                    int cmdStartX = isIndentedCmd ? (startX + 20 + (cmdNestDepth2 - 1) * 20) : startX;
                                    
                                    // 如果是流程控制内的行，去掉所有标记字符
                                    if (isIndentedCmd) {
                                        cmdText = cmdText.substr(cmdMarkerLen2);
                                        leftBracket = cmdText.find(L'(');
                                    }
                                    
                                    // 计算命令行缩进
                                    int cmdIndent = 0;
                                    size_t pos = 0;
                                    while (pos < cmdText.length() && (cmdText[pos] == L' ' || cmdText[pos] == L'\t')) {
                                        if (cmdText[pos] == L'\t') cmdIndent += 20;
                                        else cmdIndent += 4;
                                        pos++;
                                    }
                                    
                                    // 检查是否是流程控制命令（以点号开头，去掉点号后的位置）
                                    size_t dotPos = cmdText.find(L'.');
                                    if (dotPos != std::wstring::npos) {
                                        cmdText.erase(dotPos, 1);
                                        if (leftBracket > 0) leftBracket--;
                                    }
                                    
                                    // 计算两个中文字符的宽度（用于参数文本位置）
                                    SIZE twoChineseCharSize;
                                    GetTextExtentPoint32W(hdc, L"中文", 2, &twoChineseCharSize);
                                    
                                    // 参数文本位置：命令文本起始位置 + 两个中文字符宽度
                                    paramTextX = cmdStartX + cmdIndent + twoChineseCharSize.cx;
                                }
                            }
                            
                            // 计算"      ※参数名："的完整显示宽度
                            std::wstring paramName = line.substr(1, colonPos - 1);  // 跳过标记字符到冒号之前
                            std::wstring displayPrefix = L"      ※" + paramName + L"：";
                            SIZE prefixDisplaySize;
                            GetTextExtentPoint32W(hdc, displayPrefix.c_str(), (int)displayPrefix.length(), &prefixDisplaySize);
                            
                            // 计算光标位置（在参数值部分）
                            if (doc->cursorCol <= minCursorCol) {
                                caretX = paramTextX + prefixDisplaySize.cx;
                            } else {
                                std::wstring beforeCursor = line.substr(minCursorCol, doc->cursorCol - minCursorCol);
                                SIZE cursorSize;
                                GetTextExtentPoint32W(hdc, beforeCursor.c_str(), (int)beforeCursor.length(), &cursorSize);
                                caretX = paramTextX + prefixDisplaySize.cx + cursorSize.cx;
                            }
                        } else if (IsFlowControlLine(line)) {
                            // 流程控制语句，需要根据嵌套深度计算偏移
                            size_t markerLen = 0;
                            int nestingDepth = GetNestingDepth(line, &markerLen);
                            int depthOffset = nestingDepth * 20;  // 每层嵌套偏移20像素
                            
                            // 计算光标位置时跳过标记字符
                            int cursorCol = doc->cursorCol;
                            if (cursorCol < (int)markerLen) {
                                cursorCol = (int)markerLen;
                            }
                            
                            // 获取标记后的文本内容（去掉点号）
                            std::wstring textAfterMarker = line.substr(markerLen);
                            size_t dotPos = textAfterMarker.find(L'.');
                            if (dotPos != std::wstring::npos) {
                                textAfterMarker.erase(dotPos, 1);
                            }
                            
                            // 计算光标在文本中的位置
                            int posInText = cursorCol - (int)markerLen;
                            // 如果原始行中有点号，需要调整位置
                            if (cursorCol > (int)markerLen) {
                                std::wstring originalAfterMarker = line.substr(markerLen);
                                dotPos = originalAfterMarker.find(L'.');
                                if (dotPos != std::wstring::npos && (cursorCol - (int)markerLen) > (int)dotPos) {
                                    posInText--;  // 点号被删除，位置减1
                                }
                            }
                            posInText = (std::max)(0, posInText);
                            posInText = (std::min)(posInText, (int)textAfterMarker.length());
                            
                            std::wstring beforeCursor = textAfterMarker.substr(0, posInText);
                            SIZE cursorSize;
                            GetTextExtentPoint32W(hdc, beforeCursor.c_str(), (int)beforeCursor.length(), &cursorSize);
                            caretX = startX + 20 + depthOffset + cursorSize.cx;
                        } else {
                            // 普通代码行，需要检查是否是流程控制内的缩进行
                            size_t markerLen = 0;
                            int nestingDepth = GetNestingDepth(line, &markerLen);
                            
                            if (nestingDepth > 0) {
                                // 流程控制内的缩进行，每层嵌套增加20像素
                                int baseIndent = nestingDepth * 20;
                                int actualTextStart = (int)markerLen;
                                
                                // 检查是否是嵌套的流程控制命令（跳过标记后以 . 开头）
                                bool isNestedFlowControl = false;
                                if (line.length() > (size_t)actualTextStart && line[actualTextStart] == L'.') {
                                    std::wstring tempLine = line.substr(actualTextStart);
                                    isNestedFlowControl = IsFlowControlLine(tempLine);
                                    if (isNestedFlowControl) {
                                        // 嵌套流程控制命令需要额外缩进20
                                        baseIndent += 20;
                                        actualTextStart++;  // 跳过点号
                                    }
                                }
                                
                                if (doc->cursorCol <= actualTextStart) {
                                    // 光标在标记字符位置或之前
                                    caretX = startX + baseIndent;
                                } else {
                                    // 光标在文本内容中
                                    std::wstring beforeCursor = line.substr(actualTextStart, doc->cursorCol - actualTextStart);
                                    SIZE cursorSize;
                                    GetTextExtentPoint32W(hdc, beforeCursor.c_str(), (int)beforeCursor.length(), &cursorSize);
                                    caretX = startX + baseIndent + cursorSize.cx;
                                }
                            } else {
                                // 普通代码行，计算缩进（制表符等）
                                int indent = 0;
                                size_t indentEndPos = 0;
                                while (indentEndPos < line.length() && line[indentEndPos] == L'\t') {
                                    indent += 20;
                                    indentEndPos++;
                                }
                                
                                // 计算光标在实际文本部分的位置（排除缩进）
                                int cursorInText = (std::max)(0, doc->cursorCol - (int)indentEndPos);
                                int textEndPos = (std::min)((int)indentEndPos + cursorInText, (int)line.length());
                                SIZE cursorSize;
                                if (textEndPos > (int)indentEndPos) {
                                    std::wstring beforeCursor = line.substr(indentEndPos, textEndPos - indentEndPos);
                                    GetTextExtentPoint32W(hdc, beforeCursor.c_str(), (int)beforeCursor.length(), &cursorSize);
                                    caretX = startX + indent + cursorSize.cx;
                                } else {
                                    caretX = startX + indent;
                                }
                            }
                        }
                    }
                    
                    SetCaretPos(caretX, caretY);
                    
                    // 设置IME输入法候选窗口位置
                    HIMC hIMC = ImmGetContext(hWnd);
                    if (hIMC) {
                        COMPOSITIONFORM cf;
                        cf.dwStyle = CFS_POINT;
                        cf.ptCurrentPos.x = caretX;
                        cf.ptCurrentPos.y = caretY;
                        ImmSetCompositionWindow(hIMC, &cf);
                        ImmReleaseContext(hWnd, hIMC);
                    }
                }
                
                // 更新程序集区域标记和表格类型标记（用于在下一行添加间距）
                if (line.find(L"\t") != std::wstring::npos) {
                    std::vector<std::wstring> checkCells;
                    size_t start = 0, pos = 0;
                    while ((pos = line.find(L'\t', start)) != std::wstring::npos) {
                        checkCells.push_back(line.substr(start, pos - start));
                        start = pos + 1;
                    }
                    checkCells.push_back(line.substr(start));
                    
                    if (checkCells.size() > 0) {
                        if (checkCells[0] == L"程序集名") {
                            lastWasAssemblySection = true;
                            lastTableType = 1; // 程序集表
                        } else if (checkCells[0] == L"变量名" && insideAssembly) {
                            lastWasAssemblySection = true;
                            lastTableType = 1; // 程序集变量表也算程序集区域
                        } else if (checkCells[0] == L"子程序名") {
                            lastWasAssemblySection = false;
                            lastTableType = 2; // 子程序表
                        } else if (checkCells[0] == L"参数名") {
                            lastTableType = 3; // 参数表
                        } else if (checkCells[0] == L"变量名" && !insideAssembly) {
                            lastTableType = 4; // 局部变量表
                        } else if (inParamTable) {
                            // 参数表数据行，保持 lastTableType = 3
                            lastTableType = 3;
                        }
                    }
                    // 程序集变量数据行也属于程序集区域
                    if (inClassVarTable) {
                        lastWasAssemblySection = true;
                    }
                } else {
                    // 非表格行，如果在程序集变量区域内，继续标记
                    if (!inClassVarTable && !insideAssembly) {
                        lastWasAssemblySection = false;
                    }
                }
                
                currentY += rowHeight;
            }
            
            SelectObject(hdc, hOldFont);
            DeleteObject(hBoldFont);
            DeleteObject(hFont);
            
            // 绘制自定义滚动条
            // 垂直滚动条背景
            if (data->vScrollThumbRect.bottom > data->vScrollThumbRect.top) {
                HBRUSH hScrollBgBrush = CreateSolidBrush(g_CurrentTheme.scrollTrack);
                FillRect(hdc, &data->vScrollbarRect, hScrollBgBrush);
                DeleteObject(hScrollBgBrush);
                
                // 垂直滚动条滑块
                COLORREF thumbColor = data->vScrollHover ? g_CurrentTheme.scrollThumbHover : g_CurrentTheme.scrollThumb;
                HBRUSH hThumbBrush = CreateSolidBrush(thumbColor);
                FillRect(hdc, &data->vScrollThumbRect, hThumbBrush);
                DeleteObject(hThumbBrush);
            }
            
            // 水平滚动条背景
            if (data->hScrollThumbRect.right > data->hScrollThumbRect.left) {
                HBRUSH hScrollBgBrush = CreateSolidBrush(g_CurrentTheme.scrollTrack);
                FillRect(hdc, &data->hScrollbarRect, hScrollBgBrush);
                DeleteObject(hScrollBgBrush);
                
                // 水平滚动条滑块
                COLORREF thumbColor = data->hScrollHover ? g_CurrentTheme.scrollThumbHover : g_CurrentTheme.scrollThumb;
                HBRUSH hThumbBrush = CreateSolidBrush(thumbColor);
                FillRect(hdc, &data->hScrollThumbRect, hThumbBrush);
                DeleteObject(hThumbBrush);
            }
            
            // 右下角填充
            RECT cornerRect = {rect.right - data->scrollbarWidth, rect.bottom - data->scrollbarWidth, rect.right, rect.bottom};
            HBRUSH hCornerBrush = CreateSolidBrush(g_CurrentTheme.editorBg);
            FillRect(hdc, &cornerRect, hCornerBrush);
            DeleteObject(hCornerBrush);
            
            // 绘制1像素边框
            HPEN hWindowBorderPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.grid);
            HPEN hOldWindowBorderPen = (HPEN)SelectObject(hdc, hWindowBorderPen);
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Rectangle(hdc, 0, 0, rect.right, rect.bottom);
            SelectObject(hdc, hOldBrush);
            SelectObject(hdc, hOldWindowBorderPen);
            DeleteObject(hWindowBorderPen);
            
            // 绘制自动完成弹窗
            if (data->showCompletion && !data->completionItems.empty()) {
                // 计算弹窗位置
                int completionX = caretX;
                int completionY = caretY + rowHeight;
                
                // 计算弹窗大小
                int itemHeight = 24;
                int iconWidth = 20;  // 图标区域宽度
                int popupWidth = 300;
                int maxVisibleItems = 8;
                int visibleItems = std::min((int)data->completionItems.size(), maxVisibleItems);
                int popupHeight = visibleItems * itemHeight;
                bool needScrollbar = data->completionItems.size() > maxVisibleItems;
                int scrollbarWidth = needScrollbar ? 12 : 0;
                
                // 确保弹窗不超出窗口边界
                if (completionX + popupWidth > rect.right - data->scrollbarWidth) {
                    completionX = rect.right - data->scrollbarWidth - popupWidth;
                }
                if (completionY + popupHeight > rect.bottom - data->scrollbarWidth) {
                    completionY = caretY - popupHeight; // 显示在光标上方
                }
                
                // 保存弹窗区域供点击检测使用
                data->completionRect = {completionX, completionY, completionX + popupWidth, completionY + popupHeight};
                data->completionItemHeight = itemHeight;
                data->completionMaxVisible = maxVisibleItems;  // 使用固定值8，而不是实际可见项数
                
                // 确保滚动偏移有效
                if (data->completionScrollOffset < 0) data->completionScrollOffset = 0;
                int maxScroll = std::max(0, (int)data->completionItems.size() - maxVisibleItems);
                if (data->completionScrollOffset > maxScroll) data->completionScrollOffset = maxScroll;
                
                // 确保选中项可见
                if (data->selectedCompletionIndex < data->completionScrollOffset) {
                    data->completionScrollOffset = data->selectedCompletionIndex;
                }
                if (data->selectedCompletionIndex >= data->completionScrollOffset + maxVisibleItems) {
                    data->completionScrollOffset = data->selectedCompletionIndex - maxVisibleItems + 1;
                }
                
                // 绘制弹窗背景
                RECT popupRect = data->completionRect;
                HBRUSH hPopupBrush = CreateSolidBrush(g_CurrentTheme.popupBg);
                FillRect(hdc, &popupRect, hPopupBrush);
                DeleteObject(hPopupBrush);
                
                // 绘制边框
                HPEN hBorderPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.popupBorder);
                HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
                HBRUSH hOldBrush2 = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
                Rectangle(hdc, completionX, completionY, completionX + popupWidth, completionY + popupHeight);
                SelectObject(hdc, hOldBrush2);
                SelectObject(hdc, hOldPen);
                DeleteObject(hBorderPen);
                
                // 绘制每个选项
                int itemY = completionY;
                int endIndex = std::min(data->completionScrollOffset + visibleItems, (int)data->completionItems.size());
                for (int i = data->completionScrollOffset; i < endIndex; i++) {
                    bool isSelected = (i == data->selectedCompletionIndex);
                    
                    // 绘制选中项背景
                    if (isSelected) {
                        RECT selRect = {completionX + 1, itemY, completionX + popupWidth - scrollbarWidth - 1, itemY + itemHeight};
                        HBRUSH hSelBrush = CreateSolidBrush(g_CurrentTheme.popupSelection);
                        FillRect(hdc, &selRect, hSelBrush);
                        DeleteObject(hSelBrush);
                    }
                    
                    // 绘制图标
                    RECT iconRect = {completionX + 2, itemY + 2, completionX + iconWidth, itemY + itemHeight - 2};
                    COLORREF iconColor;
                    wchar_t iconChar = L'○';
                    
                    // 根据命令类型选择图标和颜色
                    if (data->completionItems[i].isLibraryCommand) {
                        iconChar = L'◆';  // 库命令
                        iconColor = g_CurrentTheme.syntaxKeyword;  // 蓝色
                    } else {
                        switch (data->completionItems[i].type) {
                            case KW_CONTROL:
                                iconChar = L'►';  // 流程控制
                                iconColor = g_CurrentTheme.syntaxFunction;  // 紫色
                                break;
                            case KW_TYPE:
                                iconChar = L'▣';  // 数据类型
                                iconColor = g_CurrentTheme.syntaxType;  // 青色
                                break;
                            case KW_OPERATOR:
                                iconChar = L'＋';  // 运算符
                                iconColor = g_CurrentTheme.syntaxNumber;  // 黄色
                                break;
                            default:
                                iconChar = L'○';  // 其他
                                iconColor = g_CurrentTheme.syntaxVariable;  // 浅蓝
                                break;
                        }
                    }
                    
                    SetBkMode(hdc, TRANSPARENT);
                    SetTextColor(hdc, iconColor);
                    DrawTextW(hdc, &iconChar, 1, &iconRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                    
                    // 绘制关键词文本
                    SetTextColor(hdc, g_CurrentTheme.popupText);
                    RECT textRect = {completionX + iconWidth + 5, itemY, completionX + popupWidth - scrollbarWidth - 5, itemY + itemHeight};
                    std::wstring displayText = data->completionItems[i].displayText;
                    // 只显示主命令名称，不显示description中的"-返回"等描述信息
                    // if (!data->completionItems[i].description.empty() && displayText.length() < 30) {
                    //     displayText += L"  - " + data->completionItems[i].description.substr(0, 40);
                    // }
                    DrawTextW(hdc, displayText.c_str(), -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
                    
                    itemY += itemHeight;
                }
                
                // 绘制滚动条
                if (needScrollbar) {
                    int scrollX = completionX + popupWidth - scrollbarWidth;
                    RECT scrollTrack = {scrollX, completionY, scrollX + scrollbarWidth, completionY + popupHeight};
                    HBRUSH hTrackBrush = CreateSolidBrush(g_CurrentTheme.scrollTrack);
                    FillRect(hdc, &scrollTrack, hTrackBrush);
                    DeleteObject(hTrackBrush);
                    
                    // 计算滚动块大小和位置
                    float thumbRatio = (float)maxVisibleItems / data->completionItems.size();
                    int thumbHeight = std::max(20, (int)(popupHeight * thumbRatio));
                    float scrollRatio = maxScroll > 0 ? (float)data->completionScrollOffset / maxScroll : 0.0f;
                    int thumbY = completionY + (int)((popupHeight - thumbHeight) * scrollRatio);
                    
                    RECT scrollThumb = {scrollX + 2, thumbY, scrollX + scrollbarWidth - 2, thumbY + thumbHeight};
                    HBRUSH hThumbBrush = CreateSolidBrush(g_CurrentTheme.scrollThumb);
                    FillRect(hdc, &scrollThumb, hThumbBrush);
                    DeleteObject(hThumbBrush);
                }
            }
            
            // 绘制数据类型补全窗口
            if (data->showTypeCompletion && !data->typeCompletionItems.empty()) {
                // 计算弹窗位置（在当前光标下方）
                int typeCompletionX = caretX;
                int typeCompletionY = caretY + rowHeight;
                
                // 计算弹窗大小
                int typeItemHeight = TYPE_COMPLETION_ITEM_HEIGHT;
                int typePopupWidth = 180;
                int typeVisibleItems = std::min((int)data->typeCompletionItems.size(), TYPE_COMPLETION_MAX_VISIBLE);
                int typePopupHeight = typeVisibleItems * typeItemHeight + 4;
                bool typeNeedScrollbar = (int)data->typeCompletionItems.size() > TYPE_COMPLETION_MAX_VISIBLE;
                int typeScrollbarWidth = typeNeedScrollbar ? 12 : 0;
                
                // 确保弹窗不超出窗口边界
                if (typeCompletionX + typePopupWidth > rect.right - data->scrollbarWidth) {
                    typeCompletionX = rect.right - data->scrollbarWidth - typePopupWidth;
                }
                if (typeCompletionY + typePopupHeight > rect.bottom - data->scrollbarWidth) {
                    typeCompletionY = caretY - typePopupHeight; // 显示在光标上方
                }
                
                // 保存弹窗区域
                data->typeCompletionRect = {typeCompletionX, typeCompletionY, 
                                            typeCompletionX + typePopupWidth, 
                                            typeCompletionY + typePopupHeight};
                
                // 确保滚动偏移有效
                if (data->typeCompletionScrollOffset < 0) data->typeCompletionScrollOffset = 0;
                int typeMaxScroll = std::max(0, (int)data->typeCompletionItems.size() - TYPE_COMPLETION_MAX_VISIBLE);
                if (data->typeCompletionScrollOffset > typeMaxScroll) data->typeCompletionScrollOffset = typeMaxScroll;
                
                // 确保选中项可见
                if (data->typeCompletionSelectedIndex < data->typeCompletionScrollOffset) {
                    data->typeCompletionScrollOffset = data->typeCompletionSelectedIndex;
                }
                if (data->typeCompletionSelectedIndex >= data->typeCompletionScrollOffset + TYPE_COMPLETION_MAX_VISIBLE) {
                    data->typeCompletionScrollOffset = data->typeCompletionSelectedIndex - TYPE_COMPLETION_MAX_VISIBLE + 1;
                }
                
                // 绘制弹窗背景
                RECT typePopupRect = data->typeCompletionRect;
                HBRUSH hTypePopupBrush = CreateSolidBrush(g_CurrentTheme.popupBg);
                FillRect(hdc, &typePopupRect, hTypePopupBrush);
                DeleteObject(hTypePopupBrush);
                
                // 绘制边框
                HPEN hTypeBorderPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.popupBorder);
                HPEN hOldTypePen = (HPEN)SelectObject(hdc, hTypeBorderPen);
                HBRUSH hOldTypeBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
                Rectangle(hdc, typeCompletionX, typeCompletionY, 
                         typeCompletionX + typePopupWidth, typeCompletionY + typePopupHeight);
                SelectObject(hdc, hOldTypeBrush);
                SelectObject(hdc, hOldTypePen);
                DeleteObject(hTypeBorderPen);
                
                // 绘制每个类型选项
                int typeItemY = typeCompletionY + 2;
                int typeEndIndex = std::min(data->typeCompletionScrollOffset + typeVisibleItems, 
                                           (int)data->typeCompletionItems.size());
                for (int i = data->typeCompletionScrollOffset; i < typeEndIndex; i++) {
                    bool isTypeSelected = (i == data->typeCompletionSelectedIndex);
                    
                    // 绘制选中项背景
                    if (isTypeSelected) {
                        RECT typeSelRect = {typeCompletionX + 1, typeItemY, 
                                           typeCompletionX + typePopupWidth - typeScrollbarWidth - 1, 
                                           typeItemY + typeItemHeight};
                        HBRUSH hTypeSelBrush = CreateSolidBrush(g_CurrentTheme.popupSelection);
                        FillRect(hdc, &typeSelRect, hTypeSelBrush);
                        DeleteObject(hTypeSelBrush);
                    }
                    
                    // 绘制类型图标
                    SetBkMode(hdc, TRANSPARENT);
                    SetTextColor(hdc, g_CurrentTheme.syntaxType);  // 青色
                    RECT typeIconRect = {typeCompletionX + 5, typeItemY, 
                                        typeCompletionX + 22, typeItemY + typeItemHeight};
                    DrawTextW(hdc, L"▣", 1, &typeIconRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                    
                    // 绘制类型名称
                    SetTextColor(hdc, g_CurrentTheme.popupText);
                    RECT typeTextRect = {typeCompletionX + 25, typeItemY, 
                                        typeCompletionX + typePopupWidth - typeScrollbarWidth - 5, 
                                        typeItemY + typeItemHeight};
                    DrawTextW(hdc, data->typeCompletionItems[i].c_str(), -1, &typeTextRect, 
                             DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                    
                    typeItemY += typeItemHeight;
                }
                
                // 绘制滚动条
                if (typeNeedScrollbar) {
                    int typeScrollX = typeCompletionX + typePopupWidth - typeScrollbarWidth;
                    RECT typeScrollTrack = {typeScrollX, typeCompletionY, 
                                           typeScrollX + typeScrollbarWidth, 
                                           typeCompletionY + typePopupHeight};
                    HBRUSH hTypeTrackBrush = CreateSolidBrush(g_CurrentTheme.scrollTrack);
                    FillRect(hdc, &typeScrollTrack, hTypeTrackBrush);
                    DeleteObject(hTypeTrackBrush);
                    
                    // 计算滚动块大小和位置
                    float typeThumbRatio = (float)TYPE_COMPLETION_MAX_VISIBLE / data->typeCompletionItems.size();
                    int typeThumbHeight = std::max(20, (int)(typePopupHeight * typeThumbRatio));
                    float typeScrollRatio = typeMaxScroll > 0 ? 
                                           (float)data->typeCompletionScrollOffset / typeMaxScroll : 0.0f;
                    int typeThumbY = typeCompletionY + (int)((typePopupHeight - typeThumbHeight) * typeScrollRatio);
                    
                    RECT typeScrollThumb = {typeScrollX + 2, typeThumbY, 
                                           typeScrollX + typeScrollbarWidth - 2, 
                                           typeThumbY + typeThumbHeight};
                    HBRUSH hTypeThumbBrush = CreateSolidBrush(g_CurrentTheme.scrollThumb);
                    FillRect(hdc, &typeScrollThumb, hTypeThumbBrush);
                    DeleteObject(hTypeThumbBrush);
                }
            }
            
            } // 结束else块（正常编辑器绘制）
            
            // 将缓冲区内容复制到窗口
            BitBlt(hdcWindow, 0, 0, rect.right, rect.bottom, hdc, 0, 0, SRCCOPY);
            
            // 清理双缓冲资源
            SelectObject(hdc, hOldFont);
            SelectObject(hdc, hOldBitmap);
            DeleteObject(hFont);
            DeleteObject(hBoldFont);
            DeleteObject(hBitmap);
            DeleteDC(hdc);
            
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_SETCURSOR:
        {
            EditorData* data = (EditorData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (!data) return DefWindowProc(hWnd, message, wParam, lParam);
            
            // 获取鼠标位置
            POINT pt;
            GetCursorPos(&pt);
            ScreenToClient(hWnd, &pt);
            
            // 如果正在显示欢迎页，处理鼠标光标
            if (data->showWelcomePage) {
                RECT rect;
                GetClientRect(hWnd, &rect);
                int centerY = rect.bottom / 2 - 80;
                int linkY = centerY + 120;
                
                // 检测是否在三个文字区域（只根据Y坐标判断）
                if ((pt.y >= linkY && pt.y < linkY + 25) ||
                    (pt.y >= linkY + 30 && pt.y < linkY + 55) ||
                    (pt.y >= linkY + 60 && pt.y < linkY + 85)) {
                    SetCursor(LoadCursor(NULL, IDC_HAND));
                } else {
                    SetCursor(LoadCursor(NULL, IDC_ARROW));
                }
                return TRUE;
            }
            
            // 检查是否在标签栏区域或顶部区域
            if (pt.y < data->tabHeight) {
                // 在标签栏区域，使用箭头光标
                SetCursor(LoadCursor(NULL, IDC_ARROW));
                return TRUE;
            }
            
            // 检查是否在滚动条区域
            if (PtInRect(&data->vScrollbarRect, pt) || PtInRect(&data->hScrollbarRect, pt)) {
                // 在滚动条区域，使用箭头光标
                SetCursor(LoadCursor(NULL, IDC_ARROW));
                return TRUE;
            }
            
            // 检查是否在行号区域（X坐标 < 75）
            if (pt.x < 75 && pt.y >= data->tabHeight) {
                // 在行号区域，使用右箭头光标（反选箭头）
                if (data->hRightArrowCursor) {
                    SetCursor(data->hRightArrowCursor);
                } else {
                    SetCursor(LoadCursor(NULL, IDC_ARROW));
                }
                return TRUE;
            }
            
            // 在编辑区域，使用I型光标
            SetCursor(LoadCursor(NULL, IDC_IBEAM));
            return TRUE;
        }
        break;
    case WM_DESTROY:
        {
            EditorData* data = (EditorData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (data) {
                delete data;
                g_EditorData = nullptr;
            }
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}