#include "GlobalVarEditor.h"
#include "EditorContext.h"
#include "NameValidator.h"
#include "Theme.h"
#include "LibraryParser.h"
#include "PinyinHelper.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <gdiplus.h>

#ifndef NOMINMAX
#define NOMINMAX
#endif

using namespace Gdiplus;

extern AppTheme g_CurrentTheme;

// COLORREF转GDI+ Color辅助函数
inline Color ColorFromCOLORREF(COLORREF colorref) {
    return Color(255, GetRValue(colorref), GetGValue(colorref), GetBValue(colorref));
}

// 构造/析构
GlobalVarEditor::GlobalVarEditor(HWND hWnd, EditorContext* context)
    : TableEditor(hWnd, context)
    , m_leftMarginWidth(60)
    , m_lineNumberAreaWidth(50)
    , m_showTypeCompletion(false)
    , m_typeCompletionSelectedIndex(0)
    , m_typeCompletionScrollOffset(0)
    , m_skipNextSpace(false)
    , m_cursorVisible(true)
    , m_isDraggingSelection(false)
    , m_dragStartRow(-1)
    , m_isDraggingInLineNumber(false) {
    
    m_currentCellRect = {0, 0, 0, 0};
    m_typeCompletionRect = {0, 0, 0, 0};
    
    m_fileName = L"未命名.egv";
    
    // 启动光标闪烁定时器
    SetTimer(m_hWnd, CURSOR_TIMER_ID, CURSOR_BLINK_INTERVAL, NULL);
    
    // 创建镜像箭头光标（右上角箭头）
    // 获取标准箭头光标
    HCURSOR hArrow = LoadCursor(NULL, IDC_ARROW);
    ICONINFO iconInfo;
    if (GetIconInfo(hArrow, &iconInfo)) {
        // 创建镜像位图
        BITMAP bm;
        GetObject(iconInfo.hbmMask, sizeof(bm), &bm);
        
        HDC hdcScreen = GetDC(NULL);
        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        HDC hdcMem2 = CreateCompatibleDC(hdcScreen);
        
        HBITMAP hbmMask = CreateCompatibleBitmap(hdcScreen, bm.bmWidth, bm.bmHeight);
        HBITMAP hbmColor = iconInfo.hbmColor ? CreateCompatibleBitmap(hdcScreen, bm.bmWidth, bm.bmHeight) : NULL;
        
        SelectObject(hdcMem, iconInfo.hbmMask);
        SelectObject(hdcMem2, hbmMask);
        
        // 水平镜像mask
        StretchBlt(hdcMem2, bm.bmWidth - 1, 0, -bm.bmWidth, bm.bmHeight,
                   hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
        
        if (iconInfo.hbmColor && hbmColor) {
            SelectObject(hdcMem, iconInfo.hbmColor);
            SelectObject(hdcMem2, hbmColor);
            StretchBlt(hdcMem2, bm.bmWidth - 1, 0, -bm.bmWidth, bm.bmHeight,
                       hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
        }
        
        DeleteDC(hdcMem);
        DeleteDC(hdcMem2);
        ReleaseDC(NULL, hdcScreen);
        
        ICONINFO mirrorInfo;
        mirrorInfo.fIcon = FALSE;
        mirrorInfo.xHotspot = bm.bmWidth - iconInfo.xHotspot - 1;
        mirrorInfo.yHotspot = iconInfo.yHotspot;
        mirrorInfo.hbmMask = hbmMask;
        mirrorInfo.hbmColor = hbmColor;
        
        m_hMirrorArrowCursor = CreateIconIndirect(&mirrorInfo);
        
        DeleteObject(hbmMask);
        if (hbmColor) DeleteObject(hbmColor);
        DeleteObject(iconInfo.hbmMask);
        if (iconInfo.hbmColor) DeleteObject(iconInfo.hbmColor);
    } else {
        m_hMirrorArrowCursor = NULL;
    }
}

GlobalVarEditor::~GlobalVarEditor() {
    // 停止光标闪烁定时器
    KillTimer(m_hWnd, CURSOR_TIMER_ID);
    
    // 销毁镜像光标
    if (m_hMirrorArrowCursor) {
        DestroyCursor(m_hMirrorArrowCursor);
        m_hMirrorArrowCursor = NULL;
    }
}

// === 重写 TableEditor 的虚接口 ===

std::vector<ColumnDef> GlobalVarEditor::GetColumnDefs() const {
    // 列定义：变量名、类型、数组、公开、备注
    std::vector<ColumnDef> cols;
    cols.push_back(ColumnDef(L"变量名", 120));           // 缩短变量名列
    cols.push_back(ColumnDef(L"类型", 100));             // 缩短类型列
    cols.push_back(ColumnDef(L"数组", 80));             // 数组维度
    cols.push_back(ColumnDef(L"公开", 50, true, true)); // 复选框，50宽度
    cols.push_back(ColumnDef(L"备注", 200));            // 备注列
    return cols;
}

int GlobalVarEditor::GetRowCount() const {
    return (int)m_variables.size();
}

std::wstring GlobalVarEditor::GetCellText(int row, int col) const {
    if (row < 0 || row >= (int)m_variables.size()) {
        return L"";
    }
    
    const GlobalVariable& var = m_variables[row];
    
    switch (col) {
        case 0: return var.name;
        case 1: return var.type;
        case 2: return var.arrayDim;  // 数组维度文本
        case 3: return var.isPublic ? L"✓" : L"";
        case 4: return var.comment;
        default: return L"";
    }
}

void GlobalVarEditor::SetCellValue(int row, int col, const std::wstring& value) {
    if (row < 0 || row >= (int)m_variables.size()) {
        return;
    }
    
    GlobalVariable& var = m_variables[row];
    
    switch (col) {
        case 0: var.name = value; break;
        case 1: var.type = value; break;
        case 2: var.arrayDim = value; break;  // 数组维度
        case 4: var.comment = value; break;
    }
    
    SetModified(true);
    
    // 通知数据变更
    if (m_pContext && col == 0) {
        m_pContext->NotifyDataChanged(EditorFileType::EgvGlobalVar, value);
    }
}

bool GlobalVarEditor::IsCellTextEditable(int row, int col) const {
    // col == 3 是公开复选框，不可文本编辑
    return col != 3;
}

bool GlobalVarEditor::GetCellCheckState(int row, int col) const {
    if (row < 0 || row >= (int)m_variables.size()) {
        return false;
    }
    
    const GlobalVariable& var = m_variables[row];
    
    if (col == 3) return var.isPublic;  // 只有公开列是复选框
    
    return false;
}

void GlobalVarEditor::SetCellCheckState(int row, int col, bool checked) {
    if (row < 0 || row >= (int)m_variables.size()) {
        return;
    }
    
    GlobalVariable& var = m_variables[row];
    
    if (col == 3) {  // 只有公开列是复选框
        var.isPublic = checked;
        SetModified(true);
    }
}

std::wstring GlobalVarEditor::ValidateCell(int row, int col, const std::wstring& value) const {
    // 验证变量名
    if (col == 0) {
        if (value.empty()) {
            return L"变量名不能为空";
        }
        
        if (m_pContext && m_pContext->GetNameValidator()) {
            return m_pContext->GetNameValidator()->ValidateName(
                value, SymbolType::GlobalVariable, m_filePath);
        }
    }
    
    // 验证类型
    if (col == 1 && value.empty()) {
        return L"类型不能为空";
    }
    
    return L"";
}

std::vector<std::wstring> GlobalVarEditor::SerializeState() const {
    std::vector<std::wstring> state;
    
    state.push_back(std::to_wstring(m_variables.size()));
    
    for (const auto& var : m_variables) {
        state.push_back(var.name);
        state.push_back(var.type);
        state.push_back(var.arrayDim);
        state.push_back(var.isPublic ? L"1" : L"0");
        state.push_back(var.comment);
    }
    
    return state;
}

void GlobalVarEditor::RestoreState(const std::vector<std::wstring>& state) {
    if (state.empty()) return;
    
    size_t index = 0;
    m_variables.clear();
    
    if (index < state.size()) {
        int count = std::stoi(state[index++]);
        
        for (int i = 0; i < count && index + 4 < state.size(); i++) {
            GlobalVariable var;
            var.name = state[index++];
            var.type = state[index++];
            var.arrayDim = state[index++];
            var.isPublic = (state[index++] == L"1");
            var.comment = state[index++];
            
            m_variables.push_back(var);
        }
    }
}

void GlobalVarEditor::ParseContent(const std::vector<std::wstring>& lines) {
    m_variables.clear();
    
    // 辅助函数：分割逗号分隔的字符串（处理引号内的逗号）
    auto splitByComma = [](const std::wstring& str) -> std::vector<std::wstring> {
        std::vector<std::wstring> parts;
        std::wstring current;
        bool inQuotes = false;
        
        for (wchar_t ch : str) {
            if (ch == L'"') {
                inQuotes = !inQuotes;
                current += ch;
            } else if (ch == L',' && !inQuotes) {
                size_t start = current.find_first_not_of(L" \t");
                size_t end = current.find_last_not_of(L" \t");
                if (start != std::wstring::npos) {
                    parts.push_back(current.substr(start, end - start + 1));
                } else {
                    parts.push_back(L"");
                }
                current.clear();
            } else {
                current += ch;
            }
        }
        // 最后一部分
        size_t start = current.find_first_not_of(L" \t");
        size_t end = current.find_last_not_of(L" \t");
        if (start != std::wstring::npos) {
            parts.push_back(current.substr(start, end - start + 1));
        } else if (!current.empty()) {
            parts.push_back(L"");
        }
        
        return parts;
    };
    
    for (const auto& line : lines) {
        if (line.empty()) continue;
        
        // 跳过版本行和注释行
        if (line.find(L".版本") == 0 || line.find(L"版本") == 0 || line[0] == L'#') continue;
        
        // 判断是全局变量
        // 支持两种格式: ".全局变量 " 和 "全局变量 "
        bool isGlobalVar = false;
        std::wstring rest;
        
        if (line.find(L".全局变量 ") == 0) {
            isGlobalVar = true;
            rest = line.substr(6);  // 跳过 ".全局变量 "
        } else if (line.find(L"全局变量 ") == 0) {
            isGlobalVar = true;
            rest = line.substr(5);  // 跳过 "全局变量 "
        }
        
        if (isGlobalVar) {
            // 格式: 全局变量 名称, 类型, 公开/私有, 数组维度, 备注
            GlobalVariable var;
            var.isPublic = false;  // 默认为私有
            
            auto parts = splitByComma(rest);
            
            if (parts.size() >= 1) var.name = parts[0];
            if (parts.size() >= 2) var.type = parts[1];
            if (parts.size() >= 3) {
                // 公开/私有
                var.isPublic = (parts[2] == L"公开");
            }
            if (parts.size() >= 4) {
                // 数组维度：可能是"0"、"5"、"5,5"等
                var.arrayDim = parts[3];
                // 移除引号
                if (var.arrayDim.length() >= 2 && var.arrayDim[0] == L'"' && var.arrayDim.back() == L'"') {
                    var.arrayDim = var.arrayDim.substr(1, var.arrayDim.length() - 2);
                }
            }
            if (parts.size() >= 5) var.comment = parts[4];
            
            m_variables.push_back(var);
        }
    }
}

std::vector<std::wstring> GlobalVarEditor::GenerateContent() const {
    std::vector<std::wstring> lines;
    
    lines.push_back(L".版本 2");
    lines.push_back(L"");
    
    for (const auto& var : m_variables) {
        std::wstring line = L".全局变量 " + var.name;
        
        // 只有在有值时才添加后续字段
        if (!var.type.empty()) {
            line += L", " + var.type;
            
            // 公开/私有
            line += L", " + std::wstring(var.isPublic ? L"公开" : L"");
            
            // 数组维度
            if (!var.arrayDim.empty()) {
                line += L", \"" + var.arrayDim + L"\"";  // 加引号
            } else {
                line += L", ";
            }
            
            // 备注
            if (!var.comment.empty()) {
                line += L", " + var.comment;
            }
        }
        
        lines.push_back(line);
    }
    
    lines.push_back(L"");
    
    return lines;
}

void GlobalVarEditor::InsertRow(int afterRow) {
    GlobalVariable newVar;
    newVar.type = L"整数型";
    newVar.arrayDim = L"";  // 空表示非数组
    newVar.isPublic = false;  // 默认不公开
    newVar.comment = L"";
    
    // 生成不重复的变量名
    std::wstring baseName = L"新变量";
    std::wstring newName = baseName;
    int suffix = 1;
    
    // 检查变量名是否已存在
    auto nameExists = [this](const std::wstring& name) -> bool {
        for (const auto& var : m_variables) {
            if (var.name == name) {
                return true;
            }
        }
        return false;
    };
    
    // 如果名称已存在，添加数字后缀
    while (nameExists(newName)) {
        newName = baseName + std::to_wstring(suffix);
        suffix++;
    }
    
    newVar.name = newName;
    
    if (afterRow < 0 || m_variables.empty()) {
        m_variables.push_back(newVar);
    } else if (afterRow >= (int)m_variables.size()) {
        m_variables.push_back(newVar);
    } else {
        m_variables.insert(m_variables.begin() + afterRow + 1, newVar);
    }
    
    SetModified(true);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void GlobalVarEditor::DeleteRow(int row) {
    if (row >= 0 && row < (int)m_variables.size()) {
        m_variables.erase(m_variables.begin() + row);
        SetModified(true);
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

// === 文件操作重写（支持JSON） ===

bool GlobalVarEditor::LoadFile(const std::wstring& path) {
    std::wstring ext = path.substr(path.find_last_of(L'.'));
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == L".json") {
        // 正确转换路径为 UTF-8
        int pathLen = WideCharToMultiByte(CP_UTF8, 0, path.c_str(), -1, NULL, 0, NULL, NULL);
        std::string pathA(pathLen - 1, '\0');
        WideCharToMultiByte(CP_UTF8, 0, path.c_str(), -1, &pathA[0], pathLen, NULL, NULL);
        std::ifstream file(pathA);
        if (!file.is_open()) {
            return false;
        }
        
        try {
            json j;
            file >> j;
            file.close();
            
            FromJson(j);
            
            m_filePath = path;
            size_t pos = path.find_last_of(L"\\/");
            m_fileName = (pos != std::wstring::npos) ? path.substr(pos + 1) : path;
            m_modified = false;
            m_showWelcomePage = false;
            
            InvalidateRect(m_hWnd, NULL, FALSE);
            return true;
        } catch (...) {
            return false;
        }
    } else {
        // 文本格式 - 使用 Windows API 读取 UTF-8 文件
        HANDLE hFile = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            return false;
        }
        
        DWORD fileSize = GetFileSize(hFile, NULL);
        if (fileSize == INVALID_FILE_SIZE || fileSize == 0) {
            CloseHandle(hFile);
            return false;
        }
        
        std::vector<char> buffer(fileSize + 1);
        DWORD bytesRead = 0;
        if (!ReadFile(hFile, buffer.data(), fileSize, &bytesRead, NULL)) {
            CloseHandle(hFile);
            return false;
        }
        CloseHandle(hFile);
        buffer[bytesRead] = '\0';
        
        // 转换 UTF-8 到宽字符
        int wideLen = MultiByteToWideChar(CP_UTF8, 0, buffer.data(), -1, NULL, 0);
        std::wstring wideContent(wideLen, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, buffer.data(), -1, &wideContent[0], wideLen);
        
        // 按行分割
        std::vector<std::wstring> lines;
        std::wstringstream wss(wideContent);
        std::wstring line;
        while (std::getline(wss, line)) {
            // 移除可能的回车符
            if (!line.empty() && line.back() == L'\r') {
                line.pop_back();
            }
            lines.push_back(line);
        }
        
        ParseContent(lines);
        
        m_filePath = path;
        size_t pos = path.find_last_of(L"\\/");
        m_fileName = (pos != std::wstring::npos) ? path.substr(pos + 1) : path;
        m_modified = false;
        m_showWelcomePage = false;
        
        InvalidateRect(m_hWnd, NULL, FALSE);
        return true;
    }
}

bool GlobalVarEditor::SaveFile(const std::wstring& path) {
    std::wstring ext = path.substr(path.find_last_of(L'.'));
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == L".json") {
        // 正确转换路径为 UTF-8
        int pathLen = WideCharToMultiByte(CP_UTF8, 0, path.c_str(), -1, NULL, 0, NULL, NULL);
        std::string pathA(pathLen - 1, '\0');
        WideCharToMultiByte(CP_UTF8, 0, path.c_str(), -1, &pathA[0], pathLen, NULL, NULL);
        std::ofstream file(pathA);
        if (!file.is_open()) {
            return false;
        }
        
        try {
            json j = ToJson();
            file << j.dump(2);
            file.close();
            
            m_filePath = path;
            size_t pos = path.find_last_of(L"\\/");
            m_fileName = (pos != std::wstring::npos) ? path.substr(pos + 1) : path;
            m_modified = false;
            
            InvalidateRect(m_hWnd, NULL, FALSE);
            return true;
        } catch (...) {
            return false;
        }
    } else {
        // 文本格式 - 使用 Windows API 写入 UTF-8 文件
        auto lines = GenerateContent();
        
        // 合并所有行
        std::wstring wideContent;
        for (const auto& line : lines) {
            wideContent += line + L"\n";
        }
        
        // 转换宽字符到 UTF-8
        int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wideContent.c_str(), -1, NULL, 0, NULL, NULL);
        std::vector<char> utf8Buffer(utf8Len);
        WideCharToMultiByte(CP_UTF8, 0, wideContent.c_str(), -1, utf8Buffer.data(), utf8Len, NULL, NULL);
        
        HANDLE hFile = CreateFileW(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            return false;
        }
        
        DWORD bytesWritten = 0;
        // 不写入终止符
        if (!WriteFile(hFile, utf8Buffer.data(), utf8Len - 1, &bytesWritten, NULL)) {
            CloseHandle(hFile);
            return false;
        }
        CloseHandle(hFile);
        
        m_filePath = path;
        size_t pos = path.find_last_of(L"\\/");
        m_fileName = (pos != std::wstring::npos) ? path.substr(pos + 1) : path;
        m_modified = false;
        
        InvalidateRect(m_hWnd, NULL, FALSE);
        return true;
    }
}

// === JSON 序列化 ===

json GlobalVarEditor::ToJson() const {
    json j;
    j["version"] = 1;
    j["variables"] = json::array();
    
    for (const auto& var : m_variables) {
        json item;
        item["name"] = WStringToUtf8String(var.name);
        item["type"] = WStringToUtf8String(var.type);
        item["arrayDim"] = WStringToUtf8String(var.arrayDim);
        item["public"] = var.isPublic;
        item["comment"] = WStringToUtf8String(var.comment);
        
        j["variables"].push_back(item);
    }
    
    return j;
}

void GlobalVarEditor::FromJson(const json& j) {
    m_variables.clear();
    
    if (j.contains("variables") && j["variables"].is_array()) {
        for (const auto& item : j["variables"]) {
            GlobalVariable var;
            
            if (item.contains("name")) var.name = Utf8StringToWString(item["name"]);
            if (item.contains("type")) var.type = Utf8StringToWString(item["type"]);
            if (item.contains("arrayDim")) var.arrayDim = Utf8StringToWString(item["arrayDim"]);
            if (item.contains("public")) var.isPublic = item["public"];
            if (item.contains("comment")) var.comment = Utf8StringToWString(item["comment"]);
            
            m_variables.push_back(var);
        }
    }
}

std::wstring GlobalVarEditor::WStringToUtf8String(const std::wstring& wstr) const {
    if (wstr.empty()) return L"";
    
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (len <= 0) return L"";
    
    std::string utf8Str(len - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8Str[0], len, NULL, NULL);
    
    return std::wstring(utf8Str.begin(), utf8Str.end());
}

std::wstring GlobalVarEditor::Utf8StringToWString(const std::string& str) const {
    if (str.empty()) return L"";
    
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    if (len <= 0) return L"";
    
    std::wstring wstr(len - 1, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], len);
    
    return wstr;
}

