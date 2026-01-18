#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <memory>

// 前向声明
class EditorContext;

// 列定义结构
struct ColumnDef {
    std::wstring name;          // 列名
    int width;                  // 列宽（像素）
    bool editable;              // 是否可编辑
    bool isCheckbox;            // 是否为复选框列
    bool resizable;             // 是否可调整宽度
    
    ColumnDef(const std::wstring& n, int w, bool edit = true, bool checkbox = false)
        : name(n), width(w), editable(edit), isCheckbox(checkbox), resizable(true) {}
};

// 撤销快照结构
struct TableSnapshot {
    std::wstring id;                  // 唯一ID
    std::wstring description;         // 操作描述
    long long timestamp;              // 时间戳
    std::vector<std::wstring> data;   // 快照数据（由子类序列化）
    
    TableSnapshot() : timestamp(0) {}
};

// 表格编辑器基类
class TableEditor {
protected:
    HWND m_hWnd;
    EditorContext* m_pContext;        // 共享上下文
    
    // 文件信息
    std::wstring m_filePath;
    std::wstring m_fileName;
    bool m_modified;
    HANDLE m_hFileLock;
    
    // 撤销/重做系统
    std::vector<TableSnapshot> m_undoStack;
    std::vector<TableSnapshot> m_redoStack;
    static const int MAX_UNDO_STACK_SIZE = 100;
    
    // UI 状态
    int m_fontSize;
    int m_rowHeight;
    int m_headerHeight;
    int m_scrollY;
    int m_scrollX;
    
    // 滚动条
    int m_scrollbarWidth;
    RECT m_vScrollbarRect;
    RECT m_vScrollThumbRect;
    RECT m_hScrollbarRect;
    RECT m_hScrollThumbRect;
    bool m_vScrollHover;
    bool m_hScrollHover;
    bool m_isDraggingVScroll;
    bool m_isDraggingHScroll;
    int m_dragStartY;
    int m_dragStartX;
    int m_dragStartScrollY;
    int m_dragStartScrollX;
    
    // 编辑状态（单元格内直接编辑）
    bool m_isEditing;
    int m_editingRow;
    int m_editingCol;
    int m_editingCursorPos;
    bool m_isSelecting;
    int m_selectionStart;
    int m_selectionEnd;
    
    // 欢迎页
    bool m_showWelcomePage;
    
public:
    TableEditor(HWND hWnd, EditorContext* context);
    virtual ~TableEditor();
    
    // === 窗口消息处理 ===
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    // 直接传递 editor 指针的版本，避免 GWLP_USERDATA 临时修改问题
    static LRESULT WndProcWithEditor(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, TableEditor* pEditor);
    
    // === 文件操作 ===
    virtual bool LoadFile(const std::wstring& path);
    virtual bool SaveFile(const std::wstring& path);
    virtual bool SaveFile();  // 保存到当前路径
    
    // === 撤销/重做系统 ===
    void CreateSnapshot(const std::wstring& description);
    void Undo();
    void Redo();
    bool CanUndo() const { return !m_undoStack.empty(); }
    bool CanRedo() const { return !m_redoStack.empty(); }
    
    // === 子类必须实现的虚接口 ===
    
    // 获取列定义
    virtual std::vector<ColumnDef> GetColumnDefs() const = 0;
    
    // 获取行数
    virtual int GetRowCount() const = 0;
    
    // 获取单元格文本
    virtual std::wstring GetCellText(int row, int col) const = 0;
    
    // 设置单元格值
    virtual void SetCellValue(int row, int col, const std::wstring& value) = 0;
    
    // 获取复选框状态
    virtual bool GetCellCheckState(int row, int col) const { return false; }
    
    // 设置复选框状态
    virtual void SetCellCheckState(int row, int col, bool checked) {}
    
    // 验证单元格（返回空字符串表示有效，否则返回错误信息）
    virtual std::wstring ValidateCell(int row, int col, const std::wstring& value) const { return L""; }
    
    // 判断单元格是否可以文本编辑（不是复选框且是可编辑的）
    virtual bool IsCellTextEditable(int row, int col) const { return true; }
    
    // 序列化当前状态到字符串数组（用于快照）
    virtual std::vector<std::wstring> SerializeState() const = 0;
    
    // 从字符串数组恢复状态
    virtual void RestoreState(const std::vector<std::wstring>& state) = 0;
    
    // 解析文件内容
    virtual void ParseContent(const std::vector<std::wstring>& lines) = 0;
    
    // 生成文件内容
    virtual std::vector<std::wstring> GenerateContent() const = 0;
    
    // === 可选的虚接口 ===
    
    // 插入新行
    virtual void InsertRow(int afterRow) {}
    
    // 删除行
    virtual void DeleteRow(int row) {}
    
    // 复制行
    virtual void CopyRow(int row) {}
    
    // 粘贴行
    virtual void PasteRow(int afterRow) {}
    
    // 文本被修改时的通知（在输入字符时立即触发）
    virtual void OnTextModified() {}
    
    // === 访问器 ===
    bool IsModified() const { return m_modified; }
    void SetModified(bool modified);
    std::wstring GetFilePath() const { return m_filePath; }
    std::wstring GetFileName() const { return m_fileName; }
    
protected:
    // 通知主窗口文件已被修改
    void NotifyModified();
    
protected:
    // === 绘制相关 ===
    void OnPaint();
    virtual void DrawTable(HDC hdc, const RECT& clientRect);  // 虚函数，允许子类重写
    void DrawHeader(HDC hdc, const RECT& rect, const std::vector<ColumnDef>& cols);
    void DrawRow(HDC hdc, const RECT& rect, int row, const std::vector<ColumnDef>& cols);
    void DrawCell(HDC hdc, const RECT& cellRect, int row, int col, const ColumnDef& colDef);
    void DrawCellTextWithSelection(HDC hdc, const std::wstring& text, const RECT& cellRect, int colPadding);
    void DrawScrollbars(HDC hdc, const RECT& clientRect);
    
    // === 交互相关 ===
    virtual void OnMouseMove(int x, int y, WPARAM wParam);
    virtual void OnLButtonDown(int x, int y);
    virtual void OnLButtonUp(int x, int y);
    virtual void OnRButtonDown(int x, int y);  // 右键点击
    virtual void OnMouseWheel(int delta);
    virtual void OnKeyDown(WPARAM wParam);
    virtual void OnChar(WPARAM wParam);
    
    // === 编辑相关 ===
    virtual void StartEditCell(int row, int col, int clickX);
    void EndEditCell(bool save);
    virtual std::wstring* GetEditingCellTextPtr();
    virtual bool GetEditingCursorScreenPos(int& x, int& y);
    
    // === 滚动条相关 ===
    void UpdateScrollbars(const RECT& clientRect);
    void UpdateVScrollThumb();
    void UpdateHScrollThumb();
    bool IsPointInVScrollThumb(int x, int y);
    bool IsPointInHScrollThumb(int x, int y);
    
    // === 工具方法 ===
    int GetTotalContentHeight() const;
    virtual int GetTotalContentWidth() const;
    virtual void HitTest(int x, int y, int& row, int& col, RECT& cellRect);
    
    // 生成唯一快照ID
    static std::wstring GenerateSnapshotId();
};
