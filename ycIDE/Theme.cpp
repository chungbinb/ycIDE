#include "Theme.h"
#include "json.hpp"
#include <string>
#include <cstdio>

using json = nlohmann::json;

// 全局主题变量
AppTheme g_DarkTheme = {};
AppTheme g_LightTheme = {};
AppTheme g_CurrentTheme = {};

extern HWND hMainWnd;

// 获取主题文件夹路径
static std::wstring GetThemesFolderPath() {
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    std::wstring path(exePath);
    size_t pos = path.find_last_of(L"\\");
    if (pos != std::wstring::npos) {
        path = path.substr(0, pos + 1) + L"themes";
    }
    return path;
}

// 解析颜色字符串 "#RRGGBB" 或 "rgb(r,g,b)"
static COLORREF ParseColor(const std::string& colorStr) {
    if (colorStr.empty()) return RGB(0, 0, 0);
    
    // 处理 #RRGGBB 格式
    if (colorStr[0] == '#' && colorStr.length() >= 7) {
        int r = std::stoi(colorStr.substr(1, 2), nullptr, 16);
        int g = std::stoi(colorStr.substr(3, 2), nullptr, 16);
        int b = std::stoi(colorStr.substr(5, 2), nullptr, 16);
        return RGB(r, g, b);
    }
    
    // 处理 rgb(r,g,b) 格式
    if (colorStr.find("rgb(") == 0) {
        size_t start = 4;
        size_t comma1 = colorStr.find(',', start);
        size_t comma2 = colorStr.find(',', comma1 + 1);
        size_t end = colorStr.find(')', comma2);
        if (comma1 != std::string::npos && comma2 != std::string::npos) {
            int r = std::stoi(colorStr.substr(start, comma1 - start));
            int g = std::stoi(colorStr.substr(comma1 + 1, comma2 - comma1 - 1));
            int b = std::stoi(colorStr.substr(comma2 + 1, end - comma2 - 1));
            return RGB(r, g, b);
        }
    }
    
    return RGB(0, 0, 0);
}

// 将颜色转换为 #RRGGBB 字符串
static std::string ColorToString(COLORREF color) {
    char buf[8];
    sprintf_s(buf, "#%02X%02X%02X", GetRValue(color), GetGValue(color), GetBValue(color));
    return std::string(buf);
}

