#pragma once
#include <windows.h>
#include <string>
#include <vector>

// 文件标签项
struct FileTab {
    std::wstring filePath;      // 文件完整路径
    std::wstring fileName;      // 文件名（显示用）
    bool isModified;            // 是否已修改
    int editorType;             // 编辑器类型：0=YiEditor, 1=EllEditor, 2=其他
    void* editorData;           // 指向对应编辑器的数据指针
    
    FileTab() : isModified(false), editorType(0), editorData(nullptr) {}
};

// 标签栏数据
struct TabBarData {
    HWND hWnd;
    std::vector<FileTab> tabs;
    int activeTabIndex;
    int hoverTabIndex;
    int tabHeight;
    int tabMinWidth;
    int tabMaxWidth;
    int closeButtonSize;
    
    TabBarData(HWND hwnd) : hWnd(hwnd), activeTabIndex(-1), hoverTabIndex(-1),
        tabHeight(35), tabMinWidth(100), tabMaxWidth(200), closeButtonSize(16) {}
    
    ~TabBarData() {}
    
    // 添加标签
    int AddTab(const std::wstring& filePath, const std::wstring& fileName, int editorType);
    
    // 关闭标签
    void CloseTab(int index);
    
    // 切换到指定标签
    void SetActiveTab(int index);
    
    // 根据文件路径查找标签
    int FindTab(const std::wstring& filePath);
    
    // 设置标签修改状态
    void SetTabModified(int index, bool modified);
    
    // 获取标签矩形区域
    RECT GetTabRect(int index, int clientWidth);
    
    // 获取关闭按钮矩形区域
    RECT GetCloseButtonRect(const RECT& tabRect);
};

// 注册标签栏窗口类
ATOM RegisterTabBarClass(HINSTANCE hInstance);

// 标签栏窗口过程
LRESULT CALLBACK TabBarWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// 获取文件类型对应的编辑器类型
int GetEditorTypeByExtension(const std::wstring& filePath);
