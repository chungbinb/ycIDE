#include "DllEditor.h"
#include "EditorContext.h"
#include "NameValidator.h"
#include "Theme.h"
#include <sstream>
#include <algorithm>
#include <set>
#include <windowsx.h>
#include <gdiplus.h>

using namespace Gdiplus;

extern AppTheme g_CurrentTheme;

// COLORREF转GDI+ Color辅助函数
inline Color ColorFromCOLORREF(COLORREF colorref) {
    return Color(255, GetRValue(colorref), GetGValue(colorref), GetBValue(colorref));
}

// 构造/析构
DllEditor::DllEditor(HWND hWnd, EditorContext* context)
    : TableEditor(hWnd, context)
    , m_version(2)
    , m_leftMarginWidth(75)  // 左边距宽度：用于行号和展开/折叠符号（与代码编辑器一致）
    , m_lineNumberAreaWidth(55)  // 行号区域宽度（用于多选和鼠标指针变化）
    , m_cursorVisible(true)
    , m_isRowSelecting(false)
    , m_rowSelectStartRow(-1)
    , m_rowSelectEndRow(-1)
    , m_hasRowSelection(false)
    , m_isCellTextSelecting(false) {
    
    m_currentCellRect = {0, 0, 0, 0};
    
    m_fileName = L"未命名.ell";
    
    // DLL编辑器不使用全局表头，每个命令有自己的表头
    m_headerHeight = 0;
    
    // 启动光标闪烁定时器
    SetTimer(m_hWnd, CURSOR_TIMER_ID, CURSOR_BLINK_INTERVAL, NULL);
}

DllEditor::~DllEditor() {
    // 停止光标闪烁定时器
    KillTimer(m_hWnd, CURSOR_TIMER_ID);
}

// === 重写 TableEditor 的虚接口 ===

std::vector<ColumnDef> DllEditor::GetColumnDefs() const {
    // 统一的列定义，兼容所有行类型
    std::vector<ColumnDef> cols;
    cols.push_back(ColumnDef(L"列1", 200));
    cols.push_back(ColumnDef(L"列2", 120));
    cols.push_back(ColumnDef(L"列3", 80, true, true));
    cols.push_back(ColumnDef(L"列4", 80, true, true));
    cols.push_back(ColumnDef(L"列5", 200));
    return cols;
}

int DllEditor::GetRowCount() const {
    int count = 0;
    for (size_t i = 0; i < m_commands.size(); i++) {
        const auto& cmd = m_commands[i];
        // 每个命令占用：
        // 1. 命令表头
        // 2. 命令数据
        // 3. 库文件名表头
        // 4. 库文件名数据
        // 5. 在库中对应的命令名表头
        // 6. DLL命令名数据
        // 7. 参数表头
        // 8. N个参数数据行
        count += 7 + (int)cmd.paramLines.size();
        
        // 每个命令后添加一个间隔行（最后一个命令除外）
        // 注：间隔行用于行数计算，但实际绘制时只是空白间隔
        if (i < m_commands.size() - 1) {
            count += 1;
        }
    }
    return count;
}

int DllEditor::GetTotalContentHeight() const {
    int totalHeight = 0;
    const int separatorHeight = 20;
    
    for (size_t i = 0; i < m_commands.size(); i++) {
        const auto& cmd = m_commands[i];
        // 如果命令被折叠，只显示2行（命令表头+命令数据）
        int visibleRows;
        if (cmd.isCollapsed) {
            visibleRows = 2;
        } else {
            // 每个命令：7个固定行 + N个参数行
            visibleRows = 7 + (int)cmd.paramLines.size();
        }
        totalHeight += visibleRows * m_rowHeight;
        
        // 命令之间的间隔（最后一个命令除外）
        if (i < m_commands.size() - 1) {
            totalHeight += separatorHeight;
        }
    }
    
    return totalHeight;
}

int DllEditor::GetTotalContentWidth() const {
    if (m_commands.empty()) {
        return m_leftMarginWidth + 10 + 600;  // 默认宽度
    }
    
    // 需要计算所有命令的最大宽度
    // 为了避免重复创建Graphics对象，这里使用估算
    // 或者缓存宽度值
    int maxWidth = 0;
    
    // 遍历所有命令，找出最大宽度
    for (const auto& cmd : m_commands) {
        // 粗略估算：每个字符约占12像素（对于中文可能更宽）
        int cmdNameWidth = (std::max)(120, (int)cmd.commandName.length() * 12 + 30);
        int returnTypeWidth = (std::max)(80, (int)cmd.returnType.length() * 12 + 30);
        int commentWidth = (std::max)(120, (int)cmd.comment.length() * 12 + 30);
        int dllFileWidth = (int)cmd.dllFileName.length() * 12 + 30;
        int dllCmdWidth = (int)cmd.dllCommandName.length() * 12 + 30;
        
        // 命令行宽度
        int cmdRowWidth = cmdNameWidth + returnTypeWidth + 80 + commentWidth;
        
        // 参数最大宽度
        int maxParamNameWidth = 120;
        int maxParamTypeWidth = 80;
        int maxParamCommentWidth = 120;
        
        for (int paramIdx : cmd.paramLines) {
            if (paramIdx >= 0 && paramIdx < (int)m_allParams.size()) {
                const DllParameter& param = m_allParams[paramIdx];
                maxParamNameWidth = (std::max)(maxParamNameWidth, (int)param.paramName.length() * 12 + 30);
                maxParamTypeWidth = (std::max)(maxParamTypeWidth, (int)param.paramType.length() * 12 + 30);
                maxParamCommentWidth = (std::max)(maxParamCommentWidth, (int)param.comment.length() * 12 + 30);
            }
        }
        
        int paramRowWidth = maxParamNameWidth + maxParamTypeWidth + 80 + 80 + maxParamCommentWidth;
        
        // 整行宽度
        int fullRowWidth = (std::max)(dllFileWidth, dllCmdWidth);
        
        // 当前命令的最大宽度
        int cmdMaxWidth = (std::max)({cmdRowWidth, paramRowWidth, fullRowWidth});
        maxWidth = (std::max)(maxWidth, cmdMaxWidth);
    }
    
    // 返回最大宽度 + 左边距 + 左偏移 + 额外滚动空间
    return maxWidth + m_leftMarginWidth + 10 + 100;
}

