#include "DataTypeEditor.h"
#include "EditorContext.h"
#include "NameValidator.h"
#include <fstream>
#include <sstream>

// 构造/析构
DataTypeEditor::DataTypeEditor(HWND hWnd, EditorContext* context)
    : TableEditor(hWnd, context) {
    
    m_fileName = L"未命名.edl";
}

DataTypeEditor::~DataTypeEditor() {
}

// === 重写 TableEditor 的虚接口 ===

std::vector<ColumnDef> DataTypeEditor::GetColumnDefs() const {
    std::vector<ColumnDef> cols;
    cols.push_back(ColumnDef(L"类型/成员名", 150));
    cols.push_back(ColumnDef(L"成员类型", 120));
    cols.push_back(ColumnDef(L"数组", 60, true, true));  // 复选框列
    cols.push_back(ColumnDef(L"数组维度", 100));
    cols.push_back(ColumnDef(L"备注", 250));
    return cols;
}

int DataTypeEditor::GetRowCount() const {
    int count = 0;
    for (const auto& dt : m_dataTypes) {
        count++;  // 数据类型本身占一行
        if (dt.isExpanded) {
            count += (int)dt.members.size();  // 成员行
        }
    }
    return count;
}

std::wstring DataTypeEditor::GetCellText(int row, int col) const {
    RowInfo info = GetRowInfo(row);
    
    if (info.dataTypeIndex < 0 || info.dataTypeIndex >= (int)m_dataTypes.size()) {
        return L"";
    }
    
    const DataType& dt = m_dataTypes[info.dataTypeIndex];
    
    if (info.isDataType) {
        // 数据类型行
        switch (col) {
            case 0: return (dt.isExpanded ? L"▼ " : L"▶ ") + dt.name;
            case 4: return dt.comment;
            default: return L"";
        }
    } else {
        // 成员行
        if (info.memberIndex < 0 || info.memberIndex >= (int)dt.members.size()) {
            return L"";
        }
        
        const DataTypeMember& member = dt.members[info.memberIndex];
        
        switch (col) {
            case 0: return L"  " + member.name;  // 缩进显示
            case 1: return member.type;
            case 2: return member.isArray ? L"✓" : L"";
            case 3: return member.arrayDim;
            case 4: return member.comment;
            default: return L"";
        }
    }
}

void DataTypeEditor::SetCellValue(int row, int col, const std::wstring& value) {
    RowInfo info = GetRowInfo(row);
    
    if (info.dataTypeIndex < 0 || info.dataTypeIndex >= (int)m_dataTypes.size()) {
        return;
    }
    
    DataType& dt = m_dataTypes[info.dataTypeIndex];
    
    if (info.isDataType) {
        // 数据类型行
        switch (col) {
            case 0: dt.name = value; break;
            case 4: dt.comment = value; break;
        }
        
        // 通知数据变更
        if (m_pContext && col == 0) {
            m_pContext->NotifyDataChanged(EditorFileType::EdlDataType, value);
        }
    } else {
        // 成员行
        if (info.memberIndex < 0 || info.memberIndex >= (int)dt.members.size()) {
            return;
        }
        
        DataTypeMember& member = dt.members[info.memberIndex];
        
        switch (col) {
            case 0: member.name = value; break;
            case 1: member.type = value; break;
            case 3: member.arrayDim = value; break;
            case 4: member.comment = value; break;
        }
    }
    
    m_modified = true;
}

