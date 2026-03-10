#include "Utils.h"
#include "YiEditor.h"
#include "ResourceExplorer.h"
#include "Keyword.h"
#include "LibraryParser.h"
#include <fstream>
#include <commdlg.h>
#include <vector>
#include <sstream>
#include <functional>

void DebugLog(const std::wstring& msg) {
    CreateDirectoryA("logs", NULL);
    std::ofstream logFile("logs\\debug_log.txt", std::ios::app);
    if (logFile.is_open()) {
        int len = WideCharToMultiByte(CP_UTF8, 0, msg.c_str(), -1, NULL, 0, NULL, NULL);
        if (len > 0) {
            std::string utf8Msg(len - 1, 0);
            WideCharToMultiByte(CP_UTF8, 0, msg.c_str(), -1, &utf8Msg[0], len, NULL, NULL);
            logFile << "[EPL_CONVERT] " << utf8Msg << std::endl;
        }
    }
}

// EditorData 定义在 YiEditor.h 中

// 检查一行是否是需要绘制流程线的流程控制命令
bool IsFlowControlLine(const std::wstring& line) {
    if (line.empty()) return false;
    
    // 去掉前导空格、制表符和嵌套标记字符
    size_t start = 0;
    while (start < line.length() && 
           (line[start] == L' ' || line[start] == L'\t' ||
            line[start] == L'\u200C' || line[start] == L'\u200D' || line[start] == L'\u200B')) {
        start++;
    }
    
    // 检查是否以点号开头
    if (start >= line.length() || line[start] != L'.') {
        return false;
    }
    
    // 提取点号后面的关键词
    size_t kwStart = start + 1;
    size_t kwEnd = kwStart;
    while (kwEnd < line.length() && line[kwEnd] != L' ' && line[kwEnd] != L'\t' && 
           line[kwEnd] != L'(' && line[kwEnd] != L')') {
        kwEnd++;
    }
    
    if (kwEnd > kwStart) {
        std::wstring keyword = line.substr(kwStart, kwEnd - kwStart);
        
        // 先检查内置关键词
        if (KeywordManager::GetInstance().NeedsFlowLine(keyword)) {
            return true;
        }
        
        // 再检查支持库命令
        const LibraryCommand* libCmd = LibraryParser::GetInstance().FindCommand(keyword);
        if (libCmd && libCmd->needsFlowLine) {
            return true;
        }
    }
    
    return false;
}

// 辅助函数：分割字符串
std::vector<std::wstring> SplitString(const std::wstring& str, const std::wstring& delimiter) {
    std::vector<std::wstring> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);
    while (end != std::wstring::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }
    tokens.push_back(str.substr(start));
    return tokens;
}

