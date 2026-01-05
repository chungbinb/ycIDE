#pragma once
#include <windows.h>
#include <string>

// 欢迎页窗口类名
constexpr auto szWelcomePageClass = L"WelcomePageClass";

// 注册欢迎页窗口类
ATOM RegisterWelcomePageClass(HINSTANCE hInstance);

// 欢迎页窗口过程
LRESULT CALLBACK WelcomePageWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// 欢迎页按钮ID
#define ID_WELCOME_NEW_PROJECT      3001
#define ID_WELCOME_OPEN_PROJECT     3002
#define ID_WELCOME_OPEN_FILE        3003
#define ID_WELCOME_OPEN_FOLDER      3004
