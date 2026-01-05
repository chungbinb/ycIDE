#pragma once
#include <windows.h>
#include <string>
#include <vector>

// DLL命令表结构
struct DllCommand {
    std::wstring commandName;      // 命令名
    std::wstring returnType;       // 返回值类型
    std::wstring dllFileName;      // DLL文件名
    std::wstring dllCommandName;   // 在DLL中对应命令名
    bool isPublic;                 // 是否公开
    std::wstring comment;          // 备注
    std::vector<int> paramLines;   // 参数行索引列表
};

// DLL参数结构
struct DllParameter {
    std::wstring paramName;        // 参数名
    std::wstring paramType;        // 参数类型
    bool byRef;                    // 是否传址/参考
    bool isArray;                  // 是否数组
    bool isOptional;               // 是否可空
    std::wstring comment;          // 备注
};

// .ell文档结构
struct EllDocument {
    std::vector<std::wstring> lines;
    int version;                   // 版本号
    std::vector<DllCommand> commands;
    std::vector<DllParameter> allParams;
    
    int cursorLine;
    int cursorCol;
    int scrollY;
    int scrollX;
    std::wstring filePath;
    std::wstring fileName;
    bool modified;
    HANDLE hFileLock;
    
    bool hasSelection;
    int selStartLine;
    int selStartCol;
    int selEndLine;
    int selEndCol;
    
    EllDocument();
    ~EllDocument();
    
    // 解析.ell文件内容
    void ParseContent();
    
    // 生成.ell文件内容
    std::wstring GenerateContent();
};

// .ell编辑器数据
struct EllEditorData {
    HWND hWnd;
    std::vector<EllDocument*> documents;
    int activeDocIndex;
    
    int fontSize;
    int rowHeight;
    int tabHeight;
    
    // 滚动条
    int scrollbarWidth;
    RECT vScrollbarRect;
    RECT vScrollThumbRect;
    RECT hScrollbarRect;
    RECT hScrollThumbRect;
    bool vScrollHover;
    bool hScrollHover;
    bool isDraggingVScroll;
    bool isDraggingHScroll;
    int dragStartY;
    int dragStartX;
    int dragStartScrollY;
    int dragStartScrollX;
    
    // 欢迎页
    bool showWelcomePage;
    
    EllEditorData(HWND hwnd);
    ~EllEditorData();
    
    EllDocument* GetActiveDoc();
    void AddDocument(const std::wstring& filePath);
    void CloseDocument(int index);
    void SetActiveDocument(int index);
};

// 注册.ell编辑器窗口类
bool RegisterEllEditorClass(HINSTANCE hInstance);

// 创建.ell编辑器窗口
HWND CreateEllEditorWindow(HINSTANCE hInstance, HWND hParent);

// .ell编辑器窗口过程
LRESULT CALLBACK EllEditorWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// 加载.ell文件
bool LoadEllFile(const std::wstring& path, EllDocument* doc);

// 保存.ell文件
bool SaveEllFile(const std::wstring& path, EllDocument* doc);

// 添加新的DLL命令
void AddDllCommand(EllDocument* doc);

// 删除DLL命令
void DeleteDllCommand(EllDocument* doc, int commandIndex);
