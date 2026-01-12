#pragma once
#include "TableEditor.h"
#include "json.hpp"
#include <vector>
#include <string>

using json = nlohmann::json;

// 全局变量结构
struct GlobalVariable {
    std::wstring name;          // 变量名
    std::wstring type;          // 数据类型
    bool isStatic;              // 是否静态
    bool isArray;               // 是否数组
    std::wstring arrayDim;      // 数组维度（如果是数组）
    std::wstring comment;       // 备注
    
    GlobalVariable() : isStatic(false), isArray(false) {}
};

// 全局变量编辑器类（继承自TableEditor）
class GlobalVarEditor : public TableEditor {
private:
    std::vector<GlobalVariable> m_variables;    // 变量列表
    std::wstring m_editBuffer;                  // 临时编辑缓冲区
    
public:
    GlobalVarEditor(HWND hWnd, EditorContext* context);
    virtual ~GlobalVarEditor();
    
    // === 重写 TableEditor 的虚接口 ===
    
    std::vector<ColumnDef> GetColumnDefs() const override;
    int GetRowCount() const override;
    std::wstring GetCellText(int row, int col) const override;
    void SetCellValue(int row, int col, const std::wstring& value) override;
    bool GetCellCheckState(int row, int col) const override;
    void SetCellCheckState(int row, int col, bool checked) override;
    std::wstring ValidateCell(int row, int col, const std::wstring& value) const override;
    
    std::vector<std::wstring> SerializeState() const override;
    void RestoreState(const std::vector<std::wstring>& state) override;
    void ParseContent(const std::vector<std::wstring>& lines) override;
    std::vector<std::wstring> GenerateContent() const override;
    
    void InsertRow(int afterRow) override;
    void DeleteRow(int row) override;
    
    // === 文件操作重写（支持JSON） ===
    bool LoadFile(const std::wstring& path) override;
    bool SaveFile(const std::wstring& path) override;
    
protected:
    std::wstring* GetEditingCellTextPtr();
    
private:
    // JSON序列化/反序列化
    json ToJson() const;
    void FromJson(const json& j);
    
    // 工具方法
    std::wstring WStringToUtf8String(const std::wstring& wstr) const;
    std::wstring Utf8StringToWString(const std::string& str) const;
};