// 从 JSON 加载主题到 AppTheme 结构
static void LoadThemeFromJson(const json& j, AppTheme& theme) {
    auto getColor = [&](const char* key, COLORREF defaultColor) -> COLORREF {
        if (j.contains(key) && j[key].is_string()) {
            return ParseColor(j[key].get<std::string>());
        }
        return defaultColor;
    };
    
    // === 基础颜色 ===
    theme.bg = getColor("bg", RGB(24, 24, 24));
    theme.text = getColor("text", RGB(204, 204, 204));
    theme.textDim = getColor("textDim", RGB(128, 128, 128));
    theme.textBright = getColor("textBright", RGB(255, 255, 255));
    theme.border = getColor("border", RGB(45, 45, 45));
    theme.borderLight = getColor("borderLight", RGB(60, 60, 60));
    
    // === 标题栏和菜单 ===
    theme.titleBarBg = getColor("titleBarBg", RGB(24, 24, 24));
    theme.menuText = getColor("menuText", RGB(204, 204, 204));
    theme.menuHoverBg = getColor("menuHoverBg", RGB(62, 62, 64));
    theme.closeButtonHover = getColor("closeButtonHover", RGB(232, 17, 35));
    
    // === 工具栏 ===
    theme.toolBarBg = getColor("toolBarBg", RGB(30, 30, 30));
    theme.toolBarHoverBg = getColor("toolBarHoverBg", RGB(62, 62, 64));
    
    // === 活动栏 ===
    theme.activityBarBg = getColor("activityBarBg", RGB(24, 24, 24));
    theme.activityBarIcon = getColor("activityBarIcon", RGB(150, 150, 150));
    theme.activityBarIconActive = getColor("activityBarIconActive", RGB(255, 255, 255));
    theme.activityBarIconHover = getColor("activityBarIconHover", RGB(220, 220, 220));
    theme.activityBarIndicator = getColor("activityBarIndicator", RGB(255, 255, 255));
    theme.activityBarHoverBg = getColor("activityBarHoverBg", RGB(40, 40, 40));
    
    // === 状态栏 ===
    theme.statusBarBg = getColor("statusBarBg", RGB(24, 24, 24));
    theme.statusBarText = getColor("statusBarText", RGB(204, 204, 204));
    
    // === AI聊天 ===
    theme.chatBg = getColor("chatBg", RGB(24, 24, 24));
    theme.chatBubbleBg = getColor("chatBubbleBg", RGB(28, 41, 53));
    theme.chatText = getColor("chatText", RGB(204, 204, 204));
    
    // === 资源管理器 ===
    theme.explorerBg = getColor("explorerBg", RGB(24, 24, 24));
    theme.explorerText = getColor("explorerText", RGB(204, 204, 204));
    theme.explorerTabBg = getColor("explorerTabBg", RGB(30, 30, 30));
    theme.explorerTabActive = getColor("explorerTabActive", RGB(24, 24, 24));
    theme.explorerTabHover = getColor("explorerTabHover", RGB(40, 40, 40));
    theme.explorerModified = getColor("explorerModified", RGB(80, 200, 120));
    theme.explorerSelection = getColor("explorerSelection", RGB(4, 57, 94));
    
    // === 代码编辑器 ===
    theme.editorBg = getColor("editorBg", RGB(31, 31, 31));
    theme.lineNumBg = getColor("lineNumBg", RGB(37, 37, 38));
    theme.lineNumText = getColor("lineNumText", RGB(128, 128, 128));
    theme.editorSelection = getColor("editorSelection", RGB(38, 79, 120));
    theme.editorCurrentLine = getColor("editorCurrentLine", RGB(40, 40, 40));
    
    // === 表格编辑器 ===
    theme.tableHeaderBg = getColor("tableHeaderBg", RGB(45, 55, 60));
    theme.tableSubHeaderBg = getColor("tableSubHeaderBg", RGB(50, 60, 70));
    theme.tableVarHeaderBg = getColor("tableVarHeaderBg", RGB(35, 50, 55));
    theme.tableGridLine = getColor("tableGridLine", RGB(60, 60, 60));
    theme.tableRowAlt = getColor("tableRowAlt", RGB(34, 34, 34));
    
    // === 语法高亮 ===
    theme.syntaxKeyword = getColor("syntaxKeyword", RGB(86, 156, 214));
    theme.syntaxType = getColor("syntaxType", RGB(78, 201, 176));
    theme.syntaxComment = getColor("syntaxComment", RGB(106, 153, 85));
    theme.syntaxString = getColor("syntaxString", RGB(206, 145, 120));
    theme.syntaxNumber = getColor("syntaxNumber", RGB(181, 206, 168));
    theme.syntaxFunction = getColor("syntaxFunction", RGB(220, 220, 170));
    theme.syntaxVariable = getColor("syntaxVariable", RGB(156, 220, 254));
    theme.syntaxOperator = getColor("syntaxOperator", RGB(212, 212, 212));
    theme.syntaxRemark = getColor("syntaxRemark", RGB(106, 153, 85));
    theme.syntaxCheckbox = getColor("syntaxCheckbox", RGB(128, 40, 40));
    
    // === 自动完成弹窗 ===
    theme.popupBg = getColor("popupBg", RGB(40, 40, 40));
    theme.popupBorder = getColor("popupBorder", RGB(69, 69, 69));
    theme.popupSelection = getColor("popupSelection", RGB(4, 57, 94));
    theme.popupText = getColor("popupText", RGB(204, 204, 204));
    
    // === 滚动条 ===
    theme.scrollTrack = getColor("scrollTrack", RGB(30, 30, 30));
    theme.scrollThumb = getColor("scrollThumb", RGB(70, 70, 70));
    theme.scrollThumbHover = getColor("scrollThumbHover", RGB(90, 90, 90));
    
    // === Diff 颜色 ===
    theme.diffAdded = getColor("diffAdded", RGB(55, 75, 55));
    theme.diffRemoved = getColor("diffRemoved", RGB(75, 55, 55));
    
    // === 欢迎页 ===
    theme.welcomeButtonHover = getColor("welcomeButtonHover", RGB(40, 40, 40));
    
    // === 窗口边框 ===
    theme.windowBorder = getColor("windowBorder", RGB(45, 45, 45));
    
    // === 兼容旧字段 ===
    theme.grid = theme.border;
    theme.margin = theme.lineNumBg;
    theme.keywordColor = theme.syntaxKeyword;
    
    // GDI 资源初始化为 nullptr
    theme.hBgBrush = nullptr;
    theme.hStatusBarBrush = nullptr;
    theme.hActivityBarBrush = nullptr;
}

