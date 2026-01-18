#pragma once
#include "TableEditor.h"
#include "json.hpp"
#include <vector>
#include <string>
#include <windows.h>

using json = nlohmann::json;

// 前向声明
class DataTypeEditor;

// 窗口相关函数
bool RegisterDataTypeEditorClass(HINSTANCE hInstance);
HWND CreateDataTypeEditorWindow(HINSTANCE hInstance, HWND hParent);
LRESULT CALLBACK DataTypeEditorWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// DataTypeEditor 窗口数据结构
struct DataTypeEditorData {
    DataTypeEditor* editor;
    
    DataTypeEditorData(HWND hWnd);
    ~DataTypeEditorData();
    
    bool LoadFile(const std::wstring& path);
    bool SaveFile(const std::wstring& path);
    bool IsModified() const;
    void SetModified(bool modified);
};

// 数据类型行类型
enum class DataTypeRowType {
    TypeHeader,         // 数据类型表头（数据类型名、公开、备注）
    TypeData,           // 数据类型数据行
    MemberHeader,       // 成员表头（成员名、类型、传址、数组、备注）
    MemberData,         // 成员数据行
    Separator           // 表格间隔行
};

// 数据类型成员
struct DataTypeMember {
    std::wstring name;      // 成员名
    std::wstring type;      // 类型
    bool byRef;             // 传址
    bool isArray;           // 数组
    std::wstring comment;   // 备注
    
    DataTypeMember() : byRef(false), isArray(false) {}
};

// 自定义数据类型
struct DataType {
    std::wstring name;      // 数据类型名
    bool isPublic;          // 是否公开
    std::wstring comment;   // 备注
    std::vector<DataTypeMember> members;
    bool isExpanded;        // 是否展开显示成员
    
    DataType() : isPublic(true), isExpanded(true) {}
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
    virtual bool IsCellTextEditable(int row, int col) const override;  // 判断单元格是否可以文本编辑
    
    virtual std::vector<std::wstring> SerializeState() const override;
    virtual void RestoreState(const std::vector<std::wstring>& state) override;
    virtual void ParseContent(const std::vector<std::wstring>& lines) override;
    virtual std::vector<std::wstring> GenerateContent() const override;
    
    virtual void InsertRow(int afterRow = -1) override;
    virtual void DeleteRow(int row) override;
    
    // 重写HitTest以正确处理DataTypeEditor的特殊布局
    void HitTest(int x, int y, int& row, int& col, RECT& cellRect) override;
    
    // 重写OnLButtonDown以正确处理复选框判断
    void OnLButtonDown(int x, int y) override;
    
    // 重写OnMouseMove和OnLButtonUp以处理行选择
    void OnMouseMove(int x, int y, WPARAM wParam) override;
    void OnLButtonUp(int x, int y) override;
    
    // 重写OnMouseWheel以支持补全窗口滚动
    void OnMouseWheel(int delta) override;
    
    // 重写OnKeyDown以处理回车键插入成员行和Delete键删除
    void OnKeyDown(WPARAM wParam) override;
    
    // 重写OnChar以处理空格上屏后跳过空格
    void OnChar(WPARAM wParam) override;
    
    // 重写StartEditCell以计算精确光标位置
    void StartEditCell(int row, int col, int clickX) override;
    
    // 获取总内容高度
    int GetTotalContentHeight() const;
    
    // 获取总内容宽度
    int GetTotalContentWidth() const override;
    
    // 重写绘制方法
    virtual void DrawTable(HDC hdc, const RECT& clientRect) override;
    
    // === 文件操作重写（支持JSON） ===
    
    virtual bool LoadFile(const std::wstring& path) override;
    virtual bool SaveFile(const std::wstring& path) override;
    
    // === 树形结构操作 ===
    
    void ToggleExpand(int row);          // 展开/折叠数据类型
    void InsertMember(int dataTypeIndex); // 插入成员
    void DeleteMember(int row);           // 删除成员
    void InsertDataType(int afterIndex);  // 插入新数据类型
    
    // 行命中测试（用于行选择）
    int HitTestRow(int x, int y);
    
protected:
    std::wstring* GetEditingCellTextPtr();
    
private:
    std::vector<DataType> m_dataTypes;
    std::wstring m_editBuffer;
    int m_leftMarginWidth;              // 左边距宽度
    int m_lineNumberAreaWidth;          // 行号区域宽度
    
    // 行选择相关
    bool m_isRowSelecting;
    int m_rowSelectStartRow;
    int m_rowSelectEndRow;
    bool m_hasRowSelection;
    
    // 客户区大小缓存
    int m_clientWidth;
    int m_clientHeight;
    
    // 数据类型自动补全相关
    bool m_showTypeCompletion;
    std::vector<std::wstring> m_typeCompletionItems;
    int m_typeCompletionSelectedIndex;
    int m_typeCompletionScrollOffset;
    RECT m_typeCompletionRect;
    RECT m_currentCellRect;
    bool m_skipNextSpace;
    static constexpr int TYPE_COMPLETION_ITEM_HEIGHT = 24;
    static constexpr int TYPE_COMPLETION_MAX_VISIBLE = 8;
    
    // 列宽缓存（用于HitTest和绘制）
    mutable int m_cachedDtCol1Width;
    mutable int m_cachedDtCol2Width;
    mutable int m_cachedDtCol3Width;
    mutable int m_cachedMemberCol1Width;
    mutable int m_cachedMemberCol2Width;
    mutable int m_cachedMemberCol3Width;
    mutable int m_cachedMemberCol4Width;
    mutable int m_cachedMemberCol5Width;
    
public:
    // 光标闪烁定时器常量
    static constexpr UINT_PTR CURSOR_TIMER_ID = 2;  // 使用不同ID避免冲突
    static constexpr UINT CURSOR_BLINK_INTERVAL = 530; // 毫秒
    bool m_cursorVisible;  // 光标可见性
    
private:
    json ToJson() const;
    void FromJson(const json& j);
    
    // UTF8 转换辅助
    std::wstring WStringToUtf8String(const std::wstring& wstr) const;
    std::wstring Utf8StringToWString(const std::string& str) const;
    
    // 行索引转换（在数据类型和成员间转换）
    struct RowInfo {
        DataTypeRowType rowType;
        int dataTypeIndex;
        int memberIndex;
    };
    RowInfo GetRowInfo(int row) const;
    int GetRowIndexFromInfo(int dataTypeIndex, int memberIndex) const;
    
    // 数据类型补全相关
    std::vector<std::wstring> GetAllDataTypes() const;
    void UpdateTypeCompletion(const std::wstring& text);
    void ApplyTypeCompletion();
    void HideTypeCompletion();
    
    // 绘制相关
    void DrawDataTypeTable(HDC hdc, int dtIndex, int& yPos, const RECT& clientRect);
    void DrawRow(HDC hdc, int row, int y, const RowInfo& info);
    void DrawTypeHeader(HDC hdc, int y);
    void DrawMemberHeader(HDC hdc, int y, int dataTypeIndex);
    void DrawTypeDataRow(HDC hdc, int row, int y, int dataTypeIndex);
    void DrawMemberDataRow(HDC hdc, int row, int y, int dataTypeIndex, int memberIndex);
    void DrawSeparator(HDC hdc, int y);
    void DrawTypeCompletionWindow(HDC hdc);
};
