#define NOMINMAX
#include "TableEditor.h"
#include "DllEditor.h"
#include "EditorContext.h"
#include "Theme.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <windowsx.h>
#include <gdiplus.h>
#include <imm.h>

#pragma comment(lib, "imm32.lib")

using namespace Gdiplus;

extern AppTheme g_CurrentTheme;

// COLORREF转GDI+ Color辅助函数
inline Color ColorFromCOLORREF(COLORREF colorref) {
    return Color(255, GetRValue(colorref), GetGValue(colorref), GetBValue(colorref));
}

// === TableEditor 构造/析构 ===

TableEditor::TableEditor(HWND hWnd, EditorContext* context)
    : m_hWnd(hWnd)
    , m_pContext(context)
    , m_modified(false)
    , m_hFileLock(INVALID_HANDLE_VALUE)
    , m_fontSize(14)
    , m_rowHeight(28)
    , m_headerHeight(32)
    , m_scrollY(0)
    , m_scrollX(0)
    , m_scrollbarWidth(12)
    , m_vScrollHover(false)
    , m_hScrollHover(false)
    , m_isDraggingVScroll(false)
    , m_isDraggingHScroll(false)
    , m_dragStartY(0)
    , m_dragStartX(0)
    , m_dragStartScrollY(0)
    , m_dragStartScrollX(0)
    , m_isEditing(false)
    , m_editingRow(-1)
    , m_editingCol(-1)
    , m_editingCursorPos(0)
    , m_isSelecting(false)
    , m_selectionStart(0)
    , m_selectionEnd(0)
    , m_showWelcomePage(true) {
    
    m_fileName = L"未命名";
    
    ZeroMemory(&m_vScrollbarRect, sizeof(RECT));
    ZeroMemory(&m_vScrollThumbRect, sizeof(RECT));
    ZeroMemory(&m_hScrollbarRect, sizeof(RECT));
    ZeroMemory(&m_hScrollThumbRect, sizeof(RECT));
}

TableEditor::~TableEditor() {
    if (m_hFileLock != INVALID_HANDLE_VALUE) {
        CloseHandle(m_hFileLock);
        m_hFileLock = INVALID_HANDLE_VALUE;
    }
}

// === 窗口消息处理 ===

LRESULT CALLBACK TableEditor::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    TableEditor* pEditor = reinterpret_cast<TableEditor*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    
    // 调试: 检查 WM_PAINT 时的 pEditor 指针
    if (message == WM_PAINT) {
        wchar_t debugMsg[256];
        swprintf_s(debugMsg, L"[TableEditor::WndProc] WM_PAINT - pEditor: %p\n", pEditor);
        OutputDebugStringW(debugMsg);
    }
    
    switch (message) {
        case WM_CREATE: {
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreate->lpCreateParams));
            return 0;
        }
        
        case WM_PAINT:
            if (pEditor) {
                OutputDebugStringW(L"[TableEditor::WndProc] 调用 OnPaint\n");
                pEditor->OnPaint();
            } else {
                OutputDebugStringW(L"[TableEditor::WndProc] pEditor 为空，跳过 OnPaint\n");
            }
            return 0;
        
        case WM_MOUSEMOVE:
            if (pEditor) pEditor->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam);
            return 0;
        
        case WM_LBUTTONDOWN:
            if (pEditor) pEditor->OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        
        case WM_LBUTTONUP:
            if (pEditor) pEditor->OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        
        case WM_MOUSEWHEEL:
            if (pEditor) pEditor->OnMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
            return 0;
        
        case WM_KEYDOWN:
            if (pEditor) pEditor->OnKeyDown(wParam);
            return 0;
        
        case WM_CHAR:
            if (pEditor) pEditor->OnChar(wParam);
            return 0;
        
        case WM_IME_STARTCOMPOSITION:
            if (pEditor && pEditor->m_isEditing) {
                // 获取精确的光标屏幕位置
                int caretX = 0, caretY = 0;
                if (pEditor->GetEditingCursorScreenPos(caretX, caretY)) {
                    // 设置输入法候选窗口位置
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
            }
            return DefWindowProc(hWnd, message, wParam, lParam);
        
        case WM_IME_COMPOSITION:
            if (pEditor && pEditor->m_isEditing && (lParam & GCS_RESULTSTR)) {
                // 获取输入法上下文
                HIMC hIMC = ImmGetContext(hWnd);
                if (hIMC) {
                    // 获取结果字符串长度
                    LONG len = ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, NULL, 0);
                    if (len > 0) {
                        // 获取结果字符串
                        std::vector<wchar_t> buffer(len / sizeof(wchar_t) + 1);
                        ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, buffer.data(), len);
                        buffer[len / sizeof(wchar_t)] = L'\0';
                        
                        // 将字符串插入到编辑位置
                        std::wstring resultStr(buffer.data());
                        for (wchar_t ch : resultStr) {
                            pEditor->OnChar(ch);
                        }
                    }
                    
                    // 释放输入法上下文
                    ImmReleaseContext(hWnd, hIMC);
                    return 0;
                }
            }
            return DefWindowProc(hWnd, message, wParam, lParam);
        
        case WM_TIMER:
            if (pEditor && pEditor->m_isEditing) {
                // 处理光标闪烁（由子类DllEditor触发）
                DllEditor* dllEditor = dynamic_cast<DllEditor*>(pEditor);
                if (dllEditor && wParam == 1) { // CURSOR_TIMER_ID = 1
                    dllEditor->m_cursorVisible = !dllEditor->m_cursorVisible;
                    InvalidateRect(hWnd, NULL, FALSE);
                }
            }
            return 0;
        
        case WM_SIZE:
            if (pEditor) InvalidateRect(hWnd, NULL, FALSE);
            return 0;
        
        case WM_ERASEBKGND:
            return 1; // 防止闪烁
        
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

