#include "GlobalVarEditor.h"
#include "EditorContext.h"
#include "NameValidator.h"
#include <fstream>
#include <sstream>

// 构造/析构
GlobalVarEditor::GlobalVarEditor(HWND hWnd, EditorContext* context)
    : TableEditor(hWnd, context) {
    
    m_fileName = L"未命名.eal";
}

GlobalVarEditor::~GlobalVarEditor() {
}

// === 重写 TableEditor 的虚接口 ===

std::vector<ColumnDef> GlobalVarEditor::GetColumnDefs() const {
    std::vector<ColumnDef> cols;
    cols.push_back(ColumnDef(L"变量名", 150));
    cols.push_back(ColumnDef(L"类型", 120));
    cols.push_back(ColumnDef(L"静态", 60, true, true));  // 复选框列
    cols.push_back(ColumnDef(L"数组", 60, true, true));  // 复选框列
    cols.push_back(ColumnDef(L"数组维度", 100));
    cols.push_back(ColumnDef(L"备注", 250));
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
        case 2: return var.isStatic ? L"✓" : L"";
        case 3: return var.isArray ? L"✓" : L"";
        case 4: return var.arrayDim;
        case 5: return var.comment;
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
        case 4: var.arrayDim = value; break;
        case 5: var.comment = value; break;
    }
    
    m_modified = true;
    
    // 通知数据变更
    if (m_pContext && col == 0) {
        m_pContext->NotifyDataChanged(EditorFileType::EalGlobalVar, value);
    }
}

bool GlobalVarEditor::GetCellCheckState(int row, int col) const {
    if (row < 0 || row >= (int)m_variables.size()) {
        return false;
    }
    
    const GlobalVariable& var = m_variables[row];
    
    switch (col) {
        case 2: return var.isStatic;
        case 3: return var.isArray;
        default: return false;
    }
}

void GlobalVarEditor::SetCellCheckState(int row, int col, bool checked) {
    if (row < 0 || row >= (int)m_variables.size()) {
        return;
    }
    
    GlobalVariable& var = m_variables[row];
    
    switch (col) {
        case 2: var.isStatic = checked; break;
        case 3: var.isArray = checked; break;
    }
    
    m_modified = true;
}

std::wstring GlobalVarEditor::ValidateCell(int row, int col, const std::wstring& value) const {
    // 检查变量名
    if (col == 0) {
        if (value.empty()) {
            return L"变量名不能为空";
        }
        
        // 使用NameValidator检查
        if (m_pContext && m_pContext->GetNameValidator()) {
            return m_pContext->GetNameValidator()->ValidateName(
                value, SymbolType::GlobalVariable, m_filePath);
        }
    }
    
    // 检查类型
    if (col == 1 && value.empty()) {
        return L"类型不能为空";
    }
    
    return L""; // 验证通过
}

std::vector<std::wstring> GlobalVarEditor::SerializeState() const {
    std::vector<std::wstring> state;
    
    // 序列化变量数量
    state.push_back(std::to_wstring(m_variables.size()));
    
    // 序列化每个变量
    for (const auto& var : m_variables) {
        state.push_back(var.name);
        state.push_back(var.type);
        state.push_back(var.isStatic ? L"1" : L"0");
        state.push_back(var.isArray ? L"1" : L"0");
        state.push_back(var.arrayDim);
        state.push_back(var.comment);
    }
    
    return state;
}

void GlobalVarEditor::RestoreState(const std::vector<std::wstring>& state) {
    if (state.empty()) return;
    
    size_t index = 0;
    m_variables.clear();
    
    // 恢复变量数量
    if (index < state.size()) {
        int count = std::stoi(state[index++]);
        
        // 恢复每个变量
        for (int i = 0; i < count && index + 5 < state.size(); i++) {
            GlobalVariable var;
            var.name = state[index++];
            var.type = state[index++];
            var.isStatic = (state[index++] == L"1");
            var.isArray = (state[index++] == L"1");
            var.arrayDim = state[index++];
            var.comment = state[index++];
            
            m_variables.push_back(var);
        }
    }
}

