#include "TabBar.h"
#include "Theme.h"
#include <windowsx.h>
#include <algorithm>

extern AppTheme g_CurrentTheme;

// 添加标签
int TabBarData::AddTab(const std::wstring& filePath, const std::wstring& fileName, int editorType) {
    FileTab tab;
    tab.filePath = filePath;
    tab.fileName = fileName;
    tab.editorType = editorType;
    tab.isModified = false;
    
    tabs.push_back(tab);
    int newIndex = (int)tabs.size() - 1;
    SetActiveTab(newIndex);
    
    InvalidateRect(hWnd, NULL, TRUE);
    return newIndex;
}

// 关闭标签
void TabBarData::CloseTab(int index) {
    if (index < 0 || index >= (int)tabs.size()) return;
    
    // 先通知父窗口标签即将关闭（此时标签还在数组中）
    HWND hParent = GetParent(hWnd);
    if (hParent) {
        SendMessage(hParent, WM_COMMAND, MAKEWPARAM(2001, index), (LPARAM)hWnd);
    }
    
    // 从数组中删除标签
    tabs.erase(tabs.begin() + index);
    
    // 调整活动标签索引
    if (activeTabIndex >= (int)tabs.size()) {
        activeTabIndex = (int)tabs.size() - 1;
    }
    
    InvalidateRect(hWnd, NULL, TRUE);
}

// 切换到指定标签
void TabBarData::SetActiveTab(int index) {
    if (index < 0 || index >= (int)tabs.size()) return;
    if (activeTabIndex == index) return;
    
    activeTabIndex = index;
    InvalidateRect(hWnd, NULL, TRUE);
    
    // 通知父窗口标签已切换
    HWND hParent = GetParent(hWnd);
    if (hParent) {
        SendMessage(hParent, WM_COMMAND, MAKEWPARAM(2000, index), (LPARAM)hWnd);
    }
}

// 根据文件路径查找标签
int TabBarData::FindTab(const std::wstring& filePath) {
    for (int i = 0; i < (int)tabs.size(); i++) {
        if (tabs[i].filePath == filePath) {
            return i;
        }
    }
    return -1;
}

// 设置标签修改状态
void TabBarData::SetTabModified(int index, bool modified) {
    if (index >= 0 && index < (int)tabs.size()) {
        tabs[index].isModified = modified;
        InvalidateRect(hWnd, NULL, TRUE);
    }
}

// 获取标签矩形区域
RECT TabBarData::GetTabRect(int index, int clientWidth) {
    RECT rect = {0};
    if (index < 0 || index >= (int)tabs.size()) return rect;
    
    int x = 5;
    for (int i = 0; i < index; i++) {
        x += tabMaxWidth + 2;
    }
    
    rect.left = x;
    rect.top = 2;
    rect.right = x + tabMaxWidth;
    rect.bottom = tabHeight - 2;
    
    return rect;
}

// 获取关闭按钮矩形区域
RECT TabBarData::GetCloseButtonRect(const RECT& tabRect) {
    RECT btnRect;
    btnRect.right = tabRect.right - 8;
    btnRect.left = btnRect.right - closeButtonSize;
    btnRect.top = tabRect.top + (tabRect.bottom - tabRect.top - closeButtonSize) / 2;
    btnRect.bottom = btnRect.top + closeButtonSize;
    return btnRect;
}

// 注册标签栏窗口类
ATOM RegisterTabBarClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex = {0};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = TabBarWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"TabBarClass";
    wcex.hIconSm = nullptr;
    
    return RegisterClassExW(&wcex);
}