// === 文件操作 ===

bool TableEditor::LoadFile(const std::wstring& path) {
    // 使用Windows API读取UTF-8文件
    HANDLE hFile = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, 
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }
    
    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        CloseHandle(hFile);
        return false;
    }
    
    // 读取文件内容（UTF-8）
    std::vector<char> buffer(fileSize + 1, 0);
    DWORD bytesRead = 0;
    if (!ReadFile(hFile, buffer.data(), fileSize, &bytesRead, NULL)) {
        CloseHandle(hFile);
        return false;
    }
    CloseHandle(hFile);
    
    // 转换UTF-8到宽字符
    int wideSize = MultiByteToWideChar(CP_UTF8, 0, buffer.data(), bytesRead, NULL, 0);
    if (wideSize == 0) {
        return false;
    }
    
    std::wstring content(wideSize, 0);
    MultiByteToWideChar(CP_UTF8, 0, buffer.data(), bytesRead, &content[0], wideSize);
    
    // 按行分割
    std::vector<std::wstring> lines;
    std::wstring line;
    for (wchar_t ch : content) {
        if (ch == L'\n') {
            // 移除行尾的\r
            if (!line.empty() && line.back() == L'\r') {
                line.pop_back();
            }
            lines.push_back(line);
            line.clear();
        } else {
            line += ch;
        }
    }
    // 添加最后一行
    if (!line.empty() || (content.empty() || content.back() == L'\n')) {
        if (!line.empty() && line.back() == L'\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }
    
    // 解析内容
    ParseContent(lines);
    
    m_filePath = path;
    size_t pos = path.find_last_of(L"\\/");
    m_fileName = (pos != std::wstring::npos) ? path.substr(pos + 1) : path;
    m_modified = false;
    m_showWelcomePage = false;
    
    // 清空撤销/重做栈
    m_undoStack.clear();
    m_redoStack.clear();
    
    InvalidateRect(m_hWnd, NULL, FALSE);
    return true;
}

bool TableEditor::SaveFile(const std::wstring& path) {
    // 在保存前先结束当前的单元格编辑
    if (m_isEditing) {
        EndEditCell(true);
    }
    
    // 生成内容
    std::vector<std::wstring> lines = GenerateContent();
    
    // 将内容合并为一个字符串
    std::wstring content;
    for (size_t i = 0; i < lines.size(); i++) {
        content += lines[i];
        if (i < lines.size() - 1) {
            content += L"\r\n";
        }
    }
    
    // 转换宽字符到UTF-8
    int utf8Size = WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, NULL, 0, NULL, NULL);
    if (utf8Size == 0) {
        return false;
    }
    
    std::vector<char> utf8Buffer(utf8Size);
    WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, utf8Buffer.data(), utf8Size, NULL, NULL);
    
    // 使用Windows API写入UTF-8文件
    HANDLE hFile = CreateFileW(path.c_str(), GENERIC_WRITE, 0, NULL,
                               CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }
    
    DWORD bytesWritten = 0;
    // 写入时不包括字符串结尾的null字符
    BOOL writeResult = WriteFile(hFile, utf8Buffer.data(), utf8Size - 1, &bytesWritten, NULL);
    CloseHandle(hFile);
    
    if (!writeResult) {
        return false;
    }
    
    m_filePath = path;
    size_t pos = path.find_last_of(L"\\/");
    m_fileName = (pos != std::wstring::npos) ? path.substr(pos + 1) : path;
    m_modified = false;
    
    InvalidateRect(m_hWnd, NULL, FALSE);
    return true;
}

bool TableEditor::SaveFile() {
    if (m_filePath.empty()) {
        return false;
    }
    return SaveFile(m_filePath);
}

// === 撤销/重做系统 ===

void TableEditor::CreateSnapshot(const std::wstring& description) {
    TableSnapshot snapshot;
    snapshot.id = GenerateSnapshotId();
    snapshot.description = description;
    snapshot.timestamp = std::time(nullptr);
    snapshot.data = SerializeState();
    
    // 添加到撤销栈
    m_undoStack.push_back(snapshot);
    
    // 限制栈大小
    if (m_undoStack.size() > MAX_UNDO_STACK_SIZE) {
        m_undoStack.erase(m_undoStack.begin());
    }
    
    // 清空重做栈
    m_redoStack.clear();
    
    m_modified = true;
}

