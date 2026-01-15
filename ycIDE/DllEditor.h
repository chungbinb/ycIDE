#pragma once
#include "TableEditor.h"
#include <vector>
#include <string>
#include <set>

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
    bool isCollapsed;              // 是否折叠
    
    DllCommand() : isPublic(false), isCollapsed(false) {}
};

// DLL参数结构
struct DllParameter {
    std::wstring paramName;        // 参数名
    std::wstring paramType;        // 参数类型
    bool byRef = false;            // 是否传址/参考
    bool isArray = false;          // 是否数组
    bool isOptional = false;       // 是否可空
    std::wstring comment;          // 备注
};

// DLL编辑器类（继承自TableEditor）
class DllEditor : public TableEditor {
private:
    int m_version;                          // 文件版本
    std::vector<DllCommand> m_commands;     // DLL命令列表
    std::vector<DllParameter> m_allParams;  // 所有参数列表
    int m_leftMarginWidth;                  // 左边距宽度（用于行号和折叠符号）
    int m_lineNumberAreaWidth;              // 行号区域宽度（用于多选和鼠标指针变化）
    
    // 临时编辑缓冲区（用于单元格编辑）
    std::wstring m_editBuffer;
    
    // 行选择相关（多选功能）
    bool m_isRowSelecting;                  // 是否正在进行行选择
    int m_rowSelectStartRow;                // 行选择起始行
    int m_rowSelectEndRow;                  // 行选择结束行
    bool m_hasRowSelection;                 // 是否有行选择
    
    // 单元格内文本选择
    bool m_isCellTextSelecting;             // 是否正在单元格内选择文本
    RECT m_currentCellRect;                 // 当前编辑单元格的矩形
    
    // 数据类型自动补全相关
    bool m_showTypeCompletion;              // 是否显示数据类型补全窗口
    std::vector<std::wstring> m_typeCompletionItems;  // 补全列表项
    int m_typeCompletionSelectedIndex;      // 当前选中的补全项索引
    int m_typeCompletionScrollOffset;       // 补全列表滚动偏移
    RECT m_typeCompletionRect;              // 补全窗口矩形
    bool m_skipNextSpace;                   // 跳过下一个空格字符（空格上屏后使用）
    static constexpr int TYPE_COMPLETION_ITEM_HEIGHT = 24;  // 补全项高度
    static constexpr int TYPE_COMPLETION_MAX_VISIBLE = 8;   // 最大可见项数
    
public:
    // 光标闪烁定时器常量
    static constexpr UINT_PTR CURSOR_TIMER_ID = 1;
    static constexpr UINT CURSOR_BLINK_INTERVAL = 530; // 毫秒
    bool m_cursorVisible;  // 光标可见性（public以便定时器访问）
    
    // 自定义消息ID和右键菜单位置（public以便消息处理访问）
    static constexpr UINT WM_SHOW_CONTEXT_MENU = WM_USER + 100;
    int m_contextMenuX = 0;
    int m_contextMenuY = 0;
    bool m_isShowingContextMenu = false;  // 是否正在显示右键菜单
    
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
    
    // 重写鼠标移动和释放以处理行选择
    void OnMouseMove(int x, int y, WPARAM wParam) override;
    void OnLButtonUp(int x, int y) override;
    
    // 重写OnMouseWheel以支持补全窗口滚动
    void OnMouseWheel(int delta) override;
    
    // 重写OnKeyDown以处理回车键插入参数行和Delete键删除
    void OnKeyDown(WPARAM wParam) override;
    
    // 重写OnChar以处理空格上屏后跳过空格
    void OnChar(WPARAM wParam) override;
    
    // 重写OnRButtonDown以支持右键菜单
    void OnRButtonDown(int x, int y) override;
    
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
    
    // 行选择相关方法
    int HitTestRow(int x, int y);                          // 根据坐标获取行号
    void ClearRowSelection();                              // 清除行选择
    void DeleteSelectedRows();                             // 删除选中的行
    bool IsRowInSelection(int row) const;                  // 判断行是否在选中范围内
    
    // 数据类型自动补全方法
    bool IsEditingTypeColumn() const;                      // 判断是否正在编辑数据类型列
    void UpdateTypeCompletion();                           // 更新数据类型补全列表
    void ShowTypeCompletion();                             // 显示补全窗口
    void HideTypeCompletion();                             // 隐藏补全窗口
    void ApplyTypeCompletion();                            // 应用选中的补全项
    void DrawTypeCompletion(HDC hdc);                      // 绘制补全窗口
    std::vector<std::wstring> GetAllDataTypes() const;     // 获取所有可用的数据类型
    
    // 新建DLL命令
    std::wstring GenerateUniqueDllCommandName();           // 生成唯一的DLL命令名
    std::wstring GenerateUniqueDllCommandName(const std::wstring& baseName);  // 基于基础名生成唯一名
    std::wstring GenerateDllCommandClipboardText(const std::set<int>& cmdIndices);  // 生成易语言格式的剪贴板文本
    std::wstring GenerateParameterClipboardText(int cmdIndex, const std::set<int>& paramIndices);  // 生成参数剪贴板文本
    void ParseAndInsertDllCommands(const std::wstring& text);  // 解析并插入DLL命令（追加到末尾）
    void ParseAndInsertDllCommandsAt(const std::wstring& text, int insertAt);  // 解析并在指定位置插入DLL命令
    void ParseAndInsertParameters(const std::wstring& text, int cmdIndex);  // 解析并插入参数到指定命令
    void CreateNewDllCommand();                            // 创建新的DLL命令
    
public:
    // 右键菜单（public以便消息处理调用）
    void ShowContextMenu(int x, int y);                    // 显示右键菜单
    void ExecuteMenuCommand(int cmdId);                    // 执行菜单命令
};