// EPL代码转内部格式
std::wstring ConvertEPLToInternal(const std::wstring& eplCode) {
    DebugLog(L"Start ConvertEPLToInternal");
    std::wstringstream ss(eplCode);
    std::wstring line;
    std::wstring result;
    int lastType = 0; // 0:None, 1:Var, 2:Param, 3:ClassVar
    
    // 第一遍：处理流程控制缩进（将4个空格转换为单空格或双空格）
    std::vector<std::wstring> allLines;
    
    while (std::getline(ss, line, L'\n')) {
        // 去除回车符
        if (!line.empty() && line.back() == L'\r') line.pop_back();
        allLines.push_back(line);
        DebugLog(L"Read line " + std::to_wstring(allLines.size()-1) + L": [" + line + L"]");
    }
    
    // 处理流程控制命令
    for (size_t i = 0; i < allLines.size(); i++) {
        std::wstring& line = allLines[i];
        
        // 获取去除前导空白的内容
        std::wstring trimmed = line;
        size_t first = trimmed.find_first_not_of(L" \t");
        if (first != std::wstring::npos) {
            trimmed = trimmed.substr(first);
        }
        
        // 检查是否是.如果()或.如果真命令
        bool isIfCommand = (trimmed.find(L".如果 (") == 0 || trimmed.find(L".如果(") == 0 ||
                           trimmed.find(L"如果 (") == 0 || trimmed.find(L"如果(") == 0);
        bool isIfTrueCommand = (trimmed.find(L".如果真") == 0 || trimmed.find(L"如果真") == 0);
        
        // 检查是否是循环命令（判断循环首/循环判断首）
        bool isLoopHeadCommand = (trimmed.find(L".判断循环首") == 0 || trimmed.find(L"判断循环首") == 0 ||
                                  trimmed.find(L".循环判断首") == 0 || trimmed.find(L"循环判断首") == 0 ||
                                  trimmed.find(L".计次循环首") == 0 || trimmed.find(L"计次循环首") == 0 ||
                                  trimmed.find(L".变量循环首") == 0 || trimmed.find(L"变量循环首") == 0);
        
        if (isLoopHeadCommand) {
            // 查找对应的循环尾命令
            std::wstring tailKeyword1, tailKeyword2;
            if (trimmed.find(L"判断循环首") != std::wstring::npos) {
                tailKeyword1 = L".判断循环尾";
                tailKeyword2 = L"判断循环尾";
            } else if (trimmed.find(L"循环判断首") != std::wstring::npos) {
                tailKeyword1 = L".循环判断尾";
                tailKeyword2 = L"循环判断尾";
            } else if (trimmed.find(L"计次循环首") != std::wstring::npos) {
                tailKeyword1 = L".计次循环尾";
                tailKeyword2 = L"计次循环尾";
            } else {
                tailKeyword1 = L".变量循环尾";
                tailKeyword2 = L"变量循环尾";
            }
            
            int tailLineIdx = -1;
            int nestLevel = 1;
            
            for (size_t j = i + 1; j < allLines.size(); j++) {
                std::wstring checkTrimmed = allLines[j];
                size_t checkFirst = checkTrimmed.find_first_not_of(L" \t");
                if (checkFirst != std::wstring::npos) {
                    checkTrimmed = checkTrimmed.substr(checkFirst);
                }
                
                // 检查嵌套的同类循环首
                if (trimmed.find(L"判断循环首") != std::wstring::npos) {
                    if (checkTrimmed.find(L".判断循环首") == 0 || checkTrimmed.find(L"判断循环首") == 0) {
                        nestLevel++;
                    }
                } else if (trimmed.find(L"循环判断首") != std::wstring::npos) {
                    if (checkTrimmed.find(L".循环判断首") == 0 || checkTrimmed.find(L"循环判断首") == 0) {
                        nestLevel++;
                    }
                } else if (trimmed.find(L"计次循环首") != std::wstring::npos) {
                    if (checkTrimmed.find(L".计次循环首") == 0 || checkTrimmed.find(L"计次循环首") == 0) {
                        nestLevel++;
                    }
                } else if (trimmed.find(L"变量循环首") != std::wstring::npos) {
                    if (checkTrimmed.find(L".变量循环首") == 0 || checkTrimmed.find(L"变量循环首") == 0) {
                        nestLevel++;
                    }
                }
                
                if (checkTrimmed.find(tailKeyword1) == 0 || checkTrimmed.find(tailKeyword2) == 0) {
                    nestLevel--;
                    if (nestLevel == 0) {
                        tailLineIdx = (int)j;
                        break;
                    }
                }
            }
            
            if (tailLineIdx == -1) {
                tailLineIdx = (int)allLines.size();
            }
            
            // 转换循环体（循环首和循环尾之间的代码）为 \u200C 标记
            for (int j = (int)i + 1; j < tailLineIdx; j++) {
                std::wstring& targetLine = allLines[j];
                if (targetLine.empty()) continue;
                
                size_t targetFirst = targetLine.find_first_not_of(L" \t");
                if (targetFirst != std::wstring::npos) {
                    std::wstring targetTrimmed = targetLine.substr(targetFirst);
                    allLines[j] = L"\u200C" + targetTrimmed;
                } else {
                    allLines[j] = L"\u200C";
                }
            }
            
            // 循环尾行保留原样（不需要缩进标记），确保有点号前缀
            if (tailLineIdx < (int)allLines.size()) {
                std::wstring& tailLine = allLines[tailLineIdx];
                size_t tailFirst = tailLine.find_first_not_of(L" \t");
                if (tailFirst != std::wstring::npos) {
                    tailLine = tailLine.substr(tailFirst);
                }
                // 确保以.开头
                if (!tailLine.empty() && tailLine[0] != L'.') {
                    tailLine = L"." + tailLine;
                }
            }
            
            // 在循环尾后面添加空行作为结束标记
            if (tailLineIdx < (int)allLines.size()) {
                // 检查循环尾后面是否已有空行
                if (tailLineIdx + 1 >= (int)allLines.size() || !allLines[tailLineIdx + 1].empty()) {
                    allLines.insert(allLines.begin() + tailLineIdx + 1, L"");
                }
            }
        } else if (isIfCommand || isIfTrueCommand) {
            DebugLog(L"Found If/IfTrue at line " + std::to_wstring(i));
            // 找到.否则的位置（如果有）
            int elseLineIdx = -1;
            int endIfLineIdx = -1;
            
            for (size_t j = i + 1; j < allLines.size(); j++) {
                std::wstring checkTrimmed = allLines[j];
                size_t checkFirst = checkTrimmed.find_first_not_of(L" \t");
                if (checkFirst != std::wstring::npos) {
                    checkTrimmed = checkTrimmed.substr(checkFirst);
                }
                
                if (elseLineIdx == -1 && (checkTrimmed.find(L".否则") == 0 || checkTrimmed.find(L"否则") == 0)) {
                    elseLineIdx = (int)j;
                } else if (checkTrimmed.find(L".如果结束") == 0 || checkTrimmed.find(L"如果结束") == 0 ||
                          checkTrimmed.find(L".如果真结束") == 0 || checkTrimmed.find(L"如果真结束") == 0) {
                    endIfLineIdx = (int)j;
                    break;
                }
            }
            
            // 如果没有找到结束标记，以文件末尾为结束
            if (endIfLineIdx == -1) {
                endIfLineIdx = (int)allLines.size();
            }
            DebugLog(L"Else at " + std::to_wstring(elseLineIdx) + L", EndIf at " + std::to_wstring(endIfLineIdx));

            // 转换条件达成分支（.如果 到 .否则 之间的代码）
            int conditionEnd = (elseLineIdx != -1) ? elseLineIdx : endIfLineIdx;
            DebugLog(L"Converting condition branch " + std::to_wstring(i + 1) + L" to " + std::to_wstring(conditionEnd));
            for (int j = i + 1; j < conditionEnd; j++) {
                std::wstring& targetLine = allLines[j];
                // 跳过空行
                if (targetLine.empty()) continue;
                
                // 去除原有缩进，转换为\u200C开头（条件达成分支）
                size_t targetFirst = targetLine.find_first_not_of(L" \t");
                if (targetFirst != std::wstring::npos) {
                    std::wstring targetTrimmed = targetLine.substr(targetFirst);
                    allLines[j] = L"\u200C" + targetTrimmed;
                } else {
                    // 全是空白的行，转换为\u200C
                    allLines[j] = L"\u200C";
                }
                DebugLog(L"Line " + std::to_wstring(j) + L" converted to: [" + allLines[j] + L"]");
            }
            
            // 如果有.否则，标记删除.否则行并转换否则分支
            if (elseLineIdx != -1) {
                allLines[elseLineIdx] = L"\x01DELETE\x01";  // 使用特殊标记表示删除
                
                // 转换否则分支（.否则 到 .如果结束 之间的代码）
                DebugLog(L"Converting else branch " + std::to_wstring(elseLineIdx + 1) + L" to " + std::to_wstring(endIfLineIdx));
                
                for (int j = elseLineIdx + 1; j < endIfLineIdx; j++) {
                    std::wstring& targetLine = allLines[j];
                    // 跳过空行
                    if (targetLine.empty()) continue;
                    
                    // 去除原有缩进，转换为\u200D开头（否则分支）
                    size_t targetFirst = targetLine.find_first_not_of(L" \t");
                    if (targetFirst != std::wstring::npos) {
                        std::wstring targetTrimmed = targetLine.substr(targetFirst);
                        allLines[j] = L"\u200D" + targetTrimmed;
                    } else {
                        // 全是空白的行，转换为\u200D
                        allLines[j] = L"\u200D";
                    }
                    DebugLog(L"Line " + std::to_wstring(j) + L" converted to: [" + allLines[j] + L"]");
                }
            }
                
            
            // 如果有.如果结束或.如果真结束，删除它并添加保护空行
            if (endIfLineIdx < (int)allLines.size()) {
                std::wstring endTrimmed = allLines[endIfLineIdx];
                size_t endFirst = endTrimmed.find_first_not_of(L" \t");
                if (endFirst != std::wstring::npos) {
                    endTrimmed = endTrimmed.substr(endFirst);
                }
                if (endTrimmed.find(L".如果结束") == 0 || endTrimmed.find(L"如果结束") == 0 ||
                    endTrimmed.find(L".如果真结束") == 0 || endTrimmed.find(L"如果真结束") == 0) {
                    allLines[endIfLineIdx] = L"";  // 空行作为保护行
                } else {
                    // 没有明确的结束命令，插入保护空行
                    allLines.insert(allLines.begin() + endIfLineIdx, L"");
                }
            } else {
                // 文件末尾，添加保护空行
                allLines.push_back(L"");
            }
        }
    }
    
    // 第二遍：处理嵌套流程控制（检测已标记行中的流程控制命令）
    // 使用循环多次处理，支持任意深度的嵌套
    bool foundNested = true;
    int maxIterations = 10;  // 最多支持10层嵌套，防止无限循环
    int iteration = 0;
    
    while (foundNested && iteration < maxIterations) {
        foundNested = false;
        iteration++;
        
        for (size_t i = 0; i < allLines.size(); i++) {
            std::wstring& line = allLines[i];
            
            // 检查是否是已标记的行（\u200C 或 \u200D 开头）
            if (line.length() < 2) continue;
            wchar_t outerMarker = line[0];
            if (outerMarker != L'\u200C' && outerMarker != L'\u200D') continue;
            
            // 计算当前行的嵌套深度（\u200B 的数量）
            int currentDepth = 1;
            size_t markerEnd = 1;
            while (markerEnd < line.length() && line[markerEnd] == L'\u200B') {
                currentDepth++;
                markerEnd++;
            }
            
            // 获取标记后的内容（去除前导空格）
            std::wstring content = line.substr(markerEnd);
            size_t contentFirst = content.find_first_not_of(L" \t");
            if (contentFirst != std::wstring::npos && contentFirst > 0) {
                content = content.substr(contentFirst);
            }
            
            // 检查是否是嵌套的流程控制命令
            bool isNestedIfCommand = (content.find(L".如果 (") == 0 || content.find(L".如果(") == 0 ||
                                      content.find(L"如果 (") == 0 || content.find(L"如果(") == 0);
            bool isNestedIfTrueCommand = (content.find(L".如果真") == 0 || content.find(L"如果真") == 0);
            
            if (isNestedIfCommand || isNestedIfTrueCommand) {
                // 检查下一行是否已经是更深嵌套（已经处理过）
                if (i + 1 < allLines.size()) {
                    const std::wstring& nextLine = allLines[i + 1];
                    if (nextLine.length() >= markerEnd + 1 && 
                        (nextLine[0] == L'\u200C' || nextLine[0] == L'\u200D')) {
                        // 计算下一行的嵌套深度
                        int nextDepth = 1;
                        size_t nextMarkerEnd = 1;
                        while (nextMarkerEnd < nextLine.length() && nextLine[nextMarkerEnd] == L'\u200B') {
                            nextDepth++;
                            nextMarkerEnd++;
                        }
                        if (nextDepth > currentDepth) {
                            // 下一行已经是更深嵌套，跳过
                            continue;
                        }
                    }
                }
                
                DebugLog(L"Found nested If/IfTrue at line " + std::to_wstring(i) + L" depth=" + std::to_wstring(currentDepth));
                foundNested = true;
                
                // 查找嵌套的结束位置
                int nestedEndIdx = -1;
                int nestLevel = 1;
                
                for (size_t j = i + 1; j < allLines.size(); j++) {
                    std::wstring& checkLine = allLines[j];
                    
                    // 跳过删除标记的行
                    if (checkLine.find(L"\x01DELETE\x01") != std::wstring::npos) continue;
                    if (checkLine.empty()) continue;
                    
                    // 检查是否仍在同一外层流程块内
                    if (checkLine[0] != L'\u200C' && checkLine[0] != L'\u200D') {
                        // 离开外层流程块
                        nestedEndIdx = (int)j;
                        break;
                    }
                    
                    // 计算此行的嵌套深度
                    int checkDepth = 1;
                    size_t checkMarkerEnd = 1;
                    while (checkMarkerEnd < checkLine.length() && checkLine[checkMarkerEnd] == L'\u200B') {
                        checkDepth++;
                        checkMarkerEnd++;
                    }
                    
                    // 如果已经是更深嵌套，跳过
                    if (checkDepth > currentDepth) {
                        continue;
                    }
                    
                    // 如果是更浅嵌套，结束
                    if (checkDepth < currentDepth) {
                        nestedEndIdx = (int)j;
                        break;
                    }
                    
                    std::wstring checkContent = checkLine.substr(checkMarkerEnd);
                    // 去除前导空格
                    size_t checkFirst = checkContent.find_first_not_of(L" \t");
                    if (checkFirst != std::wstring::npos && checkFirst > 0) {
                        checkContent = checkContent.substr(checkFirst);
                    }
                    
                    // 检查是否是另一个嵌套流程控制开始
                    bool isAnotherNestedIf = (checkContent.find(L".如果 (") == 0 || checkContent.find(L".如果(") == 0 ||
                                              checkContent.find(L"如果 (") == 0 || checkContent.find(L"如果(") == 0 ||
                                              checkContent.find(L".如果真") == 0 || checkContent.find(L"如果真") == 0);
                    
                    // 检查是否是结束标记
                    bool isNestedEnd = (checkContent.find(L".如果结束") == 0 || checkContent.find(L"如果结束") == 0 ||
                                       checkContent.find(L".如果真结束") == 0 || checkContent.find(L"如果真结束") == 0);
                    
                    if (isAnotherNestedIf) {
                        nestLevel++;
                    } else if (isNestedEnd) {
                        nestLevel--;
                        if (nestLevel == 0) {
                            // 标记删除这个结束行
                            allLines[j] = L"\x01DELETE\x01";
                            nestedEndIdx = (int)j;
                            break;
                        }
                    }
                }
                
                // 如果没有找到结束，使用外层流程块的结束
                if (nestedEndIdx == -1) {
                    for (size_t j = i + 1; j < allLines.size(); j++) {
                        if (allLines[j].length() < 1 || (allLines[j][0] != L'\u200C' && allLines[j][0] != L'\u200D')) {
                            nestedEndIdx = (int)j;
                            break;
                        }
                    }
                    if (nestedEndIdx == -1) nestedEndIdx = (int)allLines.size();
                }
                
                // 将嵌套流程块内的行增加一个 \u200B 标记
                for (int j = i + 1; j < nestedEndIdx; j++) {
                    std::wstring& targetLine = allLines[j];
                    
                    // 跳过删除标记的行
                    if (targetLine.find(L"\x01DELETE\x01") != std::wstring::npos) continue;
                    
                    // 跳过已处理的行和空行
                    if (targetLine.empty()) continue;
                    if (targetLine[0] != L'\u200C' && targetLine[0] != L'\u200D') break;
                    
                    // 计算此行的嵌套深度
                    int targetDepth = 1;
                    size_t targetMarkerEnd = 1;
                    while (targetMarkerEnd < targetLine.length() && targetLine[targetMarkerEnd] == L'\u200B') {
                        targetDepth++;
                        targetMarkerEnd++;
                    }
                    
                    // 只处理同级嵌套的行（不处理已经更深的嵌套）
                    if (targetDepth > currentDepth) continue;
                    
                    // 在标记后插入一个 \u200B
                    wchar_t marker = targetLine[0];
                    std::wstring existingMarkers = targetLine.substr(1, targetMarkerEnd - 1);
                    std::wstring innerContent = targetLine.substr(targetMarkerEnd);
                    allLines[j] = std::wstring(1, marker) + existingMarkers + L"\u200B" + innerContent;
                    DebugLog(L"Line " + std::to_wstring(j) + L" marked as depth " + std::to_wstring(targetDepth + 1));
                }
            }
        }
    }
    
    // 第四遍：过滤删除标记的行
    std::vector<std::wstring> finalLines;
    for (const std::wstring& line : allLines) {
        // 跳过删除标记的行
        if (line.find(L"\x01DELETE\x01") == std::wstring::npos) {
            finalLines.push_back(line);
        }
    }
    DebugLog(L"Pass 3: Processing " + std::to_wstring(finalLines.size()) + L" lines");
    
    // 第五遍：处理表格等其他内容
    for (const std::wstring& line : finalLines) {
        // 检查是否是流程控制代码行（以单空格或双空格开头）
        bool isFlowControlLine = false;
        if (line.length() >= 2 && line[0] == L' ' && line[1] == L' ') {
            isFlowControlLine = true;  // 双空格：否则分支
        } else if (line.length() >= 1 && line[0] == L' ' && (line.length() == 1 || line[1] != L' ')) {
            isFlowControlLine = true;  // 单空格：条件达成分支或如果真分支
        }
        
        // 如果是流程控制代码行，直接输出原始行
        if (isFlowControlLine) {
            DebugLog(L"Keeping flow control line: [" + line + L"]");
            result += line + L"\n";
            continue;
        }
        
        // 去除首尾空白（仅用于非流程控制行）
        size_t first = line.find_first_not_of(L" \t");
        if (first == std::wstring::npos) {
            result += line + L"\n";
            continue;
        }
        std::wstring trimmed = line.substr(first);
        
        // 创建无空格版本用于关键字检查
        std::wstring checkStr;
        for (wchar_t c : trimmed) {
            if (c != L' ' && c != L'\t') checkStr += c;
        }
        
        // 辅助函数：分割字符串（支持逗号或空格）
        auto splitParts = [](const std::wstring& s) -> std::vector<std::wstring> {
            std::vector<std::wstring> parts;
            // 优先尝试逗号分割
            if (s.find(L',') != std::wstring::npos) {
                size_t start = 0, end = 0;
                while ((end = s.find(L',', start)) != std::wstring::npos) {
                    std::wstring part = s.substr(start, end - start);
                    // trim
                    size_t p1 = part.find_first_not_of(L" \t");
                    if (p1 != std::wstring::npos) {
                        size_t p2 = part.find_last_not_of(L" \t");
                        parts.push_back(part.substr(p1, p2 - p1 + 1));
                    } else {
                        parts.push_back(L"");
                    }
                    start = end + 1;
                }
                std::wstring last = s.substr(start);
                size_t p1 = last.find_first_not_of(L" \t");
                if (p1 != std::wstring::npos) {
                    size_t p2 = last.find_last_not_of(L" \t");
                    parts.push_back(last.substr(p1, p2 - p1 + 1));
                } else {
                    parts.push_back(L"");
                }
            } else {
                // 空格分割
                std::wstringstream wss(s);
                std::wstring temp;
                while (wss >> temp) {
                    parts.push_back(temp);
                }
            }
            return parts;
        };

        if (checkStr.find(L".版本") == 0) {
            lastType = 0;
            std::wstring ver = L"2";
            size_t numPos = trimmed.find_first_of(L"0123456789");
            if (numPos != std::wstring::npos) {
                ver = trimmed.substr(numPos);
            }
            result += L"版本\t" + ver + L"\n";
        }
        else if (checkStr.find(L".支持库") == 0) {
            // 隐藏支持库
            continue;
        }
        else if ((checkStr.find(L".程序集") == 0 || checkStr.find(L"程序集") == 0) && 
                 checkStr.find(L"程序集变量") == std::wstring::npos && 
                 checkStr.find(L"程序集名") == std::wstring::npos) {
            lastType = 0;
            std::wstring name = trimmed;
            size_t spacePos = trimmed.find(L" ");
            if (spacePos != std::wstring::npos) {
                name = trimmed.substr(spacePos + 1);
                size_t nStart = name.find_first_not_of(L" ");
                if (nStart != std::wstring::npos) name = name.substr(nStart);
            }
            result += L"程序集名\t保留\t保留\t备注\n";
            result += name + L"\t\t\t\n";
        }
        else if ((checkStr.find(L".程序集变量") == 0 || checkStr.find(L"程序集变量") == 0) &&
                 checkStr.find(L"变量名") == std::wstring::npos) {
            if (lastType != 3) {
                result += L"变量名\t类型\t数组\t备注\n"; // 程序集变量表头 (4列)
                lastType = 3;
            }
            std::wstring content = trimmed;
            size_t spacePos = trimmed.find(L" ");
            if (spacePos != std::wstring::npos) {
                content = trimmed.substr(spacePos + 1);
                size_t nStart = content.find_first_not_of(L" ");
                if (nStart != std::wstring::npos) content = content.substr(nStart);
            }
            
            std::vector<std::wstring> parts = splitParts(content);
            
            std::wstring name = parts.size() > 0 ? parts[0] : L"";
            std::wstring type = parts.size() > 1 ? parts[1] : L"";
            std::wstring array = parts.size() > 2 ? parts[2] : L"";
            std::wstring remark = parts.size() > 3 ? parts[3] : L"";
            
            if (remark.size() >= 2 && remark.front() == L'"' && remark.back() == L'"') {
                remark = remark.substr(1, remark.size() - 2);
            }
            
            // 映射到: Name \t Type \t Array \t Remark (4列)
            result += name + L"\t" + type + L"\t" + array + L"\t" + remark + L"\n";
        }
        else if ((checkStr.find(L".子程序") == 0 || checkStr.find(L"子程序") == 0) &&
                 checkStr.find(L"子程序名") == std::wstring::npos) {
            lastType = 0;
            std::wstring content = trimmed;
            size_t spacePos = trimmed.find(L" ");
            if (spacePos != std::wstring::npos) {
                content = trimmed.substr(spacePos + 1);
                size_t nStart = content.find_first_not_of(L" ");
                if (nStart != std::wstring::npos) content = content.substr(nStart);
            }
            
            std::vector<std::wstring> parts = splitParts(content);
            
            // EPL子程序格式: .子程序 名称, 返回类型, 公开, 备注
            // 第1个是名称，第2个是返回类型，第3个是公开（可为空），第4个是备注
            std::wstring name = parts.size() > 0 ? parts[0] : L"";
            std::wstring retType = parts.size() > 1 ? parts[1] : L"";
            std::wstring pub = L"";
            std::wstring remark = L"";
            
            // 如果子程序名为"子程序"，自动重命名为"子程序1"
            if (name == L"子程序") {
                name = L"子程序1";
            }
            
            // 第3个参数是公开标记
            if (parts.size() > 2 && parts[2] == L"公开") {
                pub = L"公开";
            }
            
            // 第4个参数是备注
            if (parts.size() > 3) {
                remark = parts[3];
                // 去除可能的引号
                if (remark.size() >= 2 && remark.front() == L'"' && remark.back() == L'"') {
                    remark = remark.substr(1, remark.size() - 2);
                }
            }
            
            // 转换公开标记为√
            std::wstring pubMark = (!pub.empty()) ? L"√" : L"";
            
            result += L"子程序名\t返回值类型\t公开\t备注\n";
            result += name + L"\t" + retType + L"\t" + pubMark + L"\t" + remark + L"\n";
        }
        else if ((checkStr.find(L".参数") == 0 || checkStr.find(L"参数") == 0) &&
                 checkStr.find(L"参数名") == std::wstring::npos) {
            if (lastType != 2) {
                result += L"参数名\t类型\t参考\t可空\t数组\t备注\n";
                lastType = 2;
            }
            std::wstring content = trimmed;
            size_t spacePos = trimmed.find(L" ");
            if (spacePos != std::wstring::npos) {
                content = trimmed.substr(spacePos + 1);
                size_t nStart = content.find_first_not_of(L" ");
                if (nStart != std::wstring::npos) content = content.substr(nStart);
            }
            
            std::vector<std::wstring> parts = splitParts(content);
            
            // EPL参数格式: .参数 名称, 类型, [参考], [可空], [数组], [备注]
            // 备注可能带引号也可能不带，参考/可空/数组是特殊关键字
            std::wstring name = parts.size() > 0 ? parts[0] : L"";
            std::wstring type = parts.size() > 1 ? parts[1] : L"";
            std::wstring ref = L"";
            std::wstring opt = L"";  // 可空
            std::wstring array = L"";
            std::wstring remark = L"";
            
            // 从第3个参数开始检查
            for (size_t i = 2; i < parts.size(); i++) {
                std::wstring p = parts[i];
                if (p.empty()) continue;
                
                // 去除引号
                if (p.size() >= 2 && p.front() == L'"' && p.back() == L'"') {
                    remark = p.substr(1, p.size() - 2);
                    continue;
                }
                
                // 检查是否包含"参考"
                if (p.find(L"参考") != std::wstring::npos) {
                    ref = L"参考";
                }
                // 检查是否包含"可空"
                if (p.find(L"可空") != std::wstring::npos) {
                    opt = L"可空";
                }
                // 检查是否包含"数组"
                if (p.find(L"数组") != std::wstring::npos) {
                    array = L"数组";
                }
                
                // 如果不是特殊关键字，且不为空，可能是备注（没有引号的备注）
                if (p.find(L"参考") == std::wstring::npos && 
                    p.find(L"可空") == std::wstring::npos && 
                    p.find(L"数组") == std::wstring::npos && 
                    remark.empty()) {
                    remark = p;
                }
            }
            
            // 转换标记为√
            std::wstring refMark = (!ref.empty()) ? L"√" : L"";
            std::wstring optMark = (!opt.empty()) ? L"√" : L"";
            std::wstring arrayMark = (!array.empty()) ? L"√" : L"";
            
            result += name + L"\t" + type + L"\t" + refMark + L"\t" + optMark + L"\t" + arrayMark + L"\t" + remark + L"\n";
        }
        else if ((checkStr.find(L".局部变量") == 0 || checkStr.find(L"局部变量") == 0) &&
                 checkStr.find(L"变量名") == std::wstring::npos) {
            if (lastType != 1) {
                result += L"变量名\t类型\t静态\t数组\t备注\n";
                lastType = 1;
            }
            std::wstring content = trimmed;
            size_t spacePos = trimmed.find(L" ");
            if (spacePos != std::wstring::npos) {
                content = trimmed.substr(spacePos + 1);
                size_t nStart = content.find_first_not_of(L" ");
                if (nStart != std::wstring::npos) content = content.substr(nStart);
            }
            
            std::vector<std::wstring> parts = splitParts(content);
            
            // EPL局部变量格式: .局部变量 名称, 类型, [静态], [数组], [备注]
            std::wstring name = parts.size() > 0 ? parts[0] : L"";
            std::wstring type = parts.size() > 1 ? parts[1] : L"";
            std::wstring stat = L"";
            std::wstring array = L"";
            std::wstring remark = L"";
            
            // 从第3个参数开始检查
            for (size_t i = 2; i < parts.size(); i++) {
                std::wstring& p = parts[i];
                if (p.size() >= 2 && p.front() == L'"' && p.back() == L'"') {
                    // 带引号的是备注
                    remark = p.substr(1, p.size() - 2);
                } else if (p == L"静态") {
                    stat = p;
                } else if (p.find(L"数组") != std::wstring::npos) {
                    array = p;
                }
            }
            
            // 转换静态和数组标记为√
            std::wstring statMark = (!stat.empty()) ? L"√" : L"";
            std::wstring arrayMark = (!array.empty()) ? L"√" : L"";
            
            result += name + L"\t" + type + L"\t" + statMark + L"\t" + arrayMark + L"\t" + remark + L"\n";
        }
        else {
            lastType = 0;
            result += line + L"\n";
        }
    }
    return result;
}