void TableEditor::Undo() {
    if (m_undoStack.empty()) {
        return;
    }
    
    // 保存当前状态到重做栈
    TableSnapshot currentSnapshot;
    currentSnapshot.id = GenerateSnapshotId();
    currentSnapshot.description = L"Current";
    currentSnapshot.timestamp = std::time(nullptr);
    currentSnapshot.data = SerializeState();
    m_redoStack.push_back(currentSnapshot);
    
    // 从撤销栈恢复
    TableSnapshot snapshot = m_undoStack.back();
    m_undoStack.pop_back();
    RestoreState(snapshot.data);
    
    m_modified = true;
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void TableEditor::Redo() {
    if (m_redoStack.empty()) {
        return;
    }
    
    // 保存当前状态到撤销栈
    TableSnapshot currentSnapshot;
    currentSnapshot.id = GenerateSnapshotId();
    currentSnapshot.description = L"Current";
    currentSnapshot.timestamp = std::time(nullptr);
    currentSnapshot.data = SerializeState();
    m_undoStack.push_back(currentSnapshot);
    
    // 从重做栈恢复
    TableSnapshot snapshot = m_redoStack.back();
    m_redoStack.pop_back();
    RestoreState(snapshot.data);
    
    m_modified = true;
    InvalidateRect(m_hWnd, NULL, FALSE);
}

std::wstring TableEditor::GenerateSnapshotId() {
    static int counter = 0;
    wchar_t buffer[64];
    swprintf_s(buffer, L"%lld_%d", std::time(nullptr), counter++);
    return buffer;
}

// === 绘制相关 ===

void TableEditor::OnPaint() {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hWnd, &ps);
    
    RECT clientRect;
    GetClientRect(m_hWnd, &clientRect);
    
    wchar_t debugMsg[512];
    swprintf_s(debugMsg, L"[TableEditor::OnPaint] 开始绘制 - m_showWelcomePage: %d, 主题背景色: 0x%08X, 窗口大小: %d x %d\n", 
               m_showWelcomePage, g_CurrentTheme.bg, clientRect.right, clientRect.bottom);
    OutputDebugStringW(debugMsg);
    
    // 双缓冲
    HDC hMemDC = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
    HGDIOBJ hOldBitmap = SelectObject(hMemDC, hBitmap);
    
    // 使用 GDI+
    Graphics graphics(hMemDC);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
    
    // 清空背景
    SolidBrush bgBrush(ColorFromCOLORREF(g_CurrentTheme.bg));
    graphics.FillRectangle(&bgBrush, (INT)0, (INT)0, (INT)clientRect.right, (INT)clientRect.bottom);
    
    if (m_showWelcomePage) {
        OutputDebugStringW(L"[TableEditor::OnPaint] 绘制欢迎页\n");
        // 绘制欢迎页
        SolidBrush textBrush(ColorFromCOLORREF(g_CurrentTheme.text));
        FontFamily fontFamily(L"Microsoft YaHei");
        Font font(&fontFamily, 16, FontStyleRegular, UnitPixel);
        StringFormat format;
        format.SetAlignment(StringAlignmentCenter);
        format.SetLineAlignment(StringAlignmentCenter);
        
        RectF rect(0, 0, (REAL)clientRect.right, (REAL)clientRect.bottom);
        graphics.DrawString(L"打开文件以开始编辑", -1, &font, rect, &format, &textBrush);
    } else {
        OutputDebugStringW(L"[TableEditor::OnPaint] 绘制表格\n");
        // 绘制表格 - 子类可以重写DrawTable来自定义绘制
        DrawTable(hMemDC, clientRect);
        DrawScrollbars(hMemDC, clientRect);
    }
    
    // 复制到屏幕
    BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hMemDC, 0, 0, SRCCOPY);
    
    SelectObject(hMemDC, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hMemDC);
    
    EndPaint(m_hWnd, &ps);
}

void TableEditor::DrawTable(HDC hdc, const RECT& clientRect) {
    std::vector<ColumnDef> cols = GetColumnDefs();
    
    wchar_t debugMsg[256];
    swprintf_s(debugMsg, L"[TableEditor::DrawTable] 列数: %zu\n", cols.size());
    OutputDebugStringW(debugMsg);
    
    if (cols.empty()) {
        OutputDebugStringW(L"[TableEditor::DrawTable] 列定义为空，返回\n");
        return;
    }
    
    int rowCount = GetRowCount();
    swprintf_s(debugMsg, L"[TableEditor::DrawTable] 行数: %d\n", rowCount);
    OutputDebugStringW(debugMsg);
    
    // 只有在有表头高度时才绘制表头
    if (m_headerHeight > 0) {
        RECT headerRect = { 0, 0, clientRect.right - m_scrollbarWidth, m_headerHeight };
        DrawHeader(hdc, headerRect, cols);
    }
    
    // 绘制数据行
    int y = m_headerHeight - m_scrollY;
    for (int row = 0; row < rowCount; row++) {
        if (y + m_rowHeight < 0) {
            y += m_rowHeight;
            continue;
        }
        if (y > clientRect.bottom) {
            break;
        }
        
        RECT rowRect = { 0, y, clientRect.right - m_scrollbarWidth, y + m_rowHeight };
        DrawRow(hdc, rowRect, row, cols);
        
        y += m_rowHeight;
    }
    
    OutputDebugStringW(L"[TableEditor::DrawTable] 绘制完成\n");
}

void TableEditor::DrawHeader(HDC hdc, const RECT& rect, const std::vector<ColumnDef>& cols) {
    Graphics graphics(hdc);
    
    // 表头背景
    SolidBrush headerBrush(ColorFromCOLORREF(g_CurrentTheme.margin));
    graphics.FillRectangle(&headerBrush, (INT)rect.left, (INT)rect.top, (INT)(rect.right - rect.left), (INT)(rect.bottom - rect.top));
    
    // 绘制列
    int x = -m_scrollX;
    Pen gridPen(ColorFromCOLORREF(g_CurrentTheme.grid), 1);
    SolidBrush textBrush(ColorFromCOLORREF(g_CurrentTheme.text));
    FontFamily fontFamily(L"Microsoft YaHei");
    Font font(&fontFamily, (REAL)m_fontSize, FontStyleBold, UnitPixel);
    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentCenter);
    
    for (size_t i = 0; i < cols.size(); i++) {
        if (x + cols[i].width < 0) {
            x += cols[i].width;
            continue;
        }
        if (x > rect.right) {
            break;
        }
        
        RectF cellRect((REAL)x, (REAL)rect.top, (REAL)cols[i].width, (REAL)(rect.bottom - rect.top));
        graphics.DrawString(cols[i].name.c_str(), -1, &font, cellRect, &format, &textBrush);
        
        // 右边框
        graphics.DrawLine(&gridPen, (INT)(x + cols[i].width), (INT)rect.top, (INT)(x + cols[i].width), (INT)rect.bottom);
        
        x += cols[i].width;
    }
    
    // 底边框
    graphics.DrawLine(&gridPen, (INT)rect.left, (INT)(rect.bottom - 1), (INT)rect.right, (INT)(rect.bottom - 1));
}

