#include "DataTypeEditor.h"
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
DataTypeEditor::DataTypeEditor(HWND hWnd, EditorContext* context)
    : TableEditor(hWnd, context)
    , m_leftMarginWidth(75)
    , m_lineNumberAreaWidth(55)
    , m_clientWidth(0)
    , m_clientHeight(0)
    , m_isRowSelecting(false)
    , m_rowSelectStartRow(-1)
    , m_rowSelectEndRow(-1)
    , m_hasRowSelection(false)
    , m_showTypeCompletion(false)
    , m_typeCompletionSelectedIndex(0)
    , m_typeCompletionScrollOffset(0)
    , m_skipNextSpace(false)
    , m_cachedDtCol1Width(120)
    , m_cachedDtCol2Width(80)
    , m_cachedDtCol3Width(200)
    , m_cachedMemberCol1Width(120)
    , m_cachedMemberCol2Width(80)
    , m_cachedMemberCol3Width(40)
    , m_cachedMemberCol4Width(40)
    , m_cachedMemberCol5Width(120)
    , m_cursorVisible(true) {
    
    m_currentCellRect = {0, 0, 0, 0};
    m_typeCompletionRect = {0, 0, 0, 0};
    
    m_fileName = L"未命名.edt";
    
    // 数据类型编辑器不使用全局表头，每个数据类型有自己的表头
    m_headerHeight = 0;
    
    // 启动光标闪烁定时器
    SetTimer(m_hWnd, CURSOR_TIMER_ID, CURSOR_BLINK_INTERVAL, NULL);
}

DataTypeEditor::~DataTypeEditor() {
    // 停止光标闪烁定时器
    KillTimer(m_hWnd, CURSOR_TIMER_ID);
}

// === 重写 TableEditor 的虚接口 ===

std::vector<ColumnDef> DataTypeEditor::GetColumnDefs() const {
    // 统一的列定义，兼容所有行类型
    // 数据类型行：数据类型名 | 公开 | 备注
    // 成员行：成员名 | 类型 | 传址 | 数组 | 备注
    std::vector<ColumnDef> cols;
    cols.push_back(ColumnDef(L"列1", 200));       // 数据类型名/成员名
    cols.push_back(ColumnDef(L"列2", 120));       // 公开/类型
    cols.push_back(ColumnDef(L"列3", 80, true, true));  // 传址（复选框）
    cols.push_back(ColumnDef(L"列4", 80, true, true));  // 数组（复选框）
    cols.push_back(ColumnDef(L"列5", 200));       // 备注
    return cols;
}

int DataTypeEditor::GetRowCount() const {
    int count = 0;
    for (size_t i = 0; i < m_dataTypes.size(); i++) {
        const auto& dt = m_dataTypes[i];
        // 每个数据类型占用：
        // 1. 数据类型表头
        // 2. 数据类型数据行
        count += 2;  // 表头 + 数据行
        
        if (dt.isExpanded) {
            // 3. 成员表头
            // 4. N个成员数据行
            count += 1 + (int)dt.members.size();
        }
    }
    return count;
}

int DataTypeEditor::GetTotalContentHeight() const {
    int totalHeight = 0;
    const int separatorHeight = 20;
    
    for (size_t i = 0; i < m_dataTypes.size(); i++) {
        const auto& dt = m_dataTypes[i];
        // 数据类型表头 + 数据类型数据行 + 成员表头
        totalHeight += m_rowHeight * 3;
        
        if (dt.isExpanded) {
            totalHeight += m_rowHeight * (int)dt.members.size();
        }
        
        // 间隔行
        if (i < m_dataTypes.size() - 1) {
            totalHeight += separatorHeight;
        }
    }
    
    return totalHeight;
}

int DataTypeEditor::GetTotalContentWidth() const {
    // 左边距 + 所有列宽
    int width = m_leftMarginWidth;
    auto cols = GetColumnDefs();
    for (const auto& col : cols) {
        width += col.width;
    }
    return width;
}

DataTypeEditor::RowInfo DataTypeEditor::GetRowInfo(int row) const {
    RowInfo info;
    info.rowType = DataTypeRowType::Separator;
    info.dataTypeIndex = -1;
    info.memberIndex = -1;
    
    int currentRow = 0;
    
    for (int i = 0; i < (int)m_dataTypes.size(); i++) {
        const auto& dt = m_dataTypes[i];
        
        // 数据类型表头
        if (currentRow == row) {
            info.rowType = DataTypeRowType::TypeHeader;
            info.dataTypeIndex = i;
            return info;
        }
        currentRow++;
        
        // 数据类型数据行
        if (currentRow == row) {
            info.rowType = DataTypeRowType::TypeData;
            info.dataTypeIndex = i;
            return info;
        }
        currentRow++;
        
        // 只有展开时才有成员表头和成员数据行
        if (dt.isExpanded) {
            // 成员表头
            if (currentRow == row) {
                info.rowType = DataTypeRowType::MemberHeader;
                info.dataTypeIndex = i;
                return info;
            }
            currentRow++;
            
            // 成员数据行
            for (int j = 0; j < (int)dt.members.size(); j++) {
                if (currentRow == row) {
                    info.rowType = DataTypeRowType::MemberData;
                    info.dataTypeIndex = i;
                    info.memberIndex = j;
                    return info;
                }
                currentRow++;
            }
        }
        
        // 间隔行（最后一个数据类型除外）- 不计入行号
    }
    
    return info;
}

int DataTypeEditor::GetRowIndexFromInfo(int dataTypeIndex, int memberIndex) const {
    if (dataTypeIndex < 0 || dataTypeIndex >= (int)m_dataTypes.size()) {
        return -1;
    }
    
    int row = 0;
    
    for (int i = 0; i < dataTypeIndex; i++) {
        const auto& dt = m_dataTypes[i];
        row += 3;  // 表头 + 数据行 + 成员表头
        if (dt.isExpanded) {
            row += (int)dt.members.size();
        }
        row += 1;  // 间隔行
    }
    
    if (memberIndex < 0) {
        return row + 1;  // 返回数据类型数据行
    }
    
    row += 3;  // 跳过表头 + 数据行 + 成员表头
    
    if (m_dataTypes[dataTypeIndex].isExpanded && memberIndex < (int)m_dataTypes[dataTypeIndex].members.size()) {
        row += memberIndex;
        return row;
    }
    
    return -1;
}

std::wstring DataTypeEditor::GetCellText(int row, int col) const {
    RowInfo info = GetRowInfo(row);
    
    if (info.dataTypeIndex < 0 || info.dataTypeIndex >= (int)m_dataTypes.size()) {
        return L"";
    }
    
    const DataType& dt = m_dataTypes[info.dataTypeIndex];
    
    switch (info.rowType) {
        case DataTypeRowType::TypeHeader:
            // 数据类型表头（3列）
            switch (col) {
                case 0: return L"数据类型名";
                case 1: return L"公开";
                case 2: return L"备注";
                default: return L"";
            }
            
        case DataTypeRowType::TypeData:
            // 数据类型数据行（3列）
            switch (col) {
                case 0: return dt.name;  // 不再添加展开/折叠符号
                case 1: return dt.isPublic ? L"✓" : L"";
                case 2: return dt.comment;
                default: return L"";
            }
            
        case DataTypeRowType::MemberHeader:
            // 成员表头（5列）
            switch (col) {
                case 0: return L"成员名";  // 不再添加缩进
                case 1: return L"类型";
                case 2: return L"传址";
                case 3: return L"数组";
                case 4: return L"备注";
                default: return L"";
            }
            
        case DataTypeRowType::MemberData:
            // 成员数据行（5列）
            if (info.memberIndex >= 0 && info.memberIndex < (int)dt.members.size()) {
                const DataTypeMember& member = dt.members[info.memberIndex];
                switch (col) {
                    case 0: return member.name;  // 不再添加缩进
                    case 1: return member.type;
                    case 2: return member.byRef ? L"✓" : L"";
                    case 3: return member.isArray ? L"✓" : L"";
                    case 4: return member.comment;
                    default: return L"";
                }
            }
            return L"";
            
        case DataTypeRowType::Separator:
        default:
            return L"";
    }
}

void DataTypeEditor::SetCellValue(int row, int col, const std::wstring& value) {
    RowInfo info = GetRowInfo(row);
    
    if (info.dataTypeIndex < 0 || info.dataTypeIndex >= (int)m_dataTypes.size()) {
        return;
    }
    
    DataType& dt = m_dataTypes[info.dataTypeIndex];
    
    if (info.rowType == DataTypeRowType::TypeData) {
        // 数据类型数据行（3列：名称、公开、备注）
        switch (col) {
            case 0: {
                // 数据类型名
                dt.name = value;
                break;
            }
            case 2: dt.comment = value; break;  // 备注在第3列(col=2)
        }
        
        // 通知数据变更
        if (m_pContext && col == 0) {
            m_pContext->NotifyDataChanged(EditorFileType::EdtDataType, value);
        }
    } else if (info.rowType == DataTypeRowType::MemberData) {
        // 成员数据行（5列：名称、类型、传址、数组、备注）
        if (info.memberIndex >= 0 && info.memberIndex < (int)dt.members.size()) {
            DataTypeMember& member = dt.members[info.memberIndex];
            
            switch (col) {
                case 0: {
                    // 成员名（不需要处理缩进，缩进只是显示用）
                    member.name = value;
                    break;
                }
                case 1: member.type = value; break;
                case 4: member.comment = value; break;  // 备注在第5列(col=4)
            }
        }
    }
    
    SetModified(true);
}

bool DataTypeEditor::IsCellTextEditable(int row, int col) const {
    RowInfo info = GetRowInfo(row);
    
    // 数据类型数据行: col==1 是公开复选框
    if (info.rowType == DataTypeRowType::TypeData && col == 1) {
        return false;
    }
    
    // 成员数据行: col==2 是传址复选框, col==3 是数组复选框
    if (info.rowType == DataTypeRowType::MemberData && (col == 2 || col == 3)) {
        return false;
    }
    
    return true;
}

bool DataTypeEditor::GetCellCheckState(int row, int col) const {
    RowInfo info = GetRowInfo(row);
    
    if (info.dataTypeIndex < 0 || info.dataTypeIndex >= (int)m_dataTypes.size()) {
        return false;
    }
    
    const DataType& dt = m_dataTypes[info.dataTypeIndex];
    
    if (info.rowType == DataTypeRowType::TypeData && col == 1) {
        return dt.isPublic;
    }
    
    if (info.rowType == DataTypeRowType::MemberData) {
        if (info.memberIndex >= 0 && info.memberIndex < (int)dt.members.size()) {
            const DataTypeMember& member = dt.members[info.memberIndex];
            if (col == 2) return member.byRef;
            if (col == 3) return member.isArray;
        }
    }
    
    return false;
}

void DataTypeEditor::SetCellCheckState(int row, int col, bool checked) {
    RowInfo info = GetRowInfo(row);
    
    if (info.dataTypeIndex < 0 || info.dataTypeIndex >= (int)m_dataTypes.size()) {
        return;
    }
    
    DataType& dt = m_dataTypes[info.dataTypeIndex];
    
    if (info.rowType == DataTypeRowType::TypeData && col == 1) {
        dt.isPublic = checked;
        SetModified(true);
    }
    
    if (info.rowType == DataTypeRowType::MemberData) {
        if (info.memberIndex >= 0 && info.memberIndex < (int)dt.members.size()) {
            DataTypeMember& member = dt.members[info.memberIndex];
            if (col == 2) {
                member.byRef = checked;
                SetModified(true);
            }
            if (col == 3) {
                member.isArray = checked;
                SetModified(true);
            }
        }
    }
}

