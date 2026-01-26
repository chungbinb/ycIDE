#pragma once
#include "TableEditor.h"
#include "json.hpp"
#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>

using json = nlohmann::json;
using namespace Gdiplus;

// 常量结构
struct Constant {
    std::wstring name;          // 常量名
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
    
    // 布局参数
    int m_leftMarginWidth;                  // 左边距（行号区域）宽度
    int m_lineNumberAreaWidth;              // 行号区域宽度
    
    // 动态列宽
    int m_colWidthName;                     // 常量名列宽
    int m_colWidthValue;                    // 值列宽
    int m_colWidthPublic;                   // 公开列宽（固定）
    int m_colWidthComment;                  // 备注列宽
    
    // 编辑状态
    RECT m_currentCellRect;                 // 当前编辑单元格的矩形
    bool m_cursorVisible;                   // 光标是否可见
    
    // 多选相关
    std::vector<int> m_selectedRows;        // 选中的行列表
    bool m_isDraggingSelection;             // 是否正在拖动选择
    int m_dragStartRow;                     // 拖动起始行
    bool m_isDraggingInLineNumber;          // 是否在行号区域拖动
    HCURSOR m_hMirrorArrowCursor;           // 镜像箭头光标
    
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
    bool IsCellTextEditable(int row, int col) const override;
    
    std::vector<std::wstring> SerializeState() const override;
    void RestoreState(const std::vector<std::wstring>& state) override;
    void ParseContent(const std::vector<std::wstring>& lines) override;
    std::vector<std::wstring> GenerateContent() const override;
    
    void InsertRow(int afterRow) override;
    void DeleteRow(int row) override;
    
    // === 文件操作重写（支持JSON） ===
    bool LoadFile(const std::wstring& path) override;
    bool SaveFile(const std::wstring& path) override;
    
    // === 绘制相关 ===
    void DrawTable(HDC hdc, const RECT& clientRect) override;
    int GetTotalContentHeight() const;
    int GetTotalContentWidth() const override;
    
    // === 输入处理 ===
    void OnLButtonDown(int x, int y) override;
    void OnLButtonUp(int x, int y) override;
    void OnMouseMove(int x, int y, WPARAM wParam) override;
    void OnMouseWheel(int delta) override;
    void OnKeyDown(WPARAM wParam) override;
    void OnChar(WPARAM wParam) override;
    bool OnSetCursor(int x, int y);
    
protected:
    std::wstring* GetEditingCellTextPtr() override;
    void StartEditCell(int row, int col, int clickX = -1) override;
    
private:
    // JSON序列化/反序列化
    json ToJson() const;
    void FromJson(const json& j);
    
    // 工具方法
    std::wstring WStringToUtf8String(const std::wstring& wstr) const;
    std::wstring Utf8StringToWString(const std::string& str) const;
    
    // 绘制辅助
    void DrawConstantTable(Graphics& graphics, int yPos, const RECT& clientRect);
    
    // 输入辅助
    void HitTest(int x, int y, int& row, int& col, RECT& cellRect);
    
    // 多选辅助
    bool IsRowSelected(int row) const;
    void ClearSelection();
    void SetRowSelection(int row, bool selected);
    void SelectRowRange(int startRow, int endRow);
    int GetRowAtY(int y) const;
    
    // 列宽计算
    void CalculateColumnWidths();
};

// 常量编辑器数据结构
struct ConstantEditorData {
    ConstantEditor* editor;
    
    ConstantEditorData(HWND hWnd);
    ~ConstantEditorData();
    
    bool LoadFile(const std::wstring& path);
    bool SaveFile(const std::wstring& path);
    bool IsModified() const;
    void SetModified(bool modified);
};

// 窗口过程和创建函数
LRESULT CALLBACK ConstantEditorWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool RegisterConstantEditorClass(HINSTANCE hInstance);
HWND CreateConstantEditorWindow(HINSTANCE hInstance, HWND hParent);
