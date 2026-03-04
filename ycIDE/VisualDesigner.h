#pragma once
#include "TableEditor.h"
#include "json.hpp"
#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>
#include <memory>
#include <functional>

using json = nlohmann::json;
using namespace Gdiplus;

// 前向声明
class ControlRenderer;

// 控件信息
struct ControlInfo {
    std::wstring id;              // 控件唯一ID
    std::wstring type;            // 控件类型（如"按钮"）
    std::wstring name;            // 控件名称
    Rect bounds;                  // 位置和大小
    std::map<std::wstring, std::wstring> properties;  // 属性键值对
    int zOrder;                   // Z序（层级）
    
    ControlInfo() : zOrder(0) {}
};

// 窗体信息
struct FormInfo {
    std::wstring name;            // 窗体名称
    int width;                    // 窗体宽度
    int height;                   // 窗体高度
    std::wstring title;           // 窗体标题
    int borderStyle;              // 边框样式 (0=无边框, 1=普通可调, 2=普通固定...)
    bool hasControlBox;           // 是否有控制按钮
    bool hasMaxButton;            // 是否有最大化按钮
    bool hasMinButton;            // 是否有最小化按钮
    COLORREF backColor;           // 背景色
    std::vector<std::shared_ptr<ControlInfo>> controls;  // 控件列表
    std::map<std::wstring, std::wstring> properties;     // 窗体属性
    
    FormInfo() : width(640), height(480), title(L"窗体"), 
                 borderStyle(1), hasControlBox(true), 
                 hasMaxButton(true), hasMinButton(true),
                 backColor(RGB(240, 240, 240)) {}
};

// 设计器快照（用于撤销/重做）
struct DesignerSnapshot {
    int id;
    std::wstring description;
    SYSTEMTIME timestamp;
    FormInfo formData;
    
    DesignerSnapshot() : id(0) {}
};

// 选择信息
struct SelectionInfo {
    std::vector<std::wstring> selectedControlIds;
    Rect selectionBounds;         // 选中控件的包围框
    bool isMultiSelect;
    bool isFormSelected;          // 是否选中了窗口本身
    
    SelectionInfo() : isMultiSelect(false), isFormSelected(false) {}
};

// 拖动模式
enum class DragMode {
    None,
    Move,              // 移动控件
    ResizeTopLeft,
    ResizeTop,
    ResizeTopRight,
    ResizeRight,
    ResizeBottomRight,
    ResizeBottom,
    ResizeBottomLeft,
    ResizeLeft,
    CreateNew          // 创建新控件
};

// 网格配置
struct GridConfig {
    bool showGrid;             // 显示网格
    int gridSize;              // 网格大小（像素）
    bool snapToGrid;           // 对齐到网格
    Color gridColor;           // 网格颜色
    
    GridConfig() : showGrid(true), gridSize(8), snapToGrid(true), 
                   gridColor(Color(200, 200, 200)) {}
};

// 辅助线
struct GuideLine {
    bool isHorizontal;         // true=水平线，false=垂直线
    int position;              // 位置（像素）
    Color color;               // 颜色
    
    GuideLine() : isHorizontal(true), position(0), color(Color(255, 0, 128, 255)) {}
};

class EditorContext;

class VisualDesigner
{
public:
    VisualDesigner(HWND hWnd, EditorContext* context = nullptr);
    virtual ~VisualDesigner();
    
    // === 窗口消息处理 ===
    
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    void OnPaint(HDC hdc);
    void OnSize(int width, int height);
    void OnLButtonDown(int x, int y, UINT flags);
    void OnLButtonUp(int x, int y, UINT flags);
    void OnLButtonDblClk(int x, int y, UINT flags);  // 双击事件
    void OnMouseMove(int x, int y, UINT flags);
    void OnRButtonDown(int x, int y);
    void OnKeyDown(UINT vk, UINT flags);
    void OnMouseWheel(int delta);
    void OnHScroll(UINT nSBCode, UINT nPos);  // 水平滚动条处理
    void OnVScroll(UINT nSBCode, UINT nPos);  // 垂直滚动条处理
    
    // === 文件操作 ===
    
    bool LoadFile(const std::wstring& path);
    bool SaveFile(const std::wstring& path);
    bool IsModified() const { return m_modified; }
    void SetModified(bool modified);
    std::wstring GetFileName() const { return m_fileName; }
    std::wstring GetFilePath() const { return m_filePath; }
    void SetFilePath(const std::wstring& path) { 
        m_filePath = path; 
        size_t pos = path.find_last_of(L"\\/");
        m_fileName = (pos != std::wstring::npos) ? path.substr(pos + 1) : path;
    }
    
    // === 控件操作 ===
    
    void AddControl(const std::wstring& type, const Rect& bounds);
    void DeleteControl(const std::wstring& id);
    void DeleteSelectedControls();
    std::shared_ptr<ControlInfo> GetControl(const std::wstring& id);
    std::vector<std::shared_ptr<ControlInfo>> GetSelectedControls();
    
    // === 选择操作 ===
    
    void SelectControl(const std::wstring& id, bool addToSelection = false);
    void SelectForm();  // 选中窗口本身
    void ClearSelection();
    void SelectAll();
    std::shared_ptr<ControlInfo> HitTest(int x, int y);
    bool IsFormSelected() const { return m_selection.isFormSelected; }
    
    // === 对齐和排列 ===
    
    void AlignLeft();
    void AlignRight();
    void AlignTop();
    void AlignBottom();
    void AlignCenterHorizontal();
    void AlignCenterVertical();
    void DistributeHorizontal();
    void DistributeVertical();
    void MakeSameWidth();
    void MakeSameHeight();
    void MakeSameSize();
    
