#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include "Keyword.h"

// Diff行类型
enum DiffLineType {
    DIFF_NORMAL = 0,   // 普通行
    DIFF_ADDED = 1,    // 新增行
    DIFF_DELETED = 2   // 删除行
};

struct Snapshot {
    std::wstring id;
    std::wstring name;
    std::vector<std::wstring> lines;
    long long timestamp;
    std::wstring description;
};

struct EditorDocument {
    std::vector<std::wstring> lines;
    int cursorLine;
    int cursorCol;
    int scrollY;
    int scrollX;
    std::wstring filePath;
    std::wstring fileName;
    bool modified;
    HANDLE hFileLock; // 文件独占锁句柄
    
    bool hasSelection;
    int selStartLine;
    int selStartCol;
    int selEndLine;
    int selEndCol;
    bool isSelecting;
    
    // 撤销/重做栈
    std::vector<Snapshot> undoStack;
    std::vector<Snapshot> redoStack;
    std::vector<Snapshot> snapshots; // 命名快照（用于时间线）
    
    // Diff视图相关
    bool showDiff;                           // 是否显示diff视图
    std::vector<DiffLineType> diffTypes;     // 每一行的diff类型
    std::vector<std::wstring> originalLines; // 原始内容（用于对比）
    
    // 用户交互状态
    bool hasUserInteraction;                 // 是否有过用户点击交互
    
    // 流程控制条件展开状态
    std::vector<bool> conditionExpanded;     // 每行是否展开了条件行
    
    EditorDocument();
    ~EditorDocument();
    
    void CreateSnapshot(const std::wstring& desc, bool isNamed = false);
    void RestoreSnapshot(const Snapshot& snapshot);
    void Undo();
    void Redo();
};

struct EditorData {
    HWND hWnd; // 编辑器窗口句柄
    std::vector<EditorDocument*> documents;
    int activeDocIndex;
    
    int fontSize;
    int rowHeight;
    int tabHeight;
    
    // 自绘滚动条相关
    int scrollbarWidth;      // 滚动条宽度
    RECT vScrollbarRect;     // 垂直滚动条区域
    RECT vScrollThumbRect;   // 垂直滚动条滑块区域
    RECT hScrollbarRect;     // 水平滚动条区域
    RECT hScrollThumbRect;   // 水平滚动条滑块区域
    bool isDraggingVScroll;  // 是否正在拖动垂直滚动条
    bool isDraggingHScroll;  // 是否正在拖动水平滚动条
    int dragStartY;          // 拖动开始位置
    int dragStartX;
    int dragStartScrollY;    // 拖动开始时的滚动位置
    int dragStartScrollX;
    bool vScrollHover;       // 垂直滚动条悬停状态
    bool hScrollHover;       // 水平滚动条悬停状态
    int hoverTabIndex;       // 鼠标悬停的标签索引（-1表示无）
    
    // 鼠标拖动选择检测
    int mouseDownX;          // 鼠标按下时的X坐标
    int mouseDownY;          // 鼠标按下时的Y坐标
    bool isDraggingSelection;// 是否正在拖动选择（已移动超过阈值）
    bool isLineNumberSelection; // 是否从行号区域开始选择（整行模式）
    
    // 自动补全相关
    bool showCompletion;     // 是否显示自动补全窗口
    int completionX;         // 补全窗口X坐标
    int completionY;         // 补全窗口Y坐标
    RECT completionRect;     // 补全窗口矩形区域
    int completionItemHeight; // 补全项高度
    int completionScrollOffset; // 补全列表滚动偏移
    int completionMaxVisible;   // 最多可见项数
    std::vector<CompletionItem> completionItems;  // 补全项列表
    int selectedCompletionIndex;  // 选中的补全项索引
    std::wstring currentWord;     // 当前正在输入的词
    
    // 自定义光标
    HCURSOR hRightArrowCursor;   // 右箭头光标（用于行号区域）
    
    // 欢迎页相关
    bool showWelcomePage;        // 是否显示欢迎页
    
    EditorData();
    ~EditorData();
    
    EditorDocument* GetActiveDoc();
    void AddDocument(const std::wstring& path);
    void CloseDocument(int index);
    void SwitchToDocument(int index);
};

extern EditorData* g_EditorData;

ATOM RegisterYiEditorClass(HINSTANCE hInstance);
LRESULT CALLBACK YiEditorWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