void TableEditor::DrawRow(HDC hdc, const RECT& rect, int row, const std::vector<ColumnDef>& cols) {
    Graphics graphics(hdc);
    
    // 行背景（隔行变色）
    Color bgColor = ColorFromCOLORREF(g_CurrentTheme.bg);  // 所有行使用相同背景色
    SolidBrush bgBrush(bgColor);
    graphics.FillRectangle(&bgBrush, (INT)rect.left, (INT)rect.top, (INT)(rect.right - rect.left), (INT)(rect.bottom - rect.top));
    
    // 绘制单元格
    int x = -m_scrollX;
    Pen gridPen(ColorFromCOLORREF(g_CurrentTheme.grid), 1);
    
    for (size_t col = 0; col < cols.size(); col++) {
        if (x + cols[col].width < 0) {
            x += cols[col].width;
            continue;
        }
        if (x > rect.right) {
            break;
        }
        
        RECT cellRect = { x, rect.top, x + cols[col].width, rect.bottom };
        DrawCell(hdc, cellRect, row, (int)col, cols[col]);
        
        // 右边框
        graphics.DrawLine(&gridPen, (INT)(x + cols[col].width), (INT)rect.top, (INT)(x + cols[col].width), (INT)rect.bottom);
        
        x += cols[col].width;
    }
    
    // 底边框
    graphics.DrawLine(&gridPen, (INT)rect.left, (INT)(rect.bottom - 1), (INT)rect.right, (INT)(rect.bottom - 1));
}

void TableEditor::DrawCell(HDC hdc, const RECT& cellRect, int row, int col, const ColumnDef& colDef) {
    Graphics graphics(hdc);
    
    int colPadding = 5;
    
    if (colDef.isCheckbox) {
        // 绘制复选框
        bool checked = GetCellCheckState(row, col);
        SolidBrush textBrush(ColorFromCOLORREF(g_CurrentTheme.text));
        FontFamily fontFamily(L"Microsoft YaHei");
        Font font(&fontFamily, (REAL)m_fontSize, FontStyleRegular, UnitPixel);
        StringFormat format;
        format.SetAlignment(StringAlignmentCenter);
        format.SetLineAlignment(StringAlignmentCenter);
        
        RectF rect((REAL)cellRect.left, (REAL)cellRect.top, (REAL)(cellRect.right - cellRect.left), (REAL)(cellRect.bottom - cellRect.top));
        graphics.DrawString(checked ? L"√" : L"", -1, &font, rect, &format, &textBrush);
    } else {
        // 绘制文本
        std::wstring text = GetCellText(row, col);
        
        if (m_isEditing && m_editingRow == row && m_editingCol == col) {
            // 正在编辑的单元格
            DrawCellTextWithSelection(hdc, text, cellRect, colPadding);
        } else {
            // 普通单元格
            SolidBrush textBrush(ColorFromCOLORREF(g_CurrentTheme.text));
            FontFamily fontFamily(L"Microsoft YaHei");
            Font font(&fontFamily, (REAL)m_fontSize, FontStyleRegular, UnitPixel);
            StringFormat format;
            format.SetLineAlignment(StringAlignmentCenter);
            
            RectF rect((REAL)(cellRect.left + colPadding), (REAL)cellRect.top, 
                      (REAL)(cellRect.right - cellRect.left - colPadding * 2), (REAL)(cellRect.bottom - cellRect.top));
            graphics.DrawString(text.c_str(), -1, &font, rect, &format, &textBrush);
        }
    }
}