// 将全角运算符还原为半角（编辑器显示用全角，源码需要半角）
static std::wstring RestoreHalfWidthOperators(const std::wstring& line) {
    std::wstring restored;
    bool inStr = false;
    for (size_t i = 0; i < line.length(); i++) {
        wchar_t ch = line[i];
        if (ch == L'"') { inStr = !inStr; restored += ch; continue; }
        if (inStr) { restored += ch; continue; }
        const wchar_t* replacement = nullptr;
        switch (ch) {
            case L'\uFF1D': replacement = L"="; break;
            case L'\uFF0B': replacement = L"+"; break;
            case L'\uFF0D': replacement = L"-"; break;
            case L'\u00D7': replacement = L"*"; break;
            case L'\u00F7': replacement = L"/"; break;
            case L'\uFF1E': replacement = L">"; break;
            case L'\uFF1C': replacement = L"<"; break;
            case L'\u2265': replacement = L">="; break;
            case L'\u2264': replacement = L"<="; break;
            case L'\u2260': replacement = L"!="; break;
            default: break;
        }
        if (replacement) {
            if (!restored.empty() && restored.back() == L' ') restored.pop_back();
            restored += replacement;
            if (i + 1 < line.length() && line[i + 1] == L' ') i++;
        } else {
            restored += ch;
        }
    }
    return restored;
}

