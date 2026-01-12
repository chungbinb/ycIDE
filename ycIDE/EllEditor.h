#pragma once
#include <windows.h>
#include <string>
#include "TableEditor.h"
#include "DllEditor.h"
#include "ConstantEditor.h"
#include "GlobalVarEditor.h"
#include "DataTypeEditor.h"
#include "EditorContext.h"

// 表格编辑器窗口数据（支持所有表格类型文件）
struct EllEditorData {
    HWND hwnd;
    EditorContext* context;
    TableEditor* editor;  // 指向具体的编辑器（DllEditor、ConstantEditor等）
    std::wstring currentFilePath;
    int editorType;  // 0=DLL, 1=Constant, 2=GlobalVar, 3=DataType
    
    EllEditorData(HWND hWnd);
    ~EllEditorData();
    
    // 根据文件扩展名创建对应的编辑器
    bool CreateEditorForFile(const std::wstring& filePath);
    
    bool LoadFile(const std::wstring& filePath);
    bool SaveFile();
    bool SaveFileAs(const std::wstring& filePath);
};

// 注册表格编辑器窗口类
bool RegisterEllEditorClass(HINSTANCE hInstance);

// 创建表格编辑器窗口
HWND CreateEllEditorWindow(HINSTANCE hInstance, HWND hParent);

// 表格编辑器窗口过程
LRESULT CALLBACK EllEditorWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