// 将主题保存为 JSON 文件
static void SaveThemeToJson(const std::wstring& filePath, const AppTheme& theme, const std::string& themeName) {
    json j;
    
    j["name"] = themeName;
    j["type"] = (theme.bg == RGB(24, 24, 24) || GetRValue(theme.bg) < 128) ? "dark" : "light";
    
    // === 基础颜色 ===
    j["bg"] = ColorToString(theme.bg);
    j["text"] = ColorToString(theme.text);
    j["textDim"] = ColorToString(theme.textDim);
    j["textBright"] = ColorToString(theme.textBright);
    j["border"] = ColorToString(theme.border);
    j["borderLight"] = ColorToString(theme.borderLight);
    
    // === 标题栏和菜单 ===
    j["titleBarBg"] = ColorToString(theme.titleBarBg);
    j["menuText"] = ColorToString(theme.menuText);
    j["menuHoverBg"] = ColorToString(theme.menuHoverBg);
    j["closeButtonHover"] = ColorToString(theme.closeButtonHover);
    
    // === 工具栏 ===
    j["toolBarBg"] = ColorToString(theme.toolBarBg);
    j["toolBarHoverBg"] = ColorToString(theme.toolBarHoverBg);
    
    // === 活动栏 ===
    j["activityBarBg"] = ColorToString(theme.activityBarBg);
    j["activityBarIcon"] = ColorToString(theme.activityBarIcon);
    j["activityBarIconActive"] = ColorToString(theme.activityBarIconActive);
    j["activityBarIconHover"] = ColorToString(theme.activityBarIconHover);
    j["activityBarIndicator"] = ColorToString(theme.activityBarIndicator);
    j["activityBarHoverBg"] = ColorToString(theme.activityBarHoverBg);
    
    // === 状态栏 ===
    j["statusBarBg"] = ColorToString(theme.statusBarBg);
    j["statusBarText"] = ColorToString(theme.statusBarText);
    
    // === AI聊天 ===
    j["chatBg"] = ColorToString(theme.chatBg);
    j["chatBubbleBg"] = ColorToString(theme.chatBubbleBg);
    j["chatText"] = ColorToString(theme.chatText);
    
    // === 资源管理器 ===
    j["explorerBg"] = ColorToString(theme.explorerBg);
    j["explorerText"] = ColorToString(theme.explorerText);
    j["explorerTabBg"] = ColorToString(theme.explorerTabBg);
    j["explorerTabActive"] = ColorToString(theme.explorerTabActive);
    j["explorerTabHover"] = ColorToString(theme.explorerTabHover);
    j["explorerModified"] = ColorToString(theme.explorerModified);
    j["explorerSelection"] = ColorToString(theme.explorerSelection);
    
    // === 代码编辑器 ===
    j["editorBg"] = ColorToString(theme.editorBg);
    j["lineNumBg"] = ColorToString(theme.lineNumBg);
    j["lineNumText"] = ColorToString(theme.lineNumText);
    j["editorSelection"] = ColorToString(theme.editorSelection);
    j["editorCurrentLine"] = ColorToString(theme.editorCurrentLine);
    
    // === 表格编辑器 ===
    j["tableHeaderBg"] = ColorToString(theme.tableHeaderBg);
    j["tableSubHeaderBg"] = ColorToString(theme.tableSubHeaderBg);
    j["tableVarHeaderBg"] = ColorToString(theme.tableVarHeaderBg);
    j["tableGridLine"] = ColorToString(theme.tableGridLine);
    j["tableRowAlt"] = ColorToString(theme.tableRowAlt);
    
    // === 语法高亮 ===
    j["syntaxKeyword"] = ColorToString(theme.syntaxKeyword);
    j["syntaxType"] = ColorToString(theme.syntaxType);
    j["syntaxComment"] = ColorToString(theme.syntaxComment);
    j["syntaxString"] = ColorToString(theme.syntaxString);
    j["syntaxNumber"] = ColorToString(theme.syntaxNumber);
    j["syntaxFunction"] = ColorToString(theme.syntaxFunction);
    j["syntaxVariable"] = ColorToString(theme.syntaxVariable);
    j["syntaxOperator"] = ColorToString(theme.syntaxOperator);
    j["syntaxRemark"] = ColorToString(theme.syntaxRemark);
    j["syntaxCheckbox"] = ColorToString(theme.syntaxCheckbox);
    
    // === 自动完成弹窗 ===
    j["popupBg"] = ColorToString(theme.popupBg);
    j["popupBorder"] = ColorToString(theme.popupBorder);
    j["popupSelection"] = ColorToString(theme.popupSelection);
    j["popupText"] = ColorToString(theme.popupText);
    
    // === 滚动条 ===
    j["scrollTrack"] = ColorToString(theme.scrollTrack);
    j["scrollThumb"] = ColorToString(theme.scrollThumb);
    j["scrollThumbHover"] = ColorToString(theme.scrollThumbHover);
    
    // === Diff 颜色 ===
    j["diffAdded"] = ColorToString(theme.diffAdded);
    j["diffRemoved"] = ColorToString(theme.diffRemoved);
    
    // === 欢迎页 ===
    j["welcomeButtonHover"] = ColorToString(theme.welcomeButtonHover);
    
    // === 窗口边框 ===
    j["windowBorder"] = ColorToString(theme.windowBorder);
    
    // 写入文件 (使用 _wfopen 支持 wstring 路径)
    FILE* fp = nullptr;
    if (_wfopen_s(&fp, filePath.c_str(), L"w") == 0 && fp) {
        std::string content = j.dump(4);  // 缩进4空格
        fwrite(content.c_str(), 1, content.size(), fp);
        fclose(fp);
    }
}