// === Protected 方法 ===

std::wstring* GlobalVarEditor::GetEditingCellTextPtr() {
    if (!m_isEditing || m_editingRow < 0) {
        return nullptr;
    }
    
    // 直接返回已有的编辑缓冲区，不重新加载
    // 初始化在StartEditCell中完成
    return &m_editBuffer;
}

// === 数据类型补全相关 ===

std::vector<std::wstring> GlobalVarEditor::GetAllDataTypes() const {
    return LibraryParser::GetInstance().GetAllDataTypeNames();
}

void GlobalVarEditor::UpdateTypeCompletion(const std::wstring& text) {
    m_typeCompletionItems.clear();
    
    if (text.empty()) {
        // 显示所有类型
        m_typeCompletionItems = GetAllDataTypes();
    } else {
        // 过滤匹配的类型
        auto allTypes = GetAllDataTypes();
        
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
        // 支持：文b -> 文本型，整s -> 整数型，文ben -> 文本型
        auto mixedMatch = [&](const std::wstring& input, const std::wstring& target) -> bool {
            std::wstring lowerInput = toLowerUnicode(input);
            std::wstring lowerTarget = toLowerUnicode(target);
            
            size_t inputPos = 0;
            size_t targetPos = 0;
            
            while (inputPos < lowerInput.length() && targetPos < lowerTarget.length()) {
                wchar_t inputChar = lowerInput[inputPos];
                wchar_t targetChar = lowerTarget[targetPos];
                
                if (inputChar == targetChar) {
                    // 直接字符匹配
                    inputPos++;
                    targetPos++;
                } else if (isAsciiAlpha(inputChar)) {
                    // 输入是英文字母，尝试匹配目标字符的拼音
                    std::wstring targetCharStr(1, target[targetPos]);
                    std::wstring targetPinyin = PinyinHelper::GetStringPinyin(targetCharStr);
                    std::wstring targetInitial = PinyinHelper::GetStringInitials(targetCharStr);
                    std::wstring lowerPinyin = toLowerUnicode(targetPinyin);
                    std::wstring lowerInitial = toLowerUnicode(targetInitial);
                    
                    // 优先尝试匹配完整拼音
                    if (lowerPinyin.length() > 0 && inputPos + lowerPinyin.length() <= lowerInput.length()) {
                        std::wstring inputSubstr = lowerInput.substr(inputPos, lowerPinyin.length());
                        if (inputSubstr == lowerPinyin) {
                            inputPos += lowerPinyin.length();
                            targetPos++;
                        }
                        // 完整拼音不匹配，尝试匹配首字母
                        else if (!lowerInitial.empty() && lowerInitial[0] == inputChar) {
                            inputPos++;
                            targetPos++;
                        } else {
                            return false;
                        }
                    }
                    // 剩余字符不足以匹配完整拼音，尝试匹配首字母
                    else if (!lowerInitial.empty() && lowerInitial[0] == inputChar) {
                        inputPos++;
                        targetPos++;
                    } else {
                        return false;
                    }
                } else {
                    // 不匹配
                    return false;
                }
            }
            
            // 输入已全部匹配
            return inputPos == lowerInput.length();
        };
        
        std::wstring lowerText = toLowerUnicode(text);
        
        for (const auto& type : allTypes) {
            std::wstring lowerType = toLowerUnicode(type);
            
            // 1. 直接字符串前缀匹配
            if (lowerType.find(lowerText) == 0) {
                m_typeCompletionItems.push_back(type);
                continue;
            }
            
            // 2. 纯拼音匹配
            std::wstring fullPinyin = PinyinHelper::GetStringPinyin(type);
            std::wstring initials = PinyinHelper::GetStringInitials(type);
            std::wstring lowerFullPinyin = toLowerUnicode(fullPinyin);
            std::wstring lowerInitials = toLowerUnicode(initials);
            
            if (lowerInitials.find(lowerText) == 0 || lowerFullPinyin.find(lowerText) == 0) {
                m_typeCompletionItems.push_back(type);
                continue;
            }
            
            // 3. 中英混合智能匹配（支持"文b" -> "文本型"，"整s" -> "整数型"）
            if (mixedMatch(text, type)) {
                m_typeCompletionItems.push_back(type);
                continue;
            }
            
            // 4. 包含匹配（容错）
            if (lowerType.find(lowerText) != std::wstring::npos || 
                lowerFullPinyin.find(lowerText) != std::wstring::npos ||
                lowerInitials.find(lowerText) != std::wstring::npos) {
                m_typeCompletionItems.push_back(type);
            }
        }
    }
    
    if (!m_typeCompletionItems.empty()) {
        m_showTypeCompletion = true;
        m_typeCompletionSelectedIndex = 0;
        m_typeCompletionScrollOffset = 0;
        
        // 计算补全窗口位置
        int visibleCount = (std::min)((int)m_typeCompletionItems.size(), TYPE_COMPLETION_MAX_VISIBLE);
        int windowHeight = visibleCount * TYPE_COMPLETION_ITEM_HEIGHT + 4;
        
        m_typeCompletionRect.left = m_currentCellRect.left;
        m_typeCompletionRect.top = m_currentCellRect.bottom;
        m_typeCompletionRect.right = m_typeCompletionRect.left + 200;
        m_typeCompletionRect.bottom = m_typeCompletionRect.top + windowHeight;
    } else {
        HideTypeCompletion();
    }
}

