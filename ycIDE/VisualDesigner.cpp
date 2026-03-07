#include "VisualDesigner.h"
#include "EditorContext.h"
#include "ControlRenderer.h"
#include "LibraryParser.h"
#include "Theme.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <windowsx.h>

extern AppTheme g_CurrentTheme;

// 定义min和max宏（如果未定义）
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

// 全局ControlRenderer单例
static ControlRenderer* g_pControlRenderer = nullptr;

ControlRenderer* GetGlobalControlRenderer()
{
    if (!g_pControlRenderer) {
        g_pControlRenderer = new ControlRenderer();
    }
    return g_pControlRenderer;
}

VisualDesigner::VisualDesigner(HWND hWnd, EditorContext* context)
    : m_hWnd(hWnd)
    , m_pContext(context)
    , m_pControlRenderer(GetGlobalControlRenderer())
    , m_modified(false)
    , m_dragMode(DragMode::None)
    , m_zoom(1.0f)
    , m_scrollX(0)
    , m_scrollY(0)
    , m_nextSnapshotId(1)
    , m_showGuideLines(true)
{
    m_fileName = L"未命名.efw";
    
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
        case WM_SETCURSOR: {
            if (LOWORD(lParam) == HTCLIENT) {
                HCURSOR hCursor = GetResizeCursor();
                if (hCursor) {
                    SetCursor(hCursor);
                    return TRUE;
                }
                // 创建模式时使用十字光标
                if (!m_toolControlType.empty()) {
                    SetCursor(LoadCursor(NULL, IDC_CROSS));
                    return TRUE;
                }
                // 默认使用箭头光标
                SetCursor(LoadCursor(NULL, IDC_ARROW));
                return TRUE;
            }
            break;
        }
        case WM_LBUTTONDOWN: {
            OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (UINT)wParam);
            return 0;
        }
        case WM_LBUTTONUP: {
            OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (UINT)wParam);
            return 0;
        }
        case WM_LBUTTONDBLCLK: {
            OnLButtonDblClk(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (UINT)wParam);
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
        case WM_HSCROLL: {
            OnHScroll(LOWORD(wParam), HIWORD(wParam));
            return 0;
        }
        case WM_VSCROLL: {
            OnVScroll(LOWORD(wParam), HIWORD(wParam));
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
    
    // 绘制背景（使用主题色）
    SolidBrush bgBrush(Color(255, GetRValue(g_CurrentTheme.editorBg), GetGValue(g_CurrentTheme.editorBg), GetBValue(g_CurrentTheme.editorBg)));
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
    UpdateScrollRange();
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::OnLButtonDown(int x, int y, UINT flags)
{
    SetFocus(m_hWnd);
    Point canvasPt = ScreenToCanvas(x, y);
    
    OutputDebugStringW((L"[VisualDesigner] OnLButtonDown: toolControlType=" + m_toolControlType + L"\n").c_str());
    
    // 如果是创建模式
    if (!m_toolControlType.empty()) {
        OutputDebugStringW(L"[VisualDesigner] 进入创建模式\n");
        m_dragMode = DragMode::CreateNew;
        m_dragStartPoint = canvasPt;
        m_dragLastPoint = canvasPt;
        SetCapture(m_hWnd);
        return;
    }
    
    // 检查是否点击了窗口调整大小手柄（只有右边中点、下边中点、右下角的手柄）
    if (m_selection.isFormSelected) {
        // 手柄大小（与绘制时一致）
        const int handleSize = 8;
        const int halfHandle = handleSize / 2;
        const int tolerance = (int)(halfHandle / m_zoom) + 2;
        
        int right = m_formInfo.width;
        int bottom = m_formInfo.height;
        int centerX = right / 2;
        int centerY = bottom / 2;
        
        DragMode resizeMode = DragMode::None;
        
        // 右下角手柄
        if (abs(canvasPt.X - right) <= tolerance && abs(canvasPt.Y - bottom) <= tolerance) {
            resizeMode = DragMode::ResizeBottomRight;
        }
        // 右边中点手柄
        else if (abs(canvasPt.X - right) <= tolerance && abs(canvasPt.Y - centerY) <= tolerance) {
            resizeMode = DragMode::ResizeRight;
        }
        // 下边中点手柄
        else if (abs(canvasPt.X - centerX) <= tolerance && abs(canvasPt.Y - bottom) <= tolerance) {
            resizeMode = DragMode::ResizeBottom;
        }
        
        if (resizeMode != DragMode::None) {
            m_dragMode = resizeMode;
            m_dragStartPoint = canvasPt;
            m_dragLastPoint = canvasPt;
            // 保存原始窗口大小
            m_dragOriginalBounds = Rect(0, 0, m_formInfo.width, m_formInfo.height);
            SetCapture(m_hWnd);
            return;
        }
    }
    
    // 检查是否点击了控件调整大小手柄
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
        // 检查是否点击在窗口区域内（包括标题栏）
        const int titleBarHeight = 30;
        if (canvasPt.X >= 0 && canvasPt.X < m_formInfo.width &&
            canvasPt.Y >= -titleBarHeight && canvasPt.Y < m_formInfo.height) {
            // 选中窗口
            SelectForm();
        } else {
            if (!(flags & MK_CONTROL)) {
                ClearSelection();
            }
        }
    }
}

void VisualDesigner::OnLButtonDblClk(int x, int y, UINT flags)
{
    // 仅在选择模式（无工具控件）且设置了回调时处理
    if (!m_toolControlType.empty() || !m_dblClickCallback) return;
    
    Point canvasPt = ScreenToCanvas(x, y);
    
    // 命中测试：优先检查是否双击了某个控件
    auto hitControl = HitTest(canvasPt.X, canvasPt.Y);
    if (hitControl) {
        m_dblClickCallback(hitControl->name, hitControl->type);
        return;
    }
    
    // 检查是否双击了窗口本身（包括标题栏区域）
    const int titleBarHeight = 30;
    if (canvasPt.X >= 0 && canvasPt.X < m_formInfo.width &&
        canvasPt.Y >= -titleBarHeight && canvasPt.Y < m_formInfo.height) {
        m_dblClickCallback(L"", L"");
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
        
        // 限制控件不能超出窗口客户区
        if (left < 0) left = 0;
        if (top < 0) top = 0;
        if (right > m_formInfo.width) right = m_formInfo.width;
        if (bottom > m_formInfo.height) bottom = m_formInfo.height;
        
        // 确保控件有最小尺寸
        if (right - left < 20) right = left + 20;
        if (bottom - top < 20) bottom = top + 20;
        
        // 再次检查是否超出边界（调整最小尺寸后可能超出）
        if (right > m_formInfo.width) {
            left = m_formInfo.width - (right - left);
            right = m_formInfo.width;
            if (left < 0) left = 0;
        }
        if (bottom > m_formInfo.height) {
            top = m_formInfo.height - (bottom - top);
            bottom = m_formInfo.height;
            if (top < 0) top = 0;
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
        
        // 没有实际移动则跳过，避免仅选中就标记为已修改
        if (dx == 0 && dy == 0) return;
        
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
                
                // 限制控件不能移出窗口客户区
                if (newBounds.X < 0) newBounds.X = 0;
                if (newBounds.Y < 0) newBounds.Y = 0;
                if (newBounds.X + newBounds.Width > m_formInfo.width) {
                    newBounds.X = m_formInfo.width - newBounds.Width;
                }
                if (newBounds.Y + newBounds.Height > m_formInfo.height) {
                    newBounds.Y = m_formInfo.height - newBounds.Height;
                }
                
                ctrl->bounds = newBounds;
            }
            index++;
        }
        
        UpdateSelectionBounds();
        SetModified(true);
        
        // 实时更新属性窗口
        if (m_selectionChangedCallback) {
            m_selectionChangedCallback();
        }
        
        InvalidateRect(m_hWnd, NULL, FALSE);
    } else if (m_dragMode != DragMode::None) {
        // 调整大小
        int dx = canvasPt.X - m_dragStartPoint.X;
        int dy = canvasPt.Y - m_dragStartPoint.Y;
        
        // 检查是否是调整窗口大小
        if (m_selection.isFormSelected) {
            // 调整窗口大小 - 只允许从右边和下边调整
            int newWidth = m_dragOriginalBounds.Width;
            int newHeight = m_dragOriginalBounds.Height;
            
            // 右边调整
            if (m_dragMode == DragMode::ResizeRight || m_dragMode == DragMode::ResizeBottomRight) {
                newWidth = max(100, m_dragOriginalBounds.Width + dx);
            }
            // 下边调整
            if (m_dragMode == DragMode::ResizeBottom || m_dragMode == DragMode::ResizeBottomRight) {
                newHeight = max(100, m_dragOriginalBounds.Height + dy);
            }
            
            if (m_gridConfig.snapToGrid) {
                newWidth = SnapToGrid(newWidth);
                newHeight = SnapToGrid(newHeight);
            }
            
            m_formInfo.width = newWidth;
            m_formInfo.height = newHeight;
            m_selection.selectionBounds = Rect(0, 0, m_formInfo.width, m_formInfo.height);
            
            // 更新滚动范围
            UpdateScrollRange();
            
            SetModified(true);
            InvalidateRect(m_hWnd, NULL, FALSE);
            return;
        }
        
        // 计算新边界（控件）
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
        
        // 限制控件不能超出窗口客户区
        if (newBounds.X < 0) newBounds.X = 0;
        if (newBounds.Y < 0) newBounds.Y = 0;
        if (newBounds.X + newBounds.Width > m_formInfo.width) {
            newBounds.Width = m_formInfo.width - newBounds.X;
        }
        if (newBounds.Y + newBounds.Height > m_formInfo.height) {
            newBounds.Height = m_formInfo.height - newBounds.Y;
        }
        
        // 确保控件有最小尺寸
        if (newBounds.Width < 20) newBounds.Width = 20;
        if (newBounds.Height < 20) newBounds.Height = 20;
        
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
                
                // 限制每个控件不能超出窗口客户区
                if (ctrl->bounds.X < 0) ctrl->bounds.X = 0;
                if (ctrl->bounds.Y < 0) ctrl->bounds.Y = 0;
                if (ctrl->bounds.X + ctrl->bounds.Width > m_formInfo.width) {
                    ctrl->bounds.Width = m_formInfo.width - ctrl->bounds.X;
                }
                if (ctrl->bounds.Y + ctrl->bounds.Height > m_formInfo.height) {
                    ctrl->bounds.Height = m_formInfo.height - ctrl->bounds.Y;
                }
            }
            index++;
        }
        
        UpdateSelectionBounds();
        SetModified(true);
        
        // 实时更新属性窗口
        if (m_selectionChangedCallback) {
            m_selectionChangedCallback();
        }
        
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