// 从文件加载主题
static bool LoadThemeFromFile(const std::wstring& filePath, AppTheme& theme) {
    FILE* fp = nullptr;
    if (_wfopen_s(&fp, filePath.c_str(), L"r") != 0 || !fp) {
        return false;
    }
    
    try {
        // 读取文件内容
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        std::string content(size, '\0');
        fread(&content[0], 1, size, fp);
        fclose(fp);
        
        json j = json::parse(content);
        LoadThemeFromJson(j, theme);
        return true;
    } catch (...) {
        if (fp) fclose(fp);
        return false;
    }
}

// 创建默认深色主题数据
static void InitDefaultDarkTheme(AppTheme& theme) {
    // === 基础颜色 ===
    theme.bg = RGB(24, 24, 24);
    theme.text = RGB(204, 204, 204);
    theme.textDim = RGB(128, 128, 128);
    theme.textBright = RGB(255, 255, 255);
    theme.border = RGB(45, 45, 45);
    theme.borderLight = RGB(60, 60, 60);
    
    // === 标题栏和菜单 ===
    theme.titleBarBg = RGB(24, 24, 24);
    theme.menuText = RGB(204, 204, 204);
    theme.menuHoverBg = RGB(62, 62, 64);
    theme.closeButtonHover = RGB(232, 17, 35);
    
    // === 工具栏 ===
    theme.toolBarBg = RGB(30, 30, 30);
    theme.toolBarHoverBg = RGB(62, 62, 64);
    
    // === 活动栏 ===
    theme.activityBarBg = RGB(24, 24, 24);
    theme.activityBarIcon = RGB(150, 150, 150);
    theme.activityBarIconActive = RGB(255, 255, 255);
    theme.activityBarIconHover = RGB(220, 220, 220);
    theme.activityBarIndicator = RGB(255, 255, 255);
    theme.activityBarHoverBg = RGB(40, 40, 40);
    
    // === 状态栏 ===
    theme.statusBarBg = RGB(24, 24, 24);
    theme.statusBarText = RGB(204, 204, 204);
    
    // === AI聊天 ===
    theme.chatBg = RGB(24, 24, 24);
    theme.chatBubbleBg = RGB(28, 41, 53);
    theme.chatText = RGB(204, 204, 204);
    
    // === 资源管理器 ===
    theme.explorerBg = RGB(24, 24, 24);
    theme.explorerText = RGB(204, 204, 204);
    theme.explorerTabBg = RGB(30, 30, 30);
    theme.explorerTabActive = RGB(24, 24, 24);
    theme.explorerTabHover = RGB(40, 40, 40);
    theme.explorerModified = RGB(80, 200, 120);
    theme.explorerSelection = RGB(4, 57, 94);
    
    // === 代码编辑器 ===
    theme.editorBg = RGB(31, 31, 31);
    theme.lineNumBg = RGB(37, 37, 38);
    theme.lineNumText = RGB(128, 128, 128);
    theme.editorSelection = RGB(38, 79, 120);
    theme.editorCurrentLine = RGB(40, 40, 40);
    
    // === 表格编辑器 ===
    theme.tableHeaderBg = RGB(45, 55, 60);
    theme.tableSubHeaderBg = RGB(50, 60, 70);
    theme.tableVarHeaderBg = RGB(35, 50, 55);
    theme.tableGridLine = RGB(60, 60, 60);
    theme.tableRowAlt = RGB(34, 34, 34);
    
    // === 语法高亮 ===
    theme.syntaxKeyword = RGB(86, 156, 214);
    theme.syntaxType = RGB(78, 201, 176);
    theme.syntaxComment = RGB(106, 153, 85);
    theme.syntaxString = RGB(206, 145, 120);
    theme.syntaxNumber = RGB(181, 206, 168);
    theme.syntaxFunction = RGB(220, 220, 170);
    theme.syntaxVariable = RGB(156, 220, 254);
    theme.syntaxOperator = RGB(212, 212, 212);
    theme.syntaxRemark = RGB(106, 153, 85);
    theme.syntaxCheckbox = RGB(128, 40, 40);
    
    // === 自动完成弹窗 ===
    theme.popupBg = RGB(40, 40, 40);
    theme.popupBorder = RGB(69, 69, 69);
    theme.popupSelection = RGB(4, 57, 94);
    theme.popupText = RGB(204, 204, 204);
    
    // === 滚动条 ===
    theme.scrollTrack = RGB(30, 30, 30);
    theme.scrollThumb = RGB(70, 70, 70);
    theme.scrollThumbHover = RGB(90, 90, 90);
    
    // === Diff 颜色 ===
    theme.diffAdded = RGB(55, 75, 55);
    theme.diffRemoved = RGB(75, 55, 55);
    
    // === 欢迎页 ===
    theme.welcomeButtonHover = RGB(40, 40, 40);
    
    // === 窗口边框 ===
    theme.windowBorder = RGB(45, 45, 45);
    
    // === 兼容旧字段 ===
    theme.grid = theme.border;
    theme.margin = theme.lineNumBg;
    theme.keywordColor = theme.syntaxKeyword;
    
    // GDI 资源
    theme.hBgBrush = nullptr;
    theme.hStatusBarBrush = nullptr;
    theme.hActivityBarBrush = nullptr;
}

