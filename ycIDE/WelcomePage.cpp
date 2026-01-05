#include "WelcomePage.h"
#include "Theme.h"
#include <CommCtrl.h>
#include <windowsx.h>

extern AppTheme g_CurrentTheme;

// 欢迎页数据
struct WelcomePageData {
    HWND hWnd;
    int hoverButton;  // -1表示无悬停，否则为按钮索引
    
    WelcomePageData(HWND hwnd) : hWnd(hwnd), hoverButton(-1) {}
};

// 按钮信息
struct WelcomeButton {
    std::wstring text;
    std::wstring description;
    int id;
    RECT rect;
};

// 注册欢迎页窗口类
ATOM RegisterWelcomePageClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex = {0};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WelcomePageWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = szWelcomePageClass;
    wcex.hIconSm = nullptr;
    
    return RegisterClassExW(&wcex);
}

// 欢迎页窗口过程
LRESULT CALLBACK WelcomePageWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    WelcomePageData* data = (WelcomePageData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    
    switch (message) {
    case WM_CREATE:
        {
            data = new WelcomePageData(hWnd);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)data);
            OutputDebugStringW(L"[WelcomePage] WM_CREATE - 欢迎页窗口已创建\n");
        }
        return 0;
        
    case WM_DESTROY:
        if (data) {
            delete data;
            SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
        }
        return 0;
        
    case WM_ERASEBKGND:
        // 阻止擦除背景，避免闪烁（我们在WM_PAINT中使用双缓冲完全绘制）
        return 1;
        
    case WM_PAINT:
        {
            OutputDebugStringW(L"[WelcomePage] WM_PAINT - 开始绘制欢迎页\n");
            
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            
            wchar_t debugMsg[256];
            swprintf_s(debugMsg, L"[WelcomePage] 客户区大小: %d x %d\n", clientRect.right, clientRect.bottom);
            OutputDebugStringW(debugMsg);
            
            // 使用双缓冲
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);
            
            // 绘制背景
            HBRUSH bgBrush = CreateSolidBrush(g_CurrentTheme.editorBg);
            FillRect(memDC, &clientRect, bgBrush);
            DeleteObject(bgBrush);
            
            // 创建字体
            HFONT hTitleFont = CreateFontW(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft YaHei");
            HFONT hDescFont = CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft YaHei");
            HFONT hButtonFont = CreateFontW(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft YaHei");
            
            SetBkMode(memDC, TRANSPARENT);
            
            // 绘制标题
            HFONT oldFont = (HFONT)SelectObject(memDC, hTitleFont);
            SetTextColor(memDC, g_CurrentTheme.text);
            RECT titleRect = {0, 80, clientRect.right, 150};
            DrawTextW(memDC, L"易承语言集成开发环境", -1, &titleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            
            // 绘制副标题
            SelectObject(memDC, hDescFont);
            SetTextColor(memDC, g_CurrentTheme.text);
            RECT descRect = {0, 150, clientRect.right, 180};
            DrawTextW(memDC, L"选择一个操作开始", -1, &descRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            
            // 按钮列表
            WelcomeButton buttons[] = {
                {L"新建项目", L"创建新的易承语言项目", ID_WELCOME_NEW_PROJECT, {0}},
                {L"打开项目", L"打开现有的项目文件", ID_WELCOME_OPEN_PROJECT, {0}},
                {L"打开文件", L"打开单个易承语言文件", ID_WELCOME_OPEN_FILE, {0}},
                {L"打开文件夹", L"浏览文件夹中的文件", ID_WELCOME_OPEN_FOLDER, {0}}
            };
            
            // 计算按钮位置
            int buttonWidth = 400;
            int buttonHeight = 70;
            int buttonSpacing = 20;
            int startY = 220;
            int centerX = clientRect.right / 2;
            
            SelectObject(memDC, hButtonFont);
            
            for (int i = 0; i < 4; i++) {
                buttons[i].rect.left = centerX - buttonWidth / 2;
                buttons[i].rect.top = startY + i * (buttonHeight + buttonSpacing);
                buttons[i].rect.right = buttons[i].rect.left + buttonWidth;
                buttons[i].rect.bottom = buttons[i].rect.top + buttonHeight;
                
                // 按钮背景
                bool isHover = (data && data->hoverButton == i);
                COLORREF bgColor = isHover ? g_CurrentTheme.margin : g_CurrentTheme.bg;
                HBRUSH bgBrush = CreateSolidBrush(bgColor);
                FillRect(memDC, &buttons[i].rect, bgBrush);
                DeleteObject(bgBrush);
                
                // 按钮边框
                HPEN borderPen = CreatePen(PS_SOLID, isHover ? 2 : 1, 
                    isHover ? RGB(0, 122, 204) : g_CurrentTheme.grid);
                HPEN oldPen = (HPEN)SelectObject(memDC, borderPen);
                HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, GetStockObject(NULL_BRUSH));
                Rectangle(memDC, buttons[i].rect.left, buttons[i].rect.top, 
                         buttons[i].rect.right, buttons[i].rect.bottom);
                SelectObject(memDC, oldPen);
                SelectObject(memDC, oldBrush);
                DeleteObject(borderPen);
                
                // 绘制按钮文本（主标题）
                SetTextColor(memDC, g_CurrentTheme.text);
                RECT textRect = buttons[i].rect;
                textRect.bottom = textRect.top + 40;  // 主标题区域高度
                DrawTextW(memDC, buttons[i].text.c_str(), -1, &textRect, 
                         DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                
                // 绘制描述文本
                SelectObject(memDC, hDescFont);
                SetTextColor(memDC, g_CurrentTheme.lineNumText);
                RECT descTextRect = buttons[i].rect;
                descTextRect.top = textRect.bottom;  // 从主标题下方开始
                DrawTextW(memDC, buttons[i].description.c_str(), -1, &descTextRect,
                         DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                SelectObject(memDC, hButtonFont);
            }
            
            // 绘制底部版本信息
            SelectObject(memDC, hDescFont);
            SetTextColor(memDC, g_CurrentTheme.lineNumText);
            RECT versionRect = {0, clientRect.bottom - 50, clientRect.right, clientRect.bottom - 20};
            DrawTextW(memDC, L"ycIDE v1.0 - 易承语言现代化IDE", -1, &versionRect, 
                     DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            
            // 清理字体
            SelectObject(memDC, oldFont);
            DeleteObject(hTitleFont);
            DeleteObject(hDescFont);
            DeleteObject(hButtonFont);
            
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
            POINT pt = {x, y};
            
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            
            // 计算按钮位置（与绘制时相同）
            int buttonWidth = 400;
            int buttonHeight = 70;
            int buttonSpacing = 20;
            int startY = 220;
            int centerX = clientRect.right / 2;
            
            int buttonIds[] = {ID_WELCOME_NEW_PROJECT, ID_WELCOME_OPEN_PROJECT, 
                              ID_WELCOME_OPEN_FILE, ID_WELCOME_OPEN_FOLDER};
            
            for (int i = 0; i < 4; i++) {
                RECT btnRect;
                btnRect.left = centerX - buttonWidth / 2;
                btnRect.top = startY + i * (buttonHeight + buttonSpacing);
                btnRect.right = btnRect.left + buttonWidth;
                btnRect.bottom = btnRect.top + buttonHeight;
                
                if (PtInRect(&btnRect, pt)) {
                    // 向父窗口发送命令
                    HWND hParent = GetParent(hWnd);
                    if (hParent) {
                        SendMessage(hParent, WM_COMMAND, buttonIds[i], 0);
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
            POINT pt = {x, y};
            
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            
            // 计算按钮位置
            int buttonWidth = 400;
            int buttonHeight = 70;
            int buttonSpacing = 20;
            int startY = 220;
            int centerX = clientRect.right / 2;
            
            int oldHover = data->hoverButton;
            data->hoverButton = -1;
            
            for (int i = 0; i < 4; i++) {
                RECT btnRect;
                btnRect.left = centerX - buttonWidth / 2;
                btnRect.top = startY + i * (buttonHeight + buttonSpacing);
                btnRect.right = btnRect.left + buttonWidth;
                btnRect.bottom = btnRect.top + buttonHeight;
                
                if (PtInRect(&btnRect, pt)) {
                    data->hoverButton = i;
                    break;
                }
            }
            
            if (oldHover != data->hoverButton) {
                InvalidateRect(hWnd, NULL, FALSE);  // 使用FALSE避免擦除背景，减少闪烁
            }
        }
        return 0;
        
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    
    return 0;
}