void TableEditor::DrawCellTextWithSelection(HDC hdc, const std::wstring& text, const RECT& cellRect, int colPadding) {
    Graphics graphics(hdc);
    
    FontFamily fontFamily(L"Microsoft YaHei");
    Font font(&fontFamily, (REAL)m_fontSize, FontStyleRegular, UnitPixel);
    SolidBrush textBrush(ColorFromCOLORREF(g_CurrentTheme.text));
    SolidBrush selBrush(Color(255, 100, 100, 200)); // 选区背景色(ARGB)
    
    // 使用精确测量的StringFormat（与光标位置计算一致）
    StringFormat typographicFormat(StringFormat::GenericTypographic());
    typographicFormat.SetFormatFlags(StringFormatFlagsMeasureTrailingSpaces);
    
    // 用于垂直居中对齐的格式
    StringFormat centerFormat;
    centerFormat.SetLineAlignment(StringAlignmentCenter);
    
    int x = cellRect.left + colPadding;
    int cellHeight = cellRect.bottom - cellRect.top;
    
    // 绘制选区背景
    if (m_selectionStart != m_selectionEnd) {
        int selStart = (std::min)(m_selectionStart, m_selectionEnd);
        int selEnd = (std::max)(m_selectionStart, m_selectionEnd);
        
        std::wstring beforeSel = text.substr(0, selStart);
        std::wstring selection = text.substr(selStart, selEnd - selStart);
        
        RectF measureRect;
        graphics.MeasureString(beforeSel.c_str(), -1, &font, PointF(0, 0), &typographicFormat, &measureRect);
        int selX = x + (int)measureRect.Width;
        
        graphics.MeasureString(selection.c_str(), -1, &font, PointF(0, 0), &typographicFormat, &measureRect);
        int selWidth = (int)measureRect.Width;
        
        graphics.FillRectangle(&selBrush, selX, cellRect.top, selWidth, cellHeight);
    }
    
    // 绘制文本（使用垂直居中对齐）
    RectF textRect((REAL)x, (REAL)cellRect.top, 1000.0f, (REAL)cellHeight);
    graphics.DrawString(text.c_str(), -1, &font, textRect, &centerFormat, &textBrush);
    
    // 绘制光标（使用精确测量）
    if (m_editingCursorPos <= (int)text.length()) {
        std::wstring beforeCursor = text.substr(0, m_editingCursorPos);
        RectF measureRect;
        graphics.MeasureString(beforeCursor.c_str(), -1, &font, PointF(0, 0), &typographicFormat, &measureRect);
        int cursorX = x + (int)measureRect.Width;
        
        Pen cursorPen(ColorFromCOLORREF(g_CurrentTheme.text), 2);
        graphics.DrawLine(&cursorPen, cursorX, cellRect.top, cursorX, cellRect.bottom);
    }
}

void TableEditor::DrawScrollbars(HDC hdc, const RECT& clientRect) {
    UpdateScrollbars(clientRect);
    
    Graphics graphics(hdc);
    
    // 垂直滚动条
    if (m_vScrollbarRect.right > m_vScrollbarRect.left) {
        Color scrollbarThumb = m_vScrollHover ? Color(255, 90, 90, 90) : Color(255, 70, 70, 70);
        
        SolidBrush thumbBrush(scrollbarThumb);
        
        // 不绘制滚动条背景，只绘制滚动块
        // graphics.FillRectangle(&bgBrush, ...);
        
        graphics.FillRectangle(&thumbBrush, (INT)m_vScrollThumbRect.left, (INT)m_vScrollThumbRect.top, (INT)(m_vScrollThumbRect.right - m_vScrollThumbRect.left), (INT)(m_vScrollThumbRect.bottom - m_vScrollThumbRect.top));
    }
    
    // 水平滚动条
    if (m_hScrollbarRect.bottom > m_hScrollbarRect.top) {
        Color scrollbarThumb = m_hScrollHover ? Color(255, 90, 90, 90) : Color(255, 70, 70, 70);
        
        SolidBrush thumbBrush(scrollbarThumb);
        
        // 不绘制滚动条背景，只绘制滚动块
        // graphics.FillRectangle(&bgBrush, ...);
        
        graphics.FillRectangle(&thumbBrush, (INT)m_hScrollThumbRect.left, (INT)m_hScrollThumbRect.top, (INT)(m_hScrollThumbRect.right - m_hScrollThumbRect.left), (INT)(m_hScrollThumbRect.bottom - m_hScrollThumbRect.top));
    }
}

// === 交互相关 ===

