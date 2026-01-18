#pragma once
#include "TableEditor.h"
#include "json.hpp"
#include <vector>
#include <string>

using json = nlohmann::json;

// 常量结构
struct Constant {
    std::wstring name;          // 常量名
    std::wstring type;          // 数据类型
    std::wstring value;         // 常量值
    bool isPublic;              // 是否公开
    std::wstring comment;       // 备注
    
    Constant() : isPublic(true) {}
};

// 常量编辑器类（继承自TableEditor）
class ConstantEditor : public TableEditor {
private:
    std::vector<Constant> m_constants;      // 常量列表
    std::wstring m_editBuffer;              // 临时编辑缓冲区
    
public:
    ConstantEditor(HWND hWnd, EditorContext* context);
    virtual ~ConstantEditor();
    
    // === 重写 TableEditor 的虚接口 ===
    
    std::vector<ColumnDef> GetColumnDefs() const override;
    int GetRowCount() const override;
    std::wstring GetCellText(int row, int col) const override;
    void SetCellValue(int row, int col, const std::wstring& value) override;
    bool GetCellCheckState(int row, int col) const override;
    void SetCellCheckState(int row, int col, bool checked) override;
    std::wstring ValidateCell(int row, int col, const std::wstring& value) const override;
    bool IsCellTextEditable(int row, int col) const override;  // 判断单元格是否可以文本编辑
    
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