void GlobalVarEditor::ApplyTypeCompletion() {
    if (!m_showTypeCompletion || m_typeCompletionItems.empty()) {
        return;
    }
    
    if (m_typeCompletionSelectedIndex >= 0 && m_typeCompletionSelectedIndex < (int)m_typeCompletionItems.size()) {
        std::wstring selectedType = m_typeCompletionItems[m_typeCompletionSelectedIndex];
        
        // 设置单元格值
        SetCellValue(m_editingRow, m_editingCol, selectedType);
        
        // 更新编辑缓冲区
        m_editBuffer = selectedType;
        m_editingCursorPos = (int)selectedType.length();
        m_selectionStart = m_selectionEnd = m_editingCursorPos;
        
        m_skipNextSpace = true;
    }
    
    HideTypeCompletion();
}

void GlobalVarEditor::HideTypeCompletion() {
    m_showTypeCompletion = false;
    m_typeCompletionItems.clear();
    m_typeCompletionSelectedIndex = 0;
    m_typeCompletionScrollOffset = 0;
}

// === 输入处理 ===

void GlobalVarEditor::HitTest(int x, int y, int& row, int& col, RECT& cellRect) {
    row = -1;
    col = -1;
    cellRect = {0, 0, 0, 0};
    
    // 检查是否在左边距内（行号区域）
    if (x < m_leftMarginWidth) {
        return;  // 点击在行号区域，不进行单元格选择
    }
    
    // 调整x坐标，减去左边距
    int adjustedX = x - m_leftMarginWidth;
    
    // 获取列定义
    std::vector<ColumnDef> cols = GetColumnDefs();
    
    // 计算行号（考虑滚动和表头）
    int yWithScroll = y + m_scrollY;
    if (yWithScroll < m_rowHeight) {
        // 点击在表头上
        return;
    }
    
    row = (yWithScroll - m_rowHeight) / m_rowHeight;
    if (row < 0 || row >= (int)m_variables.size()) {
        row = -1;
        return;
    }
    
    // 计算列号
    int xWithScroll = adjustedX + m_scrollX;
    int xPos = 10;  // 加上10像素左边距，与DrawGlobalVarTable保持一致
    for (size_t i = 0; i < cols.size(); i++) {
        if (xWithScroll >= xPos && xWithScroll < xPos + cols[i].width) {
            col = (int)i;
            
            // 计算单元格矩形（考虑左边距和滚动）
            cellRect.left = m_leftMarginWidth + xPos - m_scrollX;
            cellRect.top = m_rowHeight + row * m_rowHeight - m_scrollY;
            cellRect.right = cellRect.left + cols[i].width;
            cellRect.bottom = cellRect.top + m_rowHeight;
            break;
        }
        xPos += cols[i].width;
    }
}