bool DataTypeEditor::GetCellCheckState(int row, int col) const {
    RowInfo info = GetRowInfo(row);
    
    if (info.dataTypeIndex < 0 || info.dataTypeIndex >= (int)m_dataTypes.size()) {
        return false;
    }
    
    const DataType& dt = m_dataTypes[info.dataTypeIndex];
    
    if (!info.isDataType && col == 2) {
        if (info.memberIndex >= 0 && info.memberIndex < (int)dt.members.size()) {
            return dt.members[info.memberIndex].isArray;
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
    
    if (!info.isDataType && col == 2) {
        if (info.memberIndex >= 0 && info.memberIndex < (int)dt.members.size()) {
            dt.members[info.memberIndex].isArray = checked;
            m_modified = true;
        }
    }
}

std::wstring DataTypeEditor::ValidateCell(int row, int col, const std::wstring& value) const {
    RowInfo info = GetRowInfo(row);
    
    if (info.isDataType) {
        // 验证数据类型名
        if (col == 0) {
            if (value.empty()) {
                return L"数据类型名不能为空";
            }
            
            if (m_pContext && m_pContext->GetNameValidator()) {
                return m_pContext->GetNameValidator()->ValidateName(
                    value, SymbolType::DataType, m_filePath);
            }
        }
    } else {
        // 验证成员名
        if (col == 0 && value.empty()) {
            return L"成员名不能为空";
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
        state.push_back(dt.comment);
        state.push_back(dt.isExpanded ? L"1" : L"0");
        state.push_back(std::to_wstring(dt.members.size()));
        
        for (const auto& member : dt.members) {
            state.push_back(member.name);
            state.push_back(member.type);
            state.push_back(member.isArray ? L"1" : L"0");
            state.push_back(member.arrayDim);
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
            dt.comment = state[index++];
            dt.isExpanded = (state[index++] == L"1");
            
            int memberCount = std::stoi(state[index++]);
            
            for (int j = 0; j < memberCount && index + 4 < state.size(); j++) {
                DataTypeMember member;
                member.name = state[index++];
                member.type = state[index++];
                member.isArray = (state[index++] == L"1");
                member.arrayDim = state[index++];
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
    
    for (const auto& line : lines) {
        if (line.empty() || line[0] == L'#') continue;
        
        // 判断是数据类型还是成员
        if (line.find(L".数据类型 ") == 0) {
            // 新数据类型
            DataType dt;
            
            std::wstring rest = line.substr(6);
            size_t commaPos = rest.find(L',');
            if (commaPos != std::wstring::npos) {
                dt.name = rest.substr(0, commaPos);
                dt.comment = rest.substr(commaPos + 1);
                
                // 去除空白
                size_t start = dt.name.find_first_not_of(L" \t");
                size_t end = dt.name.find_last_not_of(L" \t");
                if (start != std::wstring::npos) {
                    dt.name = dt.name.substr(start, end - start + 1);
                }
                
                start = dt.comment.find_first_not_of(L" \t");
                end = dt.comment.find_last_not_of(L" \t");
                if (start != std::wstring::npos) {
                    dt.comment = dt.comment.substr(start, end - start + 1);
                }
            } else {
                dt.name = rest;
                size_t start = dt.name.find_first_not_of(L" \t");
                size_t end = dt.name.find_last_not_of(L" \t");
                if (start != std::wstring::npos) {
                    dt.name = dt.name.substr(start, end - start + 1);
                }
            }
            
            m_dataTypes.push_back(dt);
            currentType = &m_dataTypes.back();
        } else if (line.find(L".成员 ") == 0 && currentType) {
            // 新成员
            DataTypeMember member;
            
            std::wstring rest = line.substr(4);
            
            // 按逗号分割
            std::vector<std::wstring> parts;
            std::wstring current;
            bool inQuote = false;
            
            for (wchar_t ch : rest) {
                if (ch == L'\"') {
                    inQuote = !inQuote;
                } else if (ch == L',' && !inQuote) {
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
            size_t start = current.find_first_not_of(L" \t");
            size_t end = current.find_last_not_of(L" \t");
            if (start != std::wstring::npos) {
                parts.push_back(current.substr(start, end - start + 1));
            } else if (!current.empty()) {
                parts.push_back(L"");
            }
            
            if (parts.size() >= 1) member.name = parts[0];
            if (parts.size() >= 2) member.type = parts[1];
            if (parts.size() >= 3) member.isArray = (parts[2] == L"数组");
            if (parts.size() >= 4) member.arrayDim = parts[3];
            if (parts.size() >= 5) member.comment = parts[4];
            
            currentType->members.push_back(member);
        }
    }
}

std::vector<std::wstring> DataTypeEditor::GenerateContent() const {
    std::vector<std::wstring> lines;
    
    lines.push_back(L"# 自定义数据类型定义文件");
    lines.push_back(L"");
    
    for (const auto& dt : m_dataTypes) {
        std::wstring line = L".数据类型 " + dt.name;
        if (!dt.comment.empty()) {
            line += L", " + dt.comment;
        }
        lines.push_back(line);
        
        for (const auto& member : dt.members) {
            std::wstring memberLine = L".成员 " + member.name + L", " + member.type;
            if (member.isArray) {
                memberLine += L", 数组";
                if (!member.arrayDim.empty()) {
                    memberLine += L", " + member.arrayDim;
                }
            } else {
                memberLine += L", 非数组";
            }
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
    
    if (info.isDataType || afterRow < 0) {
        // 插入新数据类型
        DataType newType;
        newType.name = L"新数据类型";
        newType.comment = L"";
        newType.isExpanded = true;
        
        if (afterRow < 0 || info.dataTypeIndex < 0) {
            m_dataTypes.insert(m_dataTypes.begin(), newType);
        } else {
            m_dataTypes.insert(m_dataTypes.begin() + info.dataTypeIndex + 1, newType);
        }
    } else {
        // 在当前数据类型中插入成员
        InsertMember(info.dataTypeIndex);
    }
    
    CreateSnapshot(L"Insert Row");
}

void DataTypeEditor::DeleteRow(int row) {
    RowInfo info = GetRowInfo(row);
    
    if (info.dataTypeIndex < 0 || info.dataTypeIndex >= (int)m_dataTypes.size()) {
        return;
    }
    
    if (info.isDataType) {
        // 删除整个数据类型
        m_dataTypes.erase(m_dataTypes.begin() + info.dataTypeIndex);
    } else {
        // 删除成员
        DataType& dt = m_dataTypes[info.dataTypeIndex];
        if (info.memberIndex >= 0 && info.memberIndex < (int)dt.members.size()) {
            dt.members.erase(dt.members.begin() + info.memberIndex);
        }
    }
    
    CreateSnapshot(L"Delete Row");
}

// === 文件操作重写（支持JSON） ===

bool DataTypeEditor::LoadFile(const std::wstring& path) {
    std::wstring ext = path.substr(path.find_last_of(L'.'));
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == L".json") {
        std::ifstream file(path);
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
            
            m_undoStack.clear();
            m_redoStack.clear();
            
            InvalidateRect(m_hWnd, NULL, FALSE);
            return true;
        } catch (...) {
            return false;
        }
    } else {
        return TableEditor::LoadFile(path);
    }
}

bool DataTypeEditor::SaveFile(const std::wstring& path) {
    std::wstring ext = path.substr(path.find_last_of(L'.'));
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == L".json") {
        std::ofstream file(path);
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
        return TableEditor::SaveFile(path);
    }
}

// === 树形结构操作 ===

void DataTypeEditor::ToggleExpand(int row) {
    RowInfo info = GetRowInfo(row);
    
    if (info.isDataType && info.dataTypeIndex >= 0 && info.dataTypeIndex < (int)m_dataTypes.size()) {
        m_dataTypes[info.dataTypeIndex].isExpanded = !m_dataTypes[info.dataTypeIndex].isExpanded;
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

void DataTypeEditor::InsertMember(int dataTypeRow) {
    if (dataTypeRow < 0 || dataTypeRow >= (int)m_dataTypes.size()) {
        return;
    }
    
    DataTypeMember newMember;
    newMember.name = L"新成员";
    newMember.type = L"整数型";
    newMember.isArray = false;
    newMember.arrayDim = L"";
    newMember.comment = L"";
    
    m_dataTypes[dataTypeRow].members.push_back(newMember);
    m_dataTypes[dataTypeRow].isExpanded = true;
    
    CreateSnapshot(L"Insert Member");
}

void DataTypeEditor::DeleteMember(int row) {
    RowInfo info = GetRowInfo(row);
    
    if (!info.isDataType && info.dataTypeIndex >= 0 && info.dataTypeIndex < (int)m_dataTypes.size()) {
        DataType& dt = m_dataTypes[info.dataTypeIndex];
        if (info.memberIndex >= 0 && info.memberIndex < (int)dt.members.size()) {
            dt.members.erase(dt.members.begin() + info.memberIndex);
            CreateSnapshot(L"Delete Member");
        }
    }
}

// === Protected 方法 ===

std::wstring* DataTypeEditor::GetEditingCellTextPtr() {
    if (!m_isEditing || m_editingRow < 0) {
        return nullptr;
    }
    
    m_editBuffer = GetCellText(m_editingRow, m_editingCol);
    
    // 移除展开/折叠符号和缩进
    if (m_editingCol == 0) {
        if (m_editBuffer.length() > 2 && (m_editBuffer[0] == L'▼' || m_editBuffer[0] == L'▶')) {
            m_editBuffer = m_editBuffer.substr(2);
        }
        if (m_editBuffer.length() > 2 && m_editBuffer.substr(0, 2) == L"  ") {
            m_editBuffer = m_editBuffer.substr(2);
        }
    }
    
    return &m_editBuffer;
}

// === Private 方法 ===

json DataTypeEditor::ToJson() const {
    json j;
    j["version"] = 1;
    j["dataTypes"] = json::array();
    
    for (const auto& dt : m_dataTypes) {
        json item;
        item["name"] = WStringToUtf8String(dt.name);
        item["comment"] = WStringToUtf8String(dt.comment);
        item["members"] = json::array();
        
        for (const auto& member : dt.members) {
            json memberItem;
            memberItem["name"] = WStringToUtf8String(member.name);
            memberItem["type"] = WStringToUtf8String(member.type);
            memberItem["array"] = member.isArray;
            memberItem["arrayDim"] = WStringToUtf8String(member.arrayDim);
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
            if (item.contains("comment")) dt.comment = Utf8StringToWString(item["comment"]);
            dt.isExpanded = true;
            
            if (item.contains("members") && item["members"].is_array()) {
                for (const auto& memberItem : item["members"]) {
                    DataTypeMember member;
                    
                    if (memberItem.contains("name")) member.name = Utf8StringToWString(memberItem["name"]);
                    if (memberItem.contains("type")) member.type = Utf8StringToWString(memberItem["type"]);
                    if (memberItem.contains("array")) member.isArray = memberItem["array"];
                    if (memberItem.contains("arrayDim")) member.arrayDim = Utf8StringToWString(memberItem["arrayDim"]);
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

// 行索引转换
DataTypeEditor::RowInfo DataTypeEditor::GetRowInfo(int row) const {
    RowInfo info;
    info.isDataType = false;
    info.dataTypeIndex = -1;
    info.memberIndex = -1;
    
    int currentRow = 0;
    
    for (int i = 0; i < (int)m_dataTypes.size(); i++) {
        if (currentRow == row) {
            info.isDataType = true;
            info.dataTypeIndex = i;
            return info;
        }
        currentRow++;
        
        if (m_dataTypes[i].isExpanded) {
            for (int j = 0; j < (int)m_dataTypes[i].members.size(); j++) {
                if (currentRow == row) {
                    info.isDataType = false;
                    info.dataTypeIndex = i;
                    info.memberIndex = j;
                    return info;
                }
                currentRow++;
            }
        }
    }
    
    return info;
}

int DataTypeEditor::GetRowIndexFromInfo(int dataTypeIndex, int memberIndex) const {
    if (dataTypeIndex < 0 || dataTypeIndex >= (int)m_dataTypes.size()) {
        return -1;
    }
    
    int row = 0;
    
    for (int i = 0; i < dataTypeIndex; i++) {
        row++;
        if (m_dataTypes[i].isExpanded) {
            row += (int)m_dataTypes[i].members.size();
        }
    }
    
    if (memberIndex < 0) {
        return row;
    }
    
    row++;  // 跳过数据类型本身
    
    if (m_dataTypes[dataTypeIndex].isExpanded && memberIndex < (int)m_dataTypes[dataTypeIndex].members.size()) {
        row += memberIndex;
        return row;
    }
    
    return -1;
}