std::wstring DataTypeEditor::ValidateCell(int row, int col, const std::wstring& value) const {
    RowInfo info = GetRowInfo(row);
    
    if (info.rowType == DataTypeRowType::TypeData) {
        // 验证数据类型名
        if (col == 0) {
            std::wstring cleanValue = value;
            if (cleanValue.length() > 2 && (cleanValue[0] == L'▼' || cleanValue[0] == L'▶')) {
                cleanValue = cleanValue.substr(2);
            }
            if (cleanValue.empty()) {
                return L"数据类型名不能为空";
            }
            
            if (m_pContext && m_pContext->GetNameValidator()) {
                return m_pContext->GetNameValidator()->ValidateName(
                    cleanValue, SymbolType::DataType, m_filePath);
            }
        }
    } else if (info.rowType == DataTypeRowType::MemberData) {
        // 验证成员名
        if (col == 0) {
            std::wstring cleanValue = value;
            if (cleanValue.length() >= 4 && cleanValue.substr(0, 4) == L"    ") {
                cleanValue = cleanValue.substr(4);
            }
            if (cleanValue.empty()) {
                return L"成员名不能为空";
            }
        }
        
        // 验证成员类型
        if (col == 1 && value.empty()) {
            return L"成员类型不能为空";
        }
    }
    
    return L"";
}

std::vector<std::wstring> DataTypeEditor::SerializeState() const {
    std::vector<std::wstring> state;
    
    state.push_back(std::to_wstring(m_dataTypes.size()));
    
    for (const auto& dt : m_dataTypes) {
        state.push_back(dt.name);
        state.push_back(dt.isPublic ? L"1" : L"0");
        state.push_back(dt.comment);
        state.push_back(dt.isExpanded ? L"1" : L"0");
        state.push_back(std::to_wstring(dt.members.size()));
        
        for (const auto& member : dt.members) {
            state.push_back(member.name);
            state.push_back(member.type);
            state.push_back(member.byRef ? L"1" : L"0");
            state.push_back(member.isArray ? L"1" : L"0");
            state.push_back(member.comment);
        }
    }
    
    return state;
}

void DataTypeEditor::RestoreState(const std::vector<std::wstring>& state) {
    if (state.empty()) return;
    
    size_t index = 0;
    m_dataTypes.clear();
    
    if (index < state.size()) {
        int dtCount = std::stoi(state[index++]);
        
        for (int i = 0; i < dtCount && index < state.size(); i++) {
            DataType dt;
            dt.name = state[index++];
            dt.isPublic = (state[index++] == L"1");
            dt.comment = state[index++];
            dt.isExpanded = (state[index++] == L"1");
            
            int memberCount = std::stoi(state[index++]);
            
            for (int j = 0; j < memberCount && index + 4 < state.size(); j++) {
                DataTypeMember member;
                member.name = state[index++];
                member.type = state[index++];
                member.byRef = (state[index++] == L"1");
                member.isArray = (state[index++] == L"1");
                member.comment = state[index++];
                
                dt.members.push_back(member);
            }
            
            m_dataTypes.push_back(dt);
        }
    }
}