// 创建默认浅色主题数据
static void InitDefaultLightTheme(AppTheme& theme) {
    // === 基础颜色 ===
    theme.bg = RGB(255, 255, 255);
    theme.text = RGB(0, 0, 0);
    theme.textDim = RGB(128, 128, 128);
    theme.textBright = RGB(0, 0, 0);
    theme.border = RGB(204, 206, 219);
    theme.borderLight = RGB(230, 230, 230);
    
    // === 标题栏和菜单 ===
    theme.titleBarBg = RGB(221, 221, 221);
    theme.menuText = RGB(30, 30, 30);
    theme.menuHoverBg = RGB(190, 230, 253);
    theme.closeButtonHover = RGB(232, 17, 35);
    
    // === 工具栏 ===
    theme.toolBarBg = RGB(236, 236, 236);
    theme.toolBarHoverBg = RGB(200, 224, 248);
    
    // === 活动栏 ===
    theme.activityBarBg = RGB(51, 51, 51);
    theme.activityBarIcon = RGB(150, 150, 150);
    theme.activityBarIconActive = RGB(255, 255, 255);
    theme.activityBarIconHover = RGB(220, 220, 220);
    theme.activityBarIndicator = RGB(255, 255, 255);
    theme.activityBarHoverBg = RGB(70, 70, 70);
    
    // === 状态栏 ===
    theme.statusBarBg = RGB(0, 122, 204);
    theme.statusBarText = RGB(255, 255, 255);
    
    // === AI聊天 ===
    theme.chatBg = RGB(255, 255, 255);
    theme.chatBubbleBg = RGB(240, 240, 240);
    theme.chatText = RGB(0, 0, 0);
    
    // === 资源管理器 ===
    theme.explorerBg = RGB(246, 246, 246);
    theme.explorerText = RGB(51, 51, 51);
    theme.explorerTabBg = RGB(236, 236, 236);
    theme.explorerTabActive = RGB(255, 255, 255);
    theme.explorerTabHover = RGB(230, 230, 230);
    theme.explorerModified = RGB(40, 180, 99);
    theme.explorerSelection = RGB(173, 214, 255);
    
    // === 代码编辑器 ===
    theme.editorBg = RGB(255, 255, 255);
    theme.lineNumBg = RGB(246, 246, 246);
    theme.lineNumText = RGB(128, 128, 128);
    theme.editorSelection = RGB(173, 214, 255);
    theme.editorCurrentLine = RGB(255, 255, 204);
    
    // === 表格编辑器 ===
    theme.tableHeaderBg = RGB(230, 240, 235);
    theme.tableSubHeaderBg = RGB(230, 235, 250);
    theme.tableVarHeaderBg = RGB(225, 240, 235);
    theme.tableGridLine = RGB(200, 200, 200);
    theme.tableRowAlt = RGB(248, 248, 248);
    
    // === 语法高亮 ===
    theme.syntaxKeyword = RGB(0, 0, 255);
    theme.syntaxType = RGB(0, 128, 128);
    theme.syntaxComment = RGB(0, 128, 0);
    theme.syntaxString = RGB(163, 21, 21);
    theme.syntaxNumber = RGB(0, 128, 0);
    theme.syntaxFunction = RGB(116, 83, 31);
    theme.syntaxVariable = RGB(31, 55, 127);
    theme.syntaxOperator = RGB(0, 0, 0);
    theme.syntaxRemark = RGB(0, 128, 0);
    theme.syntaxCheckbox = RGB(128, 40, 40);
    
    // === 自动完成弹窗 ===
    theme.popupBg = RGB(255, 255, 255);
    theme.popupBorder = RGB(200, 200, 200);
    theme.popupSelection = RGB(173, 214, 255);
    theme.popupText = RGB(0, 0, 0);
    
    // === 滚动条 ===
    theme.scrollTrack = RGB(246, 246, 246);
    theme.scrollThumb = RGB(200, 200, 200);
    theme.scrollThumbHover = RGB(180, 180, 180);
    
    // === Diff 颜色 ===
    theme.diffAdded = RGB(220, 255, 220);
    theme.diffRemoved = RGB(255, 220, 220);
    
    // === 欢迎页 ===
    theme.welcomeButtonHover = RGB(230, 230, 230);
    
    // === 窗口边框 ===
    theme.windowBorder = RGB(200, 200, 200);
    
    // === 兼容旧字段 ===
    theme.grid = theme.border;
    theme.margin = theme.lineNumBg;
    theme.keywordColor = theme.syntaxKeyword;
    
    // GDI 资源
    theme.hBgBrush = nullptr;
    theme.hStatusBarBrush = nullptr;
    theme.hActivityBarBrush = nullptr;
}