// 内部格式转EPL代码
std::wstring ConvertInternalToEPL(const std::vector<std::wstring>& lines) {
    std::wstring result;
    int currentTable = 0; // 0:None, 1:Assembly, 2:Sub, 3:Param, 4:LocalVar, 5:ClassVar
    
    // 辅助函数：剥离一层嵌套标记，返回剥离后的内容
    // 嵌套标记格式：\u200C = 深度1，\u200C\u200B\u200C = 深度2，...
    // 剥离后：深度1的行变为普通行，深度2的行变为深度1的行
    auto stripOneLevel = [](const std::wstring& ln) -> std::wstring {
        if (ln.empty()) return ln;
        // 第一个字符必须是分支标记（\u200C 或 \u200D）
        if (ln[0] != L'\u200C' && ln[0] != L'\u200D') return ln;
        // 剥离第一个分支标记
        size_t pos = 1;
        // 如果后面是 \u200B（深度分隔符），也一并剥离
        if (pos < ln.length() && ln[pos] == L'\u200B') pos++;
        return ln.substr(pos);
    };
    
    // 辅助函数：检查行是否属于当前层级的缩进行（首字符是 \u200C、\u200D 或 \u200B）
    auto isIndentedLine = [](const std::wstring& ln) -> bool {
        return !ln.empty() && (ln[0] == L'\u200C' || ln[0] == L'\u200D' || ln[0] == L'\u200B');
    };
    
    // 辅助函数：剥离行首所有标记字符，获取纯内容
    auto stripAllMarkers = [](const std::wstring& ln) -> std::wstring {
        size_t pos = 0;
        while (pos < ln.length() && (ln[pos] == L'\u200C' || ln[pos] == L'\u200D' || ln[pos] == L'\u200B')) {
            pos++;
        }
        return ln.substr(pos);
    };
    
    // 辅助函数：检查行（可能含标记前缀）是否是.如果真命令
    auto isIfTrueCmd = [&stripAllMarkers](const std::wstring& content) -> bool {
        return stripAllMarkers(content).find(L".如果真") == 0;
    };
    
    // 辅助函数：检查行（可能含标记前缀）是否是.如果()命令
    auto isIfCmd = [&stripAllMarkers](const std::wstring& content) -> bool {
        std::wstring clean = stripAllMarkers(content);
        return clean.find(L".如果 (") == 0 || clean.find(L".如果(") == 0;
    };
    
    // 递归函数：将一组缩进行转换为EPL格式
    // indentedLines: 当前层级的行（已剥离外层标记）
    // indent: 当前缩进字符串（每层4个空格）
    std::function<void(const std::vector<std::wstring>&, const std::wstring&)> convertBlock;
    convertBlock = [&](const std::vector<std::wstring>& blockLines, const std::wstring& indent) {
        for (size_t bi = 0; bi < blockLines.size(); bi++) {
            const std::wstring& bline = blockLines[bi];
            
            // 检查是否是.如果真命令
            if (isIfTrueCmd(bline)) {
                result += indent + RestoreHalfWidthOperators(stripAllMarkers(bline)) + L"\n";
                
                // 收集子行（\u200C 开头的行）
                std::vector<std::wstring> subLines;
                bi++;
                while (bi < blockLines.size() && isIndentedLine(blockLines[bi])) {
                    subLines.push_back(stripOneLevel(blockLines[bi]));
                    bi++;
                }
                bi--; // 回退一行，for循环会++
                
                // 递归处理子行
                convertBlock(subLines, indent + L"    ");
                
                // 插入.如果真结束
                result += indent + L".如果真结束\n";
            }
            // 检查是否是.如果()命令
            else if (isIfCmd(bline)) {
                result += indent + RestoreHalfWidthOperators(stripAllMarkers(bline)) + L"\n";
                
                // 收集条件达成分支行（\u200C 开头）
                std::vector<std::wstring> trueLines;
                bi++;
                while (bi < blockLines.size()) {
                    const std::wstring& nextLine = blockLines[bi];
                    if (!nextLine.empty() && nextLine[0] == L'\u200C') {
                        trueLines.push_back(stripOneLevel(nextLine));
                        bi++;
                    } else {
                        break;
                    }
                }
                
                // 递归处理条件达成分支
                convertBlock(trueLines, indent + L"    ");
                
                // 插入.否则
                result += indent + L".否则\n";
                
                // 收集否则分支行（\u200D 开头）
                std::vector<std::wstring> falseLines;
                while (bi < blockLines.size()) {
                    const std::wstring& nextLine = blockLines[bi];
                    if (!nextLine.empty() && nextLine[0] == L'\u200D') {
                        falseLines.push_back(stripOneLevel(nextLine));
                        bi++;
                    } else {
                        break;
                    }
                }
                bi--; // 回退一行
                
                // 递归处理否则分支
                convertBlock(falseLines, indent + L"    ");
                
                // 插入.如果结束
                result += indent + L".如果结束\n";
            }
            // 普通行
            else if (!bline.empty()) {
                // 清除残留标记字符并还原运算符
                std::wstring cleanLine = bline;
                while (!cleanLine.empty() && (cleanLine[0] == L'\u200C' || cleanLine[0] == L'\u200D' || cleanLine[0] == L'\u200B')) {
                    cleanLine = cleanLine.substr(1);
                }
                result += indent + RestoreHalfWidthOperators(cleanLine) + L"\n";
            } else {
                // 空行
                result += indent + L"\n";
            }
        }
    };
    
    for (size_t i = 0; i < lines.size(); i++) {
        const auto& line = lines[i];
        
        if (line.find(L"\t") != std::wstring::npos) {
            std::vector<std::wstring> cells = SplitString(line, L"\t");
            if (cells.empty()) continue;
            
            if (cells[0] == L"版本") {
                result += L".版本 " + (cells.size() > 1 ? cells[1] : L"2") + L"\n";
                currentTable = 0;
            }
            else if (cells[0] == L"程序集名") {
                currentTable = 1;
            }
            else if (cells[0] == L"子程序名") {
                currentTable = 2;
            }
            else if (cells[0] == L"参数名") {
                currentTable = 3;
            }
            else if (cells[0] == L"变量名") {
                if (currentTable == 1) currentTable = 5; // ClassVar
                else currentTable = 4; // LocalVar
            }
            else {
                // Data row
                if (currentTable == 1) { // Assembly
                    result += L".程序集 " + cells[0] + L"\n";
                }
                else if (currentTable == 2) { // Sub
                    std::wstring remark = (cells.size() > 3) ? cells[3] : L"";
                    if (!remark.empty()) remark = L" \"" + remark + L"\"";
                    
                    // 将√转换回"公开"
                    std::wstring pub = (cells.size() > 2) ? cells[2] : L"";
                    if (!pub.empty()) pub = L", 公开";
                    
                    std::wstring ret = (cells.size() > 1) ? cells[1] : L"";
                    if (!ret.empty()) ret = L", " + ret;
                    
                    result += L".子程序 " + cells[0] + ret + pub + remark + L"\n";
                }
                else if (currentTable == 3) { // Param
                    std::wstring name = cells[0];
                    std::wstring type = (cells.size() > 1) ? cells[1] : L"";
                    std::wstring ref = (cells.size() > 2) ? cells[2] : L"";
                    std::wstring opt = (cells.size() > 3) ? cells[3] : L"";  // 可空（目前未使用）
                    std::wstring array = (cells.size() > 4) ? cells[4] : L"";
                    std::wstring remark = (cells.size() > 5) ? cells[5] : L"";
                    
                    if (!remark.empty()) remark = L", \"" + remark + L"\"";
                    // 将√转换回实际的标记
                    if (!array.empty()) array = L", " + type + L"数组";
                    if (!ref.empty()) ref = L", 参考";
                    
                    result += L".参数 " + name + L", " + type + ref + array + remark + L"\n";
                }
                else if (currentTable == 4) { // LocalVar
                    std::wstring name = cells[0];
                    std::wstring type = (cells.size() > 1) ? cells[1] : L"";
                    std::wstring stat = (cells.size() > 2) ? cells[2] : L"";
                    std::wstring array = (cells.size() > 3) ? cells[3] : L"";
                    std::wstring remark = (cells.size() > 4) ? cells[4] : L"";
                    
                    if (!remark.empty()) remark = L", \"" + remark + L"\"";
                    // 将√转换回实际的标记
                    if (!array.empty()) array = L", " + type + L"数组";
                    if (!stat.empty()) stat = L", 静态";
                    
                    result += L".局部变量 " + name + L", " + type + stat + array + remark + L"\n";
                }
                else if (currentTable == 5) { // ClassVar
                    std::wstring name = cells[0];
                    std::wstring type = (cells.size() > 1) ? cells[1] : L"";
                    std::wstring array = (cells.size() > 2) ? cells[2] : L""; // Index 2 is Array
                    std::wstring remark = (cells.size() > 3) ? cells[3] : L""; // Index 3 is Remark
                    
                    if (!remark.empty()) remark = L", \"" + remark + L"\"";
                    if (!array.empty()) array = L", " + array;
                    
                    result += L".程序集变量 " + name + L", " + type + array + remark + L"\n";
                }
            }
        } else {
            // 普通代码行 - 使用递归函数处理流程控制
            std::wstring trimmedLine = line;
            size_t trimStart = trimmedLine.find_first_not_of(L" \t");
            if (trimStart != std::wstring::npos) {
                trimmedLine = trimmedLine.substr(trimStart);
            }
            
            // 检查是否是.如果真或.如果()命令行（顶层）
            if ((isIfTrueCmd(trimmedLine) || isIfCmd(trimmedLine)) && i + 1 < lines.size()) {
                // 将命令行和其子行收集到一个block中，使用递归函数处理
                std::vector<std::wstring> block;
                block.push_back(line);
                
                size_t j = i + 1;
                while (j < lines.size() && isIndentedLine(lines[j])) {
                    block.push_back(lines[j]);
                    j++;
                }
                
                // 递归转换
                convertBlock(block, L"");
                
                // 跳过已处理的行
                i = j - 1;
            } else if (!line.empty() && (line[0] == L'\u200C' || line[0] == L'\u200D' || line[0] == L'\u200B' || line[0] == L'\u2060')) {
                // 跳过孤立的特殊标记行（参数展开行或流程控制内的行）
                continue;
            } else {
                // 普通行，直接输出（去除可能的特殊字符）
                std::wstring cleanLine = line;
                while (!cleanLine.empty() && (cleanLine[0] == L'\u200C' || cleanLine[0] == L'\u200D' || cleanLine[0] == L'\u200B' || cleanLine[0] == L'\u2060')) {
                    cleanLine = cleanLine.substr(1);
                }
                result += RestoreHalfWidthOperators(cleanLine) + L"\n";
            }
        }
    }
    return result;
}