    // === 层级操作 ===
    
    void BringToFront();
    void SendToBack();
    void BringForward();
    void SendBackward();
    
    // === 撤销/重做 ===
    
    void Undo();
    void Redo();
    bool CanUndo() const { return !m_undoStack.empty(); }
    bool CanRedo() const { return !m_redoStack.empty(); }
    
    // === 剪贴板操作 ===
    
    void Cut();
    void Copy();
    void Paste();
    
    // === 网格和辅助线 ===
    
    void SetGridConfig(const GridConfig& config);
    GridConfig GetGridConfig() const { return m_gridConfig; }
    void AddGuideLine(bool isHorizontal, int position);
    void RemoveGuideLine(int index);
    void ClearGuideLines();
    
    // === 缩放和滚动 ===
    
    void SetZoom(float zoom);
    float GetZoom() const { return m_zoom; }
    void SetScroll(int x, int y);
    Point GetScroll() const { return Point(m_scrollX, m_scrollY); }
    
    // === 属性访问 ===
    
    FormInfo& GetFormInfo() { return m_formInfo; }
    void SetFormProperty(const std::wstring& key, const std::wstring& value);
    void SetControlProperty(const std::wstring& controlId, const std::wstring& key, const std::wstring& value);
    
    // === 工具模式 ===
    
    void SetToolMode(const std::wstring& controlType);  // 设置要创建的控件类型
    void SetSelectMode();  // 切换到选择模式
    
    // 获取ControlRenderer
    ControlRenderer* GetControlRenderer() const { return m_pControlRenderer; }
    
    // 选择变更通知回调
    using SelectionChangedCallback = std::function<void()>;
    void SetSelectionChangedCallback(SelectionChangedCallback callback) { m_selectionChangedCallback = callback; }
    
    // 双击回调（双击窗口/控件跳转到关联源代码文件和事件处理子程序）
    using DblClickCallback = std::function<void(const std::wstring& controlName, const std::wstring& controlType)>;
    void SetDblClickCallback(DblClickCallback callback) { m_dblClickCallback = callback; }
    
    // 光标
    HCURSOR GetResizeCursor();  // 根据鼠标位置获取调整大小光标
    
private:
    HWND m_hWnd;
    EditorContext* m_pContext;
    ControlRenderer* m_pControlRenderer;  // 控件渲染器
    
    // 窗体数据
    FormInfo m_formInfo;
    std::wstring m_fileName;
    std::wstring m_filePath;
    bool m_modified;
    
    // 选择状态
    SelectionInfo m_selection;
    SelectionChangedCallback m_selectionChangedCallback;  // 选择变更回调
    DblClickCallback m_dblClickCallback;                  // 双击回调
    
    // 拖动状态
    DragMode m_dragMode;
    Point m_dragStartPoint;
    Point m_dragLastPoint;
    Rect m_dragOriginalBounds;
    std::vector<Rect> m_dragOriginalBoundsList;  // 多选时的原始边界
    
    // 工具模式
    std::wstring m_toolControlType;  // 要创建的控件类型，空字符串表示选择模式
    
    // 网格和辅助线
    GridConfig m_gridConfig;
    std::vector<GuideLine> m_guideLines;
    bool m_showGuideLines;
    
    // 缩放和滚动
    float m_zoom;
    int m_scrollX;
    int m_scrollY;
    
    // 客户区域（标题栏下方的内容区域）
    Gdiplus::Rect m_clientAreaRect;
    
    // 撤销/重做
    std::vector<DesignerSnapshot> m_undoStack;
    std::vector<DesignerSnapshot> m_redoStack;
    int m_nextSnapshotId;
    static const int MAX_UNDO_STACK = 50;
    
    // GDI+
    ULONG_PTR m_gdiplusToken;
    
    // === 内部方法 ===
    
    // 渲染
    void DrawCanvas(Graphics& g);
    void DrawGrid(Graphics& g, const Rect& canvasRect);
    void DrawGuideLines(Graphics& g, const Rect& canvasRect);
    void DrawControls(Graphics& g);
    void DrawControl(Graphics& g, const ControlInfo& control);
    void DrawSelection(Graphics& g);
    void DrawSelectionHandles(Graphics& g, const Rect& bounds);
    void DrawFormSelectionHandles(Graphics& g, const Rect& bounds);  // 窗口选择手柄（只有右边、下边、右下角）
    
    // 滚动条管理
    void UpdateScrollRange();  // 根据窗口大小更新滚动范围
    
    // 坐标转换
    Point ScreenToCanvas(int x, int y) const;
    Rect ScreenToCanvas(const Rect& rect) const;
    Point CanvasToScreen(int x, int y) const;
    Rect CanvasToScreen(const Rect& rect) const;
    
    // 网格对齐
    int SnapToGrid(int value) const;
    Point SnapToGrid(const Point& pt) const;
    Rect SnapToGrid(const Rect& rect) const;
    
    // 命中测试
    DragMode HitTestResizeHandle(int x, int y, const Rect& bounds);
    
    // 选择框计算
    void UpdateSelectionBounds();
    
    // 快照管理
    void CreateSnapshot(const std::wstring& description);
    void RestoreSnapshot(const DesignerSnapshot& snapshot);
    
    // ID生成
    std::wstring GenerateControlId(const std::wstring& type);
    std::wstring GenerateControlName(const std::wstring& type);
    
    // JSON序列化
    json ToJson() const;
    void FromJson(const json& j);
    
    // UTF8转换
    std::string WStringToUtf8(const std::wstring& wstr) const;
    std::wstring Utf8ToWString(const std::string& str) const;
};