// 确保主题文件夹和默认主题文件存在
void EnsureDefaultThemeFiles() {
    std::wstring themesFolder = GetThemesFolderPath();
    
    // 创建 themes 文件夹
    CreateDirectoryW(themesFolder.c_str(), NULL);
    
    // 检查文件夹中是否有任何主题文件
    std::wstring searchPath = themesFolder + L"\\*.json";
    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);
    bool hasThemeFiles = (hFind != INVALID_HANDLE_VALUE);
    if (hFind != INVALID_HANDLE_VALUE) {
        FindClose(hFind);
    }
    
    // 如果没有任何主题文件，创建默认的深色和浅色主题
    if (!hasThemeFiles) {
        AppTheme defaultDark;
        InitDefaultDarkTheme(defaultDark);
        SaveThemeToJson(themesFolder + L"\\VS Code Dark+.json", defaultDark, "VS Code Dark+");
        
        AppTheme defaultLight;
        InitDefaultLightTheme(defaultLight);
        SaveThemeToJson(themesFolder + L"\\VS Code Light.json", defaultLight, "VS Code Light");
    }
}

// 初始化主题相关GDI资源
void InitThemeBrushes() {
    CleanupThemeBrushes();
    g_CurrentTheme.hBgBrush = CreateSolidBrush(g_CurrentTheme.bg);
    g_CurrentTheme.hStatusBarBrush = CreateSolidBrush(g_CurrentTheme.statusBarBg);
    g_CurrentTheme.hActivityBarBrush = CreateSolidBrush(g_CurrentTheme.activityBarBg);
}

