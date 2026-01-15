#include "framework.h"
#include "ResourceExplorer.h"
#include "LibraryParser.h"
#include "Theme.h"
#include "Utils.h"
#include "YiEditor.h"
#include "ProjectManager.h"
#include <gdiplus.h>
#include <windowsx.h>
#include <algorithm>
#include <filesystem>
#include <sstream>
#include <map>
#include <functional>
#include <shellapi.h>

#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

ResourceExplorerData g_ExplorerData;

ResourceExplorerData::ResourceExplorerData() : selectedNode(nullptr), itemHeight(30), isWorkspaceMode(false), isProjectMode(false), workspacePath(L""), 
    hDirChangeNotify(INVALID_HANDLE_VALUE), hMonitorThread(NULL), stopMonitoring(false) {
}

ResourceExplorerData::~ResourceExplorerData() {
    // 停止监控
    stopMonitoring = true;
    if (hDirChangeNotify != INVALID_HANDLE_VALUE) {
        FindCloseChangeNotification(hDirChangeNotify);
        hDirChangeNotify = INVALID_HANDLE_VALUE;
    }
    if (hMonitorThread) {
        WaitForSingleObject(hMonitorThread, 1000);
        CloseHandle(hMonitorThread);
        hMonitorThread = NULL;
    }
    
    for (auto node : rootNodes) {
        delete node;
    }
}

void FlattenNodes(FileNode* node, std::vector<FileNode*>& list) {
    list.push_back(node);
    if (node->isFolder && node->isExpanded) {
        for (auto child : node->children) {
            FlattenNodes(child, list);
        }
    }
}

void ResourceExplorerData::UpdateVisibleNodes() {
    visibleNodes.clear();
    for (auto node : rootNodes) {
        FlattenNodes(node, visibleNodes);
    }
}

// 全局时间线根节点
FileNode* g_TimelineRoot = nullptr;

void UpdateTimeline(EditorDocument* doc) {
    if (!g_TimelineRoot) {
        g_TimelineRoot = new FileNode(L"时间线", L"TIMELINE_ROOT", true, 0);
        g_ExplorerData.rootNodes.push_back(g_TimelineRoot);
    }
    
    // 清除现有子节点
    for (auto child : g_TimelineRoot->children) {
        delete child;
    }
    g_TimelineRoot->children.clear();
    
    if (doc) {
        // 添加快照
        for (const auto& snap : doc->snapshots) {
            FileNode* node = new FileNode(snap.name, snap.id, false, 1, g_TimelineRoot);
            g_TimelineRoot->children.push_back(node);
        }
    }
    
    g_ExplorerData.UpdateVisibleNodes();
    
    extern HWND hRightPanelWnd;
    if (hRightPanelWnd) {
        InvalidateRect(hRightPanelWnd, NULL, TRUE);
    }
}

ATOM RegisterResourceExplorerClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = ResourceExplorerWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"ResourceExplorer";
    wcex.hIconSm = nullptr;

    return RegisterClassExW(&wcex);
}

// 辅助函数：分割路径
std::vector<std::wstring> SplitPath(const std::wstring& path) {
    std::vector<std::wstring> parts;
    std::wstringstream ss(path);
    std::wstring item;
    while (std::getline(ss, item, L'\\')) {
        if (!item.empty()) parts.push_back(item);
    }
    // 处理正斜杠
    if (parts.empty() && !path.empty()) {
        ss = std::wstringstream(path);
        while (std::getline(ss, item, L'/')) {
            if (!item.empty()) parts.push_back(item);
        }
    }
    return parts;
}