void DataTypeEditor::ParseContent(const std::vector<std::wstring>& lines) {
    m_dataTypes.clear();
    DataType* currentType = nullptr;
    
    // 辅助函数：分割逗号分隔的字符串
    auto splitByComma = [](const std::wstring& str) -> std::vector<std::wstring> {
        std::vector<std::wstring> parts;
        std::wstring current;
        
        for (wchar_t ch : str) {
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
        
        // 跳过版本行
        if (line.find(L".版本") == 0 || line.find(L"版本") == 0) continue;
        
        // 判断是数据类型还是成员
        // 支持两种格式: ".数据类型 " 和 "数据类型 "
        bool isDataType = false;
        std::wstring rest;
        
        if (line.find(L".数据类型 ") == 0) {
            isDataType = true;
            rest = line.substr(6);  // 跳过 ".数据类型 "
        } else if (line.find(L"数据类型 ") == 0) {
            isDataType = true;
            rest = line.substr(5);  // 跳过 "数据类型 "
        }
        
        if (isDataType) {
            // 新数据类型: 数据类型 名称, 公开, 备注
            DataType dt;
            
            auto parts = splitByComma(rest);
            
            if (parts.size() >= 1) dt.name = parts[0];
            if (parts.size() >= 2) dt.isPublic = (parts[1] == L"公开");
            if (parts.size() >= 3) dt.comment = parts[2];
            
            m_dataTypes.push_back(dt);
            currentType = &m_dataTypes.back();
            continue;
        }
        
        // 判断是否是成员行
        // 支持两种格式: ".成员 " 和 "成员 "
        bool isMember = false;
        size_t memberStartPos = 0;
        
        // 检查带缩进的成员行
        if (line.find(L"    .成员 ") == 0 || line.find(L"\t.成员 ") == 0) {
            isMember = true;
            memberStartPos = line.find(L".成员 ") + 4;  // 跳过 ".成员 "
        } else if (line.find(L"    成员 ") == 0 || line.find(L"\t成员 ") == 0) {
            isMember = true;
            memberStartPos = line.find(L"成员 ") + 3;  // 跳过 "成员 "
        }
        
        if (isMember && currentType) {
            DataTypeMember member;
            
            rest = line.substr(memberStartPos);
            auto parts = splitByComma(rest);
            
            if (parts.size() >= 1) member.name = parts[0];
            if (parts.size() >= 2) member.type = parts[1];
            if (parts.size() >= 3) member.byRef = (parts[2] == L"传址");
            if (parts.size() >= 4) member.isArray = !parts[3].empty() && parts[3] != L"" && parts[3] != L"0";
            if (parts.size() >= 5) member.comment = parts[4];
            
            currentType->members.push_back(member);
        }
    }
}

std::vector<std::wstring> DataTypeEditor::GenerateContent() const {
    std::vector<std::wstring> lines;
    
    lines.push_back(L".版本 2");
    lines.push_back(L"");
    
    for (const auto& dt : m_dataTypes) {
        std::wstring line = L".数据类型 " + dt.name;
        line += L", " + std::wstring(dt.isPublic ? L"公开" : L"私有");
        if (!dt.comment.empty()) {
            line += L", " + dt.comment;
        }
        lines.push_back(line);
        
        for (const auto& member : dt.members) {
            std::wstring memberLine = L"    .成员 " + member.name + L", " + member.type;
            
            // 传址
            if (member.byRef) {
                memberLine += L", 传址";
            } else {
                memberLine += L", ";
            }
            
            // 数组
            if (member.isArray) {
                memberLine += L", \"0\"";  // 数组标记
            } else {
                memberLine += L", ";
            }
            
            // 备注
            if (!member.comment.empty()) {
                memberLine += L", " + member.comment;
            }
            
            lines.push_back(memberLine);
        }
        
        lines.push_back(L"");
    }
    
    return lines;
}

void DataTypeEditor::InsertRow(int afterRow) {
    RowInfo info = GetRowInfo(afterRow);
    
    if (info.rowType == DataTypeRowType::TypeHeader || 
        info.rowType == DataTypeRowType::TypeData ||
        info.rowType == DataTypeRowType::Separator ||
        afterRow < 0) {
        // 插入新数据类型
        InsertDataType(info.dataTypeIndex);
    } else if (info.rowType == DataTypeRowType::MemberHeader ||
               info.rowType == DataTypeRowType::MemberData) {
        // 在当前数据类型中插入成员
        InsertMember(info.dataTypeIndex);
    }
    
    SetModified(true);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void DataTypeEditor::DeleteRow(int row) {
    RowInfo info = GetRowInfo(row);
    
    if (info.dataTypeIndex < 0 || info.dataTypeIndex >= (int)m_dataTypes.size()) {
        return;
    }
    
    if (info.rowType == DataTypeRowType::TypeData) {
        // 删除整个数据类型
        m_dataTypes.erase(m_dataTypes.begin() + info.dataTypeIndex);
        SetModified(true);
    } else if (info.rowType == DataTypeRowType::MemberData) {
        // 删除成员
        DataType& dt = m_dataTypes[info.dataTypeIndex];
        if (info.memberIndex >= 0 && info.memberIndex < (int)dt.members.size()) {
            dt.members.erase(dt.members.begin() + info.memberIndex);
            SetModified(true);
        }
    }
    
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void DataTypeEditor::InsertDataType(int afterIndex) {
    DataType newType;
    newType.name = L"新数据类型";
    newType.isPublic = true;
    newType.comment = L"";
    newType.isExpanded = true;
    
    // 添加一个默认成员
    DataTypeMember member;
    member.name = L"成员1";
    member.type = L"整数型";
    member.byRef = false;
    member.isArray = false;
    newType.members.push_back(member);
    
    if (afterIndex < 0 || afterIndex >= (int)m_dataTypes.size()) {
        m_dataTypes.push_back(newType);
    } else {
        m_dataTypes.insert(m_dataTypes.begin() + afterIndex + 1, newType);
    }
}

void DataTypeEditor::InsertMember(int dataTypeIndex) {
    if (dataTypeIndex < 0 || dataTypeIndex >= (int)m_dataTypes.size()) {
        return;
    }
    
    DataTypeMember newMember;
    newMember.name = L"新成员";
    newMember.type = L"整数型";
    newMember.byRef = false;
    newMember.isArray = false;
    newMember.comment = L"";
    
    m_dataTypes[dataTypeIndex].members.push_back(newMember);
    m_dataTypes[dataTypeIndex].isExpanded = true;
}

void DataTypeEditor::DeleteMember(int row) {
    RowInfo info = GetRowInfo(row);
    
    if (info.rowType == DataTypeRowType::MemberData &&
        info.dataTypeIndex >= 0 && info.dataTypeIndex < (int)m_dataTypes.size()) {
        DataType& dt = m_dataTypes[info.dataTypeIndex];
        if (info.memberIndex >= 0 && info.memberIndex < (int)dt.members.size()) {
            dt.members.erase(dt.members.begin() + info.memberIndex);
            SetModified(true);
        }
    }
}

void DataTypeEditor::ToggleExpand(int row) {
    RowInfo info = GetRowInfo(row);
    
    if (info.rowType == DataTypeRowType::TypeData &&
        info.dataTypeIndex >= 0 && info.dataTypeIndex < (int)m_dataTypes.size()) {
        m_dataTypes[info.dataTypeIndex].isExpanded = !m_dataTypes[info.dataTypeIndex].isExpanded;
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

// === 文件操作重写（支持JSON） ===

bool DataTypeEditor::LoadFile(const std::wstring& path) {
    std::wstring ext = path.substr(path.find_last_of(L'.'));
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == L".json") {
        // 直接使用宽字符路径打开文件，避免中文路径编码问题
        std::ifstream file(path.c_str());
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

bool DataTypeEditor::SaveFile(const std::wstring& path) {
    std::wstring ext = path.substr(path.find_last_of(L'.'));
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == L".json") {
        // 直接使用宽字符路径打开文件，避免中文路径编码问题
        std::ofstream file(path.c_str());
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

json DataTypeEditor::ToJson() const {
    json j;
    j["version"] = 2;
    j["dataTypes"] = json::array();
    
    for (const auto& dt : m_dataTypes) {
        json item;
        item["name"] = WStringToUtf8String(dt.name);
        item["public"] = dt.isPublic;
        item["comment"] = WStringToUtf8String(dt.comment);
        item["members"] = json::array();
        
        for (const auto& member : dt.members) {
            json memberItem;
            memberItem["name"] = WStringToUtf8String(member.name);
            memberItem["type"] = WStringToUtf8String(member.type);
            memberItem["byRef"] = member.byRef;
            memberItem["array"] = member.isArray;
            memberItem["comment"] = WStringToUtf8String(member.comment);
            
            item["members"].push_back(memberItem);
        }
        
        j["dataTypes"].push_back(item);
    }
    
    return j;
}

void DataTypeEditor::FromJson(const json& j) {
    m_dataTypes.clear();
    
    if (j.contains("dataTypes") && j["dataTypes"].is_array()) {
        for (const auto& item : j["dataTypes"]) {
            DataType dt;
            
            if (item.contains("name")) dt.name = Utf8StringToWString(item["name"]);
            if (item.contains("public")) dt.isPublic = item["public"];
            if (item.contains("comment")) dt.comment = Utf8StringToWString(item["comment"]);
            dt.isExpanded = true;
            
            if (item.contains("members") && item["members"].is_array()) {
                for (const auto& memberItem : item["members"]) {
                    DataTypeMember member;
                    
                    if (memberItem.contains("name")) member.name = Utf8StringToWString(memberItem["name"]);
                    if (memberItem.contains("type")) member.type = Utf8StringToWString(memberItem["type"]);
                    if (memberItem.contains("byRef")) member.byRef = memberItem["byRef"];
                    if (memberItem.contains("array")) member.isArray = memberItem["array"];
                    if (memberItem.contains("comment")) member.comment = Utf8StringToWString(memberItem["comment"]);
                    
                    dt.members.push_back(member);
                }
            }
            
            m_dataTypes.push_back(dt);
        }
    }
}

std::wstring DataTypeEditor::WStringToUtf8String(const std::wstring& wstr) const {
    if (wstr.empty()) return L"";
    
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (len <= 0) return L"";
    
    std::string utf8Str(len - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8Str[0], len, NULL, NULL);
    
    return std::wstring(utf8Str.begin(), utf8Str.end());
}

std::wstring DataTypeEditor::Utf8StringToWString(const std::string& str) const {
    if (str.empty()) return L"";
    
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    if (len <= 0) return L"";
    
    std::wstring wstr(len - 1, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], len);
    
    return wstr;
}

// === Protected 方法 ===

std::wstring* DataTypeEditor::GetEditingCellTextPtr() {
    if (!m_isEditing || m_editingRow < 0) {
        return nullptr;
    }
    
    // 直接返回已有的编辑缓冲区，不重新加载
    // 初始化在StartEditCell中完成
    return &m_editBuffer;
}

// === 数据类型补全相关 ===

std::vector<std::wstring> DataTypeEditor::GetAllDataTypes() const {
    return LibraryParser::GetInstance().GetAllDataTypeNames();
}

void DataTypeEditor::UpdateTypeCompletion(const std::wstring& text) {
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

void DataTypeEditor::ApplyTypeCompletion() {
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

void DataTypeEditor::HideTypeCompletion() {
    m_showTypeCompletion = false;
    m_typeCompletionItems.clear();
    m_typeCompletionSelectedIndex = 0;
    m_typeCompletionScrollOffset = 0;
}

// === 输入处理 ===

void DataTypeEditor::HitTest(int x, int y, int& row, int& col, RECT& cellRect) {
    row = -1;
    col = -1;
    cellRect = {0, 0, 0, 0};
    
    // 检查是否在左边距区域
    if (x < m_leftMarginWidth) {
        return;
    }
    
    // 表格左偏移
    const int tableLeftOffset = 10;
    int tableStartX = m_leftMarginWidth + tableLeftOffset - m_scrollX;
    
    // 计算行和数据类型索引
    int currentY = -m_scrollY;
    const int separatorHeight = 20;
    int currentRow = 0;
    int foundDtIndex = -1;
    int subRowInDt = -1;  // 0=表头, 1=数据, 2=成员表头, 3+=成员数据
    
    for (size_t i = 0; i < m_dataTypes.size(); i++) {
        const auto& dt = m_dataTypes[i];
        
        // 数据类型表头 (subRow=0)
        if (y >= currentY && y < currentY + m_rowHeight) {
            row = currentRow;
            foundDtIndex = (int)i;
            subRowInDt = 0;
            cellRect.top = currentY;
            cellRect.bottom = currentY + m_rowHeight;
            break;
        }
        currentY += m_rowHeight;
        currentRow++;
        
        // 数据类型数据行 (subRow=1)
        if (y >= currentY && y < currentY + m_rowHeight) {
            row = currentRow;
            foundDtIndex = (int)i;
            subRowInDt = 1;
            cellRect.top = currentY;
            cellRect.bottom = currentY + m_rowHeight;
            break;
        }
        currentY += m_rowHeight;
        currentRow++;
        
        if (dt.isExpanded) {
            // 成员表头 (subRow=2)
            if (y >= currentY && y < currentY + m_rowHeight) {
                row = currentRow;
                foundDtIndex = (int)i;
                subRowInDt = 2;
                cellRect.top = currentY;
                cellRect.bottom = currentY + m_rowHeight;
                break;
            }
            currentY += m_rowHeight;
            currentRow++;
            
            // 成员数据行 (subRow=3+)
            for (int j = 0; j < (int)dt.members.size(); j++) {
                if (y >= currentY && y < currentY + m_rowHeight) {
                    row = currentRow;
                    foundDtIndex = (int)i;
                    subRowInDt = 3 + j;
                    cellRect.top = currentY;
                    cellRect.bottom = currentY + m_rowHeight;
                    break;
                }
                currentY += m_rowHeight;
                currentRow++;
            }
            if (row >= 0) break;
        }
        
        // 间隔行
        if (i < m_dataTypes.size() - 1) {
            if (y >= currentY && y < currentY + separatorHeight) {
                // 在间隔行上
                return;
            }
            currentY += separatorHeight;
        }
    }
    
    if (row < 0 || foundDtIndex < 0) return;
    
    // 使用缓存的列宽计算点击的列
    // 数据类型行（表头和数据）有3列：名称、公开、备注
    // 成员行（表头和数据）有5列：名称、类型、传址、数组、备注
    
    int colX = tableStartX;
    
    if (subRowInDt <= 1) {
        // 数据类型行（表头或数据）：3列
        // 列0：数据类型名
        if (x >= colX && x < colX + m_cachedDtCol1Width) {
            col = 0;
            cellRect.left = colX;
            cellRect.right = colX + m_cachedDtCol1Width;
            return;
        }
        colX += m_cachedDtCol1Width;
        
        // 列1：公开
        if (x >= colX && x < colX + m_cachedDtCol2Width) {
            col = 1;
            cellRect.left = colX;
            cellRect.right = colX + m_cachedDtCol2Width;
            return;
        }
        colX += m_cachedDtCol2Width;
        
        // 列2：备注
        if (x >= colX && x < colX + m_cachedDtCol3Width) {
            col = 2;
            cellRect.left = colX;
            cellRect.right = colX + m_cachedDtCol3Width;
            return;
        }
    } else {
        // 成员行（表头或数据）：5列
        // 列0：成员名
        if (x >= colX && x < colX + m_cachedMemberCol1Width) {
            col = 0;
            cellRect.left = colX;
            cellRect.right = colX + m_cachedMemberCol1Width;
            return;
        }
        colX += m_cachedMemberCol1Width;
        
        // 列1：类型
        if (x >= colX && x < colX + m_cachedMemberCol2Width) {
            col = 1;
            cellRect.left = colX;
            cellRect.right = colX + m_cachedMemberCol2Width;
            return;
        }
        colX += m_cachedMemberCol2Width;
        
        // 列2：传址
        if (x >= colX && x < colX + m_cachedMemberCol3Width) {
            col = 2;
            cellRect.left = colX;
            cellRect.right = colX + m_cachedMemberCol3Width;
            return;
        }
        colX += m_cachedMemberCol3Width;
        
        // 列3：数组
        if (x >= colX && x < colX + m_cachedMemberCol4Width) {
            col = 3;
            cellRect.left = colX;
            cellRect.right = colX + m_cachedMemberCol4Width;
            return;
        }
        colX += m_cachedMemberCol4Width;
        
        // 列4：备注
        if (x >= colX && x < colX + m_cachedMemberCol5Width) {
            col = 4;
            cellRect.left = colX;
            cellRect.right = colX + m_cachedMemberCol5Width;
            return;
        }
    }
}

int DataTypeEditor::HitTestRow(int x, int y) {
    // 计算点击的行号（只看Y坐标）
    int currentY = -m_scrollY;
    const int separatorHeight = 20;
    int currentRow = 0;
    
    for (size_t i = 0; i < m_dataTypes.size(); i++) {
        const auto& dt = m_dataTypes[i];
        
        // 数据类型表头
        if (y >= currentY && y < currentY + m_rowHeight) {
            return currentRow;
        }
        currentY += m_rowHeight;
        currentRow++;
        
        // 数据类型数据行
        if (y >= currentY && y < currentY + m_rowHeight) {
            return currentRow;
        }
        currentY += m_rowHeight;
        currentRow++;
        
        if (dt.isExpanded) {
            // 成员表头
            if (y >= currentY && y < currentY + m_rowHeight) {
                return currentRow;
            }
            currentY += m_rowHeight;
            currentRow++;
            
            // 成员数据行
            for (int j = 0; j < (int)dt.members.size(); j++) {
                if (y >= currentY && y < currentY + m_rowHeight) {
                    return currentRow;
                }
                currentY += m_rowHeight;
                currentRow++;
            }
        }
        
        // 间隔行
        if (i < m_dataTypes.size() - 1) {
            if (y >= currentY && y < currentY + separatorHeight) {
                return -1;  // 间隔行不可选
            }
            currentY += separatorHeight;
        }
    }
    
    return -1;
}

void DataTypeEditor::OnLButtonDown(int x, int y) {
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
    
    // 检查展开/折叠图标点击（在左边距区域）
    // 折叠按钮绘制在表头行(TypeHeader)的左边距右侧
    int foldBtnSize = 12;
    int foldBtnLeft = m_leftMarginWidth - foldBtnSize - 8;
    int foldBtnRight = m_leftMarginWidth - 8;
    
    // 检查是否点击在行号区域（折叠按钮左边）- 开始行选择
    if (x < foldBtnLeft) {
        int hitRow = HitTestRow(x, y);
        if (hitRow >= 0) {
            m_isRowSelecting = true;
            m_rowSelectStartRow = hitRow;
            m_rowSelectEndRow = hitRow;
            m_hasRowSelection = true;
            EndEditCell(true);  // 结束任何编辑
            SetFocus(m_hWnd);   // 获取键盘焦点
            SetCapture(m_hWnd);
            InvalidateRect(m_hWnd, NULL, FALSE);
            return;
        }
    } else {
        // 点击非行号区域，清除行选择
        if (m_hasRowSelection) {
            m_hasRowSelection = false;
            m_rowSelectStartRow = -1;
            m_rowSelectEndRow = -1;
            InvalidateRect(m_hWnd, NULL, FALSE);
        }
    }
    
    if (x >= foldBtnLeft && x <= foldBtnRight) {
        // 计算点击的是哪个数据类型的折叠按钮
        int currentY = -m_scrollY;
        const int separatorHeight = 20;
        
        for (size_t i = 0; i < m_dataTypes.size(); i++) {
            const auto& dt = m_dataTypes[i];
            
            // 表头行的Y范围
            int headerTop = currentY;
            int headerBottom = currentY + m_rowHeight;
            
            // 检查点击是否在这个表头行的Y范围内
            if (y >= headerTop && y < headerBottom) {
                // 点击在折叠按钮上，切换展开状态
                m_dataTypes[i].isExpanded = !m_dataTypes[i].isExpanded;
                InvalidateRect(m_hWnd, NULL, FALSE);
                return;
            }
            
            // 计算下一个数据类型的起始Y
            if (dt.isExpanded) {
                currentY += (3 + (int)dt.members.size()) * m_rowHeight;
            } else {
                currentY += 2 * m_rowHeight;
            }
            
            // 间隔行
            if (i < m_dataTypes.size() - 1) {
                currentY += separatorHeight;
            }
        }
    }
    
    // 进行 HitTest 获取完整信息
    int row, col;
    RECT cellRect;
    HitTest(x, y, row, col, cellRect);
    
    if (row >= 0 && col >= 0) {
        RowInfo info = GetRowInfo(row);
        
        // 检查是否是表头行，表头行不允许编辑
        if (info.rowType == DataTypeRowType::TypeHeader || 
            info.rowType == DataTypeRowType::MemberHeader) {
            return;
        }
        
        // 数据类型数据行：col=1是公开复选框
        if (info.rowType == DataTypeRowType::TypeData && col == 1) {
            if (info.dataTypeIndex >= 0 && info.dataTypeIndex < (int)m_dataTypes.size()) {
                m_dataTypes[info.dataTypeIndex].isPublic = !m_dataTypes[info.dataTypeIndex].isPublic;
                SetModified(true);
                CreateSnapshot(L"Toggle public");
                InvalidateRect(m_hWnd, NULL, FALSE);
            }
            return;
        }
        
        // 成员数据行：col=2是传址复选框，col=3是数组复选框
        if (info.rowType == DataTypeRowType::MemberData) {
            if (info.dataTypeIndex >= 0 && info.dataTypeIndex < (int)m_dataTypes.size() &&
                info.memberIndex >= 0 && info.memberIndex < (int)m_dataTypes[info.dataTypeIndex].members.size()) {
                
                if (col == 2) {
                    m_dataTypes[info.dataTypeIndex].members[info.memberIndex].byRef = 
                        !m_dataTypes[info.dataTypeIndex].members[info.memberIndex].byRef;
                    SetModified(true);
                    CreateSnapshot(L"Toggle byRef");
                    InvalidateRect(m_hWnd, NULL, FALSE);
                    return;
                }
                if (col == 3) {
                    m_dataTypes[info.dataTypeIndex].members[info.memberIndex].isArray = 
                        !m_dataTypes[info.dataTypeIndex].members[info.memberIndex].isArray;
                    SetModified(true);
                    CreateSnapshot(L"Toggle isArray");
                    InvalidateRect(m_hWnd, NULL, FALSE);
                    return;
                }
            }
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
        // 点击空白区域（表格外）
        EndEditCell(true);
        
        // 在左边距右边的任何位置点击都可以开始行选择
        if (x >= m_leftMarginWidth) {
            int hitRow = HitTestRow(x, y);
            // 即使 hitRow 是 -1（完全空白区域），也开始行选择模式
            // 这样拖动到有效行时可以选中
            m_isRowSelecting = true;
            m_rowSelectStartRow = hitRow;  // 可能是 -1
            m_rowSelectEndRow = hitRow;
            m_hasRowSelection = (hitRow >= 0);  // 只有命中有效行时才显示选择
            SetFocus(m_hWnd);
            SetCapture(m_hWnd);
            if (hitRow >= 0) {
                InvalidateRect(m_hWnd, NULL, FALSE);
            }
        }
    }
    
    // 保存当前单元格矩形用于补全窗口定位
    if (m_isEditing) {
        m_currentCellRect = cellRect;
    }
}

void DataTypeEditor::OnMouseMove(int x, int y, WPARAM wParam) {
    // 折叠按钮的位置
    int foldBtnSize = 12;
    int foldBtnLeft = m_leftMarginWidth - foldBtnSize - 8;
    
    // 如果鼠标在行号区域（折叠按钮左边），设置镜像箭头光标
    if (x < foldBtnLeft) {
        // 创建镜像光标
        static HCURSOR hReverseCursor = NULL;
        if (hReverseCursor == NULL) {
            HCURSOR hArrow = LoadCursor(NULL, IDC_ARROW);
            ICONINFO iconInfo;
            if (GetIconInfo(hArrow, &iconInfo)) {
                // 获取位图信息
                BITMAP bm;
                GetObject(iconInfo.hbmMask, sizeof(BITMAP), &bm);
                
                // 创建镜像位图
                HDC hdcScreen = GetDC(NULL);
                HDC hdcSrc = CreateCompatibleDC(hdcScreen);
                HDC hdcDst = CreateCompatibleDC(hdcScreen);
                
                // 镜像掩码位图
                HBITMAP hbmMaskMirror = CreateCompatibleBitmap(hdcScreen, bm.bmWidth, bm.bmHeight * (iconInfo.hbmColor ? 1 : 2));
                SelectObject(hdcSrc, iconInfo.hbmMask);
                SelectObject(hdcDst, hbmMaskMirror);
                StretchBlt(hdcDst, bm.bmWidth - 1, 0, -bm.bmWidth, bm.bmHeight * (iconInfo.hbmColor ? 1 : 2),
                          hdcSrc, 0, 0, bm.bmWidth, bm.bmHeight * (iconInfo.hbmColor ? 1 : 2), SRCCOPY);
                
                // 镜像颜色位图（如果有）
                HBITMAP hbmColorMirror = NULL;
                if (iconInfo.hbmColor) {
                    GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bm);
                    hbmColorMirror = CreateCompatibleBitmap(hdcScreen, bm.bmWidth, bm.bmHeight);
                    SelectObject(hdcSrc, iconInfo.hbmColor);
                    SelectObject(hdcDst, hbmColorMirror);
                    StretchBlt(hdcDst, bm.bmWidth - 1, 0, -bm.bmWidth, bm.bmHeight,
                              hdcSrc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
                }
                
                DeleteDC(hdcSrc);
                DeleteDC(hdcDst);
                ReleaseDC(NULL, hdcScreen);
                
                // 创建镜像光标
                ICONINFO mirrorInfo;
                mirrorInfo.fIcon = FALSE;
                mirrorInfo.xHotspot = bm.bmWidth - 1 - iconInfo.xHotspot;
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
        if (hitRow >= 0) {
            // 如果起始行是 -1（从空白区域开始），设置起始行为当前行
            if (m_rowSelectStartRow < 0) {
                m_rowSelectStartRow = hitRow;
            }
            if (hitRow != m_rowSelectEndRow) {
                m_rowSelectEndRow = hitRow;
                m_hasRowSelection = true;
                InvalidateRect(m_hWnd, NULL, FALSE);
            }
        }
        return;
    }
    
    TableEditor::OnMouseMove(x, y, wParam);
}

void DataTypeEditor::OnLButtonUp(int x, int y) {
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
    
    TableEditor::OnLButtonUp(x, y);
}

void DataTypeEditor::OnMouseWheel(int delta) {
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

void DataTypeEditor::OnKeyDown(WPARAM wParam) {
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
    
    // 回车键在成员数据行插入新成员
    if (wParam == VK_RETURN && m_isEditing) {
        RowInfo info = GetRowInfo(m_editingRow);
        if (info.rowType == DataTypeRowType::MemberData ||
            info.rowType == DataTypeRowType::MemberHeader) {
            // 结束编辑
            EndEditCell(true);
            // 插入新成员
            InsertMember(info.dataTypeIndex);
            SetModified(true);
            InvalidateRect(m_hWnd, NULL, FALSE);
            return;
        }
    }
    
    // Delete键删除行
    if (wParam == VK_DELETE && !m_isEditing) {
        int row = m_editingRow;
        if (row >= 0) {
            DeleteRow(row);
        }
        return;
    }
    
    TableEditor::OnKeyDown(wParam);
}

void DataTypeEditor::OnChar(WPARAM wParam) {
    // 空格上屏后跳过
    if (m_skipNextSpace && wParam == L' ') {
        m_skipNextSpace = false;
        return;
    }
    
    // 检查是否在编辑类型列
    if (m_isEditing) {
        RowInfo info = GetRowInfo(m_editingRow);
        
        // 成员数据行的类型列（第2列，索引1）
        if (info.rowType == DataTypeRowType::MemberData && m_editingCol == 1) {
            // 调用基类处理字符输入
            TableEditor::OnChar(wParam);
            
            // 更新补全（使用编辑缓冲区，而不是从数据结构读取）
            UpdateTypeCompletion(m_editBuffer);
            
            InvalidateRect(m_hWnd, NULL, FALSE);
            return;
        }
    }
    
    TableEditor::OnChar(wParam);
}

void DataTypeEditor::StartEditCell(int row, int col, int clickX) {
    // 初始化编辑缓冲区
    m_editBuffer = GetCellText(row, col);
    
    m_isEditing = true;
    m_editingRow = row;
    m_editingCol = col;
    
    // 根据点击位置计算光标位置
    int cursorPos = 0;
    int colPadding = 5;
    
    if (clickX > colPadding) {
        // 使用 GDI+ 测量文本宽度来计算光标位置
        HDC hdc = GetDC(m_hWnd);
        if (hdc) {
            Graphics graphics(hdc);
            FontFamily fontFamily(L"Microsoft YaHei");
            Font dataFont(&fontFamily, (REAL)m_fontSize, FontStyleRegular, UnitPixel);
            StringFormat leftFormat(StringFormat::GenericTypographic());
            leftFormat.SetFormatFlags(StringFormatFlagsMeasureTrailingSpaces);
            
            int textStartX = colPadding;
            int relativeClickX = clickX - textStartX;
            
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

// === 绘制 ===

void DataTypeEditor::DrawTable(HDC hdc, const RECT& clientRect) {
    m_clientWidth = clientRect.right;
    m_clientHeight = clientRect.bottom;
    
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
    
    if (m_dataTypes.empty()) {
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
    
    // 不绘制全局表头，每个数据类型有自己的表头
    int yPos = 0 - m_scrollY;
    const int separatorHeight = 20;
    int dataLineNumber = 1;  // 数据行号（只计算数据行，跳过表头）
    
    // 绘制每个数据类型的独立表格
    for (size_t dtIdx = 0; dtIdx < m_dataTypes.size(); dtIdx++) {
        const DataType& dt = m_dataTypes[dtIdx];
        
        // 计算这个数据类型表格的总行数
        // 数据类型表头(1) + 数据类型数据(1) + 成员表头(1) + 成员数据(N)
        int fullRowCount = 3 + (int)dt.members.size();
        // 数据行数：数据类型数据(1) + 成员数据(N)
        int dataRowsInDt = 1 + (int)dt.members.size();
        
        // 根据折叠状态决定显示的行数
        int visibleRows;
        int tableHeight;
        if (!dt.isExpanded) {
            visibleRows = 2;  // 折叠时只显示2行（数据类型表头+数据类型数据）
            tableHeight = 2 * m_rowHeight;
        } else {
            visibleRows = fullRowCount;
            tableHeight = visibleRows * m_rowHeight;
        }
        
        // 记录这个数据类型的起始数据行号
        int dtStartDataLine = dataLineNumber;
        
        // 绘制这个表格的行号和折叠符号
        for (int rowInTable = 0; rowInTable < visibleRows; rowInTable++) {
            int rowY = yPos + rowInTable * m_rowHeight;
            
            // 判断是否是数据行（subRow=1是数据类型数据，subRow>=3是成员数据）
            bool isDataRow = (rowInTable == 1 || rowInTable >= 3);
            
            // 只绘制可见区域的行号
            if (rowY + m_rowHeight >= 0 && rowY < clientRect.bottom) {
                // 只在数据行显示行号
                if (isDataRow) {
                    // 计算这个数据行的行号
                    int lineNum;
                    if (rowInTable == 1) lineNum = dtStartDataLine;      // 数据类型数据
                    else lineNum = dtStartDataLine + 1 + (rowInTable - 3);    // 成员数据
                    
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
                    if (!dt.isExpanded) {
                        graphics.DrawLine(&foldPen, foldBtnX + foldBtnSize / 2, foldBtnY + 3, 
                                        foldBtnX + foldBtnSize / 2, foldBtnY + foldBtnSize - 3);
                    }
                }
            }
        }
        
        // 绘制表格内容
        if (yPos + tableHeight >= 0 && yPos < clientRect.bottom) {
            DrawDataTypeTable(hdc, (int)dtIdx, yPos, clientRect);
        }
        
        yPos += tableHeight;
        
        // 更新数据行号
        dataLineNumber += dataRowsInDt;
        
        // 添加表格间隔（最后一个表格除外）
        if (dtIdx < m_dataTypes.size() - 1) {
            yPos += separatorHeight;
        }
    }
    
    // 绘制数据类型自动补全窗口（最后绘制以确保在最上层）
    if (m_showTypeCompletion) {
        DrawTypeCompletionWindow(hdc);
    }
}

void DataTypeEditor::DrawTypeHeader(HDC hdc, int y) {
    if (y + m_rowHeight < 0 || y > m_clientHeight) return;
    
    auto cols = GetColumnDefs();
    int x = m_leftMarginWidth - m_scrollX;
    
    // 表头背景
    HBRUSH headerBrush = CreateSolidBrush(g_CurrentTheme.tableHeaderBg);
    RECT headerRect = {m_leftMarginWidth, y, m_clientWidth, y + m_rowHeight};
    FillRect(hdc, &headerRect, headerBrush);
    DeleteObject(headerBrush);
    
    // 绘制表头文字
    SetTextColor(hdc, g_CurrentTheme.text);
    SetBkMode(hdc, TRANSPARENT);
    
    HFONT hFont = CreateFont(-14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft YaHei UI");
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
    
    const wchar_t* headers[] = {L"数据类型名", L"公开", L"", L"", L"备注"};
    
    for (int c = 0; c < (int)cols.size(); c++) {
        RECT cellRect = {x + 5, y, x + cols[c].width - 5, y + m_rowHeight};
        DrawTextW(hdc, headers[c], -1, &cellRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        x += cols[c].width;
    }
    
    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
    
    // 绘制底部边框
    HPEN pen = CreatePen(PS_SOLID, 1, g_CurrentTheme.grid);
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
    MoveToEx(hdc, m_leftMarginWidth, y + m_rowHeight - 1, NULL);
    LineTo(hdc, m_clientWidth, y + m_rowHeight - 1);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

void DataTypeEditor::DrawMemberHeader(HDC hdc, int y, int dataTypeIndex) {
    if (y + m_rowHeight < 0 || y > m_clientHeight) return;
    
    auto cols = GetColumnDefs();
    int x = m_leftMarginWidth - m_scrollX;
    
    // 表头背景（稍微不同的颜色）
    HBRUSH headerBrush = CreateSolidBrush(RGB(
        (std::min)(255, GetRValue(g_CurrentTheme.tableHeaderBg) + 10),
        (std::min)(255, GetGValue(g_CurrentTheme.tableHeaderBg) + 10),
        (std::min)(255, GetBValue(g_CurrentTheme.tableHeaderBg) + 10)
    ));
    RECT headerRect = {m_leftMarginWidth, y, m_clientWidth, y + m_rowHeight};
    FillRect(hdc, &headerRect, headerBrush);
    DeleteObject(headerBrush);
    
    // 绘制表头文字
    SetTextColor(hdc, g_CurrentTheme.text);
    SetBkMode(hdc, TRANSPARENT);
    
    HFONT hFont = CreateFont(-13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft YaHei UI");
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
    
    const wchar_t* headers[] = {L"    成员名", L"类型", L"传址", L"数组", L"备注"};
    
    for (int c = 0; c < (int)cols.size(); c++) {
        RECT cellRect = {x + 5, y, x + cols[c].width - 5, y + m_rowHeight};
        DrawTextW(hdc, headers[c], -1, &cellRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        x += cols[c].width;
    }
    
    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
    
    // 绘制底部边框
    HPEN pen = CreatePen(PS_SOLID, 1, g_CurrentTheme.grid);
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
    MoveToEx(hdc, m_leftMarginWidth, y + m_rowHeight - 1, NULL);
    LineTo(hdc, m_clientWidth, y + m_rowHeight - 1);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

void DataTypeEditor::DrawTypeDataRow(HDC hdc, int rowNum, int y, int dataTypeIndex) {
    if (y + m_rowHeight < 0 || y > m_clientHeight) return;
    if (dataTypeIndex < 0 || dataTypeIndex >= (int)m_dataTypes.size()) return;
    
    const auto& dt = m_dataTypes[dataTypeIndex];
    auto cols = GetColumnDefs();
    int x = m_leftMarginWidth - m_scrollX;
    
    // 行背景
    COLORREF bgColor = (dataTypeIndex % 2 == 0) ? 
        g_CurrentTheme.editorBg : RGB((std::max)(0, GetRValue(g_CurrentTheme.editorBg) - 10),
                                       (std::max)(0, GetGValue(g_CurrentTheme.editorBg) - 10),
                                       (std::max)(0, GetBValue(g_CurrentTheme.editorBg) - 10));
    HBRUSH rowBrush = CreateSolidBrush(bgColor);
    RECT rowRect = {m_leftMarginWidth, y, m_clientWidth, y + m_rowHeight};
    FillRect(hdc, &rowRect, rowBrush);
    DeleteObject(rowBrush);
    
    // 绘制行号
    SetTextColor(hdc, g_CurrentTheme.lineNumText);
    SetBkMode(hdc, TRANSPARENT);
    
    HFONT hFont = CreateFont(-13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft YaHei UI");
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
    
    wchar_t lineNumStr[16];
    swprintf_s(lineNumStr, L"%d", rowNum);
    RECT lineNumRect = {5, y, m_lineNumberAreaWidth - 5, y + m_rowHeight};
    DrawTextW(hdc, lineNumStr, -1, &lineNumRect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
    
    // 绘制展开/折叠图标
    SetTextColor(hdc, g_CurrentTheme.text);
    wchar_t expandIcon[4];
    wcscpy_s(expandIcon, dt.isExpanded ? L"▼" : L"▶");
    RECT iconRect = {m_lineNumberAreaWidth, y, m_leftMarginWidth, y + m_rowHeight};
    DrawTextW(hdc, expandIcon, -1, &iconRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    
    // 绘制单元格内容
    SetTextColor(hdc, g_CurrentTheme.text);
    
    // 第1列：数据类型名
    RECT cellRect = {x + 5, y, x + cols[0].width - 5, y + m_rowHeight};
    DrawTextW(hdc, dt.name.c_str(), -1, &cellRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    x += cols[0].width;
    
    // 第2列：公开（复选框）
    RECT checkRect = {x + (cols[1].width - 16) / 2, y + (m_rowHeight - 16) / 2, 
                      x + (cols[1].width + 16) / 2, y + (m_rowHeight + 16) / 2};
    DrawFrameControl(hdc, &checkRect, DFC_BUTTON, DFCS_BUTTONCHECK | (dt.isPublic ? DFCS_CHECKED : 0));
    x += cols[1].width;
    
    // 第3、4列跳过
    x += cols[2].width + cols[3].width;
    
    // 第5列：备注
    cellRect = {x + 5, y, x + cols[4].width - 5, y + m_rowHeight};
    DrawTextW(hdc, dt.comment.c_str(), -1, &cellRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    
    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
    
    // 绘制单元格边框
    HPEN pen = CreatePen(PS_SOLID, 1, g_CurrentTheme.grid);
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
    
    x = m_leftMarginWidth - m_scrollX;
    for (int c = 0; c < (int)cols.size(); c++) {
        MoveToEx(hdc, x + cols[c].width, y, NULL);
        LineTo(hdc, x + cols[c].width, y + m_rowHeight);
        x += cols[c].width;
    }
    
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

void DataTypeEditor::DrawMemberDataRow(HDC hdc, int rowNum, int y, int dataTypeIndex, int memberIndex) {
    if (y + m_rowHeight < 0 || y > m_clientHeight) return;
    if (dataTypeIndex < 0 || dataTypeIndex >= (int)m_dataTypes.size()) return;
    
    const auto& dt = m_dataTypes[dataTypeIndex];
    if (memberIndex < 0 || memberIndex >= (int)dt.members.size()) return;
    
    const auto& member = dt.members[memberIndex];
    auto cols = GetColumnDefs();
    int x = m_leftMarginWidth - m_scrollX;
    
    // 行背景
    COLORREF bgColor = (memberIndex % 2 == 0) ? 
        g_CurrentTheme.editorBg : RGB((std::max)(0, GetRValue(g_CurrentTheme.editorBg) - 10),
                                       (std::max)(0, GetGValue(g_CurrentTheme.editorBg) - 10),
                                       (std::max)(0, GetBValue(g_CurrentTheme.editorBg) - 10));
    HBRUSH rowBrush = CreateSolidBrush(bgColor);
    RECT rowRect = {m_leftMarginWidth, y, m_clientWidth, y + m_rowHeight};
    FillRect(hdc, &rowRect, rowBrush);
    DeleteObject(rowBrush);
    
    // 绘制行号
    SetTextColor(hdc, g_CurrentTheme.lineNumText);
    SetBkMode(hdc, TRANSPARENT);
    
    HFONT hFont = CreateFont(-13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft YaHei UI");
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
    
    wchar_t lineNumStr[16];
    swprintf_s(lineNumStr, L"%d", rowNum);
    RECT lineNumRect = {5, y, m_lineNumberAreaWidth - 5, y + m_rowHeight};
    DrawTextW(hdc, lineNumStr, -1, &lineNumRect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
    
    // 绘制单元格内容
    SetTextColor(hdc, g_CurrentTheme.text);
    
    // 第1列：成员名（带缩进）
    std::wstring displayName = L"    " + member.name;
    RECT cellRect = {x + 5, y, x + cols[0].width - 5, y + m_rowHeight};
    DrawTextW(hdc, displayName.c_str(), -1, &cellRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    x += cols[0].width;
    
    // 第2列：类型
    cellRect = {x + 5, y, x + cols[1].width - 5, y + m_rowHeight};
    DrawTextW(hdc, member.type.c_str(), -1, &cellRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    x += cols[1].width;
    
    // 第3列：传址（复选框）
    RECT checkRect = {x + (cols[2].width - 16) / 2, y + (m_rowHeight - 16) / 2, 
                      x + (cols[2].width + 16) / 2, y + (m_rowHeight + 16) / 2};
    DrawFrameControl(hdc, &checkRect, DFC_BUTTON, DFCS_BUTTONCHECK | (member.byRef ? DFCS_CHECKED : 0));
    x += cols[2].width;
    
    // 第4列：数组（复选框）
    checkRect = {x + (cols[3].width - 16) / 2, y + (m_rowHeight - 16) / 2, 
                 x + (cols[3].width + 16) / 2, y + (m_rowHeight + 16) / 2};
    DrawFrameControl(hdc, &checkRect, DFC_BUTTON, DFCS_BUTTONCHECK | (member.isArray ? DFCS_CHECKED : 0));
    x += cols[3].width;
    
    // 第5列：备注
    cellRect = {x + 5, y, x + cols[4].width - 5, y + m_rowHeight};
    DrawTextW(hdc, member.comment.c_str(), -1, &cellRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    
    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
    
    // 绘制单元格边框
    HPEN pen = CreatePen(PS_SOLID, 1, g_CurrentTheme.grid);
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
    
    x = m_leftMarginWidth - m_scrollX;
    for (int c = 0; c < (int)cols.size(); c++) {
        MoveToEx(hdc, x + cols[c].width, y, NULL);
        LineTo(hdc, x + cols[c].width, y + m_rowHeight);
        x += cols[c].width;
    }
    
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

void DataTypeEditor::DrawDataTypeTable(HDC hdc, int dtIndex, int& yPos, const RECT& clientRect) {
    if (dtIndex < 0 || dtIndex >= (int)m_dataTypes.size()) {
        return;
    }
    
    const DataType& dt = m_dataTypes[dtIndex];
    Graphics graphics(hdc);
    
    // 设置裁剪区域：表格内容只在行号区域之外绘制
    Region clipRegion(Rect(m_leftMarginWidth, 0, clientRect.right - m_leftMarginWidth, clientRect.bottom));
    graphics.SetClip(&clipRegion);
    
    // 表格开始Y坐标
    int tableStartY = yPos;
    
    // 计算这个数据类型表格的总行数（根据折叠状态）
    int totalRows;
    int tableHeight;
    if (!dt.isExpanded) {
        totalRows = 2;  // 折叠时只显示2行（数据类型表头+数据类型数据）
        tableHeight = 2 * m_rowHeight;
    } else {
        totalRows = 3 + (int)dt.members.size();  // 表头+数据+成员表头+成员行
        tableHeight = totalRows * m_rowHeight;
    }
    
    // 如果表格完全不可见，直接返回
    if (tableStartY > clientRect.bottom || tableStartY + tableHeight < 0) {
        return;
    }
    
    // 创建GDI+绘图对象
    Pen gridPen(ColorFromCOLORREF(g_CurrentTheme.grid), 2);
    SolidBrush textBrush(ColorFromCOLORREF(g_CurrentTheme.text));
    SolidBrush commentBrush(ColorFromCOLORREF(g_CurrentTheme.syntaxRemark));  // 备注列
    SolidBrush typeBrush(ColorFromCOLORREF(g_CurrentTheme.syntaxType));       // 类型列
    SolidBrush dtNameBrush(ColorFromCOLORREF(g_CurrentTheme.syntaxFunction)); // 数据类型名
    SolidBrush memberNameBrush(ColorFromCOLORREF(g_CurrentTheme.syntaxVariable)); // 成员名
    SolidBrush dtHeaderBrush(ColorFromCOLORREF(g_CurrentTheme.tableHeaderBg));    // 数据类型表头背景色
    SolidBrush memberHeaderBrush(ColorFromCOLORREF(g_CurrentTheme.tableSubHeaderBg)); // 成员表头背景色
    SolidBrush bgBrush(ColorFromCOLORREF(g_CurrentTheme.editorBg));            // 编辑器背景色
    SolidBrush rowSelectBrush(Color(100, GetRValue(g_CurrentTheme.editorSelection),
                                     GetGValue(g_CurrentTheme.editorSelection),
                                     GetBValue(g_CurrentTheme.editorSelection)));  // 行选择高亮背景色
    
    FontFamily fontFamily(L"Microsoft YaHei");
    Font headerFont(&fontFamily, (REAL)m_fontSize, FontStyleBold, UnitPixel);
    Font dataFont(&fontFamily, (REAL)m_fontSize, FontStyleRegular, UnitPixel);
    
    StringFormat centerFormat;
    centerFormat.SetAlignment(StringAlignmentCenter);
    centerFormat.SetLineAlignment(StringAlignmentCenter);
    
    StringFormat leftFormat(StringFormat::GenericTypographic());
    leftFormat.SetAlignment(StringAlignmentNear);
    leftFormat.SetLineAlignment(StringAlignmentCenter);
    leftFormat.SetFormatFlags(StringFormatFlagsMeasureTrailingSpaces);
    
    int colPadding = 5;
    int currentY = tableStartY;
    
    // 动态计算列宽
    RectF measureRect;
    
    // 测量数据类型名宽度
    graphics.MeasureString(dt.name.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
    int dtNameWidth = (std::max)(120, (int)measureRect.Width + colPadding * 2 + 10);
    
    // 测量成员列宽
    int maxMemberNameWidth = 120;
    int maxMemberTypeWidth = 80;
    int maxMemberCommentWidth = 120;
    
    for (const auto& member : dt.members) {
        graphics.MeasureString(member.name.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
        maxMemberNameWidth = (std::max)(maxMemberNameWidth, (int)measureRect.Width + colPadding * 2 + 10);
        
        graphics.MeasureString(member.type.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
        maxMemberTypeWidth = (std::max)(maxMemberTypeWidth, (int)measureRect.Width + colPadding * 2 + 10);
        
        graphics.MeasureString(member.comment.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
        maxMemberCommentWidth = (std::max)(maxMemberCommentWidth, (int)measureRect.Width + colPadding * 2 + 10);
    }
    
    // 测量数据类型备注宽度
    graphics.MeasureString(dt.comment.c_str(), -1, &dataFont, PointF(0, 0), &measureRect);
    int dtCommentWidth = (std::max)(120, (int)measureRect.Width + colPadding * 2 + 10);
    
    // 固定列宽
    const int byRefWidth = 40;   // 传址列固定宽度
    const int arrayWidth = 40;   // 数组列固定宽度
    const int publicWidth = 40;  // 公开列固定宽度
    
    // 成员行布局：成员名 | 类型 | 传址 | 数组 | 备注
    int memberCol1Width = (std::max)(dtNameWidth, maxMemberNameWidth);  // 成员名
    int memberCol2Width = maxMemberTypeWidth;                           // 类型
    int memberCol3Width = byRefWidth;                                   // 传址（固定）
    int memberCol4Width = arrayWidth;                                   // 数组（固定）
    int memberCol5Width = maxMemberCommentWidth;                        // 成员备注
    
    // 数据类型行布局：数据类型名 | 公开 | 备注
    // 数据类型"备注"列的左边 = 成员"传址"列的左边
    // 数据类型"备注"列的右边 = 成员"备注"列的右边
    // 所以：数据类型备注宽度 = 传址 + 数组 + 成员备注
    int dtCol1Width = memberCol1Width;   // 数据类型名（与成员名对齐）
    int dtCol2Width = memberCol2Width;   // 公开（与类型列对齐）
    int dtCol3Width = memberCol3Width + memberCol4Width + memberCol5Width;  // 备注（跨传址+数组+成员备注）
    
    // 确保数据类型备注列足够宽
    dtCol3Width = (std::max)(dtCol3Width, dtCommentWidth);
    // 如果数据类型备注更宽，需要调整成员备注宽度
    if (dtCol3Width > memberCol3Width + memberCol4Width + memberCol5Width) {
        memberCol5Width = dtCol3Width - memberCol3Width - memberCol4Width;
    }
    
    // 保存列宽到缓存（用于HitTest）
    m_cachedDtCol1Width = dtCol1Width;
    m_cachedDtCol2Width = dtCol2Width;
    m_cachedDtCol3Width = dtCol3Width;
    m_cachedMemberCol1Width = memberCol1Width;
    m_cachedMemberCol2Width = memberCol2Width;
    m_cachedMemberCol3Width = memberCol3Width;
    m_cachedMemberCol4Width = memberCol4Width;
    m_cachedMemberCol5Width = memberCol5Width;
    
    int tableWidth = dtCol1Width + dtCol2Width + dtCol3Width;
    
    // 表格左偏移
    const int tableLeftOffset = 10;
    int tableStartX = m_leftMarginWidth + tableLeftOffset - m_scrollX;
    
    // 绘制表格外边框
    graphics.DrawRectangle(&gridPen, tableStartX, tableStartY, tableWidth, tableHeight);
    
    // ========== 第1行：数据类型表头（3列：数据类型名、公开、备注） ==========
    int x = tableStartX;
    
    // 数据类型名表头
    RectF dtNameHeaderRect((REAL)x, (REAL)currentY, (REAL)dtCol1Width, (REAL)m_rowHeight);
    graphics.FillRectangle(&dtHeaderBrush, dtNameHeaderRect);
    PointF headerPos((REAL)(x + colPadding), (REAL)(currentY + m_rowHeight / 2));
    graphics.DrawString(L"数据类型名", -1, &headerFont, headerPos, &leftFormat, &textBrush);
    graphics.DrawLine(&gridPen, x + dtCol1Width, currentY, x + dtCol1Width, currentY + m_rowHeight);
    x += dtCol1Width;
    
    // 公开表头
    RectF publicHeaderRect((REAL)x, (REAL)currentY, (REAL)dtCol2Width, (REAL)m_rowHeight);
    graphics.FillRectangle(&dtHeaderBrush, publicHeaderRect);
    headerPos = PointF((REAL)(x + colPadding), (REAL)(currentY + m_rowHeight / 2));
    graphics.DrawString(L"公开", -1, &headerFont, headerPos, &leftFormat, &textBrush);
    graphics.DrawLine(&gridPen, x + dtCol2Width, currentY, x + dtCol2Width, currentY + m_rowHeight);
    x += dtCol2Width;
    
    // 备注表头
    RectF commentHeaderRect((REAL)x, (REAL)currentY, (REAL)dtCol3Width, (REAL)m_rowHeight);
    graphics.FillRectangle(&dtHeaderBrush, commentHeaderRect);
    headerPos = PointF((REAL)(x + colPadding), (REAL)(currentY + m_rowHeight / 2));
    graphics.DrawString(L"备注", -1, &headerFont, headerPos, &leftFormat, &textBrush);
    
    graphics.DrawLine(&gridPen, tableStartX, currentY + m_rowHeight, 
                     tableStartX + tableWidth, currentY + m_rowHeight);
    currentY += m_rowHeight;
    
    // 计算这个数据类型的全局起始行号
    int globalRow = 0;
    for (int i = 0; i < dtIndex; i++) {
        globalRow += 2;  // 表头 + 数据
        if (m_dataTypes[i].isExpanded) {
            globalRow += 1 + (int)m_dataTypes[i].members.size();  // 成员表头 + 成员
        }
    }
    // 当前数据类型的行：globalRow=表头, globalRow+1=数据
    int dtDataRow = globalRow + 1;
    
    // 检查数据类型数据行是否被选中
    bool isDtDataRowSelected = false;
    if (m_hasRowSelection) {
        int selStart = (std::min)(m_rowSelectStartRow, m_rowSelectEndRow);
        int selEnd = (std::max)(m_rowSelectStartRow, m_rowSelectEndRow);
        isDtDataRowSelected = (dtDataRow >= selStart && dtDataRow <= selEnd);
    }
    
    // ========== 第2行：数据类型数据（3列） ==========
    x = tableStartX;
    
    // 检查是否正在编辑数据类型数据行
    bool isEditingDtName = (m_isEditing && m_editingRow == dtDataRow && m_editingCol == 0);
    bool isEditingDtComment = (m_isEditing && m_editingRow == dtDataRow && m_editingCol == 2);
    
    // 第1列：数据类型名
    RectF cell1Rect((REAL)x, (REAL)currentY, (REAL)dtCol1Width, (REAL)m_rowHeight);
    graphics.FillRectangle(&bgBrush, cell1Rect);
    // 绘制行选择高亮
    if (isDtDataRowSelected) {
        graphics.FillRectangle(&rowSelectBrush, cell1Rect);
    }
    int textX = x + colPadding;
    int textY = currentY + m_rowHeight / 2;
    
    // 编辑时使用m_editBuffer，否则使用原文本
    const std::wstring& dtNameDisplay = isEditingDtName ? m_editBuffer : dt.name;
    
    // 如果正在编辑且有选区，先绘制选区背景
    if (isEditingDtName && m_selectionStart != m_selectionEnd) {
        int selStart = (std::min)(m_selectionStart, m_selectionEnd);
        int selEnd = (std::max)(m_selectionStart, m_selectionEnd);
        selStart = (std::min)(selStart, (int)dtNameDisplay.length());
        selEnd = (std::min)(selEnd, (int)dtNameDisplay.length());
        
        std::wstring beforeSel = dtNameDisplay.substr(0, selStart);
        std::wstring selection = dtNameDisplay.substr(selStart, selEnd - selStart);
        
        RectF beforeRect, selRect;
        graphics.MeasureString(beforeSel.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &beforeRect);
        graphics.MeasureString(selection.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &selRect);
        
        int selX = textX + (int)beforeRect.Width;
        int selWidth = (int)selRect.Width;
        
        SolidBrush selBrush(ColorFromCOLORREF(g_CurrentTheme.editorSelection));
        graphics.FillRectangle(&selBrush, selX, currentY + 2, selWidth, m_rowHeight - 4);
    }
    
    PointF textPos((REAL)textX, (REAL)textY);
    graphics.DrawString(dtNameDisplay.c_str(), -1, &dataFont, textPos, &leftFormat, &dtNameBrush);
    
    // 如果正在编辑，绘制光标
    if (isEditingDtName && m_cursorVisible) {
        std::wstring beforeCursor = dtNameDisplay.substr(0, (std::min)(m_editingCursorPos, (int)dtNameDisplay.length()));
        RectF boundingBox;
        graphics.MeasureString(beforeCursor.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &boundingBox);
        int cursorX = textX + (int)boundingBox.Width;
        Pen cursorPen(ColorFromCOLORREF(g_CurrentTheme.text), 1);
        graphics.DrawLine(&cursorPen, cursorX, currentY + 2, cursorX, currentY + m_rowHeight - 2);
    }
    
    graphics.DrawLine(&gridPen, x + dtCol1Width, currentY, x + dtCol1Width, currentY + m_rowHeight);
    x += dtCol1Width;
    
    // 第2列：公开（复选框）
    RectF cell2Rect((REAL)x, (REAL)currentY, (REAL)dtCol2Width, (REAL)m_rowHeight);
    graphics.FillRectangle(&bgBrush, cell2Rect);
    if (isDtDataRowSelected) {
        graphics.FillRectangle(&rowSelectBrush, cell2Rect);
    }
    graphics.DrawString(dt.isPublic ? L"✓" : L"", -1, &dataFont, cell2Rect, &centerFormat, &textBrush);
    graphics.DrawLine(&gridPen, x + dtCol2Width, currentY, x + dtCol2Width, currentY + m_rowHeight);
    x += dtCol2Width;
    
    // 第3列：备注
    RectF cell3Rect((REAL)x, (REAL)currentY, (REAL)dtCol3Width, (REAL)m_rowHeight);
    graphics.FillRectangle(&bgBrush, cell3Rect);
    if (isDtDataRowSelected) {
        graphics.FillRectangle(&rowSelectBrush, cell3Rect);
    }
    textX = x + colPadding;
    
    // 编辑时使用m_editBuffer，否则使用原文本
    const std::wstring& dtCommentDisplay = isEditingDtComment ? m_editBuffer : dt.comment;
    
    // 如果正在编辑且有选区，先绘制选区背景
    if (isEditingDtComment && m_selectionStart != m_selectionEnd) {
        int selStart = (std::min)(m_selectionStart, m_selectionEnd);
        int selEnd = (std::max)(m_selectionStart, m_selectionEnd);
        selStart = (std::min)(selStart, (int)dtCommentDisplay.length());
        selEnd = (std::min)(selEnd, (int)dtCommentDisplay.length());
        
        std::wstring beforeSel = dtCommentDisplay.substr(0, selStart);
        std::wstring selection = dtCommentDisplay.substr(selStart, selEnd - selStart);
        
        RectF beforeRect, selRect;
        graphics.MeasureString(beforeSel.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &beforeRect);
        graphics.MeasureString(selection.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &selRect);
        
        int selX = textX + (int)beforeRect.Width;
        int selWidth = (int)selRect.Width;
        
        SolidBrush selBrush(ColorFromCOLORREF(g_CurrentTheme.editorSelection));
        graphics.FillRectangle(&selBrush, selX, currentY + 2, selWidth, m_rowHeight - 4);
    }
    
    textPos = PointF((REAL)textX, (REAL)textY);
    graphics.DrawString(dtCommentDisplay.c_str(), -1, &dataFont, textPos, &leftFormat, &commentBrush);
    
    // 如果正在编辑，绘制光标
    if (isEditingDtComment && m_cursorVisible) {
        std::wstring beforeCursor = dtCommentDisplay.substr(0, (std::min)(m_editingCursorPos, (int)dtCommentDisplay.length()));
        RectF boundingBox;
        graphics.MeasureString(beforeCursor.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &boundingBox);
        int cursorX = textX + (int)boundingBox.Width;
        Pen cursorPen(ColorFromCOLORREF(g_CurrentTheme.text), 1);
        graphics.DrawLine(&cursorPen, cursorX, currentY + 2, cursorX, currentY + m_rowHeight - 2);
    }
    
    graphics.DrawLine(&gridPen, tableStartX, currentY + m_rowHeight, 
                     tableStartX + tableWidth, currentY + m_rowHeight);
    currentY += m_rowHeight;
    
    // 如果折叠，到此为止
    if (!dt.isExpanded) {
        return;
    }
    
    // ========== 第3行：成员表头（5列：成员名、类型、传址、数组、备注） ==========
    x = tableStartX;
    
    // 成员名表头
    RectF memberNameHeaderRect((REAL)x, (REAL)currentY, (REAL)memberCol1Width, (REAL)m_rowHeight);
    graphics.FillRectangle(&memberHeaderBrush, memberNameHeaderRect);
    headerPos = PointF((REAL)(x + colPadding), (REAL)(currentY + m_rowHeight / 2));
    graphics.DrawString(L"成员名", -1, &headerFont, headerPos, &leftFormat, &textBrush);
    graphics.DrawLine(&gridPen, x + memberCol1Width, currentY, x + memberCol1Width, currentY + m_rowHeight);
    x += memberCol1Width;
    
    // 类型表头
    RectF typeHeaderRect((REAL)x, (REAL)currentY, (REAL)memberCol2Width, (REAL)m_rowHeight);
    graphics.FillRectangle(&memberHeaderBrush, typeHeaderRect);
    headerPos = PointF((REAL)(x + colPadding), (REAL)(currentY + m_rowHeight / 2));
    graphics.DrawString(L"类型", -1, &headerFont, headerPos, &leftFormat, &textBrush);
    graphics.DrawLine(&gridPen, x + memberCol2Width, currentY, x + memberCol2Width, currentY + m_rowHeight);
    x += memberCol2Width;
    
    // 传址表头
    RectF byRefHeaderRect((REAL)x, (REAL)currentY, (REAL)memberCol3Width, (REAL)m_rowHeight);
    graphics.FillRectangle(&memberHeaderBrush, byRefHeaderRect);
    headerPos = PointF((REAL)(x + colPadding), (REAL)(currentY + m_rowHeight / 2));
    graphics.DrawString(L"传址", -1, &headerFont, headerPos, &leftFormat, &textBrush);
    graphics.DrawLine(&gridPen, x + memberCol3Width, currentY, x + memberCol3Width, currentY + m_rowHeight);
    x += memberCol3Width;
    
    // 数组表头
    RectF arrayHeaderRect((REAL)x, (REAL)currentY, (REAL)memberCol4Width, (REAL)m_rowHeight);
    graphics.FillRectangle(&memberHeaderBrush, arrayHeaderRect);
    headerPos = PointF((REAL)(x + colPadding), (REAL)(currentY + m_rowHeight / 2));
    graphics.DrawString(L"数组", -1, &headerFont, headerPos, &leftFormat, &textBrush);
    graphics.DrawLine(&gridPen, x + memberCol4Width, currentY, x + memberCol4Width, currentY + m_rowHeight);
    x += memberCol4Width;
    
    // 成员备注表头
    RectF memberCommentHeaderRect((REAL)x, (REAL)currentY, (REAL)memberCol5Width, (REAL)m_rowHeight);
    graphics.FillRectangle(&memberHeaderBrush, memberCommentHeaderRect);
    headerPos = PointF((REAL)(x + colPadding), (REAL)(currentY + m_rowHeight / 2));
    graphics.DrawString(L"备注", -1, &headerFont, headerPos, &leftFormat, &textBrush);
    
    graphics.DrawLine(&gridPen, tableStartX, currentY + m_rowHeight, 
                     tableStartX + tableWidth, currentY + m_rowHeight);
    currentY += m_rowHeight;
    
    // 成员数据行的起始全局行号 = dtDataRow + 2 (成员表头)
    int memberStartRow = dtDataRow + 2;
    
    // ========== 第4行及以后：成员数据（5列） ==========
    for (size_t memberIdx = 0; memberIdx < dt.members.size(); memberIdx++) {
        const DataTypeMember& member = dt.members[memberIdx];
        x = tableStartX;
        
        // 当前成员的全局行号
        int memberRow = memberStartRow + (int)memberIdx;
        
        // 检查成员数据行是否被选中
        bool isMemberRowSelected = false;
        if (m_hasRowSelection) {
            int selStart = (std::min)(m_rowSelectStartRow, m_rowSelectEndRow);
            int selEnd = (std::max)(m_rowSelectStartRow, m_rowSelectEndRow);
            isMemberRowSelected = (memberRow >= selStart && memberRow <= selEnd);
        }
        
        // 检查是否正在编辑这个成员行的各列
        bool isEditingMemberName = (m_isEditing && m_editingRow == memberRow && m_editingCol == 0);
        bool isEditingMemberType = (m_isEditing && m_editingRow == memberRow && m_editingCol == 1);
        bool isEditingMemberComment = (m_isEditing && m_editingRow == memberRow && m_editingCol == 4);
        
        // 第1列：成员名（不再使用缩进）
        RectF mCell1Rect((REAL)x, (REAL)currentY, (REAL)memberCol1Width, (REAL)m_rowHeight);
        graphics.FillRectangle(&bgBrush, mCell1Rect);
        if (isMemberRowSelected) {
            graphics.FillRectangle(&rowSelectBrush, mCell1Rect);
        }
        
        // 编辑时使用m_editBuffer，否则使用原文本
        std::wstring memberNameDisplay;
        if (isEditingMemberName) {
            memberNameDisplay = m_editBuffer;
        } else {
            memberNameDisplay = member.name;
        }
        
        textX = x + colPadding;
        textY = currentY + m_rowHeight / 2;
        
        // 如果正在编辑且有选区，先绘制选区背景
        if (isEditingMemberName && m_selectionStart != m_selectionEnd) {
            int selStart = (std::min)(m_selectionStart, m_selectionEnd);
            int selEnd = (std::max)(m_selectionStart, m_selectionEnd);
            selStart = (std::min)(selStart, (int)m_editBuffer.length());
            selEnd = (std::min)(selEnd, (int)m_editBuffer.length());
            
            std::wstring beforeSel = m_editBuffer.substr(0, selStart);
            std::wstring selection = m_editBuffer.substr(selStart, selEnd - selStart);
            
            RectF beforeRect, selRect;
            graphics.MeasureString(beforeSel.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &beforeRect);
            graphics.MeasureString(selection.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &selRect);
            
            int selX = textX + (int)beforeRect.Width;
            int selWidth = (int)selRect.Width;
            
            SolidBrush selBrush(ColorFromCOLORREF(g_CurrentTheme.editorSelection));
            graphics.FillRectangle(&selBrush, selX, currentY + 2, selWidth, m_rowHeight - 4);
        }
        
        PointF textPos((REAL)textX, (REAL)textY);
        graphics.DrawString(memberNameDisplay.c_str(), -1, &dataFont, textPos, &leftFormat, &memberNameBrush);
        
        // 如果正在编辑，绘制光标
        if (isEditingMemberName && m_cursorVisible) {
            std::wstring beforeCursor = m_editBuffer.substr(0, (std::min)(m_editingCursorPos, (int)m_editBuffer.length()));
            RectF boundingBox;
            graphics.MeasureString(beforeCursor.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &boundingBox);
            int cursorX = textX + (int)boundingBox.Width;
            Pen cursorPen(ColorFromCOLORREF(g_CurrentTheme.text), 1);
            graphics.DrawLine(&cursorPen, cursorX, currentY + 2, cursorX, currentY + m_rowHeight - 2);
        }
        
        graphics.DrawLine(&gridPen, x + memberCol1Width, currentY, x + memberCol1Width, currentY + m_rowHeight);
        x += memberCol1Width;
        
        // 第2列：类型
        RectF mCell2Rect((REAL)x, (REAL)currentY, (REAL)memberCol2Width, (REAL)m_rowHeight);
        graphics.FillRectangle(&bgBrush, mCell2Rect);
        if (isMemberRowSelected) {
            graphics.FillRectangle(&rowSelectBrush, mCell2Rect);
        }
        textX = x + colPadding;
        
        const std::wstring& memberTypeDisplay = isEditingMemberType ? m_editBuffer : member.type;
        
        // 如果正在编辑且有选区
        if (isEditingMemberType && m_selectionStart != m_selectionEnd) {
            int selStart = (std::min)(m_selectionStart, m_selectionEnd);
            int selEnd = (std::max)(m_selectionStart, m_selectionEnd);
            selStart = (std::min)(selStart, (int)memberTypeDisplay.length());
            selEnd = (std::min)(selEnd, (int)memberTypeDisplay.length());
            
            std::wstring beforeSel = memberTypeDisplay.substr(0, selStart);
            std::wstring selection = memberTypeDisplay.substr(selStart, selEnd - selStart);
            
            RectF beforeRect, selRect;
            graphics.MeasureString(beforeSel.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &beforeRect);
            graphics.MeasureString(selection.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &selRect);
            
            int selX = textX + (int)beforeRect.Width;
            int selWidth = (int)selRect.Width;
            
            SolidBrush selBrush(ColorFromCOLORREF(g_CurrentTheme.editorSelection));
            graphics.FillRectangle(&selBrush, selX, currentY + 2, selWidth, m_rowHeight - 4);
        }
        
        textPos = PointF((REAL)textX, (REAL)textY);
        graphics.DrawString(memberTypeDisplay.c_str(), -1, &dataFont, textPos, &leftFormat, &typeBrush);
        
        // 如果正在编辑，绘制光标
        if (isEditingMemberType && m_cursorVisible) {
            std::wstring beforeCursor = memberTypeDisplay.substr(0, (std::min)(m_editingCursorPos, (int)memberTypeDisplay.length()));
            RectF boundingBox;
            graphics.MeasureString(beforeCursor.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &boundingBox);
            int cursorX = textX + (int)boundingBox.Width;
            Pen cursorPen(ColorFromCOLORREF(g_CurrentTheme.text), 1);
            graphics.DrawLine(&cursorPen, cursorX, currentY + 2, cursorX, currentY + m_rowHeight - 2);
        }
        
        graphics.DrawLine(&gridPen, x + memberCol2Width, currentY, x + memberCol2Width, currentY + m_rowHeight);
        x += memberCol2Width;
        
        // 第3列：传址
        RectF mCell3Rect((REAL)x, (REAL)currentY, (REAL)memberCol3Width, (REAL)m_rowHeight);
        graphics.FillRectangle(&bgBrush, mCell3Rect);
        if (isMemberRowSelected) {
            graphics.FillRectangle(&rowSelectBrush, mCell3Rect);
        }
        graphics.DrawString(member.byRef ? L"✓" : L"", -1, &dataFont, mCell3Rect, &centerFormat, &textBrush);
        graphics.DrawLine(&gridPen, x + memberCol3Width, currentY, x + memberCol3Width, currentY + m_rowHeight);
        x += memberCol3Width;
        
        // 第4列：数组
        RectF mCell4Rect((REAL)x, (REAL)currentY, (REAL)memberCol4Width, (REAL)m_rowHeight);
        graphics.FillRectangle(&bgBrush, mCell4Rect);
        if (isMemberRowSelected) {
            graphics.FillRectangle(&rowSelectBrush, mCell4Rect);
        }
        graphics.DrawString(member.isArray ? L"✓" : L"", -1, &dataFont, mCell4Rect, &centerFormat, &textBrush);
        graphics.DrawLine(&gridPen, x + memberCol4Width, currentY, x + memberCol4Width, currentY + m_rowHeight);
        x += memberCol4Width;
        
        // 第5列：备注
        RectF mCell5Rect((REAL)x, (REAL)currentY, (REAL)memberCol5Width, (REAL)m_rowHeight);
        graphics.FillRectangle(&bgBrush, mCell5Rect);
        if (isMemberRowSelected) {
            graphics.FillRectangle(&rowSelectBrush, mCell5Rect);
        }
        textX = x + colPadding;
        
        const std::wstring& memberCommentDisplay = isEditingMemberComment ? m_editBuffer : member.comment;
        
        // 如果正在编辑且有选区
        if (isEditingMemberComment && m_selectionStart != m_selectionEnd) {
            int selStart = (std::min)(m_selectionStart, m_selectionEnd);
            int selEnd = (std::max)(m_selectionStart, m_selectionEnd);
            selStart = (std::min)(selStart, (int)memberCommentDisplay.length());
            selEnd = (std::min)(selEnd, (int)memberCommentDisplay.length());
            
            std::wstring beforeSel = memberCommentDisplay.substr(0, selStart);
            std::wstring selection = memberCommentDisplay.substr(selStart, selEnd - selStart);
            
            RectF beforeRect, selRect;
            graphics.MeasureString(beforeSel.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &beforeRect);
            graphics.MeasureString(selection.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &selRect);
            
            int selX = textX + (int)beforeRect.Width;
            int selWidth = (int)selRect.Width;
            
            SolidBrush selBrush(ColorFromCOLORREF(g_CurrentTheme.editorSelection));
            graphics.FillRectangle(&selBrush, selX, currentY + 2, selWidth, m_rowHeight - 4);
        }
        
        textPos = PointF((REAL)textX, (REAL)textY);
        graphics.DrawString(memberCommentDisplay.c_str(), -1, &dataFont, textPos, &leftFormat, &commentBrush);
        
        // 如果正在编辑，绘制光标
        if (isEditingMemberComment && m_cursorVisible) {
            std::wstring beforeCursor = memberCommentDisplay.substr(0, (std::min)(m_editingCursorPos, (int)memberCommentDisplay.length()));
            RectF boundingBox;
            graphics.MeasureString(beforeCursor.c_str(), -1, &dataFont, PointF(0, 0), &leftFormat, &boundingBox);
            int cursorX = textX + (int)boundingBox.Width;
            Pen cursorPen(ColorFromCOLORREF(g_CurrentTheme.text), 1);
            graphics.DrawLine(&cursorPen, cursorX, currentY + 2, cursorX, currentY + m_rowHeight - 2);
        }
        
        graphics.DrawLine(&gridPen, tableStartX, currentY + m_rowHeight, 
                         tableStartX + tableWidth, currentY + m_rowHeight);
        currentY += m_rowHeight;
    }
}

void DataTypeEditor::DrawSeparator(HDC hdc, int y) {
    const int separatorHeight = 20;
    
    if (y + separatorHeight < 0 || y > m_clientHeight) return;
    
    // 绘制间隔背景
    HBRUSH sepBrush = CreateSolidBrush(g_CurrentTheme.editorBg);
    RECT sepRect = {m_leftMarginWidth, y, m_clientWidth, y + separatorHeight};
    FillRect(hdc, &sepRect, sepBrush);
    DeleteObject(sepBrush);
}

void DataTypeEditor::DrawTypeCompletionWindow(HDC hdc) {
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
            HBRUSH selBrush = CreateSolidBrush(g_CurrentTheme.popupSelection);
            FillRect(hdc, &itemRect, selBrush);
            DeleteObject(selBrush);
            SetTextColor(hdc, g_CurrentTheme.textBright);  // 高亮文字
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
        HBRUSH trackBrush = CreateSolidBrush(g_CurrentTheme.scrollTrack);
        FillRect(hdc, &trackRect, trackBrush);
        DeleteObject(trackBrush);
        
        // 滚动条滑块
        int thumbHeight = (std::max)(20, scrollBarHeight * TYPE_COMPLETION_MAX_VISIBLE / (int)m_typeCompletionItems.size());
        int maxScroll = (int)m_typeCompletionItems.size() - TYPE_COMPLETION_MAX_VISIBLE;
        int thumbY = m_typeCompletionRect.top + 2 + 
                     (scrollBarHeight - thumbHeight) * m_typeCompletionScrollOffset / maxScroll;
        
        RECT thumbRect = {scrollBarX, thumbY, scrollBarX + scrollBarWidth, thumbY + thumbHeight};
        HBRUSH thumbBrush = CreateSolidBrush(g_CurrentTheme.scrollThumb);
        FillRect(hdc, &thumbRect, thumbBrush);
        DeleteObject(thumbBrush);
    }
}

// === 窗口过程和创建函数 ===

// DataTypeEditorData 成员函数实现
DataTypeEditorData::DataTypeEditorData(HWND hWnd) : editor(nullptr) {
    editor = new DataTypeEditor(hWnd, nullptr);
}

DataTypeEditorData::~DataTypeEditorData() {
    if (editor) {
        delete editor;
        editor = nullptr;
    }
}

bool DataTypeEditorData::LoadFile(const std::wstring& path) {
    if (editor) {
        return editor->LoadFile(path);
    }
    return false;
}

bool DataTypeEditorData::SaveFile(const std::wstring& path) {
    if (editor) {
        return editor->SaveFile(path);
    }
    return false;
}

bool DataTypeEditorData::IsModified() const {
    if (editor) {
        return editor->IsModified();
    }
    return false;
}

void DataTypeEditorData::SetModified(bool modified) {
    if (editor) {
        editor->SetModified(modified);
    }
}

// DataTypeEditor 窗口过程
LRESULT CALLBACK DataTypeEditorWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_CREATE) {
        OutputDebugStringW(L"[DataTypeEditorWndProc] WM_CREATE - 创建窗口\n");
        DataTypeEditorData* data = new DataTypeEditorData(hWnd);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));
        OutputDebugStringW(L"[DataTypeEditorWndProc] WM_CREATE - 完成\n");
        return 0;
    }
    
    DataTypeEditorData* data = reinterpret_cast<DataTypeEditorData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    
    if (message == WM_DESTROY) {
        OutputDebugStringW(L"[DataTypeEditorWndProc] WM_DESTROY\n");
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

// 注册 DataTypeEditor 窗口类
bool RegisterDataTypeEditorClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {0};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = DataTypeEditorWndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"DataTypeEditor";
    
    return RegisterClassExW(&wcex) != 0;
}

// 创建 DataTypeEditor 窗口
HWND CreateDataTypeEditorWindow(HINSTANCE hInstance, HWND hParent)
{
    return CreateWindowW(L"DataTypeEditor", L"数据类型编辑器",
        WS_CHILD,
        0, 0, 0, 0, hParent, nullptr, hInstance, nullptr);
}
