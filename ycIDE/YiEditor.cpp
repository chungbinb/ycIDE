#include "framework.h"
#include "YiEditor.h"
#include "ycIDE.h"
#include "Theme.h"
#include "ResourceExplorer.h"
#include "Resource.h"
#include "Utils.h"
#include "Keyword.h"
#include "LibraryParser.h"
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <set>
#include <fstream>
#include <gdiplus.h>
#include <imm.h>
#include <windowsx.h>  // GET_X_LPARAM, GET_Y_LPARAM
#pragma comment(lib, "imm32.lib")

using namespace Gdiplus;

extern WCHAR szYiEditorClass[];
extern bool g_LeftPanelVisible;  // 左侧AI面板是否可见

// 打勾图片
static HBITMAP g_hCheckIcon = nullptr;

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
    InvalidateRect(hWnd, NULL, TRUE);
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
    
    // 向上查找命令行（跳过其他参数行）
    int cmdLineIndex = paramLineIndex - 1;
    int paramIndex = 0;  // 当前参数行是第几个参数（从0开始）
    
    while (cmdLineIndex >= 0) {
        const std::wstring& checkLine = doc->lines[cmdLineIndex];
        if (checkLine.length() > 0 && checkLine[0] == L'\u2060') {
            // 这是另一个参数行，继续向上查找
            paramIndex++;
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
    
    // 收集所有参数行的值
    std::vector<std::wstring> paramValues;
    for (int i = cmdLineIndex + 1; i < (int)doc->lines.size(); i++) {
        const std::wstring& line = doc->lines[i];
        if (line.length() > 0 && line[0] == L'\u2060') {
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

// 检查并格式化行上的关键词（离开行时调用）
static void CheckAndFormatKeywords(EditorDocument* doc, int lineIndex) {
    if (lineIndex < 0 || lineIndex >= (int)doc->lines.size()) return;
    
    std::wstring& line = doc->lines[lineIndex];
    if (line.empty()) return;
    
    // 获取行首的单词（去掉前导空格）
    size_t start = 0;
    while (start < line.length() && (line[start] == L' ' || line[start] == L'\t')) {
        start++;
    }
    
    // 查找第一个单词
    size_t end = start;
    while (end < line.length() && line[end] != L' ' && line[end] != L'\t' && line[end] != L'(' && line[end] != L')') {
        end++;
    }
    
    if (end > start) {
        std::wstring word = line.substr(start, end - start);
        
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
            bool hasDot = (start > 0 && line[start - 1] == L'.');
            bool hasBracket = (end < line.length() && line[end] == L'(');
            
            // 如果需要流程线但没有点号，自动在前面添加点号
            if (needsFlowLine && !hasDot) {
                line.insert(start, L".");
                end++;  // 调整end位置，因为插入了一个字符
            }
            
            // 如果需要括号但没有括号，自动添加
            if (!hasBracket && needsBrackets) {
                line.insert(end, L"()");
                hasBracket = true;  // 更新hasBracket状态
            }
            
            // 如果需要流程线，在后面添加相应的行
            if (needsFlowLine) {
                // 检查是否是.如果()命令（带括号，有否则分支）
                bool hasElseBranch = (word == L"如果" && needsBrackets && hasBracket);
                
                if (hasElseBranch) {
                    // .如果()：创建四行结构（使用空格缩进标记）
                    // 1. 单空格（条件达成分支）
                    // 2. 双空格（否则分支）
                    // 3. 普通空行（流程控制外保护行）
                    bool needsStructure = true;
                    if (lineIndex + 1 < (int)doc->lines.size()) {
                        // 检查是否已经有结构（第一行以单空格开头，第二行以双空格开头）
                        const std::wstring& line1 = doc->lines[lineIndex + 1];
                        const std::wstring& line2 = lineIndex + 2 < (int)doc->lines.size() ? doc->lines[lineIndex + 2] : L"";
                        
                        // 检查：第一行以单空格开头（且不是双空格），第二行以双空格开头
                        bool hasLine1 = (line1.length() > 0 && line1[0] == L' ' && (line1.length() == 1 || line1[1] != L' '));
                        bool hasLine2 = (line2.length() >= 2 && line2[0] == L' ' && line2[1] == L' ');
                        
                        if (hasLine1 && hasLine2) {
                            needsStructure = false;
                        }
                    }
                    if (needsStructure) {
                        // 插入三行（使用特殊字符作为行类型标记）
                        doc->lines.insert(doc->lines.begin() + lineIndex + 1, L"\u200C");   // 零宽非连接符：条件达成分支
                        doc->lines.insert(doc->lines.begin() + lineIndex + 2, L"\u200D");   // 零宽连接符：否则分支
                        doc->lines.insert(doc->lines.begin() + lineIndex + 3, L"");    // 普通空行：流程控制外保护行
                    }
                } else if (word == L"如果真") {
                    // 如果真命令：创建两行（不创建结束行，复制时自动添加）
                    // 1. 缩进的空行（流程控制内）
                    // 2. 普通空行（流程控制外）
                    bool needsStructure = true;
                    if (lineIndex + 1 < (int)doc->lines.size()) {
                        // 检查是否已经有结构（使用特殊字符标记）
                        if ((doc->lines[lineIndex + 1].length() > 0 && doc->lines[lineIndex + 1][0] == L'\u200C') && 
                            lineIndex + 2 < (int)doc->lines.size() &&
                            doc->lines[lineIndex + 2].empty()) {
                            needsStructure = false;
                        }
                    }
                    if (needsStructure) {
                        doc->lines.insert(doc->lines.begin() + lineIndex + 1, L"\u200C");  // 零宽非连接符：流程控制内的条件达成分支
                        doc->lines.insert(doc->lines.begin() + lineIndex + 2, L"");  // 流程控制外
                    }
                } else {
                    // 其他流程控制命令：创建两行
                    // 1. 缩进的空行（流程控制内）
                    // 2. 普通空行（流程控制外）
                    bool needsStructure = true;
                    if (lineIndex + 1 < (int)doc->lines.size()) {
                        // 检查是否已经有结构（使用特殊字符标记）
                        if ((doc->lines[lineIndex + 1].length() > 0 && doc->lines[lineIndex + 1][0] == L'\u200C') && 
                            lineIndex + 2 < (int)doc->lines.size() &&
                            doc->lines[lineIndex + 2].empty()) {
                            needsStructure = false;
                        }
                    }
                    if (needsStructure) {
                        doc->lines.insert(doc->lines.begin() + lineIndex + 1, L"\u200C");  // 零宽非连接符：流程控制内的条件达成分支
                        doc->lines.insert(doc->lines.begin() + lineIndex + 2, L"");  // 流程控制外
                    }
                }
            }
        }
    }
}

EditorData* g_EditorData = nullptr;

EditorDocument::EditorDocument() : cursorLine(0), cursorCol(0), scrollY(0), scrollX(0), modified(false),
                                   hasSelection(false), selStartLine(0), selStartCol(0), selEndLine(0), selEndCol(0), isSelecting(false), hFileLock(INVALID_HANDLE_VALUE),
                                   showDiff(false), hasUserInteraction(false), fileType(FILE_TYPE_EYC) {
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
    hRightArrowCursor(NULL), showWelcomePage(true) {
    // 启动时显示欢迎页，不创建默认文档
    
    // 初始化滚动条矩形
    vScrollbarRect = {0, 0, 0, 0};
    vScrollThumbRect = {0, 0, 0, 0};
    hScrollbarRect = {0, 0, 0, 0};
    hScrollThumbRect = {0, 0, 0, 0};
    
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
        int len = (int)line.length();
        int s = (startC > len) ? len : startC;
        int e = (endC > len) ? len : endC;
        if (s < e) {
            result = line.substr(s, e - s);
        }
    } else {
        // 第一行
        const std::wstring& startLine = doc->lines[startL];
        if (startC < (int)startLine.length()) {
            result += startLine.substr(startC);
        }
        result += L"\r\n";

        // 中间行
        for (int i = startL + 1; i < endL; i++) {
            result += doc->lines[i];
            result += L"\r\n";
        }

        // 最后一行
        const std::wstring& endLine = doc->lines[endL];
        if (endC > 0) {
            result += endLine.substr(0, (std::min)(endC, (int)endLine.length()));
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
    
    // 清理孤立的表头
    CleanupOrphanedHeaders(doc);
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
                        }
                    }

                } else {
                    // 非EPL格式，直接插入
                    int insertStartLine = doc->cursorLine;
                    InsertText(doc, text);
                    
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

    // 计算水平滚动条
    int maxWidth = 0;
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
            std::wofstream debugFile(L"d:\\chungbin\\ycide\\yieditor_init_debug.txt");
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
            if (doc && mouseX >= 58 && mouseX <= 72) {  // 展开符号的X范围（行号右侧，更大的点击区域）
                int adjustedY = mouseY - data->tabHeight + doc->scrollY;
                int rowHeight = data->rowHeight;
                int startY = 5;
                
                bool hasVersion = (!doc->lines.empty() && doc->lines[0].find(L"版本\t") == 0);
                size_t startLine = hasVersion ? 1 : 0;
                
                int currentY = startY;
                for (size_t i = startLine; i < doc->lines.size(); i++) {
                    const std::wstring& line = doc->lines[i];
                    
                    if (adjustedY >= currentY && adjustedY < currentY + rowHeight) {
                        // 检查这一行是否是有参数的命令（流程控制或普通命令）
                        bool isCommandLine = IsFlowControlLine(line);
                        if (!isCommandLine && line.find(L'(') != std::wstring::npos) {
                            // 可能是普通命令，检查是否有参数
                            isCommandLine = true;
                        }
                        
                        if (isCommandLine) {
                            // 提取命令名（去掉前导点号、空格和括号）
                            std::wstring cmdName;
                            size_t start = 0;
                            // 跳过前导空格
                            while (start < line.length() && line[start] == L' ') start++;
                            // 跳过点号（如果有）
                            if (start < line.length() && line[start] == L'.') start++;
                            // 再次跳过空格
                            while (start < line.length() && line[start] == L' ') start++;
                            
                            size_t end = line.find(L'(', start);
                            if (end == std::wstring::npos) end = line.length();
                            cmdName = line.substr(start, end - start);
                            
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
                                    for (size_t p = 0; p < params.size(); p++) {
                                        // 参数行格式：\u2060 + 参数名 + : （冒号后面是参数值）
                                        std::wstring paramLine = L"\u2060" + params[p].name + L":";
                                        doc->lines.insert(doc->lines.begin() + i + p + 1, paramLine);
                                        doc->parametersExpanded.insert(doc->parametersExpanded.begin() + i + p + 1, false);
                                    }
                                } else {
                                    // 折叠：删除参数行
                                    for (size_t p = 0; p < params.size(); p++) {
                                        if (i + 1 < doc->lines.size() && doc->lines[i + 1].length() > 0 && doc->lines[i + 1][0] == L'\u2060') {
                                            doc->lines.erase(doc->lines.begin() + i + 1);
                                            doc->parametersExpanded.erase(doc->parametersExpanded.begin() + i + 1);
                                        }
                                    }
                                }
                                
                                doc->modified = true;
                                InvalidateRect(hWnd, NULL, TRUE);
                                return 0;
                            }
                        }
                        break;
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
                        // 参数行：格式为 \u2060参数名:参数值
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
                                size_t dotPos = cmdText.find(L'.');
                                if (dotPos != std::wstring::npos) {
                                    cmdText.erase(dotPos, 1);
                                    if (leftBracket > 0) leftBracket--;
                                }
                                SIZE textSize;
                                GetTextExtentPoint32W(hdc, cmdText.c_str(), (int)leftBracket, &textSize);
                                int bracketX = startX + 20 + textSize.cx;
                                
                                // 计算※符号位置
                                SIZE prefixSize;
                                GetTextExtentPoint32W(hdc, L"      ", 6, &prefixSize);
                                int minArrowX = bracketX + 20;
                                int defaultArrowX = startX + 20 + prefixSize.cx;
                                int arrowX = (std::max)(minArrowX, defaultArrowX);
                                
                                paramTextX = arrowX - prefixSize.cx;
                            }
                        }
                        
                        // 绘制时整个displayText从paramTextX开始，所以光标计算也要从这里开始
                        // 但光标逻辑上的位置应该是在 line 中的位置（colonPos + 1）
                        // 需要计算 "      ※参数名：" 的显示宽度来确定参数值的起始X坐标
                        std::wstring paramName = line.substr(1, colonPos - 1);  // 跳过标记字符，到冒号之前
                        std::wstring displayPrefix = L"      ※" + paramName + L"：";
                        SIZE prefixDisplaySize;
                        GetTextExtentPoint32W(hdc, displayPrefix.c_str(), (int)displayPrefix.length(), &prefixDisplaySize);
                        
                        // 参数值的实际绘制起始位置
                        textStartX = paramTextX + prefixDisplaySize.cx;
                        
                        // 光标只能在冒号之后（colonPos + 1），不能编辑参数名部分
                        indentChars = colonPos + 1;  // 跳过 "\u2060参数名:"，光标从冒号后开始
                    } else {
                        // 计算缩进
                        int indent = 0;
                        while (indentChars < line.length() && line[indentChars] == L' ') {
                            indent += 4;
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
                CreateCaret(hWnd, NULL, 2, caretHeight);
                ShowCaret(hWnd);
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
                        line.insert(doc->cursorCol, 1, ch);
                        doc->cursorCol++;
                        doc->modified = true;
                        
                        // 如果是参数行，同步更新命令行括号内的参数值
                        if (line.length() > 0 && line[0] == L'\u2060') {
                            SyncParamLineToCommandLine(doc, doc->cursorLine);
                        }
                        
                        if (ch != L' ') {
                            int wordStart;
                            std::wstring currentWord = GetCurrentWord(line, doc->cursorCol, wordStart);
                            if (!currentWord.empty() && currentWord.length() >= 1) {  // 改为1个字符就触发补全
                                std::vector<CompletionItem> completions = KeywordManager::GetInstance().GetCompletions(currentWord);
                                if (!completions.empty()) {
                                    data->showCompletion = true;
                                    data->completionItems = completions;
                                    data->selectedCompletionIndex = 0;
                                    data->completionScrollOffset = 0;  // 重置滚动偏移
                                    data->currentWord = currentWord;
                                    data->completionMaxVisible = 8;  // 设置最大可见项数
                                    data->completionItemHeight = 24;  // 设置项目高度
                                } else {
                                    data->showCompletion = false;
                                }
                            } else {
                                data->showCompletion = false;
                            }
                        }
                        
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                }
            } else if (ch == VK_RETURN) {  // 回车
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
                            std::wstring cmdName;
                            const std::wstring& cmdLine = doc->lines[cmdLineIndex];
                            size_t start = 0;
                            while (start < cmdLine.length() && cmdLine[start] == L' ') start++;
                            if (start < cmdLine.length() && cmdLine[start] == L'.') start++;
                            while (start < cmdLine.length() && cmdLine[start] == L' ') start++;
                            size_t end = cmdLine.find(L'(', start);
                            if (end == std::wstring::npos) end = cmdLine.length();
                            cmdName = cmdLine.substr(start, end - start);
                            
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
                                InvalidateRect(hWnd, NULL, TRUE);
                                return 0;
                            }
                        }
                        // 如果没有更多参数，继续到下面的普通处理
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
                                doc->lines.insert(doc->lines.begin() + lastParamLine + 1, L"");
                                while (doc->parametersExpanded.size() <= lastParamLine + 1) {
                                    doc->parametersExpanded.push_back(false);
                                }
                                doc->parametersExpanded.insert(doc->parametersExpanded.begin() + lastParamLine + 1, false);
                                doc->cursorLine = lastParamLine + 1;
                                doc->cursorCol = 0;
                                doc->modified = true;
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
                                InvalidateRect(hWnd, NULL, TRUE);
                                handled = true;
                            }
                        } else if (headerType == L"参数名") {
                            // 在参数表中（表头或数据行）：插入新参数行
                            doc->lines.insert(doc->lines.begin() + doc->cursorLine + 1, L"\t整数型\t\t\t\t");
                            doc->cursorLine++;
                            doc->cursorCol = 0;
                            doc->modified = true;
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
                    
                    // 检查当前行是否是流程控制命令
                    if (IsFlowControlLine(line)) {
                        // 在流程控制命令行按回车，跳转到下一行（应该是缩进空行）
                        if (doc->cursorLine + 1 < (int)doc->lines.size()) {
                            doc->cursorLine++;
                            // 移动到标记字符后的位置（跳过特殊标记字符）
                            std::wstring& nextLine = doc->lines[doc->cursorLine];
                            int indentLen = 0;
                            if (nextLine.length() > 0 && (nextLine[0] == L'\u200C' || nextLine[0] == L'\u200D')) {
                                indentLen = 1;  // 跳过标记字符
                            }
                            doc->cursorCol = indentLen;
                        }
                    } else {
                        // 普通行，正常处理
                        std::wstring newLine = line.substr(doc->cursorCol);
                        line = line.substr(0, doc->cursorCol);
                        
                        int newCursorCol = 0;
                        // 检查当前行是否在流程控制块内（使用特殊字符标记）
                        bool isInTrueBranch = (line.length() >= 1 && line[0] == L'\u200C');
                        bool isInElseBranch = (line.length() >= 1 && line[0] == L'\u200D');
                        
                        if (isInElseBranch) {
                            // 当前行在否则分支内，新行也保持否则分支标记
                            newLine = L"\u200D" + newLine;
                            newCursorCol = 1;
                        } else if (isInTrueBranch) {
                            // 当前行在条件达成分支内，新行也保持该标记
                            newLine = L"\u200C" + newLine;
                            newCursorCol = 1;
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
                    // 检查是否清空了数据行，需要清理表头
                    CleanupOrphanedHeaders(doc);
                    
                    // 如果是参数行，同步更新命令行括号内的参数值
                    if (line.length() > 0 && line[0] == L'\u2060') {
                        SyncParamLineToCommandLine(doc, doc->cursorLine);
                    }
                    
                    // 更新自动补全提示
                    if (data->showCompletion) {
                        int wordStart;
                        std::wstring currentWord = GetCurrentWord(line, doc->cursorCol, wordStart);
                        if (!currentWord.empty() && currentWord.length() >= 1) {
                            std::vector<CompletionItem> completions = KeywordManager::GetInstance().GetCompletions(currentWord);
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

                    // 合并到上一行
                    std::wstring currentLine = doc->lines[doc->cursorLine];
                    doc->lines.erase(doc->lines.begin() + doc->cursorLine);
                    doc->cursorLine--;
                    doc->cursorCol = (int)doc->lines[doc->cursorLine].length();
                    doc->lines[doc->cursorLine] += currentLine;
                    doc->modified = true;
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
                        // 表格行：插入制表符
                        line.insert(doc->cursorCol, L"\t");
                        doc->cursorCol++;
                    } else if (isIndented) {
                        // 流程控制内：插入单个空格（保持缩进标记）
                        line.insert(doc->cursorCol, L" ");
                        doc->cursorCol++;
                    } else {
                        // 普通代码行：插入4个空格
                        line.insert(doc->cursorCol, L"    ");
                        doc->cursorCol += 4;
                    }
                    
                    doc->modified = true;
                    InvalidateRect(hWnd, NULL, TRUE);
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
                    return 0;
                case VK_SPACE:
                    ApplyCompletion(hWnd, data, doc);
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
                    
                    if (doc->cursorCol < (int)line.length()) {
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
                        // 检查是否清空了数据行，需要清理表头
                        CleanupOrphanedHeaders(doc);
                        
                        // 如果是参数行，同步更新命令行括号内的参数值
                        if (line.length() > 0 && line[0] == L'\u2060') {
                            SyncParamLineToCommandLine(doc, doc->cursorLine);
                        }
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

                        // 合并下一行
                        doc->lines[doc->cursorLine] += doc->lines[doc->cursorLine + 1];
                        doc->lines.erase(doc->lines.begin() + doc->cursorLine + 1);
                        doc->modified = true;
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
                    CreateCaret(hWnd, NULL, 2, caretHeight);
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
        UpdateScrollBars(hWnd);
        break;

    case WM_MOUSEWHEEL:
        {
            EditorData* data = (EditorData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (!data) return 0;
            
            int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            int lines = delta / WHEEL_DELTA;
            
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
                SetTextColor(hdc, RGB(200, 200, 200));
                RECT titleRect = {0, centerY, rect.right, centerY + 50};
                DrawTextW(hdc, L"ycIDE", -1, &titleRect, DT_CENTER | DT_SINGLELINE);
                
                // 绘制副标题
                SelectObject(hdc, hSubFont);
                SetTextColor(hdc, RGB(130, 130, 130));
                RECT subRect = {0, centerY + 50, rect.right, centerY + 80};
                DrawTextW(hdc, L"易承语言集成开发环境", -1, &subRect, DT_CENTER | DT_SINGLELINE);
                
                // 绘制快捷操作提示
                SelectObject(hdc, hLinkFont);
                SetTextColor(hdc, RGB(80, 160, 230));
                
                int linkY = centerY + 120;
                RECT linkRect1 = {0, linkY, rect.right, linkY + 25};
                DrawTextW(hdc, L"新建文件 (Ctrl+N)", -1, &linkRect1, DT_CENTER | DT_SINGLELINE);
                
                RECT linkRect2 = {0, linkY + 30, rect.right, linkY + 55};
                DrawTextW(hdc, L"打开文件 (Ctrl+O)", -1, &linkRect2, DT_CENTER | DT_SINGLELINE);
                
                RECT linkRect3 = {0, linkY + 60, rect.right, linkY + 85};
                DrawTextW(hdc, L"打开文件夹", -1, &linkRect3, DT_CENTER | DT_SINGLELINE);
                
                // 绘制版本信息
                SetTextColor(hdc, RGB(80, 80, 80));
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
            
            // 颜色定义
            COLORREF clrText = g_CurrentTheme.text;
            COLORREF clrKeyword = RGB(170, 79, 110);  // 流程块文本颜色
            COLORREF clrComment = RGB(0, 128, 0);
            COLORREF clrLineNum = RGB(110, 118, 129);  // 行号颜色
            COLORREF clrGridLine = RGB(80, 80, 80);  // 表格框线颜色，调亮
            COLORREF clrSubHeaderBg = RGB(58, 65, 81);  // 子程序表头背景色
            COLORREF clrVarHeaderBg = RGB(56, 56, 48);  // 变量表头背景色
            
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
                    if (!isCommandLine && line.find(L'(') != std::wstring::npos) {
                        // 可能是普通命令，检查是否有参数
                        isCommandLine = true;
                    }
                    
                    if (isCommandLine) {
                        // 提取命令名（去掉前导点号、空格和括号）
                        std::wstring cmdName;
                        size_t start = 0;
                        // 跳过前导空格
                        while (start < line.length() && line[start] == L' ') start++;
                        // 跳过点号（如果有）
                        if (start < line.length() && line[start] == L'.') start++;
                        // 再次跳过空格
                        while (start < line.length() && line[start] == L' ') start++;
                        
                        size_t end = line.find(L'(', start);
                        if (end == std::wstring::npos) end = line.length();
                        cmdName = line.substr(start, end - start);
                        
                        // 获取命令参数
                        std::vector<LibraryParameter> params = GetCommandParameters(cmdName);
                        
                        // 只有当命令有参数时才绘制展开/折叠符号
                        if (!params.empty()) {
                            while (doc->parametersExpanded.size() <= currentLineIndex) {
                                doc->parametersExpanded.push_back(false);
                            }
                            bool isExpanded = doc->parametersExpanded[currentLineIndex];
                            SetTextColor(hdc, RGB(150, 150, 150));
                            SelectObject(hdc, hBoldFont);  // 使用加粗字体使符号更明显
                            const wchar_t* expandSymbol = isExpanded ? L"-" : L"+";
                            TextOutW(hdc, 61, textY - 1, expandSymbol, 1);  // 稍微调整位置使其居中
                            SelectObject(hdc, hFont);  // 恢复普通字体
                            SetTextColor(hdc, clrLineNum);
                        }
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
            
            // Lambda 函数：绘制表格单元格
            auto DrawTableCell = [&](int x, int y, int w, int h, const wchar_t* text, COLORREF color, bool isHeader = false, COLORREF headerBgColor = RGB(192, 192, 192), bool centerAlign = false, bool isCheckbox = false, COLORREF diffBgColor = RGB(0, 0, 0), bool hasDiffBg = false) {
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
            
            // Lambda 函数：绘制实心向下箭头
            auto DrawDownArrow = [&](int x, int y) {
                HBRUSH hArrowBrush = CreateSolidBrush(RGB(128, 128, 128));
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

            // Lambda 函数：绘制实心向右箭头
            auto DrawRightArrow = [&](int x, int y) {
                HBRUSH hArrowBrush = CreateSolidBrush(RGB(128, 128, 128));
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
            
            // 绘制每一行（遍历data->lines）
            std::vector<int> currentTableColWidths;  // 当前表格的列宽
            
            // 第一遍遍历：计算每列的最大宽度
            std::vector<std::vector<int>> tableMaxWidths;  // 存储每个表格每列的最大宽度
            std::vector<int> tableTypes; // 0: Generic, 1: Assembly
            int currentTableIndex = -1;
            bool insideAssembly = false; // 标记是否在程序集块内
            bool inAssemblyVarSection = false; // 标记是否在程序集变量区域内
            
            // 检查是否有版本行
            bool hasVersion = (!doc->lines.empty() && doc->lines[0].find(L"版本\t") == 0);
            size_t startLine = hasVersion ? 1 : 0;
            
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
            
            // 第二遍遍历：实际绘制
            currentTableIndex = -1;
            bool inParamTable = false;  // 标记当前是否在参数表区域内
            bool inClassVarTable = false; // 标记当前是否在程序集变量表区域内
            insideAssembly = false; // 标记是否在程序集块内
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
                        
                        if (IsFlowControlLine(line)) {
                            // 流程控制语句，文本从 startX + 20 开始
                            textStartX = startX + 20;
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
                            
                            // 计算缩进字符数量
                            size_t indentChars = 0;
                            while (indentChars < line.length() && (line[indentChars] == L' ' || line[indentChars] == L'\t')) {
                                indentChars++;
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
                            HBRUSH selBrush = CreateSolidBrush(RGB(38, 79, 120));  // 选中背景色
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
                    COLORREF headerBg = RGB(220, 220, 220);
                    if (cells.size() > 0) {
                        if (cells[0] == L"子程序名") {
                            headerBg = clrSubHeaderBg;
                        } else if (cells[0] == L"程序集名") {
                            headerBg = RGB(58, 65, 81);
                        } else if (cells[0] == L"变量名") {
                            headerBg = clrVarHeaderBg;
                        } else if (cells[0] == L"参数名") {
                            headerBg = RGB(56, 56, 48);  // 参数表头背景色
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
                    COLORREF diffBgColor = RGB(0, 0, 0);
                    if (doc->showDiff && i < doc->diffTypes.size()) {
                        DiffLineType diffType = doc->diffTypes[i];
                        if (diffType == DIFF_ADDED || diffType == DIFF_DELETED) {
                            hasDiffBg = true;
                            diffBgColor = (diffType == DIFF_ADDED) ? RGB(76, 94, 36) : RGB(76, 25, 25);
                        }
                    }
                    
                    // 如果在参数表区域内且有超过5列，使用分割绘制
                    if (inParamTable && cells.size() >= 6 && colWidths.size() >= 4) {
                        // 参数表：使用子程序表的4列宽度，将第3列拆分为可空+数组+备注
                        COLORREF checkColor = isHeader ? clrText : RGB(139, 0, 0);  // 深红色
                        COLORREF typeColor = isHeader ? clrText : RGB(78, 201, 176);   // 类型列颜色
                        COLORREF remarkColor = isHeader ? clrText : RGB(0, 128, 0); // 备注列绿色
                        
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
                        COLORREF checkColor = RGB(139, 0, 0);  // 深红色
                        COLORREF typeColor = isHeader ? clrText : RGB(78, 201, 176);   // 类型列颜色
                        COLORREF remarkColor = isHeader ? clrText : RGB(0, 128, 0); // 备注列绿色
                        
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
                    
                    SetTextColor(hdc, clrKeyword);
                    // 隐藏点号，只显示命令本身
                    std::wstring displayText = line;
                    size_t dotPos = displayText.find(L'.');
                    if (dotPos != std::wstring::npos) {
                        displayText.erase(dotPos, 1);  // 删除点号
                    }
                    TextOutW(hdc, startX + 20, currentY + (rowHeight - fontSize) / 2, displayText.c_str(), (int)displayText.length());
                    
                    // 检查是否是"如果()"命令（带括号，需要绘制否则分支）
                    std::wstring trimmedLine = line;
                    size_t trimStart = trimmedLine.find_first_not_of(L" \t");
                    if (trimStart != std::wstring::npos) {
                        trimmedLine = trimmedLine.substr(trimStart);
                    }
                    // 检查是否包含"如果("或"如果 ("
                    bool hasElseBranch = (trimmedLine.find(L".如果(") == 0 || trimmedLine.find(L".如果 (") == 0);
                    
                    // 绘制横向虚线
                    int lineX = startX + 10;
                    int textCenterOffset = (rowHeight - fontSize) / 2 + fontSize / 2;  // 文本垂直中心位置
                    HPEN hDashPen = CreatePen(PS_DOT, 1, RGB(128, 128, 128));
                    HPEN hOldDashPen = (HPEN)SelectObject(hdc, hDashPen);
                    MoveToEx(hdc, lineX, currentY + textCenterOffset, NULL);
                    LineTo(hdc, startX + 18, currentY + textCenterOffset);
                    
                    if (hasElseBranch) {
                        // .如果() 绘制if-else结构（新的流程控制线绘制）
                        // 查找双空格行（否则分支的位置）和最后一个缩进行
                        int elseLineIdx = -1;  // 否则分支的行号（第一个双空格行）
                        int lastIndentedLine = -1; // 否则分支的最后一个缩进行
                        
                        // 从.如果()下一行开始查找
                        for (size_t j = i + 1; j < doc->lines.size(); j++) {
                            const std::wstring& checkLine = doc->lines[j];
                            
                            // 跳过参数行（使用\u2060标记）
                            if (checkLine.length() > 0 && checkLine[0] == L'\u2060') {
                                continue;  // 参数行，跳过
                            }
                            
                            if (elseLineIdx == -1) {
                                // 还没找到否则分支，查找否则分支标记（\u200D）
                                if (checkLine.length() >= 1 && checkLine[0] == L'\u200D') {
                                    elseLineIdx = (int)j;
                                    lastIndentedLine = (int)j;  // 初始化为否则分支起始行
                                }
                            } else {
                                // 已找到否则分支，继续查找所有带标记的双空格行
                                // 空行：视为分支内的一部分，但不更新 lastIndentedLine
                                if (checkLine.empty()) {
                                    continue;
                                }

                                // 检查是否是否则分支标记的行（否则分支的延续）
                                bool isElseBranch = (checkLine.length() >= 1 && checkLine[0] == L'\u200D');

                                if (isElseBranch) {
                                    lastIndentedLine = (int)j;  // 更新最后一个否则分支行
                                } else {
                                    // 遇到非标记行，结束
                                    break;
                                }
                            }
                        }
                        
                        if (elseLineIdx > 0 && lastIndentedLine >= elseLineIdx) {

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
                            LineTo(hdc, startX + 18, elseY);
                            // 向右箭头
                            DrawRightArrow(startX + 18, elseY);
                            
                            // 绘制第二条虚线：从条件达成分支到否则分支下方的向下箭头
                            // 先向左绘制横线连接到竖线
                            MoveToEx(hdc, rightX, trueBranchY, NULL);
                            LineTo(hdc, rightX - 13, trueBranchY);
                            // 再向下绘制到否则分支行的底部位置（否则分支结束位置）
                            // 使用 lastIndentedLine 计算底部位置
                            int elseBranchBottomY = currentY + (lastIndentedLine - (int)i) * rowHeight + rowHeight - 5;
                            MoveToEx(hdc, rightX - 13, trueBranchY, NULL);
                            LineTo(hdc, rightX - 13, elseBranchBottomY);
                            // 向下箭头（在否则分支行的底部）
                            DrawDownArrow(rightX - 13, elseBranchBottomY);
                        } else {
                            // 没找到完整的if-else结构，绘制单条向下的线
                            int endY = currentY + rowHeight;
                            
                            for (size_t j = i + 1; j < doc->lines.size(); j++) {
                                const std::wstring& checkLine = doc->lines[j];
                                
                                // 遇到参数行，更新endY到该行之后，然后继续查找缩进行
                                if (checkLine.length() > 0 && checkLine[0] == L'\u2060') {
                                    endY = currentY + (int)(j - i + 2) * rowHeight;  // 延伸到参数行之后
                                    continue;
                                }
                                
                                // 检查是否是条件达成分支或否则分支的行
                                bool isIndented = (checkLine.length() >= 1 && (checkLine[0] == L'\u200C' || checkLine[0] == L'\u200D'));
                                if (isIndented) {
                                    endY = currentY + (int)(j - i + 1) * rowHeight;
                                } else if (checkLine.empty()) {
                                    continue;
                                } else {
                                    break;
                                }
                            }
                            
                            MoveToEx(hdc, lineX, currentY + textCenterOffset, NULL);
                            LineTo(hdc, lineX, endY);
                            DrawDownArrow(lineX, endY);
                        }
                    } else {
                        // .如果真 或其他流程控制，绘制单条线
                        // 查找流程控制内最后一个有内容的行（有缩进的行）
                        int endY = currentY + rowHeight;  // 默认至少延伸一行
                        
                        for (size_t j = i + 1; j < doc->lines.size(); j++) {
                            const std::wstring& checkLine = doc->lines[j];
                            
                            // 遇到参数行，更新endY到该行之后，然后继续查找缩进行
                            if (checkLine.length() > 0 && checkLine[0] == L'\u2060') {
                                endY = currentY + (int)(j - i + 2) * rowHeight;  // 延伸到参数行之后
                                continue;
                            }
                            
                            // 检查是否是条件达成分支或否则分支的行
                            bool isIndented = (checkLine.length() >= 1 && (checkLine[0] == L'\u200C' || checkLine[0] == L'\u200D'));
                            if (isIndented) {
                                // 更新结束位置为这一行的底部
                                endY = currentY + (int)(j - i + 1) * rowHeight;
                            } else if (checkLine.empty()) {
                                // 空行，继续查找
                                continue;
                            } else {
                                // 遇到非缩进行，结束查找
                                break;
                            }
                        }
                        
                        MoveToEx(hdc, lineX, currentY + textCenterOffset, NULL);
                        LineTo(hdc, lineX, endY);
                        
                        // 绘制向下箭头
                        DrawDownArrow(lineX, endY);
                    }
                    
                    SelectObject(hdc, hOldDashPen);
                    DeleteObject(hDashPen);
                } else if (line.length() > 0 && line[0] == L'\u2060') {
                    // 参数行（命令展开的参数输入行）
                    
                    int paramTextX = startX + 20;  // 默认位置
                    
                    // 绘制从上一行命令括号到参数行的虚线
                    if (i > 0) {
                        // 找到上一行命令行（可能需要向上查找多行，跳过其他参数行）
                        int cmdLineIndex = (int)i - 1;
                        while (cmdLineIndex >= 0 && doc->lines[cmdLineIndex].length() > 0 && doc->lines[cmdLineIndex][0] == L'\u2060') {
                            cmdLineIndex--;
                        }
                        
                        if (cmdLineIndex >= 0) {
                            const std::wstring& prevLine = doc->lines[cmdLineIndex];
                            // 找到括号位置
                            size_t leftBracket = prevLine.find(L'(');
                            if (leftBracket != std::wstring::npos) {
                                int textCenterOffset = (rowHeight - fontSize) / 2 + fontSize / 2;
                                int prevLineY = currentY - (int)(i - cmdLineIndex) * rowHeight;
                                int prevLineBottom = prevLineY + rowHeight;  // 上一行的底部
                                int currY = currentY + textCenterOffset;  // 当前行的中心
                                
                                // 计算括号的X坐标
                                std::wstring cmdText = prevLine;
                                
                                // 检查是否是流程控制内的行（使用特殊字符标记）
                                bool isIndentedCmd = (cmdText.length() >= 1 && (cmdText[0] == L'\u200C' || cmdText[0] == L'\u200D'));
                                int cmdStartX = isIndentedCmd ? (startX + 20) : startX;
                                
                                // 如果是流程控制内的行，去掉标记字符
                                if (isIndentedCmd) {
                                    cmdText = cmdText.substr(1);
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
                                    if (leftBracket > 0) leftBracket--;  // 调整括号位置
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
                                
                                // 绘制虚线：从参数文本位置向下，然后向右到参数内容位置
                                HPEN hDashPen = CreatePen(PS_DOT, 1, RGB(128, 128, 128));
                                HPEN hOldPen = (HPEN)SelectObject(hdc, hDashPen);
                                
                                // 垂直线：从上一行底部到当前行中心
                                MoveToEx(hdc, flowLineX, prevLineBottom, NULL);
                                LineTo(hdc, flowLineX, currY);
                                
                                // 水平线：从垂直线到参数文本前
                                MoveToEx(hdc, flowLineX, currY, NULL);
                                LineTo(hdc, arrowX, currY);
                                
                                SelectObject(hdc, hOldPen);
                                DeleteObject(hDashPen);
                                
                                // 在命令底部绘制向上的实心三角形箭头（只在第一个参数行绘制）
                                if ((int)i == cmdLineIndex + 1) {
                                    POINT upArrow[3];
                                    upArrow[0] = {flowLineX, prevLineBottom - 5};      // 顶点（向上）
                                    upArrow[1] = {flowLineX - 5, prevLineBottom};      // 左下角
                                    upArrow[2] = {flowLineX + 5, prevLineBottom};      // 右下角
                                    HBRUSH hArrowBrush = CreateSolidBrush(RGB(128, 128, 128));
                                    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hArrowBrush);
                                    Polygon(hdc, upArrow, 3);
                                    SelectObject(hdc, hOldBrush);
                                    DeleteObject(hArrowBrush);
                                }
                            }
                        }
                    }
                    
                    SetTextColor(hdc, RGB(100, 150, 200));  // 浅蓝色
                    // 提取参数名和参数值（格式：\u2060参数名:参数值）
                    std::wstring lineContent = line.substr(1);  // 跳过标记字符
                    size_t colonPos = lineContent.find(L':');
                    std::wstring paramName, paramValue;
                    if (colonPos != std::wstring::npos) {
                        paramName = lineContent.substr(0, colonPos);
                        paramValue = lineContent.substr(colonPos + 1);
                    } else {
                        paramName = lineContent;
                        paramValue = L"";
                    }
                    std::wstring displayText = L"      ※" + paramName + L"：" + paramValue;
                    TextOutW(hdc, paramTextX, currentY + (rowHeight - fontSize) / 2, displayText.c_str(), (int)displayText.length());
                } else if (line.length() > 0 && line[0] == L'\'') {
                    // 注释行
                    SetTextColor(hdc, clrComment);
                    TextOutW(hdc, startX, currentY + (rowHeight - fontSize) / 2, line.c_str(), (int)line.length());
                } else if (line.find(L"返回") == 0) {
                    // 返回语句
                    SetTextColor(hdc, clrKeyword);
                    TextOutW(hdc, startX, currentY + (rowHeight - fontSize) / 2, line.c_str(), (int)line.length());
                } else {
                    // 普通代码行
                    
                    // 绘制diff背景（如果启用了diff视图）- 只显示背景色，不显示+/-符号
                    if (doc->showDiff && i < doc->diffTypes.size()) {
                        DiffLineType diffType = doc->diffTypes[i];
                        if (diffType == DIFF_ADDED || diffType == DIFF_DELETED) {
                            // 绘制背景色（删除行：深红色，新增行：深绿色），只绘制代码区域（从startX开始）
                            COLORREF bgColor = (diffType == DIFF_ADDED) ? RGB(76, 94, 36) : RGB(76, 25, 25);
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
                    
                    // 检查是否是流程控制内的行（使用特殊字符标记）
                    bool isIndented = (displayLine.length() >= 1 && (displayLine[0] == L'\u200C' || displayLine[0] == L'\u200D'));
                    
                    if (isIndented) {
                        // 流程控制内的行，从 startX + 20 开始绘制（与流程命令对齐）
                        // 跳过标记字符
                        pos = 1;
                        SetTextColor(hdc, clrText);
                        TextOutW(hdc, startX + 20, currentY + (rowHeight - fontSize) / 2, displayLine.c_str() + pos, (int)(displayLine.length() - pos));
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
                        
                        SetTextColor(hdc, clrText);
                        TextOutW(hdc, startX + indent, currentY + (rowHeight - fontSize) / 2, displayLine.c_str() + pos, (int)(displayLine.length() - pos));
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
                                    
                                    // 检查是否是流程控制内的行（使用特殊字符标记）
                                    bool isIndentedCmd = (cmdText.length() >= 1 && (cmdText[0] == L'\u200C' || cmdText[0] == L'\u200D'));
                                    int cmdStartX = isIndentedCmd ? (startX + 20) : startX;
                                    
                                    // 如果是流程控制内的行，去掉标记字符
                                    if (isIndentedCmd) {
                                        cmdText = cmdText.substr(1);
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
                            // 流程控制语句，文本从 startX + 20 开始
                            SIZE cursorSize;
                            std::wstring beforeCursor = line.substr(0, (std::min)(doc->cursorCol, (int)line.length()));
                            GetTextExtentPoint32W(hdc, beforeCursor.c_str(), (int)beforeCursor.length(), &cursorSize);
                            caretX = startX + 20 + cursorSize.cx;
                        } else {
                            // 普通代码行，需要检查是否是流程控制内的缩进行
                            bool isIndented = (line.length() >= 1 && (line[0] == L'\u200C' || line[0] == L'\u200D'));
                            
                            if (isIndented) {
                                // 流程控制内的缩进行，固定从 startX + 20 开始
                                // 光标位置：跳过标记字符后计算
                                int actualTextStart = 1;  // 跳过标记字符
                                
                                if (doc->cursorCol <= actualTextStart) {
                                    // 光标在标记字符位置或之前
                                    caretX = startX + 20;
                                } else {
                                    // 光标在文本内容中
                                    std::wstring beforeCursor = line.substr(actualTextStart, doc->cursorCol - actualTextStart);
                                    SIZE cursorSize;
                                    GetTextExtentPoint32W(hdc, beforeCursor.c_str(), (int)beforeCursor.length(), &cursorSize);
                                    caretX = startX + 20 + cursorSize.cx;
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
                HBRUSH hScrollBgBrush = CreateSolidBrush(g_CurrentTheme.editorBg);
                FillRect(hdc, &data->vScrollbarRect, hScrollBgBrush);
                DeleteObject(hScrollBgBrush);
                
                // 垂直滚动条滑块
                COLORREF thumbColor = data->vScrollHover ? RGB(90, 90, 90) : RGB(70, 70, 70);
                HBRUSH hThumbBrush = CreateSolidBrush(thumbColor);
                FillRect(hdc, &data->vScrollThumbRect, hThumbBrush);
                DeleteObject(hThumbBrush);
            }
            
            // 水平滚动条背景
            if (data->hScrollThumbRect.right > data->hScrollThumbRect.left) {
                HBRUSH hScrollBgBrush = CreateSolidBrush(g_CurrentTheme.editorBg);
                FillRect(hdc, &data->hScrollbarRect, hScrollBgBrush);
                DeleteObject(hScrollBgBrush);
                
                // 水平滚动条滑块
                COLORREF thumbColor = data->hScrollHover ? RGB(90, 90, 90) : RGB(70, 70, 70);
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
                HBRUSH hPopupBrush = CreateSolidBrush(RGB(40, 40, 40));
                FillRect(hdc, &popupRect, hPopupBrush);
                DeleteObject(hPopupBrush);
                
                // 绘制边框
                HPEN hBorderPen = CreatePen(PS_SOLID, 1, RGB(80, 80, 80));
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
                        HBRUSH hSelBrush = CreateSolidBrush(RGB(60, 100, 150));
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
                        iconColor = RGB(86, 156, 214);  // 蓝色
                    } else {
                        switch (data->completionItems[i].type) {
                            case KW_CONTROL:
                                iconChar = L'►';  // 流程控制
                                iconColor = RGB(197, 134, 192);  // 紫色
                                break;
                            case KW_TYPE:
                                iconChar = L'▣';  // 数据类型
                                iconColor = RGB(78, 201, 176);  // 青色
                                break;
                            case KW_OPERATOR:
                                iconChar = L'＋';  // 运算符
                                iconColor = RGB(220, 220, 170);  // 黄色
                                break;
                            default:
                                iconChar = L'○';  // 其他
                                iconColor = RGB(156, 220, 254);  // 浅蓝
                                break;
                        }
                    }
                    
                    SetBkMode(hdc, TRANSPARENT);
                    SetTextColor(hdc, iconColor);
                    DrawTextW(hdc, &iconChar, 1, &iconRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                    
                    // 绘制关键词文本
                    SetTextColor(hdc, RGB(220, 220, 220));
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
                    HBRUSH hTrackBrush = CreateSolidBrush(RGB(60, 60, 60));
                    FillRect(hdc, &scrollTrack, hTrackBrush);
                    DeleteObject(hTrackBrush);
                    
                    // 计算滚动块大小和位置
                    float thumbRatio = (float)maxVisibleItems / data->completionItems.size();
                    int thumbHeight = std::max(20, (int)(popupHeight * thumbRatio));
                    float scrollRatio = maxScroll > 0 ? (float)data->completionScrollOffset / maxScroll : 0.0f;
                    int thumbY = completionY + (int)((popupHeight - thumbHeight) * scrollRatio);
                    
                    RECT scrollThumb = {scrollX + 2, thumbY, scrollX + scrollbarWidth - 2, thumbY + thumbHeight};
                    HBRUSH hThumbBrush = CreateSolidBrush(RGB(120, 120, 120));
                    FillRect(hdc, &scrollThumb, hThumbBrush);
                    DeleteObject(hThumbBrush);
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