// 清理主题GDI资源
void CleanupThemeBrushes() {
    if (g_CurrentTheme.hBgBrush) { DeleteObject(g_CurrentTheme.hBgBrush); g_CurrentTheme.hBgBrush = nullptr; }
    if (g_CurrentTheme.hStatusBarBrush) { DeleteObject(g_CurrentTheme.hStatusBarBrush); g_CurrentTheme.hStatusBarBrush = nullptr; }
    if (g_CurrentTheme.hActivityBarBrush) { DeleteObject(g_CurrentTheme.hActivityBarBrush); g_CurrentTheme.hActivityBarBrush = nullptr; }
}

// 当前加载的主题文件名（空字符串表示尚未加载）
std::wstring g_CurrentThemeFile = L"";

// 从 JSON 文件中只读取主题信息（名称和类型）
static bool GetThemeInfoFromFile(const std::wstring& filePath, ThemeInfo& info) {
    FILE* fp = nullptr;
    if (_wfopen_s(&fp, filePath.c_str(), L"r") != 0 || !fp) {
        return false;
    }
    
    try {
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        std::string content(size, '\0');
        fread(&content[0], 1, size, fp);
        fclose(fp);
        
        json j = json::parse(content);
        
        // 获取名称
        if (j.contains("name") && j["name"].is_string()) {
            std::string name = j["name"].get<std::string>();
            // 转换 UTF-8 到 wstring
            int wlen = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, nullptr, 0);
            if (wlen > 0) {
                info.name.resize(wlen - 1);
                MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, &info.name[0], wlen);
            }
        }
        
        // 获取类型
        if (j.contains("type") && j["type"].is_string()) {
            info.type = j["type"].get<std::string>();
        }
        
        return true;
    } catch (...) {
        if (fp) fclose(fp);
        return false;
    }
}

// 获取可用主题列表
std::vector<ThemeInfo> GetAvailableThemes() {
    std::vector<ThemeInfo> themes;
    std::wstring themesFolder = GetThemesFolderPath();
    std::wstring searchPath = themesFolder + L"\\*.json";
    
    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);
    
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                ThemeInfo info;
                info.filename = findData.cFileName;
                
                std::wstring fullPath = themesFolder + L"\\" + info.filename;
                if (GetThemeInfoFromFile(fullPath, info)) {
                    // 如果没有读到名称，使用文件名作为名称
                    if (info.name.empty()) {
                        info.name = info.filename;
                        // 移除 .json 扩展名
                        size_t pos = info.name.rfind(L".json");
                        if (pos != std::wstring::npos) {
                            info.name = info.name.substr(0, pos);
                        }
                    }
                    themes.push_back(info);
                }
            }
        } while (FindNextFileW(hFind, &findData));
        FindClose(hFind);
    }
    
    return themes;
}

