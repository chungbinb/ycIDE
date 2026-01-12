#include "VisualDesigner.h"
#include "EditorContext.h"
#include "ControlRenderer.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <windowsx.h>

VisualDesigner::VisualDesigner(HWND hWnd, EditorContext* context)
    : m_hWnd(hWnd)
    , m_pContext(context)
    , m_modified(false)
    , m_dragMode(DragMode::None)
    , m_zoom(1.0f)
    , m_scrollX(0)
    , m_scrollY(0)
    , m_nextSnapshotId(1)
    , m_showGuideLines(true)
{
    m_fileName = L"未命名.eform";
    
    // 初始化GDI+
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
    
    // 设置默认窗体
    m_formInfo.width = 640;
    m_formInfo.height = 480;
    m_formInfo.title = L"窗体";
    m_formInfo.name = L"窗体1";
}

VisualDesigner::~VisualDesigner()
{
    GdiplusShutdown(m_gdiplusToken);
}

// === 窗口消息处理 ===

LRESULT VisualDesigner::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(m_hWnd, &ps);
            OnPaint(hdc);
            EndPaint(m_hWnd, &ps);
            return 0;
        }
        case WM_SIZE: {
            OnSize(LOWORD(lParam), HIWORD(lParam));
            return 0;
        }
        case WM_LBUTTONDOWN: {
            OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (UINT)wParam);
            return 0;
        }
        case WM_LBUTTONUP: {
            OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (UINT)wParam);
            return 0;
        }
        case WM_MOUSEMOVE: {
            OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (UINT)wParam);
            return 0;
        }
        case WM_RBUTTONDOWN: {
            OnRButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        }
        case WM_KEYDOWN: {
            OnKeyDown((UINT)wParam, (UINT)lParam);
            return 0;
        }
        case WM_MOUSEWHEEL: {
            OnMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
            return 0;
        }
    }
    return DefWindowProc(m_hWnd, msg, wParam, lParam);
}

void VisualDesigner::OnPaint(HDC hdc)
{
    RECT rc;
    GetClientRect(m_hWnd, &rc);
    
    // 创建双缓冲
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);
    
    Graphics g(hdcMem);
    g.SetSmoothingMode(SmoothingModeAntiAlias);
    g.SetTextRenderingHint(TextRenderingHintAntiAlias);
    
    // 绘制背景
    SolidBrush bgBrush(Color(240, 240, 240));
    g.FillRectangle(&bgBrush, 0, 0, rc.right, rc.bottom);
    
    // 绘制画布
    DrawCanvas(g);
    
    // 复制到屏幕
    BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, SRCCOPY);
    
    SelectObject(hdcMem, hbmOld);
    DeleteObject(hbmMem);
    DeleteDC(hdcMem);
}