void ExplorerAddFile(const std::wstring& path) {
    // 如果不是工作区模式，只显示文件名
    if (!g_ExplorerData.isWorkspaceMode) {
        // 提取文件名
        size_t lastSlash = path.find_last_of(L"\\/");
        std::wstring fileName = (lastSlash != std::wstring::npos) ? path.substr(lastSlash + 1) : path;
        
        // 检查是否已存在（按路径匹配）
        for (auto node : g_ExplorerData.rootNodes) {
            if (node->path == path) {
                return; // 文件已存在，不重复添加
            }
        }
        
        // 创建单个文件节点
        FileNode* newNode = new FileNode(fileName, path, false, 0, nullptr);
        g_ExplorerData.rootNodes.push_back(newNode);
        g_ExplorerData.UpdateVisibleNodes();
        
        extern HWND hRightPanelWnd;
        if (hRightPanelWnd) {
            InvalidateRect(hRightPanelWnd, NULL, TRUE);
        }
        return;
    }
    
    // 工作区模式：显示完整层级结构
    std::vector<std::wstring> parts = SplitPath(path);
    if (parts.empty()) return;

    std::vector<FileNode*>* currentLevel = &g_ExplorerData.rootNodes;
    FileNode* parent = nullptr;
    int level = 0;

    for (size_t i = 0; i < parts.size(); ++i) {
        std::wstring partName = parts[i];
        bool isLast = (i == parts.size() - 1);
        
        // 在当前层级查找是否存在
        FileNode* foundNode = nullptr;
        for (auto node : *currentLevel) {
            if (node->name == partName) {
                foundNode = node;
                break;
            }
        }

        if (foundNode) {
            if (isLast && !foundNode->isFolder) {
                // 文件已存在，不做处理
                return;
            }
            if (!isLast && !foundNode->isFolder) {
                // 路径冲突：同名文件和文件夹
                return; 
            }
            // 进入下一级
            currentLevel = &foundNode->children;
            parent = foundNode;
        } else {
            // 创建新节点
            bool isFolder = !isLast;
            // 如果是文件，使用完整路径；如果是文件夹，路径可以为空或构造
            std::wstring nodePath = isLast ? path : L""; 
            
            FileNode* newNode = new FileNode(partName, nodePath, isFolder, level, parent);
            currentLevel->push_back(newNode);
            
            currentLevel = &newNode->children;
            parent = newNode;
        }
        level++;
    }
    
    g_ExplorerData.UpdateVisibleNodes();
    
    extern HWND hRightPanelWnd;
    if (hRightPanelWnd) {
        InvalidateRect(hRightPanelWnd, NULL, TRUE);
    }
}

void SetModifiedRecursive(FileNode* node, const std::wstring& path, bool modified) {
    if (!node->isFolder && node->path == path) {
        node->isModified = modified;
        return;
    }
    for (auto child : node->children) {
        SetModifiedRecursive(child, path, modified);
    }
}

void ExplorerSetFileModified(const std::wstring& path, bool modified) {
    for (auto node : g_ExplorerData.rootNodes) {
        SetModifiedRecursive(node, path, modified);
    }
    extern HWND hRightPanelWnd;
    if (hRightPanelWnd) {
        InvalidateRect(hRightPanelWnd, NULL, TRUE);
    }
}

std::wstring ExplorerGetSelectedFile() {
    if (g_ExplorerData.selectedNode && !g_ExplorerData.selectedNode->isFolder) {
        return g_ExplorerData.selectedNode->path;
    }
    return L"";
}

// 选中指定文件
void ExplorerSelectFile(const std::wstring& path) {
    if (path.empty()) return;
    
    // 首先取消所有选中状态
    if (g_ExplorerData.selectedNode) {
        g_ExplorerData.selectedNode->isSelected = false;
        g_ExplorerData.selectedNode = nullptr;
    }
    
    // 查找匹配的节点
    for (auto node : g_ExplorerData.visibleNodes) {
        if (!node->isFolder && node->path == path) {
            node->isSelected = true;
            g_ExplorerData.selectedNode = node;
            
            // 刷新资源管理器显示
            extern HWND hRightPanelWnd;
            if (hRightPanelWnd) {
                InvalidateRect(hRightPanelWnd, NULL, TRUE);
            }
            break;
        }
    }
}

