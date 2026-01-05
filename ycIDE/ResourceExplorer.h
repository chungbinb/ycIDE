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
    
    ResourceExplorerData();
    ~ResourceExplorerData();
    
    void UpdateVisibleNodes(); // 根据展开状态更新 visibleNodes
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
void ExplorerCloseProject();