// 标签栏窗口过程
LRESULT CALLBACK TabBarWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    TabBarData* data = (TabBarData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    
    switch (message) {
    case WM_CREATE:
        {
            data = new TabBarData(hWnd);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)data);
        }
        return 0;
        
    case WM_DESTROY:
        if (data) {
            delete data;
            SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
        }
        return 0;
        
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            
            // 使用双缓冲
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);
            
            // 绘制背景
            FillRect(memDC, &clientRect, CreateSolidBrush(g_CurrentTheme.bg));
            
            if (!data || data->tabs.empty()) {
                // 没有标签时显示提示
                SetTextColor(memDC, g_CurrentTheme.text);
                SetBkMode(memDC, TRANSPARENT);
                DrawTextW(memDC, L"没有打开的文件", -1, &clientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            } else {
                // 设置字体
                HFONT hFont = CreateFontW(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                    DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft YaHei");
                HFONT oldFont = (HFONT)SelectObject(memDC, hFont);
                SetBkMode(memDC, TRANSPARENT);
                
                // 绘制每个标签
                for (int i = 0; i < (int)data->tabs.size(); i++) {
                    RECT tabRect = data->GetTabRect(i, clientRect.right);
                    bool isActive = (i == data->activeTabIndex);
                    bool isHover = (i == data->hoverTabIndex);
                    
                    // 标签背景
                    COLORREF bgColor = isActive ? g_CurrentTheme.editorBg : 
                                       isHover ? g_CurrentTheme.margin : g_CurrentTheme.bg;
                    FillRect(memDC, &tabRect, CreateSolidBrush(bgColor));
                    
                    // 标签边框
                    HPEN borderPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.grid);
                    HPEN oldPen = (HPEN)SelectObject(memDC, borderPen);
                    
                    if (isActive) {
                        // 活动标签：上、左、右边框
                        MoveToEx(memDC, tabRect.left, tabRect.bottom, NULL);
                        LineTo(memDC, tabRect.left, tabRect.top);
                        LineTo(memDC, tabRect.right, tabRect.top);
                        LineTo(memDC, tabRect.right, tabRect.bottom);
                    } else {
                        // 非活动标签：右边框
                        MoveToEx(memDC, tabRect.right, tabRect.top, NULL);
                        LineTo(memDC, tabRect.right, tabRect.bottom);
                    }
                    
                    SelectObject(memDC, oldPen);
                    DeleteObject(borderPen);
                    
                    // 文件名文本
                    SetTextColor(memDC, g_CurrentTheme.text);
                    RECT textRect = tabRect;
                    textRect.left += 10;
                    textRect.right -= 25; // 为关闭按钮留空间
                    
                    std::wstring displayText = data->tabs[i].fileName;
                    if (data->tabs[i].isModified) {
                        displayText = L"● " + displayText; // 已修改标记
                    }
                    
                    DrawTextW(memDC, displayText.c_str(), -1, &textRect, 
                             DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
                    
                    // 关闭按钮
                    RECT closeRect = data->GetCloseButtonRect(tabRect);
                    SetTextColor(memDC, RGB(150, 150, 150));
                    DrawTextW(memDC, L"×", -1, &closeRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }
                
                SelectObject(memDC, oldFont);
                DeleteObject(hFont);
            }
            
            // 复制到屏幕
            BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, memDC, 0, 0, SRCCOPY);
            
            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);
            
            EndPaint(hWnd, &ps);
        }
        return 0;
        
    case WM_LBUTTONDOWN:
        {
            if (!data) return 0;
            
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            
            // 检查点击了哪个标签
            for (int i = 0; i < (int)data->tabs.size(); i++) {
                RECT tabRect = data->GetTabRect(i, clientRect.right);
                if (PtInRect(&tabRect, {x, y})) {
                    // 检查是否点击了关闭按钮
                    RECT closeRect = data->GetCloseButtonRect(tabRect);
                    if (PtInRect(&closeRect, {x, y})) {
                        data->CloseTab(i);
                    } else {
                        data->SetActiveTab(i);
                    }
                    return 0;
                }
            }
        }
        return 0;
        
    case WM_MOUSEMOVE:
        {
            if (!data) return 0;
            
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            
            int oldHover = data->hoverTabIndex;
            data->hoverTabIndex = -1;
            
            // 检查鼠标悬停在哪个标签
            for (int i = 0; i < (int)data->tabs.size(); i++) {
                RECT tabRect = data->GetTabRect(i, clientRect.right);
                if (PtInRect(&tabRect, {x, y})) {
                    data->hoverTabIndex = i;
                    break;
                }
            }
            
            if (oldHover != data->hoverTabIndex) {
                InvalidateRect(hWnd, NULL, TRUE);
            }
            
            // 追踪鼠标离开
            TRACKMOUSEEVENT tme = {0};
            tme.cbSize = sizeof(tme);
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = hWnd;
            TrackMouseEvent(&tme);
        }
        return 0;
        
    case WM_MOUSELEAVE:
        if (data) {
            data->hoverTabIndex = -1;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        return 0;
        
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    
    return 0;
}

// 获取文件类型对应的编辑器类型
int GetEditorTypeByExtension(const std::wstring& filePath) {
    size_t dotPos = filePath.find_last_of(L'.');
    if (dotPos == std::wstring::npos) return 0; // 默认YiEditor
    
    std::wstring ext = filePath.substr(dotPos);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::towlower);
    
    if (ext == L".eyc") return 0;  // YiEditor
    if (ext == L".ell") return 1;  // EllEditor
    
    return 0; // 默认YiEditor
}
