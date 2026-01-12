#pragma once
#include "TableEditor.h"
#include <vector>
#include <string>

// GDI+ 前置声明
namespace Gdiplus {
    class Graphics;
    class Font;
    class SolidBrush;
}

// DLL表格行类型
enum class DllRowType {
    CommandHeader,      // DLL命令名、返回值类型、公开、备注（表头）
    CommandData,        // 命令数据行
    LibFileHeader,      // 库文件名（表头）
    LibFileData,        // 库文件名数据
    DllNameHeader,      // 在库中对应的命令名（表头）
    DllNameData,        // DLL命令名数据
    ParamHeader,        // 参数名、类型、传址、数组、备注（表头）
    ParamData,          // 参数数据行
    Separator           // 表格间隔行
};

// DLL命令结构
struct DllCommand {
    std::wstring commandName;      // 命令名
    std::wstring returnType;       // 返回值类型
    std::wstring dllFileName;      // DLL文件名
    std::wstring dllCommandName;   // 在DLL中对应命令名
    bool isPublic;                 // 是否公开
    std::wstring comment;          // 备注
    std::vector<int> paramLines;   // 参数行索引列表
};

// DLL参数结构
struct DllParameter {
    std::wstring paramName;        // 参数名
    std::wstring paramType;        // 参数类型
    bool byRef;                    // 是否传址/参考
    bool isArray;                  // 是否数组
    bool isOptional;               // 是否可空
    std::wstring comment;          // 备注
};

// DLL编辑器类（继承自TableEditor）
class DllEditor : public TableEditor {
private:
    int m_version;                          // 文件版本
    std::vector<DllCommand> m_commands;     // DLL命令列表
    std::vector<DllParameter> m_allParams;  // 所有参数列表
    int m_leftMarginWidth;                  // 左边距宽度（用于行号和折叠符号）
    
    // 临时编辑缓冲区（用于单元格编辑）
    std::wstring m_editBuffer;
    
public:
    // 光标闪烁定时器常量
    static constexpr UINT_PTR CURSOR_TIMER_ID = 1;
    static constexpr UINT CURSOR_BLINK_INTERVAL = 530; // 毫秒
    bool m_cursorVisible;  // 光标可见性（public以便定时器访问）
    
public:
    DllEditor(HWND hWnd, EditorContext* context);
    virtual ~DllEditor();
    
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
    void CopyRow(int row) override;
    void PasteRow(int afterRow) override;
    void OnTextModified() override;
    
    // 重写以正确计算包含间隔的总高度
    int GetTotalContentHeight() const;
    
    // 重写以正确计算表格的总宽度
    int GetTotalContentWidth() const override;
    
    // 重写HitTest以正确处理DllEditor的特殊布局
    void HitTest(int x, int y, int& row, int& col, RECT& cellRect) override;
    
    // 重写OnLButtonDown以正确处理复选框判断
    void OnLButtonDown(int x, int y) override;
    
    // 重写OnKeyDown以处理回车键插入参数行
    void OnKeyDown(WPARAM wParam) override;
    
    // 重写以提供精确的光标屏幕位置（用于输入法定位）
    bool GetEditingCursorScreenPos(int& x, int& y) override;
    
protected:
    // 获取编辑单元格文本指针
    std::wstring* GetEditingCellTextPtr() override;
    
    // 重写开始编辑方法，根据点击位置计算光标位置
    void StartEditCell(int row, int col, int clickX) override;
    
    // 重写绘制方法以支持独立表格
    void DrawTable(HDC hdc, const RECT& clientRect) override;
    void DrawCommandTable(HDC hdc, int cmdIndex, int& yPos, const RECT& clientRect);
    void DrawCellWithCursor(Gdiplus::Graphics& graphics, const std::wstring& text, int x, int y, Gdiplus::Font& font, Gdiplus::SolidBrush& textBrush);
    
private:
    // 内部辅助方法
    int GetCommandIndexFromRow(int row, int& subRow) const;
    DllRowType GetRowType(int row, int& cmdIndex, int& paramIndex) const;
    int GetGlobalRowFromCmdAndSubRow(int cmdIndex, int subRowInCmd) const;  // 根据命令索引和子行号计算全局行号
    bool IsCommandRow(int row) const;
    bool IsParamRow(int row, int& cmdIndex, int& paramIndex) const;
    bool IsCellCheckbox(int row, int col) const;  // 判断某个单元格是否是复选框
    
    // 插入/删除命令和参数
    void InsertCommand(int afterCmdIndex);
    void DeleteCommand(int cmdIndex);
    void InsertParameter(int cmdIndex, int afterParamIndex);
    void DeleteParameter(int cmdIndex, int paramIndex);
};