void DrawRoundedRectangle(Graphics& graphics, Pen* pen, RectF rect, float radius) {
    GraphicsPath path;
    path.AddLine(rect.X + radius, rect.Y, rect.X + rect.Width - radius, rect.Y);
    path.AddArc(rect.X + rect.Width - radius * 2, rect.Y, radius * 2, radius * 2, 270, 90);
    path.AddLine(rect.X + rect.Width, rect.Y + radius, rect.X + rect.Width, rect.Y + rect.Height - radius);
    path.AddArc(rect.X + rect.Width - radius * 2, rect.Y + rect.Height - radius * 2, radius * 2, radius * 2, 0, 90);
    path.AddLine(rect.X + rect.Width - radius, rect.Y + rect.Height, rect.X + radius, rect.Y + rect.Height);
    path.AddArc(rect.X, rect.Y + rect.Height - radius * 2, radius * 2, radius * 2, 90, 90);
    path.AddLine(rect.X, rect.Y + rect.Height - radius, rect.X, rect.Y + radius);
    path.AddArc(rect.X, rect.Y, radius * 2, radius * 2, 180, 90);
    path.CloseFigure();
    graphics.DrawPath(pen, &path);
}

void DrawTriangle(Graphics& graphics, Brush* brush, float x, float y, float size, bool expanded) {
    PointF points[3];
    if (expanded) {
        // 向下箭头
        points[0] = PointF(x, y + size / 4);
        points[1] = PointF(x + size, y + size / 4);
        points[2] = PointF(x + size / 2, y + size * 3 / 4);
    } else {
        // 向右箭头
        points[0] = PointF(x + size / 4, y);
        points[1] = PointF(x + size * 3 / 4, y + size / 2);
        points[2] = PointF(x + size / 4, y + size);
    }
    graphics.FillPolygon(brush, points, 3);
}