void GlobalVarEditor::OnLButtonDown(int x, int y) {
    // 检查补全窗口点击
    if (m_showTypeCompletion) {
        POINT pt = {x, y};
        if (PtInRect(&m_typeCompletionRect, pt)) {
            int clickedIndex = (y - m_typeCompletionRect.top - 2) / TYPE_COMPLETION_ITEM_HEIGHT + m_typeCompletionScrollOffset;
            if (clickedIndex >= 0 && clickedIndex < (int)m_typeCompletionItems.size()) {
                m_typeCompletionSelectedIndex = clickedIndex;
                ApplyTypeCompletion();
                InvalidateRect(m_hWnd, NULL, FALSE);
                return;
            }
        } else {
            HideTypeCompletion();
        }
    }
    
    // 检查是否在行号区域点击
    if (x >= 0 && x < m_leftMarginWidth) {
        int row = GetRowAtY(y);
        if (row >= 0 && row < (int)m_variables.size()) {
            // 先退出编辑模式
            if (m_isEditing) {
                EndEditCell(true);
            }
            
            // 在行号区域点击，立即选择该行
            m_isDraggingSelection = true;
            m_isDraggingInLineNumber = true;
            m_dragStartRow = row;
            
            // 如果按住Ctrl键，切换该行的选择状态
            if (GetKeyState(VK_CONTROL) & 0x8000) {
                bool isSelected = IsRowSelected(row);
                SetRowSelection(row, !isSelected);
            } else {
                // 否则清除其他选择，只选择当前行
                ClearSelection();
                SetRowSelection(row, true);
            }
            
            InvalidateRect(m_hWnd, NULL, FALSE);
            SetCapture(m_hWnd);  // 捕获鼠标以支持拖动到窗口外
            return;
        }
    }
    
    // 进行 HitTest
    int row, col;
    RECT cellRect;
    HitTest(x, y, row, col, cellRect);
    
    if (row >= 0 && col >= 0) {
        // 清除多选状态
        ClearSelection();
        
        // 检查是否是复选框列
        if (col == 3) {  // 公开复选框
            if (row >= 0 && row < (int)m_variables.size()) {
                m_variables[row].isPublic = !m_variables[row].isPublic;
                SetModified(true);
                CreateSnapshot(L"Toggle public");
                InvalidateRect(m_hWnd, NULL, FALSE);
            }
            return;
        }
        
        // 如果正在编辑其他单元格，先保存
        if (m_isEditing && (m_editingRow != row || m_editingCol != col)) {
            EndEditCell(true);
        }
        
        // 开始编辑，传递相对于单元格左边的坐标
        int relativeX = x - cellRect.left;
        StartEditCell(row, col, relativeX);
        
        // 开始编辑时确保光标可见
        m_cursorVisible = true;
        
        // 确保窗口获得键盘焦点以接收键盘输入
        SetFocus(m_hWnd);
    } else {
        // 点击在表格外区域，先退出编辑模式
        if (m_isEditing) {
            EndEditCell(true);
        }
        
        // 开始拖动多选
        int clickRow = GetRowAtY(y);
        
        // 如果点击在空白区域（没有行号的地方），从最后一行开始选择
        if (clickRow < 0) {
            // 检查是否在编辑器有效区域内
            int yWithScroll = y + m_scrollY;
            if (yWithScroll >= m_rowHeight && !m_variables.empty()) {
                // 计算点击位置的逻辑行号
                int logicalRow = (yWithScroll - m_rowHeight) / m_rowHeight;
                // 如果超出实际行数，使用最后一行
                if (logicalRow >= (int)m_variables.size()) {
                    clickRow = (int)m_variables.size() - 1;
                }
            }
        }
        
        if (clickRow >= 0) {
            m_isDraggingSelection = true;
            m_isDraggingInLineNumber = false;
            m_dragStartRow = clickRow;
            
            // 单击时清除选择，等待拖动
            ClearSelection();
            InvalidateRect(m_hWnd, NULL, FALSE);
            
            SetCapture(m_hWnd);
        } else {
            // 点击空白区域
            ClearSelection();
            InvalidateRect(m_hWnd, NULL, FALSE);
        }
    }
    
    // 保存当前单元格矩形用于补全窗口定位
    if (m_isEditing) {
        m_currentCellRect = cellRect;
    }
}

