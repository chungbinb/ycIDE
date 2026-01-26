#include "EllEditor.h"
#include <algorithm>

// EllEditorData 实现
EllEditorData::EllEditorData(HWND hWnd)
    : hwnd(hWnd)
    , context(EditorContext::GetInstance())
    , editor(nullptr)
    , editorType(-1)
{
}

EllEditorData::~EllEditorData()
{
    if (editor) {
        delete editor;
        editor = nullptr;
    }
}

bool EllEditorData::CreateEditorForFile(const std::wstring& filePath)
{
    // 删除旧的编辑器
    if (editor) {
        delete editor;
        editor = nullptr;
    }
    
    // 获取文件扩展名
    std::wstring ext;
    size_t dotPos = filePath.find_last_of(L'.');
    if (dotPos != std::wstring::npos) {
        ext = filePath.substr(dotPos);
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    }
    
    // 根据扩展名创建对应的编辑器
    if (ext == L".ell") {
        editor = new DllEditor(hwnd, context);
        editorType = 0;
        OutputDebugStringW(L"[EllEditor] 创建 DllEditor\n");
    } else if (ext == L".ecs") {
        editor = new ConstantEditor(hwnd, context);
        editorType = 1;
        OutputDebugStringW(L"[EllEditor] 创建 ConstantEditor\n");
    } else if (ext == L".egv") {
        editor = new GlobalVarEditor(hwnd, context);
        editorType = 2;
        OutputDebugStringW(L"[EllEditor] 创建 GlobalVarEditor\n");
    } else if (ext == L".edt") {
        editor = new DataTypeEditor(hwnd, context);
        editorType = 3;
        OutputDebugStringW(L"[EllEditor] 创建 DataTypeEditor\n");
    } else {
        OutputDebugStringW(L"[EllEditor] 未知文件类型，默认使用 DllEditor\n");
        editor = new DllEditor(hwnd, context);
        editorType = 0;
    }
    
    return editor != nullptr;
}

bool EllEditorData::LoadFile(const std::wstring& filePath)
{
    OutputDebugStringW(L"[EllEditorData::LoadFile] 开始加载文件\n");
    
    // 创建对应类型的编辑器
    if (!CreateEditorForFile(filePath)) {
        OutputDebugStringW(L"[EllEditorData::LoadFile] 创建编辑器失败\n");
        return false;
    }
    
    OutputDebugStringW(L"[EllEditorData::LoadFile] 编辑器创建成功\n");
    
    // 使用 TableEditor::LoadFile 加载文件
    bool result = editor->LoadFile(filePath);
    
    if (result) {
        currentFilePath = filePath;
        OutputDebugStringW(L"[EllEditorData::LoadFile] 加载成功\n");
        InvalidateRect(hwnd, NULL, TRUE);
    } else {
        OutputDebugStringW(L"[EllEditorData::LoadFile] 加载失败\n");
    }
    
    return result;
}

bool EllEditorData::SaveFile()
{
    if (currentFilePath.empty() || !editor) {
        return false;
    }
    return editor->SaveFile();
}

bool EllEditorData::SaveFileAs(const std::wstring& filePath)
{
    if (!editor) {
        return false;
    }
    
    bool result = editor->SaveFile(filePath);
    if (result) {
        currentFilePath = filePath;
    }
    return result;
}

// 注册表格编辑器窗口类
bool RegisterEllEditorClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = EllEditorWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = sizeof(LONG_PTR);  // 存储 EllEditorData 指针
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"EllEditor";
    
    return RegisterClassExW(&wcex) != 0;
}

// 创建表格编辑器窗口
HWND CreateEllEditorWindow(HINSTANCE hInstance, HWND hParent)
{
    return CreateWindowW(L"EllEditor", L"表格编辑器",
        WS_CHILD,
        0, 0, 0, 0, hParent, nullptr, hInstance, nullptr);
}

// 表格编辑器窗口过程
LRESULT CALLBACK EllEditorWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_CREATE) {
        OutputDebugStringW(L"[EllEditorWndProc] WM_CREATE - 创建窗口\n");
        // WM_CREATE 时创建 EllEditorData
        EllEditorData* data = new EllEditorData(hWnd);
        // 将 data 指针存储到 GWLP_USERDATA
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));
        OutputDebugStringW(L"[EllEditorWndProc] WM_CREATE - 完成\n");
        return 0;
    }
    
    // 从 GWLP_USERDATA 获取 data 指针
    EllEditorData* data = reinterpret_cast<EllEditorData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    
    // 调试输出
    if (message == WM_SIZE && data && data->editor) {
        RECT rect;
        GetClientRect(hWnd, &rect);
        wchar_t debugMsg[256];
        swprintf_s(debugMsg, L"[EllEditorWndProc] WM_SIZE - 窗口大小: %d x %d\n", 
                   rect.right - rect.left, rect.bottom - rect.top);
        OutputDebugStringW(debugMsg);
    }
    
    if (message == WM_PAINT) {
        RECT rect;
        GetClientRect(hWnd, &rect);
        wchar_t debugMsg[256];
        swprintf_s(debugMsg, L"[EllEditorWndProc] WM_PAINT - 窗口大小: %d x %d, editor: %p\n", 
                   rect.right - rect.left, rect.bottom - rect.top, data ? data->editor : nullptr);
        OutputDebugStringW(debugMsg);
    }
    
    if (message == WM_DESTROY) {
        OutputDebugStringW(L"[EllEditorWndProc] WM_DESTROY\n");
        if (data) {
            delete data;
        }
        SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
        return 0;
    }
    
    // 将所有消息转发给 TableEditor::WndProc
    // 注意：不再临时修改 GWLP_USERDATA，因为这会在 TrackPopupMenu 等
    // 有内部消息循环的函数中导致问题
    if (data && data->editor) {
        // 直接调用 TableEditor::WndProc，传递 editor 指针
        // 我们修改 TableEditor::WndProc 来接受额外参数或使用其他方式
        return TableEditor::WndProcWithEditor(hWnd, message, wParam, lParam, data->editor);
    }
    
    // 如果没有 editor，显示默认背景
    if (message == WM_PAINT) {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hWnd, &ps);
        return 0;
    }
    
    return DefWindowProc(hWnd, message, wParam, lParam);
}
