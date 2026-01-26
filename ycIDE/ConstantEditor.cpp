#include "ConstantEditor.h"
#include "EditorContext.h"
#include "NameValidator.h"
#include "Theme.h"
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

// 常量定义
static const int CURSOR_TIMER_ID = 1;
static const int CURSOR_BLINK_INTERVAL = 500;

// COLORREF转GDI+ Color辅助函数
inline Color ColorFromCOLORREF(COLORREF colorref) {
    return Color(255, GetRValue(colorref), GetGValue(colorref), GetBValue(colorref));
}

// 构造/析构
ConstantEditor::ConstantEditor(HWND hWnd, EditorContext* context)
    : TableEditor(hWnd, context)
    , m_leftMarginWidth(60)
    , m_lineNumberAreaWidth(50)
    , m_colWidthName(120)
    , m_colWidthValue(80)
    , m_colWidthPublic(50)
    , m_colWidthComment(200)
    , m_cursorVisible(true)
    , m_isDraggingSelection(false)
    , m_dragStartRow(-1)
    , m_isDraggingInLineNumber(false) {
    
    m_currentCellRect = {0, 0, 0, 0};
    
    m_fileName = L"未命名.ecs";
    
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

ConstantEditor::~ConstantEditor() {
    // 停止光标闪烁定时器
    KillTimer(m_hWnd, CURSOR_TIMER_ID);
    
    // 销毁镜像光标
    if (m_hMirrorArrowCursor) {
        DestroyCursor(m_hMirrorArrowCursor);
        m_hMirrorArrowCursor = NULL;
    }
}

// === 重写 TableEditor 的虚接口 ===

std::vector<ColumnDef> ConstantEditor::GetColumnDefs() const {
    // 列定义：常量名、值、公开、备注（使用动态计算的列宽）
    std::vector<ColumnDef> cols;
    cols.push_back(ColumnDef(L"常量名", m_colWidthName));           // 常量名列
    cols.push_back(ColumnDef(L"值", m_colWidthValue));              // 值列
    cols.push_back(ColumnDef(L"公开", m_colWidthPublic, true, true));   // 复选框，固定宽度
    cols.push_back(ColumnDef(L"备注", m_colWidthComment));          // 备注列
    return cols;
}

int ConstantEditor::GetRowCount() const {
    return (int)m_constants.size();
}

std::wstring ConstantEditor::GetCellText(int row, int col) const {
    if (row < 0 || row >= (int)m_constants.size()) {
        return L"";
    }
    
    const Constant& constant = m_constants[row];
    
    switch (col) {
        case 0: return constant.name;
        case 1: return constant.value;
        case 2: return constant.isPublic ? L"✓" : L"";
        case 3: return constant.comment;
        default: return L"";
    }
}

void ConstantEditor::SetCellValue(int row, int col, const std::wstring& value) {
    if (row < 0 || row >= (int)m_constants.size()) {
        return;
    }
    
    Constant& constant = m_constants[row];
    
    switch (col) {
        case 0: constant.name = value; break;
        case 1: constant.value = value; break;
        case 3: constant.comment = value; break;
    }
    
    SetModified(true);
    
    // 重新计算列宽（仅当修改了名称、值或备注列时）
    if (col == 0 || col == 1 || col == 3) {
        CalculateColumnWidths();
    }
    
    // 通知数据变更
    if (m_pContext && col == 0) {
        m_pContext->NotifyDataChanged(EditorFileType::EcsConstant, value);
    }
}

bool ConstantEditor::IsCellTextEditable(int row, int col) const {
    // col == 2 是公开复选框，不可文本编辑
    return col != 2;
}

bool ConstantEditor::GetCellCheckState(int row, int col) const {
    if (row < 0 || row >= (int)m_constants.size()) {
        return false;
    }
    
    const Constant& constant = m_constants[row];
    
    if (col == 2) return constant.isPublic;  // 只有公开列是复选框
    
    return false;
}

void ConstantEditor::SetCellCheckState(int row, int col, bool checked) {
    if (row < 0 || row >= (int)m_constants.size()) {
        return;
    }
    
    Constant& constant = m_constants[row];
    
    if (col == 2) {  // 只有公开列是复选框
        constant.isPublic = checked;
        SetModified(true);
    }
}

std::wstring ConstantEditor::ValidateCell(int row, int col, const std::wstring& value) const {
    // 验证常量名
    if (col == 0) {
        if (value.empty()) {
            return L"常量名不能为空";
        }
        
        if (m_pContext && m_pContext->GetNameValidator()) {
            return m_pContext->GetNameValidator()->ValidateName(
                value, SymbolType::Constant, m_filePath);
        }
    }
    
    return L"";
}

std::vector<std::wstring> ConstantEditor::SerializeState() const {
    std::vector<std::wstring> state;
    
    state.push_back(std::to_wstring(m_constants.size()));
    
    for (const auto& constant : m_constants) {
        state.push_back(constant.name);
        state.push_back(constant.value);
        state.push_back(constant.isPublic ? L"1" : L"0");
        state.push_back(constant.comment);
    }
    
    return state;
}

void ConstantEditor::RestoreState(const std::vector<std::wstring>& state) {
    if (state.empty()) return;
    
    size_t index = 0;
    m_constants.clear();
    
    if (index < state.size()) {
        int count = std::stoi(state[index++]);
        
        for (int i = 0; i < count && index + 3 < state.size(); i++) {
            Constant constant;
            constant.name = state[index++];
            constant.value = state[index++];
            constant.isPublic = (state[index++] == L"1");
            constant.comment = state[index++];
            
            m_constants.push_back(constant);
        }
    }
}

void ConstantEditor::ParseContent(const std::vector<std::wstring>& lines) {
    m_constants.clear();
    
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
        
        // 判断是常量
        // 支持两种格式: ".常量 " 和 "常量 "
        bool isConstant = false;
        std::wstring rest;
        
        if (line.find(L".常量 ") == 0) {
            isConstant = true;
            rest = line.substr(4);  // 跳过 ".常量 "
        } else if (line.find(L"常量 ") == 0) {
            isConstant = true;
            rest = line.substr(3);  // 跳过 "常量 "
        }
        
        if (isConstant) {
            // 格式: .常量 名称, "值", 公开/私有, 备注
            Constant constant;
            constant.isPublic = true;  // 默认为公开
            
            auto parts = splitByComma(rest);
            
            if (parts.size() >= 1) constant.name = parts[0];
            if (parts.size() >= 2) {
                // 值：移除引号
                constant.value = parts[1];
                if (constant.value.length() >= 2 && constant.value[0] == L'"' && constant.value.back() == L'"') {
                    constant.value = constant.value.substr(1, constant.value.length() - 2);
                }
            }
            if (parts.size() >= 3) {
                // 公开/私有
                constant.isPublic = (parts[2] == L"公开");
            }
            if (parts.size() >= 4) constant.comment = parts[3];
            
            m_constants.push_back(constant);
        }
    }
    
    // 解析完成后计算列宽
    const_cast<ConstantEditor*>(this)->CalculateColumnWidths();
}