void GlobalVarEditor::OnMouseWheel(int delta) {
    if (m_showTypeCompletion) {
        // 滚动补全窗口
        int scrollAmount = (delta > 0) ? -1 : 1;
        m_typeCompletionScrollOffset += scrollAmount;
        
        int maxScroll = (std::max)(0, (int)m_typeCompletionItems.size() - TYPE_COMPLETION_MAX_VISIBLE);
        m_typeCompletionScrollOffset = (std::max)(0, (std::min)(m_typeCompletionScrollOffset, maxScroll));
        
        InvalidateRect(m_hWnd, NULL, FALSE);
        return;
    }
    
    TableEditor::OnMouseWheel(delta);
}

void GlobalVarEditor::OnKeyDown(WPARAM wParam) {
    if (m_showTypeCompletion) {
        switch (wParam) {
            case VK_UP:
                m_typeCompletionSelectedIndex--;
                if (m_typeCompletionSelectedIndex < 0) {
                    m_typeCompletionSelectedIndex = (int)m_typeCompletionItems.size() - 1;
                }
                // 调整滚动
                if (m_typeCompletionSelectedIndex < m_typeCompletionScrollOffset) {
                    m_typeCompletionScrollOffset = m_typeCompletionSelectedIndex;
                }
                if (m_typeCompletionSelectedIndex >= m_typeCompletionScrollOffset + TYPE_COMPLETION_MAX_VISIBLE) {
                    m_typeCompletionScrollOffset = m_typeCompletionSelectedIndex - TYPE_COMPLETION_MAX_VISIBLE + 1;
                }
                InvalidateRect(m_hWnd, NULL, FALSE);
                return;
                
            case VK_DOWN:
                m_typeCompletionSelectedIndex++;
                if (m_typeCompletionSelectedIndex >= (int)m_typeCompletionItems.size()) {
                    m_typeCompletionSelectedIndex = 0;
                }
                // 调整滚动
                if (m_typeCompletionSelectedIndex < m_typeCompletionScrollOffset) {
                    m_typeCompletionScrollOffset = m_typeCompletionSelectedIndex;
                }
                if (m_typeCompletionSelectedIndex >= m_typeCompletionScrollOffset + TYPE_COMPLETION_MAX_VISIBLE) {
                    m_typeCompletionScrollOffset = m_typeCompletionSelectedIndex - TYPE_COMPLETION_MAX_VISIBLE + 1;
                }
                InvalidateRect(m_hWnd, NULL, FALSE);
                return;
                
            case VK_RETURN:
            case VK_TAB:
                ApplyTypeCompletion();
                InvalidateRect(m_hWnd, NULL, FALSE);
                return;
                
            case VK_ESCAPE:
                HideTypeCompletion();
                InvalidateRect(m_hWnd, NULL, FALSE);
                return;
        }
    }
    
    // 回车键插入新行（在编辑模式或非编辑模式都可以）
    if (wParam == VK_RETURN) {
        int currentRow = m_editingRow;
        
        // 如果正在编辑，先结束编辑
        if (m_isEditing) {
            EndEditCell(true);
        }
        
        // 如果有选中的行，使用选中的最后一行
        if (!m_selectedRows.empty()) {
            currentRow = *std::max_element(m_selectedRows.begin(), m_selectedRows.end());
        }
        
        // 如果没有有效行，在最后插入
        if (currentRow < 0) {
            currentRow = (int)m_variables.size() - 1;
        }
        
        // 插入新行
        InsertRow(currentRow);
        SetModified(true);
        InvalidateRect(m_hWnd, NULL, FALSE);
        return;
    }
    
    // Backspace键：如果在编辑模式，交给基类处理；如果不在编辑模式，什么也不做
    if (wParam == VK_BACK) {
        if (m_isEditing) {
            TableEditor::OnKeyDown(wParam);
        }
        return;
    }
    
    // Delete键删除行
    if (wParam == VK_DELETE && !m_isEditing) {
        // 检查是否有多选的行
        if (!m_selectedRows.empty()) {
            // 从后往前删除，避免索引错乱
            std::vector<int> sortedRows = m_selectedRows;
            std::sort(sortedRows.begin(), sortedRows.end(), std::greater<int>());
            for (int row : sortedRows) {
                if (row >= 0 && row < (int)m_variables.size()) {
                    m_variables.erase(m_variables.begin() + row);
                }
            }
            SetModified(true);
            ClearSelection();
            InvalidateRect(m_hWnd, NULL, FALSE);
        } else {
            // 没有多选时，删除当前编辑行
            int row = m_editingRow;
            if (row >= 0) {
                DeleteRow(row);
            }
        }
        return;
    }
    
    TableEditor::OnKeyDown(wParam);
}

void GlobalVarEditor::OnChar(WPARAM wParam) {
    // 空格上屏后跳过
    if (m_skipNextSpace && wParam == L' ') {
        m_skipNextSpace = false;
        return;
    }
    
    // 检查是否在编辑类型列
    if (m_isEditing && m_editingCol == 1) {  // 类型列
        // 调用基类处理字符输入
        TableEditor::OnChar(wParam);
        
        // 更新补全（使用编辑缓冲区）
        UpdateTypeCompletion(m_editBuffer);
        
        InvalidateRect(m_hWnd, NULL, FALSE);
        return;
    }
    
    TableEditor::OnChar(wParam);
}

bool GlobalVarEditor::OnSetCursor(int x, int y) {
    // 检查是否在行号区域
    if (x >= 0 && x < m_leftMarginWidth) {
        // 在行号区域，设置镜像箭头光标
        if (m_hMirrorArrowCursor) {
            ::SetCursor(m_hMirrorArrowCursor);
            return true;
        }
    }
    // 不在行号区域，使用默认光标
    return false;
}

