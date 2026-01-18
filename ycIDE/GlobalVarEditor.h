#pragma once
#include "TableEditor.h"
#include "json.hpp"
#include <vector>
#include <string>
#include <windows.h>
#include <gdiplus.h>

using json = nlohmann::json;
using namespace Gdiplus;

// 前向声明
class GlobalVarEditor;

// 窗口相关函数
bool RegisterGlobalVarEditorClass(HINSTANCE hInstance);
HWND CreateGlobalVarEditorWindow(HINSTANCE hInstance, HWND hParent);
LRESULT CALLBACK GlobalVarEditorWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// GlobalVarEditor 窗口数据结构
struct GlobalVarEditorData {
    GlobalVarEditor* editor;
    
    GlobalVarEditorData(HWND hWnd);
    ~GlobalVarEditorData();
    
    bool LoadFile(const std::wstring& path);
    bool SaveFile(const std::wstring& path);
    bool IsModified() const;
    void SetModified(bool modified);
};

// 全局变量
struct GlobalVariable {
    std::wstring name;         // 变量名
    std::wstring type;         // 类型
    std::wstring arrayDim;     // 数组维度（空=非数组，0=动态数组，5=一维5个元素，5,5=二维5x5）
    bool isPublic;             // 是否公开
    std::wstring comment;      // 备注
    
    GlobalVariable() : isPublic(true) {}
};

class GlobalVarEditor : public TableEditor
{
public:
    GlobalVarEditor(HWND hWnd, EditorContext* context = nullptr);
    virtual ~GlobalVarEditor();
    
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
    
    // 重写HitTest以考虑左边距
    void HitTest(int x, int y, int& row, int& col, RECT& cellRect) override;
    
    // 重写OnLButtonDown以正确处理复选框判断
    void OnLButtonDown(int x, int y) override;
    
    // 重写OnMouseWheel以支持补全窗口滚动
    void OnMouseWheel(int delta) override;
    
    // 重写OnKeyDown以处理回车键插入行和Delete键删除
    void OnKeyDown(WPARAM wParam) override;
    
    // 重写OnChar以处理空格上屏后跳过空格
    void OnChar(WPARAM wParam) override;
    
    // 重写StartEditCell以计算精确光标位置
    void StartEditCell(int row, int col, int clickX) override;
    
    // 光标处理
    bool OnSetCursor(int x, int y);
    
    // 重写OnMouseMove以支持拖动选择
    void OnMouseMove(int x, int y, WPARAM wParam) override;
    
    // 重写OnLButtonUp以处理拖动结束
    void OnLButtonUp(int x, int y) override;
    
    // 获取总内容高度
    int GetTotalContentHeight() const;
    
    // 获取总内容宽度
    int GetTotalContentWidth() const override;
    
    // 重写绘制方法
    virtual void DrawTable(HDC hdc, const RECT& clientRect) override;
    
    // === 文件操作重写（支持JSON） ===
    
    virtual bool LoadFile(const std::wstring& path) override;
    virtual bool SaveFile(const std::wstring& path) override;
    
protected:
    std::wstring* GetEditingCellTextPtr();
    
    // 绘制全局变量表格（表头+数据行）
    void DrawGlobalVarTable(Graphics& graphics, int yPos, const RECT& clientRect);
    
private:
    std::vector<GlobalVariable> m_variables;
    std::wstring m_editBuffer;
    
    // 行号区域相关
    int m_leftMarginWidth;              // 左边距宽度
    int m_lineNumberAreaWidth;          // 行号区域宽度
    
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
    
    // 多选相关
    std::vector<int> m_selectedRows;     // 选中的行索引
    bool m_isDraggingSelection;          // 是否正在拖动选择
    int m_dragStartRow;                  // 拖动开始的行
    bool m_isDraggingInLineNumber;      // 是否在行号区域拖动
    
public:
    // 光标闪烁定时器常量
    static constexpr UINT_PTR CURSOR_TIMER_ID = 3;  // 使用不同ID避免冲突
    static constexpr UINT CURSOR_BLINK_INTERVAL = 530; // 毫秒
    bool m_cursorVisible;  // 光标可见性
    
    // 镜像箭头光标（用于行号区域）
    HCURSOR m_hMirrorArrowCursor;
    
private:
    json ToJson() const;
    void FromJson(const json& j);
    
    // UTF8 转换辅助
    std::wstring WStringToUtf8String(const std::wstring& wstr) const;
    std::wstring Utf8StringToWString(const std::string& str) const;
    
    // 数据类型补全相关
    std::vector<std::wstring> GetAllDataTypes() const;
    void UpdateTypeCompletion(const std::wstring& text);
    void ApplyTypeCompletion();
    void HideTypeCompletion();
    
    // 多选辅助方法
    bool IsRowSelected(int row) const;
    void ClearSelection();
    void SetRowSelection(int row, bool selected);
    void SelectRowRange(int startRow, int endRow);
    int GetRowAtY(int y) const;
    
    // 绘制相关
    void DrawTypeCompletionWindow(HDC hdc);
};