bool SaveFile(const std::wstring& path, EditorDocument* data) {
    // 先关闭文件锁以便保存
    if (data->hFileLock != INVALID_HANDLE_VALUE) {
        CloseHandle(data->hFileLock);
        data->hFileLock = INVALID_HANDLE_VALUE;
    }
    
    // 根据文件类型转换格式
    std::wstring outputCode;
    if (data->fileType == FILE_TYPE_ELL) {
        // .ell文件直接保存原始内容，不进行EPL格式转换
        for (size_t i = 0; i < data->lines.size(); i++) {
            outputCode += data->lines[i];
            if (i < data->lines.size() - 1) {
                outputCode += L"\n";
            }
        }
    } else {
        // .eyc文件转换内部格式为EPL源码格式
        outputCode = ConvertInternalToEPL(data->lines);
    }
    
    // 直接使用宽字符路径打开文件，避免中文路径编码问题
    std::ofstream file(path.c_str(), std::ios::binary);
    if (!file.is_open()) {
        // 保存失败，尝试重新锁定文件（允许共享读取）
        data->hFileLock = CreateFileW(
            path.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ, // 允许其他程序读取
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
        return false;
    }
    
    int len = WideCharToMultiByte(CP_UTF8, 0, outputCode.c_str(), -1, NULL, 0, NULL, NULL);
    if (len > 0) {
        std::string utf8Code(len - 1, 0);
        WideCharToMultiByte(CP_UTF8, 0, outputCode.c_str(), -1, &utf8Code[0], len, NULL, NULL);
        file << utf8Code;
    }
    
    file.close();
    data->modified = false;
    data->filePath = path;
    
    // 保存成功后重新锁定文件（允许共享读取）
    data->hFileLock = CreateFileW(
        path.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ, // 允许其他程序读取（如编译器）
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    
    ExplorerSetFileModified(path, false);
    ExplorerAddFile(path);
    
    return true;
}

bool LoadFile(const std::wstring& path, EditorDocument* data) {
    wchar_t debugMsg[512];
    swprintf_s(debugMsg, L"[LoadFile] 开始加载文件: %s\n", path.c_str());
    OutputDebugStringW(debugMsg);
    
    // 根据文件扩展名设置文件类型
    size_t dotPos = path.find_last_of(L'.');
    if (dotPos != std::wstring::npos) {
        std::wstring ext = path.substr(dotPos);
        if (ext == L".ell" || ext == L".ELL") {
            data->fileType = FILE_TYPE_ELL;
        } else {
            data->fileType = FILE_TYPE_EYC;
        }
    } else {
        data->fileType = FILE_TYPE_EYC;
    }
    
    // 先关闭之前的文件锁
    if (data->hFileLock != INVALID_HANDLE_VALUE) {
        CloseHandle(data->hFileLock);
        data->hFileLock = INVALID_HANDLE_VALUE;
    }
    
    // 以共享读取模式打开文件（允许其他程序读取，但不允许写入）
    HANDLE hFile = CreateFileW(
        path.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ, // 允许其他程序读取（如编译器）
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    
    if (hFile == INVALID_HANDLE_VALUE) {
        // 如果独占模式失败，尝试只读模式
        hFile = CreateFileW(
            path.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
        
        if (hFile == INVALID_HANDLE_VALUE) {
            OutputDebugStringW(L"[LoadFile] 文件打开失败（只读模式也失败），返回false\n");
            return false;
        }
        
        MessageBoxW(NULL, L"文件正被其他程序使用，已以只读模式打开", L"提示", MB_OK | MB_ICONWARNING);
    }
    
    // 读取文件内容
    DWORD fileSize = GetFileSize(hFile, NULL);
    std::string content;
    if (fileSize > 0) {
        content.resize(fileSize);
        DWORD bytesRead;
        ReadFile(hFile, &content[0], fileSize, &bytesRead, NULL);
    }
    
    // 保持文件句柄以维持锁定
    data->hFileLock = hFile;
    
    std::wstring wContent;
    if (!content.empty()) {
        int len = MultiByteToWideChar(CP_UTF8, 0, content.c_str(), -1, NULL, 0);
        if (len > 0) {
            wContent.resize(len - 1);
            MultiByteToWideChar(CP_UTF8, 0, content.c_str(), -1, &wContent[0], len);
        }
    }
    
    // 根据文件类型处理内容
    std::wstring internalCode;
    if (data->fileType == FILE_TYPE_ELL) {
        // .ell文件直接使用原始内容，不进行EPL格式转换
        internalCode = wContent;
    } else {
        // .eyc文件转换EPL源码格式为内部格式
        internalCode = ConvertEPLToInternal(wContent);
    }
    
    // 分割为行
    data->lines.clear();
    std::wstringstream ss(internalCode);
    std::wstring line;
    while (std::getline(ss, line, L'\n')) {
        if (!line.empty() && line.back() == L'\r') line.pop_back();
        data->lines.push_back(line);
    }
    
    if (data->lines.empty()) data->lines.push_back(L"");
    
    data->modified = false;
    data->filePath = path;
    data->cursorLine = 0;
    data->cursorCol = 0;
    data->scrollY = 0;
    data->scrollX = 0;
    
    ExplorerSetFileModified(path, false);
    
    OutputDebugStringW(L"[LoadFile] 文件加载成功，返回true\n");
    
    // 只在非工作区模式和非项目模式下添加文件到资源管理器
    extern ResourceExplorerData g_ExplorerData;
    if (!g_ExplorerData.isWorkspaceMode && !g_ExplorerData.isProjectMode) {
        ExplorerAddFile(path);
    }
    
    return true;
}

bool ShowSaveDialog(HWND hWnd, std::wstring& path) {
    OPENFILENAMEW ofn = { 0 };
    WCHAR szFile[260] = { 0 };
    
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(WCHAR);
    ofn.lpstrFilter = L"YiCode Files (*.eyc)\0*.eyc\0DLL声明文件 (*.ell)\0*.ell\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrDefExt = L"eyc";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
    
    if (GetSaveFileNameW(&ofn)) {
        path = szFile;
        return true;
    }
    return false;
}

bool ShowOpenDialog(HWND hWnd, std::wstring& path) {
    OPENFILENAMEW ofn = { 0 };
    WCHAR szFile[260] = { 0 };
    
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(WCHAR);
    ofn.lpstrFilter = L"YiCode Files (*.eyc)\0*.eyc\0DLL声明文件 (*.ell)\0*.ell\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrDefExt = L"eyc";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    
    if (GetOpenFileNameW(&ofn)) {
        path = szFile;
        return true;
    }
    return false;
}