void VisualDesigner::OnSize(int width, int height)
{
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::OnLButtonDown(int x, int y, UINT flags)
{
    Point canvasPt = ScreenToCanvas(x, y);
    
    // 如果是创建模式
    if (!m_toolControlType.empty()) {
        m_dragMode = DragMode::CreateNew;
        m_dragStartPoint = canvasPt;
        m_dragLastPoint = canvasPt;
        SetCapture(m_hWnd);
        return;
    }
    
    // 检查是否点击了调整大小手柄
    if (!m_selection.selectedControlIds.empty()) {
        DragMode resizeMode = HitTestResizeHandle(canvasPt.X, canvasPt.Y, m_selection.selectionBounds);
        if (resizeMode != DragMode::None) {
            m_dragMode = resizeMode;
            m_dragStartPoint = canvasPt;
            m_dragLastPoint = canvasPt;
            m_dragOriginalBounds = m_selection.selectionBounds;
            
            // 保存所有选中控件的原始位置
            m_dragOriginalBoundsList.clear();
            for (const auto& id : m_selection.selectedControlIds) {
                auto ctrl = GetControl(id);
                if (ctrl) {
                    m_dragOriginalBoundsList.push_back(ctrl->bounds);
                }
            }
            
            SetCapture(m_hWnd);
            return;
        }
    }
    
    // 命中测试
    auto hitControl = HitTest(canvasPt.X, canvasPt.Y);
    
    if (hitControl) {
        bool addToSelection = (flags & MK_CONTROL) != 0;
        SelectControl(hitControl->id, addToSelection);
        
        // 开始移动
        m_dragMode = DragMode::Move;
        m_dragStartPoint = canvasPt;
        m_dragLastPoint = canvasPt;
        
        // 保存所有选中控件的原始位置
        m_dragOriginalBoundsList.clear();
        for (const auto& id : m_selection.selectedControlIds) {
            auto ctrl = GetControl(id);
            if (ctrl) {
                m_dragOriginalBoundsList.push_back(ctrl->bounds);
            }
        }
        
        SetCapture(m_hWnd);
    } else {
        if (!(flags & MK_CONTROL)) {
            ClearSelection();
        }
    }
}

void VisualDesigner::OnLButtonUp(int x, int y, UINT flags)
{
    if (m_dragMode == DragMode::CreateNew && !m_toolControlType.empty()) {
        Point canvasPt = ScreenToCanvas(x, y);
        
        int left = min(m_dragStartPoint.X, canvasPt.X);
        int top = min(m_dragStartPoint.Y, canvasPt.Y);
        int right = max(m_dragStartPoint.X, canvasPt.X);
        int bottom = max(m_dragStartPoint.Y, canvasPt.Y);
        
        // 最小尺寸检查
        if (right - left < 20 || bottom - top < 20) {
            right = left + 80;
            bottom = top + 24;
        }
        
        Rect bounds(left, top, right - left, bottom - top);
        bounds = SnapToGrid(bounds);
        
        AddControl(m_toolControlType, bounds);
        
        // 切换回选择模式
        SetSelectMode();
    } else if (m_dragMode == DragMode::Move || m_dragMode != DragMode::None) {
        CreateSnapshot(L"Move/Resize Controls");
    }
    
    m_dragMode = DragMode::None;
    ReleaseCapture();
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::OnMouseMove(int x, int y, UINT flags)
{
    Point canvasPt = ScreenToCanvas(x, y);
    
    if (m_dragMode == DragMode::CreateNew) {
        m_dragLastPoint = canvasPt;
        InvalidateRect(m_hWnd, NULL, FALSE);
    } else if (m_dragMode == DragMode::Move) {
        int dx = canvasPt.X - m_dragStartPoint.X;
        int dy = canvasPt.Y - m_dragStartPoint.Y;
        
        // 移动所有选中的控件
        int index = 0;
        for (const auto& id : m_selection.selectedControlIds) {
            auto ctrl = GetControl(id);
            if (ctrl && index < m_dragOriginalBoundsList.size()) {
                Rect newBounds = m_dragOriginalBoundsList[index];
                newBounds.X += dx;
                newBounds.Y += dy;
                
                if (m_gridConfig.snapToGrid) {
                    newBounds = SnapToGrid(newBounds);
                }
                
                ctrl->bounds = newBounds;
            }
            index++;
        }
        
        UpdateSelectionBounds();
        m_modified = true;
        InvalidateRect(m_hWnd, NULL, FALSE);
    } else if (m_dragMode != DragMode::None) {
        // 调整大小
        int dx = canvasPt.X - m_dragStartPoint.X;
        int dy = canvasPt.Y - m_dragStartPoint.Y;
        
        // 计算新边界
        Rect newBounds = m_dragOriginalBounds;
        
        if (m_dragMode == DragMode::ResizeRight || m_dragMode == DragMode::ResizeTopRight || 
            m_dragMode == DragMode::ResizeBottomRight) {
            newBounds.Width = max(20, m_dragOriginalBounds.Width + dx);
        }
        if (m_dragMode == DragMode::ResizeLeft || m_dragMode == DragMode::ResizeTopLeft || 
            m_dragMode == DragMode::ResizeBottomLeft) {
            int newX = m_dragOriginalBounds.X + dx;
            int newWidth = m_dragOriginalBounds.Width - dx;
            if (newWidth >= 20) {
                newBounds.X = newX;
                newBounds.Width = newWidth;
            }
        }
        if (m_dragMode == DragMode::ResizeBottom || m_dragMode == DragMode::ResizeBottomLeft || 
            m_dragMode == DragMode::ResizeBottomRight) {
            newBounds.Height = max(20, m_dragOriginalBounds.Height + dy);
        }
        if (m_dragMode == DragMode::ResizeTop || m_dragMode == DragMode::ResizeTopLeft || 
            m_dragMode == DragMode::ResizeTopRight) {
            int newY = m_dragOriginalBounds.Y + dy;
            int newHeight = m_dragOriginalBounds.Height - dy;
            if (newHeight >= 20) {
                newBounds.Y = newY;
                newBounds.Height = newHeight;
            }
        }
        
        if (m_gridConfig.snapToGrid) {
            newBounds = SnapToGrid(newBounds);
        }
        
        // 应用到所有选中控件（等比例缩放）
        float scaleX = (float)newBounds.Width / m_dragOriginalBounds.Width;
        float scaleY = (float)newBounds.Height / m_dragOriginalBounds.Height;
        
        int index = 0;
        for (const auto& id : m_selection.selectedControlIds) {
            auto ctrl = GetControl(id);
            if (ctrl && index < m_dragOriginalBoundsList.size()) {
                Rect originalBounds = m_dragOriginalBoundsList[index];
                
                ctrl->bounds.Width = (int)(originalBounds.Width * scaleX);
                ctrl->bounds.Height = (int)(originalBounds.Height * scaleY);
                ctrl->bounds.X = newBounds.X + (int)((originalBounds.X - m_dragOriginalBounds.X) * scaleX);
                ctrl->bounds.Y = newBounds.Y + (int)((originalBounds.Y - m_dragOriginalBounds.Y) * scaleY);
            }
            index++;
        }
        
        UpdateSelectionBounds();
        m_modified = true;
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

void VisualDesigner::OnRButtonDown(int x, int y)
{
    // TODO: 显示上下文菜单
}

void VisualDesigner::OnKeyDown(UINT vk, UINT flags)
{
    bool ctrlPressed = GetKeyState(VK_CONTROL) & 0x8000;
    
    if (ctrlPressed) {
        switch (vk) {
            case 'Z': Undo(); break;
            case 'Y': Redo(); break;
            case 'X': Cut(); break;
            case 'C': Copy(); break;
            case 'V': Paste(); break;
            case 'A': SelectAll(); break;
        }
    } else {
        switch (vk) {
            case VK_DELETE: DeleteSelectedControls(); break;
            case VK_ESCAPE: SetSelectMode(); ClearSelection(); break;
        }
    }
}

void VisualDesigner::OnMouseWheel(int delta)
{
    // TODO: 缩放支持
}

// === 文件操作 ===

bool VisualDesigner::LoadFile(const std::wstring& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }
    
    try {
        json j;
        file >> j;
        file.close();
        
        FromJson(j);
        
        m_filePath = path;
        size_t pos = path.find_last_of(L"\\/");
        m_fileName = (pos != std::wstring::npos) ? path.substr(pos + 1) : path;
        m_modified = false;
        
        m_undoStack.clear();
        m_redoStack.clear();
        
        InvalidateRect(m_hWnd, NULL, FALSE);
        return true;
    } catch (...) {
        return false;
    }
}

bool VisualDesigner::SaveFile(const std::wstring& path)
{
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }
    
    try {
        json j = ToJson();
        file << j.dump(2);
        file.close();
        
        m_filePath = path;
        size_t pos = path.find_last_of(L"\\/");
        m_fileName = (pos != std::wstring::npos) ? path.substr(pos + 1) : path;
        m_modified = false;
        
        InvalidateRect(m_hWnd, NULL, FALSE);
        return true;
    } catch (...) {
        return false;
    }
}

// === 控件操作 ===

void VisualDesigner::AddControl(const std::wstring& type, const Rect& bounds)
{
    auto control = std::make_shared<ControlInfo>();
    control->id = GenerateControlId(type);
    control->type = type;
    control->name = GenerateControlName(type);
    control->bounds = bounds;
    control->zOrder = (int)m_formInfo.controls.size();
    
    // 设置默认属性
    control->properties[L"标题"] = type;
    control->properties[L"可视"] = L"真";
    control->properties[L"禁止"] = L"假";
    
    m_formInfo.controls.push_back(control);
    
    SelectControl(control->id);
    CreateSnapshot(L"Add Control");
    
    m_modified = true;
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::DeleteControl(const std::wstring& id)
{
    auto it = std::find_if(m_formInfo.controls.begin(), m_formInfo.controls.end(),
        [&id](const std::shared_ptr<ControlInfo>& ctrl) { return ctrl->id == id; });
    
    if (it != m_formInfo.controls.end()) {
        m_formInfo.controls.erase(it);
        
        // 从选择中移除
        auto selIt = std::find(m_selection.selectedControlIds.begin(), 
                              m_selection.selectedControlIds.end(), id);
        if (selIt != m_selection.selectedControlIds.end()) {
            m_selection.selectedControlIds.erase(selIt);
        }
        
        UpdateSelectionBounds();
        m_modified = true;
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

void VisualDesigner::DeleteSelectedControls()
{
    if (m_selection.selectedControlIds.empty()) {
        return;
    }
    
    auto ids = m_selection.selectedControlIds;  // 复制一份
    for (const auto& id : ids) {
        DeleteControl(id);
    }
    
    CreateSnapshot(L"Delete Controls");
}

std::shared_ptr<ControlInfo> VisualDesigner::GetControl(const std::wstring& id)
{
    for (auto& ctrl : m_formInfo.controls) {
        if (ctrl->id == id) {
            return ctrl;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<ControlInfo>> VisualDesigner::GetSelectedControls()
{
    std::vector<std::shared_ptr<ControlInfo>> result;
    for (const auto& id : m_selection.selectedControlIds) {
        auto ctrl = GetControl(id);
        if (ctrl) {
            result.push_back(ctrl);
        }
    }
    return result;
}

// === 选择操作 ===

void VisualDesigner::SelectControl(const std::wstring& id, bool addToSelection)
{
    if (!addToSelection) {
        m_selection.selectedControlIds.clear();
    }
    
    auto it = std::find(m_selection.selectedControlIds.begin(), 
                       m_selection.selectedControlIds.end(), id);
    
    if (it == m_selection.selectedControlIds.end()) {
        m_selection.selectedControlIds.push_back(id);
    }
    
    m_selection.isMultiSelect = m_selection.selectedControlIds.size() > 1;
    UpdateSelectionBounds();
    
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::ClearSelection()
{
    m_selection.selectedControlIds.clear();
    m_selection.isMultiSelect = false;
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::SelectAll()
{
    m_selection.selectedControlIds.clear();
    for (const auto& ctrl : m_formInfo.controls) {
        m_selection.selectedControlIds.push_back(ctrl->id);
    }
    m_selection.isMultiSelect = m_selection.selectedControlIds.size() > 1;
    UpdateSelectionBounds();
    InvalidateRect(m_hWnd, NULL, FALSE);
}

std::shared_ptr<ControlInfo> VisualDesigner::HitTest(int x, int y)
{
    // 从前往后（Z序从大到小）查找
    for (auto it = m_formInfo.controls.rbegin(); it != m_formInfo.controls.rend(); ++it) {
        auto& ctrl = *it;
        if (x >= ctrl->bounds.X && x < ctrl->bounds.X + ctrl->bounds.Width &&
            y >= ctrl->bounds.Y && y < ctrl->bounds.Y + ctrl->bounds.Height) {
            return ctrl;
        }
    }
    return nullptr;
}

// 省略其他方法实现...（因篇幅限制，后续可以继续实现）

// === 内部方法 ===

void VisualDesigner::DrawCanvas(Graphics& g)
{
    RECT clientRect;
    GetClientRect(m_hWnd, &clientRect);
    
    // 计算画布位置（居中）
    int canvasScreenX = (clientRect.right - (int)(m_formInfo.width * m_zoom)) / 2 + m_scrollX;
    int canvasScreenY = (clientRect.bottom - (int)(m_formInfo.height * m_zoom)) / 2 + m_scrollY;
    
    Rect canvasRect(canvasScreenX, canvasScreenY, 
                    (int)(m_formInfo.width * m_zoom), 
                    (int)(m_formInfo.height * m_zoom));
    
    // 绘制画布阴影
    SolidBrush shadowBrush(Color(150, 0, 0, 0));
    g.FillRectangle(&shadowBrush, canvasRect.X + 4, canvasRect.Y + 4, 
                    canvasRect.Width, canvasRect.Height);
    
    // 绘制画布背景
    SolidBrush canvasBrush(Color(255, 255, 255));
    g.FillRectangle(&canvasBrush, canvasRect);
    
    // 绘制网格
    if (m_gridConfig.showGrid) {
        DrawGrid(g, canvasRect);
    }
    
    // 绘制辅助线
    if (m_showGuideLines) {
        DrawGuideLines(g, canvasRect);
    }
    
    // 绘制控件
    DrawControls(g);
    
    // 绘制选择框
    DrawSelection(g);
    
    // 如果正在创建新控件，绘制预览框
    if (m_dragMode == DragMode::CreateNew) {
        Point start = CanvasToScreen(m_dragStartPoint.X, m_dragStartPoint.Y);
        Point end = CanvasToScreen(m_dragLastPoint.X, m_dragLastPoint.Y);
        
        Rect previewRect(min(start.X, end.X), min(start.Y, end.Y),
                        abs(end.X - start.X), abs(end.Y - start.Y));
        
        Pen dashedPen(Color(100, 0, 0, 255), 1.0f);
        dashedPen.SetDashStyle(DashStyleDash);
        g.DrawRectangle(&dashedPen, previewRect);
    }
    
    // 绘制画布边框
    Pen borderPen(Color(200, 200, 200), 1.0f);
    g.DrawRectangle(&borderPen, canvasRect);
}

void VisualDesigner::DrawGrid(Graphics& g, const Rect& canvasRect)
{
    Pen gridPen(m_gridConfig.gridColor, 1.0f);
    
    int gridSize = (int)(m_gridConfig.gridSize * m_zoom);
    
    // 绘制垂直线
    for (int x = 0; x <= m_formInfo.width; x += m_gridConfig.gridSize) {
        int screenX = canvasRect.X + (int)(x * m_zoom);
        g.DrawLine(&gridPen, screenX, canvasRect.Y, screenX, canvasRect.Y + canvasRect.Height);
    }
    
    // 绘制水平线
    for (int y = 0; y <= m_formInfo.height; y += m_gridConfig.gridSize) {
        int screenY = canvasRect.Y + (int)(y * m_zoom);
        g.DrawLine(&gridPen, canvasRect.X, screenY, canvasRect.X + canvasRect.Width, screenY);
    }
}

void VisualDesigner::DrawGuideLines(Graphics& g, const Rect& canvasRect)
{
    for (const auto& guide : m_guideLines) {
        Pen guidePen(guide.color, 1.0f);
        guidePen.SetDashStyle(DashStyleDot);
        
        if (guide.isHorizontal) {
            int screenY = canvasRect.Y + (int)(guide.position * m_zoom);
            g.DrawLine(&guidePen, canvasRect.X, screenY, 
                      canvasRect.X + canvasRect.Width, screenY);
        } else {
            int screenX = canvasRect.X + (int)(guide.position * m_zoom);
            g.DrawLine(&guidePen, screenX, canvasRect.Y, 
                      screenX, canvasRect.Y + canvasRect.Height);
        }
    }
}

void VisualDesigner::DrawControls(Graphics& g)
{
    // TODO: 使用ControlRenderer绘制控件
    for (const auto& ctrl : m_formInfo.controls) {
        DrawControl(g, *ctrl);
    }
}

void VisualDesigner::DrawControl(Graphics& g, const ControlInfo& control)
{
    Rect screenBounds = CanvasToScreen(control.bounds);
    
    // 简单的默认渲染（实际应该使用ControlRenderer）
    SolidBrush bgBrush(Color(240, 240, 240));
    Pen borderPen(Color(128, 128, 128), 1.0f);
    
    g.FillRectangle(&bgBrush, screenBounds);
    g.DrawRectangle(&borderPen, screenBounds);
    
    // 绘制控件名称
    FontFamily fontFamily(L"微软雅黑");
    Font font(&fontFamily, 9, FontStyleRegular, UnitPoint);
    SolidBrush textBrush(Color(0, 0, 0));
    
    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentCenter);
    
    RectF textRect((REAL)screenBounds.X, (REAL)screenBounds.Y, 
                   (REAL)screenBounds.Width, (REAL)screenBounds.Height);
    g.DrawString(control.type.c_str(), -1, &font, textRect, &format, &textBrush);
}

void VisualDesigner::DrawSelection(Graphics& g)
{
    if (m_selection.selectedControlIds.empty()) {
        return;
    }
    
    Rect screenBounds = CanvasToScreen(m_selection.selectionBounds);
    
    // 绘制选择框
    Pen selectionPen(Color(0, 120, 215), 2.0f);
    g.DrawRectangle(&selectionPen, screenBounds);
    
    // 绘制调整大小手柄
    DrawSelectionHandles(g, screenBounds);
}

void VisualDesigner::DrawSelectionHandles(Graphics& g, const Rect& bounds)
{
    const int handleSize = 6;
    SolidBrush handleBrush(Color(255, 255, 255));
    Pen handlePen(Color(0, 120, 215), 1.0f);
    
    Point handles[8] = {
        Point(bounds.X, bounds.Y),  // 左上
        Point(bounds.X + bounds.Width / 2, bounds.Y),  // 上中
        Point(bounds.X + bounds.Width, bounds.Y),  // 右上
        Point(bounds.X + bounds.Width, bounds.Y + bounds.Height / 2),  // 右中
        Point(bounds.X + bounds.Width, bounds.Y + bounds.Height),  // 右下
        Point(bounds.X + bounds.Width / 2, bounds.Y + bounds.Height),  // 下中
        Point(bounds.X, bounds.Y + bounds.Height),  // 左下
        Point(bounds.X, bounds.Y + bounds.Height / 2)  // 左中
    };
    
    for (const auto& pt : handles) {
        Rect handleRect(pt.X - handleSize / 2, pt.Y - handleSize / 2, handleSize, handleSize);
        g.FillRectangle(&handleBrush, handleRect);
        g.DrawRectangle(&handlePen, handleRect);
    }
}

// 坐标转换
Point VisualDesigner::ScreenToCanvas(int x, int y) const
{
    RECT clientRect;
    GetClientRect(m_hWnd, &clientRect);
    
    int canvasScreenX = (clientRect.right - (int)(m_formInfo.width * m_zoom)) / 2 + m_scrollX;
    int canvasScreenY = (clientRect.bottom - (int)(m_formInfo.height * m_zoom)) / 2 + m_scrollY;
    
    int canvasX = (int)((x - canvasScreenX) / m_zoom);
    int canvasY = (int)((y - canvasScreenY) / m_zoom);
    
    return Point(canvasX, canvasY);
}

Rect VisualDesigner::ScreenToCanvas(const Rect& rect) const
{
    Point topLeft = ScreenToCanvas(rect.X, rect.Y);
    int width = (int)(rect.Width / m_zoom);
    int height = (int)(rect.Height / m_zoom);
    
    return Rect(topLeft.X, topLeft.Y, width, height);
}

Point VisualDesigner::CanvasToScreen(int x, int y) const
{
    RECT clientRect;
    GetClientRect(m_hWnd, &clientRect);
    
    int canvasScreenX = (clientRect.right - (int)(m_formInfo.width * m_zoom)) / 2 + m_scrollX;
    int canvasScreenY = (clientRect.bottom - (int)(m_formInfo.height * m_zoom)) / 2 + m_scrollY;
    
    int screenX = (int)(x * m_zoom) + canvasScreenX;
    int screenY = (int)(y * m_zoom) + canvasScreenY;
    
    return Point(screenX, screenY);
}

Rect VisualDesigner::CanvasToScreen(const Rect& rect) const
{
    Point topLeft = CanvasToScreen(rect.X, rect.Y);
    int width = (int)(rect.Width * m_zoom);
    int height = (int)(rect.Height * m_zoom);
    
    return Rect(topLeft.X, topLeft.Y, width, height);
}

// 网格对齐
int VisualDesigner::SnapToGrid(int value) const
{
    if (!m_gridConfig.snapToGrid) {
        return value;
    }
    return (value / m_gridConfig.gridSize) * m_gridConfig.gridSize;
}

Point VisualDesigner::SnapToGrid(const Point& pt) const
{
    return Point(SnapToGrid(pt.X), SnapToGrid(pt.Y));
}

Rect VisualDesigner::SnapToGrid(const Rect& rect) const
{
    return Rect(SnapToGrid(rect.X), SnapToGrid(rect.Y), 
                SnapToGrid(rect.Width), SnapToGrid(rect.Height));
}

// 命中测试调整大小手柄
DragMode VisualDesigner::HitTestResizeHandle(int x, int y, const Rect& bounds)
{
    const int tolerance = 4;
    
    bool nearLeft = abs(x - bounds.X) <= tolerance;
    bool nearRight = abs(x - (bounds.X + bounds.Width)) <= tolerance;
    bool nearTop = abs(y - bounds.Y) <= tolerance;
    bool nearBottom = abs(y - (bounds.Y + bounds.Height)) <= tolerance;
    bool inHRange = x >= bounds.X - tolerance && x <= bounds.X + bounds.Width + tolerance;
    bool inVRange = y >= bounds.Y - tolerance && y <= bounds.Y + bounds.Height + tolerance;
    
    if (nearLeft && nearTop) return DragMode::ResizeTopLeft;
    if (nearRight && nearTop) return DragMode::ResizeTopRight;
    if (nearLeft && nearBottom) return DragMode::ResizeBottomLeft;
    if (nearRight && nearBottom) return DragMode::ResizeBottomRight;
    if (nearLeft && inVRange) return DragMode::ResizeLeft;
    if (nearRight && inVRange) return DragMode::ResizeRight;
    if (nearTop && inHRange) return DragMode::ResizeTop;
    if (nearBottom && inHRange) return DragMode::ResizeBottom;
    
    return DragMode::None;
}

// 更新选择框边界
void VisualDesigner::UpdateSelectionBounds()
{
    if (m_selection.selectedControlIds.empty()) {
        return;
    }
    
    int minX = INT_MAX, minY = INT_MAX;
    int maxX = INT_MIN, maxY = INT_MIN;
    
    for (const auto& id : m_selection.selectedControlIds) {
        auto ctrl = GetControl(id);
        if (ctrl) {
            minX = min(minX, ctrl->bounds.X);
            minY = min(minY, ctrl->bounds.Y);
            maxX = max(maxX, ctrl->bounds.X + ctrl->bounds.Width);
            maxY = max(maxY, ctrl->bounds.Y + ctrl->bounds.Height);
        }
    }
    
    m_selection.selectionBounds = Rect(minX, minY, maxX - minX, maxY - minY);
}

// 快照管理
void VisualDesigner::CreateSnapshot(const std::wstring& description)
{
    DesignerSnapshot snapshot;
    snapshot.id = m_nextSnapshotId++;
    snapshot.description = description;
    GetSystemTime(&snapshot.timestamp);
    snapshot.formData = m_formInfo;
    
    m_undoStack.push_back(snapshot);
    
    if (m_undoStack.size() > MAX_UNDO_STACK) {
        m_undoStack.erase(m_undoStack.begin());
    }
    
    m_redoStack.clear();
}

void VisualDesigner::RestoreSnapshot(const DesignerSnapshot& snapshot)
{
    m_formInfo = snapshot.formData;
    ClearSelection();
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::Undo()
{
    if (m_undoStack.empty()) {
        return;
    }
    
    // 保存当前状态到重做栈
    DesignerSnapshot currentSnapshot;
    currentSnapshot.id = m_nextSnapshotId++;
    currentSnapshot.description = L"Current";
    GetSystemTime(&currentSnapshot.timestamp);
    currentSnapshot.formData = m_formInfo;
    m_redoStack.push_back(currentSnapshot);
    
    // 恢复上一个状态
    DesignerSnapshot snapshot = m_undoStack.back();
    m_undoStack.pop_back();
    RestoreSnapshot(snapshot);
}

void VisualDesigner::Redo()
{
    if (m_redoStack.empty()) {
        return;
    }
    
    DesignerSnapshot snapshot = m_redoStack.back();
    m_redoStack.pop_back();
    
    CreateSnapshot(L"Redo");
    RestoreSnapshot(snapshot);
}

// ID和名称生成
std::wstring VisualDesigner::GenerateControlId(const std::wstring& type)
{
    static int idCounter = 1;
    return L"ctrl_" + std::to_wstring(idCounter++);
}

std::wstring VisualDesigner::GenerateControlName(const std::wstring& type)
{
    int count = 1;
    for (const auto& ctrl : m_formInfo.controls) {
        if (ctrl->type == type) {
            count++;
        }
    }
    return type + std::to_wstring(count);
}

// 工具模式
void VisualDesigner::SetToolMode(const std::wstring& controlType)
{
    m_toolControlType = controlType;
    // TODO: 更改鼠标光标
}

void VisualDesigner::SetSelectMode()
{
    m_toolControlType.clear();
    // TODO: 恢复默认光标
}

// JSON序列化
json VisualDesigner::ToJson() const
{
    json j;
    j["version"] = 1;
    j["formName"] = WStringToUtf8(m_formInfo.name);
    j["formTitle"] = WStringToUtf8(m_formInfo.title);
    j["formWidth"] = m_formInfo.width;
    j["formHeight"] = m_formInfo.height;
    
    j["controls"] = json::array();
    for (const auto& ctrl : m_formInfo.controls) {
        json ctrlJson;
        ctrlJson["id"] = WStringToUtf8(ctrl->id);
        ctrlJson["type"] = WStringToUtf8(ctrl->type);
        ctrlJson["name"] = WStringToUtf8(ctrl->name);
        ctrlJson["x"] = ctrl->bounds.X;
        ctrlJson["y"] = ctrl->bounds.Y;
        ctrlJson["width"] = ctrl->bounds.Width;
        ctrlJson["height"] = ctrl->bounds.Height;
        ctrlJson["zOrder"] = ctrl->zOrder;
        
        json props = json::object();
        for (const auto& prop : ctrl->properties) {
            props[WStringToUtf8(prop.first)] = WStringToUtf8(prop.second);
        }
        ctrlJson["properties"] = props;
        
        j["controls"].push_back(ctrlJson);
    }
    
    return j;
}

void VisualDesigner::FromJson(const json& j)
{
    m_formInfo.controls.clear();
    
    if (j.contains("formName")) m_formInfo.name = Utf8ToWString(j["formName"]);
    if (j.contains("formTitle")) m_formInfo.title = Utf8ToWString(j["formTitle"]);
    if (j.contains("formWidth")) m_formInfo.width = j["formWidth"];
    if (j.contains("formHeight")) m_formInfo.height = j["formHeight"];
    
    if (j.contains("controls") && j["controls"].is_array()) {
        for (const auto& ctrlJson : j["controls"]) {
            auto ctrl = std::make_shared<ControlInfo>();
            
            if (ctrlJson.contains("id")) ctrl->id = Utf8ToWString(ctrlJson["id"]);
            if (ctrlJson.contains("type")) ctrl->type = Utf8ToWString(ctrlJson["type"]);
            if (ctrlJson.contains("name")) ctrl->name = Utf8ToWString(ctrlJson["name"]);
            
            int x = ctrlJson.value("x", 0);
            int y = ctrlJson.value("y", 0);
            int width = ctrlJson.value("width", 80);
            int height = ctrlJson.value("height", 24);
            ctrl->bounds = Rect(x, y, width, height);
            
            ctrl->zOrder = ctrlJson.value("zOrder", 0);
            
            if (ctrlJson.contains("properties") && ctrlJson["properties"].is_object()) {
                for (auto& [key, value] : ctrlJson["properties"].items()) {
                    ctrl->properties[Utf8ToWString(key)] = Utf8ToWString(value.get<std::string>());
                }
            }
            
            m_formInfo.controls.push_back(ctrl);
        }
    }
}

std::string VisualDesigner::WStringToUtf8(const std::wstring& wstr) const
{
    if (wstr.empty()) return "";
    
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (len <= 0) return "";
    
    std::string utf8Str(len - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8Str[0], len, NULL, NULL);
    
    return utf8Str;
}

std::wstring VisualDesigner::Utf8ToWString(const std::string& str) const
{
    if (str.empty()) return L"";
    
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    if (len <= 0) return L"";
    
    std::wstring wstr(len - 1, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], len);
    
    return wstr;
}

// === 剪贴板操作 ===

void VisualDesigner::Cut()
{
    Copy();
    DeleteSelectedControls();
}

void VisualDesigner::Copy()
{
    auto selectedControls = GetSelectedControls();
    if (selectedControls.empty()) {
        return;
    }
    
    // 创建JSON数组保存控件信息
    json clipboardData = json::array();
    
    for (const auto& ctrl : selectedControls) {
        json ctrlJson;
        ctrlJson["id"] = WStringToUtf8(ctrl->id);
        ctrlJson["type"] = WStringToUtf8(ctrl->type);
        ctrlJson["name"] = WStringToUtf8(ctrl->name);
        ctrlJson["x"] = ctrl->bounds.X;
        ctrlJson["y"] = ctrl->bounds.Y;
        ctrlJson["width"] = ctrl->bounds.Width;
        ctrlJson["height"] = ctrl->bounds.Height;
        ctrlJson["zOrder"] = ctrl->zOrder;
        
        // 保存属性
        json propsJson;
        for (const auto& [key, value] : ctrl->properties) {
            propsJson[WStringToUtf8(key)] = WStringToUtf8(value);
        }
        ctrlJson["properties"] = propsJson;
        
        clipboardData.push_back(ctrlJson);
    }
    
    // 转换为字符串并复制到剪贴板
    std::string clipboardText = clipboardData.dump();
    
    if (OpenClipboard(m_hWnd)) {
        EmptyClipboard();
        
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, clipboardText.size() + 1);
        if (hMem) {
            char* pMem = (char*)GlobalLock(hMem);
            if (pMem) {
                memcpy(pMem, clipboardText.c_str(), clipboardText.size() + 1);
                GlobalUnlock(hMem);
                SetClipboardData(CF_TEXT, hMem);
            }
        }
        
        CloseClipboard();
    }
}

void VisualDesigner::Paste()
{
    if (!IsClipboardFormatAvailable(CF_TEXT)) {
        return;
    }
    
    if (!OpenClipboard(m_hWnd)) {
        return;
    }
    
    HGLOBAL hMem = GetClipboardData(CF_TEXT);
    if (!hMem) {
        CloseClipboard();
        return;
    }
    
    char* pMem = (char*)GlobalLock(hMem);
    if (!pMem) {
        CloseClipboard();
        return;
    }
    
    std::string clipboardText(pMem);
    GlobalUnlock(hMem);
    CloseClipboard();
    
    // 尝试解析JSON
    try {
        json clipboardData = json::parse(clipboardText);
        
        if (!clipboardData.is_array()) {
            return;
        }
        
        // 清除当前选择
        ClearSelection();
        
        // 粘贴偏移量（避免完全重叠）
        int offsetX = 20;
        int offsetY = 20;
        
        // 创建快照
        CreateSnapshot(L"粘贴控件");
        
        for (const auto& ctrlJson : clipboardData) {
            if (!ctrlJson.is_object()) {
                continue;
            }
            
            // 创建新控件
            auto ctrl = std::make_shared<ControlInfo>();
            ctrl->type = Utf8ToWString(ctrlJson.value("type", ""));
            ctrl->id = GenerateControlId(ctrl->type);  // 生成新ID
            ctrl->name = Utf8ToWString(ctrlJson.value("name", "")) + L"_副本";
            
            // 设置位置（加上偏移）
            ctrl->bounds.X = ctrlJson.value("x", 0) + offsetX;
            ctrl->bounds.Y = ctrlJson.value("y", 0) + offsetY;
            ctrl->bounds.Width = ctrlJson.value("width", 80);
            ctrl->bounds.Height = ctrlJson.value("height", 24);
            ctrl->zOrder = static_cast<int>(m_formInfo.controls.size());
            
            // 恢复属性
            if (ctrlJson.contains("properties") && ctrlJson["properties"].is_object()) {
                for (auto& [key, value] : ctrlJson["properties"].items()) {
                    ctrl->properties[Utf8ToWString(key)] = Utf8ToWString(value.get<std::string>());
                }
            }
            
            m_formInfo.controls.push_back(ctrl);
            
            // 选中新粘贴的控件
            SelectControl(ctrl->id, true);
        }
        
        m_modified = true;
        InvalidateRect(m_hWnd, NULL, FALSE);
        
    } catch (...) {
        // 剪贴板内容不是有效的控件数据
        return;
    }
}

// 省略其他方法实现（对齐、排列、层级操作等）...
