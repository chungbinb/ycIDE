#include "ConstantEditor.h"
#include "EditorContext.h"
#include "NameValidator.h"
#include <fstream>
#include <sstream>
#include <codecvt>

// 构造/析构
ConstantEditor::ConstantEditor(HWND hWnd, EditorContext* context)
    : TableEditor(hWnd, context) {
    
    m_fileName = L"未命名.ecl";
}

ConstantEditor::~ConstantEditor() {
}

// === 重写 TableEditor 的虚接口 ===

std::vector<ColumnDef> ConstantEditor::GetColumnDefs() const {
    std::vector<ColumnDef> cols;
    cols.push_back(ColumnDef(L"常量名", 150));
    cols.push_back(ColumnDef(L"类型", 100));
    cols.push_back(ColumnDef(L"值", 200));
    cols.push_back(ColumnDef(L"公开", 60, true, true));  // 复选框列
    cols.push_back(ColumnDef(L"备注", 250));
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
        case 1: return constant.type;
        case 2: return constant.value;
        case 3: return constant.isPublic ? L"✓" : L"";
        case 4: return constant.comment;
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
        case 1: constant.type = value; break;
        case 2: constant.value = value; break;
        case 4: constant.comment = value; break;
    }
    
    SetModified(true);
    
    // 通知数据变更
    if (m_pContext && col == 0) {
        m_pContext->NotifyDataChanged(EditorFileType::EcsConstant, value);
    }
}

bool ConstantEditor::IsCellTextEditable(int row, int col) const {
    // col == 3 是公开复选框，不可文本编辑
    return col != 3;
}

bool ConstantEditor::GetCellCheckState(int row, int col) const {
    if (row < 0 || row >= (int)m_constants.size()) {
        return false;
    }
    
    if (col == 3) {
        return m_constants[row].isPublic;
    }
    
    return false;
}

void ConstantEditor::SetCellCheckState(int row, int col, bool checked) {
    if (row < 0 || row >= (int)m_constants.size()) {
        return;
    }
    
    if (col == 3) {
        m_constants[row].isPublic = checked;
        SetModified(true);
    }
}

std::wstring ConstantEditor::ValidateCell(int row, int col, const std::wstring& value) const {
    // 检查常量名
    if (col == 0) {
        if (value.empty()) {
            return L"常量名不能为空";
        }
        
        // 使用NameValidator检查
        if (m_pContext && m_pContext->GetNameValidator()) {
            return m_pContext->GetNameValidator()->ValidateName(
                value, SymbolType::Constant, m_filePath);
        }
    }
    
    // 检查类型
    if (col == 1 && value.empty()) {
        return L"类型不能为空";
    }
    
    return L""; // 验证通过
}

