#include "Theme.h"
#include <string>

// 现代深色主题
AppTheme g_DarkTheme = { 
    RGB(24, 24, 24),      // bg - 主背景
    RGB(204, 204, 204),   // text - 文本颜色（现代深色）
    RGB(45, 45, 45),      // grid - 边框/分隔线
    RGB(37, 37, 38),      // margin - 侧边栏/工具栏
    
    RGB(24, 24, 24),      // titleBarBg - 标题栏背景
    RGB(204, 204, 204),   // menuText - 菜单文本色
    
    RGB(24, 24, 24),      // chatBg - AI聊天窗口背景
    RGB(28, 41, 53),      // chatBubbleBg - AI气泡背景
    RGB(204, 204, 204),   // chatText - AI聊天文本
    
    RGB(24, 24, 24),      // explorerBg - 资源管理器背景
    RGB(204, 204, 204),   // explorerText - 资源管理器文本
    
    RGB(31, 31, 31),      // editorBg - 代码编辑器背景
    RGB(37, 37, 38),      // lineNumBg - 行号背景
    RGB(128, 128, 128),   // lineNumText - 行号文本
    
    RGB(86, 156, 214),    // keywordColor - 关键字颜色（蓝色）
    RGB(45, 55, 60),      // tableHeaderBg - 表格表头背景
    
    nullptr 
};

// Visual Studio 浅色主题
AppTheme g_LightTheme = { 
    RGB(255, 255, 255),   // bg - 白色背景
    RGB(0, 0, 0),         // text - 黑色文本
    RGB(204, 206, 219),   // grid - 边框 #CCCEDB
    RGB(246, 246, 246),   // margin - 工具栏/侧边栏 #F6F6F6
    
    RGB(255, 255, 255),   // titleBarBg - 标题栏背景
    RGB(0, 0, 0),         // menuText - 菜单文本色
    
    RGB(255, 255, 255),   // chatBg - AI聊天窗口背景
    RGB(240, 240, 240),   // chatBubbleBg - AI气泡背景
    RGB(0, 0, 0),         // chatText - AI聊天文本
    
    RGB(255, 255, 255),   // explorerBg - 资源管理器背景
    RGB(0, 0, 0),         // explorerText - 资源管理器文本
    
    RGB(255, 255, 255),   // editorBg - 代码编辑器背景
    RGB(246, 246, 246),   // lineNumBg - 行号背景
    RGB(96, 96, 96),      // lineNumText - 行号文本
    
    RGB(0, 0, 255),       // keywordColor - 关键字颜色（蓝色）
    RGB(230, 240, 235),   // tableHeaderBg - 表格表头背景
    
    nullptr 
};

AppTheme g_CurrentTheme;

extern HWND hMainWnd;

void SetTheme(bool isDark) {
    if (g_CurrentTheme.hBgBrush) DeleteObject(g_CurrentTheme.hBgBrush);
    
    if (isDark) {
        g_CurrentTheme = g_DarkTheme;
    } else {
        g_CurrentTheme = g_LightTheme;
    }
    g_CurrentTheme.hBgBrush = CreateSolidBrush(g_CurrentTheme.bg);
}

// 保存主题配置到models.ini
void SaveThemeConfig() {
    wchar_t configPath[MAX_PATH];
    GetModuleFileNameW(NULL, configPath, MAX_PATH);
    std::wstring path(configPath);
    size_t pos = path.find_last_of(L"\\");
    if (pos != std::wstring::npos) {
        path = path.substr(0, pos + 1) + L"models.ini";
    }
    
    // 判断当前是否为深色主题
    bool isDark = (g_CurrentTheme.bg == g_DarkTheme.bg);
    WritePrivateProfileStringW(L"Config", L"Theme", isDark ? L"Dark" : L"Light", path.c_str());
}

// 从配置文件加载主题
void LoadThemeConfig() {
    wchar_t configPath[MAX_PATH];
    GetModuleFileNameW(NULL, configPath, MAX_PATH);
    std::wstring path(configPath);
    size_t pos = path.find_last_of(L"\\");
    if (pos != std::wstring::npos) {
        path = path.substr(0, pos + 1) + L"models.ini";
    }
    
    wchar_t buffer[32];
    GetPrivateProfileStringW(L"Config", L"Theme", L"Dark", buffer, 32, path.c_str());
    bool isDark = (wcscmp(buffer, L"Dark") == 0);
    SetTheme(isDark);
}