void GlobalVarEditor::OnMouseMove(int x, int y, WPARAM wParam) {
    // 如果正在拖动选择
    if (m_isDraggingSelection) {
        int currentRow = GetRowAtY(y);
        
        // 如果鼠标在空白区域，使用最后一行
        if (currentRow < 0 && !m_variables.empty()) {
            int yWithScroll = y + m_scrollY;
            if (yWithScroll >= m_rowHeight) {
                int logicalRow = (yWithScroll - m_rowHeight) / m_rowHeight;
                if (logicalRow >= (int)m_variables.size()) {
                    currentRow = (int)m_variables.size() - 1;
                }
            }
        }
        
        if (currentRow >= 0 && currentRow < (int)m_variables.size()) {
            // 检测是否有实际拖动（当前行与起始行不同）
            // 或者鼠标移动到了不同位置
            if (currentRow != m_dragStartRow || !m_selectedRows.empty()) {
                // 选择从起始行到当前行的范围
                SelectRowRange(m_dragStartRow, currentRow);
                InvalidateRect(m_hWnd, NULL, FALSE);
            }
        }
        return;
    }
    
    // 调用基类的OnMouseMove
    TableEditor::OnMouseMove(x, y, wParam);
}

void GlobalVarEditor::OnLButtonUp(int x, int y) {
    // 如果正在拖动选择，结束拖动
    if (m_isDraggingSelection) {
        m_isDraggingSelection = false;
        m_isDraggingInLineNumber = false;
        ReleaseCapture();
        return;
    }
    
    // 调用基类的OnLButtonUp
    TableEditor::OnLButtonUp(x, y);
}

// === 多选辅助方法 ===

bool GlobalVarEditor::IsRowSelected(int row) const {
    return std::find(m_selectedRows.begin(), m_selectedRows.end(), row) != m_selectedRows.end();
}

void GlobalVarEditor::ClearSelection() {
    m_selectedRows.clear();
}

void GlobalVarEditor::SetRowSelection(int row, bool selected) {
    if (selected) {
        if (!IsRowSelected(row)) {
            m_selectedRows.push_back(row);
        }
    } else {
        auto it = std::find(m_selectedRows.begin(), m_selectedRows.end(), row);
        if (it != m_selectedRows.end()) {
            m_selectedRows.erase(it);
        }
    }
}

void GlobalVarEditor::SelectRowRange(int startRow, int endRow) {
    ClearSelection();
    
    int minRow = (std::min)(startRow, endRow);
    int maxRow = (std::max)(startRow, endRow);
    
    for (int row = minRow; row <= maxRow && row < (int)m_variables.size(); row++) {
        m_selectedRows.push_back(row);
    }
}

int GlobalVarEditor::GetRowAtY(int y) const {
    int yWithScroll = y + m_scrollY;
    
    // 减去表头高度
    if (yWithScroll < m_rowHeight) {
        return -1;
    }
    
    int row = (yWithScroll - m_rowHeight) / m_rowHeight;
    if (row < 0 || row >= (int)m_variables.size()) {
        return -1;
    }
    
    return row;
}

void GlobalVarEditor::StartEditCell(int row, int col, int clickX) {
    // 初始化编辑缓冲区
    m_editBuffer = GetCellText(row, col);
    
    m_isEditing = true;
    m_editingRow = row;
    m_editingCol = col;
    
    // 根据点击位置计算光标位置
    int cursorPos = 0;
    int colPadding = 5;
    
    if (clickX > 0) {
        // 使用 GDI+ 测量文本宽度来计算光标位置
        HDC hdc = GetDC(m_hWnd);
        if (hdc) {
            Graphics graphics(hdc);
            FontFamily fontFamily(L"Microsoft YaHei");
            Font dataFont(&fontFamily, (REAL)m_fontSize, FontStyleRegular, UnitPixel);
            StringFormat leftFormat(StringFormat::GenericTypographic());
            leftFormat.SetFormatFlags(StringFormatFlagsMeasureTrailingSpaces);
            
            // clickX已经是相对于单元格左边的坐标，直接使用
            int relativeClickX = clickX - colPadding;
            
            // 逐字符测量，找到最接近点击位置的光标位置
            for (size_t i = 0; i <= m_editBuffer.length(); i++) {
                std::wstring subStr = m_editBuffer.substr(0, i);
                RectF boundingBox;
                graphics.MeasureString(subStr.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &boundingBox);
                
                int textWidth = (int)boundingBox.Width;
                
                if (i == 0) {
                    if (relativeClickX <= textWidth / 2) {
                        cursorPos = 0;
                        break;
                    }
                } else {
                    std::wstring prevSubStr = m_editBuffer.substr(0, i - 1);
                    RectF prevBox;
                    graphics.MeasureString(prevSubStr.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &prevBox);
                    int prevWidth = (int)prevBox.Width;
                    int charMid = (prevWidth + textWidth) / 2;
                    
                    if (relativeClickX <= charMid) {
                        cursorPos = (int)(i - 1);
                        break;
                    }
                }
                
                cursorPos = (int)i;
            }
            
            ReleaseDC(m_hWnd, hdc);
        } else {
            // 无法获取 DC，默认放到末尾
            cursorPos = (int)m_editBuffer.length();
        }
    }
    
    m_editingCursorPos = cursorPos;
    m_selectionStart = cursorPos;
    m_selectionEnd = cursorPos;
    
    // 确保光标可见
    m_cursorVisible = true;
    
    InvalidateRect(m_hWnd, NULL, FALSE);
}

int GlobalVarEditor::GetTotalContentHeight() const {
    // 1个表头 + N个数据行
    int totalRows = 1 + (int)m_variables.size();
    return totalRows * m_rowHeight;
}

int GlobalVarEditor::GetTotalContentWidth() const {
    int width = m_leftMarginWidth;  // 加上左边距宽度
    auto cols = GetColumnDefs();
    for (const auto& col : cols) {
        width += col.width;
    }
    return width;
}

// === 绘制 ===

void GlobalVarEditor::DrawTable(HDC hdc, const RECT& clientRect) {
    Graphics graphics(hdc);
    
    // 绘制整个编辑器背景（rgb(31, 31, 31)）
    SolidBrush bgBrush(Color(255, 31, 31, 31));
    graphics.FillRectangle(&bgBrush, 0, 0, clientRect.right, clientRect.bottom);
    
    // 绘制左边距背景（行号区域，rgb(37, 37, 38)）
    SolidBrush lineNumBgBrush(Color(255, 37, 37, 38));
    RectF lineNumRect(0, 0, (REAL)m_leftMarginWidth, (REAL)clientRect.bottom);
    graphics.FillRectangle(&lineNumBgBrush, lineNumRect);
    
    // 绘制左边距右边框（分割线）
    Pen dividerPen(ColorFromCOLORREF(g_CurrentTheme.grid), 1);
    graphics.DrawLine(&dividerPen, m_leftMarginWidth, 0, m_leftMarginWidth, clientRect.bottom);
    
    // 绘制行号
    FontFamily fontFamily(L"Microsoft YaHei");
    Font lineNumFont(&fontFamily, (REAL)(m_fontSize - 2), FontStyleRegular, UnitPixel);
    SolidBrush lineNumBrush(Color(150, GetRValue(g_CurrentTheme.text), 
                                   GetGValue(g_CurrentTheme.text), 
                                   GetBValue(g_CurrentTheme.text)));
    StringFormat rightFormat;
    rightFormat.SetAlignment(StringAlignmentFar);
    rightFormat.SetLineAlignment(StringAlignmentCenter);
    
    // 计算内容区域起始位置和总行数
    int yPos = 0 - m_scrollY;
    int totalRows = 1 + (int)m_variables.size(); // 1个表头 + N个数据行
    
    // 绘制所有行号（只绘制数据行，跳过表头）
    if (!m_variables.empty()) {
    for (int row = 1; row < totalRows; row++) {
        int rowY = yPos + row * m_rowHeight;
        
        // 只绘制可见区域的行号
        if (rowY + m_rowHeight >= 0 && rowY < clientRect.bottom) {
            std::wstring lineNumText = std::to_wstring(row);
            RectF lineNumTextRect((REAL)5, (REAL)rowY, 
                                  (REAL)(m_leftMarginWidth - 10), (REAL)m_rowHeight);
            StringFormat rightAlignFormat;
            rightAlignFormat.SetAlignment(StringAlignmentFar);
            rightAlignFormat.SetLineAlignment(StringAlignmentCenter);
            graphics.DrawString(lineNumText.c_str(), -1, &lineNumFont, 
                              lineNumTextRect, &rightAlignFormat, &lineNumBrush);
        }
    }
    }
    
    // 设置裁剪区域：表格内容只在行号区域之外绘制
    Region clipRegion(Rect(m_leftMarginWidth, 0, clientRect.right - m_leftMarginWidth, clientRect.bottom));
    graphics.SetClip(&clipRegion);
    
    // 绘制表格内容（表头+数据行）
    DrawGlobalVarTable(graphics, yPos, clientRect);
    
    // 绘制数据类型自动补全窗口（最后绘制以确保在最上层）
    if (m_showTypeCompletion) {
        DrawTypeCompletionWindow(hdc);
    }
}