void GlobalVarEditor::ParseContent(const std::vector<std::wstring>& lines) {
    m_variables.clear();
    
    // 简单的文本格式解析
    // 格式: 变量名, 类型, 静态/非静态, 数组/非数组, 数组维度, 备注
    for (const auto& line : lines) {
        if (line.empty() || line[0] == L'#') continue;
        
        GlobalVariable var;
        
        // 按逗号分割
        std::vector<std::wstring> parts;
        std::wstring current;
        bool inQuote = false;
        
        for (wchar_t ch : line) {
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
        // 添加最后一个部分
        size_t start = current.find_first_not_of(L" \t");
        size_t end = current.find_last_not_of(L" \t");
        if (start != std::wstring::npos) {
            parts.push_back(current.substr(start, end - start + 1));
        } else if (!current.empty()) {
            parts.push_back(L"");
        }
        
        // 解析字段
        if (parts.size() >= 1) var.name = parts[0];
        if (parts.size() >= 2) var.type = parts[1];
        if (parts.size() >= 3) var.isStatic = (parts[2] == L"静态");
        if (parts.size() >= 4) var.isArray = (parts[3] == L"数组");
        if (parts.size() >= 5) var.arrayDim = parts[4];
        if (parts.size() >= 6) var.comment = parts[5];
        
        if (!var.name.empty()) {
            m_variables.push_back(var);
        }
    }
}

std::vector<std::wstring> GlobalVarEditor::GenerateContent() const {
    std::vector<std::wstring> lines;
    
    // 添加注释头
    lines.push_back(L"# 全局变量定义文件");
    lines.push_back(L"# 格式: 变量名, 类型, 静态/非静态, 数组/非数组, 数组维度, 备注");
    lines.push_back(L"");
    
    // 生成每个变量
    for (const auto& var : m_variables) {
        std::wstring line = var.name + L", ";
        line += var.type + L", ";
        line += (var.isStatic ? std::wstring(L"静态") : std::wstring(L"非静态")) + L", ";
        line += (var.isArray ? std::wstring(L"数组") : std::wstring(L"非数组"));
        if (var.isArray && !var.arrayDim.empty()) {
            line += L", " + var.arrayDim;
        } else {
            line += L", ";
        }
        if (!var.comment.empty()) {
            line += L", " + var.comment;
        }
        lines.push_back(line);
    }
    
    return lines;
}

void GlobalVarEditor::InsertRow(int afterRow) {
    GlobalVariable newVar;
    newVar.name = L"新变量";
    newVar.type = L"整数型";
    newVar.isStatic = false;
    newVar.isArray = false;
    newVar.arrayDim = L"";
    newVar.comment = L"";
    
    if (afterRow < 0) {
        m_variables.insert(m_variables.begin(), newVar);
    } else if (afterRow >= (int)m_variables.size()) {
        m_variables.push_back(newVar);
    } else {
        m_variables.insert(m_variables.begin() + afterRow + 1, newVar);
    }
    
    CreateSnapshot(L"Insert Variable");
}

void GlobalVarEditor::DeleteRow(int row) {
    if (row >= 0 && row < (int)m_variables.size()) {
        m_variables.erase(m_variables.begin() + row);
        CreateSnapshot(L"Delete Variable");
    }
}

// === 文件操作重写（支持JSON） ===

bool GlobalVarEditor::LoadFile(const std::wstring& path) {
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

bool GlobalVarEditor::SaveFile(const std::wstring& path) {
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

// === Protected 方法 ===

std::wstring* GlobalVarEditor::GetEditingCellTextPtr() {
    if (!m_isEditing || m_editingRow < 0 || m_editingRow >= (int)m_variables.size()) {
        return nullptr;
    }
    
    m_editBuffer = GetCellText(m_editingRow, m_editingCol);
    return &m_editBuffer;
}

// === Private 方法 ===

json GlobalVarEditor::ToJson() const {
    json j;
    j["version"] = 1;
    j["variables"] = json::array();
    
    for (const auto& var : m_variables) {
        json item;
        item["name"] = WStringToUtf8String(var.name);
        item["type"] = WStringToUtf8String(var.type);
        item["static"] = var.isStatic;
        item["array"] = var.isArray;
        item["arrayDim"] = WStringToUtf8String(var.arrayDim);
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
            if (item.contains("static")) var.isStatic = item["static"];
            if (item.contains("array")) var.isArray = item["array"];
            if (item.contains("arrayDim")) var.arrayDim = Utf8StringToWString(item["arrayDim"]);
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
