#pragma once
#include "TableEditor.h"
#include "json.hpp"
#include <vector>
#include <string>

using json = nlohmann::json;

// 数据类型成员
struct DataTypeMember {
    std::wstring name;
    std::wstring type;
    bool isArray;
    std::wstring arrayDim;
    std::wstring comment;
    
    DataTypeMember() : isArray(false) {}
};

// 自定义数据类型
struct DataType {
    std::wstring name;
    std::wstring comment;
    std::vector<DataTypeMember> members;
    bool isExpanded;  // 是否展开显示成员
    
    DataType() : isExpanded(true) {}
};

class DataTypeEditor : public TableEditor
{
public:
    DataTypeEditor(HWND hWnd, EditorContext* context = nullptr);
    virtual ~DataTypeEditor();
    
    // === 重写 TableEditor 的虚接口 ===
    
    virtual std::vector<ColumnDef> GetColumnDefs() const override;
    virtual int GetRowCount() const override;
    virtual std::wstring GetCellText(int row, int col) const override;
    virtual void SetCellValue(int row, int col, const std::wstring& value) override;
    virtual bool GetCellCheckState(int row, int col) const override;
    virtual void SetCellCheckState(int row, int col, bool checked) override;
    virtual std::wstring ValidateCell(int row, int col, const std::wstring& value) const override;
    
    virtual std::vector<std::wstring> SerializeState() const override;
    virtual void RestoreState(const std::vector<std::wstring>& state) override;
    virtual void ParseContent(const std::vector<std::wstring>& lines) override;
    virtual std::vector<std::wstring> GenerateContent() const override;
    
    virtual void InsertRow(int afterRow = -1) override;
    virtual void DeleteRow(int row) override;
    
    // === 文件操作重写（支持JSON） ===
    
    virtual bool LoadFile(const std::wstring& path) override;
    virtual bool SaveFile(const std::wstring& path) override;
    
    // === 树形结构操作 ===
    
    void ToggleExpand(int row);  // 展开/折叠数据类型
    void InsertMember(int dataTypeRow);  // 插入成员
    void DeleteMember(int row);  // 删除成员
    
protected:
    std::wstring* GetEditingCellTextPtr();
    
private:
    std::vector<DataType> m_dataTypes;
    std::wstring m_editBuffer;
    
    // JSON 序列化
    json ToJson() const;
    void FromJson(const json& j);
    
    // UTF8 转换辅助
    std::wstring WStringToUtf8String(const std::wstring& wstr) const;
    std::wstring Utf8StringToWString(const std::string& str) const;
    
    // 行索引转换（在数据类型和成员间转换）
    struct RowInfo {
        bool isDataType;
        int dataTypeIndex;
        int memberIndex;
    };
    RowInfo GetRowInfo(int row) const;
    int GetRowIndexFromInfo(int dataTypeIndex, int memberIndex) const;
};