std::vector<std::wstring> ConstantEditor::GenerateContent() const {
    std::vector<std::wstring> lines;
    
    lines.push_back(L".版本 2");
    lines.push_back(L"");
    
    for (const auto& constant : m_constants) {
        std::wstring line = L".常量 " + constant.name;
        
        // 值（带引号）
        line += L", \"" + constant.value + L"\"";
        
        // 公开/私有
        line += L", " + std::wstring(constant.isPublic ? L"公开" : L"");
        
        // 备注
        if (!constant.comment.empty()) {
            line += L", " + constant.comment;
        }
        
        lines.push_back(line);
    }
    
    lines.push_back(L"");
    
    return lines;
}

void ConstantEditor::InsertRow(int afterRow) {
    Constant newConstant;
    newConstant.value = L"";
    newConstant.isPublic = true;  // 默认公开
    newConstant.comment = L"";
    
    // 生成不重复的常量名
    std::wstring baseName = L"新常量";
    std::wstring newName = baseName;
    int suffix = 1;
    
    // 检查常量名是否已存在
    auto nameExists = [this](const std::wstring& name) -> bool {
        for (const auto& constant : m_constants) {
            if (constant.name == name) {
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
    
    newConstant.name = newName;
    
    if (afterRow < 0 || m_constants.empty()) {
        m_constants.push_back(newConstant);
    } else if (afterRow >= (int)m_constants.size()) {
        m_constants.push_back(newConstant);
    } else {
        m_constants.insert(m_constants.begin() + afterRow + 1, newConstant);
    }
    
    SetModified(true);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void ConstantEditor::DeleteRow(int row) {
    if (row >= 0 && row < (int)m_constants.size()) {
        m_constants.erase(m_constants.begin() + row);
        SetModified(true);
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

// === 文件操作重写（支持JSON） ===

bool ConstantEditor::LoadFile(const std::wstring& path) {
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

bool ConstantEditor::SaveFile(const std::wstring& path) {
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

json ConstantEditor::ToJson() const {
    json j;
    j["version"] = 1;
    j["constants"] = json::array();
    
    for (const auto& constant : m_constants) {
        json item;
        item["name"] = WStringToUtf8String(constant.name);
        item["value"] = WStringToUtf8String(constant.value);
        item["public"] = constant.isPublic;
        item["comment"] = WStringToUtf8String(constant.comment);
        
        j["constants"].push_back(item);
    }
    
    return j;
}

void ConstantEditor::FromJson(const json& j) {
    m_constants.clear();
    
    if (j.contains("constants") && j["constants"].is_array()) {
        for (const auto& item : j["constants"]) {
            Constant constant;
            
            if (item.contains("name")) constant.name = Utf8StringToWString(item["name"]);
            if (item.contains("value")) constant.value = Utf8StringToWString(item["value"]);
            if (item.contains("public")) constant.isPublic = item["public"];
            if (item.contains("comment")) constant.comment = Utf8StringToWString(item["comment"]);
            
            m_constants.push_back(constant);
        }
    }
    
    // 加载完成后计算列宽
    CalculateColumnWidths();
}

std::wstring ConstantEditor::WStringToUtf8String(const std::wstring& wstr) const {
    if (wstr.empty()) return L"";
    
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (len <= 0) return L"";
    
    std::string utf8Str(len - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8Str[0], len, NULL, NULL);
    
    return std::wstring(utf8Str.begin(), utf8Str.end());
}

std::wstring ConstantEditor::Utf8StringToWString(const std::string& str) const {
    if (str.empty()) return L"";
    
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    if (len <= 0) return L"";
    
    std::wstring wstr(len - 1, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], len);
    
    return wstr;
}

// === Protected 方法 ===

std::wstring* ConstantEditor::GetEditingCellTextPtr() {
    if (!m_isEditing || m_editingRow < 0) {
        return nullptr;
    }
    
    // 直接返回已有的编辑缓冲区，不重新加载
    // 初始化在StartEditCell中完成
    return &m_editBuffer;
}

// === 输入处理 ===

void ConstantEditor::HitTest(int x, int y, int& row, int& col, RECT& cellRect) {
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
    if (row < 0 || row >= (int)m_constants.size()) {
        row = -1;
        return;
    }
    
    // 计算列号
    int xWithScroll = adjustedX + m_scrollX;
    int xPos = 10;  // 加上10像素左边距，与DrawConstantTable保持一致
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

void ConstantEditor::OnLButtonDown(int x, int y) {
    // 检查是否在行号区域点击
    if (x >= 0 && x < m_leftMarginWidth) {
        int row = GetRowAtY(y);
        if (row >= 0 && row < (int)m_constants.size()) {
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
        if (col == 2) {  // 公开复选框（第3列，索引为2）
            if (row >= 0 && row < (int)m_constants.size()) {
                m_constants[row].isPublic = !m_constants[row].isPublic;
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
            if (yWithScroll >= m_rowHeight && !m_constants.empty()) {
                // 计算点击位置的逻辑行号
                int logicalRow = (yWithScroll - m_rowHeight) / m_rowHeight;
                // 如果超出实际行数，使用最后一行
                if (logicalRow >= (int)m_constants.size()) {
                    clickRow = (int)m_constants.size() - 1;
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
    
    // 保存当前单元格矩形
    if (m_isEditing) {
        m_currentCellRect = cellRect;
    }
}

void ConstantEditor::OnMouseWheel(int delta) {
    TableEditor::OnMouseWheel(delta);
}

void ConstantEditor::OnKeyDown(WPARAM wParam) {
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
            currentRow = (int)m_constants.size() - 1;
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
                if (row >= 0 && row < (int)m_constants.size()) {
                    m_constants.erase(m_constants.begin() + row);
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

void ConstantEditor::OnChar(WPARAM wParam) {
    TableEditor::OnChar(wParam);
}

bool ConstantEditor::OnSetCursor(int x, int y) {
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

void ConstantEditor::OnMouseMove(int x, int y, WPARAM wParam) {
    // 如果正在拖动选择
    if (m_isDraggingSelection) {
        int currentRow = GetRowAtY(y);
        
        // 如果鼠标在空白区域，使用最后一行
        if (currentRow < 0 && !m_constants.empty()) {
            int yWithScroll = y + m_scrollY;
            if (yWithScroll >= m_rowHeight) {
                int logicalRow = (yWithScroll - m_rowHeight) / m_rowHeight;
                if (logicalRow >= (int)m_constants.size()) {
                    currentRow = (int)m_constants.size() - 1;
                }
            }
        }
        
        if (currentRow >= 0 && currentRow < (int)m_constants.size()) {
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

void ConstantEditor::OnLButtonUp(int x, int y) {
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

bool ConstantEditor::IsRowSelected(int row) const {
    return std::find(m_selectedRows.begin(), m_selectedRows.end(), row) != m_selectedRows.end();
}

void ConstantEditor::ClearSelection() {
    m_selectedRows.clear();
}

void ConstantEditor::SetRowSelection(int row, bool selected) {
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

void ConstantEditor::SelectRowRange(int startRow, int endRow) {
    ClearSelection();
    
    int minRow = (std::min)(startRow, endRow);
    int maxRow = (std::max)(startRow, endRow);
    
    for (int row = minRow; row <= maxRow && row < (int)m_constants.size(); row++) {
        m_selectedRows.push_back(row);
    }
}

int ConstantEditor::GetRowAtY(int y) const {
    int yWithScroll = y + m_scrollY;
    
    // 减去表头高度
    if (yWithScroll < m_rowHeight) {
        return -1;
    }
    
    int row = (yWithScroll - m_rowHeight) / m_rowHeight;
    if (row < 0 || row >= (int)m_constants.size()) {
        return -1;
    }
    
    return row;
}

// === 列宽计算 ===

void ConstantEditor::CalculateColumnWidths() {
    // 使用GDI+测量文本宽度
    HDC hdc = GetDC(m_hWnd);
    if (!hdc) return;
    
    Graphics graphics(hdc);
    FontFamily fontFamily(L"Microsoft YaHei");
    Font font(&fontFamily, (REAL)m_fontSize, FontStyleRegular, UnitPixel);
    
    StringFormat format;
    format.SetAlignment(StringAlignmentNear);
    
    // 最小列宽
    int minNameWidth = 80;
    int minValueWidth = 60;
    int minCommentWidth = 80;
    
    // 最大列宽限制
    int maxNameWidth = 300;
    int maxValueWidth = 200;
    int maxCommentWidth = 500;
    
    // 计算常量名列宽度
    int maxNameW = minNameWidth;
    for (const auto& constant : m_constants) {
        RectF boundingBox;
        graphics.MeasureString(constant.name.c_str(), -1, &font, PointF(0, 0), &format, &boundingBox);
        int w = (int)boundingBox.Width + 20;  // 加上边距
        if (w > maxNameW) maxNameW = w;
    }
    m_colWidthName = (std::min)(maxNameW, maxNameWidth);
    
    // 计算值列宽度
    int maxValueW = minValueWidth;
    for (const auto& constant : m_constants) {
        RectF boundingBox;
        graphics.MeasureString(constant.value.c_str(), -1, &font, PointF(0, 0), &format, &boundingBox);
        int w = (int)boundingBox.Width + 20;  // 加上边距
        if (w > maxValueW) maxValueW = w;
    }
    m_colWidthValue = (std::min)(maxValueW, maxValueWidth);
    
    // 计算备注列宽度
    int maxCommentW = minCommentWidth;
    for (const auto& constant : m_constants) {
        RectF boundingBox;
        graphics.MeasureString(constant.comment.c_str(), -1, &font, PointF(0, 0), &format, &boundingBox);
        int w = (int)boundingBox.Width + 20;  // 加上边距
        if (w > maxCommentW) maxCommentW = w;
    }
    m_colWidthComment = (std::min)(maxCommentW, maxCommentWidth);
    
    // 公开列固定宽度
    m_colWidthPublic = 50;
    
    ReleaseDC(m_hWnd, hdc);
}

void ConstantEditor::StartEditCell(int row, int col, int clickX) {
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

int ConstantEditor::GetTotalContentHeight() const {
    // 1个表头 + N个数据行
    int totalRows = 1 + (int)m_constants.size();
    return totalRows * m_rowHeight;
}

int ConstantEditor::GetTotalContentWidth() const {
    int width = m_leftMarginWidth;  // 加上左边距宽度
    auto cols = GetColumnDefs();
    for (const auto& col : cols) {
        width += col.width;
    }
    return width;
}

// === 绘制 ===

void ConstantEditor::DrawTable(HDC hdc, const RECT& clientRect) {
    Graphics graphics(hdc);
    
    // 绘制整个编辑器背景
    SolidBrush bgBrush(ColorFromCOLORREF(g_CurrentTheme.editorBg));
    graphics.FillRectangle(&bgBrush, 0, 0, clientRect.right, clientRect.bottom);
    
    // 绘制左边距背景（行号区域）
    SolidBrush lineNumBgBrush(ColorFromCOLORREF(g_CurrentTheme.lineNumBg));
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
    int totalRows = 1 + (int)m_constants.size(); // 1个表头 + N个数据行
    
    // 绘制所有行号（只绘制数据行，跳过表头）
    if (!m_constants.empty()) {
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
    DrawConstantTable(graphics, yPos, clientRect);
}

void ConstantEditor::DrawConstantTable(Graphics& graphics, int yPos, const RECT& clientRect) {
    
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
    SolidBrush headerBrush(ColorFromCOLORREF(g_CurrentTheme.tableHeaderBg));  // 表头背景色
    SolidBrush valueBrush(ColorFromCOLORREF(g_CurrentTheme.syntaxString));    // 值列文本色（使用字符串颜色）
    SolidBrush commentBrush(ColorFromCOLORREF(g_CurrentTheme.syntaxRemark));  // 备注列文本色
    
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
                     m_leftMarginWidth + 10, yPos + (1 + (int)m_constants.size()) * m_rowHeight);
    
    yPos += m_rowHeight;
    
    // 绘制数据行
    for (int row = 0; row < (int)m_constants.size(); row++) {
        int rowY = yPos + row * m_rowHeight;
        
        // 只绘制可见区域的行
        if (rowY + m_rowHeight < 0 || rowY >= clientRect.bottom) {
            continue;
        }
        
        const Constant& constant = m_constants[row];
        
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
                if (constant.isPublic) {
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
                    if (col == 1) {  // 值列
                        editTextBrush = &valueBrush;
                    } else if (col == 3) {  // 备注列
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
                    if (col == 1) {  // 值列
                        cellTextBrush = &valueBrush;
                    } else if (col == 3) {  // 备注列
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
                         yPos + (int)m_constants.size() * m_rowHeight);
    }
}

// === 窗口过程和创建函数 ===

// ConstantEditorData 成员函数实现
ConstantEditorData::ConstantEditorData(HWND hWnd) : editor(nullptr) {
    editor = new ConstantEditor(hWnd, nullptr);
}

ConstantEditorData::~ConstantEditorData() {
    if (editor) {
        delete editor;
        editor = nullptr;
    }
}

bool ConstantEditorData::LoadFile(const std::wstring& path) {
    if (editor) {
        return editor->LoadFile(path);
    }
    return false;
}

bool ConstantEditorData::SaveFile(const std::wstring& path) {
    if (editor) {
        return editor->SaveFile(path);
    }
    return false;
}

bool ConstantEditorData::IsModified() const {
    if (editor) {
        return editor->IsModified();
    }
    return false;
}

void ConstantEditorData::SetModified(bool modified) {
    if (editor) {
        editor->SetModified(modified);
    }
}

// ConstantEditor 窗口过程
LRESULT CALLBACK ConstantEditorWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_CREATE) {
        OutputDebugStringW(L"[ConstantEditorWndProc] WM_CREATE - 创建窗口\n");
        ConstantEditorData* data = new ConstantEditorData(hWnd);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));
        OutputDebugStringW(L"[ConstantEditorWndProc] WM_CREATE - 完成\n");
        return 0;
    }
    
    ConstantEditorData* data = reinterpret_cast<ConstantEditorData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    
    if (message == WM_DESTROY) {
        OutputDebugStringW(L"[ConstantEditorWndProc] WM_DESTROY\n");
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

// 注册 ConstantEditor 窗口类
bool RegisterConstantEditorClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {0};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = ConstantEditorWndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"ConstantEditor";
    
    return RegisterClassExW(&wcex) != 0;
}

// 创建 ConstantEditor 窗口
HWND CreateConstantEditorWindow(HINSTANCE hInstance, HWND hParent)
{
    return CreateWindowW(L"ConstantEditor", L"常量编辑器",
        WS_CHILD,
        0, 0, 0, 0, hParent, nullptr, hInstance, nullptr);
}