LRESULT CALLBACK ResourceExplorerWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_NCHITTEST:
        {
            // 让右边缘5px的鼠标消息穿透到主窗口，用于调整窗口大小
            POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            ScreenToClient(hWnd, &pt);
            RECT rect;
            GetClientRect(hWnd, &rect);
            if (pt.x > rect.right - 5) {
                return HTTRANSPARENT;
            }
            return HTCLIENT;
        }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            RECT rect;
            GetClientRect(hWnd, &rect);
            
            // 双缓冲
            HDC hMemDC = CreateCompatibleDC(hdc);
            HBITMAP hBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
            SelectObject(hMemDC, hBitmap);
            
            Graphics graphics(hMemDC);
            graphics.SetSmoothingMode(SmoothingModeAntiAlias);
            
            // 背景色
            Color bgColor;
            bgColor.SetFromCOLORREF(g_CurrentTheme.bg);
            SolidBrush bgBrush(bgColor);
            graphics.FillRectangle(&bgBrush, 0, 0, rect.right, rect.bottom);
            
            // 绘制单线边框
            Color borderColor;
            borderColor.SetFromCOLORREF(g_CurrentTheme.grid);
            Pen borderPen(borderColor, 1.0f);
            graphics.DrawRectangle(&borderPen, 0, 0, rect.right - 1, rect.bottom - 1);
            
            // 文本颜色
            Color textColor;
            textColor.SetFromCOLORREF(g_CurrentTheme.text);
            SolidBrush textBrush(textColor);
            // 使用支持完整Unicode字符集的字体（包括emoji等特殊字符）
            Font font(L"Microsoft YaHei UI", 10);
            
            // 创建GDI字体用于绘制文件名（GDI的DrawTextW支持emoji）
            HFONT hGdiFont = CreateFontW(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft YaHei");
            HFONT oldFont = (HFONT)SelectObject(hMemDC, hGdiFont);
            SetTextColor(hMemDC, g_CurrentTheme.text);
            SetBkMode(hMemDC, TRANSPARENT);
            
            // 绘制文件列表
            int y = 0;
            int indentSize = 20;
            
            for (size_t i = 0; i < g_ExplorerData.visibleNodes.size(); ++i) {
                FileNode* node = g_ExplorerData.visibleNodes[i];
                
                // 选中项背景
                if (node == g_ExplorerData.selectedNode) {
                    Color selColor(50, 100, 100, 100); // 半透明灰色
                    SolidBrush selBrush(selColor);
                    graphics.FillRectangle(&selBrush, 0, y, rect.right, g_ExplorerData.itemHeight);
                }
                
                int x = 10 + node->level * indentSize;
                
                // 绘制展开/折叠箭头 (仅文件夹)
                if (node->isFolder) {
                    DrawTriangle(graphics, &textBrush, (float)x - 12, (float)y + 10, 10, node->isExpanded);
                }
                
                // 绘制图标 (简单模拟)
                // 这里可以用 DrawImage 绘制真实的图标
                // 暂时用文字颜色区分：文件夹黄色，文件白色(跟随主题)
                Color iconColor;
                if (node->isFolder) {
                    iconColor = Color(255, 220, 180, 80); // 文件夹黄色
                } else if (node->IsModuleFile()) {
                    // .ec 模块文件用绿色图标
                    iconColor = node->isModuleLoaded ? Color(255, 80, 220, 100) : Color(255, 100, 180, 100);
                } else {
                    iconColor = Color(255, 80, 80, 80); // 普通文件灰色
                }
                SolidBrush iconBrush(iconColor);
                // 绘制文件图标占位方块
                graphics.FillRectangle(&iconBrush, (REAL)x, (REAL)y + 8, 14.0f, 14.0f);
                
                // 文件名（在图标后面）- 使用GDI的DrawTextW以支持emoji
                RECT textRect;
                textRect.left = x + 18;
                textRect.top = y + 5;
                textRect.right = rect.right - 60;
                textRect.bottom = y + g_ExplorerData.itemHeight;
                DrawTextW(hMemDC, node->name.c_str(), -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
                
                // 模块已加载标记
                if (node->IsModuleFile() && node->isModuleLoaded) {
                    // 在文件名右侧显示 [已加载] 标记
                    // 先测量文件名宽度
                    SIZE textSize;
                    GetTextExtentPoint32W(hMemDC, node->name.c_str(), (int)node->name.length(), &textSize);
                    
                    // 使用小号字体绘制标记
                    HFONT hSmallFont = CreateFontW(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft YaHei");
                    HFONT prevFont = (HFONT)SelectObject(hMemDC, hSmallFont);
                    SetTextColor(hMemDC, RGB(80, 220, 100));
                    
                    RECT labelRect;
                    labelRect.left = x + 18 + textSize.cx + 5;
                    labelRect.top = y + 7;
                    labelRect.right = labelRect.left + 60;
                    labelRect.bottom = y + g_ExplorerData.itemHeight;
                    DrawTextW(hMemDC, L"[已加载]", -1, &labelRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                    
                    SelectObject(hMemDC, prevFont);
                    DeleteObject(hSmallFont);
                    SetTextColor(hMemDC, g_CurrentTheme.text); // 恢复文本颜色
                }
                
                // 修改标记 (圆角矩形 + 圆点)
                if (node->isModified) {
                    float boxSize = 16.0f;
                    float boxX = rect.right - boxSize - 10;
                    float boxY = y + (g_ExplorerData.itemHeight - boxSize) / 2.0f;
                    
                    Pen pen(textColor, 1.0f);
                    RectF boxRect(boxX, boxY, boxSize, boxSize);
                    DrawRoundedRectangle(graphics, &pen, boxRect, 4.0f);
                    
                    float dotSize = 6.0f;
                    float dotX = boxX + (boxSize - dotSize) / 2.0f;
                    float dotY = boxY + (boxSize - dotSize) / 2.0f;
                    graphics.FillEllipse(&textBrush, dotX, dotY, dotSize, dotSize);
                }
                
                y += g_ExplorerData.itemHeight;
            }
            
            // 清理GDI字体资源
            SelectObject(hMemDC, oldFont);
            DeleteObject(hGdiFont);
            
            BitBlt(hdc, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);
            
            DeleteObject(hBitmap);
            DeleteDC(hMemDC);
            EndPaint(hWnd, &ps);
        }
        break;
        
    case WM_LBUTTONDOWN:
        {
            int y = HIWORD(lParam);
            size_t index = y / g_ExplorerData.itemHeight;
            
            if (index < g_ExplorerData.visibleNodes.size()) {
                FileNode* node = g_ExplorerData.visibleNodes[index];
                g_ExplorerData.selectedNode = node;
                
                // 检查是否点击了展开箭头区域
                int indentSize = 20;
                int x = LOWORD(lParam);
                int arrowX = 10 + node->level * indentSize - 12;
                
                // 如果点击了箭头区域或者是文件夹被点击
                if (node->isFolder && (x < arrowX + 20)) { // 简单判定点击左侧区域
                    node->isExpanded = !node->isExpanded;
                    g_ExplorerData.UpdateVisibleNodes();
                }
                
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        break;
        
    case WM_RBUTTONDOWN:
        {
            // 右键菜单
            int y = HIWORD(lParam);
            size_t index = y / g_ExplorerData.itemHeight;
            
            if (index < g_ExplorerData.visibleNodes.size()) {
                FileNode* node = g_ExplorerData.visibleNodes[index];
                
                // 选中该节点
                if (g_ExplorerData.selectedNode) {
                    g_ExplorerData.selectedNode->isSelected = false;
                }
                node->isSelected = true;
                g_ExplorerData.selectedNode = node;
                InvalidateRect(hWnd, NULL, TRUE);
                
                // 如果是.ec模块文件，显示模块操作菜单
                if (node->IsModuleFile()) {
                    HMENU hMenu = CreatePopupMenu();
                    
                    if (node->isModuleLoaded) {
                        AppendMenuW(hMenu, MF_STRING, 1, L"卸载模块");
                        AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
                        AppendMenuW(hMenu, MF_STRING, 2, L"重新加载模块");
                    } else {
                        AppendMenuW(hMenu, MF_STRING, 1, L"加载模块");
                    }
                    
                    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
                    AppendMenuW(hMenu, MF_STRING, 3, L"在文件管理器中打开");
                    
                    POINT pt;
                    GetCursorPos(&pt);
                    int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_LEFTALIGN | TPM_TOPALIGN, 
                                            pt.x, pt.y, 0, hWnd, NULL);
                    
                    switch (cmd) {
                        case 1: // 加载/卸载模块 - 已移除
                            /*
                            if (node->isModuleLoaded) {
                                ExplorerUnloadModule(node);
                            } else {
                                ExplorerLoadModule(node);
                            }
                            */
                            break;
                        case 2: // 重新加载 - 已移除
                            /*
                            ExplorerUnloadModule(node);
                            ExplorerLoadModule(node);
                            */
                            break;
                        case 3: // 在文件管理器中打开
                            {
                                std::wstring cmd = L"/select,\"" + node->path + L"\"";
                                ShellExecuteW(NULL, L"open", L"explorer.exe", cmd.c_str(), NULL, SW_SHOWNORMAL);
                            }
                            break;
                    }
                    
                    DestroyMenu(hMenu);
                }
            }
        }
        break;
        
    case WM_LBUTTONDBLCLK:
        {
            int y = HIWORD(lParam);
            size_t index = y / g_ExplorerData.itemHeight;
            
            if (index < g_ExplorerData.visibleNodes.size()) {
                FileNode* node = g_ExplorerData.visibleNodes[index];
                
                if (node->isFolder) {
                    node->isExpanded = !node->isExpanded;
                    g_ExplorerData.UpdateVisibleNodes();
                    InvalidateRect(hWnd, NULL, TRUE);
                } else if (node->IsModuleFile()) {
                    // 模块支持已移除
                    // ExplorerLoadModule(node);
                } else {
                    // 检查是否是时间线快照
                    if (node->parent && node->parent->path == L"TIMELINE_ROOT") {
                        // 恢复快照
                        if (g_EditorData) {
                            EditorDocument* doc = g_EditorData->GetActiveDoc();
                            if (doc) {
                                for (const auto& snap : doc->snapshots) {
                                    if (snap.id == node->path) {
                                        doc->RestoreSnapshot(snap);
                                        extern HWND hEditorWnd;
                                        if (hEditorWnd) InvalidateRect(hEditorWnd, NULL, TRUE);
                                        break;
                                    }
                                }
                            }
                        }
                    } else {
                        // 发送双击消息给父窗口
                        SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(GetWindowLong(hWnd, GWL_ID), LBN_DBLCLK), (LPARAM)hWnd);
                    }
                }
            }
        }
        break;
        
    case WM_ERASEBKGND:
        return 1;
        
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 文件系统监控线程
DWORD WINAPI FolderMonitorThread(LPVOID lpParam) {
    while (!g_ExplorerData.stopMonitoring) {
        if (g_ExplorerData.hDirChangeNotify != INVALID_HANDLE_VALUE) {
            DWORD waitStatus = WaitForSingleObject(g_ExplorerData.hDirChangeNotify, 500);
            
            if (waitStatus == WAIT_OBJECT_0) {
                // 文件系统变化，刷新资源管理器
                ExplorerRefreshFolder();
                
                // 继续监控
                FindNextChangeNotification(g_ExplorerData.hDirChangeNotify);
            }
        } else {
            Sleep(500);
        }
    }
    return 0;
}

// 递归添加文件夹中的所有文件
void AddFolderRecursive(const std::wstring& folderPath, FileNode* parentNode, int level) {
    namespace fs = std::filesystem;
    
    std::vector<FileNode*> folders;
    std::vector<FileNode*> files;
    
    try {
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            std::wstring entryPath = entry.path().wstring();
            std::wstring entryName = entry.path().filename().wstring();
            
            if (entry.is_directory()) {
                // 创建文件夹节点
                FileNode* folderNode = new FileNode(entryName, entryPath, true, level, parentNode);
                folders.push_back(folderNode);
                
                // 递归添加子文件夹
                AddFolderRecursive(entryPath, folderNode, level + 1);
            } else if (entry.is_regular_file()) {
                // 添加所有文件
                FileNode* fileNode = new FileNode(entryName, entryPath, false, level, parentNode);
                files.push_back(fileNode);
            }
        }
    } catch (...) {
        // 忽略访问错误
    }
    
    // 先添加文件夹，再添加文件
    if (parentNode) {
        for (auto folder : folders) {
            parentNode->children.push_back(folder);
        }
        for (auto file : files) {
            parentNode->children.push_back(file);
        }
    } else {
        for (auto folder : folders) {
            g_ExplorerData.rootNodes.push_back(folder);
        }
        for (auto file : files) {
            g_ExplorerData.rootNodes.push_back(file);
        }
    }
}

void ExplorerAddFolder(const std::wstring& folderPath) {
    // 清空现有内容
    for (auto node : g_ExplorerData.rootNodes) {
        delete node;
    }
    g_ExplorerData.rootNodes.clear();
    
    // 设置为工作区模式
    g_ExplorerData.isWorkspaceMode = true;
    g_ExplorerData.workspacePath = folderPath;
    
    // 提取文件夹名称
    size_t lastSlash = folderPath.find_last_of(L"\\/");
    std::wstring folderName = (lastSlash != std::wstring::npos) ? folderPath.substr(lastSlash + 1) : folderPath;
    
    // 创建根文件夹节点
    FileNode* rootFolder = new FileNode(folderName, folderPath, true, 0, nullptr);
    g_ExplorerData.rootNodes.push_back(rootFolder);
    
    // 递归添加文件夹内容
    AddFolderRecursive(folderPath, rootFolder, 1);
    
    g_ExplorerData.UpdateVisibleNodes();
    
    // 启动文件系统监控
    g_ExplorerData.stopMonitoring = false;
    g_ExplorerData.hDirChangeNotify = FindFirstChangeNotificationW(
        folderPath.c_str(),
        TRUE, // 监控子目录
        FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | 
        FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE
    );
    
    if (g_ExplorerData.hDirChangeNotify != INVALID_HANDLE_VALUE) {
        g_ExplorerData.hMonitorThread = CreateThread(NULL, 0, FolderMonitorThread, NULL, 0, NULL);
    }
    
    extern HWND hRightPanelWnd;
    if (hRightPanelWnd) {
        InvalidateRect(hRightPanelWnd, NULL, TRUE);
    }
}

void ExplorerCloseFolder() {
    // 停止监控
    g_ExplorerData.stopMonitoring = true;
    if (g_ExplorerData.hDirChangeNotify != INVALID_HANDLE_VALUE) {
        FindCloseChangeNotification(g_ExplorerData.hDirChangeNotify);
        g_ExplorerData.hDirChangeNotify = INVALID_HANDLE_VALUE;
    }
    if (g_ExplorerData.hMonitorThread) {
        WaitForSingleObject(g_ExplorerData.hMonitorThread, 1000);
        CloseHandle(g_ExplorerData.hMonitorThread);
        g_ExplorerData.hMonitorThread = NULL;
    }
    
    // 清空所有节点
    for (auto node : g_ExplorerData.rootNodes) {
        delete node;
    }
    g_ExplorerData.rootNodes.clear();
    
    // 退出工作区模式
    g_ExplorerData.isWorkspaceMode = false;
    g_ExplorerData.workspacePath = L"";
    
    g_ExplorerData.UpdateVisibleNodes();
    
    extern HWND hRightPanelWnd;
    if (hRightPanelWnd) {
        InvalidateRect(hRightPanelWnd, NULL, TRUE);
    }
}

void ExplorerRefreshFolder() {
    if (!g_ExplorerData.isWorkspaceMode || g_ExplorerData.workspacePath.empty()) {
        return;
    }
    
    // 保存当前展开状态
    std::map<std::wstring, bool> expandedState;
    std::function<void(FileNode*)> saveExpandState = [&](FileNode* node) {
        if (node->isFolder) {
            expandedState[node->path] = node->isExpanded;
            for (auto child : node->children) {
                saveExpandState(child);
            }
        }
    };
    for (auto node : g_ExplorerData.rootNodes) {
        saveExpandState(node);
    }
    
    // 保存工作区路径
    std::wstring folderPath = g_ExplorerData.workspacePath;
    
    // 清空并重新加载
    for (auto node : g_ExplorerData.rootNodes) {
        delete node;
    }
    g_ExplorerData.rootNodes.clear();
    
    // 提取文件夹名称
    size_t lastSlash = folderPath.find_last_of(L"\\/");
    std::wstring folderName = (lastSlash != std::wstring::npos) ? folderPath.substr(lastSlash + 1) : folderPath;
    
    // 创建根文件夹节点
    FileNode* rootFolder = new FileNode(folderName, folderPath, true, 0, nullptr);
    g_ExplorerData.rootNodes.push_back(rootFolder);
    
    // 递归添加文件夹内容
    AddFolderRecursive(folderPath, rootFolder, 1);
    
    // 恢复展开状态
    std::function<void(FileNode*)> restoreExpandState = [&](FileNode* node) {
        if (node->isFolder) {
            auto it = expandedState.find(node->path);
            if (it != expandedState.end()) {
                node->isExpanded = it->second;
            }
            for (auto child : node->children) {
                restoreExpandState(child);
            }
        }
    };
    for (auto node : g_ExplorerData.rootNodes) {
        restoreExpandState(node);
    }
    
    g_ExplorerData.UpdateVisibleNodes();
    
    extern HWND hRightPanelWnd;
    if (hRightPanelWnd) {
        InvalidateRect(hRightPanelWnd, NULL, TRUE);
    }
}

// 模块加载功能已移除

// 加载项目文件到资源管理器
void ExplorerLoadProject() {
    // 导入ProjectManager头文件
    extern class ProjectManager& GetProjectManager();
    auto& pm = ProjectManager::GetInstance();
    
    if (!pm.HasOpenProject()) {
        return;
    }
    
    const auto* project = pm.GetCurrentProject();
    if (!project) {
        return;
    }
    
    // 清空现有节点（保留时间线）
    for (auto it = g_ExplorerData.rootNodes.begin(); it != g_ExplorerData.rootNodes.end();) {
        if ((*it)->path == L"TIMELINE_ROOT") {
            ++it;
        } else {
            delete *it;
            it = g_ExplorerData.rootNodes.erase(it);
        }
    }
    
    // 设置项目模式
    g_ExplorerData.isProjectMode = true;
    g_ExplorerData.isWorkspaceMode = false;
    
    // 创建项目根节点
    FileNode* projectRoot = new FileNode(project->projectName, project->projectPath, true, 0, nullptr);
    projectRoot->isExpanded = true;
    
    // 按文件类型分组
    std::vector<ProjectFileItem> eycFiles;
    std::vector<ProjectFileItem> ellFiles;
    std::vector<ProjectFileItem> ecFiles;
    std::vector<ProjectFileItem> otherFiles;
    
    for (const auto& item : project->files) {
        switch (item.fileType) {
            case PROJECT_FILE_EYC: eycFiles.push_back(item); break;
            case PROJECT_FILE_ELL: ellFiles.push_back(item); break;
            case PROJECT_FILE_EC: ecFiles.push_back(item); break;
            default: otherFiles.push_back(item); break;
        }
    }
    
    // 添加源代码文件组
    if (!eycFiles.empty()) {
        FileNode* eycGroup = new FileNode(L"源代码文件", L"GROUP_EYC", true, 1, projectRoot);
        eycGroup->isExpanded = true;
        for (const auto& item : eycFiles) {
            std::wstring fullPath = pm.GetAbsolutePath(item.filePath);
            FileNode* fileNode = new FileNode(item.fileName, fullPath, false, 2, eycGroup);
            if (item.isMainFile) {
                fileNode->name = L"★ " + fileNode->name; // 主文件标记
            }
            eycGroup->children.push_back(fileNode);
        }
        projectRoot->children.push_back(eycGroup);
    }
    
    // 添加DLL声明文件组
    if (!ellFiles.empty()) {
        FileNode* ellGroup = new FileNode(L"DLL声明文件", L"GROUP_ELL", true, 1, projectRoot);
        ellGroup->isExpanded = true;
        for (const auto& item : ellFiles) {
            std::wstring fullPath = pm.GetAbsolutePath(item.filePath);
            FileNode* fileNode = new FileNode(item.fileName, fullPath, false, 2, ellGroup);
            ellGroup->children.push_back(fileNode);
        }
        projectRoot->children.push_back(ellGroup);
    }
    
    // 添加模块文件组
    if (!ecFiles.empty()) {
        FileNode* ecGroup = new FileNode(L"模块文件", L"GROUP_EC", true, 1, projectRoot);
        ecGroup->isExpanded = true;
        for (const auto& item : ecFiles) {
            std::wstring fullPath = pm.GetAbsolutePath(item.filePath);
            FileNode* fileNode = new FileNode(item.fileName, fullPath, false, 2, ecGroup);
            ecGroup->children.push_back(fileNode);
        }
        projectRoot->children.push_back(ecGroup);
    }
    
    // 添加其他文件组
    if (!otherFiles.empty()) {
        FileNode* otherGroup = new FileNode(L"其他文件", L"GROUP_OTHER", true, 1, projectRoot);
        otherGroup->isExpanded = true;
        for (const auto& item : otherFiles) {
            std::wstring fullPath = pm.GetAbsolutePath(item.filePath);
            FileNode* fileNode = new FileNode(item.fileName, fullPath, false, 2, otherGroup);
            otherGroup->children.push_back(fileNode);
        }
        projectRoot->children.push_back(otherGroup);
    }
    
    g_ExplorerData.rootNodes.insert(g_ExplorerData.rootNodes.begin(), projectRoot);
    
    g_ExplorerData.UpdateVisibleNodes();
    
    extern HWND hRightPanelWnd;
    if (hRightPanelWnd) {
        InvalidateRect(hRightPanelWnd, NULL, TRUE);
    }
}

// 关闭项目
void ExplorerCloseProject() {
    // 清空项目节点（保留时间线）
    for (auto it = g_ExplorerData.rootNodes.begin(); it != g_ExplorerData.rootNodes.end();) {
        if ((*it)->path == L"TIMELINE_ROOT") {
            ++it;
        } else {
            delete *it;
            it = g_ExplorerData.rootNodes.erase(it);
        }
    }
    
    g_ExplorerData.isProjectMode = false;
    g_ExplorerData.UpdateVisibleNodes();
    
    extern HWND hRightPanelWnd;
    if (hRightPanelWnd) {
        InvalidateRect(hRightPanelWnd, NULL, TRUE);
    }
}