// 通过文件名加载主题
void SetThemeByFile(const std::wstring& filename) {
    CleanupThemeBrushes();
    
    std::wstring themesFolder = GetThemesFolderPath();
    std::wstring themePath = themesFolder + L"\\" + filename;
    
    AppTheme theme;
    if (LoadThemeFromFile(themePath, theme)) {
        g_CurrentTheme = theme;
        g_CurrentThemeFile = filename;
    } else {
        // 加载失败，尝试加载第一个可用主题
        auto themes = GetAvailableThemes();
        if (!themes.empty()) {
            std::wstring firstThemePath = themesFolder + L"\\" + themes[0].filename;
            if (LoadThemeFromFile(firstThemePath, theme)) {
                g_CurrentTheme = theme;
                g_CurrentThemeFile = themes[0].filename;
            } else {
                InitDefaultDarkTheme(g_CurrentTheme);
                g_CurrentThemeFile = L"";
            }
        } else {
            InitDefaultDarkTheme(g_CurrentTheme);
            g_CurrentThemeFile = L"";
        }
    }
    
    InitThemeBrushes();
}

// 加载主题（旧接口，保持兼容 - 查找类型匹配的第一个主题）
void SetTheme(bool isDark) {
    auto themes = GetAvailableThemes();
    std::wstring targetType = isDark ? L"dark" : L"light";
    
    // 查找类型匹配的第一个主题
    for (const auto& t : themes) {
        std::wstring type(t.type.begin(), t.type.end());
        if (type == targetType) {
            SetThemeByFile(t.filename);
            return;
        }
    }
    
    // 没找到匹配类型，使用第一个主题
    if (!themes.empty()) {
        SetThemeByFile(themes[0].filename);
    }
}

// 保存当前主题配置到 models.ini
void SaveThemeConfig() {
    wchar_t configPath[MAX_PATH];
    GetModuleFileNameW(NULL, configPath, MAX_PATH);
    std::wstring path(configPath);
    size_t pos = path.find_last_of(L"\\");
    if (pos != std::wstring::npos) {
        path = path.substr(0, pos + 1) + L"models.ini";
    }
    
    // 保存当前主题文件名
    WritePrivateProfileStringW(L"Config", L"ThemeFile", g_CurrentThemeFile.c_str(), path.c_str());
}

// 从配置文件加载主题
void LoadThemeConfig() {
    // 确保主题文件存在
    EnsureDefaultThemeFiles();
    
    wchar_t configPath[MAX_PATH];
    GetModuleFileNameW(NULL, configPath, MAX_PATH);
    std::wstring path(configPath);
    size_t pos = path.find_last_of(L"\\");
    if (pos != std::wstring::npos) {
        path = path.substr(0, pos + 1) + L"models.ini";
    }
    
    wchar_t buffer[MAX_PATH];
    GetPrivateProfileStringW(L"Config", L"ThemeFile", L"", buffer, MAX_PATH, path.c_str());
    
    if (buffer[0] != L'\0') {
        // 有保存的主题文件，尝试加载
        SetThemeByFile(buffer);
    } else {
        // 没有保存的主题，加载第一个可用主题
        auto themes = GetAvailableThemes();
        if (!themes.empty()) {
            SetThemeByFile(themes[0].filename);
        } else {
            // 没有任何主题文件，使用内置默认
            InitDefaultDarkTheme(g_CurrentTheme);
            InitThemeBrushes();
        }
    }
}

// 保存当前主题到文件
void SaveCurrentTheme(const std::wstring& filename, const std::string& themeName) {
    std::wstring themesFolder = GetThemesFolderPath();
    CreateDirectoryW(themesFolder.c_str(), NULL);
    
    std::wstring filePath = themesFolder + L"\\" + filename;
    SaveThemeToJson(filePath, g_CurrentTheme, themeName);
}