std::vector<std::wstring> ConstantEditor::SerializeState() const {
    std::vector<std::wstring> state;
    
    // 序列化常量数量
    state.push_back(std::to_wstring(m_constants.size()));
    
    // 序列化每个常量
    for (const auto& constant : m_constants) {
        state.push_back(constant.name);
        state.push_back(constant.type);
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
    
    // 恢复常量数量
    if (index < state.size()) {
        int count = std::stoi(state[index++]);
        
        // 恢复每个常量
        for (int i = 0; i < count && index < state.size(); i++) {
            Constant constant;
            constant.name = state[index++];
            constant.type = state[index++];
            constant.value = state[index++];
            constant.isPublic = (state[index++] == L"1");
            constant.comment = state[index++];
            
            m_constants.push_back(constant);
        }
    }
}

void ConstantEditor::ParseContent(const std::vector<std::wstring>& lines) {
    m_constants.clear();
    
    // 简单的文本格式解析（每行一个常量）
    // 格式: 常量名, 类型, 值, 公开/私有, 备注
    for (const auto& line : lines) {
        if (line.empty() || line[0] == L'#') continue; // 跳过空行和注释
        
        Constant constant;
        
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
        if (parts.size() >= 1) constant.name = parts[0];
        if (parts.size() >= 2) constant.type = parts[1];
        if (parts.size() >= 3) {
            constant.value = parts[2];
            // 去除引号
            if (constant.value.length() >= 2 && constant.value.front() == L'\"' && constant.value.back() == L'\"') {
                constant.value = constant.value.substr(1, constant.value.length() - 2);
            }
        }
        if (parts.size() >= 4) constant.isPublic = (parts[3] == L"公开");
        if (parts.size() >= 5) constant.comment = parts[4];
        
        if (!constant.name.empty()) {
            m_constants.push_back(constant);
        }
    }
}

std::vector<std::wstring> ConstantEditor::GenerateContent() const {
    std::vector<std::wstring> lines;
    
    // 添加注释头
    lines.push_back(L"# 常量定义文件");
    lines.push_back(L"# 格式: 常量名, 类型, 值, 公开/私有, 备注");
    lines.push_back(L"");
    
    // 生成每个常量
    for (const auto& constant : m_constants) {
        std::wstring line = constant.name + L", ";
        line += constant.type + L", ";
        line += L"\"" + constant.value + L"\", ";
        line += (constant.isPublic ? L"公开" : L"私有");
        if (!constant.comment.empty()) {
            line += L", " + constant.comment;
        }
        lines.push_back(line);
    }
    
    return lines;
}

void ConstantEditor::InsertRow(int afterRow) {
    Constant newConstant;
    newConstant.name = L"新常量";
    newConstant.type = L"整数型";
    newConstant.value = L"0";
    newConstant.isPublic = true;
    newConstant.comment = L"";
    
    if (afterRow < 0) {
        m_constants.insert(m_constants.begin(), newConstant);
    } else if (afterRow >= (int)m_constants.size()) {
        m_constants.push_back(newConstant);
    } else {
        m_constants.insert(m_constants.begin() + afterRow + 1, newConstant);
    }
    
    CreateSnapshot(L"Insert Constant");
}

void ConstantEditor::DeleteRow(int row) {
    if (row >= 0 && row < (int)m_constants.size()) {
        m_constants.erase(m_constants.begin() + row);
        CreateSnapshot(L"Delete Constant");
    }
}

// === 文件操作重写（支持JSON） ===

bool ConstantEditor::LoadFile(const std::wstring& path) {
    // 检查文件扩展名
    std::wstring ext = path.substr(path.find_last_of(L'.'));
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == L".json") {
        // 加载JSON格式
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
        // 使用基类的文本格式加载
        return TableEditor::LoadFile(path);
    }
}

bool ConstantEditor::SaveFile(const std::wstring& path) {
    // 检查文件扩展名
    std::wstring ext = path.substr(path.find_last_of(L'.'));
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == L".json") {
        // 保存为JSON格式
        std::ofstream file(path);
        if (!file.is_open()) {
            return false;
        }
        
        try {
            json j = ToJson();
            file << j.dump(2); // 2空格缩进
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
        // 使用基类的文本格式保存
        return TableEditor::SaveFile(path);
    }
}

// === Protected 方法 ===

std::wstring* ConstantEditor::GetEditingCellTextPtr() {
    if (!m_isEditing || m_editingRow < 0 || m_editingRow >= (int)m_constants.size()) {
        return nullptr;
    }
    
    m_editBuffer = GetCellText(m_editingRow, m_editingCol);
    return &m_editBuffer;
}

// === Private 方法 ===

json ConstantEditor::ToJson() const {
    json j;
    j["version"] = 1;
    j["constants"] = json::array();
    
    for (const auto& constant : m_constants) {
        json item;
        item["name"] = WStringToUtf8String(constant.name);
        item["type"] = WStringToUtf8String(constant.type);
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
            if (item.contains("type")) constant.type = Utf8StringToWString(item["type"]);
            if (item.contains("value")) constant.value = Utf8StringToWString(item["value"]);
            if (item.contains("public")) constant.isPublic = item["public"];
            if (item.contains("comment")) constant.comment = Utf8StringToWString(item["comment"]);
            
            m_constants.push_back(constant);
        }
    }
}

std::wstring ConstantEditor::WStringToUtf8String(const std::wstring& wstr) const {
    if (wstr.empty()) return L"";
    
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (len <= 0) return L"";
    
    std::string utf8Str(len - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8Str[0], len, NULL, NULL);
    
    // 转换回wstring（JSON库会处理UTF-8）
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
