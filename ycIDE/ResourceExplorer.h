#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <algorithm>

struct FileNode {
    std::wstring name;
    std::wstring path; // 完整路径，如果是文件夹则可能为空或作为ID
    bool isFolder;
    bool isExpanded;
    bool isModified;
    bool isSelected;
    bool isModuleLoaded; // 模块是否已加载
    int level; // 缩进层级，0为根
    std::vector<FileNode*> children;
    FileNode* parent;

    FileNode(std::wstring n, std::wstring p, bool folder, int l, FileNode* par = nullptr)
        : name(n), path(p), isFolder(folder), isExpanded(true), isModified(false), isSelected(false), isModuleLoaded(false), level(l), parent(par) {}
    
    ~FileNode() {
        for (auto child : children) {
            delete child;
        }
    }
    
    // 判断是否是.ec模块文件
    bool IsModuleFile() const {
        if (isFolder) return false;
        size_t dotPos = name.find_last_of(L'.');
        if (dotPos == std::wstring::npos) return false;
        std::wstring ext = name.substr(dotPos);
        std::transform(ext.begin(), ext.end(), ext.begin(), ::towlower);
        return ext == L".ec";
    }
};

// 资源管理器标签类型
enum ExplorerTabType {
    TAB_LIBRARY = 0,    // 支持库
    TAB_PROJECT = 1,    // 项目
    TAB_PROPERTY = 2    // 属性
};

// 支持库树形节点类型
enum LibTreeNodeType {
    LIB_NODE_LIBRARY = 0,    // 支持库根节点
    LIB_NODE_CATEGORY = 1,   // 命令类别
    LIB_NODE_COMMAND = 2,    // 命令
    LIB_NODE_DATATYPE = 3,   // 数据类型
};

// 支持库树形节点
struct LibTreeNode {
    std::wstring name;
    LibTreeNodeType nodeType;
    int level;
    bool isExpanded;
    std::vector<LibTreeNode*> children;
    
    LibTreeNode(const std::wstring& n, LibTreeNodeType t, int l)
        : name(n), nodeType(t), level(l), isExpanded(false) {}
    ~LibTreeNode() {
        for (auto c : children) delete c;
    }
};

// 资源管理器数据
struct ResourceExplorerData {
    std::vector<FileNode*> rootNodes; // 根节点列表
    std::vector<FileNode*> visibleNodes; // 当前可见的节点列表（扁平化，用于绘制）
    FileNode* selectedNode;
    int itemHeight;
    bool isWorkspaceMode; // 是否为工作区模式（打开文件夹）
    bool isProjectMode; // 是否为项目模式（打开.epp项目）
    std::wstring workspacePath; // 工作区根路径
    HANDLE hDirChangeNotify; // 文件系统监控句柄
    HANDLE hMonitorThread; // 监控线程句柄
    bool stopMonitoring; // 停止监控标志
    
    // 标签相关
    ExplorerTabType activeTab;  // 当前激活的标签
    int tabBarHeight;           // 标签栏高度
    int hoverTab;               // 悬停的标签 (-1表示无)
    
    // 边框拖动相关
    bool isBorderHover;         // 鼠标是否在边框区域
    bool isDraggingBorder;      // 是否正在拖动边框
    bool isTrackingMouse;       // 是否已注册鼠标离开追踪
    
    // 支持库树形视图
    std::vector<LibTreeNode*> libTreeRoots;        // 支持库树根节点
    std::vector<LibTreeNode*> libTreeVisible;      // 扁平化可见节点
    LibTreeNode* libTreeSelected;                  // 选中节点
    bool libTreeBuilt;                             // 树是否已构建
    int libTreeScrollY;                            // 滚动偏移
    
    ResourceExplorerData();
    ~ResourceExplorerData();
    
    void UpdateVisibleNodes(); // 根据展开状态更新 visibleNodes
    void BuildLibraryTree();   // 构建支持库树
    void UpdateLibTreeVisible(); // 更新支持库可见节点
};

extern ResourceExplorerData g_ExplorerData;

ATOM RegisterResourceExplorerClass(HINSTANCE hInstance);
LRESULT CALLBACK ResourceExplorerWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// 添加文件到资源管理器
void ExplorerAddFile(const std::wstring& path);
// 添加文件夹到资源管理器
void ExplorerAddFolder(const std::wstring& folderPath);
// 关闭工作区文件夹
void ExplorerCloseFolder();
// 刷新当前文件夹
void ExplorerRefreshFolder();
// 更新文件修改状态
void ExplorerSetFileModified(const std::wstring& path, bool modified);
// 获取当前选中的文件路径
std::wstring ExplorerGetSelectedFile();
// 选中指定文件
void ExplorerSelectFile(const std::wstring& path);
// 加载模块文件
bool ExplorerLoadModule(FileNode* node);
// 卸载模块文件
void ExplorerUnloadModule(FileNode* node);
// 加载项目文件到资源管理器
void ExplorerLoadProject();
// 关闭项目
void ExplorerCloseProject();// 切换到指定标签
void ExplorerSwitchToTab(ExplorerTabType tab);
// 设置属性面板窗口句柄（用于嵌入）
void ExplorerSetPropertyGridWindow(HWND hPropertyGrid);
// 获取嵌入的属性面板窗口句柄
HWND ExplorerGetEmbeddedPropertyGrid();
// 获取属性面板所需的区域
RECT ExplorerGetPropertyPanelRect(HWND hExplorer);