void TableEditor::OnMouseMove(int x, int y, WPARAM wParam) {
    if (m_isDraggingVScroll) {
        int deltaY = y - m_dragStartY;
        RECT clientRect;
        GetClientRect(m_hWnd, &clientRect);
        
        int scrollbarHeight = m_vScrollbarRect.bottom - m_vScrollbarRect.top;
        int thumbHeight = m_vScrollThumbRect.bottom - m_vScrollThumbRect.top;
        int maxThumbY = scrollbarHeight - thumbHeight;
        
        if (maxThumbY > 0) {
            int totalContentHeight = GetTotalContentHeight();
            int viewHeight = clientRect.bottom - m_headerHeight;
            int maxScrollY = (std::max)(0, totalContentHeight - viewHeight);
            
            float scrollRatio = (float)deltaY / maxThumbY;
            m_scrollY = m_dragStartScrollY + (int)(scrollRatio * maxScrollY);
            m_scrollY = (std::max)(0, (std::min)(m_scrollY, maxScrollY));
            
            InvalidateRect(m_hWnd, NULL, FALSE);
        }
        return;
    }
    
    if (m_isDraggingHScroll) {
        int deltaX = x - m_dragStartX;
        RECT clientRect;
        GetClientRect(m_hWnd, &clientRect);
        
        int scrollbarWidth = m_hScrollbarRect.right - m_hScrollbarRect.left;
        int thumbWidth = m_hScrollThumbRect.right - m_hScrollThumbRect.left;
        int maxThumbX = scrollbarWidth - thumbWidth;
        
        if (maxThumbX > 0) {
            int totalContentWidth = GetTotalContentWidth();
            
            // 横向滚动：需要考虑DllEditor的左边距
            int leftMargin = 0;
            DllEditor* dllEditor = dynamic_cast<DllEditor*>(this);
            if (dllEditor) {
                leftMargin = 75;  // DllEditor的左边距宽度
            }
            int viewWidth = clientRect.right - m_scrollbarWidth - leftMargin;
            int maxScrollX = (std::max)(0, totalContentWidth - viewWidth);
            
            float scrollRatio = (float)deltaX / maxThumbX;
            m_scrollX = m_dragStartScrollX + (int)(scrollRatio * maxScrollX);
            m_scrollX = (std::max)(0, (std::min)(m_scrollX, maxScrollX));
            
            InvalidateRect(m_hWnd, NULL, FALSE);
        }
        return;
    }
    
    // 检查是否悬停在滚动条上
    bool prevVHover = m_vScrollHover;
    bool prevHHover = m_hScrollHover;
    
    m_vScrollHover = IsPointInVScrollThumb(x, y);
    m_hScrollHover = IsPointInHScrollThumb(x, y);
    
    if (prevVHover != m_vScrollHover || prevHHover != m_hScrollHover) {
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
    
    // 处理单元格内的文本选择
    if (m_isEditing && (wParam & MK_LBUTTON)) {
        // 计算光标位置
        int row, col;
        RECT cellRect;
        HitTest(x, y, row, col, cellRect);
        
        if (row == m_editingRow && col == m_editingCol) {
            std::wstring* text = GetEditingCellTextPtr();
            if (text) {
                // TODO: 根据鼠标位置计算精确的选区
                // 这里简化处理，实际需要测量文本宽度
            }
        }
    }
}

void TableEditor::OnLButtonDown(int x, int y) {
    // 检查滚动条
    if (IsPointInVScrollThumb(x, y)) {
        m_isDraggingVScroll = true;
        m_dragStartY = y;
        m_dragStartScrollY = m_scrollY;
        SetCapture(m_hWnd);
        return;
    }
    
    if (IsPointInHScrollThumb(x, y)) {
        m_isDraggingHScroll = true;
        m_dragStartX = x;
        m_dragStartScrollX = m_scrollX;
        SetCapture(m_hWnd);
        return;
    }
    
    // 命中测试
    int row, col;
    RECT cellRect;
    HitTest(x, y, row, col, cellRect);
    
    if (row >= 0 && col >= 0) {
        std::vector<ColumnDef> cols = GetColumnDefs();
        
        if (col < (int)cols.size() && cols[col].isCheckbox) {
            // 复选框点击
            bool currentState = GetCellCheckState(row, col);
            SetCellCheckState(row, col, !currentState);
            CreateSnapshot(L"Toggle checkbox");
            InvalidateRect(m_hWnd, NULL, FALSE);
        } else {
            // 如果正在编辑其他单元格，先保存
            if (m_isEditing && (m_editingRow != row || m_editingCol != col)) {
                EndEditCell(true);
            }
            
            // 开始编辑，传递相对于单元格左边的坐标
            int relativeX = x - cellRect.left;
            StartEditCell(row, col, relativeX);
        }
    } else {
        // 点击空白区域，结束编辑
        EndEditCell(true);
    }
}

void TableEditor::OnLButtonUp(int x, int y) {
    if (m_isDraggingVScroll || m_isDraggingHScroll) {
        m_isDraggingVScroll = false;
        m_isDraggingHScroll = false;
        ReleaseCapture();
    }
}

void TableEditor::OnMouseWheel(int delta) {
    int scrollAmount = (delta / WHEEL_DELTA) * -m_rowHeight * 3;
    
    RECT clientRect;
    GetClientRect(m_hWnd, &clientRect);
    
    int totalContentHeight = GetTotalContentHeight();
    int viewHeight = clientRect.bottom - m_headerHeight;
    int maxScrollY = (std::max)(0, totalContentHeight - viewHeight);
    
    m_scrollY += scrollAmount;
    m_scrollY = (std::max)(0, (std::min)(m_scrollY, maxScrollY));
    
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void TableEditor::OnKeyDown(WPARAM wParam) {
    if (m_isEditing) {
        std::wstring* text = GetEditingCellTextPtr();
        if (!text) return;
        
        switch (wParam) {
            case VK_LEFT:
                if (m_editingCursorPos > 0) {
                    m_editingCursorPos--;
                    m_selectionStart = m_selectionEnd = m_editingCursorPos;
                    InvalidateRect(m_hWnd, NULL, FALSE);
                }
                break;
            
            case VK_RIGHT:
                if (m_editingCursorPos < (int)text->length()) {
                    m_editingCursorPos++;
                    m_selectionStart = m_selectionEnd = m_editingCursorPos;
                    InvalidateRect(m_hWnd, NULL, FALSE);
                }
                break;
            
            case VK_HOME:
                m_editingCursorPos = 0;
                m_selectionStart = m_selectionEnd = 0;
                InvalidateRect(m_hWnd, NULL, FALSE);
                break;
            
            case VK_END:
                m_editingCursorPos = (int)text->length();
                m_selectionStart = m_selectionEnd = m_editingCursorPos;
                InvalidateRect(m_hWnd, NULL, FALSE);
                break;
            
            case VK_RETURN:
                EndEditCell(true);
                break;
            
            case VK_ESCAPE:
                EndEditCell(false);
                break;
            
            case VK_BACK:
                if (m_selectionStart != m_selectionEnd) {
                    int selStart = (std::min)(m_selectionStart, m_selectionEnd);
                    int selEnd = (std::max)(m_selectionStart, m_selectionEnd);
                    text->erase(selStart, selEnd - selStart);
                    m_editingCursorPos = selStart;
                    m_selectionStart = m_selectionEnd = m_editingCursorPos;
                } else if (m_editingCursorPos > 0) {
                    text->erase(m_editingCursorPos - 1, 1);
                    m_editingCursorPos--;
                }
                // 立即保存到数据结构（实时保存）
                SetCellValue(m_editingRow, m_editingCol, *text);
                OnTextModified();
                InvalidateRect(m_hWnd, NULL, FALSE);
                break;
            
            case VK_DELETE:
                if (m_selectionStart != m_selectionEnd) {
                    int selStart = (std::min)(m_selectionStart, m_selectionEnd);
                    int selEnd = (std::max)(m_selectionStart, m_selectionEnd);
                    text->erase(selStart, selEnd - selStart);
                    m_editingCursorPos = selStart;
                    m_selectionStart = m_selectionEnd = m_editingCursorPos;
                } else if (m_editingCursorPos < (int)text->length()) {
                    text->erase(m_editingCursorPos, 1);
                }
                // 立即保存到数据结构（实时保存）
                SetCellValue(m_editingRow, m_editingCol, *text);
                OnTextModified();
                InvalidateRect(m_hWnd, NULL, FALSE);
                break;
        }
    } else {
        // 非编辑模式的快捷键
        if (GetKeyState(VK_CONTROL) & 0x8000) {
            switch (wParam) {
                case 'Z':
                    Undo();
                    break;
                case 'Y':
                    Redo();
                    break;
            }
        }
    }
}

bool TableEditor::GetEditingCursorScreenPos(int& x, int& y) {
    if (!m_isEditing) {
        return false;
    }
    
    // 这是基类的默认实现，子类应该重写以提供精确位置
    // 这里提供一个简单的估计
    x = 100;
    y = m_editingRow * m_rowHeight - m_scrollY;
    return true;
}

void TableEditor::OnChar(WPARAM wParam) {
    if (m_isEditing) {
        std::wstring* text = GetEditingCellTextPtr();
        if (!text) return;
        
        wchar_t ch = (wchar_t)wParam;
        
        // 忽略控制字符
        if (ch < 32 || ch == 127) {
            return;
        }
        
        // 删除选区
        if (m_selectionStart != m_selectionEnd) {
            int selStart = (std::min)(m_selectionStart, m_selectionEnd);
            int selEnd = (std::max)(m_selectionStart, m_selectionEnd);
            text->erase(selStart, selEnd - selStart);
            m_editingCursorPos = selStart;
            m_selectionStart = m_selectionEnd = m_editingCursorPos;
        }
        
        // 插入字符
        text->insert(m_editingCursorPos, 1, ch);
        m_editingCursorPos++;
        
        // 立即保存到数据结构（实时保存）
        SetCellValue(m_editingRow, m_editingCol, *text);
        
        // 通知文本已修改
        OnTextModified();
        
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

// === 编辑相关 ===

void TableEditor::StartEditCell(int row, int col, int clickX) {
    m_isEditing = true;
    m_editingRow = row;
    m_editingCol = col;
    m_editingCursorPos = 0;
    m_selectionStart = 0;
    m_selectionEnd = 0;
    
    // TODO: 根据 clickX 计算精确的光标位置
    std::wstring* text = GetEditingCellTextPtr();
    if (text) {
        m_editingCursorPos = (int)text->length();
    }
    
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void TableEditor::EndEditCell(bool save) {
    if (!m_isEditing) {
        return;
    }
    
    if (save) {
        // 验证并保存
        std::wstring* text = GetEditingCellTextPtr();
        if (text) {
            std::wstring error = ValidateCell(m_editingRow, m_editingCol, *text);
            if (error.empty()) {
                SetCellValue(m_editingRow, m_editingCol, *text);
                CreateSnapshot(L"Edit cell");
            } else {
                // TODO: 显示错误消息
            }
        }
    }
    
    m_isEditing = false;
    m_editingRow = -1;
    m_editingCol = -1;
    
    InvalidateRect(m_hWnd, NULL, FALSE);
}

std::wstring* TableEditor::GetEditingCellTextPtr() {
    // 子类需要重写以返回实际的文本指针
    // 这里返回 nullptr，子类实现具体逻辑
    return nullptr;
}

// === 滚动条相关 ===

void TableEditor::UpdateScrollbars(const RECT& clientRect) {
    int totalContentHeight = GetTotalContentHeight();
    int totalContentWidth = GetTotalContentWidth();
    int viewHeight = clientRect.bottom - m_headerHeight;
    
    // 横向滚动：需要考虑DllEditor的左边距（如果有的话）
    // 对于DllEditor，左边距75px固定可见，只滚动表格内容部分
    int leftMargin = 0;
    DllEditor* dllEditor = dynamic_cast<DllEditor*>(this);
    if (dllEditor) {
        leftMargin = 75;  // DllEditor的左边距宽度
    }
    int viewWidth = clientRect.right - m_scrollbarWidth - leftMargin;
    
    // 垂直滚动条
    if (totalContentHeight > viewHeight) {
        m_vScrollbarRect.left = clientRect.right - m_scrollbarWidth;
        m_vScrollbarRect.top = m_headerHeight;
        m_vScrollbarRect.right = clientRect.right;
        m_vScrollbarRect.bottom = clientRect.bottom - m_scrollbarWidth;
        
        UpdateVScrollThumb();
    } else {
        ZeroMemory(&m_vScrollbarRect, sizeof(RECT));
        ZeroMemory(&m_vScrollThumbRect, sizeof(RECT));
    }
    
    // 水平滚动条
    if (totalContentWidth > viewWidth) {
        m_hScrollbarRect.left = leftMargin;  // 从左边距开始，不覆盖行号区域
        m_hScrollbarRect.top = clientRect.bottom - m_scrollbarWidth;
        m_hScrollbarRect.right = clientRect.right - m_scrollbarWidth;
        m_hScrollbarRect.bottom = clientRect.bottom;
        
        UpdateHScrollThumb();
    } else {
        ZeroMemory(&m_hScrollbarRect, sizeof(RECT));
        ZeroMemory(&m_hScrollThumbRect, sizeof(RECT));
    }
}

void TableEditor::UpdateVScrollThumb() {
    RECT clientRect;
    GetClientRect(m_hWnd, &clientRect);
    
    int scrollbarHeight = m_vScrollbarRect.bottom - m_vScrollbarRect.top;
    int totalContentHeight = GetTotalContentHeight();
    int viewHeight = clientRect.bottom - m_headerHeight;
    
    if (totalContentHeight <= viewHeight) {
        ZeroMemory(&m_vScrollThumbRect, sizeof(RECT));
        return;
    }
    
    float thumbRatio = (float)viewHeight / totalContentHeight;
    thumbRatio = (std::min)(thumbRatio * 1.8f, 1.0f);  // 增加滚动块长度，但不超过100%
    int thumbHeight = (std::max)(20, (int)(scrollbarHeight * thumbRatio));
    
    float scrollRatio = (float)m_scrollY / (totalContentHeight - viewHeight);
    int thumbY = (int)(scrollRatio * (scrollbarHeight - thumbHeight));
    
    m_vScrollThumbRect.left = m_vScrollbarRect.left;
    m_vScrollThumbRect.top = m_vScrollbarRect.top + thumbY;
    m_vScrollThumbRect.right = m_vScrollbarRect.right;
    m_vScrollThumbRect.bottom = m_vScrollThumbRect.top + thumbHeight;
}

void TableEditor::UpdateHScrollThumb() {
    RECT clientRect;
    GetClientRect(m_hWnd, &clientRect);
    
    int scrollbarWidth = m_hScrollbarRect.right - m_hScrollbarRect.left;
    int totalContentWidth = GetTotalContentWidth();
    
    // 横向滚动：需要考虑DllEditor的左边距
    int leftMargin = 0;
    DllEditor* dllEditor = dynamic_cast<DllEditor*>(this);
    if (dllEditor) {
        leftMargin = 75;  // DllEditor的左边距宽度
    }
    int viewWidth = clientRect.right - m_scrollbarWidth - leftMargin;
    
    if (totalContentWidth <= viewWidth) {
        ZeroMemory(&m_hScrollThumbRect, sizeof(RECT));
        return;
    }
    
    float thumbRatio = (float)viewWidth / totalContentWidth;
    // 横向滚动条不需要放大倍数
    int thumbWidth = (std::max)(20, (int)(scrollbarWidth * thumbRatio));
    
    float scrollRatio = (float)m_scrollX / (totalContentWidth - viewWidth);
    int thumbX = (int)(scrollRatio * (scrollbarWidth - thumbWidth));
    
    m_hScrollThumbRect.left = m_hScrollbarRect.left + thumbX;
    m_hScrollThumbRect.top = m_hScrollbarRect.top;
    m_hScrollThumbRect.right = m_hScrollThumbRect.left + thumbWidth;
    m_hScrollThumbRect.bottom = m_hScrollbarRect.bottom;
}

bool TableEditor::IsPointInVScrollThumb(int x, int y) {
    return (x >= m_vScrollThumbRect.left && x <= m_vScrollThumbRect.right &&
            y >= m_vScrollThumbRect.top && y <= m_vScrollThumbRect.bottom);
}

bool TableEditor::IsPointInHScrollThumb(int x, int y) {
    return (x >= m_hScrollThumbRect.left && x <= m_hScrollThumbRect.right &&
            y >= m_hScrollThumbRect.top && y <= m_hScrollThumbRect.bottom);
}

// === 工具方法 ===

int TableEditor::GetTotalContentHeight() const {
    return GetRowCount() * m_rowHeight;
}

int TableEditor::GetTotalContentWidth() const {
    std::vector<ColumnDef> cols = GetColumnDefs();
    int totalWidth = 0;
    for (const auto& col : cols) {
        totalWidth += col.width;
    }
    return totalWidth;
}

void TableEditor::HitTest(int x, int y, int& row, int& col, RECT& cellRect) {
    row = -1;
    col = -1;
    ZeroMemory(&cellRect, sizeof(RECT));
    
    // 检查是否在表头
    if (y < m_headerHeight) {
        return;
    }
    
    // 检查是否在滚动条区域
    RECT clientRect;
    GetClientRect(m_hWnd, &clientRect);
    if (x >= clientRect.right - m_scrollbarWidth || y >= clientRect.bottom - m_scrollbarWidth) {
        return;
    }
    
    // 计算行
    int relativeY = y - m_headerHeight + m_scrollY;
    row = relativeY / m_rowHeight;
    
    if (row >= GetRowCount()) {
        row = -1;
        return;
    }
    
    // 计算列
    std::vector<ColumnDef> cols = GetColumnDefs();
    int currentX = -m_scrollX;
    for (size_t i = 0; i < cols.size(); i++) {
        if (x >= currentX && x < currentX + cols[i].width) {
            col = (int)i;
            cellRect.left = currentX;
            cellRect.top = m_headerHeight + row * m_rowHeight - m_scrollY;
            cellRect.right = currentX + cols[i].width;
            cellRect.bottom = cellRect.top + m_rowHeight;
            return;
        }
        currentX += cols[i].width;
    }
}
