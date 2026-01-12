#include "DllEditor.h"
#include "EditorContext.h"
#include "NameValidator.h"
#include "Theme.h"
#include <sstream>
#include <algorithm>
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
    , m_cursorVisible(true) {
    
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
        // 每个命令：7个固定行 + N个参数行
        int commandRows = 7 + (int)cmd.paramLines.size();
        totalHeight += commandRows * m_rowHeight;
        
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
            int globalRow = GetGlobalRowFromCmdAndSubRow(cmdIdx, subRow);
            
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
        }
    } else {
        // 点击空白区域，结束编辑
        EndEditCell(true);
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
        int cmdRowCount = 7 + (int)m_commands[cmdIdx].paramLines.size();
        
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
    int globalRowIndex = 0;  // 全局行号
    
    // 绘制每个命令的独立表格
    for (size_t cmdIdx = 0; cmdIdx < m_commands.size(); cmdIdx++) {
        const DllCommand& cmd = m_commands[cmdIdx];
        int tableRows = 7 + (int)cmd.paramLines.size();
        int tableHeight = tableRows * m_rowHeight;
        
        // 绘制这个表格的行号和折叠符号
        for (int rowInTable = 0; rowInTable < tableRows; rowInTable++) {
            int rowY = yPos + rowInTable * m_rowHeight;
            
            // 只绘制可见区域的行号
            if (rowY + m_rowHeight >= 0 && rowY < clientRect.bottom) {
                // 绘制行号（右对齐）
                std::wstring lineNumText = std::to_wstring(globalRowIndex + 1);
                RectF lineNumTextRect((REAL)5, (REAL)rowY, 
                                      (REAL)(m_leftMarginWidth - 25), (REAL)m_rowHeight);
                StringFormat rightAlignFormat;
                rightAlignFormat.SetAlignment(StringAlignmentFar);
                rightAlignFormat.SetLineAlignment(StringAlignmentCenter);
                graphics.DrawString(lineNumText.c_str(), -1, &lineNumFont, 
                                  lineNumTextRect, &rightAlignFormat, &lineNumBrush);
                
                // 在第一行绘制折叠符号（放在右边）
                if (rowInTable == 0) {
                    int foldBtnSize = 12;
                    int foldBtnX = m_leftMarginWidth - foldBtnSize - 8;  // 距离右边缘8像素
                    int foldBtnY = rowY + (m_rowHeight - foldBtnSize) / 2;
                    
                    Pen foldPen(ColorFromCOLORREF(g_CurrentTheme.text), 1);
                    graphics.DrawRectangle(&foldPen, foldBtnX, foldBtnY, foldBtnSize, foldBtnSize);
                    graphics.DrawLine(&foldPen, foldBtnX + 3, foldBtnY + foldBtnSize / 2, 
                                    foldBtnX + foldBtnSize - 3, foldBtnY + foldBtnSize / 2);
                }
            }
            globalRowIndex++;
        }
        
        // 绘制表格内容
        if (yPos + tableHeight >= 0 && yPos < clientRect.bottom) {
            DrawCommandTable(hdc, (int)cmdIdx, yPos, clientRect);
        }
        
        yPos += tableHeight;
        
        // 添加表格间隔（最后一个表格除外）
        if (cmdIdx < m_commands.size() - 1) {
            yPos += separatorHeight;
            // 间隔不算行号，不增加globalRowIndex
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
    
    // 计算这个命令表格的总行数（7个固定行 + 参数行）
    int totalRows = 7 + (int)cmd.paramLines.size();
    int tableHeight = totalRows * m_rowHeight;
    
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
    
    // 绘制命令表头（第1行）
    int x = tableStartX;
    for (size_t i = 0; i < cmdCols.size(); i++) {
        RectF cellRect((REAL)x, (REAL)currentY, (REAL)cmdCols[i].width, (REAL)m_rowHeight);
        graphics.FillRectangle(&cmdHeaderBrush, cellRect);
        graphics.DrawString(cmdCols[i].name.c_str(), -1, &headerFont, cellRect, &centerFormat, &textBrush);
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
    graphics.DrawLine(&gridPen, tableStartX, currentY + m_rowHeight, tableStartX + tableWidth, currentY + m_rowHeight);
    currentY += m_rowHeight;
    
    // 第3-4行：库文件名
    
    // 库文件名表头（第3行）
    RectF libHeaderRect((REAL)tableStartX, (REAL)currentY, (REAL)tableWidth, (REAL)m_rowHeight);
    graphics.FillRectangle(&cmdHeaderBrush, libHeaderRect);
    graphics.DrawString(L"库文件名", -1, &headerFont, libHeaderRect, &centerFormat, &textBrush);
    graphics.DrawLine(&gridPen, tableStartX, currentY + m_rowHeight, tableStartX + tableWidth, currentY + m_rowHeight);
    currentY += m_rowHeight;
    
    // 库文件名数据（第4行）
    RectF libDataRect((REAL)tableStartX, (REAL)currentY, (REAL)tableWidth, (REAL)m_rowHeight);
    graphics.FillRectangle(&bgBrush, libDataRect);
    int libTextX = tableStartX + colPadding;
    int libTextY = currentY + m_rowHeight / 2;  // 垂直居中
    
    // 编辑时显示m_editBuffer，否则显示原文本（使用前面计算的isEditingLibFile和dllFileText）
    const std::wstring& libDisplayText = dllFileText;
    
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
    graphics.DrawLine(&gridPen, tableStartX, currentY + m_rowHeight, tableStartX + tableWidth, currentY + m_rowHeight);
    currentY += m_rowHeight;
    
    // 第5-6行：在库中对应的命令名
    // DLL命令名表头（第5行）
    RectF dllHeaderRect((REAL)tableStartX, (REAL)currentY, (REAL)tableWidth, (REAL)m_rowHeight);
    graphics.FillRectangle(&cmdHeaderBrush, dllHeaderRect);
    graphics.DrawString(L"在库中对应的命令名", -1, &headerFont, dllHeaderRect, &centerFormat, &textBrush);
    graphics.DrawLine(&gridPen, tableStartX, currentY + m_rowHeight, tableStartX + tableWidth, currentY + m_rowHeight);
    currentY += m_rowHeight;
    
    // DLL命令名数据（第6行）
    RectF dllDataRect((REAL)tableStartX, (REAL)currentY, (REAL)tableWidth, (REAL)m_rowHeight);
    graphics.FillRectangle(&bgBrush, dllDataRect);
    int dllTextX = tableStartX + colPadding;
    int dllTextY = currentY + m_rowHeight / 2;  // 垂直居中
    
    // 编辑时显示m_editBuffer，否则显示原文本（使用前面计算的isEditingDllCmd和dllCmdText）
    const std::wstring& dllDisplayText = dllCmdText;
    
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
    graphics.DrawLine(&gridPen, tableStartX, currentY + m_rowHeight, tableStartX + tableWidth, currentY + m_rowHeight);
    currentY += m_rowHeight;
    
    // 第7行及以后：参数表头 + 参数数据
    // (参数列已经在前面定义)
    
    // 参数表头（第7行）
    x = tableStartX;
    for (size_t i = 0; i < paramCols.size(); i++) {
        RectF cellRect((REAL)x, (REAL)currentY, (REAL)paramCols[i].width, (REAL)m_rowHeight);
        graphics.FillRectangle(&paramHeaderBrush, cellRect);
        graphics.DrawString(paramCols[i].name.c_str(), -1, &headerFont, cellRect, &centerFormat, &textBrush);
        graphics.DrawLine(&gridPen, x + paramCols[i].width, currentY, x + paramCols[i].width, currentY + m_rowHeight);
        x += paramCols[i].width;
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