void DllEditor::HitTest(int x, int y, int& row, int& col, RECT& cellRect) {
    row = -1;
    col = -1;
    ZeroMemory(&cellRect, sizeof(RECT));
    
    // 检查是否在行号区域
    if (x < m_leftMarginWidth) {
        return;
    }
    
    // 检查是否在滚动条区域
    RECT clientRect;
    GetClientRect(m_hWnd, &clientRect);
    if (x >= clientRect.right - m_scrollbarWidth || y >= clientRect.bottom - m_scrollbarWidth) {
        return;
    }
    
    // DllEditor的布局比较复杂，需要遍历所有命令来确定点击了哪一行
    int yPos = 0 - m_scrollY;
    const int separatorHeight = 20;
    const int tableLeftOffset = 10;
    int tableStartX = m_leftMarginWidth + tableLeftOffset - m_scrollX;
    
    for (size_t cmdIdx = 0; cmdIdx < m_commands.size(); cmdIdx++) {
        const DllCommand& cmd = m_commands[cmdIdx];
        
        // 跳过间隔行
        if (cmdIdx > 0) {
            if (y >= yPos && y < yPos + separatorHeight) {
                return;  // 点击在间隔区域
            }
            yPos += separatorHeight;
        }
        
        // 计算这个命令表格的总行数
        int totalRows = 7 + (int)cmd.paramLines.size();
        int tableHeight = totalRows * m_rowHeight;
        
        // 检查是否点击在这个命令的表格内
        if (y >= yPos && y < yPos + tableHeight) {
            // 确定点击了表格中的哪一行
            int relativeY = y - yPos;
            int subRow = relativeY / m_rowHeight;
            
            // 根据subRow确定全局row和col
            int globalRow = GetGlobalRowFromCmdAndSubRow((int)cmdIdx, subRow);
            
            // 根据subRow类型确定列
            if (subRow == 0) {
                // 表头行，不可编辑
                return;
            } else if (subRow == 1) {
                // DLL命令数据行：4列（DLL命令名、返回值类型、公开、备注）
                row = globalRow;
                
                // 计算命令列宽度（与DrawCommandTable中的对齐逻辑一致）
                HDC hdc = GetDC(m_hWnd);
                Graphics graphics(hdc);
                FontFamily fontFamily(L"Microsoft YaHei");
                Font dataFont(&fontFamily, (REAL)m_fontSize, FontStyleRegular, UnitPixel);
                int colPadding = 5;
                
                RectF measureRect;
                graphics.MeasureString(cmd.commandName.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
                int cmdNameWidth = (std::max)(120, (int)measureRect.Width + colPadding * 2 + 10);
                
                graphics.MeasureString(cmd.returnType.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
                int returnTypeWidth = (std::max)(80, (int)measureRect.Width + colPadding * 2 + 10);
                
                graphics.MeasureString(cmd.comment.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
                int commentWidth = (std::max)(120, (int)measureRect.Width + colPadding * 2 + 10);
                
                // 测量参数宽度以计算对齐
                int maxParamNameWidth = 120;
                int maxParamTypeWidth = 80;
                int maxParamCommentWidth = 120;
                
                for (int paramIdx : cmd.paramLines) {
                    if (paramIdx >= 0 && paramIdx < (int)m_allParams.size()) {
                        const DllParameter& param = m_allParams[paramIdx];
                        
                        graphics.MeasureString(param.paramName.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
                        maxParamNameWidth = (std::max)(maxParamNameWidth, (int)measureRect.Width + colPadding * 2 + 10);
                        
                        graphics.MeasureString(param.paramType.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
                        maxParamTypeWidth = (std::max)(maxParamTypeWidth, (int)measureRect.Width + colPadding * 2 + 10);
                        
                        graphics.MeasureString(param.comment.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
                        maxParamCommentWidth = (std::max)(maxParamCommentWidth, (int)measureRect.Width + colPadding * 2 + 10);
                    }
                }
                
                ReleaseDC(m_hWnd, hdc);
                
                // 计算对齐后的列宽（与DrawCommandTable一致）
                int col1Width = (std::max)(cmdNameWidth, maxParamNameWidth);
                int col2Width = (std::max)(returnTypeWidth, maxParamTypeWidth);
                int col3Width = 40;
                int col4Width = (std::max)(commentWidth, 40 + maxParamCommentWidth);
                
                std::vector<int> colWidths = {col1Width, col2Width, col3Width, col4Width};
                
                // 根据X坐标确定列号
                int currentX = tableStartX;
                col = -1;
                for (size_t i = 0; i < colWidths.size(); i++) {
                    if (x >= currentX && x < currentX + colWidths[i]) {
                        col = (int)i;
                        cellRect.left = currentX;
                        cellRect.right = currentX + colWidths[i];
                        break;
                    }
                    currentX += colWidths[i];
                }
                
                // 如果没有找到匹配的列，检查是否点击在表格右侧空白区域
                if (col == -1 && x >= tableStartX) {
                    // 默认选择最后一列（备注列）
                    col = (int)colWidths.size() - 1;
                    // 计算最后一列的位置
                    int lastColX = tableStartX;
                    for (size_t i = 0; i < colWidths.size() - 1; i++) {
                        lastColX += colWidths[i];
                    }
                    cellRect.left = lastColX;
                    cellRect.right = lastColX + colWidths[col];
                }
                
                cellRect.top = yPos + subRow * m_rowHeight;
                cellRect.bottom = cellRect.top + m_rowHeight;
                return;
            } else if (subRow == 2) {
                // 库文件名表头
                return;
            } else if (subRow == 3) {
                // 库文件名数据（单列）
                row = globalRow;
                col = 0;
                cellRect.left = tableStartX;
                cellRect.top = yPos + subRow * m_rowHeight;
                cellRect.right = clientRect.right;
                cellRect.bottom = cellRect.top + m_rowHeight;
                return;
            } else if (subRow == 4) {
                // DLL命令名表头
                return;
            } else if (subRow == 5) {
                // DLL命令名数据（单列）
                row = globalRow;
                col = 0;
                cellRect.left = tableStartX;
                cellRect.top = yPos + subRow * m_rowHeight;
                cellRect.right = clientRect.right;
                cellRect.bottom = cellRect.top + m_rowHeight;
                return;
            } else if (subRow == 6) {
                // 参数表头
                return;
            } else {
                // 参数数据行：5列（参数名、类型、传址、数组、备注）
                row = globalRow;
                
                // 计算参数列宽度（与DrawCommandTable中的对齐逻辑一致）
                HDC hdc = GetDC(m_hWnd);
                Graphics graphics(hdc);
                FontFamily fontFamily(L"Microsoft YaHei");
                Font dataFont(&fontFamily, (REAL)m_fontSize, FontStyleRegular, UnitPixel);
                int colPadding = 5;
                
                RectF measureRect;
                // 测量命令宽度
                graphics.MeasureString(cmd.commandName.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
                int cmdNameWidth = (std::max)(120, (int)measureRect.Width + colPadding * 2 + 10);
                
                graphics.MeasureString(cmd.returnType.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
                int returnTypeWidth = (std::max)(80, (int)measureRect.Width + colPadding * 2 + 10);
                
                graphics.MeasureString(cmd.comment.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
                int commentWidth = (std::max)(120, (int)measureRect.Width + colPadding * 2 + 10);
                
                // 测量参数宽度
                int maxParamNameWidth = 120;
                int maxParamTypeWidth = 80;
                int maxParamCommentWidth = 120;
                
                for (int paramIdx : cmd.paramLines) {
                    if (paramIdx >= 0 && paramIdx < (int)m_allParams.size()) {
                        const DllParameter& param = m_allParams[paramIdx];
                        
                        graphics.MeasureString(param.paramName.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
                        maxParamNameWidth = (std::max)(maxParamNameWidth, (int)measureRect.Width + colPadding * 2 + 10);
                        
                        graphics.MeasureString(param.paramType.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
                        maxParamTypeWidth = (std::max)(maxParamTypeWidth, (int)measureRect.Width + colPadding * 2 + 10);
                        
                        graphics.MeasureString(param.comment.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
                        maxParamCommentWidth = (std::max)(maxParamCommentWidth, (int)measureRect.Width + colPadding * 2 + 10);
                    }
                }
                
                ReleaseDC(m_hWnd, hdc);
                
                // 计算对齐后的列宽（与DrawCommandTable一致）
                int col1Width = (std::max)(cmdNameWidth, maxParamNameWidth);
                int col2Width = (std::max)(returnTypeWidth, maxParamTypeWidth);
                int col3Width = 40;
                int col4Width = (std::max)(commentWidth, 40 + maxParamCommentWidth);
                
                std::vector<int> colWidths = {col1Width, col2Width, col3Width, 40, col4Width - 40};
                
                // 根据X坐标确定列号
                int currentX = tableStartX;
                col = -1;
                for (size_t i = 0; i < colWidths.size(); i++) {
                    if (x >= currentX && x < currentX + colWidths[i]) {
                        col = (int)i;
                        cellRect.left = currentX;
                        cellRect.right = currentX + colWidths[i];
                        break;
                    }
                    currentX += colWidths[i];
                }
                
                // 如果没有找到匹配的列，检查是否点击在表格右侧空白区域
                if (col == -1 && x >= tableStartX) {
                    // 默认选择最后一列（备注列）
                    col = (int)colWidths.size() - 1;
                    // 计算最后一列的位置
                    int lastColX = tableStartX;
                    for (size_t i = 0; i < colWidths.size() - 1; i++) {
                        lastColX += colWidths[i];
                    }
                    cellRect.left = lastColX;
                    cellRect.right = lastColX + colWidths[col];
                }
                
                cellRect.top = yPos + subRow * m_rowHeight;
                cellRect.bottom = cellRect.top + m_rowHeight;
                return;
            }
        }
        
        yPos += tableHeight;
    }
}

std::wstring DllEditor::GetCellText(int row, int col) const {
    int cmdIndex, paramIndex;
    DllRowType rowType = GetRowType(row, cmdIndex, paramIndex);
    
    // 间隔行返回空
    if (rowType == DllRowType::Separator) {
        return L"";
    }
    
    if (cmdIndex < 0 || cmdIndex >= (int)m_commands.size()) {
        return L"";
    }
    
    const DllCommand& cmd = m_commands[cmdIndex];
    
    switch (rowType) {
        case DllRowType::CommandHeader:
            // 第一行：DLL命令名、返回值类型、公开、备注（表头）
            switch (col) {
                case 0: return L"DLL命令名";
                case 1: return L"返回值类型";
                case 2: return L"公开";
                case 3: return L"备注";
                default: return L"";
            }
            
        case DllRowType::CommandData:
            // 第二行：命令数据
            switch (col) {
                case 0: return cmd.commandName;
                case 1: return cmd.returnType;
                case 2: return cmd.isPublic ? L"✓" : L"";
                case 3: return cmd.comment;
                default: return L"";
            }
            
        case DllRowType::LibFileHeader:
            // 第三行：库文件名（表头）
            if (col == 0) return L"库文件名";
            return L"";
            
        case DllRowType::LibFileData:
            // 第四行：库文件名数据
            if (col == 0) return cmd.dllFileName;
            return L"";
            
        case DllRowType::DllNameHeader:
            // 第五行：在库中对应的命令名（表头）
            if (col == 0) return L"在库中对应的命令名";
            return L"";
            
        case DllRowType::DllNameData:
            // 第六行：DLL命令名数据
            if (col == 0) return cmd.dllCommandName;
            return L"";
            
        case DllRowType::ParamHeader:
            // 第七行：参数名、类型、传址、数组、备注（表头）
            switch (col) {
                case 0: return L"参数名";
                case 1: return L"类型";
                case 2: return L"传址";
                case 3: return L"数组";
                case 4: return L"备注";
                default: return L"";
            }
            
        case DllRowType::ParamData:
            // 参数数据行
            if (paramIndex >= 0 && paramIndex < (int)cmd.paramLines.size()) {
                int paramIdx = cmd.paramLines[paramIndex];
                if (paramIdx >= 0 && paramIdx < (int)m_allParams.size()) {
                    const DllParameter& param = m_allParams[paramIdx];
                    
                    switch (col) {
                        case 0: return param.paramName;
                        case 1: return param.paramType;
                        case 2: return param.byRef ? L"✓" : L"";
                        case 3: return param.isArray ? L"✓" : L"";
                        case 4: return param.comment;
                        default: return L"";
                    }
                }
            }
            break;
    }
    
    return L"";
}

void DllEditor::SetCellValue(int row, int col, const std::wstring& value) {
    int cmdIndex, paramIndex;
    DllRowType rowType = GetRowType(row, cmdIndex, paramIndex);
    
    if (cmdIndex < 0 || cmdIndex >= (int)m_commands.size()) {
        return;
    }
    
    DllCommand& cmd = m_commands[cmdIndex];
    
    switch (rowType) {
        case DllRowType::CommandHeader:
            // 表头不允许编辑
            break;
            
        case DllRowType::CommandData:
            // 命令数据行
            switch (col) {
                case 0: cmd.commandName = value; break;
                case 1: cmd.returnType = value; break;
                case 3: cmd.comment = value; break;
            }
            break;
            
        case DllRowType::LibFileHeader:
        case DllRowType::DllNameHeader:
        case DllRowType::ParamHeader:
            // 表头不允许编辑
            break;
            
        case DllRowType::LibFileData:
            // 库文件名数据
            if (col == 0) cmd.dllFileName = value;
            break;
            
        case DllRowType::DllNameData:
            // DLL命令名数据
            if (col == 0) cmd.dllCommandName = value;
            break;
            
        case DllRowType::ParamData:
            // 参数数据行
            if (paramIndex >= 0 && paramIndex < (int)cmd.paramLines.size()) {
                int paramIdx = cmd.paramLines[paramIndex];
                if (paramIdx >= 0 && paramIdx < (int)m_allParams.size()) {
                    DllParameter& param = m_allParams[paramIdx];
                    
                    switch (col) {
                        case 0: param.paramName = value; break;
                        case 1: param.paramType = value; break;
                        case 4: param.comment = value; break;
                    }
                }
            }
            break;
    }
    
    m_modified = true;
    
    // 通知父窗口文件已修改
    HWND hParent = GetParent(m_hWnd);
    if (hParent) {
        SendMessage(hParent, WM_COMMAND, MAKEWPARAM(0, 0x1000), (LPARAM)m_hWnd);
    }
}

bool DllEditor::GetCellCheckState(int row, int col) const {
    int cmdIndex, paramIndex;
    DllRowType rowType = GetRowType(row, cmdIndex, paramIndex);
    
    if (cmdIndex < 0 || cmdIndex >= (int)m_commands.size()) {
        return false;
    }
    
    const DllCommand& cmd = m_commands[cmdIndex];
    
    switch (rowType) {
        case DllRowType::CommandData:
            // 命令数据行 - 公开复选框
            if (col == 2) return cmd.isPublic;
            break;
            
        case DllRowType::ParamData:
            // 参数数据行
            if (paramIndex >= 0 && paramIndex < (int)cmd.paramLines.size()) {
                int paramIdx = cmd.paramLines[paramIndex];
                if (paramIdx >= 0 && paramIdx < (int)m_allParams.size()) {
                    const DllParameter& param = m_allParams[paramIdx];
                    
                    switch (col) {
                        case 2: return param.byRef;    // 传址
                        case 3: return param.isArray;  // 数组
                    }
                }
            }
            break;
            
        default:
            break;
    }
    
    return false;
}

void DllEditor::SetCellCheckState(int row, int col, bool checked) {
    int cmdIndex, paramIndex;
    DllRowType rowType = GetRowType(row, cmdIndex, paramIndex);
    
    if (cmdIndex < 0 || cmdIndex >= (int)m_commands.size()) {
        return;
    }
    
    DllCommand& cmd = m_commands[cmdIndex];
    
    switch (rowType) {
        case DllRowType::CommandData:
            // 命令数据行 - 公开复选框
            if (col == 2) cmd.isPublic = checked;
            break;
            
        case DllRowType::ParamData:
            // 参数数据行
            if (paramIndex >= 0 && paramIndex < (int)cmd.paramLines.size()) {
                int paramIdx = cmd.paramLines[paramIndex];
                if (paramIdx >= 0 && paramIdx < (int)m_allParams.size()) {
                    DllParameter& param = m_allParams[paramIdx];
                    
                    switch (col) {
                        case 2: param.byRef = checked; break;    // 传址
                        case 3: param.isArray = checked; break;  // 数组
                    }
                }
            }
            break;
            
        default:
            break;
    }
    
    m_modified = true;
    
    // 通知父窗口文件已修改
    HWND hParent = GetParent(m_hWnd);
    if (hParent) {
        SendMessage(hParent, WM_COMMAND, MAKEWPARAM(0, 0x1000), (LPARAM)m_hWnd);
    }
}

bool DllEditor::IsCellCheckbox(int row, int col) const {
    int cmdIndex, paramIndex;
    DllRowType rowType = GetRowType(row, cmdIndex, paramIndex);
    
    switch (rowType) {
        case DllRowType::CommandData:
            // 命令数据行：第2列（公开）是复选框
            return (col == 2);
            
        case DllRowType::ParamData:
            // 参数数据行：第2列（传址）和第3列（数组）是复选框
            return (col == 2 || col == 3);
            
        default:
            return false;
    }
}

void DllEditor::OnKeyDown(WPARAM wParam) {
    // 处理 Ctrl 组合键
    bool ctrlPressed = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
    
    if (ctrlPressed) {
        switch (wParam) {
            case 'A':
                // Ctrl+A: 全选
                if (m_isEditing) {
                    // 编辑状态下全选当前单元格文本
                    m_selectionStart = 0;
                    m_selectionEnd = (int)m_editBuffer.length();
                    m_editingCursorPos = m_selectionEnd;
                    InvalidateRect(m_hWnd, NULL, FALSE);
                } else {
                    // 非编辑状态下全选所有行
                    if (!m_commands.empty()) {
                        m_rowSelectStartRow = 0;
                        m_rowSelectEndRow = GetRowCount() - 1;
                        m_hasRowSelection = true;
                        InvalidateRect(m_hWnd, NULL, FALSE);
                    }
                }
                return;
                
            case 'S':
                // Ctrl+S: 保存
                SaveFile();
                return;
                
            case 'C':
                // Ctrl+C: 复制
                if (m_isEditing && m_selectionStart != m_selectionEnd) {
                    // 复制选中的文本到剪贴板
                    int selStart = (std::min)(m_selectionStart, m_selectionEnd);
                    int selEnd = (std::max)(m_selectionStart, m_selectionEnd);
                    std::wstring selectedText = m_editBuffer.substr(selStart, selEnd - selStart);
                    
                    if (OpenClipboard(m_hWnd)) {
                        EmptyClipboard();
                        size_t size = (selectedText.length() + 1) * sizeof(wchar_t);
                        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);
                        if (hMem) {
                            wchar_t* pMem = (wchar_t*)GlobalLock(hMem);
                            wcscpy_s(pMem, selectedText.length() + 1, selectedText.c_str());
                            GlobalUnlock(hMem);
                            SetClipboardData(CF_UNICODETEXT, hMem);
                        }
                        CloseClipboard();
                    }
                }
                return;
                
            case 'X':
                // Ctrl+X: 剪切
                if (m_isEditing && m_selectionStart != m_selectionEnd) {
                    // 先复制选中的文本到剪贴板
                    int selStart = (std::min)(m_selectionStart, m_selectionEnd);
                    int selEnd = (std::max)(m_selectionStart, m_selectionEnd);
                    std::wstring selectedText = m_editBuffer.substr(selStart, selEnd - selStart);
                    
                    if (OpenClipboard(m_hWnd)) {
                        EmptyClipboard();
                        size_t size = (selectedText.length() + 1) * sizeof(wchar_t);
                        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);
                        if (hMem) {
                            wchar_t* pMem = (wchar_t*)GlobalLock(hMem);
                            wcscpy_s(pMem, selectedText.length() + 1, selectedText.c_str());
                            GlobalUnlock(hMem);
                            SetClipboardData(CF_UNICODETEXT, hMem);
                        }
                        CloseClipboard();
                    }
                    
                    // 然后删除选中的文本
                    m_editBuffer.erase(selStart, selEnd - selStart);
                    m_editingCursorPos = selStart;
                    m_selectionStart = m_selectionEnd = m_editingCursorPos;
                    SetCellValue(m_editingRow, m_editingCol, m_editBuffer);
                    OnTextModified();
                    InvalidateRect(m_hWnd, NULL, FALSE);
                }
                return;
                
            case 'V':
                // Ctrl+V: 粘贴
                if (m_isEditing) {
                    if (OpenClipboard(m_hWnd)) {
                        HANDLE hData = GetClipboardData(CF_UNICODETEXT);
                        if (hData) {
                            wchar_t* pText = (wchar_t*)GlobalLock(hData);
                            if (pText) {
                                std::wstring pasteText = pText;
                                GlobalUnlock(hData);
                                
                                // 如果有选区，先删除选区
                                if (m_selectionStart != m_selectionEnd) {
                                    int selStart = (std::min)(m_selectionStart, m_selectionEnd);
                                    int selEnd = (std::max)(m_selectionStart, m_selectionEnd);
                                    m_editBuffer.erase(selStart, selEnd - selStart);
                                    m_editingCursorPos = selStart;
                                }
                                
                                // 插入粘贴的文本
                                m_editBuffer.insert(m_editingCursorPos, pasteText);
                                m_editingCursorPos += (int)pasteText.length();
                                m_selectionStart = m_selectionEnd = m_editingCursorPos;
                                
                                SetCellValue(m_editingRow, m_editingCol, m_editBuffer);
                                OnTextModified();
                                InvalidateRect(m_hWnd, NULL, FALSE);
                            }
                        }
                        CloseClipboard();
                    }
                }
                return;
                
            case 'N':
                // Ctrl+N: 新建DLL命令
                CreateNewDllCommand();
                return;
        }
    }
    
    // 如果有行选择且按下Delete或Backspace键，删除选中的行
    if (m_hasRowSelection && (wParam == VK_DELETE || wParam == VK_BACK)) {
        DeleteSelectedRows();
        return;
    }
    
    // 如果正在编辑参数行且按下回车键，插入新参数行
    if (m_isEditing && wParam == VK_RETURN) {
        int cmdIndex, paramIndex;
        DllRowType rowType = GetRowType(m_editingRow, cmdIndex, paramIndex);
        
        if (rowType == DllRowType::ParamData) {
            // 先保存当前编辑
            EndEditCell(true);
            
            // 插入新参数（在当前参数后面）
            InsertParameter(cmdIndex, paramIndex);
            
            // 标记为已修改
            m_modified = true;
            
            // 创建快照
            CreateSnapshot(L"Insert parameter");
            
            // 刷新显示
            InvalidateRect(m_hWnd, NULL, FALSE);
            
            // 开始编辑新插入的参数行的第一列（参数名）
            int newParamRow = GetGlobalRowFromCmdAndSubRow(cmdIndex, 7 + paramIndex + 1);
            StartEditCell(newParamRow, 0, 0);
            
            return;
        }
    }
    
    // 其他情况调用基类处理
    TableEditor::OnKeyDown(wParam);
}

bool DllEditor::GetEditingCursorScreenPos(int& x, int& y) {
    if (!m_isEditing) {
        return false;
    }
    
    // 获取客户区
    RECT clientRect;
    GetClientRect(m_hWnd, &clientRect);
    
    // 需要找到当前编辑单元格的屏幕矩形
    // 遍历所有命令找到对应的位置
    int yPos = 0 - m_scrollY;
    const int separatorHeight = 20;
    const int tableLeftOffset = 10;
    int tableStartX = m_leftMarginWidth + tableLeftOffset - m_scrollX;
    
    int cmdIndex, paramIndex;
    DllRowType rowType = GetRowType(m_editingRow, cmdIndex, paramIndex);
    
    if (cmdIndex < 0 || cmdIndex >= (int)m_commands.size()) {
        return false;
    }
    
    // 累加到目标命令之前的所有高度
    for (int i = 0; i < cmdIndex; i++) {
        const DllCommand& cmd = m_commands[i];
        int paramCount = (int)cmd.paramLines.size();
        int tableHeight = 7 * m_rowHeight + paramCount * m_rowHeight;
        yPos += tableHeight;
        
        // 加上间隔
        if (i < (int)m_commands.size() - 1) {
            yPos += separatorHeight;
        }
    }
    
    // 计算当前单元格在表格中的子行号
    int subRow;
    GetCommandIndexFromRow(m_editingRow, subRow);
    
    // 计算单元格的 Y 位置
    int cellY = yPos + subRow * m_rowHeight;
    
    // 使用 GDI+ 测量从单元格开始到光标位置的文本宽度
    HDC hdc = GetDC(m_hWnd);
    Graphics graphics(hdc);
    FontFamily fontFamily(L"Microsoft YaHei");
    Font font(&fontFamily, (REAL)m_fontSize, FontStyleRegular, UnitPixel);
    
    StringFormat format(StringFormat::GenericTypographic());
    format.SetFormatFlags(StringFormatFlagsMeasureTrailingSpaces);
    
    // 测量光标前的文本宽度
    std::wstring textBeforeCursor = m_editBuffer.substr(0, m_editingCursorPos);
    RectF boundingBox;
    graphics.MeasureString(textBeforeCursor.c_str(), -1, &font, PointF(0, 0), &format, &boundingBox);
    
    ReleaseDC(m_hWnd, hdc);
    
    // 计算列的起始 X 位置（简化版本，使用固定列宽）
    const int colPadding = 5;
    int colWidths[] = {200, 100, 50, 300, 200}; // 各列宽度估算
    
    int cellX = tableStartX;
    for (int i = 0; i < m_editingCol && i < 5; i++) {
        cellX += colWidths[i];
    }
    
    // 计算光标的屏幕位置
    x = cellX + colPadding + (int)boundingBox.Width;
    y = cellY + 2;
    
    return true;
}

void DllEditor::OnLButtonDown(int x, int y) {
    // 清除之前的行选择
    ClearRowSelection();
    
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
    
    // 检查是否点击在行号区域（表格外面）- 开始行选择
    // 只在行号区域（0到m_lineNumberAreaWidth像素）进行多选
    // 右边的展开/折叠按钮区域不处理多选
    if (x < m_lineNumberAreaWidth) {
        int hitRow = HitTestRow(x, y);
        if (hitRow >= 0) {
            m_isRowSelecting = true;
            m_rowSelectStartRow = hitRow;
            m_rowSelectEndRow = hitRow;
            m_hasRowSelection = true;
            EndEditCell(true);  // 结束任何编辑
            SetFocus(m_hWnd);   // 获取键盘焦点以便接收按键事件
            SetCapture(m_hWnd);
            InvalidateRect(m_hWnd, NULL, FALSE);
            return;
        }
    }
    
    // 检查是否点击在展开/折叠按钮区域
    if (x >= m_lineNumberAreaWidth && x < m_leftMarginWidth) {
        // 展开/折叠按钮区域
        int hitRow = HitTestRow(x, y);
        if (hitRow >= 0) {
            // 检查是否点击在某个命令的第一行（只有第一行有折叠按钮）
            int cmdIndex, paramIndex;
            DllRowType rowType = GetRowType(hitRow, cmdIndex, paramIndex);
            
            // 只有命令表头行（subRow=0）才有折叠按钮
            if (rowType == DllRowType::CommandHeader && cmdIndex >= 0 && cmdIndex < (int)m_commands.size()) {
                // 切换折叠状态
                m_commands[cmdIndex].isCollapsed = !m_commands[cmdIndex].isCollapsed;
                InvalidateRect(m_hWnd, NULL, FALSE);
                return;
            }
        }
        // 点击在非折叠按钮的位置，忽略
        return;
    }
    
    // 命中测试
    int row, col;
    RECT cellRect;
    HitTest(x, y, row, col, cellRect);
    
    if (row >= 0 && col >= 0) {
        // 检查点击位置是否确实在单元格范围内，而不是表格右边的空白区域
        // 如果 x 超过了单元格右边界较多，则认为是点击空白区域进行多选
        if (x > cellRect.right + 20) {
            // 点击在表格右边的空白区域
            int hitRow = HitTestRow(x, y);
            if (hitRow >= 0) {
                m_isRowSelecting = true;
                m_rowSelectStartRow = hitRow;
                m_rowSelectEndRow = hitRow;
                m_hasRowSelection = true;
                EndEditCell(true);
                SetFocus(m_hWnd);
                SetCapture(m_hWnd);
                InvalidateRect(m_hWnd, NULL, FALSE);
                return;
            }
        }
        
        // 使用DllEditor特定的复选框判断逻辑
        if (IsCellCheckbox(row, col)) {
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
            
            // 保存当前单元格矩形，开始文本选择模式
            m_currentCellRect = cellRect;
            m_isCellTextSelecting = true;
            m_selectionStart = m_editingCursorPos;
            m_selectionEnd = m_editingCursorPos;
            SetCapture(m_hWnd);
        }
    } else if (row >= 0 && col < 0) {
        // 点击在表格右边的空白区域（能命中行但没有命中列）
        // 开始行选择
        m_isRowSelecting = true;
        m_rowSelectStartRow = row;
        m_rowSelectEndRow = row;
        m_hasRowSelection = true;
        EndEditCell(true);  // 结束任何编辑
        SetFocus(m_hWnd);   // 获取键盘焦点以便接收按键事件
        SetCapture(m_hWnd);
        InvalidateRect(m_hWnd, NULL, FALSE);
    } else {
        // 点击完全空白的区域（表格下方或未能识别的区域）
        // 尝试使用HitTestRow看是否能命中某行
        int hitRow = HitTestRow(x, y);
        if (hitRow >= 0) {
            // 能命中行，开始行选择
            m_isRowSelecting = true;
            m_rowSelectStartRow = hitRow;
            m_rowSelectEndRow = hitRow;
            m_hasRowSelection = true;
            EndEditCell(true);
            SetFocus(m_hWnd);
            SetCapture(m_hWnd);
            InvalidateRect(m_hWnd, NULL, FALSE);
        } else {
            // 真正的空白区域，结束编辑
            EndEditCell(true);
        }
    }
}

std::wstring DllEditor::ValidateCell(int row, int col, const std::wstring& value) const {
    int cmdIndex, paramIndex;
    DllRowType rowType = GetRowType(row, cmdIndex, paramIndex);
    
    // 检查空名称
    if (col == 0 && value.empty()) {
        return L"名称不能为空";
    }
    
    // 使用NameValidator检查命令名
    if (col == 0 && m_pContext && m_pContext->GetNameValidator()) {
        if (rowType == DllRowType::CommandData) {
            // 命令名验证
            std::wstring cleanName = value;
            size_t start = cleanName.find_first_not_of(L" \t");
            if (start != std::wstring::npos) {
                cleanName = cleanName.substr(start);
            }
            
            return m_pContext->GetNameValidator()->ValidateName(
                cleanName, SymbolType::DllCommand, m_filePath);
        }
    }
    
    return L""; // 验证通过
}

std::vector<std::wstring> DllEditor::SerializeState() const {
    std::vector<std::wstring> state;
    
    // 序列化版本号
    state.push_back(std::to_wstring(m_version));
    
    // 序列化命令数量
    state.push_back(std::to_wstring(m_commands.size()));
    
    // 序列化每个命令
    for (const auto& cmd : m_commands) {
        state.push_back(cmd.commandName);
        state.push_back(cmd.returnType);
        state.push_back(cmd.dllFileName);
        state.push_back(cmd.dllCommandName);
        state.push_back(cmd.isPublic ? L"1" : L"0");
        state.push_back(cmd.comment);
        
        // 序列化参数数量
        state.push_back(std::to_wstring(cmd.paramLines.size()));
        
        // 序列化参数索引
        for (int paramIdx : cmd.paramLines) {
            state.push_back(std::to_wstring(paramIdx));
        }
    }
    
    // 序列化参数总数
    state.push_back(std::to_wstring(m_allParams.size()));
    
    // 序列化每个参数
    for (const auto& param : m_allParams) {
        state.push_back(param.paramName);
        state.push_back(param.paramType);
        state.push_back(param.byRef ? L"1" : L"0");
        state.push_back(param.isArray ? L"1" : L"0");
        state.push_back(param.isOptional ? L"1" : L"0");
        state.push_back(param.comment);
    }
    
    return state;
}

void DllEditor::RestoreState(const std::vector<std::wstring>& state) {
    if (state.empty()) return;
    
    size_t index = 0;
    
    // 恢复版本号
    if (index < state.size()) {
        m_version = std::stoi(state[index++]);
    }
    
    // 恢复命令
    m_commands.clear();
    if (index < state.size()) {
        int cmdCount = std::stoi(state[index++]);
        
        for (int i = 0; i < cmdCount && index < state.size(); i++) {
            DllCommand cmd;
            cmd.commandName = state[index++];
            cmd.returnType = state[index++];
            cmd.dllFileName = state[index++];
            cmd.dllCommandName = state[index++];
            cmd.isPublic = (state[index++] == L"1");
            cmd.comment = state[index++];
            
            int paramCount = std::stoi(state[index++]);
            for (int j = 0; j < paramCount && index < state.size(); j++) {
                cmd.paramLines.push_back(std::stoi(state[index++]));
            }
            
            m_commands.push_back(cmd);
        }
    }
    
    // 恢复参数
    m_allParams.clear();
    if (index < state.size()) {
        int paramCount = std::stoi(state[index++]);
        
        for (int i = 0; i < paramCount && index < state.size(); i++) {
            DllParameter param;
            param.paramName = state[index++];
            param.paramType = state[index++];
            param.byRef = (state[index++] == L"1");
            param.isArray = (state[index++] == L"1");
            param.isOptional = (state[index++] == L"1");
            param.comment = state[index++];
            
            m_allParams.push_back(param);
        }
    }
}

void DllEditor::ParseContent(const std::vector<std::wstring>& lines) {
    wchar_t debugMsg[512];
    swprintf_s(debugMsg, L"[DllEditor::ParseContent] 开始解析，总行数: %zu\n", lines.size());
    OutputDebugStringW(debugMsg);
    
    // 输出前10行用于调试
    for (size_t i = 0; i < (std::min)(lines.size(), (size_t)10); i++) {
        swprintf_s(debugMsg, L"[DllEditor] 第%zu行: %s\n", i + 1, lines[i].c_str());
        OutputDebugStringW(debugMsg);
    }
    
    m_commands.clear();
    m_allParams.clear();
    
    DllCommand* currentCommand = nullptr;
    
    for (const auto& line : lines) {
        // 跳过空行
        if (line.empty()) continue;
        
        // 解析版本号
        if (line.find(L".版本") == 0) {
            size_t spacePos = line.find(L' ');
            if (spacePos != std::wstring::npos) {
                m_version = std::stoi(line.substr(spacePos + 1));
            }
            continue;
        }
        
        // 解析DLL命令
        if (line.find(L".DLL命令") == 0) {
            m_commands.push_back(DllCommand());
            currentCommand = &m_commands.back();
            
            swprintf_s(debugMsg, L"[DllEditor::ParseContent] 解析到DLL命令: %s\n", line.c_str());
            OutputDebugStringW(debugMsg);
            
            size_t pos = line.find(L' ');
            if (pos != std::wstring::npos) {
                std::wstring params = line.substr(pos + 1);
                
                // 按逗号分割参数
                std::vector<std::wstring> parts;
                std::wstring current;
                bool inQuote = false;
                
                for (wchar_t ch : params) {
                    if (ch == L'\"') {
                        inQuote = !inQuote;
                    } else if (ch == L',' && !inQuote) {
                        // 去除首尾空格
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
                // 添加最后一个部分
                size_t start = current.find_first_not_of(L" \t");
                size_t end = current.find_last_not_of(L" \t");
                if (start != std::wstring::npos) {
                    parts.push_back(current.substr(start, end - start + 1));
                } else {
                    parts.push_back(L"");
                }
                
                // 解析各字段
                if (parts.size() >= 1) currentCommand->commandName = parts[0];
                if (parts.size() >= 2) currentCommand->returnType = parts[1];
                if (parts.size() >= 3) {
                    // 去除引号
                    currentCommand->dllFileName = parts[2];
                    if (!currentCommand->dllFileName.empty() && 
                        currentCommand->dllFileName.front() == L'\"' && 
                        currentCommand->dllFileName.back() == L'\"') {
                        currentCommand->dllFileName = currentCommand->dllFileName.substr(
                            1, currentCommand->dllFileName.length() - 2);
                    }
                }
                if (parts.size() >= 4) {
                    currentCommand->dllCommandName = parts[3];
                    if (!currentCommand->dllCommandName.empty() && 
                        currentCommand->dllCommandName.front() == L'\"' && 
                        currentCommand->dllCommandName.back() == L'\"') {
                        currentCommand->dllCommandName = currentCommand->dllCommandName.substr(
                            1, currentCommand->dllCommandName.length() - 2);
                    }
                }
                if (parts.size() >= 5) {
                    currentCommand->isPublic = (parts[4] == L"公开");
                }
                if (parts.size() >= 6) {
                    currentCommand->comment = parts[5];
                }
            }
            continue;
        }
        
        // 解析参数
        if (line.find(L"    .参数") == 0 || line.find(L"\t.参数") == 0) {
            if (!currentCommand) continue;
            
            DllParameter param;
            // 初始化标志为false
            param.byRef = false;
            param.isArray = false;
            param.isOptional = false;
            
            size_t pos = line.find(L".参数");
            if (pos != std::wstring::npos) {
                pos += 4; // 跳过 ".参数"
                std::wstring paramStr = line.substr(pos);
                
                // 按逗号分割
                std::vector<std::wstring> parts;
                std::wstring current;
                
                for (wchar_t ch : paramStr) {
                    if (ch == L',') {
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
                // 添加最后一个部分
                size_t start = current.find_first_not_of(L" \t");
                size_t end = current.find_last_not_of(L" \t");
                if (start != std::wstring::npos) {
                    parts.push_back(current.substr(start, end - start + 1));
                } else if (!current.empty()) {
                    parts.push_back(L"");
                }
                
                // 解析字段
                if (parts.size() >= 1) param.paramName = parts[0];
                if (parts.size() >= 2) param.paramType = parts[1];
                
                // 解析标志和备注
                // 第3个字段可能包含"传址"、"数组"或两者，用空格分隔
                if (parts.size() >= 3) {
                    std::wstring flagsField = parts[2];
                    if (flagsField.find(L"传址") != std::wstring::npos || flagsField.find(L"参考") != std::wstring::npos) {
                        param.byRef = true;
                    }
                    if (flagsField.find(L"数组") != std::wstring::npos) {
                        param.isArray = true;
                    }
                }
                
                // 第4个字段是备注
                if (parts.size() >= 4) {
                    param.comment = parts[3];
                }
            }
            
            // 添加到参数列表
            m_allParams.push_back(param);
            currentCommand->paramLines.push_back((int)m_allParams.size() - 1);
        }
    }
    
    wchar_t finalMsg[256];
    swprintf_s(finalMsg, L"[DllEditor::ParseContent] 解析完成 - 命令数: %zu, 参数总数: %zu\n", 
               m_commands.size(), m_allParams.size());
    OutputDebugStringW(finalMsg);
}

std::vector<std::wstring> DllEditor::GenerateContent() const {
    std::vector<std::wstring> lines;
    
    // 生成版本行
    lines.push_back(L".版本 " + std::to_wstring(m_version));
    lines.push_back(L"");
    
    // 生成每个命令
    for (const auto& cmd : m_commands) {
        std::wstring cmdLine = L".DLL命令 ";
        cmdLine += cmd.commandName + L", ";
        cmdLine += cmd.returnType + L", ";
        cmdLine += L"\"" + cmd.dllFileName + L"\", ";
        cmdLine += L"\"" + cmd.dllCommandName + L"\", ";
        cmdLine += (cmd.isPublic ? L"公开" : L"私有");
        if (!cmd.comment.empty()) {
            cmdLine += L", " + cmd.comment;
        }
        lines.push_back(cmdLine);
        
        // 生成参数
        for (int paramIdx : cmd.paramLines) {
            if (paramIdx < (int)m_allParams.size()) {
                const auto& param = m_allParams[paramIdx];
                std::wstring paramLine = L"    .参数 " + param.paramName + L", " + param.paramType + L", ";
                
                // 第3个字段：传址和数组标记（用空格分隔）
                std::wstring flags;
                if (param.byRef) {
                    flags += L"传址";
                }
                if (param.isArray) {
                    if (!flags.empty()) flags += L" ";
                    flags += L"数组";
                }
                paramLine += flags;
                
                // 第4个字段：备注
                if (!param.comment.empty()) {
                    paramLine += L", " + param.comment;
                }
                
                lines.push_back(paramLine);
            }
        }
        
        lines.push_back(L""); // 空行分隔命令
    }
    
    return lines;
}

void DllEditor::InsertRow(int afterRow) {
    int cmdIndex, paramIndex;
    DllRowType rowType = GetRowType(afterRow, cmdIndex, paramIndex);
    
    if (rowType == DllRowType::ParamData) {
        // 在参数后插入新参数
        InsertParameter(cmdIndex, paramIndex);
    } else {
        // 在命令后插入新命令
        InsertCommand(cmdIndex);
    }
    
    CreateSnapshot(L"Insert Row");
}

void DllEditor::DeleteRow(int row) {
    int cmdIndex, paramIndex;
    DllRowType rowType = GetRowType(row, cmdIndex, paramIndex);
    
    if (cmdIndex < 0 || cmdIndex >= (int)m_commands.size()) {
        return;
    }
    
    if (rowType == DllRowType::ParamData) {
        // 删除参数
        DeleteParameter(cmdIndex, paramIndex);
    } else if (rowType == DllRowType::CommandData) {
        // 删除命令
        DeleteCommand(cmdIndex);
    }
    
    CreateSnapshot(L"Delete Row");
}

void DllEditor::CopyRow(int row) {
    // TODO: 实现复制功能
}

void DllEditor::PasteRow(int afterRow) {
    // TODO: 实现粘贴功能
}

// === Protected 方法 ===

std::wstring* DllEditor::GetEditingCellTextPtr() {
    if (!m_isEditing) return nullptr;
    
    int subRow;
    int cmdIndex = GetCommandIndexFromRow(m_editingRow, subRow);
    
    if (cmdIndex < 0 || cmdIndex >= (int)m_commands.size()) {
        return nullptr;
    }
    
    // 直接返回编辑缓冲区的指针（在StartEditCell时已初始化）
    return &m_editBuffer;
}

void DllEditor::OnTextModified() {
    // 标记文件为已修改
    if (!m_modified) {
        m_modified = true;
        
        // 通知父窗口文件已被修改
        HWND hParent = GetParent(m_hWnd);
        if (hParent) {
            SendMessage(hParent, WM_COMMAND, MAKEWPARAM(0, 0x1000), (LPARAM)m_hWnd);
        }
    }
}

void DllEditor::StartEditCell(int row, int col, int clickX) {
    m_isEditing = true;
    m_editingRow = row;
    m_editingCol = col;
    m_selectionStart = 0;
    m_selectionEnd = 0;
    m_cursorVisible = true;  // 开始编辑时确保光标可见
    
    // 确保窗口获取键盘焦点
    SetFocus(m_hWnd);
    
    // 初始化编辑缓冲区
    m_editBuffer = GetCellText(row, col);
    
    // 根据点击位置计算光标位置
    if (clickX >= 0 && !m_editBuffer.empty()) {
        // clickX现在是相对于单元格左边的坐标
        // 需要减去padding得到相对于文本起始的坐标
        const int colPadding = 5;
        int relativeClickX = clickX - colPadding;
        
        // 如果相对坐标为负，说明点击在padding区域，光标放在开头
        if (relativeClickX <= 0) {
            m_editingCursorPos = 0;
        } else {
            // 使用GDI+测量文本宽度来找到最接近的字符边界
            HDC hdc = GetDC(m_hWnd);
            Graphics graphics(hdc);
            FontFamily fontFamily(L"Microsoft YaHei");
            Font font(&fontFamily, (REAL)m_fontSize, FontStyleRegular, UnitPixel);
            
            // 使用与绘制时相同的 leftFormat
            StringFormat format;
            format.SetAlignment(StringAlignmentNear);
            format.SetLineAlignment(StringAlignmentCenter);
            format.SetFormatFlags(StringFormatFlagsMeasureTrailingSpaces | StringFormatFlagsNoWrap);
            
            // 先测量整个文本的宽度
            RectF totalBox;
            graphics.MeasureString(m_editBuffer.c_str(), -1, &font, PointF(0, 0), &format, &totalBox);
            
            // 如果点击在文本末尾之后，直接放在末尾
            if (relativeClickX >= totalBox.Width) {
                m_editingCursorPos = (int)m_editBuffer.length();
            } else {
                // 找到点击位置所在的字符
                int bestPos = 0;
                
                for (size_t i = 0; i < m_editBuffer.length(); i++) {
                    // 测量字符i左边界的位置（前i个字符的整体宽度）
                    float leftEdge = 0;
                    if (i > 0) {
                        std::wstring leftSubstr = m_editBuffer.substr(0, i);
                        RectF leftBox;
                        graphics.MeasureString(leftSubstr.c_str(), -1, &font, PointF(0, 0), &format, &leftBox);
                        leftEdge = leftBox.Width;
                    }
                    
                    // 测量字符i右边界的位置（前i+1个字符的整体宽度）
                    std::wstring rightSubstr = m_editBuffer.substr(0, i + 1);
                    RectF rightBox;
                    graphics.MeasureString(rightSubstr.c_str(), -1, &font, PointF(0, 0), &format, &rightBox);
                    float rightEdge = rightBox.Width;
                    
                    // 如果点击位置在这个字符范围内
                    if (relativeClickX >= leftEdge && relativeClickX < rightEdge) {
                        // 计算字符的中点
                        float midPoint = (leftEdge + rightEdge) / 2.0f;
                        
                        // 如果点击位置在字符左半部分，光标在字符左边；否则在字符右边
                        if (relativeClickX < midPoint) {
                            bestPos = (int)i;
                        } else {
                            bestPos = (int)i + 1;
                        }
                        break;
                    }
                }
                
                m_editingCursorPos = bestPos;
            }
            
            ReleaseDC(m_hWnd, hdc);
        }
    } else {
        // 如果没有点击位置信息，默认放在文本末尾
        m_editingCursorPos = (int)m_editBuffer.length();
    }
    
    InvalidateRect(m_hWnd, NULL, FALSE);
}

// === Private 辅助方法 ===

DllRowType DllEditor::GetRowType(int row, int& cmdIndex, int& paramIndex) const {
    cmdIndex = -1;
    paramIndex = -1;
    
    int currentRow = 0;
    
    for (size_t cmdIdx = 0; cmdIdx < m_commands.size(); cmdIdx++) {
        const DllCommand& cmd = m_commands[cmdIdx];
        int cmdRowCount = 7 + (int)cmd.paramLines.size();  // 7行固定行 + 参数行
        
        if (row < currentRow + cmdRowCount) {
            cmdIndex = (int)cmdIdx;
            int subRow = row - currentRow;
            
            // 判断是哪一行
            if (subRow == 0) return DllRowType::CommandHeader;        // 第1行：命令表头
            if (subRow == 1) return DllRowType::CommandData;          // 第2行：命令数据
            if (subRow == 2) return DllRowType::LibFileHeader;        // 第3行：库文件名表头
            if (subRow == 3) return DllRowType::LibFileData;          // 第4行：库文件名数据
            if (subRow == 4) return DllRowType::DllNameHeader;        // 第5行：DLL命令名表头
            if (subRow == 5) return DllRowType::DllNameData;          // 第6行：DLL命令名数据
            if (subRow == 6) return DllRowType::ParamHeader;          // 第7行：参数表头
            
            // 第8行及以后：参数数据
            paramIndex = subRow - 7;
            return DllRowType::ParamData;
        }
        
        currentRow += cmdRowCount;
        
        // 检查是否是间隔行（最后一个命令除外）
        if (cmdIdx < m_commands.size() - 1) {
            if (row == currentRow) {
                cmdIndex = (int)cmdIdx;
                return DllRowType::Separator;
            }
            currentRow += 1;  // 跳过间隔行
        }
    }
    
    return DllRowType::Separator;  // 默认返回间隔行
}

int DllEditor::GetCommandIndexFromRow(int row, int& subRow) const {
    int currentRow = 0;
    
    for (size_t cmdIdx = 0; cmdIdx < m_commands.size(); cmdIdx++) {
        const DllCommand& cmd = m_commands[cmdIdx];
        
        // 如果命令被折叠，只有1行
        int cmdRowCount;
        if (cmd.isCollapsed) {
            cmdRowCount = 1;
        } else {
            cmdRowCount = 7 + (int)cmd.paramLines.size();
        }
        
        if (row < currentRow + cmdRowCount) {
            subRow = row - currentRow;
            return (int)cmdIdx;
        }
        
        currentRow += cmdRowCount;
        
        // 跳过间隔行（最后一个命令除外）
        if (cmdIdx < m_commands.size() - 1) {
            if (row == currentRow) {
                // 这是间隔行
                subRow = -1;
                return (int)cmdIdx;
            }
            currentRow += 1;
        }
    }
    
    subRow = 0;
    return -1;
}

bool DllEditor::IsCommandRow(int row) const {
    int subRow;
    GetCommandIndexFromRow(row, subRow);
    return subRow == 0;
}

// 根据命令索引和子行号计算全局行号
int DllEditor::GetGlobalRowFromCmdAndSubRow(int cmdIndex, int subRowInCmd) const {
    int globalRow = 0;
    
    for (int i = 0; i < cmdIndex; i++) {
        int cmdRowCount = 7 + (int)m_commands[i].paramLines.size();
        globalRow += cmdRowCount;
        
        // 添加间隔行（最后一个命令除外）
        if (i < (int)m_commands.size() - 1) {
            globalRow += 1;
        }
    }
    
    globalRow += subRowInCmd;
    return globalRow;
}

bool DllEditor::IsParamRow(int row, int& cmdIndex, int& paramIndex) const {
    int subRow;
    cmdIndex = GetCommandIndexFromRow(row, subRow);
    
    if (cmdIndex >= 0 && subRow > 0) {
        paramIndex = subRow - 1;
        return true;
    }
    
    return false;
}

void DllEditor::InsertCommand(int afterCmdIndex) {
    DllCommand newCmd;
    newCmd.commandName = L"新命令";
    newCmd.returnType = L"整数型";
    newCmd.dllFileName = L"";
    newCmd.dllCommandName = L"";
    newCmd.isPublic = true;
    newCmd.comment = L"";
    
    if (afterCmdIndex < 0) {
        m_commands.insert(m_commands.begin(), newCmd);
    } else if (afterCmdIndex >= (int)m_commands.size()) {
        m_commands.push_back(newCmd);
    } else {
        m_commands.insert(m_commands.begin() + afterCmdIndex + 1, newCmd);
    }
    
    m_modified = true;
}

void DllEditor::DeleteCommand(int cmdIndex) {
    if (cmdIndex >= 0 && cmdIndex < (int)m_commands.size()) {
        // 删除关联的参数索引（不删除参数本身，避免索引混乱）
        m_commands.erase(m_commands.begin() + cmdIndex);
        m_modified = true;
    }
}

void DllEditor::InsertParameter(int cmdIndex, int afterParamIndex) {
    if (cmdIndex < 0 || cmdIndex >= (int)m_commands.size()) {
        return;
    }
    
    DllParameter newParam;
    newParam.paramName = L"";
    newParam.paramType = L"整数型";
    newParam.byRef = false;
    newParam.isArray = false;
    newParam.isOptional = false;
    newParam.comment = L"";
    
    // 添加到全局参数列表
    m_allParams.push_back(newParam);
    int newParamIdx = (int)m_allParams.size() - 1;
    
    // 添加到命令的参数列表
    DllCommand& cmd = m_commands[cmdIndex];
    if (afterParamIndex < 0) {
        cmd.paramLines.insert(cmd.paramLines.begin(), newParamIdx);
    } else if (afterParamIndex >= (int)cmd.paramLines.size()) {
        cmd.paramLines.push_back(newParamIdx);
    } else {
        cmd.paramLines.insert(cmd.paramLines.begin() + afterParamIndex + 1, newParamIdx);
    }
    
    m_modified = true;
}

void DllEditor::DeleteParameter(int cmdIndex, int paramIndex) {
    if (cmdIndex < 0 || cmdIndex >= (int)m_commands.size()) {
        return;
    }
    
    DllCommand& cmd = m_commands[cmdIndex];
    if (paramIndex >= 0 && paramIndex < (int)cmd.paramLines.size()) {
        cmd.paramLines.erase(cmd.paramLines.begin() + paramIndex);
        m_modified = true;
    }
}

// === 绘制方法重写 ===

void DllEditor::DrawTable(HDC hdc, const RECT& clientRect) {
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
    
    if (m_commands.empty()) {
        return;
    }
    
    // 绘制行号
    FontFamily fontFamily(L"Microsoft YaHei");
    Font lineNumFont(&fontFamily, (REAL)(m_fontSize - 2), FontStyleRegular, UnitPixel);
    SolidBrush lineNumBrush(Color(150, GetRValue(g_CurrentTheme.text), 
                                   GetGValue(g_CurrentTheme.text), 
                                   GetBValue(g_CurrentTheme.text)));
    StringFormat rightFormat;
    rightFormat.SetAlignment(StringAlignmentFar);
    rightFormat.SetLineAlignment(StringAlignmentCenter);
    
    // 不绘制全局表头，每个DLL命令有自己的表头
    int yPos = 0 - m_scrollY;
    const int separatorHeight = 20;
    int dataLineNumber = 1;  // 数据行号（只计算数据行，跳过表头）
    
    // 绘制每个命令的独立表格
    for (size_t cmdIdx = 0; cmdIdx < m_commands.size(); cmdIdx++) {
        const DllCommand& cmd = m_commands[cmdIdx];
        int fullRowCount = 7 + (int)cmd.paramLines.size();  // 完整的行数（数据层面）
        // 数据行数：命令数据(1) + 库文件名数据(1) + DLL命令名数据(1) + 参数数据(N) = 3 + N
        int dataRowsInCmd = 3 + (int)cmd.paramLines.size();
        
        // 根据折叠状态决定显示的行数
        int visibleRows;
        int tableHeight;
        if (cmd.isCollapsed) {
            visibleRows = 2;  // 折叠时只显示2行（命令表头+命令数据）
            tableHeight = 2 * m_rowHeight;
        } else {
            visibleRows = fullRowCount;
            tableHeight = visibleRows * m_rowHeight;
        }
        
        // 记录这个命令的起始数据行号
        int cmdStartDataLine = dataLineNumber;
        
        // 绘制这个表格的行号和折叠符号
        for (int rowInTable = 0; rowInTable < visibleRows; rowInTable++) {
            int rowY = yPos + rowInTable * m_rowHeight;
            
            // 判断是否是数据行（subRow=1,3,5,7+是数据行，0,2,4,6是表头）
            bool isDataRow = (rowInTable == 1 || rowInTable == 3 || rowInTable == 5 || rowInTable >= 7);
            
            // 只绘制可见区域的行号
            if (rowY + m_rowHeight >= 0 && rowY < clientRect.bottom) {
                // 只在数据行显示行号
                if (isDataRow) {
                    // 计算这个数据行的行号
                    int lineNum;
                    if (rowInTable == 1) lineNum = cmdStartDataLine;      // 命令数据
                    else if (rowInTable == 3) lineNum = cmdStartDataLine + 1;  // 库文件名数据
                    else if (rowInTable == 5) lineNum = cmdStartDataLine + 2;  // DLL命令名数据
                    else lineNum = cmdStartDataLine + 3 + (rowInTable - 7);    // 参数数据
                    
                    std::wstring lineNumText = std::to_wstring(lineNum);
                    RectF lineNumTextRect((REAL)5, (REAL)rowY, 
                                          (REAL)(m_leftMarginWidth - 25), (REAL)m_rowHeight);
                    StringFormat rightAlignFormat;
                    rightAlignFormat.SetAlignment(StringAlignmentFar);
                    rightAlignFormat.SetLineAlignment(StringAlignmentCenter);
                    graphics.DrawString(lineNumText.c_str(), -1, &lineNumFont, 
                                      lineNumTextRect, &rightAlignFormat, &lineNumBrush);
                }
                
                // 在第一行绘制折叠符号（放在右边）
                if (rowInTable == 0) {
                    int foldBtnSize = 12;
                    int foldBtnX = m_leftMarginWidth - foldBtnSize - 8;  // 距离右边缘8像素
                    int foldBtnY = rowY + (m_rowHeight - foldBtnSize) / 2;
                    
                    Pen foldPen(ColorFromCOLORREF(g_CurrentTheme.text), 1);
                    graphics.DrawRectangle(&foldPen, foldBtnX, foldBtnY, foldBtnSize, foldBtnSize);
                    
                    // 绘制横线（-）
                    graphics.DrawLine(&foldPen, foldBtnX + 3, foldBtnY + foldBtnSize / 2, 
                                    foldBtnX + foldBtnSize - 3, foldBtnY + foldBtnSize / 2);
                    
                    // 如果是折叠状态，绘制竖线形成（+）
                    if (cmd.isCollapsed) {
                        graphics.DrawLine(&foldPen, foldBtnX + foldBtnSize / 2, foldBtnY + 3, 
                                        foldBtnX + foldBtnSize / 2, foldBtnY + foldBtnSize - 3);
                    }
                }
            }
        }
        
        // 绘制表格内容
        if (yPos + tableHeight >= 0 && yPos < clientRect.bottom) {
            DrawCommandTable(hdc, (int)cmdIdx, yPos, clientRect);
        }
        
        yPos += tableHeight;
        
        // 更新数据行号（只计算数据行：命令数据+库文件名数据+DLL命令名数据+参数数据）
        dataLineNumber += dataRowsInCmd;
        
        // 添加表格间隔（最后一个表格除外）
        if (cmdIdx < m_commands.size() - 1) {
            yPos += separatorHeight;
            // 间隔行不计入数据行号
        }
    }
}

void DllEditor::DrawCommandTable(HDC hdc, int cmdIndex, int& yPos, const RECT& clientRect) {
    if (cmdIndex < 0 || cmdIndex >= (int)m_commands.size()) {
        return;
    }
    
    const DllCommand& cmd = m_commands[cmdIndex];
    Graphics graphics(hdc);
    
    // 设置裁剪区域：表格内容只在行号区域之外绘制
    // 这样当表格向左滚动时，会被行号区域遮挡
    Region clipRegion(Rect(m_leftMarginWidth, 0, clientRect.right - m_leftMarginWidth, clientRect.bottom));
    graphics.SetClip(&clipRegion);
    
    // 表格开始Y坐标
    int tableStartY = yPos;
    
    // 计算这个命令表格的总行数（根据折叠状态）
    int totalRows;
    int tableHeight;
    if (cmd.isCollapsed) {
        totalRows = 2;  // 折叠时只显示2行（命令表头+命令数据）
        tableHeight = 2 * m_rowHeight;
    } else {
        totalRows = 7 + (int)cmd.paramLines.size();
        tableHeight = totalRows * m_rowHeight;
    }
    
    // 如果表格完全不可见，直接返回
    if (tableStartY > clientRect.bottom || tableStartY + tableHeight < 0) {
        return;
    }
    
    Pen gridPen(ColorFromCOLORREF(g_CurrentTheme.grid), 2);  // 使用稍粗的边框
    SolidBrush textBrush(ColorFromCOLORREF(g_CurrentTheme.text));
    SolidBrush commentBrush(Color(255, 0, 200, 0));  // 备注列：绿色
    SolidBrush typeBrush(Color(255, 174, 144, 194));  // 类型列：rgb(174, 144, 194)
    SolidBrush libInfoBrush(Color(255, 238, 121, 121));  // 库文件名和DLL命令名：rgb(238, 121, 121)
    SolidBrush paramNameBrush(Color(255, 153, 216, 249));  // 参数名：rgb(153, 216, 249)
    SolidBrush cmdNameBrush(Color(255, 220, 220, 170));  // DLL命令名：rgb(220, 220, 170)
    SolidBrush cmdHeaderBrush(Color(255, 58, 65, 81));  // 命令表头背景色
    SolidBrush paramHeaderBrush(Color(255, 56, 56, 48));  // 参数表头背景色
    SolidBrush bgBrush(Color(255, 31, 31, 31));  // 编辑器背景色
    SolidBrush rowSelectBrush(Color(100, 51, 153, 255));  // 行选择高亮背景色（半透明蓝色）
    FontFamily fontFamily(L"Microsoft YaHei");
    Font headerFont(&fontFamily, (REAL)m_fontSize, FontStyleBold, UnitPixel);
    Font dataFont(&fontFamily, (REAL)m_fontSize, FontStyleRegular, UnitPixel);
    StringFormat centerFormat;
    centerFormat.SetAlignment(StringAlignmentCenter);
    centerFormat.SetLineAlignment(StringAlignmentCenter);
    // 使用GenericTypographic格式确保绘制和测量一致
    StringFormat leftFormat(StringFormat::GenericTypographic());
    leftFormat.SetAlignment(StringAlignmentNear);
    leftFormat.SetLineAlignment(StringAlignmentCenter);
    leftFormat.SetFormatFlags(StringFormatFlagsMeasureTrailingSpaces);
    
    int currentRow = 0;
    int colPadding = 5;
    int currentY = tableStartY;
    
    // 动态计算列宽 - 根据内容自适应（包括正在编辑的内容）
    RectF measureRect;
    
    // 判断命令数据行是否正在编辑
    int cmdDataRow = GetGlobalRowFromCmdAndSubRow(cmdIndex, 1);  // 命令数据行
    bool isEditingCmdName = (m_isEditing && cmdDataRow == m_editingRow && m_editingCol == 0);
    bool isEditingReturnType = (m_isEditing && cmdDataRow == m_editingRow && m_editingCol == 1);
    bool isEditingCmdComment = (m_isEditing && cmdDataRow == m_editingRow && m_editingCol == 3);
    
    // 测量DLL命令名宽度（如果正在编辑，使用m_editBuffer）
    const std::wstring& cmdNameText = isEditingCmdName ? m_editBuffer : cmd.commandName;
    graphics.MeasureString(cmdNameText.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
    int cmdNameWidth = (std::max)(120, (int)measureRect.Width + colPadding * 2 + 10);  // 最小120px
    
    // 测量返回值类型宽度（如果正在编辑，使用m_editBuffer）
    const std::wstring& returnTypeText = isEditingReturnType ? m_editBuffer : cmd.returnType;
    graphics.MeasureString(returnTypeText.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
    int returnTypeWidth = (std::max)(80, (int)measureRect.Width + colPadding * 2 + 10);  // 最小80px
    
    // 测量备注宽度（如果正在编辑，使用m_editBuffer）
    const std::wstring& commentText = isEditingCmdComment ? m_editBuffer : cmd.comment;
    graphics.MeasureString(commentText.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
    int commentWidth = (std::max)(120, (int)measureRect.Width + colPadding * 2 + 10);  // 最小120px
    
    // 判断库文件名和DLL命令名是否正在编辑
    int libFileRow = GetGlobalRowFromCmdAndSubRow(cmdIndex, 3);
    int dllCmdRow = GetGlobalRowFromCmdAndSubRow(cmdIndex, 5);
    bool isEditingLibFile = (m_isEditing && libFileRow == m_editingRow && m_editingCol == 0);
    bool isEditingDllCmd = (m_isEditing && dllCmdRow == m_editingRow && m_editingCol == 0);
    
    // 测量库文件名和DLL命令名宽度（如果正在编辑，使用m_editBuffer）
    const std::wstring& dllFileText = isEditingLibFile ? m_editBuffer : cmd.dllFileName;
    graphics.MeasureString(dllFileText.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
    int dllFileWidth = (int)measureRect.Width + colPadding * 2 + 10;
    
    const std::wstring& dllCmdText = isEditingDllCmd ? m_editBuffer : cmd.dllCommandName;
    graphics.MeasureString(dllCmdText.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
    int dllCmdWidth = (int)measureRect.Width + colPadding * 2 + 10;
    
    // 测量所有参数的宽度（包括正在编辑的参数）
    int maxParamNameWidth = 120;  // 最小宽度
    int maxParamTypeWidth = 80;
    int maxParamCommentWidth = 120;
    
    int paramRowIndex = 0;
    for (int paramIdx : cmd.paramLines) {
        if (paramIdx >= 0 && paramIdx < (int)m_allParams.size()) {
            const DllParameter& param = m_allParams[paramIdx];
            
            // 判断当前参数行是否正在编辑
            int paramGlobalRow = GetGlobalRowFromCmdAndSubRow(cmdIndex, 7 + paramRowIndex);
            bool isEditingParamName = (m_isEditing && paramGlobalRow == m_editingRow && m_editingCol == 0);
            bool isEditingParamType = (m_isEditing && paramGlobalRow == m_editingRow && m_editingCol == 1);
            bool isEditingParamComment = (m_isEditing && paramGlobalRow == m_editingRow && m_editingCol == 4);
            
            // 使用编辑内容或原始内容
            const std::wstring& paramNameText = isEditingParamName ? m_editBuffer : param.paramName;
            const std::wstring& paramTypeText = isEditingParamType ? m_editBuffer : param.paramType;
            const std::wstring& paramCommentText = isEditingParamComment ? m_editBuffer : param.comment;
            
            graphics.MeasureString(paramNameText.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
            maxParamNameWidth = (std::max)(maxParamNameWidth, (int)measureRect.Width + colPadding * 2 + 10);
            
            graphics.MeasureString(paramTypeText.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
            maxParamTypeWidth = (std::max)(maxParamTypeWidth, (int)measureRect.Width + colPadding * 2 + 10);
            
            graphics.MeasureString(paramCommentText.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
            maxParamCommentWidth = (std::max)(maxParamCommentWidth, (int)measureRect.Width + colPadding * 2 + 10);
            
            paramRowIndex++;
        }
    }
    
    // 计算对齐后的列宽
    // 第1列：DLL命令名 vs 参数名，取最大值
    int col1Width = (std::max)(cmdNameWidth, maxParamNameWidth);
    // 第2列：返回值类型 vs 类型，取最大值
    int col2Width = (std::max)(returnTypeWidth, maxParamTypeWidth);
    // 第3列：公开/传址，固定40
    int col3Width = 40;
    // 第4列：命令备注 vs (参数数组40 + 参数备注)，取最大值
    int col4Width = (std::max)(commentWidth, 40 + maxParamCommentWidth);
    
    // 构建命令列定义（使用对齐后的宽度）
    std::vector<ColumnDef> cmdCols;
    cmdCols.push_back(ColumnDef(L"DLL命令名", col1Width));
    cmdCols.push_back(ColumnDef(L"返回值类型", col2Width));
    cmdCols.push_back(ColumnDef(L"公开", col3Width));
    cmdCols.push_back(ColumnDef(L"备注", col4Width));
    
    // 构建参数列定义（使用对齐后的宽度）
    std::vector<ColumnDef> paramCols;
    paramCols.push_back(ColumnDef(L"参数名", col1Width));
    paramCols.push_back(ColumnDef(L"类型", col2Width));
    paramCols.push_back(ColumnDef(L"传址", col3Width));
    paramCols.push_back(ColumnDef(L"数组", 40));  // 固定40
    paramCols.push_back(ColumnDef(L"备注", col4Width - 40));  // 总第4列宽度 - 数组列宽度
    
    int cmdWidth = 0;
    for (const auto& col : cmdCols) {
        cmdWidth += col.width;
    }
    
    int paramWidth = 0;
    for (const auto& col : paramCols) {
        paramWidth += col.width;
    }
    
    // 还要考虑整行内容的宽度（库文件名、DLL命令名）
    int fullRowWidth = (std::max)(dllFileWidth, dllCmdWidth);
    
    // 使用最大宽度作为表格宽度（命令行和参数行现在宽度相同，都是col1+col2+col3+col4）
    int tableWidth = (std::max)(cmdWidth, fullRowWidth);
    
    // 表格左偏移（在行号区域右侧留出10像素空白）
    const int tableLeftOffset = 10;
    int tableStartX = m_leftMarginWidth + tableLeftOffset - m_scrollX;
    
    // 绘制表格外边框（从左边距之后开始）
    graphics.DrawRectangle(&gridPen, tableStartX, tableStartY, tableWidth, tableHeight);
    
    // 第1-2行：DLL命令名、返回值类型、公开、备注
    
    // 绘制命令表头（第1行）- subRow=0
    int cmdHeaderGlobalRow = GetGlobalRowFromCmdAndSubRow(cmdIndex, 0);
    int x = tableStartX;
    for (size_t i = 0; i < cmdCols.size(); i++) {
        RectF cellRect((REAL)x, (REAL)currentY, (REAL)cmdCols[i].width, (REAL)m_rowHeight);
        graphics.FillRectangle(&cmdHeaderBrush, cellRect);
        // 表头左对齐，添加左边距
        int headerTextX = x + colPadding;
        int headerTextY = currentY + m_rowHeight / 2;
        PointF headerPos((REAL)headerTextX, (REAL)headerTextY);
        graphics.DrawString(cmdCols[i].name.c_str(), -1, &headerFont, headerPos, &leftFormat, &textBrush);
        // 只在列之间绘制垂直线，不在最后一列后面
        if (i < cmdCols.size() - 1) {
            graphics.DrawLine(&gridPen, x + cmdCols[i].width, currentY, x + cmdCols[i].width, currentY + m_rowHeight);
        }
        x += cmdCols[i].width;
    }
    // 填充右侧空白区域（如果参数列更宽）
    if (x < tableStartX + tableWidth) {
        RectF fillRect((REAL)x, (REAL)currentY, (REAL)(tableWidth - cmdWidth), (REAL)m_rowHeight);
        graphics.FillRectangle(&cmdHeaderBrush, fillRect);
    }
    // 绘制行选中高亮（第1行）
    if (IsRowInSelection(cmdHeaderGlobalRow)) {
        RectF selectRect((REAL)tableStartX, (REAL)currentY, (REAL)tableWidth, (REAL)m_rowHeight);
        graphics.FillRectangle(&rowSelectBrush, selectRect);
    }
    graphics.DrawLine(&gridPen, tableStartX, currentY + m_rowHeight, tableStartX + tableWidth, currentY + m_rowHeight);
    currentY += m_rowHeight;
    
    // 绘制命令数据（第2行）
    x = tableStartX;
    std::vector<std::wstring> cmdData = {cmd.commandName, cmd.returnType, 
                                         cmd.isPublic ? L"✓" : L"", cmd.comment};
    int cmdDataGlobalRow = GetGlobalRowFromCmdAndSubRow(cmdIndex, 1);  // 第2行，subRow=1
    for (size_t i = 0; i < cmdCols.size(); i++) {
        RectF cellRect((REAL)x, (REAL)currentY, (REAL)cmdCols[i].width, (REAL)m_rowHeight);
        graphics.FillRectangle(&bgBrush, cellRect);
        
        // 根据列选择颜色
        SolidBrush* currentBrush = &textBrush;
        if (i == 0) {  // DLL命令名列
            currentBrush = &cmdNameBrush;
        } else if (i == 1) {  // 返回值类型列
            currentBrush = &typeBrush;
        } else if (i == 3) {  // 备注列
            currentBrush = &commentBrush;
        }
        
        // 检查是否正在编辑此单元格
        bool isEditingThisCell = (m_isEditing && cmdDataGlobalRow == m_editingRow && (int)i == m_editingCol);
        
        if (i == 2) {  // 公开列居中
            graphics.DrawString(cmdData[i].c_str(), -1, &dataFont, cellRect, &centerFormat, currentBrush);
        } else {
            int textX = x + colPadding;
            int textY = currentY + m_rowHeight / 2;  // 垂直居中
            
            // 编辑时显示m_editBuffer，否则显示原文本
            const std::wstring& displayText = isEditingThisCell ? m_editBuffer : cmdData[i];
            
            // 如果正在编辑且有选区，先绘制选区背景
            if (isEditingThisCell && m_selectionStart != m_selectionEnd) {
                int selStart = (std::min)(m_selectionStart, m_selectionEnd);
                int selEnd = (std::max)(m_selectionStart, m_selectionEnd);
                selStart = (std::min)(selStart, (int)displayText.length());
                selEnd = (std::min)(selEnd, (int)displayText.length());
                
                std::wstring beforeSel = displayText.substr(0, selStart);
                std::wstring selection = displayText.substr(selStart, selEnd - selStart);
                
                RectF beforeRect, selRect;
                graphics.MeasureString(beforeSel.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &beforeRect);
                graphics.MeasureString(selection.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &selRect);
                
                int selX = textX + (int)beforeRect.Width;
                int selWidth = (int)selRect.Width;
                
                SolidBrush selBrush(Color(255, 51, 153, 255));  // 选区背景色
                graphics.FillRectangle(&selBrush, selX, currentY + 2, selWidth, m_rowHeight - 4);
            }
            
            PointF textPos((REAL)textX, (REAL)textY);
            graphics.DrawString(displayText.c_str(), -1, &dataFont, textPos, &leftFormat, currentBrush);
            
            // 如果正在编辑，额外绘制光标
            if (isEditingThisCell && m_cursorVisible) {
                std::wstring beforeCursor = displayText.substr(0, (std::min)(m_editingCursorPos, (int)displayText.length()));
                RectF boundingBox;
                // 使用与绘制相同的leftFormat测量
                graphics.MeasureString(beforeCursor.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &boundingBox);
                int cursorX = textX + (int)boundingBox.Width;
                // 光标高度跨越整个行高
                int cursorTop = currentY + 2;
                int cursorBottom = currentY + m_rowHeight - 2;
                Pen cursorPen(ColorFromCOLORREF(g_CurrentTheme.text), 1);
                graphics.DrawLine(&cursorPen, cursorX, cursorTop, cursorX, cursorBottom);
            }
        }
        // 只在列之间绘制垂直线，不在最后一列后面
        if (i < cmdCols.size() - 1) {
            graphics.DrawLine(&gridPen, x + cmdCols[i].width, currentY, x + cmdCols[i].width, currentY + m_rowHeight);
        }
        x += cmdCols[i].width;
    }
    // 填充右侧空白区域（如果参数列更宽）
    if (x < tableStartX + tableWidth) {
        RectF fillRect((REAL)x, (REAL)currentY, (REAL)(tableWidth - cmdWidth), (REAL)m_rowHeight);
        graphics.FillRectangle(&bgBrush, fillRect);
    }
    // 绘制行选中高亮（第2行）
    if (IsRowInSelection(cmdDataGlobalRow)) {
        RectF selectRect((REAL)tableStartX, (REAL)currentY, (REAL)tableWidth, (REAL)m_rowHeight);
        graphics.FillRectangle(&rowSelectBrush, selectRect);
    }
    graphics.DrawLine(&gridPen, tableStartX, currentY + m_rowHeight, tableStartX + tableWidth, currentY + m_rowHeight);
    currentY += m_rowHeight;
    
    // 如果命令被折叠，绘制完表头行和命令数据行后返回
    if (cmd.isCollapsed) {
        return;
    }
    
    // 第3-4行：库文件名
    
    // 库文件名表头（第3行）- subRow=2
    int libHeaderGlobalRow = GetGlobalRowFromCmdAndSubRow(cmdIndex, 2);
    RectF libHeaderRect((REAL)tableStartX, (REAL)currentY, (REAL)tableWidth, (REAL)m_rowHeight);
    graphics.FillRectangle(&cmdHeaderBrush, libHeaderRect);
    // 表头左对齐
    int libHeaderTextX = tableStartX + colPadding;
    int libHeaderTextY = currentY + m_rowHeight / 2;
    PointF libHeaderPos((REAL)libHeaderTextX, (REAL)libHeaderTextY);
    graphics.DrawString(L"库文件名", -1, &headerFont, libHeaderPos, &leftFormat, &textBrush);
    // 绘制行选中高亮（第3行）
    if (IsRowInSelection(libHeaderGlobalRow)) {
        RectF selectRect((REAL)tableStartX, (REAL)currentY, (REAL)tableWidth, (REAL)m_rowHeight);
        graphics.FillRectangle(&rowSelectBrush, selectRect);
    }
    graphics.DrawLine(&gridPen, tableStartX, currentY + m_rowHeight, tableStartX + tableWidth, currentY + m_rowHeight);
    currentY += m_rowHeight;
    
    // 库文件名数据（第4行）
    RectF libDataRect((REAL)tableStartX, (REAL)currentY, (REAL)tableWidth, (REAL)m_rowHeight);
    graphics.FillRectangle(&bgBrush, libDataRect);
    int libTextX = tableStartX + colPadding;
    int libTextY = currentY + m_rowHeight / 2;  // 垂直居中
    
    // 编辑时显示m_editBuffer，否则显示原文本（使用前面计算的isEditingLibFile和dllFileText）
    const std::wstring& libDisplayText = dllFileText;
    
    // 如果正在编辑且有选区，先绘制选区背景
    if (isEditingLibFile && m_selectionStart != m_selectionEnd) {
        int selStart = (std::min)(m_selectionStart, m_selectionEnd);
        int selEnd = (std::max)(m_selectionStart, m_selectionEnd);
        selStart = (std::min)(selStart, (int)libDisplayText.length());
        selEnd = (std::min)(selEnd, (int)libDisplayText.length());
        
        std::wstring beforeSel = libDisplayText.substr(0, selStart);
        std::wstring selection = libDisplayText.substr(selStart, selEnd - selStart);
        
        RectF beforeRect, selRect;
        graphics.MeasureString(beforeSel.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &beforeRect);
        graphics.MeasureString(selection.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &selRect);
        
        int selX = libTextX + (int)beforeRect.Width;
        int selWidth = (int)selRect.Width;
        
        SolidBrush selBrush(Color(255, 51, 153, 255));  // 选区背景色
        graphics.FillRectangle(&selBrush, selX, currentY + 2, selWidth, m_rowHeight - 4);
    }
    
    PointF libTextPos((REAL)libTextX, (REAL)libTextY);
    graphics.DrawString(libDisplayText.c_str(), -1, &dataFont, libTextPos, &leftFormat, &libInfoBrush);
    
    // 如果正在编辑，额外绘制光标
    if (isEditingLibFile && m_cursorVisible) {
        std::wstring beforeCursor = libDisplayText.substr(0, (std::min)(m_editingCursorPos, (int)libDisplayText.length()));
        RectF boundingBox;
        // 使用与绘制相同的leftFormat测量
        graphics.MeasureString(beforeCursor.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &boundingBox);
        int cursorX = libTextX + (int)boundingBox.Width;
        int cursorTop = currentY + 2;
        int cursorBottom = currentY + m_rowHeight - 2;
        Pen cursorPen(ColorFromCOLORREF(g_CurrentTheme.text), 1);
        graphics.DrawLine(&cursorPen, cursorX, cursorTop, cursorX, cursorBottom);
    }
    // 绘制行选中高亮（第4行）- subRow=3
    int libDataGlobalRow = GetGlobalRowFromCmdAndSubRow(cmdIndex, 3);
    if (IsRowInSelection(libDataGlobalRow)) {
        RectF selectRect((REAL)tableStartX, (REAL)currentY, (REAL)tableWidth, (REAL)m_rowHeight);
        graphics.FillRectangle(&rowSelectBrush, selectRect);
    }
    graphics.DrawLine(&gridPen, tableStartX, currentY + m_rowHeight, tableStartX + tableWidth, currentY + m_rowHeight);
    currentY += m_rowHeight;
    
    // 第5-6行：在库中对应的命令名
    // DLL命令名表头（第5行）- subRow=4
    int dllHeaderGlobalRow = GetGlobalRowFromCmdAndSubRow(cmdIndex, 4);
    RectF dllHeaderRect((REAL)tableStartX, (REAL)currentY, (REAL)tableWidth, (REAL)m_rowHeight);
    graphics.FillRectangle(&cmdHeaderBrush, dllHeaderRect);
    // 表头左对齐
    int dllHeaderTextX = tableStartX + colPadding;
    int dllHeaderTextY = currentY + m_rowHeight / 2;
    PointF dllHeaderPos((REAL)dllHeaderTextX, (REAL)dllHeaderTextY);
    graphics.DrawString(L"在库中对应的命令名", -1, &headerFont, dllHeaderPos, &leftFormat, &textBrush);
    // 绘制行选中高亮（第5行）
    if (IsRowInSelection(dllHeaderGlobalRow)) {
        RectF selectRect((REAL)tableStartX, (REAL)currentY, (REAL)tableWidth, (REAL)m_rowHeight);
        graphics.FillRectangle(&rowSelectBrush, selectRect);
    }
    graphics.DrawLine(&gridPen, tableStartX, currentY + m_rowHeight, tableStartX + tableWidth, currentY + m_rowHeight);
    currentY += m_rowHeight;
    
    // DLL命令名数据（第6行）- subRow=5
    int dllDataGlobalRow = GetGlobalRowFromCmdAndSubRow(cmdIndex, 5);
    RectF dllDataRect((REAL)tableStartX, (REAL)currentY, (REAL)tableWidth, (REAL)m_rowHeight);
    graphics.FillRectangle(&bgBrush, dllDataRect);
    int dllTextX = tableStartX + colPadding;
    int dllTextY = currentY + m_rowHeight / 2;  // 垂直居中
    
    // 编辑时显示m_editBuffer，否则显示原文本（使用前面计算的isEditingDllCmd和dllCmdText）
    const std::wstring& dllDisplayText = dllCmdText;
    
    // 如果正在编辑且有选区，先绘制选区背景
    if (isEditingDllCmd && m_selectionStart != m_selectionEnd) {
        int selStart = (std::min)(m_selectionStart, m_selectionEnd);
        int selEnd = (std::max)(m_selectionStart, m_selectionEnd);
        selStart = (std::min)(selStart, (int)dllDisplayText.length());
        selEnd = (std::min)(selEnd, (int)dllDisplayText.length());
        
        std::wstring beforeSel = dllDisplayText.substr(0, selStart);
        std::wstring selection = dllDisplayText.substr(selStart, selEnd - selStart);
        
        RectF beforeRect, selRect;
        graphics.MeasureString(beforeSel.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &beforeRect);
        graphics.MeasureString(selection.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &selRect);
        
        int selX = dllTextX + (int)beforeRect.Width;
        int selWidth = (int)selRect.Width;
        
        SolidBrush selBrush(Color(255, 51, 153, 255));  // 选区背景色
        graphics.FillRectangle(&selBrush, selX, currentY + 2, selWidth, m_rowHeight - 4);
    }
    
    PointF dllTextPos((REAL)dllTextX, (REAL)dllTextY);
    graphics.DrawString(dllDisplayText.c_str(), -1, &dataFont, dllTextPos, &leftFormat, &libInfoBrush);
    
    // 如果正在编辑，额外绘制光标
    if (isEditingDllCmd && m_cursorVisible) {
        std::wstring beforeCursor = dllDisplayText.substr(0, (std::min)(m_editingCursorPos, (int)dllDisplayText.length()));
        RectF boundingBox;
        // 使用与绘制相同的leftFormat测量
        graphics.MeasureString(beforeCursor.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &boundingBox);
        int cursorX = dllTextX + (int)boundingBox.Width;
        int cursorTop = currentY + 2;
        int cursorBottom = currentY + m_rowHeight - 2;
        Pen cursorPen(ColorFromCOLORREF(g_CurrentTheme.text), 1);
        graphics.DrawLine(&cursorPen, cursorX, cursorTop, cursorX, cursorBottom);
    }
    // 绘制行选中高亮（第6行）
    if (IsRowInSelection(dllDataGlobalRow)) {
        RectF selectRect((REAL)tableStartX, (REAL)currentY, (REAL)tableWidth, (REAL)m_rowHeight);
        graphics.FillRectangle(&rowSelectBrush, selectRect);
    }
    graphics.DrawLine(&gridPen, tableStartX, currentY + m_rowHeight, tableStartX + tableWidth, currentY + m_rowHeight);
    currentY += m_rowHeight;
    
    // 第7行及以后：参数表头 + 参数数据
    // (参数列已经在前面定义)
    
    // 参数表头（第7行）- subRow=6
    int paramHeaderGlobalRow = GetGlobalRowFromCmdAndSubRow(cmdIndex, 6);
    x = tableStartX;
    for (size_t i = 0; i < paramCols.size(); i++) {
        RectF cellRect((REAL)x, (REAL)currentY, (REAL)paramCols[i].width, (REAL)m_rowHeight);
        graphics.FillRectangle(&paramHeaderBrush, cellRect);
        // 表头左对齐，添加左边距
        int paramHeaderTextX = x + colPadding;
        int paramHeaderTextY = currentY + m_rowHeight / 2;
        PointF paramHeaderPos((REAL)paramHeaderTextX, (REAL)paramHeaderTextY);
        graphics.DrawString(paramCols[i].name.c_str(), -1, &headerFont, paramHeaderPos, &leftFormat, &textBrush);
        graphics.DrawLine(&gridPen, x + paramCols[i].width, currentY, x + paramCols[i].width, currentY + m_rowHeight);
        x += paramCols[i].width;
    }
    // 绘制行选中高亮（第7行）
    if (IsRowInSelection(paramHeaderGlobalRow)) {
        RectF selectRect((REAL)tableStartX, (REAL)currentY, (REAL)tableWidth, (REAL)m_rowHeight);
        graphics.FillRectangle(&rowSelectBrush, selectRect);
    }
    graphics.DrawLine(&gridPen, tableStartX, currentY + m_rowHeight, tableStartX + tableWidth, currentY + m_rowHeight);
    currentY += m_rowHeight;
    
    // 参数数据行（重新使用paramRowIndex）
    paramRowIndex = 0;
    for (int paramIdx : cmd.paramLines) {
        if (paramIdx >= 0 && paramIdx < (int)m_allParams.size()) {
            const DllParameter& param = m_allParams[paramIdx];
            
            int paramGlobalRow = GetGlobalRowFromCmdAndSubRow(cmdIndex, 7 + paramRowIndex);  // 第8行开始
            
            // 判断当前参数行的各列是否正在编辑
            bool isEditingThisParamName = (m_isEditing && paramGlobalRow == m_editingRow && m_editingCol == 0);
            bool isEditingThisParamType = (m_isEditing && paramGlobalRow == m_editingRow && m_editingCol == 1);
            bool isEditingThisParamComment = (m_isEditing && paramGlobalRow == m_editingRow && m_editingCol == 4);
            
            // 使用编辑内容或原始内容
            std::wstring displayParamName = isEditingThisParamName ? m_editBuffer : param.paramName;
            std::wstring displayParamType = isEditingThisParamType ? m_editBuffer : param.paramType;
            std::wstring displayParamComment = isEditingThisParamComment ? m_editBuffer : param.comment;
            
            x = tableStartX;
            std::vector<std::wstring> paramData = {displayParamName, displayParamType, 
                                                   param.byRef ? L"✓" : L"", 
                                                   param.isArray ? L"✓" : L"", 
                                                   displayParamComment};
            
            for (size_t i = 0; i < paramCols.size(); i++) {
                RectF cellRect((REAL)x, (REAL)currentY, (REAL)paramCols[i].width, (REAL)m_rowHeight);
                graphics.FillRectangle(&bgBrush, cellRect);
                
                // 根据列选择颜色
                SolidBrush* currentBrush = &textBrush;
                if (i == 0) {  // 参数名列
                    currentBrush = &paramNameBrush;
                } else if (i == 1) {  // 类型列
                    currentBrush = &typeBrush;
                } else if (i == 4) {  // 备注列
                    currentBrush = &commentBrush;
                }
                
                // 检查是否正在编辑此单元格
                bool isEditingThisCell = (m_isEditing && paramGlobalRow == m_editingRow && (int)i == m_editingCol);
                
                if (i == 2 || i == 3) {  // 传址、数组列居中
                    graphics.DrawString(paramData[i].c_str(), -1, &dataFont, cellRect, &centerFormat, currentBrush);
                } else {
                    int textX = x + colPadding;
                    int textY = currentY + m_rowHeight / 2;  // 垂直居中
                    
                    // paramData已经包含了编辑状态的文本
                    const std::wstring& displayText = paramData[i];
                    
                    // 如果正在编辑且有选区，先绘制选区背景
                    if (isEditingThisCell && m_selectionStart != m_selectionEnd) {
                        int selStart = (std::min)(m_selectionStart, m_selectionEnd);
                        int selEnd = (std::max)(m_selectionStart, m_selectionEnd);
                        selStart = (std::min)(selStart, (int)displayText.length());
                        selEnd = (std::min)(selEnd, (int)displayText.length());
                        
                        std::wstring beforeSel = displayText.substr(0, selStart);
                        std::wstring selection = displayText.substr(selStart, selEnd - selStart);
                        
                        RectF beforeRect, selRect;
                        graphics.MeasureString(beforeSel.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &beforeRect);
                        graphics.MeasureString(selection.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &selRect);
                        
                        int selX = textX + (int)beforeRect.Width;
                        int selWidth = (int)selRect.Width;
                        
                        SolidBrush selBrush(Color(255, 51, 153, 255));  // 选区背景色
                        graphics.FillRectangle(&selBrush, selX, currentY + 2, selWidth, m_rowHeight - 4);
                    }
                    
                    PointF textPos((REAL)textX, (REAL)textY);
                    graphics.DrawString(displayText.c_str(), -1, &dataFont, textPos, &leftFormat, currentBrush);
                    
                    // 如果正在编辑，额外绘制光标
                    if (isEditingThisCell && m_cursorVisible) {
                        std::wstring beforeCursor = displayText.substr(0, (std::min)(m_editingCursorPos, (int)displayText.length()));
                        RectF boundingBox;
                        // 使用与绘制相同的leftFormat测量
                        graphics.MeasureString(beforeCursor.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &boundingBox);
                        int cursorX = textX + (int)boundingBox.Width;
                        int cursorTop = currentY + 2;
                        int cursorBottom = currentY + m_rowHeight - 2;
                        Pen cursorPen(ColorFromCOLORREF(g_CurrentTheme.text), 1);
                        graphics.DrawLine(&cursorPen, cursorX, cursorTop, cursorX, cursorBottom);
                    }
                }
                graphics.DrawLine(&gridPen, x + paramCols[i].width, currentY, x + paramCols[i].width, currentY + m_rowHeight);
                x += paramCols[i].width;
            }
            // 绘制行选中高亮（参数数据行）
            if (IsRowInSelection(paramGlobalRow)) {
                RectF selectRect((REAL)tableStartX, (REAL)currentY, (REAL)tableWidth, (REAL)m_rowHeight);
                graphics.FillRectangle(&rowSelectBrush, selectRect);
            }
            graphics.DrawLine(&gridPen, tableStartX, currentY + m_rowHeight, tableStartX + tableWidth, currentY + m_rowHeight);
            currentY += m_rowHeight;
            paramRowIndex++;  // 增加参数行索引
        }
    }
}

// 绘制带光标的单元格文本
void DllEditor::DrawCellWithCursor(Gdiplus::Graphics& graphics, const std::wstring& text, int x, int y, Gdiplus::Font& font, Gdiplus::SolidBrush& textBrush) {
    // 使用GenericTypographic格式确保绘制和测量一致性
    StringFormat leftFormat(StringFormat::GenericTypographic());
    leftFormat.SetAlignment(StringAlignmentNear);
    leftFormat.SetLineAlignment(StringAlignmentCenter);
    leftFormat.SetFormatFlags(StringFormatFlagsMeasureTrailingSpaces | StringFormatFlagsNoWrap);
    
    // 绘制选区背景（如果有）
    if (m_selectionStart != m_selectionEnd) {
        int selStart = (std::min)(m_selectionStart, m_selectionEnd);
        int selEnd = (std::max)(m_selectionStart, m_selectionEnd);
        
        std::wstring beforeSel = text.substr(0, selStart);
        std::wstring selection = text.substr(selStart, selEnd - selStart);
        
        RectF measureRect;
        // 使用相同的格式测量
        graphics.MeasureString(beforeSel.c_str(), -1, &font, PointF(0, 0), &leftFormat, &measureRect);
        int selX = x + (int)measureRect.Width;
        
        graphics.MeasureString(selection.c_str(), -1, &font, PointF(0, 0), &leftFormat, &measureRect);
        int selWidth = (int)measureRect.Width;
        
        SolidBrush selBrush(Color(255, 100, 100, 200)); // 选区背景色
        graphics.FillRectangle(&selBrush, selX, y, selWidth, m_rowHeight);
    }
    
    // 绘制文本（使用垂直居中对齐）
    RectF textRect((REAL)x, (REAL)y, 1000.0f, (REAL)m_rowHeight);  // 宽度设为足够大
    graphics.DrawString(text.c_str(), -1, &font, textRect, &leftFormat, &textBrush);
    
    // 绘制光标（如果可见）
    if (m_cursorVisible && m_editingCursorPos <= (int)text.length()) {
        std::wstring beforeCursor = text.substr(0, m_editingCursorPos);
        RectF measureRect;
        // 使用与绘制相同的leftFormat测量
        graphics.MeasureString(beforeCursor.c_str(), -1, &font, PointF(0, 0), &leftFormat, &measureRect);
        int cursorX = x + (int)measureRect.Width;
        
        Pen cursorPen(ColorFromCOLORREF(g_CurrentTheme.text), 1);
        graphics.DrawLine(&cursorPen, (INT)cursorX, (INT)y, (INT)cursorX, (INT)(y + m_rowHeight));
    }
}

// 根据坐标获取行号
int DllEditor::HitTestRow(int x, int y) {
    // 调整滚动偏移
    int adjustedY = y + m_scrollY;
    
    // 计算表格起始位置
    int tableStartY = m_headerHeight;  // DllEditor的m_headerHeight为0
    
    if (adjustedY < tableStartY) {
        return -1;
    }
    
    // 遍历所有命令计算行号
    int currentY = tableStartY;
    int globalRow = 0;
    const int separatorHeight = 20;
    
    for (size_t cmdIdx = 0; cmdIdx < m_commands.size(); cmdIdx++) {
        const DllCommand& cmd = m_commands[cmdIdx];
        int fullRowCount = 7 + (int)cmd.paramLines.size();  // 完整的行数（数据层面）
        
        // 如果命令被折叠，屏幕上只显示2行，但数据行号不变
        int visibleRowCount;
        if (cmd.isCollapsed) {
            visibleRowCount = 2;
        } else {
            visibleRowCount = fullRowCount;
        }
        
        for (int subRow = 0; subRow < visibleRowCount; subRow++) {
            if (adjustedY >= currentY && adjustedY < currentY + m_rowHeight) {
                // 返回真实的数据行号
                return globalRow + subRow;
            }
            currentY += m_rowHeight;
        }
        
        // 移动到下一个命令的起始行号（使用完整行数）
        globalRow += fullRowCount;
        
        // 命令间隔行（最后一个命令除外）
        if (cmdIdx < m_commands.size() - 1) {
            if (adjustedY >= currentY && adjustedY < currentY + separatorHeight) {
                return globalRow;  // 间隔行
            }
            currentY += separatorHeight;
            globalRow++;  // 间隔行也是一行
        }
    }
    
    return -1;
}

// 清除行选择
void DllEditor::ClearRowSelection() {
    m_hasRowSelection = false;
    m_isRowSelecting = false;
    m_rowSelectStartRow = -1;
    m_rowSelectEndRow = -1;
}

// 判断行是否在选中范围内
bool DllEditor::IsRowInSelection(int row) const {
    if (!m_hasRowSelection) return false;
    
    int startRow = (std::min)(m_rowSelectStartRow, m_rowSelectEndRow);
    int endRow = (std::max)(m_rowSelectStartRow, m_rowSelectEndRow);
    
    return row >= startRow && row <= endRow;
}

// 鼠标移动处理
void DllEditor::OnMouseMove(int x, int y, WPARAM wParam) {
    // 在行号区域显示镜像鼠标指针（右侧的展开/折叠按钮区域不改变）
    if (x >= 0 && x < m_lineNumberAreaWidth) {
        // 行号区域，使用反向箭头光标（左右镜像）
        // 创建或获取镜像光标
        static HCURSOR hReverseCursor = NULL;
        if (hReverseCursor == NULL) {
            // 通过复制并翻转标准箭头光标来创建镜像光标
            HCURSOR hArrow = LoadCursor(NULL, IDC_ARROW);
            ICONINFO iconInfo;
            if (GetIconInfo(hArrow, &iconInfo)) {
                // 获取位图信息
                BITMAP bmpMask, bmpColor;
                GetObject(iconInfo.hbmMask, sizeof(BITMAP), &bmpMask);
                
                // 创建镜像的位图
                HDC hdcScreen = GetDC(NULL);
                HDC hdcSrc = CreateCompatibleDC(hdcScreen);
                HDC hdcDst = CreateCompatibleDC(hdcScreen);
                
                // 镜像掩码位图
                HBITMAP hbmMaskMirror = CreateCompatibleBitmap(hdcScreen, bmpMask.bmWidth, bmpMask.bmHeight);
                SelectObject(hdcSrc, iconInfo.hbmMask);
                SelectObject(hdcDst, hbmMaskMirror);
                StretchBlt(hdcDst, bmpMask.bmWidth - 1, 0, -bmpMask.bmWidth, bmpMask.bmHeight,
                          hdcSrc, 0, 0, bmpMask.bmWidth, bmpMask.bmHeight, SRCCOPY);
                
                // 镜像颜色位图（如果有）
                HBITMAP hbmColorMirror = NULL;
                if (iconInfo.hbmColor) {
                    GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bmpColor);
                    hbmColorMirror = CreateCompatibleBitmap(hdcScreen, bmpColor.bmWidth, bmpColor.bmHeight);
                    SelectObject(hdcSrc, iconInfo.hbmColor);
                    SelectObject(hdcDst, hbmColorMirror);
                    StretchBlt(hdcDst, bmpColor.bmWidth - 1, 0, -bmpColor.bmWidth, bmpColor.bmHeight,
                              hdcSrc, 0, 0, bmpColor.bmWidth, bmpColor.bmHeight, SRCCOPY);
                }
                
                DeleteDC(hdcSrc);
                DeleteDC(hdcDst);
                ReleaseDC(NULL, hdcScreen);
                
                // 创建镜像光标
                ICONINFO mirrorInfo;
                mirrorInfo.fIcon = FALSE;
                mirrorInfo.xHotspot = bmpMask.bmWidth - iconInfo.xHotspot - 1;  // 镜像热点
                mirrorInfo.yHotspot = iconInfo.yHotspot;
                mirrorInfo.hbmMask = hbmMaskMirror;
                mirrorInfo.hbmColor = hbmColorMirror;
                
                hReverseCursor = CreateIconIndirect(&mirrorInfo);
                
                // 清理
                DeleteObject(iconInfo.hbmMask);
                if (iconInfo.hbmColor) DeleteObject(iconInfo.hbmColor);
                DeleteObject(hbmMaskMirror);
                if (hbmColorMirror) DeleteObject(hbmColorMirror);
            }
            
            // 如果创建失败，使用默认箭头
            if (hReverseCursor == NULL) {
                hReverseCursor = hArrow;
            }
        }
        SetCursor(hReverseCursor);
    } else if (x < m_leftMarginWidth) {
        // 展开/折叠按钮区域，使用默认箭头
        SetCursor(LoadCursor(NULL, IDC_ARROW));
    }
    
    // 如果正在进行行选择
    if (m_isRowSelecting) {
        int hitRow = HitTestRow(x, y);
        if (hitRow >= 0 && hitRow != m_rowSelectEndRow) {
            m_rowSelectEndRow = hitRow;
            InvalidateRect(m_hWnd, NULL, FALSE);
        }
        return;
    }
    
    // 如果正在单元格内选择文本
    if (m_isCellTextSelecting && m_isEditing) {
        // 计算鼠标在单元格内的相对位置
        int relativeX = x - m_currentCellRect.left;
        
        // 获取当前编辑文本
        const std::wstring& text = m_editBuffer;
        
        // 使用GDI+测量文本宽度来确定光标位置
        HDC hdc = GetDC(m_hWnd);
        Gdiplus::Graphics graphics(hdc);
        Gdiplus::FontFamily fontFamily(L"Microsoft YaHei");
        Gdiplus::Font font(&fontFamily, (Gdiplus::REAL)m_fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
        Gdiplus::StringFormat format(Gdiplus::StringFormat::GenericTypographic());
        format.SetFormatFlags(Gdiplus::StringFormatFlagsMeasureTrailingSpaces);
        
        int colPadding = 5;
        int textX = relativeX - colPadding;
        
        // 找到最接近的字符位置
        int newCursorPos = 0;
        for (size_t i = 0; i <= text.length(); i++) {
            std::wstring sub = text.substr(0, i);
            Gdiplus::RectF measureRect;
            graphics.MeasureString(sub.c_str(), -1, &font, Gdiplus::PointF(0, 0), &format, &measureRect);
            
            if ((int)measureRect.Width >= textX) {
                // 检查是否更接近前一个字符
                if (i > 0) {
                    std::wstring prevSub = text.substr(0, i - 1);
                    Gdiplus::RectF prevRect;
                    graphics.MeasureString(prevSub.c_str(), -1, &font, Gdiplus::PointF(0, 0), &format, &prevRect);
                    int prevDiff = textX - (int)prevRect.Width;
                    int currDiff = (int)measureRect.Width - textX;
                    newCursorPos = (prevDiff < currDiff) ? (int)(i - 1) : (int)i;
                } else {
                    newCursorPos = (int)i;
                }
                break;
            }
            newCursorPos = (int)i;
        }
        
        ReleaseDC(m_hWnd, hdc);
        
        // 更新选择结束位置
        m_selectionEnd = newCursorPos;
        m_editingCursorPos = newCursorPos;
        
        InvalidateRect(m_hWnd, NULL, FALSE);
        return;
    }
    
    // 调用基类的鼠标移动处理
    TableEditor::OnMouseMove(x, y, wParam);
}

// 鼠标释放处理
void DllEditor::OnLButtonUp(int x, int y) {
    // 如果正在进行行选择
    if (m_isRowSelecting) {
        m_isRowSelecting = false;
        ReleaseCapture();
        
        // 确保有有效的选择
        if (m_rowSelectStartRow >= 0 && m_rowSelectEndRow >= 0) {
            m_hasRowSelection = true;
        }
        return;
    }
    
    // 如果正在单元格内选择文本
    if (m_isCellTextSelecting) {
        m_isCellTextSelecting = false;
        ReleaseCapture();
        return;
    }
    
    // 调用基类的鼠标释放处理
    TableEditor::OnLButtonUp(x, y);
}

// 删除选中的行
void DllEditor::DeleteSelectedRows() {
    if (!m_hasRowSelection) return;
    
    int startRow = (std::min)(m_rowSelectStartRow, m_rowSelectEndRow);
    int endRow = (std::max)(m_rowSelectStartRow, m_rowSelectEndRow);
    
    // 分析选中区域包含哪些行类型
    int cmdIndex, paramIndex;
    DllRowType startRowType = GetRowType(startRow, cmdIndex, paramIndex);
    int startCmdIndex = cmdIndex;
    int startSubRow = -1;
    int startParamIndex = paramIndex;
    GetCommandIndexFromRow(startRow, startSubRow);
    
    DllRowType endRowType = GetRowType(endRow, cmdIndex, paramIndex);
    int endCmdIndex = cmdIndex;
    int endSubRow = -1;
    int endParamIndex = paramIndex;
    GetCommandIndexFromRow(endRow, endSubRow);
    
    // 如果起始命令和结束命令不同，或者选中了DllNameData（subRow 5）及以上的行
    // 则需要删除整个命令
    
    // 检查是否选中了"在库中对应的命令名数据行"（subRow 5）或以上的行
    bool shouldDeleteEntireCommand = false;
    
    // 判断选择范围内是否包含了DllNameData及以上的行
    // DllNameData的subRow是5，如果startSubRow <= 5，说明选中了关键行
    if (startSubRow >= 0 && startSubRow <= 5) {
        shouldDeleteEntireCommand = true;
    }
    
    // 如果跨越了多个命令，也需要删除整个命令
    if (startCmdIndex != endCmdIndex) {
        shouldDeleteEntireCommand = true;
    }
    
    if (shouldDeleteEntireCommand) {
        // 删除整个命令（可能是多个）
        CreateSnapshot(L"删除DLL命令");
        
        // 从后往前删除，避免索引问题
        int firstCmd = (std::min)(startCmdIndex, endCmdIndex);
        int lastCmd = (std::max)(startCmdIndex, endCmdIndex);
        
        for (int i = lastCmd; i >= firstCmd; i--) {
            if (i >= 0 && i < (int)m_commands.size()) {
                DeleteCommand(i);
            }
        }
    } else {
        // 只选中了参数相关的行（subRow >= 6）
        // subRow 6 是参数表头，subRow 7+ 是参数数据行
        
        if (startSubRow == 6) {
            // 如果起始行是参数表头，不删除任何东西（保留表头）
            ClearRowSelection();
            InvalidateRect(m_hWnd, NULL, FALSE);
            return;
        }
        
        if (startSubRow >= 7) {
            // 选中的是参数数据行
            CreateSnapshot(L"删除参数行");
            
            // 计算要删除的参数范围
            int firstParamIdx = startSubRow - 7;  // 参数在命令中的索引
            int lastParamIdx = endSubRow - 7;
            
            if (lastParamIdx >= (int)m_commands[startCmdIndex].paramLines.size()) {
                lastParamIdx = (int)m_commands[startCmdIndex].paramLines.size() - 1;
            }
            
            // 从后往前删除参数
            for (int i = lastParamIdx; i >= firstParamIdx; i--) {
                if (i >= 0 && i < (int)m_commands[startCmdIndex].paramLines.size()) {
                    DeleteParameter(startCmdIndex, i);
                }
            }
        }
    }
    
    ClearRowSelection();
    m_modified = true;
    InvalidateRect(m_hWnd, NULL, FALSE);
}
// === 新建DLL命令 ===

std::wstring DllEditor::GenerateUniqueDllCommandName() {
    return GenerateUniqueDllCommandName(L"DLL命令");
}

std::wstring DllEditor::GenerateUniqueDllCommandName(const std::wstring& baseName) {
    // 先检查基础名本身是否已存在
    bool baseExists = false;
    for (const auto& cmd : m_commands) {
        if (cmd.commandName == baseName) {
            baseExists = true;
            break;
        }
    }
    if (!baseExists) {
        return baseName;  // 基础名不存在，直接返回
    }
    
    // 基础名已存在，尝试添加数字后缀
    int number = 1;
    while (true) {
        std::wstring candidateName = baseName + std::to_wstring(number);
        
        // 检查是否已存在
        bool exists = false;
        for (const auto& cmd : m_commands) {
            if (cmd.commandName == candidateName) {
                exists = true;
                break;
            }
        }
        
        if (!exists) {
            return candidateName;
        }
        
        number++;
    }
}

// 解析易语言格式的DLL命令文本并插入到编辑器
void DllEditor::ParseAndInsertDllCommands(const std::wstring& text) {
    // 按行分割
    std::vector<std::wstring> lines;
    std::wistringstream stream(text);
    std::wstring line;
    while (std::getline(stream, line)) {
        // 移除行尾的 \r
        if (!line.empty() && line.back() == L'\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }
    
    DllCommand* currentCmd = nullptr;
    std::vector<DllCommand> newCommands;
    
    for (const auto& line : lines) {
        // 跳过空行和版本行
        if (line.empty() || line.find(L".版本") == 0) {
            continue;
        }
        
        // 解析 .DLL命令 行
        if (line.find(L".DLL命令 ") == 0) {
            // 新建命令
            newCommands.push_back(DllCommand());
            currentCmd = &newCommands.back();
            currentCmd->isPublic = false;
            currentCmd->isCollapsed = false;
            
            // 解析命令行内容
            std::wstring content = line.substr(8);  // 跳过 ".DLL命令 "
            
            // 分割参数（逗号分隔，但要考虑引号内的逗号）
            std::vector<std::wstring> parts;
            std::wstring part;
            bool inQuotes = false;
            for (size_t i = 0; i < content.length(); i++) {
                wchar_t c = content[i];
                if (c == L'"') {
                    inQuotes = !inQuotes;
                } else if (c == L',' && !inQuotes) {
                    // 去除首尾空格
                    size_t start = part.find_first_not_of(L' ');
                    size_t end = part.find_last_not_of(L' ');
                    if (start != std::wstring::npos) {
                        parts.push_back(part.substr(start, end - start + 1));
                    } else {
                        parts.push_back(L"");
                    }
                    part.clear();
                    continue;
                }
                part += c;
            }
            // 添加最后一部分
            size_t start = part.find_first_not_of(L' ');
            size_t end = part.find_last_not_of(L' ');
            if (start != std::wstring::npos) {
                parts.push_back(part.substr(start, end - start + 1));
            } else {
                parts.push_back(L"");
            }
            
            // 解析各个部分
            if (parts.size() > 0) currentCmd->commandName = parts[0];
            if (parts.size() > 1) currentCmd->returnType = parts[1];
            if (parts.size() > 2) {
                // 去除引号
                std::wstring& s = parts[2];
                if (s.length() >= 2 && s.front() == L'"' && s.back() == L'"') {
                    currentCmd->dllFileName = s.substr(1, s.length() - 2);
                } else {
                    currentCmd->dllFileName = s;
                }
            }
            if (parts.size() > 3) {
                std::wstring& s = parts[3];
                if (s.length() >= 2 && s.front() == L'"' && s.back() == L'"') {
                    currentCmd->dllCommandName = s.substr(1, s.length() - 2);
                } else {
                    currentCmd->dllCommandName = s;
                }
            }
            // 检查公开标志和备注
            for (size_t i = 4; i < parts.size(); i++) {
                if (parts[i] == L"公开") {
                    currentCmd->isPublic = true;
                } else if (!parts[i].empty()) {
                    // 作为备注
                    currentCmd->comment = parts[i];
                }
            }
        }
        // 解析 .参数 行
        else if (line.find(L"    .参数 ") == 0 || line.find(L"\t.参数 ") == 0) {
            if (!currentCmd) continue;
            
            // 解析参数行内容
            size_t startPos = line.find(L".参数 ");
            if (startPos == std::wstring::npos) continue;
            std::wstring content = line.substr(startPos + 4);  // 跳过 ".参数 "
            
            // 分割参数
            std::vector<std::wstring> parts;
            std::wstring part;
            for (size_t i = 0; i < content.length(); i++) {
                wchar_t c = content[i];
                if (c == L',') {
                    size_t start = part.find_first_not_of(L' ');
                    size_t end = part.find_last_not_of(L' ');
                    if (start != std::wstring::npos) {
                        parts.push_back(part.substr(start, end - start + 1));
                    } else {
                        parts.push_back(L"");
                    }
                    part.clear();
                    continue;
                }
                part += c;
            }
            size_t start = part.find_first_not_of(L' ');
            size_t end = part.find_last_not_of(L' ');
            if (start != std::wstring::npos) {
                parts.push_back(part.substr(start, end - start + 1));
            } else {
                parts.push_back(L"");
            }
            
            // 创建参数
            DllParameter param;
            if (parts.size() > 0) param.paramName = parts[0];
            if (parts.size() > 1) param.paramType = parts[1];
            // 第3部分是属性（传址/数组）
            if (parts.size() > 2) {
                std::wstring attrs = parts[2];
                param.byRef = (attrs.find(L"传址") != std::wstring::npos);
                param.isArray = (attrs.find(L"数组") != std::wstring::npos);
            }
            if (parts.size() > 3) param.comment = parts[3];
            
            // 添加参数
            int paramIdx = (int)m_allParams.size();
            m_allParams.push_back(param);
            currentCmd->paramLines.push_back(paramIdx);
        }
    }
    
    // 将解析的命令添加到编辑器
    if (!newCommands.empty()) {
        CreateSnapshot(L"粘贴DLL命令");
        for (auto& cmd : newCommands) {
            // 确保命令名唯一
            cmd.commandName = GenerateUniqueDllCommandName(cmd.commandName.empty() ? L"DLL命令" : cmd.commandName);
            m_commands.push_back(std::move(cmd));
        }
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

// 在指定位置插入DLL命令
void DllEditor::ParseAndInsertDllCommandsAt(const std::wstring& text, int insertAt) {
    // 按行分割
    std::vector<std::wstring> lines;
    std::wistringstream stream(text);
    std::wstring line;
    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == L'\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }
    
    std::vector<DllCommand> newCommands;
    DllCommand* currentCmd = nullptr;
    
    for (const auto& line : lines) {
        std::wstring trimmedLine = line;
        // 去除前导空格
        size_t start = trimmedLine.find_first_not_of(L" \t");
        if (start != std::wstring::npos) {
            trimmedLine = trimmedLine.substr(start);
        }
        
        if (trimmedLine.empty() || trimmedLine == L".版本 2") continue;
        
        if (trimmedLine.find(L".DLL命令 ") == 0) {
            // 新命令
            newCommands.push_back(DllCommand());
            currentCmd = &newCommands.back();
            
            // 解析命令
            std::wstring content = trimmedLine.substr(7);  // 跳过 ".DLL命令 "
            
            std::vector<std::wstring> parts;
            std::wstring current;
            bool inQuote = false;
            for (size_t i = 0; i < content.length(); i++) {
                wchar_t ch = content[i];
                if (ch == L'"') {
                    inQuote = !inQuote;
                } else if (ch == L',' && !inQuote) {
                    // 去除前后空格
                    size_t s = current.find_first_not_of(L" \t");
                    size_t e = current.find_last_not_of(L" \t");
                    if (s != std::wstring::npos) {
                        parts.push_back(current.substr(s, e - s + 1));
                    } else {
                        parts.push_back(L"");
                    }
                    current.clear();
                } else {
                    current += ch;
                }
            }
            // 最后一部分
            size_t s = current.find_first_not_of(L" \t");
            size_t e = current.find_last_not_of(L" \t");
            if (s != std::wstring::npos) {
                parts.push_back(current.substr(s, e - s + 1));
            } else if (!current.empty()) {
                parts.push_back(L"");
            }
            
            if (parts.size() > 0) currentCmd->commandName = parts[0];
            if (parts.size() > 1) currentCmd->returnType = parts[1];
            if (parts.size() > 2) currentCmd->dllFileName = parts[2];
            if (parts.size() > 3) currentCmd->dllCommandName = parts[3];
            if (parts.size() > 4) currentCmd->isPublic = (parts[4] == L"公开");
            if (parts.size() > 5) currentCmd->comment = parts[5];
            
        } else if (trimmedLine.find(L".参数 ") == 0 && currentCmd) {
            // 参数
            std::wstring content = trimmedLine.substr(4);  // 跳过 ".参数 "
            
            std::vector<std::wstring> parts;
            std::wstring current;
            for (size_t i = 0; i < content.length(); i++) {
                wchar_t ch = content[i];
                if (ch == L',') {
                    size_t s = current.find_first_not_of(L" \t");
                    size_t e = current.find_last_not_of(L" \t");
                    if (s != std::wstring::npos) {
                        parts.push_back(current.substr(s, e - s + 1));
                    } else {
                        parts.push_back(L"");
                    }
                    current.clear();
                } else {
                    current += ch;
                }
            }
            size_t s = current.find_first_not_of(L" \t");
            size_t e = current.find_last_not_of(L" \t");
            if (s != std::wstring::npos) {
                parts.push_back(current.substr(s, e - s + 1));
            }
            
            DllParameter param;
            if (parts.size() > 0) param.paramName = parts[0];
            if (parts.size() > 1) param.paramType = parts[1];
            if (parts.size() > 2) {
                std::wstring attrs = parts[2];
                param.byRef = (attrs.find(L"传址") != std::wstring::npos);
                param.isArray = (attrs.find(L"数组") != std::wstring::npos);
            }
            if (parts.size() > 3) param.comment = parts[3];
            
            int paramIdx = (int)m_allParams.size();
            m_allParams.push_back(param);
            currentCmd->paramLines.push_back(paramIdx);
        }
    }
    
    // 在指定位置插入命令
    if (!newCommands.empty()) {
        CreateSnapshot(L"粘贴DLL命令");
        
        // 确保插入位置有效
        if (insertAt < 0) insertAt = 0;
        if (insertAt > (int)m_commands.size()) insertAt = (int)m_commands.size();
        
        for (auto& cmd : newCommands) {
            cmd.commandName = GenerateUniqueDllCommandName(cmd.commandName.empty() ? L"DLL命令" : cmd.commandName);
            m_commands.insert(m_commands.begin() + insertAt, std::move(cmd));
            insertAt++;
        }
        m_modified = true;
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

// 解析参数并插入到指定命令
void DllEditor::ParseAndInsertParameters(const std::wstring& text, int cmdIndex) {
    if (cmdIndex < 0 || cmdIndex >= (int)m_commands.size()) return;
    
    std::vector<std::wstring> lines;
    std::wistringstream stream(text);
    std::wstring line;
    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == L'\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }
    
    std::vector<DllParameter> newParams;
    
    for (const auto& line : lines) {
        std::wstring trimmedLine = line;
        size_t start = trimmedLine.find_first_not_of(L" \t");
        if (start != std::wstring::npos) {
            trimmedLine = trimmedLine.substr(start);
        }
        
        if (trimmedLine.find(L".参数 ") == 0) {
            std::wstring content = trimmedLine.substr(4);  // 跳过 ".参数 "
            
            std::vector<std::wstring> parts;
            std::wstring current;
            for (size_t i = 0; i < content.length(); i++) {
                wchar_t ch = content[i];
                if (ch == L',') {
                    size_t s = current.find_first_not_of(L" \t");
                    size_t e = current.find_last_not_of(L" \t");
                    if (s != std::wstring::npos) {
                        parts.push_back(current.substr(s, e - s + 1));
                    } else {
                        parts.push_back(L"");
                    }
                    current.clear();
                } else {
                    current += ch;
                }
            }
            size_t s = current.find_first_not_of(L" \t");
            size_t e = current.find_last_not_of(L" \t");
            if (s != std::wstring::npos) {
                parts.push_back(current.substr(s, e - s + 1));
            }
            
            DllParameter param;
            if (parts.size() > 0) param.paramName = parts[0];
            if (parts.size() > 1) param.paramType = parts[1];
            if (parts.size() > 2) {
                std::wstring attrs = parts[2];
                param.byRef = (attrs.find(L"传址") != std::wstring::npos);
                param.isArray = (attrs.find(L"数组") != std::wstring::npos);
            }
            if (parts.size() > 3) param.comment = parts[3];
            
            newParams.push_back(param);
        }
    }
    
    if (!newParams.empty()) {
        CreateSnapshot(L"粘贴参数");
        
        DllCommand& cmd = m_commands[cmdIndex];
        for (auto& param : newParams) {
            int paramIdx = (int)m_allParams.size();
            m_allParams.push_back(param);
            cmd.paramLines.push_back(paramIdx);
        }
        
        m_modified = true;
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

// 生成易语言格式的参数文本
std::wstring DllEditor::GenerateParameterClipboardText(int cmdIndex, const std::set<int>& paramIndices) {
    std::wstring text = L".版本 2\r\n\r\n";
    
    if (cmdIndex < 0 || cmdIndex >= (int)m_commands.size()) return text;
    
    const DllCommand& cmd = m_commands[cmdIndex];
    
    for (int localParamIdx : paramIndices) {
        if (localParamIdx < 0 || localParamIdx >= (int)cmd.paramLines.size()) continue;
        
        int globalParamIdx = cmd.paramLines[localParamIdx];
        if (globalParamIdx < 0 || globalParamIdx >= (int)m_allParams.size()) continue;
        
        const DllParameter& param = m_allParams[globalParamIdx];
        text += L".参数 " + param.paramName;
        
        // 类型
        if (!param.paramType.empty()) {
            text += L", " + param.paramType;
        }
        
        // 属性（传址/数组）
        std::wstring attrs;
        if (param.byRef) {
            attrs = L"传址";
        }
        if (param.isArray) {
            if (!attrs.empty()) attrs += L" ";
            attrs += L"数组";
        }
        if (!attrs.empty()) {
            text += L", " + attrs;
        }
        
        // 备注
        if (!param.comment.empty()) {
            if (attrs.empty()) text += L", ";
            text += L", " + param.comment;
        }
        
        text += L"\r\n";
    }
    
    return text;
}

// 生成易语言格式的DLL命令文本用于剪贴板
std::wstring DllEditor::GenerateDllCommandClipboardText(const std::set<int>& cmdIndices) {
    std::wstring text = L".版本 2\r\n\r\n";
    
    for (int cmdIdx : cmdIndices) {
        if (cmdIdx < 0 || cmdIdx >= (int)m_commands.size()) continue;
        
        const DllCommand& cmd = m_commands[cmdIdx];
        
        // 生成易语言格式：.DLL命令 命令名, 返回值类型, "库文件", "DLL命令名", 公开, 备注
        text += L".DLL命令 " + cmd.commandName;
        
        // 返回值类型（如果有）
        if (!cmd.returnType.empty()) {
            text += L", " + cmd.returnType;
        }
        
        // 库文件名
        if (!cmd.dllFileName.empty()) {
            text += L", \"" + cmd.dllFileName + L"\"";
        }
        
        // DLL命令名
        if (!cmd.dllCommandName.empty()) {
            text += L", \"" + cmd.dllCommandName + L"\"";
        }
        
        // 公开标志
        if (cmd.isPublic) {
            text += L", 公开";
        }
        
        // 备注
        if (!cmd.comment.empty()) {
            text += L", " + cmd.comment;
        }
        
        text += L"\r\n";
        
        // 添加参数 - 易语言格式：.参数 参数名, 类型, 传址/数组, 备注
        for (int paramIdx : cmd.paramLines) {
            if (paramIdx >= 0 && paramIdx < (int)m_allParams.size()) {
                const DllParameter& param = m_allParams[paramIdx];
                text += L".参数 " + param.paramName;
                
                // 类型
                if (!param.paramType.empty()) {
                    text += L", " + param.paramType;
                }
                
                // 属性（传址/数组）
                std::wstring attrs;
                if (param.byRef) {
                    attrs = L"传址";
                }
                if (param.isArray) {
                    if (!attrs.empty()) attrs += L" ";
                    attrs += L"数组";
                }
                if (!attrs.empty()) {
                    text += L", " + attrs;
                }
                
                // 备注
                if (!param.comment.empty()) {
                    if (attrs.empty()) text += L", ";
                    text += L", " + param.comment;
                }
                
                text += L"\r\n";
            }
        }
        
        text += L"\r\n";
    }
    
    return text;
}

void DllEditor::CreateNewDllCommand() {
    // 如果正在编辑，先结束编辑
    if (m_isEditing) {
        EndEditCell(true);
    }
    
    // 清除行选择
    ClearRowSelection();
    
    // 创建快照用于撤销
    CreateSnapshot(L"新建DLL命令");
    
    // 创建新的DLL命令
    DllCommand newCmd;
    newCmd.commandName = GenerateUniqueDllCommandName();
    newCmd.returnType = L"";      // 返回值类型为空
    newCmd.dllFileName = L"";     // DLL文件名为空
    newCmd.dllCommandName = L"";  // DLL中对应命令名为空
    newCmd.isPublic = false;      // 不公开
    newCmd.comment = L"";         // 备注为空
    newCmd.isCollapsed = false;   // 不折叠
    
    // 添加到命令列表末尾
    m_commands.push_back(newCmd);
    
    // 标记为已修改
    m_modified = true;
    
    // 刷新界面
    InvalidateRect(m_hWnd, NULL, FALSE);
}

// === 右键菜单 ===

// 定时器ID用于延迟显示菜单
#define CONTEXT_MENU_TIMER_ID 2

void DllEditor::OnRButtonDown(int x, int y) {
    // 保存右键点击位置
    m_contextMenuX = x;
    m_contextMenuY = y;
    
    // 使用定时器延迟显示菜单（1ms后触发）
    SetTimer(m_hWnd, CONTEXT_MENU_TIMER_ID, 1, NULL);
}

// 右键菜单命令ID
#define ID_MENU_NEW_DLL_COMMAND     1001
#define ID_MENU_CUT                 1002
#define ID_MENU_COPY                1003
#define ID_MENU_PASTE               1004
#define ID_MENU_DELETE              1005
#define ID_MENU_SELECT_ALL          1006

// 自绘菜单项数据结构
struct CustomMenuItem {
    std::wstring text;
    std::wstring shortcut;
    int id;
    bool enabled;
    bool separator;
};

// 自绘菜单窗口类名
static const wchar_t* g_CustomMenuClassName = L"DllEditorCustomMenu";
static bool g_CustomMenuClassRegistered = false;

// 自绘菜单状态
struct CustomMenuState {
    std::vector<CustomMenuItem> items;
    int hoverIndex;
    int selectedId;
    DllEditor* editor;
    HWND hWnd;
};

static CustomMenuState* g_MenuState = nullptr;

// 自绘菜单窗口过程
static LRESULT CALLBACK DllEditorCustomMenuWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE:
            SetCapture(hWnd);
            return 0;
            
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hWnd, &ps);
                
                RECT clientRect;
                GetClientRect(hWnd, &clientRect);
                
                // 创建内存DC进行双缓冲
                HDC memDC = CreateCompatibleDC(hdc);
                HBITMAP memBitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
                HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);
                
                // 颜色定义
                COLORREF bgColor = RGB(37, 37, 38);
                COLORREF hoverColor = RGB(4, 57, 94);
                COLORREF textColor = RGB(204, 204, 204);
                COLORREF disabledColor = RGB(109, 109, 109);
                COLORREF shortcutColor = RGB(133, 133, 133);
                COLORREF separatorColor = RGB(72, 72, 72);
                COLORREF borderColor = RGB(69, 69, 69);
                
                // 绘制背景
                HBRUSH bgBrush = CreateSolidBrush(bgColor);
                FillRect(memDC, &clientRect, bgBrush);
                DeleteObject(bgBrush);
                
                // 绘制边框
                HPEN borderPen = CreatePen(PS_SOLID, 1, borderColor);
                HPEN oldPen = (HPEN)SelectObject(memDC, borderPen);
                HBRUSH nullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
                HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, nullBrush);
                Rectangle(memDC, 0, 0, clientRect.right, clientRect.bottom);
                SelectObject(memDC, oldBrush);
                SelectObject(memDC, oldPen);
                DeleteObject(borderPen);
                
                // 绘制菜单项
                HFONT hFont = CreateFontW(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                    CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Microsoft YaHei UI");
                HFONT oldFont = (HFONT)SelectObject(memDC, hFont);
                SetBkMode(memDC, TRANSPARENT);
                
                if (g_MenuState) {
                    int y = 4;
                    for (size_t i = 0; i < g_MenuState->items.size(); i++) {
                        const CustomMenuItem& item = g_MenuState->items[i];
                        
                        if (item.separator) {
                            // 绘制分隔线
                            HPEN sepPen = CreatePen(PS_SOLID, 1, separatorColor);
                            HPEN oldSepPen = (HPEN)SelectObject(memDC, sepPen);
                            int sepY = y + 4;
                            MoveToEx(memDC, 12, sepY, NULL);
                            LineTo(memDC, clientRect.right - 12, sepY);
                            SelectObject(memDC, oldSepPen);
                            DeleteObject(sepPen);
                            y += 9;
                        } else {
                            RECT itemRect = { 4, y, clientRect.right - 4, y + 26 };
                            
                            // 绘制悬停背景
                            if ((int)i == g_MenuState->hoverIndex && item.enabled) {
                                HBRUSH hoverBrush = CreateSolidBrush(hoverColor);
                                FillRect(memDC, &itemRect, hoverBrush);
                                DeleteObject(hoverBrush);
                            }
                            
                            // 绘制文本
                            SetTextColor(memDC, item.enabled ? textColor : disabledColor);
                            RECT textRect = itemRect;
                            textRect.left += 20;
                            textRect.right -= 8;
                            DrawTextW(memDC, item.text.c_str(), -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                            
                            // 绘制快捷键
                            if (!item.shortcut.empty()) {
                                SetTextColor(memDC, item.enabled ? shortcutColor : disabledColor);
                                DrawTextW(memDC, item.shortcut.c_str(), -1, &textRect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
                            }
                            
                            y += 26;
                        }
                    }
                }
                
                SelectObject(memDC, oldFont);
                DeleteObject(hFont);
                
                // 复制到屏幕
                BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, memDC, 0, 0, SRCCOPY);
                
                SelectObject(memDC, oldBitmap);
                DeleteObject(memBitmap);
                DeleteDC(memDC);
                
                EndPaint(hWnd, &ps);
            }
            return 0;
            
        case WM_MOUSEMOVE:
            {
                if (g_MenuState) {
                    int y = GET_Y_LPARAM(lParam);
                    int newHoverIndex = -1;
                    int currentY = 4;
                    
                    for (size_t i = 0; i < g_MenuState->items.size(); i++) {
                        const CustomMenuItem& item = g_MenuState->items[i];
                        int itemHeight = item.separator ? 9 : 26;
                        
                        if (!item.separator && y >= currentY && y < currentY + itemHeight) {
                            newHoverIndex = (int)i;
                            break;
                        }
                        currentY += itemHeight;
                    }
                    
                    if (newHoverIndex != g_MenuState->hoverIndex) {
                        g_MenuState->hoverIndex = newHoverIndex;
                        InvalidateRect(hWnd, NULL, FALSE);
                    }
                }
            }
            return 0;
            
        case WM_LBUTTONDOWN:
            {
                if (g_MenuState && g_MenuState->hoverIndex >= 0) {
                    const CustomMenuItem& item = g_MenuState->items[g_MenuState->hoverIndex];
                    if (item.enabled && !item.separator) {
                        g_MenuState->selectedId = item.id;
                    }
                }
                ReleaseCapture();
                DestroyWindow(hWnd);
            }
            return 0;
            
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
            ReleaseCapture();
            DestroyWindow(hWnd);
            return 0;
            
        case WM_CAPTURECHANGED:
            // 失去捕获时关闭菜单
            if ((HWND)lParam != hWnd) {
                DestroyWindow(hWnd);
            }
            return 0;
            
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) {
                ReleaseCapture();
                DestroyWindow(hWnd);
            } else if (wParam == VK_RETURN && g_MenuState && g_MenuState->hoverIndex >= 0) {
                const CustomMenuItem& item = g_MenuState->items[g_MenuState->hoverIndex];
                if (item.enabled && !item.separator) {
                    g_MenuState->selectedId = item.id;
                }
                ReleaseCapture();
                DestroyWindow(hWnd);
            } else if (wParam == VK_UP && g_MenuState) {
                // 向上移动
                int newIndex = g_MenuState->hoverIndex - 1;
                while (newIndex >= 0 && g_MenuState->items[newIndex].separator) newIndex--;
                if (newIndex >= 0) {
                    g_MenuState->hoverIndex = newIndex;
                    InvalidateRect(hWnd, NULL, FALSE);
                }
            } else if (wParam == VK_DOWN && g_MenuState) {
                // 向下移动
                int newIndex = g_MenuState->hoverIndex + 1;
                while (newIndex < (int)g_MenuState->items.size() && g_MenuState->items[newIndex].separator) newIndex++;
                if (newIndex < (int)g_MenuState->items.size()) {
                    g_MenuState->hoverIndex = newIndex;
                    InvalidateRect(hWnd, NULL, FALSE);
                }
            }
            return 0;
            
        case WM_DESTROY:
            if (g_MenuState && g_MenuState->hWnd == hWnd) {
                g_MenuState->hWnd = NULL;
            }
            return 0;
            
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

void DllEditor::ShowContextMenu(int x, int y) {
    // 防止重入
    if (m_isShowingContextMenu) return;
    m_isShowingContextMenu = true;
    
    // 注册窗口类（只注册一次）
    if (!g_CustomMenuClassRegistered) {
        WNDCLASSEXW wcex = { 0 };
        wcex.cbSize = sizeof(WNDCLASSEXW);
        wcex.style = CS_DROPSHADOW;
        wcex.lpfnWndProc = DllEditorCustomMenuWndProc;
        wcex.hInstance = GetModuleHandle(NULL);
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.lpszClassName = g_CustomMenuClassName;
        RegisterClassExW(&wcex);
        g_CustomMenuClassRegistered = true;
    }
    
    // 准备菜单项
    std::vector<CustomMenuItem> menuItems;
    
    // 检查是否有有效选择
    bool hasValidSelection = m_hasRowSelection;
    
    // 添加菜单项
    menuItems.push_back({L"新建DLL命令", L"Ctrl+N", ID_MENU_NEW_DLL_COMMAND, true, false});
    menuItems.push_back({L"", L"", 0, false, true});  // 分隔符
    
    if (m_isEditing) {
        bool hasSelection = (m_selectionStart != m_selectionEnd);
        menuItems.push_back({L"剪切", L"Ctrl+X", ID_MENU_CUT, hasSelection, false});
        menuItems.push_back({L"复制", L"Ctrl+C", ID_MENU_COPY, hasSelection, false});
        
        bool hasClipboardText = false;
        if (OpenClipboard(m_hWnd)) {
            hasClipboardText = (GetClipboardData(CF_UNICODETEXT) != NULL);
            CloseClipboard();
        }
        menuItems.push_back({L"粘贴", L"Ctrl+V", ID_MENU_PASTE, hasClipboardText, false});
        menuItems.push_back({L"", L"", 0, false, true});
        menuItems.push_back({L"全选", L"Ctrl+A", ID_MENU_SELECT_ALL, true, false});
    } else {
        // 检查剪贴板是否有DLL命令或参数数据
        bool hasClipboardDllCommandOrParam = false;
        if (OpenClipboard(m_hWnd)) {
            HANDLE hData = GetClipboardData(CF_UNICODETEXT);
            if (hData) {
                wchar_t* pText = (wchar_t*)GlobalLock(hData);
                if (pText) {
                    std::wstring text = pText;
                    // 检查是否包含易语言DLL命令或参数格式
                    hasClipboardDllCommandOrParam = (text.find(L".DLL命令 ") != std::wstring::npos || 
                                                      text.find(L".参数 ") != std::wstring::npos);
                    GlobalUnlock(hData);
                }
            }
            CloseClipboard();
        }
        
        menuItems.push_back({L"剪切", L"Ctrl+X", ID_MENU_CUT, hasValidSelection, false});
        menuItems.push_back({L"复制", L"Ctrl+C", ID_MENU_COPY, hasValidSelection, false});
        menuItems.push_back({L"粘贴", L"Ctrl+V", ID_MENU_PASTE, hasClipboardDllCommandOrParam, false});
        menuItems.push_back({L"", L"", 0, false, true});
        menuItems.push_back({L"删除", L"Delete", ID_MENU_DELETE, hasValidSelection, false});
        menuItems.push_back({L"", L"", 0, false, true});
        menuItems.push_back({L"全选", L"Ctrl+A", ID_MENU_SELECT_ALL, !m_commands.empty(), false});
    }
    
    // 计算菜单大小
    int menuWidth = 200;
    int menuHeight = 8;  // 上下边距
    for (const auto& item : menuItems) {
        menuHeight += item.separator ? 9 : 26;
    }
    
    // 计算菜单位置
    POINT pt = { x, y };
    ClientToScreen(m_hWnd, &pt);
    
    // 确保菜单不超出屏幕
    RECT workArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
    if (pt.x + menuWidth > workArea.right) pt.x = workArea.right - menuWidth;
    if (pt.y + menuHeight > workArea.bottom) pt.y = workArea.bottom - menuHeight;
    
    // 设置全局菜单状态
    CustomMenuState menuState;
    menuState.items = menuItems;
    menuState.hoverIndex = -1;
    menuState.selectedId = 0;
    menuState.editor = this;
    g_MenuState = &menuState;
    
    // 创建菜单窗口
    HWND hMenuWnd = CreateWindowExW(
        WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
        g_CustomMenuClassName,
        NULL,
        WS_POPUP,
        pt.x, pt.y, menuWidth, menuHeight,
        m_hWnd, NULL, GetModuleHandle(NULL), NULL);
    
    menuState.hWnd = hMenuWnd;
    ShowWindow(hMenuWnd, SW_SHOWNA);
    UpdateWindow(hMenuWnd);
    
    // 消息循环，直到菜单关闭
    MSG msg;
    while (IsWindow(hMenuWnd) && GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    int selectedId = menuState.selectedId;
    g_MenuState = nullptr;
    m_isShowingContextMenu = false;
    
    // 执行选中的命令
    if (selectedId > 0) {
        ExecuteMenuCommand(selectedId);
    }
}

void DllEditor::ExecuteMenuCommand(int cmd) {
    switch (cmd) {
        case ID_MENU_NEW_DLL_COMMAND:
            CreateNewDllCommand();
            break;
            
        case ID_MENU_CUT:
            if (m_isEditing && m_selectionStart != m_selectionEnd) {
                // 编辑状态：剪切选中的文本
                int selStart = (std::min)(m_selectionStart, m_selectionEnd);
                int selEnd = (std::max)(m_selectionStart, m_selectionEnd);
                std::wstring selectedText = m_editBuffer.substr(selStart, selEnd - selStart);
                
                if (OpenClipboard(m_hWnd)) {
                    EmptyClipboard();
                    size_t size = (selectedText.length() + 1) * sizeof(wchar_t);
                    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);
                    if (hMem) {
                        wchar_t* pMem = (wchar_t*)GlobalLock(hMem);
                        wcscpy_s(pMem, selectedText.length() + 1, selectedText.c_str());
                        GlobalUnlock(hMem);
                        SetClipboardData(CF_UNICODETEXT, hMem);
                    }
                    CloseClipboard();
                }
                
                m_editBuffer.erase(selStart, selEnd - selStart);
                m_editingCursorPos = selStart;
                m_selectionStart = m_selectionEnd = m_editingCursorPos;
                SetCellValue(m_editingRow, m_editingCol, m_editBuffer);
                OnTextModified();
                InvalidateRect(m_hWnd, NULL, FALSE);
            } else if (m_hasRowSelection) {
                // 非编辑状态：剪切选中的行（先复制再删除）
                int startRow = (std::min)(m_rowSelectStartRow, m_rowSelectEndRow);
                int endRow = (std::max)(m_rowSelectStartRow, m_rowSelectEndRow);
                
                // 安全检查
                if (startRow < 0) startRow = 0;
                int totalRows = GetRowCount();
                if (endRow >= totalRows) endRow = totalRows - 1;
                if (startRow > endRow || m_commands.empty()) {
                    break;
                }
                
                // 分析选中的行类型，判断是只有参数还是包含整个命令
                bool onlyParamData = true;
                int paramCmdIndex = -1;
                std::set<int> selectedParamIndices;
                
                for (int row = startRow; row <= endRow; row++) {
                    int cmdIndex = -1, paramIndex = -1;
                    DllRowType rowType = GetRowType(row, cmdIndex, paramIndex);
                    
                    if (rowType == DllRowType::Separator) {
                        continue;
                    }
                    
                    if (rowType == DllRowType::ParamData) {
                        if (paramCmdIndex == -1) {
                            paramCmdIndex = cmdIndex;
                        } else if (paramCmdIndex != cmdIndex) {
                            onlyParamData = false;
                            break;
                        }
                        if (paramIndex >= 0) {
                            selectedParamIndices.insert(paramIndex);
                        }
                    } else {
                        onlyParamData = false;
                        break;
                    }
                }
                
                std::wstring clipboardText;
                
                if (onlyParamData && !selectedParamIndices.empty() && paramCmdIndex >= 0) {
                    clipboardText = GenerateParameterClipboardText(paramCmdIndex, selectedParamIndices);
                } else {
                    std::set<int> selectedCmdIndices;
                    for (int row = startRow; row <= endRow; row++) {
                        int cmdIndex = -1, paramIndex = -1;
                        DllRowType rowType = GetRowType(row, cmdIndex, paramIndex);
                        if (rowType != DllRowType::Separator && cmdIndex >= 0 && cmdIndex < (int)m_commands.size()) {
                            selectedCmdIndices.insert(cmdIndex);
                        }
                    }
                    clipboardText = GenerateDllCommandClipboardText(selectedCmdIndices);
                }
                
                if (OpenClipboard(m_hWnd)) {
                    EmptyClipboard();
                    size_t size = (clipboardText.length() + 1) * sizeof(wchar_t);
                    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);
                    if (hMem) {
                        wchar_t* pMem = (wchar_t*)GlobalLock(hMem);
                        wcscpy_s(pMem, clipboardText.length() + 1, clipboardText.c_str());
                        GlobalUnlock(hMem);
                        SetClipboardData(CF_UNICODETEXT, hMem);
                    }
                    CloseClipboard();
                }
                
                // 删除选中的行
                DeleteSelectedRows();
            }
            break;
            
        case ID_MENU_COPY:
            if (m_isEditing && m_selectionStart != m_selectionEnd) {
                // 编辑状态：复制选中的文本
                int selStart = (std::min)(m_selectionStart, m_selectionEnd);
                int selEnd = (std::max)(m_selectionStart, m_selectionEnd);
                std::wstring selectedText = m_editBuffer.substr(selStart, selEnd - selStart);
                
                if (OpenClipboard(m_hWnd)) {
                    EmptyClipboard();
                    size_t size = (selectedText.length() + 1) * sizeof(wchar_t);
                    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);
                    if (hMem) {
                        wchar_t* pMem = (wchar_t*)GlobalLock(hMem);
                        wcscpy_s(pMem, selectedText.length() + 1, selectedText.c_str());
                        GlobalUnlock(hMem);
                        SetClipboardData(CF_UNICODETEXT, hMem);
                    }
                    CloseClipboard();
                }
            } else if (m_hasRowSelection) {
                // 非编辑状态：复制选中的行
                int startRow = (std::min)(m_rowSelectStartRow, m_rowSelectEndRow);
                int endRow = (std::max)(m_rowSelectStartRow, m_rowSelectEndRow);
                
                // 安全检查
                if (startRow < 0) startRow = 0;
                int totalRows = GetRowCount();
                if (endRow >= totalRows) endRow = totalRows - 1;
                if (startRow > endRow || m_commands.empty()) {
                    break;
                }
                
                // 分析选中的行类型，判断是只有参数还是包含整个命令
                bool onlyParamData = true;  // 是否只选中了参数数据行
                int paramCmdIndex = -1;     // 参数所属的命令索引
                std::set<int> selectedParamIndices;  // 选中的参数本地索引
                
                for (int row = startRow; row <= endRow; row++) {
                    int cmdIndex = -1, paramIndex = -1;
                    DllRowType rowType = GetRowType(row, cmdIndex, paramIndex);
                    
                    if (rowType == DllRowType::Separator) {
                        continue;  // 跳过间隔行
                    }
                    
                    if (rowType == DllRowType::ParamData) {
                        // 参数数据行
                        if (paramCmdIndex == -1) {
                            paramCmdIndex = cmdIndex;
                        } else if (paramCmdIndex != cmdIndex) {
                            // 跨命令选择参数，视为选择整个命令
                            onlyParamData = false;
                            break;
                        }
                        if (paramIndex >= 0) {
                            selectedParamIndices.insert(paramIndex);
                        }
                    } else {
                        // 不是参数数据行（包括表头、命令行等），需要复制整个命令
                        onlyParamData = false;
                        break;
                    }
                }
                
                std::wstring clipboardText;
                
                if (onlyParamData && !selectedParamIndices.empty() && paramCmdIndex >= 0) {
                    // 只选中了参数数据行，生成参数格式
                    clipboardText = GenerateParameterClipboardText(paramCmdIndex, selectedParamIndices);
                } else {
                    // 选中了整个命令，获取命令索引
                    std::set<int> selectedCmdIndices;
                    for (int row = startRow; row <= endRow; row++) {
                        int cmdIndex = -1, paramIndex = -1;
                        DllRowType rowType = GetRowType(row, cmdIndex, paramIndex);
                        if (rowType != DllRowType::Separator && cmdIndex >= 0 && cmdIndex < (int)m_commands.size()) {
                            selectedCmdIndices.insert(cmdIndex);
                        }
                    }
                    clipboardText = GenerateDllCommandClipboardText(selectedCmdIndices);
                }
                
                if (OpenClipboard(m_hWnd)) {
                    EmptyClipboard();
                    size_t size = (clipboardText.length() + 1) * sizeof(wchar_t);
                    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);
                    if (hMem) {
                        wchar_t* pMem = (wchar_t*)GlobalLock(hMem);
                        wcscpy_s(pMem, clipboardText.length() + 1, clipboardText.c_str());
                        GlobalUnlock(hMem);
                        SetClipboardData(CF_UNICODETEXT, hMem);
                    }
                    CloseClipboard();
                }
            }
            break;
            
        case ID_MENU_PASTE:
            {
                if (!OpenClipboard(m_hWnd)) break;
                
                HANDLE hData = GetClipboardData(CF_UNICODETEXT);
                if (!hData) {
                    CloseClipboard();
                    break;
                }
                
                wchar_t* pText = (wchar_t*)GlobalLock(hData);
                if (!pText) {
                    CloseClipboard();
                    break;
                }
                
                std::wstring pasteText = pText;
                GlobalUnlock(hData);
                CloseClipboard();
                
                // 判断粘贴内容类型
                bool hasDllCommand = (pasteText.find(L".DLL命令 ") != std::wstring::npos);
                bool hasParamOnly = (!hasDllCommand && pasteText.find(L".参数 ") != std::wstring::npos);
                
                if (m_isEditing) {
                    // 正在编辑单元格 - 先保存当前编辑行，因为EndEditCell会重置m_editingRow
                    int currentEditingRow = m_editingRow;
                    
                    if (hasDllCommand) {
                        // 完整DLL命令代码，结束当前编辑，在当前命令后插入新命令
                        EndEditCell(true);
                        
                        // 找到当前命令索引
                        int cmdIndex = -1, paramIndex = -1;
                        GetRowType(currentEditingRow, cmdIndex, paramIndex);
                        
                        // 保存插入位置
                        int insertAfter = cmdIndex;
                        
                        // 解析并插入命令
                        ParseAndInsertDllCommandsAt(pasteText, insertAfter + 1);
                    } else if (hasParamOnly) {
                        // 参数代码，结束当前编辑，添加参数到当前命令
                        EndEditCell(true);
                        
                        int cmdIndex = -1, paramIndex = -1;
                        GetRowType(currentEditingRow, cmdIndex, paramIndex);
                        
                        if (cmdIndex >= 0 && cmdIndex < (int)m_commands.size()) {
                            ParseAndInsertParameters(pasteText, cmdIndex);
                        }
                    } else {
                        // 纯文本，粘贴到当前单元格（移除换行）
                        // 移除换行符
                        std::wstring cleanText;
                        for (wchar_t ch : pasteText) {
                            if (ch != L'\r' && ch != L'\n') {
                                cleanText += ch;
                            }
                        }
                        
                        if (m_selectionStart != m_selectionEnd) {
                            int selStart = (std::min)(m_selectionStart, m_selectionEnd);
                            int selEnd = (std::max)(m_selectionStart, m_selectionEnd);
                            m_editBuffer.erase(selStart, selEnd - selStart);
                            m_editingCursorPos = selStart;
                        }
                        
                        m_editBuffer.insert(m_editingCursorPos, cleanText);
                        m_editingCursorPos += (int)cleanText.length();
                        m_selectionStart = m_selectionEnd = m_editingCursorPos;
                        
                        SetCellValue(m_editingRow, m_editingCol, m_editBuffer);
                        OnTextModified();
                        InvalidateRect(m_hWnd, NULL, FALSE);
                    }
                } else {
                    // 非编辑状态
                    if (hasDllCommand) {
                        // 完整DLL命令代码，插入新命令
                        // 如果有行选择，在选中的命令后插入
                        int insertAfter = -1;
                        if (m_hasRowSelection) {
                            int endRow = (std::max)(m_rowSelectStartRow, m_rowSelectEndRow);
                            int cmdIndex = -1, paramIndex = -1;
                            GetRowType(endRow, cmdIndex, paramIndex);
                            insertAfter = cmdIndex;
                        } else if (!m_commands.empty()) {
                            insertAfter = (int)m_commands.size() - 1;
                        }
                        
                        ParseAndInsertDllCommandsAt(pasteText, insertAfter + 1);
                    } else if (hasParamOnly) {
                        // 参数代码，添加到当前选中的命令
                        int cmdIndex = -1;
                        if (m_hasRowSelection) {
                            int startRow = (std::min)(m_rowSelectStartRow, m_rowSelectEndRow);
                            int paramIndex = -1;
                            GetRowType(startRow, cmdIndex, paramIndex);
                        } else if (!m_commands.empty()) {
                            cmdIndex = 0;  // 默认添加到第一个命令
                        }
                        
                        if (cmdIndex >= 0 && cmdIndex < (int)m_commands.size()) {
                            ParseAndInsertParameters(pasteText, cmdIndex);
                        }
                    }
                    // 纯文本在非编辑状态下不处理
                }
            }
            break;
            
        case ID_MENU_DELETE:
            if (m_hasRowSelection) {
                DeleteSelectedRows();
            }
            break;
            
        case ID_MENU_SELECT_ALL:
            if (m_isEditing) {
                m_selectionStart = 0;
                m_selectionEnd = (int)m_editBuffer.length();
                m_editingCursorPos = m_selectionEnd;
                InvalidateRect(m_hWnd, NULL, FALSE);
            } else {
                if (!m_commands.empty()) {
                    m_rowSelectStartRow = 0;
                    m_rowSelectEndRow = GetRowCount() - 1;
                    m_hasRowSelection = true;
                    InvalidateRect(m_hWnd, NULL, FALSE);
                }
            }
            break;
    }
}