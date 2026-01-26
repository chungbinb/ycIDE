#pragma once
#include <windows.h>
#include <string>
#include <vector>

struct AppTheme {
    // === 基础颜色 ===
    COLORREF bg;                    // 主背景色
    COLORREF text;                  // 主文本颜色
    COLORREF textDim;               // 暗淡文本（禁用/次要）
    COLORREF textBright;            // 高亮文本
    COLORREF border;                // 边框/分隔线
    COLORREF borderLight;           // 浅色边框
    
    // === 标题栏和菜单 ===
    COLORREF titleBarBg;            // 标题栏背景
    COLORREF menuText;              // 菜单文本色
    COLORREF menuHoverBg;           // 菜单悬停背景
    COLORREF closeButtonHover;      // 关闭按钮悬停色（红色）
    
    // === 工具栏 ===
    COLORREF toolBarBg;             // 工具栏背景
    COLORREF toolBarHoverBg;        // 工具栏按钮悬停背景
    
    // === 活动栏 ===
    COLORREF activityBarBg;         // 活动栏背景
    COLORREF activityBarIcon;       // 活动栏图标颜色
    COLORREF activityBarIconActive; // 活动栏选中图标颜色
    COLORREF activityBarIconHover;  // 活动栏悬停图标颜色
    COLORREF activityBarIndicator;  // 活动栏选中指示条
    COLORREF activityBarHoverBg;    // 活动栏悬停背景
    
    // === 状态栏 ===
    COLORREF statusBarBg;           // 状态栏背景
    COLORREF statusBarText;         // 状态栏文本
    
    // === AI聊天 ===
    COLORREF chatBg;                // AI聊天窗口背景
    COLORREF chatBubbleBg;          // AI气泡背景
    COLORREF chatText;              // AI聊天文本
    
    // === 资源管理器 ===
    COLORREF explorerBg;            // 资源管理器背景
    COLORREF explorerText;          // 资源管理器文本
    COLORREF explorerTabBg;         // 资源管理器标签背景
    COLORREF explorerTabActive;     // 资源管理器选中标签
    COLORREF explorerTabHover;      // 资源管理器悬停标签
    COLORREF explorerModified;      // 修改标记颜色（绿色）
    COLORREF explorerSelection;     // 资源管理器选中项背景
    
    // === 代码编辑器 ===
    COLORREF editorBg;              // 代码编辑器背景
    COLORREF lineNumBg;             // 行号背景
    COLORREF lineNumText;           // 行号文本
    COLORREF editorSelection;       // 选中背景
    COLORREF editorCurrentLine;     // 当前行背景
    
    // === 表格编辑器 ===
    COLORREF tableHeaderBg;         // 表格表头背景
    COLORREF tableSubHeaderBg;      // 子程序表头背景
    COLORREF tableVarHeaderBg;      // 变量表头背景
    COLORREF tableGridLine;         // 表格框线
    COLORREF tableRowAlt;           // 表格交替行背景
    
    // === 语法高亮 ===
    COLORREF syntaxKeyword;         // 关键字（流程控制）
    COLORREF syntaxType;            // 类型名称（青色）
    COLORREF syntaxComment;         // 注释（绿色）
    COLORREF syntaxString;          // 字符串
    COLORREF syntaxNumber;          // 数字
    COLORREF syntaxFunction;        // 函数名
    COLORREF syntaxVariable;        // 变量名
    COLORREF syntaxOperator;        // 运算符
    COLORREF syntaxRemark;          // 备注列（绿色）
    COLORREF syntaxCheckbox;        // 复选框列（深红色）
    
    // === 自动完成弹窗 ===
    COLORREF popupBg;               // 弹窗背景
    COLORREF popupBorder;           // 弹窗边框
    COLORREF popupSelection;        // 弹窗选中项
    COLORREF popupText;             // 弹窗文本
    
    // === 滚动条 ===
    COLORREF scrollTrack;           // 滚动条轨道
    COLORREF scrollThumb;           // 滚动条滑块
    COLORREF scrollThumbHover;      // 滚动条滑块悬停
    
    // === Diff 颜色 ===
    COLORREF diffAdded;             // 新增行背景
    COLORREF diffRemoved;           // 删除行背景
    
    // === 欢迎页 ===
    COLORREF welcomeButtonHover;    // 欢迎页按钮悬停
    
    // === 窗口边框 ===
    COLORREF windowBorder;          // 窗口外边框
    
    // === 兼容旧字段（将被移除） ===
    COLORREF grid;                  // 边框/分隔线 (= border)
    COLORREF margin;                // 侧边栏/工具栏
    COLORREF keywordColor;          // 关键字颜色 (= syntaxKeyword)
    
    // GDI资源（运行时创建）
    HBRUSH hBgBrush;
    HBRUSH hStatusBarBrush;
    HBRUSH hActivityBarBrush;
};

// 主题信息结构
struct ThemeInfo {
    std::wstring filename;    // 文件名（不含路径）
    std::wstring name;        // 显示名称（来自 JSON 的 name 字段）
    std::string type;         // "dark" 或 "light"
};

extern AppTheme g_DarkTheme;
extern AppTheme g_LightTheme;
extern AppTheme g_CurrentTheme;
extern std::wstring g_CurrentThemeFile;  // 当前加载的主题文件名

void SetTheme(bool isDark);
void SetThemeByFile(const std::wstring& filename);  // 通过文件名加载主题
void SaveThemeConfig();
void LoadThemeConfig();
void InitThemeBrushes();
void CleanupThemeBrushes();
void EnsureDefaultThemeFiles();
void SaveCurrentTheme(const std::wstring& filename, const std::string& themeName);
std::vector<ThemeInfo> GetAvailableThemes();  // 获取可用主题列表