void VisualDesigner::OnRButtonDown(int x, int y)
{
    Point canvasPt = ScreenToCanvas(x, y);
    
    // 首先检查是否点击了某个控件
    auto hitControl = HitTest(canvasPt.X, canvasPt.Y);
    if (hitControl) {
        // 如果点击的控件未被选中，选中它
        if (std::find(m_selection.selectedControlIds.begin(),
                     m_selection.selectedControlIds.end(),
                     hitControl->id) == m_selection.selectedControlIds.end()) {
            SelectControl(hitControl->id, false);
        }
    }
    
    // 创建上下文菜单
    HMENU hMenu = CreatePopupMenu();
    
    if (m_selection.selectedControlIds.empty()) {
        // 没有选中控件时的菜单
        AppendMenuW(hMenu, MF_STRING, 101, L"粘贴(&V)\tCtrl+V");
        AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
        AppendMenuW(hMenu, MF_STRING, 102, L"全选(&A)\tCtrl+A");
        AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
        AppendMenuW(hMenu, m_gridConfig.showGrid ? MF_STRING | MF_CHECKED : MF_STRING, 103, L"显示网格");
        AppendMenuW(hMenu, m_gridConfig.snapToGrid ? MF_STRING | MF_CHECKED : MF_STRING, 104, L"对齐到网格");
    } else {
        // 有选中控件时的菜单
        AppendMenuW(hMenu, MF_STRING, 105, L"剪切(&T)\tCtrl+X");
        AppendMenuW(hMenu, MF_STRING, 106, L"复制(&C)\tCtrl+C");
        AppendMenuW(hMenu, MF_STRING, 107, L"删除(&D)\tDelete");
        AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
        
        // 对齐子菜单
        HMENU hAlignMenu = CreatePopupMenu();
        AppendMenuW(hAlignMenu, MF_STRING, 111, L"左对齐");
        AppendMenuW(hAlignMenu, MF_STRING, 112, L"右对齐");
        AppendMenuW(hAlignMenu, MF_STRING, 113, L"顶端对齐");
        AppendMenuW(hAlignMenu, MF_STRING, 114, L"底端对齐");
        AppendMenuW(hAlignMenu, MF_SEPARATOR, 0, NULL);
        AppendMenuW(hAlignMenu, MF_STRING, 115, L"水平居中");
        AppendMenuW(hAlignMenu, MF_STRING, 116, L"垂直居中");
        AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hAlignMenu, L"对齐(&A)");
        
        // 大小子菜单
        HMENU hSizeMenu = CreatePopupMenu();
        AppendMenuW(hSizeMenu, MF_STRING, 121, L"相同宽度");
        AppendMenuW(hSizeMenu, MF_STRING, 122, L"相同高度");
        AppendMenuW(hSizeMenu, MF_STRING, 123, L"相同大小");
        AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hSizeMenu, L"调整大小(&S)");
        
        // 层级子菜单
        HMENU hOrderMenu = CreatePopupMenu();
        AppendMenuW(hOrderMenu, MF_STRING, 131, L"置于顶层");
        AppendMenuW(hOrderMenu, MF_STRING, 132, L"置于底层");
        AppendMenuW(hOrderMenu, MF_STRING, 133, L"上移一层");
        AppendMenuW(hOrderMenu, MF_STRING, 134, L"下移一层");
        AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hOrderMenu, L"层级(&O)");
        
        AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
        AppendMenuW(hMenu, m_gridConfig.showGrid ? MF_STRING | MF_CHECKED : MF_STRING, 103, L"显示网格");
        AppendMenuW(hMenu, m_gridConfig.snapToGrid ? MF_STRING | MF_CHECKED : MF_STRING, 104, L"对齐到网格");
    }
    
    // 显示菜单
    POINT pt = { x, y };
    ClientToScreen(m_hWnd, &pt);
    int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_LEFTALIGN | TPM_TOPALIGN,
                            pt.x, pt.y, 0, m_hWnd, NULL);
    DestroyMenu(hMenu);
    
    // 处理菜单命令
    switch (cmd) {
        case 101: Paste(); break;
        case 102: SelectAll(); break;
        case 103: m_gridConfig.showGrid = !m_gridConfig.showGrid; InvalidateRect(m_hWnd, NULL, FALSE); break;
        case 104: m_gridConfig.snapToGrid = !m_gridConfig.snapToGrid; break;
        case 105: Cut(); break;
        case 106: Copy(); break;
        case 107: DeleteSelectedControls(); break;
        case 111: AlignLeft(); break;
        case 112: AlignRight(); break;
        case 113: AlignTop(); break;
        case 114: AlignBottom(); break;
        case 115: AlignCenterHorizontal(); break;
        case 116: AlignCenterVertical(); break;
        case 121: MakeSameWidth(); break;
        case 122: MakeSameHeight(); break;
        case 123: MakeSameSize(); break;
        case 131: BringToFront(); break;
        case 132: SendToBack(); break;
        case 133: BringForward(); break;
        case 134: SendBackward(); break;
    }
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
            case VK_DELETE:
            case VK_BACK: DeleteSelectedControls(); break;
            case VK_ESCAPE: SetSelectMode(); ClearSelection(); break;
        }
    }
}