void GlobalVarEditor::DrawGlobalVarTable(Graphics& graphics, int yPos, const RECT& clientRect) {
    
    // 获取列定义
    std::vector<ColumnDef> cols = GetColumnDefs();
    
    // 计算列位置（距离行号区域有10像素左边距）
    std::vector<int> colX;
    int xPos = m_leftMarginWidth + 10 - m_scrollX;  // 加上10像素左边距
    for (size_t i = 0; i < cols.size(); i++) {
        colX.push_back(xPos);
        xPos += cols[i].width;
    }
    
    Pen gridPen(ColorFromCOLORREF(g_CurrentTheme.grid), 1);
    SolidBrush textBrush(ColorFromCOLORREF(g_CurrentTheme.text));
    SolidBrush headerBrush(Color(255, 58, 65, 81));  // 表头背景色（与DataTypeEditor一致）
    SolidBrush typeBrush(Color(255, 174, 144, 194)); // 类型列文本色（紫色）
    SolidBrush commentBrush(Color(255, 0, 200, 0));  // 备注列文本色（绿色）
    
    FontFamily fontFamily(L"Microsoft YaHei");
    Font normalFont(&fontFamily, (REAL)m_fontSize, FontStyleRegular, UnitPixel);
    Font boldFont(&fontFamily, (REAL)m_fontSize, FontStyleBold, UnitPixel);
    
    StringFormat centerFormat;
    centerFormat.SetAlignment(StringAlignmentCenter);
    centerFormat.SetLineAlignment(StringAlignmentCenter);
    
    StringFormat leftFormat;
    leftFormat.SetAlignment(StringAlignmentNear);
    leftFormat.SetLineAlignment(StringAlignmentCenter);
    
    // 计算表格总宽度
    int tableWidth = 0;
    for (const auto& col : cols) {
        tableWidth += col.width;
    }
    
    // 绘制表头（从左边距+10开始，宽度为所有列总宽）
    RectF headerRect((REAL)(m_leftMarginWidth + 10), (REAL)yPos, 
                     (REAL)tableWidth, (REAL)m_rowHeight);
    graphics.FillRectangle(&headerBrush, headerRect);
    
    // 绘制表头文字
    for (size_t col = 0; col < cols.size(); col++) {
        RectF cellRect((REAL)colX[col] + 5, (REAL)yPos, 
                       (REAL)(cols[col].width - 10), (REAL)m_rowHeight);
        graphics.DrawString(cols[col].name.c_str(), -1, &boldFont, 
                          cellRect, &leftFormat, &textBrush);
    }
    
    // 绘制表头下边框（只绘制表格宽度）
    graphics.DrawLine(&gridPen, m_leftMarginWidth + 10, yPos + m_rowHeight, 
                     m_leftMarginWidth + 10 + tableWidth, yPos + m_rowHeight);
    
    // 绘制表格左边框
    graphics.DrawLine(&gridPen, m_leftMarginWidth + 10, yPos, 
                     m_leftMarginWidth + 10, yPos + (1 + (int)m_variables.size()) * m_rowHeight);
    
    yPos += m_rowHeight;
    
    // 绘制数据行
    for (int row = 0; row < (int)m_variables.size(); row++) {
        int rowY = yPos + row * m_rowHeight;
        
        // 只绘制可见区域的行
        if (rowY + m_rowHeight < 0 || rowY >= clientRect.bottom) {
            continue;
        }
        
        const GlobalVariable& var = m_variables[row];
        
        // 如果该行被选中，绘制选中背景
        if (IsRowSelected(row)) {
            SolidBrush selectionBrush(Color(80, 100, 150, 255));  // 半透明蓝色背景
            RectF selectionRect((REAL)(m_leftMarginWidth), (REAL)rowY, 
                               (REAL)(tableWidth + 10), (REAL)m_rowHeight);
            graphics.FillRectangle(&selectionBrush, selectionRect);
        }
        
        // 绘制单元格内容
        for (size_t col = 0; col < cols.size(); col++) {
            RectF cellRect((REAL)colX[col] + 5, (REAL)rowY, 
                           (REAL)(cols[col].width - 10), (REAL)m_rowHeight);
            
            if (cols[col].isCheckbox) {
                // 只在勾选时绘制勾选标记，不绘制复选框方框
                if (var.isPublic) {
                    int checkSize = 16;
                    int checkX = colX[col] + (cols[col].width - checkSize) / 2;
                    int checkY = rowY + (m_rowHeight - checkSize) / 2;
                    
                    // 绘制勾选标记
                    Pen checkPen(ColorFromCOLORREF(g_CurrentTheme.text), 2);
                    graphics.DrawLine(&checkPen, checkX + 3, checkY + checkSize / 2, 
                                    checkX + checkSize / 2, checkY + checkSize - 3);
                    graphics.DrawLine(&checkPen, checkX + checkSize / 2, checkY + checkSize - 3, 
                                    checkX + checkSize - 3, checkY + 3);
                }
            } else {
                // 绘制文本
                std::wstring text = GetCellText(row, (int)col);
                
                // 如果是正在编辑的单元格，绘制编辑框
                if (m_isEditing && m_editingRow == row && m_editingCol == (int)col) {
                    // 不绘制编辑框背景，只绘制文本和光标
                    
                    // 根据列类型选择颜色
                    SolidBrush* editTextBrush = &textBrush;
                    if (col == 1) {  // 类型列
                        editTextBrush = &typeBrush;
                    } else if (col == 4) {  // 备注列
                        editTextBrush = &commentBrush;
                    }
                    
                    // 绘制编辑中的文本
                    graphics.DrawString(m_editBuffer.c_str(), -1, &normalFont, 
                                      cellRect, &leftFormat, editTextBrush);
                    
                    // 绘制光标
                    if (m_cursorVisible) {
                        std::wstring beforeCursor = m_editBuffer.substr(0, m_editingCursorPos);
                        RectF measureRect(0, 0, 10000, (REAL)m_rowHeight);
                        RectF boundingBox;
                        graphics.MeasureString(beforeCursor.c_str(), -1, &normalFont, 
                                             measureRect, &leftFormat, &boundingBox);
                        
                        int cursorX = colX[col] + 5 + (int)boundingBox.Width;
                        Pen cursorPen(ColorFromCOLORREF(g_CurrentTheme.text), 1);
                        graphics.DrawLine(&cursorPen, cursorX, rowY + 4, 
                                        cursorX, rowY + m_rowHeight - 4);
                    }
                } else {
                    // 绘制普通文本
                    SolidBrush* cellTextBrush = &textBrush;
                    
                    // 根据列类型选择颜色
                    if (col == 1) {  // 类型列
                        cellTextBrush = &typeBrush;
                    } else if (col == 4) {  // 备注列
                        cellTextBrush = &commentBrush;
                    }
                    
                    graphics.DrawString(text.c_str(), -1, &normalFont, 
                                      cellRect, &leftFormat, cellTextBrush);
                }
            }
        }
        
        // 绘制行底部边框（只绘制表格宽度）
        graphics.DrawLine(&gridPen, m_leftMarginWidth + 10, rowY + m_rowHeight, 
                         m_leftMarginWidth + 10 + tableWidth, rowY + m_rowHeight);
    }
    
    // 绘制列分隔线
    for (size_t col = 0; col < cols.size(); col++) {
        int lineX = colX[col] + cols[col].width;
        graphics.DrawLine(&gridPen, lineX, yPos - m_rowHeight, lineX, 
                         yPos + (int)m_variables.size() * m_rowHeight);
    }
}

