#pragma once
#include <windows.h>

struct AppTheme {
    // 基础颜色
    COLORREF bg;               // 主背景色
    COLORREF text;             // 文本颜色
    COLORREF grid;             // 边框/分隔线
    COLORREF margin;           // 侧边栏/工具栏
    
    // 标题栏
    COLORREF titleBarBg;       // 标题栏背景
    COLORREF menuText;         // 菜单文本色
    
    // AI聊天
    COLORREF chatBg;           // AI聊天窗口背景
    COLORREF chatBubbleBg;     // AI气泡背景
    COLORREF chatText;         // AI聊天文本
    
    // 资源管理器
    COLORREF explorerBg;       // 资源管理器背景
    COLORREF explorerText;     // 资源管理器文本
    
    // 代码编辑器
    COLORREF editorBg;         // 代码编辑器背景
    COLORREF lineNumBg;        // 行号背景
    COLORREF lineNumText;      // 行号文本
    
    // 语法高亮
    COLORREF keywordColor;     // 关键字颜色
    COLORREF tableHeaderBg;    // 表格表头背景
    
    HBRUSH hBgBrush;
};

extern AppTheme g_DarkTheme;
extern AppTheme g_LightTheme;
extern AppTheme g_CurrentTheme;

void SetTheme(bool isDark);
void SaveThemeConfig();
void LoadThemeConfig();