void VisualDesigner::OnMouseWheel(int delta)
{
    // Ctrl+滚轮进行缩放
    if (GetKeyState(VK_CONTROL) & 0x8000) {
        float zoomStep = 0.1f;
        if (delta > 0) {
            SetZoom(m_zoom + zoomStep);
        } else {
            SetZoom(m_zoom - zoomStep);
        }
    } else {
        // 普通滚动 - 垂直滚动
        int scrollStep = 30;
        SCROLLINFO si = { sizeof(SCROLLINFO), SIF_POS | SIF_RANGE | SIF_PAGE };
        GetScrollInfo(m_hWnd, SB_VERT, &si);
        
        int maxScroll = si.nMax - (int)si.nPage;
        if (maxScroll > 0) {
            if (delta > 0) {
                m_scrollY = min(0, m_scrollY + scrollStep);
            } else {
                m_scrollY = max(-maxScroll, m_scrollY - scrollStep);
            }
            SetScrollPos(m_hWnd, SB_VERT, -m_scrollY, TRUE);
            InvalidateRect(m_hWnd, NULL, FALSE);
        }
    }
}

void VisualDesigner::OnHScroll(UINT nSBCode, UINT nPos)
{
    SCROLLINFO si = { sizeof(SCROLLINFO), SIF_POS | SIF_RANGE | SIF_PAGE | SIF_TRACKPOS };
    GetScrollInfo(m_hWnd, SB_HORZ, &si);
    
    int maxScroll = si.nMax - (int)si.nPage;
    int newPos = -m_scrollX;
    
    switch (nSBCode) {
        case SB_LINELEFT:
            newPos = max(0, newPos - 20);
            break;
        case SB_LINERIGHT:
            newPos = min(maxScroll, newPos + 20);
            break;
        case SB_PAGELEFT:
            newPos = max(0, newPos - (int)si.nPage);
            break;
        case SB_PAGERIGHT:
            newPos = min(maxScroll, newPos + (int)si.nPage);
            break;
        case SB_THUMBTRACK:
        case SB_THUMBPOSITION:
            newPos = si.nTrackPos;
            break;
    }
    
    m_scrollX = -newPos;
    SetScrollPos(m_hWnd, SB_HORZ, newPos, TRUE);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::OnVScroll(UINT nSBCode, UINT nPos)
{
    SCROLLINFO si = { sizeof(SCROLLINFO), SIF_POS | SIF_RANGE | SIF_PAGE | SIF_TRACKPOS };
    GetScrollInfo(m_hWnd, SB_VERT, &si);
    
    int maxScroll = si.nMax - (int)si.nPage;
    int newPos = -m_scrollY;
    
    switch (nSBCode) {
        case SB_LINEUP:
            newPos = max(0, newPos - 20);
            break;
        case SB_LINEDOWN:
            newPos = min(maxScroll, newPos + 20);
            break;
        case SB_PAGEUP:
            newPos = max(0, newPos - (int)si.nPage);
            break;
        case SB_PAGEDOWN:
            newPos = min(maxScroll, newPos + (int)si.nPage);
            break;
        case SB_THUMBTRACK:
        case SB_THUMBPOSITION:
            newPos = si.nTrackPos;
            break;
    }
    
    m_scrollY = -newPos;
    SetScrollPos(m_hWnd, SB_VERT, newPos, TRUE);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

// === 文件操作 ===

bool VisualDesigner::LoadFile(const std::wstring& path)
{
    // 直接使用宽字符路径打开文件，避免中文路径编码问题
    std::ifstream file(path.c_str());
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
    wchar_t debugMsg[512];
    swprintf_s(debugMsg, L"[VisualDesigner::SaveFile] Saving to: %s\n", path.c_str());
    OutputDebugStringW(debugMsg);
    
    // 直接使用宽字符路径打开文件，避免中文路径编码问题
    std::ofstream file(path.c_str());
    if (!file.is_open()) {
        OutputDebugStringW(L"[VisualDesigner::SaveFile] Failed to open file!\n");
        return false;
    }
    
    try {
        json j = ToJson();
        std::string content = j.dump(2);
        swprintf_s(debugMsg, L"[VisualDesigner::SaveFile] Content size: %zu bytes\n", content.size());
        OutputDebugStringW(debugMsg);
        
        file << content;
        file.close();
        
        m_filePath = path;
        size_t pos = path.find_last_of(L"\\/");
        m_fileName = (pos != std::wstring::npos) ? path.substr(pos + 1) : path;
        m_modified = false;
        
        OutputDebugStringW(L"[VisualDesigner::SaveFile] File saved successfully!\n");
        InvalidateRect(m_hWnd, NULL, FALSE);
        return true;
    } catch (const std::exception& e) {
        char errMsg[256];
        sprintf_s(errMsg, "[VisualDesigner::SaveFile] Exception: %s\n", e.what());
        OutputDebugStringA(errMsg);
        return false;
    } catch (...) {
        OutputDebugStringW(L"[VisualDesigner::SaveFile] Unknown exception!\n");
        return false;
    }
}

void VisualDesigner::SetModified(bool modified) {
    bool wasModified = m_modified;
    m_modified = modified;
    
    // 只有从未修改变为已修改时才通知
    if (modified && !wasModified) {
        // 向主窗口发送修改通知消息
        HWND hMainWnd = GetAncestor(m_hWnd, GA_ROOT);
        if (hMainWnd) {
            PostMessage(hMainWnd, WM_COMMAND, MAKEWPARAM(0, 0x1000), (LPARAM)m_hWnd);
        }
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
    
    // 从支持库获取组件默认属性
    const WindowUnitInfo* unitInfo = LibraryParser::GetInstance().FindWindowUnit(type);
    if (unitInfo) {
        OutputDebugStringW((L"[VisualDesigner] 从支持库加载控件属性: " + type + 
            L", 属性数: " + std::to_wstring(unitInfo->properties.size()) + L"\n").c_str());
        
        // 设置默认属性
        for (const auto& prop : unitInfo->properties) {
            // 设置属性默认值
            std::wstring defaultValue;
            switch (prop.type) {
                case PropertyType::Bool:
                    // 可视属性默认为真
                    if (prop.name == L"可视" || prop.englishName == L"visible") {
                        defaultValue = L"真";
                    } else {
                        defaultValue = L"假";
                    }
                    break;
                case PropertyType::Int:
                case PropertyType::PickInt:
                case PropertyType::PickSpecInt:
                    defaultValue = L"0";
                    break;
                case PropertyType::Text:
                case PropertyType::PickText:
                case PropertyType::EditPickText:
                    // 对于标题属性，使用控件名称
                    if (prop.name == L"标题" || prop.englishName == L"caption") {
                        defaultValue = control->name;
                    } else {
                        defaultValue = L"";
                    }
                    break;
                case PropertyType::Color:
                case PropertyType::ColorTrans:
                case PropertyType::ColorBack:
                    defaultValue = L"16777215"; // 白色
                    break;
                default:
                    defaultValue = L"";
                    break;
            }
            
            // 如果有选择选项，使用第一个选项作为默认值
            if (!prop.pickOptions.empty() && 
                (prop.type == PropertyType::PickInt || 
                 prop.type == PropertyType::PickText ||
                 prop.type == PropertyType::PickSpecInt)) {
                defaultValue = L"0"; // 选择第一项（索引0）
            }
            
            control->properties[prop.name] = defaultValue;
        }
    }
    else {
        // 后备：使用基本默认属性
        control->properties[L"标题"] = control->name;
        control->properties[L"可视"] = L"真";
        control->properties[L"禁止"] = L"假";
    }
    
    // 确保基本属性存在
    if (control->properties.find(L"左边") == control->properties.end()) {
        control->properties[L"左边"] = std::to_wstring(bounds.X);
    }
    if (control->properties.find(L"顶边") == control->properties.end()) {
        control->properties[L"顶边"] = std::to_wstring(bounds.Y);
    }
    if (control->properties.find(L"宽度") == control->properties.end()) {
        control->properties[L"宽度"] = std::to_wstring(bounds.Width);
    }
    if (control->properties.find(L"高度") == control->properties.end()) {
        control->properties[L"高度"] = std::to_wstring(bounds.Height);
    }
    
    m_formInfo.controls.push_back(control);
    
    SelectControl(control->id);
    CreateSnapshot(L"Add Control");
    
    SetModified(true);
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
        SetModified(true);
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
    // 选择控件时取消窗口选中
    m_selection.isFormSelected = false;
    
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
    
    // 触发选择变更回调
    if (m_selectionChangedCallback) {
        m_selectionChangedCallback();
    }
    
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::SelectForm()
{
    // 清除控件选择，选中窗口
    m_selection.selectedControlIds.clear();
    m_selection.isMultiSelect = false;
    m_selection.isFormSelected = true;
    m_selection.selectionBounds = Rect(0, 0, m_formInfo.width, m_formInfo.height);
    
    // 触发选择变更回调
    if (m_selectionChangedCallback) {
        m_selectionChangedCallback();
    }
    
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::ClearSelection()
{
    m_selection.selectedControlIds.clear();
    m_selection.isMultiSelect = false;
    m_selection.isFormSelected = false;
    
    // 触发选择变更回调
    if (m_selectionChangedCallback) {
        m_selectionChangedCallback();
    }
    
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
    
    // 标题栏高度
    const int titleBarHeight = 30;
    const int borderWidth = 1;
    const int margin = 20;  // 边距
    
    // 计算画布位置（左上角基点 + 滚动偏移）
    int totalHeight = m_formInfo.height + titleBarHeight;
    int canvasScreenX = margin + m_scrollX;
    int canvasScreenY = margin + m_scrollY;
    
    int scaledWidth = (int)(m_formInfo.width * m_zoom);
    int scaledTotalHeight = (int)(totalHeight * m_zoom);
    int scaledTitleHeight = (int)(titleBarHeight * m_zoom);
    int scaledClientHeight = (int)(m_formInfo.height * m_zoom);
    
    // 整个窗口区域（包含标题栏）
    Rect windowRect(canvasScreenX, canvasScreenY, scaledWidth, scaledTotalHeight);
    
    // 绘制窗口阴影（Windows 11 风格 - 更柔和的阴影）
    for (int i = 8; i > 0; i--) {
        int alpha = 15 - i;
        SolidBrush shadowBrush(Color(alpha, 0, 0, 0));
        g.FillRectangle(&shadowBrush, windowRect.X + i, windowRect.Y + i, 
                        windowRect.Width, windowRect.Height);
    }
    
    // Windows 11 风格圆角（使用普通矩形，因为 GDI+ 圆角效果有限）
    // 绘制窗口背景
    SolidBrush windowBgBrush(Color(243, 243, 243)); // Windows 11 窗口背景色
    g.FillRectangle(&windowBgBrush, windowRect);
    
    // 绘制窗口边框
    if (m_formInfo.borderStyle > 0) {
        Pen borderPen(Color(200, 200, 200), 1.0f);
        g.DrawRectangle(&borderPen, windowRect);
    }
    
    // 绘制标题栏（Windows 11 风格 - 浅色标题栏）
    Rect titleBarRect(canvasScreenX + borderWidth, canvasScreenY + borderWidth, 
                      scaledWidth - borderWidth * 2, scaledTitleHeight - borderWidth);
    SolidBrush titleBarBrush(Color(243, 243, 243));  // Windows 11 浅色标题栏
    g.FillRectangle(&titleBarBrush, titleBarRect);
    
    // 绘制窗口图标区域（小图标占位）
    int iconSize = (int)(16 * m_zoom);
    int iconX = titleBarRect.X + (int)(10 * m_zoom);
    int iconY = titleBarRect.Y + (titleBarRect.Height - iconSize) / 2;
    // 绘制一个简单的应用图标占位
    SolidBrush iconBrush(Color(0, 120, 215));
    g.FillRectangle(&iconBrush, iconX, iconY, iconSize, iconSize);
    
    // 绘制标题文字
    Font titleFont(L"Segoe UI", 9.0f * m_zoom, FontStyleRegular, UnitPoint);
    SolidBrush titleTextBrush(Color(0, 0, 0));  // 黑色文字
    StringFormat titleFormat;
    titleFormat.SetAlignment(StringAlignmentNear);
    titleFormat.SetLineAlignment(StringAlignmentCenter);
    RectF titleTextRect((float)iconX + iconSize + 8 * m_zoom, (float)titleBarRect.Y,
                        (float)titleBarRect.Width - 150 * m_zoom, (float)titleBarRect.Height);
    g.DrawString(m_formInfo.title.c_str(), -1, &titleFont, titleTextRect, &titleFormat, &titleTextBrush);
    
    // 绘制控制按钮（Windows 11 风格）
    if (m_formInfo.hasControlBox) {
        int btnWidth = (int)(46 * m_zoom);
        int btnHeight = scaledTitleHeight - borderWidth;
        int btnX = titleBarRect.X + titleBarRect.Width - btnWidth;
        int btnY = titleBarRect.Y;
        
        // 关闭按钮（鼠标悬停时红色，默认透明）
        Rect closeRect(btnX, btnY, btnWidth, btnHeight);
        // 绘制 X（Windows 11 风格细线）
        Pen closePen(Color(100, 100, 100), 1.0f * m_zoom);
        int cx = closeRect.X + closeRect.Width / 2;
        int cy = closeRect.Y + closeRect.Height / 2;
        int cs = (int)(5 * m_zoom);
        g.DrawLine(&closePen, cx - cs, cy - cs, cx + cs, cy + cs);
        g.DrawLine(&closePen, cx + cs, cy - cs, cx - cs, cy + cs);
        
        // 最大化按钮
        if (m_formInfo.hasMaxButton) {
            btnX -= btnWidth;
            Rect maxRect(btnX, btnY, btnWidth, btnHeight);
            // 绘制方块（Windows 11 风格）
            Pen maxPen(Color(100, 100, 100), 1.0f * m_zoom);
            int mx = maxRect.X + maxRect.Width / 2;
            int my = maxRect.Y + maxRect.Height / 2;
            int ms = (int)(5 * m_zoom);
            g.DrawRectangle(&maxPen, mx - ms, my - ms, ms * 2, ms * 2);
        }
        
        // 最小化按钮
        if (m_formInfo.hasMinButton) {
            btnX -= btnWidth;
            Rect minRect(btnX, btnY, btnWidth, btnHeight);
            // 绘制横线（Windows 11 风格）
            Pen minPen(Color(100, 100, 100), 1.0f * m_zoom);
            int mx = minRect.X + minRect.Width / 2;
            int my = minRect.Y + minRect.Height / 2;
            int ms = (int)(5 * m_zoom);
            g.DrawLine(&minPen, mx - ms, my, mx + ms, my);
        }
    }
    
    // 绘制客户区（窗口内容区域）
    Rect clientArea(canvasScreenX + borderWidth, 
                    canvasScreenY + scaledTitleHeight,
                    scaledWidth - borderWidth * 2, 
                    scaledClientHeight - borderWidth);
    
    // 背景色
    SolidBrush clientBrush(Color(GetRValue(m_formInfo.backColor), 
                                 GetGValue(m_formInfo.backColor), 
                                 GetBValue(m_formInfo.backColor)));
    g.FillRectangle(&clientBrush, clientArea);
    
    // 保存客户区信息用于后续绘制
    m_clientAreaRect = clientArea;
    
    // 绘制网格
    if (m_gridConfig.showGrid) {
        DrawGrid(g, clientArea);
    }
    
    // 绘制辅助线
    if (m_showGuideLines) {
        DrawGuideLines(g, clientArea);
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
    // 按zOrder排序后绘制控件
    for (const auto& ctrl : m_formInfo.controls) {
        DrawControl(g, *ctrl);
    }
}

void VisualDesigner::DrawControl(Graphics& g, const ControlInfo& control)
{
    Rect screenBounds = CanvasToScreen(control.bounds);
    
    // 创建一个临时的控件信息用于渲染（屏幕坐标）
    ControlInfo screenCtrl = control;
    screenCtrl.bounds = screenBounds;
    
    // 使用ControlRenderer进行专业渲染
    if (m_pControlRenderer) {
        bool isSelected = std::find(m_selection.selectedControlIds.begin(),
                                   m_selection.selectedControlIds.end(),
                                   control.id) != m_selection.selectedControlIds.end();
        m_pControlRenderer->RenderControl(g, screenCtrl, isSelected);
    } else {
        // 备用的简单渲染（当ControlRenderer不可用时）
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
}

void VisualDesigner::DrawSelection(Graphics& g)
{
    // 绘制窗口选择框
    if (m_selection.isFormSelected) {
        // 窗口选中时，选择框应包围整个窗口（包括标题栏）
        const int titleBarHeight = 30;
        const int margin = 20;
        int totalHeight = m_formInfo.height + titleBarHeight;
        
        // 计算整个窗口的屏幕坐标（左上角基点）
        int canvasScreenX = margin + m_scrollX;
        int canvasScreenY = margin + m_scrollY;
        
        int scaledWidth = (int)(m_formInfo.width * m_zoom);
        int scaledTotalHeight = (int)(totalHeight * m_zoom);
        
        Rect screenBounds(canvasScreenX, canvasScreenY, scaledWidth, scaledTotalHeight);
        
        // 绘制选择框
        Pen selectionPen(Color(0, 120, 215), 2.0f);
        g.DrawRectangle(&selectionPen, screenBounds);
        
        // 绘制调整大小手柄（只绘制右边、下边和右下角）
        DrawFormSelectionHandles(g, screenBounds);
        return;
    }
    
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

// 窗口选择手柄（显示所有8个，但只有右边、下边、右下角可调整）
void VisualDesigner::DrawFormSelectionHandles(Graphics& g, const Rect& bounds)
{
    const int handleSize = 6;
    SolidBrush activeBrush(Color(255, 255, 255));   // 可调整的手柄 - 白色
    SolidBrush inactiveBrush(Color(200, 200, 200)); // 不可调整的手柄 - 灰色
    Pen handlePen(Color(0, 120, 215), 1.0f);
    
    // 所有8个手柄位置
    struct HandleInfo {
        Point position;
        bool canResize;  // 是否可调整
    };
    
    HandleInfo handles[8] = {
        { Point(bounds.X, bounds.Y), false },                                    // 左上 - 不可调整
        { Point(bounds.X + bounds.Width / 2, bounds.Y), false },                 // 上中 - 不可调整
        { Point(bounds.X + bounds.Width, bounds.Y), false },                     // 右上 - 不可调整
        { Point(bounds.X + bounds.Width, bounds.Y + bounds.Height / 2), true },  // 右中 - 可调整
        { Point(bounds.X + bounds.Width, bounds.Y + bounds.Height), true },      // 右下 - 可调整
        { Point(bounds.X + bounds.Width / 2, bounds.Y + bounds.Height), true },  // 下中 - 可调整
        { Point(bounds.X, bounds.Y + bounds.Height), false },                    // 左下 - 不可调整
        { Point(bounds.X, bounds.Y + bounds.Height / 2), false }                 // 左中 - 不可调整
    };
    
    for (const auto& handle : handles) {
        Rect handleRect(handle.position.X - handleSize / 2, handle.position.Y - handleSize / 2, handleSize, handleSize);
        g.FillRectangle(handle.canResize ? &activeBrush : &inactiveBrush, handleRect);
        g.DrawRectangle(&handlePen, handleRect);
    }
}

// 更新滚动范围
void VisualDesigner::UpdateScrollRange()
{
    RECT clientRect;
    GetClientRect(m_hWnd, &clientRect);
    
    const int titleBarHeight = 30;
    const int margin = 20;
    
    // 计算窗口实际大小（包含标题栏和边距）
    int totalWidth = (int)(m_formInfo.width * m_zoom) + margin * 2;
    int totalHeight = (int)((m_formInfo.height + titleBarHeight) * m_zoom) + margin * 2;
    
    // 计算滚动范围
    int scrollMaxX = max(0, totalWidth - clientRect.right);
    int scrollMaxY = max(0, totalHeight - clientRect.bottom);
    
    // 设置滚动条信息
    SCROLLINFO siH = { sizeof(SCROLLINFO), SIF_RANGE | SIF_PAGE };
    siH.nMin = 0;
    siH.nMax = scrollMaxX > 0 ? totalWidth : 0;
    siH.nPage = clientRect.right;
    SetScrollInfo(m_hWnd, SB_HORZ, &siH, TRUE);
    
    SCROLLINFO siV = { sizeof(SCROLLINFO), SIF_RANGE | SIF_PAGE };
    siV.nMin = 0;
    siV.nMax = scrollMaxY > 0 ? totalHeight : 0;
    siV.nPage = clientRect.bottom;
    SetScrollInfo(m_hWnd, SB_VERT, &siV, TRUE);
    
    // 显示或隐藏滚动条
    ShowScrollBar(m_hWnd, SB_HORZ, scrollMaxX > 0);
    ShowScrollBar(m_hWnd, SB_VERT, scrollMaxY > 0);
    
    // 确保滚动位置在有效范围内
    if (m_scrollX < -scrollMaxX) m_scrollX = -scrollMaxX;
    if (m_scrollX > 0) m_scrollX = 0;
    if (m_scrollY < -scrollMaxY) m_scrollY = -scrollMaxY;
    if (m_scrollY > 0) m_scrollY = 0;
}

// 坐标转换
Point VisualDesigner::ScreenToCanvas(int x, int y) const
{
    const int titleBarHeight = 30;
    const int borderWidth = 1;
    const int margin = 20;
    
    // 左上角基点 + 滚动偏移
    int canvasScreenX = margin + m_scrollX;
    int canvasScreenY = margin + m_scrollY;
    
    // 转换到客户区坐标（标题栏下方）
    int clientAreaScreenY = canvasScreenY + (int)(titleBarHeight * m_zoom);
    
    int canvasX = (int)((x - canvasScreenX - borderWidth) / m_zoom);
    int canvasY = (int)((y - clientAreaScreenY) / m_zoom);
    
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
    const int titleBarHeight = 30;
    const int borderWidth = 1;
    const int margin = 20;
    
    // 左上角基点 + 滚动偏移
    int canvasScreenX = margin + m_scrollX;
    int canvasScreenY = margin + m_scrollY;
    
    // 转换到客户区坐标（标题栏下方）
    int clientAreaScreenY = canvasScreenY + (int)(titleBarHeight * m_zoom);
    
    int screenX = (int)(x * m_zoom) + canvasScreenX + borderWidth;
    int screenY = (int)(y * m_zoom) + clientAreaScreenY;
    
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
    // 容差值需要考虑缩放，以确保在不同缩放级别下都能正确检测
    // 使用较大的容差值使手柄更容易被点击
    const int tolerance = (int)(10 / m_zoom);  // 在屏幕上约10像素的容差
    
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

// 根据鼠标位置获取调整大小光标
HCURSOR VisualDesigner::GetResizeCursor()
{
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(m_hWnd, &pt);
    
    // 获取画布坐标
    Point canvasPt = ScreenToCanvas(pt.x, pt.y);
    
    DragMode mode = DragMode::None;
    
    // 手柄大小（与绘制时一致）
    const int handleSize = 8;
    const int halfHandle = handleSize / 2;
    
    // 检查窗口调整手柄（只检查右边中点、下边中点、右下角）
    if (m_selection.isFormSelected) {
        // 窗口边界
        int right = m_formInfo.width;
        int bottom = m_formInfo.height;
        int centerX = right / 2;
        int centerY = bottom / 2;
        
        const int tolerance = (int)(halfHandle / m_zoom) + 2;
        
        // 右下角手柄
        if (abs(canvasPt.X - right) <= tolerance && abs(canvasPt.Y - bottom) <= tolerance) {
            mode = DragMode::ResizeBottomRight;
        }
        // 右边中点手柄
        else if (abs(canvasPt.X - right) <= tolerance && abs(canvasPt.Y - centerY) <= tolerance) {
            mode = DragMode::ResizeRight;
        }
        // 下边中点手柄
        else if (abs(canvasPt.X - centerX) <= tolerance && abs(canvasPt.Y - bottom) <= tolerance) {
            mode = DragMode::ResizeBottom;
        }
    }
    // 检查控件调整手柄（所有8个方向）
    else if (!m_selection.selectedControlIds.empty()) {
        mode = HitTestResizeHandle(canvasPt.X, canvasPt.Y, m_selection.selectionBounds);
    }
    
    // 根据调整模式返回对应光标
    switch (mode) {
        case DragMode::ResizeTopLeft:
        case DragMode::ResizeBottomRight:
            return LoadCursor(NULL, IDC_SIZENWSE);
        case DragMode::ResizeTopRight:
        case DragMode::ResizeBottomLeft:
            return LoadCursor(NULL, IDC_SIZENESW);
        case DragMode::ResizeTop:
        case DragMode::ResizeBottom:
            return LoadCursor(NULL, IDC_SIZENS);
        case DragMode::ResizeLeft:
        case DragMode::ResizeRight:
            return LoadCursor(NULL, IDC_SIZEWE);
        default:
            return NULL;  // 使用默认光标
    }
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
    OutputDebugStringW((L"[VisualDesigner] SetToolMode: " + controlType + L"\n").c_str());
}

void VisualDesigner::SetSelectMode()
{
    m_toolControlType.clear();
    
    // 通知组件箱清除选择状态
    HWND hMainWnd = GetAncestor(m_hWnd, GA_ROOT);
    if (hMainWnd) {
        // 发送消息通知主窗口切换到选择模式
        PostMessage(hMainWnd, WM_USER + 200, 0, 0);
    }
}

// JSON序列化
json VisualDesigner::ToJson() const
{
    json j;
    j["type"] = "window";
    j["name"] = WStringToUtf8(m_formInfo.name);
    j["title"] = WStringToUtf8(m_formInfo.title);
    j["width"] = m_formInfo.width;
    j["height"] = m_formInfo.height;
    
    // 保存窗体属性
    json formProps = json::object();
    for (const auto& prop : m_formInfo.properties) {
        formProps[WStringToUtf8(prop.first)] = WStringToUtf8(prop.second);
    }
    if (!formProps.empty()) {
        j["properties"] = formProps;
    }
    
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
    m_formInfo.properties.clear();
    
    // 兼容两种格式：旧格式使用 name/title/width/height，新格式使用 formName/formTitle 等
    if (j.contains("formName")) {
        m_formInfo.name = Utf8ToWString(j["formName"]);
    } else if (j.contains("name")) {
        m_formInfo.name = Utf8ToWString(j["name"]);
    }
    
    if (j.contains("formTitle")) {
        m_formInfo.title = Utf8ToWString(j["formTitle"]);
    } else if (j.contains("title")) {
        m_formInfo.title = Utf8ToWString(j["title"]);
    }
    
    if (j.contains("formWidth")) {
        m_formInfo.width = j["formWidth"];
    } else if (j.contains("width")) {
        m_formInfo.width = j["width"];
    }
    
    if (j.contains("formHeight")) {
        m_formInfo.height = j["formHeight"];
    } else if (j.contains("height")) {
        m_formInfo.height = j["height"];
    }
    
    // 加载窗体属性
    if (j.contains("properties") && j["properties"].is_object()) {
        for (auto& [key, value] : j["properties"].items()) {
            if (value.is_string()) {
                m_formInfo.properties[Utf8ToWString(key)] = Utf8ToWString(value.get<std::string>());
            }
        }
    }
    
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
        
        SetModified(true);
        InvalidateRect(m_hWnd, NULL, FALSE);
        
    } catch (...) {
        // 剪贴板内容不是有效的控件数据
        return;
    }
}

// === 对齐操作 ===

void VisualDesigner::AlignLeft()
{
    auto controls = GetSelectedControls();
    if (controls.size() < 2) return;
    
    int minX = INT_MAX;
    for (const auto& ctrl : controls) {
        minX = min(minX, ctrl->bounds.X);
    }
    
    CreateSnapshot(L"左对齐");
    for (auto& ctrl : controls) {
        ctrl->bounds.X = minX;
    }
    
    UpdateSelectionBounds();
    SetModified(true);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::AlignRight()
{
    auto controls = GetSelectedControls();
    if (controls.size() < 2) return;
    
    int maxRight = INT_MIN;
    for (const auto& ctrl : controls) {
        maxRight = max(maxRight, ctrl->bounds.X + ctrl->bounds.Width);
    }
    
    CreateSnapshot(L"右对齐");
    for (auto& ctrl : controls) {
        ctrl->bounds.X = maxRight - ctrl->bounds.Width;
    }
    
    UpdateSelectionBounds();
    SetModified(true);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::AlignTop()
{
    auto controls = GetSelectedControls();
    if (controls.size() < 2) return;
    
    int minY = INT_MAX;
    for (const auto& ctrl : controls) {
        minY = min(minY, ctrl->bounds.Y);
    }
    
    CreateSnapshot(L"顶端对齐");
    for (auto& ctrl : controls) {
        ctrl->bounds.Y = minY;
    }
    
    UpdateSelectionBounds();
    SetModified(true);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::AlignBottom()
{
    auto controls = GetSelectedControls();
    if (controls.size() < 2) return;
    
    int maxBottom = INT_MIN;
    for (const auto& ctrl : controls) {
        maxBottom = max(maxBottom, ctrl->bounds.Y + ctrl->bounds.Height);
    }
    
    CreateSnapshot(L"底端对齐");
    for (auto& ctrl : controls) {
        ctrl->bounds.Y = maxBottom - ctrl->bounds.Height;
    }
    
    UpdateSelectionBounds();
    SetModified(true);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::AlignCenterHorizontal()
{
    auto controls = GetSelectedControls();
    if (controls.size() < 2) return;
    
    int sumCenterX = 0;
    for (const auto& ctrl : controls) {
        sumCenterX += ctrl->bounds.X + ctrl->bounds.Width / 2;
    }
    int avgCenterX = sumCenterX / (int)controls.size();
    
    CreateSnapshot(L"水平居中");
    for (auto& ctrl : controls) {
        ctrl->bounds.X = avgCenterX - ctrl->bounds.Width / 2;
    }
    
    UpdateSelectionBounds();
    SetModified(true);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::AlignCenterVertical()
{
    auto controls = GetSelectedControls();
    if (controls.size() < 2) return;
    
    int sumCenterY = 0;
    for (const auto& ctrl : controls) {
        sumCenterY += ctrl->bounds.Y + ctrl->bounds.Height / 2;
    }
    int avgCenterY = sumCenterY / (int)controls.size();
    
    CreateSnapshot(L"垂直居中");
    for (auto& ctrl : controls) {
        ctrl->bounds.Y = avgCenterY - ctrl->bounds.Height / 2;
    }
    
    UpdateSelectionBounds();
    SetModified(true);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::DistributeHorizontal()
{
    auto controls = GetSelectedControls();
    if (controls.size() < 3) return;
    
    // 按X坐标排序
    std::sort(controls.begin(), controls.end(),
        [](const std::shared_ptr<ControlInfo>& a, const std::shared_ptr<ControlInfo>& b) {
            return a->bounds.X < b->bounds.X;
        });
    
    int minX = controls.front()->bounds.X;
    int maxX = controls.back()->bounds.X + controls.back()->bounds.Width;
    int totalWidth = 0;
    for (const auto& ctrl : controls) {
        totalWidth += ctrl->bounds.Width;
    }
    
    int totalGap = maxX - minX - totalWidth;
    int gapCount = (int)controls.size() - 1;
    int gap = gapCount > 0 ? totalGap / gapCount : 0;
    
    CreateSnapshot(L"水平分布");
    int currentX = minX;
    for (auto& ctrl : controls) {
        ctrl->bounds.X = currentX;
        currentX += ctrl->bounds.Width + gap;
    }
    
    UpdateSelectionBounds();
    SetModified(true);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::DistributeVertical()
{
    auto controls = GetSelectedControls();
    if (controls.size() < 3) return;
    
    // 按Y坐标排序
    std::sort(controls.begin(), controls.end(),
        [](const std::shared_ptr<ControlInfo>& a, const std::shared_ptr<ControlInfo>& b) {
            return a->bounds.Y < b->bounds.Y;
        });
    
    int minY = controls.front()->bounds.Y;
    int maxY = controls.back()->bounds.Y + controls.back()->bounds.Height;
    int totalHeight = 0;
    for (const auto& ctrl : controls) {
        totalHeight += ctrl->bounds.Height;
    }
    
    int totalGap = maxY - minY - totalHeight;
    int gapCount = (int)controls.size() - 1;
    int gap = gapCount > 0 ? totalGap / gapCount : 0;
    
    CreateSnapshot(L"垂直分布");
    int currentY = minY;
    for (auto& ctrl : controls) {
        ctrl->bounds.Y = currentY;
        currentY += ctrl->bounds.Height + gap;
    }
    
    UpdateSelectionBounds();
    SetModified(true);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::MakeSameWidth()
{
    auto controls = GetSelectedControls();
    if (controls.size() < 2) return;
    
    // 使用第一个选中控件的宽度作为基准
    int targetWidth = controls.front()->bounds.Width;
    
    CreateSnapshot(L"相同宽度");
    for (auto& ctrl : controls) {
        ctrl->bounds.Width = targetWidth;
    }
    
    UpdateSelectionBounds();
    SetModified(true);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::MakeSameHeight()
{
    auto controls = GetSelectedControls();
    if (controls.size() < 2) return;
    
    int targetHeight = controls.front()->bounds.Height;
    
    CreateSnapshot(L"相同高度");
    for (auto& ctrl : controls) {
        ctrl->bounds.Height = targetHeight;
    }
    
    UpdateSelectionBounds();
    SetModified(true);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::MakeSameSize()
{
    auto controls = GetSelectedControls();
    if (controls.size() < 2) return;
    
    int targetWidth = controls.front()->bounds.Width;
    int targetHeight = controls.front()->bounds.Height;
    
    CreateSnapshot(L"相同大小");
    for (auto& ctrl : controls) {
        ctrl->bounds.Width = targetWidth;
        ctrl->bounds.Height = targetHeight;
    }
    
    UpdateSelectionBounds();
    SetModified(true);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

// === 层级操作 ===

void VisualDesigner::BringToFront()
{
    auto controls = GetSelectedControls();
    if (controls.empty()) return;
    
    CreateSnapshot(L"置于顶层");
    
    // 获取最大zOrder
    int maxZOrder = 0;
    for (const auto& ctrl : m_formInfo.controls) {
        maxZOrder = max(maxZOrder, ctrl->zOrder);
    }
    
    // 设置选中控件的zOrder为最大值+1
    for (auto& ctrl : controls) {
        ctrl->zOrder = ++maxZOrder;
    }
    
    // 重新排序控件列表（按zOrder）
    std::sort(m_formInfo.controls.begin(), m_formInfo.controls.end(),
        [](const std::shared_ptr<ControlInfo>& a, const std::shared_ptr<ControlInfo>& b) {
            return a->zOrder < b->zOrder;
        });
    
    SetModified(true);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::SendToBack()
{
    auto controls = GetSelectedControls();
    if (controls.empty()) return;
    
    CreateSnapshot(L"置于底层");
    
    // 获取最小zOrder
    int minZOrder = INT_MAX;
    for (const auto& ctrl : m_formInfo.controls) {
        minZOrder = min(minZOrder, ctrl->zOrder);
    }
    
    // 设置选中控件的zOrder为最小值-1
    for (auto& ctrl : controls) {
        ctrl->zOrder = --minZOrder;
    }
    
    // 重新排序控件列表
    std::sort(m_formInfo.controls.begin(), m_formInfo.controls.end(),
        [](const std::shared_ptr<ControlInfo>& a, const std::shared_ptr<ControlInfo>& b) {
            return a->zOrder < b->zOrder;
        });
    
    SetModified(true);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::BringForward()
{
    auto controls = GetSelectedControls();
    if (controls.empty()) return;
    
    CreateSnapshot(L"上移一层");
    
    for (auto& ctrl : controls) {
        // 找到zOrder比当前控件大的最小zOrder控件
        int nextZOrder = INT_MAX;
        std::shared_ptr<ControlInfo> nextCtrl = nullptr;
        
        for (auto& other : m_formInfo.controls) {
            if (other->id != ctrl->id && other->zOrder > ctrl->zOrder && other->zOrder < nextZOrder) {
                nextZOrder = other->zOrder;
                nextCtrl = other;
            }
        }
        
        if (nextCtrl) {
            // 交换zOrder
            std::swap(ctrl->zOrder, nextCtrl->zOrder);
        }
    }
    
    // 重新排序
    std::sort(m_formInfo.controls.begin(), m_formInfo.controls.end(),
        [](const std::shared_ptr<ControlInfo>& a, const std::shared_ptr<ControlInfo>& b) {
            return a->zOrder < b->zOrder;
        });
    
    SetModified(true);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::SendBackward()
{
    auto controls = GetSelectedControls();
    if (controls.empty()) return;
    
    CreateSnapshot(L"下移一层");
    
    for (auto& ctrl : controls) {
        // 找到zOrder比当前控件小的最大zOrder控件
        int prevZOrder = INT_MIN;
        std::shared_ptr<ControlInfo> prevCtrl = nullptr;
        
        for (auto& other : m_formInfo.controls) {
            if (other->id != ctrl->id && other->zOrder < ctrl->zOrder && other->zOrder > prevZOrder) {
                prevZOrder = other->zOrder;
                prevCtrl = other;
            }
        }
        
        if (prevCtrl) {
            // 交换zOrder
            std::swap(ctrl->zOrder, prevCtrl->zOrder);
        }
    }
    
    // 重新排序
    std::sort(m_formInfo.controls.begin(), m_formInfo.controls.end(),
        [](const std::shared_ptr<ControlInfo>& a, const std::shared_ptr<ControlInfo>& b) {
            return a->zOrder < b->zOrder;
        });
    
    SetModified(true);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

// === 网格和辅助线 ===

void VisualDesigner::SetGridConfig(const GridConfig& config)
{
    m_gridConfig = config;
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::AddGuideLine(bool isHorizontal, int position)
{
    GuideLine guide;
    guide.isHorizontal = isHorizontal;
    guide.position = position;
    m_guideLines.push_back(guide);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::RemoveGuideLine(int index)
{
    if (index >= 0 && index < (int)m_guideLines.size()) {
        m_guideLines.erase(m_guideLines.begin() + index);
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

void VisualDesigner::ClearGuideLines()
{
    m_guideLines.clear();
    InvalidateRect(m_hWnd, NULL, FALSE);
}

// === 缩放和滚动 ===

void VisualDesigner::SetZoom(float zoom)
{
    m_zoom = max(0.1f, min(4.0f, zoom));
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::SetScroll(int x, int y)
{
    m_scrollX = x;
    m_scrollY = y;
    InvalidateRect(m_hWnd, NULL, FALSE);
}

// === 属性设置 ===

void VisualDesigner::SetFormProperty(const std::wstring& key, const std::wstring& value)
{
    // 辅助函数：判断值是否为真
    auto isTrueValue = [](const std::wstring& v) {
        return v == L"真" || v == L"true" || v == L"1" || v == L"True" || v == L"TRUE";
    };
    
    if (key == L"名称" || key == L"name") {
        m_formInfo.name = value;
    } else if (key == L"标题" || key == L"title") {
        m_formInfo.title = value;
    } else if (key == L"宽度" || key == L"width") {
        try { m_formInfo.width = std::stoi(value); } catch (...) {}
    } else if (key == L"高度" || key == L"height") {
        try { m_formInfo.height = std::stoi(value); } catch (...) {}
    } else if (key == L"控制按钮" || key == L"controlBox") {
        m_formInfo.hasControlBox = isTrueValue(value);
        m_formInfo.properties[key] = value;
    } else if (key == L"最大化按钮" || key == L"maxButton") {
        m_formInfo.hasMaxButton = isTrueValue(value);
        m_formInfo.properties[key] = value;
    } else if (key == L"最小化按钮" || key == L"minButton") {
        m_formInfo.hasMinButton = isTrueValue(value);
        m_formInfo.properties[key] = value;
    } else if (key == L"边框" || key == L"border") {
        // 边框样式映射
        if (value == L"无边框") {
            m_formInfo.borderStyle = 0;
        } else if (value == L"普通可调边框") {
            m_formInfo.borderStyle = 1;
        } else if (value == L"普通固定边框") {
            m_formInfo.borderStyle = 2;
        }
        m_formInfo.properties[key] = value;
    } else if (key == L"背景颜色" || key == L"backColor") {
        try { m_formInfo.backColor = std::stoul(value); } catch (...) {}
        m_formInfo.properties[key] = value;
    } else {
        m_formInfo.properties[key] = value;
    }
    SetModified(true);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void VisualDesigner::SetControlProperty(const std::wstring& controlId, const std::wstring& key, const std::wstring& value)
{
    auto ctrl = GetControl(controlId);
    if (!ctrl) return;
    
    if (key == L"名称" || key == L"name") {
        ctrl->name = value;
    } else if (key == L"左边" || key == L"left") {
        ctrl->bounds.X = std::stoi(value);
    } else if (key == L"顶边" || key == L"top") {
        ctrl->bounds.Y = std::stoi(value);
    } else if (key == L"宽度" || key == L"width") {
        ctrl->bounds.Width = std::stoi(value);
    } else if (key == L"高度" || key == L"height") {
        ctrl->bounds.Height = std::stoi(value);
    } else {
        ctrl->properties[key] = value;
    }
    
    UpdateSelectionBounds();
    SetModified(true);
    InvalidateRect(m_hWnd, NULL, FALSE);
}