void GlobalVarEditor::DrawTypeCompletionWindow(HDC hdc) {
    if (!m_showTypeCompletion || m_typeCompletionItems.empty()) return;
    
    // 窗口背景
    HBRUSH bgBrush = CreateSolidBrush(g_CurrentTheme.editorBg);
    FillRect(hdc, &m_typeCompletionRect, bgBrush);
    DeleteObject(bgBrush);
    
    // 窗口边框
    HPEN borderPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.grid);
    HPEN oldPen = (HPEN)SelectObject(hdc, borderPen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, m_typeCompletionRect.left, m_typeCompletionRect.top,
              m_typeCompletionRect.right, m_typeCompletionRect.bottom);
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(borderPen);
    
    // 绘制项目
    HFONT hFont = CreateFont(-13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft YaHei UI");
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
    
    SetBkMode(hdc, TRANSPARENT);
    
    int y = m_typeCompletionRect.top + 2;
    int visibleCount = (std::min)((int)m_typeCompletionItems.size() - m_typeCompletionScrollOffset, TYPE_COMPLETION_MAX_VISIBLE);
    
    for (int i = 0; i < visibleCount; i++) {
        int itemIndex = i + m_typeCompletionScrollOffset;
        if (itemIndex >= (int)m_typeCompletionItems.size()) break;
        
        RECT itemRect = {m_typeCompletionRect.left + 2, y,
                         m_typeCompletionRect.right - 2, y + TYPE_COMPLETION_ITEM_HEIGHT};
        
        // 选中项背景
        if (itemIndex == m_typeCompletionSelectedIndex) {
            HBRUSH selBrush = CreateSolidBrush(RGB(0, 120, 215));  // 蓝色选中
            FillRect(hdc, &itemRect, selBrush);
            DeleteObject(selBrush);
            SetTextColor(hdc, RGB(255, 255, 255));  // 白色文字
        } else {
            SetTextColor(hdc, g_CurrentTheme.text);
        }
        
        // 绘制文字
        itemRect.left += 5;
        DrawTextW(hdc, m_typeCompletionItems[itemIndex].c_str(), -1, &itemRect,
                  DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        
        y += TYPE_COMPLETION_ITEM_HEIGHT;
    }
    
    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
    
    // 绘制滚动条（如果需要）
    if ((int)m_typeCompletionItems.size() > TYPE_COMPLETION_MAX_VISIBLE) {
        int scrollBarWidth = 8;
        int scrollBarX = m_typeCompletionRect.right - scrollBarWidth - 2;
        int scrollBarHeight = m_typeCompletionRect.bottom - m_typeCompletionRect.top - 4;
        
        // 滚动条轨道
        RECT trackRect = {scrollBarX, m_typeCompletionRect.top + 2,
                          scrollBarX + scrollBarWidth, m_typeCompletionRect.bottom - 2};
        HBRUSH trackBrush = CreateSolidBrush(RGB(60, 60, 60));
        FillRect(hdc, &trackRect, trackBrush);
        DeleteObject(trackBrush);
        
        // 滚动条滑块
        int thumbHeight = (std::max)(20, scrollBarHeight * TYPE_COMPLETION_MAX_VISIBLE / (int)m_typeCompletionItems.size());
        int maxScroll = (int)m_typeCompletionItems.size() - TYPE_COMPLETION_MAX_VISIBLE;
        int thumbY = m_typeCompletionRect.top + 2 + 
                     (scrollBarHeight - thumbHeight) * m_typeCompletionScrollOffset / maxScroll;
        
        RECT thumbRect = {scrollBarX, thumbY, scrollBarX + scrollBarWidth, thumbY + thumbHeight};
        HBRUSH thumbBrush = CreateSolidBrush(RGB(100, 100, 100));
        FillRect(hdc, &thumbRect, thumbBrush);
        DeleteObject(thumbBrush);
    }
}

// === 窗口过程和创建函数 ===

// GlobalVarEditorData 成员函数实现
GlobalVarEditorData::GlobalVarEditorData(HWND hWnd) : editor(nullptr) {
    editor = new GlobalVarEditor(hWnd, nullptr);
}

GlobalVarEditorData::~GlobalVarEditorData() {
    if (editor) {
        delete editor;
        editor = nullptr;
    }
}

bool GlobalVarEditorData::LoadFile(const std::wstring& path) {
    if (editor) {
        return editor->LoadFile(path);
    }
    return false;
}

bool GlobalVarEditorData::SaveFile(const std::wstring& path) {
    if (editor) {
        return editor->SaveFile(path);
    }
    return false;
}

bool GlobalVarEditorData::IsModified() const {
    if (editor) {
        return editor->IsModified();
    }
    return false;
}

void GlobalVarEditorData::SetModified(bool modified) {
    if (editor) {
        editor->SetModified(modified);
    }
}

// GlobalVarEditor 窗口过程
LRESULT CALLBACK GlobalVarEditorWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_CREATE) {
        OutputDebugStringW(L"[GlobalVarEditorWndProc] WM_CREATE - 创建窗口\n");
        GlobalVarEditorData* data = new GlobalVarEditorData(hWnd);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));
        OutputDebugStringW(L"[GlobalVarEditorWndProc] WM_CREATE - 完成\n");
        return 0;
    }
    
    GlobalVarEditorData* data = reinterpret_cast<GlobalVarEditorData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    
    if (message == WM_DESTROY) {
        OutputDebugStringW(L"[GlobalVarEditorWndProc] WM_DESTROY\n");
        if (data) {
            delete data;
        }
        SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
        return 0;
    }
    
    // 将消息转发给 TableEditor::WndProc
    if (data && data->editor) {
        return TableEditor::WndProcWithEditor(hWnd, message, wParam, lParam, data->editor);
    }
    
    // 如果没有 editor，显示默认背景
    if (message == WM_PAINT) {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hWnd, &ps);
        return 0;
    }
    
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// 注册 GlobalVarEditor 窗口类
bool RegisterGlobalVarEditorClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {0};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = GlobalVarEditorWndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"GlobalVarEditor";
    
    return RegisterClassExW(&wcex) != 0;
}

// 创建 GlobalVarEditor 窗口
HWND CreateGlobalVarEditorWindow(HINSTANCE hInstance, HWND hParent)
{
    return CreateWindowW(L"GlobalVarEditor", L"全局变量编辑器",
        WS_CHILD,
        0, 0, 0, 0, hParent, nullptr, hInstance, nullptr);
}
