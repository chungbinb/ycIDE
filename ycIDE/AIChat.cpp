#include "framework.h"
#include "AIChat.h"
#include "AIModel.h"
#include "Resource.h"
#include "Utils.h"
#include "Theme.h"
#include "YiEditor.h"
#include "ResourceExplorer.h"
#include <gdiplus.h>
#include <commctrl.h>
#include <windowsx.h>
#include <fstream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <Shlwapi.h>
#include <vector>
#include <algorithm>
#include <mutex>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "Shlwapi.lib")

using namespace Gdiplus;

extern HINSTANCE hInst;
extern INT_PTR CALLBACK ModelSettingsDlg(HWND, UINT, WPARAM, LPARAM);

// 全局变量：待处理的编辑列表
std::vector<FileEditInfo> g_PendingEdits;

// 调试日志互斥锁
std::mutex g_LogMutex;

// 写入调试日志
void WriteDebugLog(const wchar_t* message) {
    std::lock_guard<std::mutex> lock(g_LogMutex);
    
    // 创建 logs 文件夹（如果不存在）
    CreateDirectoryW(L"logs", NULL);
    
    // 生成日志文件名（使用当前日期）
    time_t now = time(nullptr);
    tm localTime;
    localtime_s(&localTime, &now);
    
    wchar_t filename[256];
    swprintf_s(filename, L"logs\\debug_%04d%02d%02d.log",
               localTime.tm_year + 1900,
               localTime.tm_mon + 1,
               localTime.tm_mday);
    
    // 追加写入日志
    std::wofstream file(filename, std::ios::app);
    if (file.is_open()) {
        // 写入时间戳
        wchar_t timestamp[64];
        swprintf_s(timestamp, L"[%02d:%02d:%02d] ",
                   localTime.tm_hour,
                   localTime.tm_min,
                   localTime.tm_sec);
        file << timestamp << message << std::endl;
        file.close();
    } else {
        // 如果打开失败，尝试使用绝对路径
        wchar_t absolutePath[MAX_PATH];
        GetCurrentDirectoryW(MAX_PATH, absolutePath);
        wcscat_s(absolutePath, L"\\logs\\");
        CreateDirectoryW(absolutePath, NULL);
        wcscat_s(absolutePath, L"debug_");
        wchar_t dateStr[32];
        swprintf_s(dateStr, L"%04d%02d%02d.log",
                   localTime.tm_year + 1900,
                   localTime.tm_mon + 1,
                   localTime.tm_mday);
        wcscat_s(absolutePath, dateStr);
        
        std::wofstream file2(absolutePath, std::ios::app);
        if (file2.is_open()) {
            wchar_t timestamp[64];
            swprintf_s(timestamp, L"[%02d:%02d:%02d] ",
                       localTime.tm_hour,
                       localTime.tm_min,
                       localTime.tm_sec);
            file2 << timestamp << message << std::endl;
            file2.close();
        }
    }
    
    // 同时输出到调试输出
    OutputDebugStringW(message);
    OutputDebugStringW(L"\n");
}

int g_CurrentChatMode = 0;
int g_CurrentModelIndex = 3;  // 默认选中DeepSeek(索引为3)
bool g_StopRequested = false;  // 停止AI返回的标志

// 自定义模式和模型选择区域
RECT g_ModeRect = {};
RECT g_ModelRect = {};
RECT g_AcceptBtnRect = {};
RECT g_UndoBtnRect = {};
RECT g_StatusBarRect = {};
bool g_FileListExpanded = false;  // 文件列表展开状态
const wchar_t* g_ModeText = L"Ask";
std::wstring g_ModelText = L"DeepSeek";

// 保存设置到文件
void SaveChatSettings() {
    std::wofstream file(L"chat_settings.ini");
    if (file.is_open()) {
        file << L"[Settings]" << std::endl;
        file << L"Mode=" << g_CurrentChatMode << std::endl;
        file << L"Model=" << g_CurrentModelIndex << std::endl;
        file.close();
        WriteDebugLog(L"[SaveChatSettings] Settings saved");
    }
}

// 加载设置从文件
void LoadChatSettings() {
    std::wifstream file(L"chat_settings.ini");
    if (file.is_open()) {
        std::wstring line;
        while (std::getline(file, line)) {
            if (line.find(L"Mode=") == 0) {
                g_CurrentChatMode = std::stoi(line.substr(5));
            } else if (line.find(L"Model=") == 0) {
                g_CurrentModelIndex = std::stoi(line.substr(6));
            }
        }
        file.close();
        
        // 更新模式文本
        const wchar_t* modes[] = {L"Ask", L"Edit", L"Agent"};
        if (g_CurrentChatMode >= 0 && g_CurrentChatMode < 3) {
            g_ModeText = modes[g_CurrentChatMode];
        }
        
        // 更新模型文本
        if (g_CurrentModelIndex >= 0 && g_CurrentModelIndex < (int)g_AIModels.size()) {
            g_ModelText = g_AIModels[g_CurrentModelIndex].name;
        }
        
        WriteDebugLog(L"[LoadChatSettings] Settings loaded");
    }
}

HWND hChatWindow = nullptr;
HWND hChatInput = nullptr;
HWND hChatSendBtn = nullptr;
HWND hChatModeCombo = nullptr;
HWND hChatModelCombo = nullptr;
HFONT hChatControlFont = nullptr;
WNDPROC g_OriginalEditProc = nullptr;
WNDPROC g_OriginalInputProc = nullptr;

// 输入框边框颜色（默认为编辑器边框颜色，焦点时为蓝色）
COLORREF g_InputBorderColor = RGB(0, 120, 215);
bool g_InputHasFocus = false;

// 菜单项数据结构
struct MenuItemData {
    std::wstring text;
    bool isSeparator;
    bool isChecked;
};

// 自定义弹出菜单窗口
HWND g_hCustomMenu = nullptr;
std::vector<MenuItemData> g_MenuItems;
int g_HoverMenuItemIndex = -1;
int g_MenuResult = 0;

HBITMAP g_hSendIcon = nullptr;
HBITMAP g_hPauseIcon = nullptr;
HBITMAP g_hRetryIcon = nullptr;
HBITMAP g_hCopyIcon = nullptr;
HBITMAP g_hUndoIcon = nullptr;
HBITMAP g_hCodeCopyIcon = nullptr;   // 代码块复制按钮
HBITMAP g_hCodeInsertIcon = nullptr; // 代码块插入按钮

struct AIRequestParam {
    HWND hChatWindow;
    std::wstring userMsg;
    int modelIndex;
    std::wstring mode;
};

// 流式回调函数
bool StreamCallbackFunc(const std::wstring& chunk, void* param) {
    HWND hWnd = (HWND)param;
    if (!IsWindow(hWnd)) return false;
    
    // 检查是否请求停止
    extern bool g_StopRequested;
    if (g_StopRequested) {
        WriteDebugLog(L"[StreamCallback] Stop requested, returning false to stop stream");
        return false;
    }
    
    // 记录接收到的数据块
    wchar_t logMsg[512];
    swprintf_s(logMsg, L"[StreamCallback] Received chunk, length: %d", (int)chunk.length());
    WriteDebugLog(logMsg);
    
    // 记录数据块内容（前100字符）
    std::wstring preview = chunk.substr(0, (std::min)(100, (int)chunk.length()));
    WriteDebugLog((L"[StreamCallback] Content preview: " + preview).c_str());
    
    std::wstring* pChunk = new std::wstring(chunk);
    PostMessageW(hWnd, WM_AI_STREAM_RESULT, 1, (LPARAM)pChunk); // wParam=1 表示数据块
    return true;
}

// 输入框子类化处理，用于处理焦点变化
LRESULT CALLBACK InputSubclassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_SETFOCUS:
        g_InputHasFocus = true;
        g_InputBorderColor = RGB(0, 120, 215);  // 焦点时为蓝色
        InvalidateRect(GetParent(hWnd), nullptr, FALSE);
        break;
        
    case WM_KILLFOCUS:
        g_InputHasFocus = false;
        g_InputBorderColor = g_CurrentTheme.grid;  // 失去焦点时为编辑器边框颜色
        InvalidateRect(GetParent(hWnd), nullptr, FALSE);
        break;
    }
    
    return CallWindowProc(g_OriginalInputProc, hWnd, message, wParam, lParam);
}

// 自定义菜单窗口过程
LRESULT CALLBACK CustomMenuWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_ERASEBKGND:
        return 1;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            RECT rect;
            GetClientRect(hWnd, &rect);
            
            // 填充背景
            HBRUSH hBgBrush = CreateSolidBrush(g_CurrentTheme.editorBg);
            FillRect(hdc, &rect, hBgBrush);
            DeleteObject(hBgBrush);
            
            // 绘制1像素边框
            HPEN hBorderPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.grid);
            HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));
            Rectangle(hdc, 0, 0, rect.right, rect.bottom);
            SelectObject(hdc, hOldPen);
            SelectObject(hdc, hOldBrush);
            DeleteObject(hBorderPen);
            
            // 绘制菜单项
            int y = 1;
            for (size_t i = 0; i < g_MenuItems.size(); i++) {
                const auto& item = g_MenuItems[i];
                int itemHeight = item.isSeparator ? 5 : 26;
                RECT itemRect = {1, y, rect.right - 1, y + itemHeight};
                
                if (item.isSeparator) {
                    // 绘制分隔符
                    HPEN hPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.grid);
                    HPEN hOldPen2 = (HPEN)SelectObject(hdc, hPen);
                    MoveToEx(hdc, itemRect.left + 5, (itemRect.top + itemRect.bottom) / 2, nullptr);
                    LineTo(hdc, itemRect.right - 5, (itemRect.top + itemRect.bottom) / 2);
                    SelectObject(hdc, hOldPen2);
                    DeleteObject(hPen);
                } else {
                    // 绘制悬停背景
                    if ((int)i == g_HoverMenuItemIndex) {
                        HBRUSH hHoverBrush = CreateSolidBrush(RGB(40, 40, 40));
                        FillRect(hdc, &itemRect, hHoverBrush);
                        DeleteObject(hHoverBrush);
                    }
                    
                    // 绘制选中标记
                    if (item.isChecked) {
                        SetBkMode(hdc, TRANSPARENT);
                        SetTextColor(hdc, g_CurrentTheme.text);
                        HFONT hOldFont = (HFONT)SelectObject(hdc, hChatControlFont);
                        RECT checkRect = itemRect;
                        checkRect.left += 5;
                        checkRect.right = checkRect.left + 15;
                        DrawTextW(hdc, L"✓", -1, &checkRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                        SelectObject(hdc, hOldFont);
                    }
                    
                    // 绘制文字
                    SetBkMode(hdc, TRANSPARENT);
                    SetTextColor(hdc, g_CurrentTheme.text);
                    HFONT hOldFont = (HFONT)SelectObject(hdc, hChatControlFont);
                    RECT textRect = itemRect;
                    textRect.left += 25;
                    DrawTextW(hdc, item.text.c_str(), -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                    SelectObject(hdc, hOldFont);
                }
                
                y += itemHeight;
            }
            
            EndPaint(hWnd, &ps);
        }
        return 0;
        
    case WM_MOUSEMOVE:
        {
            POINT pt;
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            
            // 计算悬停的菜单项
            int y = 1;
            int newHoverIndex = -1;
            for (size_t i = 0; i < g_MenuItems.size(); i++) {
                int itemHeight = g_MenuItems[i].isSeparator ? 5 : 26;
                if (pt.y >= y && pt.y < y + itemHeight && !g_MenuItems[i].isSeparator) {
                    newHoverIndex = (int)i;
                    break;
                }
                y += itemHeight;
            }
            
            if (newHoverIndex != g_HoverMenuItemIndex) {
                g_HoverMenuItemIndex = newHoverIndex;
                InvalidateRect(hWnd, nullptr, FALSE);
            }
        }
        return 0;
        
    case WM_LBUTTONDOWN:
        {
            if (g_HoverMenuItemIndex >= 0 && g_HoverMenuItemIndex < (int)g_MenuItems.size()) {
                g_MenuResult = g_HoverMenuItemIndex + 1;
                PostMessage(hWnd, WM_CLOSE, 0, 0);
            }
        }
        return 0;
        
    case WM_KILLFOCUS:
        PostMessage(hWnd, WM_CLOSE, 0, 0);
        return 0;
        
    case WM_CLOSE:
        DestroyWindow(hWnd);
        return 0;
        
    case WM_DESTROY:
        g_hCustomMenu = nullptr;
        return 0;
    }
    
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// 注册自定义菜单窗口类
void RegisterCustomMenuClass(HINSTANCE hInstance) {
    static bool registered = false;
    if (registered) return;
    
    WNDCLASSW wc = {};
    wc.lpfnWndProc = CustomMenuWndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = L"CustomMenuWindow";
    wc.style = CS_DROPSHADOW;
    RegisterClassW(&wc);
    
    registered = true;
}

// 显示自定义菜单
int ShowCustomMenu(HWND hParent, int x, int y, const std::vector<MenuItemData>& items) {
    g_MenuItems = items;
    g_HoverMenuItemIndex = -1;
    g_MenuResult = 0;
    
    // 计算菜单大小
    int width = 150;
    int height = 2;  // 上下各1像素边框
    for (const auto& item : items) {
        height += item.isSeparator ? 5 : 26;
    }
    
    // 创建菜单窗口
    g_hCustomMenu = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        L"CustomMenuWindow",
        nullptr,
        WS_POPUP,
        x, y, width, height,
        hParent,
        nullptr,
        hInst,
        nullptr
    );
    
    if (!g_hCustomMenu) return 0;
    
    ShowWindow(g_hCustomMenu, SW_SHOW);
    SetFocus(g_hCustomMenu);
    
    // 消息循环
    MSG msg;
    while (g_hCustomMenu && GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        
        if (!IsWindow(g_hCustomMenu)) break;
    }
    
    return g_MenuResult;
}

DWORD WINAPI AIRequestThreadProc(LPVOID lpParam) {
    WriteDebugLog(L"[AIRequestThread] Thread started");
    
    // 重置停止标志
    g_StopRequested = false;
    
    AIRequestParam* param = (AIRequestParam*)lpParam;
    if (!param) {
        WriteDebugLog(L"[AIRequestThread] ERROR: param is NULL");
        return 0;
    }

    int index = param->modelIndex;
    if (index < 0 || index >= (int)g_AIModels.size()) {
        wchar_t logMsg[256];
        swprintf_s(logMsg, L"[AIRequestThread] ERROR: Invalid model index: %d", index);
        WriteDebugLog(logMsg);
        delete param;
        return 0;
    }

    AIModel model = g_AIModels[index];  // 拷贝一份，避免线程安全问题
    
    wchar_t logMsg[512];
    swprintf_s(logMsg, L"[AIRequestThread] Model: %s, Mode: %s", model.name.c_str(), param->mode.c_str());
    WriteDebugLog(logMsg);
    
    // 构建完整的提示词，包含文件内容
    std::wstring fullPrompt = param->userMsg;
    
    // 如果有打开的文件，添加文件内容作为上下文
    extern EditorData* g_EditorData;
    int fileReadStartLine = 0;
    int fileReadEndLine = 0;
    std::wstring fileNameForDisplay;
    
    if (g_EditorData) {
        EditorDocument* doc = g_EditorData->GetActiveDoc();
        if (doc && !doc->filePath.empty() && doc->lines.size() > 0) {
            // 提取文件名
            std::wstring filePath = doc->filePath;
            size_t lastSlash = filePath.find_last_of(L"\\/");
            std::wstring fileName = (lastSlash != std::wstring::npos) ? filePath.substr(lastSlash + 1) : filePath;
            fileNameForDisplay = fileName;
            
            // 限制文件大小，避免内存溢出（最大1000行）
            size_t maxLines = (doc->lines.size() > 1000) ? 1000 : doc->lines.size();
            fileReadStartLine = 1;
            fileReadEndLine = (int)maxLines;
            
            // 构建文件内容
            std::wstring fileContent;
            fileContent.reserve(maxLines * 100);  // 预分配内存
            
            for (size_t i = 0; i < maxLines; i++) {
                // 额外检查：防止在线程执行过程中文档被修改
                if (i >= doc->lines.size()) {
                    fileReadEndLine = (int)i;
                    break;
                }
                if (fileContent.length() > 50000) {  // 限制最大50000字符
                    fileContent += L"\n... (内容过长，已截断)\n";
                    fileReadEndLine = (int)(i + 1);
                    break;
                }
                fileContent += doc->lines[i] + L"\n";
            }
            
            // 在用户消息前添加文件上下文
            std::wstring context = L"当前打开的文件: " + fileName + L"\n\n";
            context += L"文件内容:\n```eyc\n" + fileContent + L"```\n\n";
            context += L"用户问题: ";
            fullPrompt = context + param->userMsg;
            
            // 在Edit和Agent模式下，自动添加代码块要求
            if (param->mode == L"EDIT" || param->mode == L"AGENT") {
                fullPrompt += L"\n\n【重要指令】请务必遵守以下格式要求：\n";
                fullPrompt += L"1. 必须使用markdown代码块格式输出代码，格式为：```eyc\n代码内容\n```\n";
                fullPrompt += L"2. 必须输出完整的、可直接运行的代码文件，不要省略任何部分\n";
                fullPrompt += L"3. 不要使用 '...省略的代码...' 或类似的占位符\n";
                fullPrompt += L"4. 代码块中的内容将直接替换当前文件，请确保代码完整";
            }
            
            // 在Edit和Agent模式下，向AI消息添加文件读取信息块
            if (param->mode == L"EDIT" || param->mode == L"AGENT") {
                // 创建文件读取信息块
                MessageBlock readBlock(BLOCK_FILE_READ, L"");
                readBlock.fileName = fileName;
                readBlock.readStartLine = fileReadStartLine;
                readBlock.readEndLine = fileReadEndLine;
                
                // 使用PostMessage安全地添加到当前AI消息（避免跨线程直接访问）
                // 注意：这里我们不直接访问chatData，而是通过消息传递
                // 由于插入文件读取块需要同步操作，我们暂时保留直接访问，但添加更严格的检查
                if (IsWindow(param->hChatWindow)) {
                    AIChatData* chatData = (AIChatData*)GetWindowLongPtr(param->hChatWindow, GWLP_USERDATA);
                    if (chatData && chatData->isStreaming && chatData->streamingMsgIdx >= 0 && 
                        chatData->streamingMsgIdx < (int)chatData->messages.size() &&
                        !chatData->messages.empty()) {
                        ChatMessage& msg = chatData->messages[chatData->streamingMsgIdx];
                        msg.blocks.insert(msg.blocks.begin(), readBlock);
                        InvalidateRect(param->hChatWindow, nullptr, FALSE);
                    }
                }
            }
        }
    }
    
    // 记录完整提示词信息
    wchar_t promptLog[512];
    swprintf_s(promptLog, L"[AIRequestThread] Sending request, prompt length: %d characters", (int)fullPrompt.length());
    WriteDebugLog(promptLog);
    
    // 使用流式调用
    WriteDebugLog(L"[AIRequestThread] Calling CallAIAPI...");
    std::wstring result = CallAIAPI(fullPrompt, model, param->mode, StreamCallbackFunc, (void*)param->hChatWindow);
    
    wchar_t resultLog[512];
    swprintf_s(resultLog, L"[AIRequestThread] CallAIAPI returned, result length: %d", (int)result.length());
    WriteDebugLog(resultLog);
    
    // 如果有返回值（通常是错误信息），也作为流式内容发送
    if (!result.empty()) {
        StreamCallbackFunc(result, (void*)param->hChatWindow);
    }

    // 发送完成消息
    PostMessageW(param->hChatWindow, WM_AI_STREAM_RESULT, 2, 0); // wParam=2 表示完成

    delete param;
    return 0;
}

// 加载PNG图片
HBITMAP LoadPNGImage(const wchar_t* filename) {
    Bitmap* bitmap = new Bitmap(filename);
    if (bitmap->GetLastStatus() != Ok) {
        delete bitmap;
        return nullptr;
    }
    HBITMAP hBitmap;
    bitmap->GetHBITMAP(Color(0, 0, 0, 0), &hBitmap);
    delete bitmap;
    return hBitmap;
}

// 保存AI返回数据到日志文件
void SaveAIResponseLog(const std::wstring& response) {
    WriteDebugLog(L"[SaveAIResponseLog] Saving AI response to file");
    
    // 创建 logs 文件夹（如果不存在）
    CreateDirectoryW(L"logs", NULL);
    
    // 生成时间戳文件名
    time_t now = time(nullptr);
    tm localTime;
    localtime_s(&localTime, &now);
    
    wchar_t filename[256];
    swprintf_s(filename, L"logs\\ai_response_%04d%02d%02d_%02d%02d%02d.txt",
               localTime.tm_year + 1900,
               localTime.tm_mon + 1,
               localTime.tm_mday,
               localTime.tm_hour,
               localTime.tm_min,
               localTime.tm_sec);
    
    std::wstring logMsg = L"[SaveAIResponseLog] File: ";
    logMsg += filename;
    logMsg += L", Length: " + std::to_wstring(response.length());
    WriteDebugLog(logMsg.c_str());
    
    if (response.empty()) {
        WriteDebugLog(L"[SaveAIResponseLog] Response is empty, skipping save");
        return;
    }
    
    // 转换为 UTF-8 并保存
    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, response.c_str(), -1, NULL, 0, NULL, NULL);
    if (utf8Len > 0) {
        std::vector<char> utf8Buffer(utf8Len);
        WideCharToMultiByte(CP_UTF8, 0, response.c_str(), -1, &utf8Buffer[0], utf8Len, NULL, NULL);
        
        std::ofstream file(filename, std::ios::out | std::ios::binary);
        if (file.is_open()) {
            // 写入 UTF-8 BOM
            const unsigned char bom[] = {0xEF, 0xBB, 0xBF};
            file.write((const char*)bom, sizeof(bom));
            
            // 写入内容
            file.write(&utf8Buffer[0], utf8Len - 1);  // -1 去掉结尾的 null
            file.close();
            WriteDebugLog(L"[SaveAIResponseLog] Successfully saved AI response");
        } else {
            WriteDebugLog(L"[SaveAIResponseLog] Failed to open file for writing");
        }
    } else {
        WriteDebugLog(L"[SaveAIResponseLog] Failed to convert to UTF-8");
    }
}

// 简单的diff算法：比较两个文件内容并生成包含删除和新增行的完整diff视图
void ComputeDiff(const std::vector<std::wstring>& oldLines, 
                 const std::vector<std::wstring>& newLines,
                 std::vector<std::wstring>& mergedLines,
                 std::vector<DiffLineType>& diffTypes) {
    mergedLines.clear();
    diffTypes.clear();
    
    // 简单的逐行比较算法
    size_t oldIdx = 0;
    size_t newIdx = 0;
    
    while (oldIdx < oldLines.size() || newIdx < newLines.size()) {
        // 如果旧文件已经遍历完，剩余的都是新增行
        if (oldIdx >= oldLines.size()) {
            mergedLines.push_back(newLines[newIdx]);
            diffTypes.push_back(DIFF_ADDED);
            newIdx++;
            continue;
        }
        
        // 如果新文件已经遍历完，剩余的都是删除行
        if (newIdx >= newLines.size()) {
            mergedLines.push_back(oldLines[oldIdx]);
            diffTypes.push_back(DIFF_DELETED);
            oldIdx++;
            continue;
        }
        
        // 如果当前行相同，标记为正常
        if (oldLines[oldIdx] == newLines[newIdx]) {
            mergedLines.push_back(oldLines[oldIdx]);
            diffTypes.push_back(DIFF_NORMAL);
            oldIdx++;
            newIdx++;
        } else {
            // 行不同，需要判断是修改、删除还是新增
            // 简单策略：先添加删除的旧行，再添加新增的新行
            // 向前查找是否能匹配（最多查找5行）
            bool foundMatch = false;
            for (size_t lookAhead = 1; lookAhead <= 5 && newIdx + lookAhead < newLines.size(); lookAhead++) {
                if (oldLines[oldIdx] == newLines[newIdx + lookAhead]) {
                    // 找到匹配，说明中间是新增的行
                    for (size_t k = 0; k < lookAhead; k++) {
                        mergedLines.push_back(newLines[newIdx + k]);
                        diffTypes.push_back(DIFF_ADDED);
                    }
                    newIdx += lookAhead;
                    foundMatch = true;
                    break;
                }
            }
            
            if (!foundMatch) {
                // 向前查找旧行是否能匹配
                for (size_t lookAhead = 1; lookAhead <= 5 && oldIdx + lookAhead < oldLines.size(); lookAhead++) {
                    if (oldLines[oldIdx + lookAhead] == newLines[newIdx]) {
                        // 找到匹配，说明中间是删除的行
                        for (size_t k = 0; k < lookAhead; k++) {
                            mergedLines.push_back(oldLines[oldIdx + k]);
                            diffTypes.push_back(DIFF_DELETED);
                        }
                        oldIdx += lookAhead;
                        foundMatch = true;
                        break;
                    }
                }
            }
            
            if (!foundMatch) {
                // 没找到匹配，当前行被修改了（先删除后新增）
                mergedLines.push_back(oldLines[oldIdx]);
                diffTypes.push_back(DIFF_DELETED);
                mergedLines.push_back(newLines[newIdx]);
                diffTypes.push_back(DIFF_ADDED);
                oldIdx++;
                newIdx++;
            }
        }
    }
    
    // 输出调试信息
    int addedCount = 0, deletedCount = 0;
    for (auto type : diffTypes) {
        if (type == DIFF_ADDED) addedCount++;
        else if (type == DIFF_DELETED) deletedCount++;
    }
    wchar_t debugMsg[256];
    swprintf_s(debugMsg, L"[ComputeDiff] Total lines: %zu, Added: %d, Deleted: %d", 
              mergedLines.size(), addedCount, deletedCount);
    WriteDebugLog(debugMsg);
}

// 应用AI的代码修改
void ApplyAICodeChanges(const std::wstring& aiResponse, int msgIdx = -1) {
    extern EditorData* g_EditorData;
    extern HWND hEditorWnd;
    extern HWND hChatWindow;
    
    WriteDebugLog(L"[ApplyAICodeChanges] Function called");
    
    // 在聊天窗口显示"⏳ 正在应用代码修改..."状态
    if (hChatWindow && msgIdx >= 0) {
        AIChatData* chatData = (AIChatData*)GetWindowLongPtr(hChatWindow, GWLP_USERDATA);
        if (chatData && msgIdx < (int)chatData->messages.size()) {
            ChatMessage& msg = chatData->messages[msgIdx];
            // 添加临时状态块
            MessageBlock statusBlock(BLOCK_TEXT, L"⏳ 正在应用代码修改...");
            msg.blocks.push_back(statusBlock);
            InvalidateRect(hChatWindow, nullptr, FALSE);
            UpdateWindow(hChatWindow);
        }
    }
    
    if (!g_EditorData) {
        WriteDebugLog(L"[ApplyAICodeChanges] g_EditorData is NULL");
        return;
    }
    
    EditorDocument* doc = g_EditorData->GetActiveDoc();
    if (!doc) {
        WriteDebugLog(L"[ApplyAICodeChanges] No active document");
        return;
    }
    
    if (doc->filePath.empty()) {
        WriteDebugLog(L"[ApplyAICodeChanges] File path is empty");
        return;
    }
    
    std::wstring logMsg = L"[ApplyAICodeChanges] Active file: " + doc->filePath;
    WriteDebugLog(logMsg.c_str());
    
    // 查找代码块 ```eyc 或 ``` 
    size_t codeStart = aiResponse.find(L"```eyc");
    if (codeStart == std::wstring::npos) {
        codeStart = aiResponse.find(L"```");
    }
    
    if (codeStart == std::wstring::npos) {
        WriteDebugLog(L"[ApplyAICodeChanges] No code block found in AI response");
        
        // 记录AI响应的完整长度和前500个字符用于调试
        std::wstring lengthMsg = L"[ApplyAICodeChanges] AI Response length: " + std::to_wstring(aiResponse.length());
        WriteDebugLog(lengthMsg.c_str());
        
        if (aiResponse.length() > 0) {
            std::wstring preview = aiResponse.substr(0, std::min<size_t>(500, aiResponse.length()));
            std::wstring logMsg = L"[ApplyAICodeChanges] AI Response preview: " + preview;
            WriteDebugLog(logMsg.c_str());
        } else {
            WriteDebugLog(L"[ApplyAICodeChanges] AI Response is empty");
        }
        
        // 在聊天窗口添加提示信息
        if (hChatWindow && msgIdx >= 0) {
            AIChatData* chatData = (AIChatData*)GetWindowLongPtr(hChatWindow, GWLP_USERDATA);
            if (chatData && msgIdx < (int)chatData->messages.size()) {
                ChatMessage& msg = chatData->messages[msgIdx];
                
                // 添加提示文本块
                MessageBlock tipBlock(BLOCK_TEXT, L"⚠️ AI未返回可编辑的代码块。请在提示中明确要求AI生成完整代码。");
                msg.blocks.push_back(tipBlock);
                InvalidateRect(hChatWindow, nullptr, FALSE);
            }
        }
        return;
    }
    
    WriteDebugLog(L"[ApplyAICodeChanges] Code block found, processing...");
    
    wchar_t posMsg[256];
    swprintf_s(posMsg, L"[ApplyAICodeChanges] Code block starts at position: %zu", codeStart);
    WriteDebugLog(posMsg);
    
    if (codeStart != std::wstring::npos) {
        // 找到代码块结束位置
        size_t contentStart = aiResponse.find(L"\n", codeStart) + 1;
        size_t codeEnd = aiResponse.find(L"```", contentStart);
        
        wchar_t endMsg[256];
        swprintf_s(endMsg, L"[ApplyAICodeChanges] Content start: %zu, Code end: %zu", 
                  contentStart, codeEnd);
        WriteDebugLog(endMsg);
        
        if (codeEnd != std::wstring::npos && contentStart < codeEnd) {
            // 提取代码内容
            std::wstring newCode = aiResponse.substr(contentStart, codeEnd - contentStart);
            
            wchar_t codeMsg[256];
            swprintf_s(codeMsg, L"[ApplyAICodeChanges] Extracted code length: %zu", newCode.length());
            WriteDebugLog(codeMsg);
            
            // 记录代码前100字符
            std::wstring codePreview = newCode.substr(0, (std::min)(100, (int)newCode.length()));
            WriteDebugLog((L"[ApplyAICodeChanges] Code preview: " + codePreview).c_str());
            
            // 记录编辑信息
            FileEditInfo editInfo;
            editInfo.filePath = doc->filePath;
            
            // 提取文件名
            size_t lastSlash = doc->filePath.find_last_of(L"\\/");
            editInfo.fileName = (lastSlash != std::wstring::npos) ? 
                               doc->filePath.substr(lastSlash + 1) : doc->filePath;
            
            // 保存原始内容
            editInfo.startLine = 1;
            editInfo.endLine = (int)doc->lines.size();
            editInfo.originalContent = doc->lines;
            
            // 解析新代码为行
            std::vector<std::wstring> newLines;
            std::wstringstream ss(newCode);
            std::wstring line;
            while (std::getline(ss, line)) {
                // 移除行尾的 \r
                if (!line.empty() && line.back() == L'\r') {
                    line.pop_back();
                }
                newLines.push_back(line);
            }
            
            // 确保至少有一行
            if (newLines.empty()) {
                newLines.push_back(L"");
            }
            
            editInfo.newContent = newLines;
            
            // 计算diff并统计实际的增删行数
            std::vector<std::wstring> mergedLines;
            std::vector<DiffLineType> diffTypes;
            ComputeDiff(editInfo.originalContent, newLines, mergedLines, diffTypes);
            
            // 统计diffTypes中的增加和删除行数
            int addedCount = 0;
            int deletedCount = 0;
            for (DiffLineType type : diffTypes) {
                if (type == DIFF_ADDED) addedCount++;
                else if (type == DIFF_DELETED) deletedCount++;
            }
            
            editInfo.addedLines = addedCount;
            editInfo.deletedLines = deletedCount;
            
            wchar_t linesMsg[256];
            swprintf_s(linesMsg, L"[ApplyAICodeChanges] Lines: deleted=%d, added=%d", 
                      editInfo.deletedLines, editInfo.addedLines);
            WriteDebugLog(linesMsg);
            
            // 保存原始内容用于diff视图
            doc->originalLines = doc->lines;
            
            // 应用到编辑器（显示包含删除行的diff视图）
            doc->lines = mergedLines;
            doc->diffTypes = diffTypes;
            doc->cursorLine = 0;
            doc->cursorCol = 0;
            doc->scrollX = 0;
            doc->scrollY = 0;
            doc->modified = true;
            editInfo.applied = true;
            
            WriteDebugLog(L"[ApplyAICodeChanges] Code applied to document with diff view");
            
            // 启用diff视图
            doc->showDiff = true;
            
            WriteDebugLog(L"[ApplyAICodeChanges] Diff computed");
            
            // 添加到待处理列表
            g_PendingEdits.push_back(editInfo);
            int editIndex = (int)g_PendingEdits.size() - 1;
            
            wchar_t editMsg[256];
            swprintf_s(editMsg, L"[ApplyAICodeChanges] Added to pending edits, index=%d, total=%zu", 
                      editIndex, g_PendingEdits.size());
            WriteDebugLog(editMsg);
            
            // 刷新编辑器显示
            if (hEditorWnd) {
                InvalidateRect(hEditorWnd, NULL, TRUE);
                UpdateWindow(hEditorWnd);
                WriteDebugLog(L"[ApplyAICodeChanges] Editor updated");
            }
            
            // 刷新父窗口以显示状态栏
            if (hChatWindow) {
                HWND hParent = GetParent(hChatWindow);
                if (hParent) {
                    // 发送WM_SIZE消息重新布局
                    RECT parentRect;
                    GetClientRect(hParent, &parentRect);
                    SendMessage(hParent, WM_SIZE, 0, MAKELPARAM(parentRect.right, parentRect.bottom));
                    
                    // 刷新父窗口
                    InvalidateRect(hParent, NULL, TRUE);
                    UpdateWindow(hParent);
                    WriteDebugLog(L"[ApplyAICodeChanges] Parent window updated to show status bar");
                }
            }
            
            // 更新资源管理器的修改状态
            ExplorerSetFileModified(doc->filePath, true);
            
            std::wstring logMsg = L"[ApplyAICodeChanges] Code applied - Added: " + 
                                 std::to_wstring(editInfo.addedLines) + 
                                 L", Deleted: " + std::to_wstring(editInfo.deletedLines);
            WriteDebugLog(logMsg.c_str());
            WriteDebugLog(L"[ApplyAICodeChanges] Creating edit info block");
            
            // 向AI消息添加编辑信息块（使用传入的消息索引）
            if (hChatWindow && msgIdx >= 0) {
                AIChatData* chatData = (AIChatData*)GetWindowLongPtr(hChatWindow, GWLP_USERDATA);
                if (chatData && msgIdx < (int)chatData->messages.size() &&
                    !chatData->messages.empty()) {
                    ChatMessage& msg = chatData->messages[msgIdx];
                    
                    // 移除临时的"正在应用代码修改..."状态块（如果存在）
                    for (auto it = msg.blocks.begin(); it != msg.blocks.end(); ) {
                        if (it->type == BLOCK_TEXT && 
                            (it->content.find(L"⏳ 正在应用代码修改") != std::wstring::npos ||
                             it->content.find(L"⏳ 正在编辑文件") != std::wstring::npos)) {
                            it = msg.blocks.erase(it);
                        } else {
                            ++it;
                        }
                    }
                    
                    // 创建编辑信息块
                    MessageBlock editBlock(BLOCK_EDIT_INFO, L"");
                    editBlock.fileName = editInfo.fileName;
                    editBlock.editIndex = editIndex;
                    editBlock.addedLines = editInfo.addedLines;
                    editBlock.deletedLines = editInfo.deletedLines;
                    
                    // 添加到消息块列表
                    msg.blocks.push_back(editBlock);
                    InvalidateRect(hChatWindow, nullptr, FALSE);
                    WriteDebugLog(L"[ApplyAICodeChanges] Edit info block added to chat");
                }
            } else {
                WriteDebugLog(L"[ApplyAICodeChanges] Failed to add edit info block: invalid msgIdx or chatData");
            }
        }
    }
}

// 解析消息为文本块和代码块
void ParseMessageBlocks(const std::wstring& content, std::vector<MessageBlock>& blocks, bool skipCodeBlocks) {
    blocks.clear();
    size_t pos = 0;
    
    while (pos < content.length()) {
        size_t mdStart = content.find(L"```", pos);
        
        // 查找 EPL 代码块起始 (.版本)
        // 我们只在行首查找 .版本
        size_t eplStart = std::wstring::npos;
        size_t searchPos = pos;
        while (true) {
            size_t found = content.find(L".版本", searchPos);
            if (found == std::wstring::npos) break;
            
            // 检查是否在行首
            if (found == 0 || content[found - 1] == L'\n') {
                eplStart = found;
                break;
            }
            searchPos = found + 1;
        }
        
        // 确定哪个先出现
        bool isMd = false;
        size_t codeStart = std::wstring::npos;
        
        if (mdStart != std::wstring::npos && eplStart != std::wstring::npos) {
            if (mdStart < eplStart) {
                codeStart = mdStart;
                isMd = true;
            } else {
                codeStart = eplStart;
                isMd = false;
            }
        } else if (mdStart != std::wstring::npos) {
            codeStart = mdStart;
            isMd = true;
        } else if (eplStart != std::wstring::npos) {
            codeStart = eplStart;
            isMd = false;
        }
        
        if (codeStart == std::wstring::npos) {
            // 没有更多代码块
            if (pos < content.length()) {
                blocks.push_back(MessageBlock(BLOCK_TEXT, content.substr(pos)));
            }
            break;
        }
        
        // 添加代码块前的文本
        if (codeStart > pos) {
            blocks.push_back(MessageBlock(BLOCK_TEXT, content.substr(pos, codeStart - pos)));
        }
        
        if (isMd) {
            // Markdown 代码块
            size_t lineEnd = content.find(L"\n", codeStart);
            if (lineEnd == std::wstring::npos) lineEnd = content.length();
            
            std::wstring lang;
            if (lineEnd > codeStart + 3) {
                lang = content.substr(codeStart + 3, lineEnd - (codeStart + 3));
                // Trim
                size_t first = lang.find_first_not_of(L" \t\r");
                if (first != std::wstring::npos) {
                    size_t last = lang.find_last_not_of(L" \t\r");
                    lang = lang.substr(first, last - first + 1);
                } else {
                    lang = L"";
                }
            }
            
            size_t blockEnd = content.find(L"```", lineEnd);
            if (blockEnd == std::wstring::npos) blockEnd = content.length();
            
            std::wstring code;
            if (lineEnd < blockEnd) {
                code = content.substr(lineEnd + 1, blockEnd - (lineEnd + 1));
            }
            
            // 检查是否为 EPL 代码
            bool isEpl = (lang == L"e" || lang == L"epl" || lang == L"易语言");
            if (!isEpl && code.find(L".版本") != std::wstring::npos) {
                isEpl = true;
            }
            
            if (!skipCodeBlocks) {
                if (isEpl) {
                    // 转换为内部格式
                    std::wstring converted = ConvertEPLToInternal(code);
                    blocks.push_back(MessageBlock(BLOCK_CODE, converted));
                } else {
                    blocks.push_back(MessageBlock(BLOCK_CODE, code));
                }
            }
            
            pos = blockEnd + 3;
        } else {
            // 裸 EPL 代码块
            // 寻找结束位置：遇到下一个 ``` 或者 连续两个换行后接非代码行
            size_t blockEnd = content.length();
            size_t nextMd = content.find(L"```", codeStart);
            if (nextMd != std::wstring::npos) blockEnd = nextMd;
            
            // 扫描寻找非代码行
            size_t current = codeStart;
            bool lastWasEmpty = false;
            
            while (current < blockEnd) {
                size_t lineEnd = content.find(L"\n", current);
                if (lineEnd == std::wstring::npos) lineEnd = blockEnd;
                if (lineEnd > blockEnd) lineEnd = blockEnd;
                
                std::wstring line = content.substr(current, lineEnd - current);
                // Trim
                size_t first = line.find_first_not_of(L" \t\r");
                if (first == std::wstring::npos) {
                    lastWasEmpty = true;
                } else {
                    std::wstring trimmed = line.substr(first);
                    bool looksLikeCode = false;
                    if (trimmed[0] == L'.') looksLikeCode = true;
                    else if (trimmed.find(L"版本") == 0) looksLikeCode = true;
                    else if (trimmed.find(L"程序集") == 0) looksLikeCode = true;
                    else if (trimmed.find(L"子程序") == 0) looksLikeCode = true;
                    else if (trimmed.find(L"局部变量") == 0) looksLikeCode = true;
                    else if (trimmed.find(L"如果") == 0) looksLikeCode = true;
                    else if (trimmed.find(L"返回") == 0) looksLikeCode = true;
                    else if (trimmed.find(L"=") != std::wstring::npos) looksLikeCode = true; // 赋值
                    
                    if (lastWasEmpty && !looksLikeCode) {
                        // 空行后接非代码行，认为是结束
                        blockEnd = current;
                        break;
                    }
                    lastWasEmpty = false;
                }
                
                current = lineEnd + 1;
            }
            
            std::wstring code = content.substr(codeStart, blockEnd - codeStart);
            // 修剪末尾空白
            size_t lastChar = code.find_last_not_of(L" \t\r\n");
            if (lastChar != std::wstring::npos) {
                code = code.substr(0, lastChar + 1);
            }
            
            if (!skipCodeBlocks) {
                std::wstring converted = ConvertEPLToInternal(code);
                blocks.push_back(MessageBlock(BLOCK_CODE, converted));
            }
            
            pos = blockEnd;
        }
    }
}

// 添加用户消息
void AddUserMessage(AIChatData* data, const std::wstring& content) {
    ChatMessage msg(true);
    ParseMessageBlocks(content, msg.blocks);
    data->messages.push_back(msg);
}

// 开始AI消息(流式输出)
void StartAIMessage(AIChatData* data) {
    data->isStreaming = true;
    data->streamBuffer.clear();
    data->messages.push_back(ChatMessage(false));
    data->streamingMsgIdx = (int)data->messages.size() - 1;
}

// 追加AI消息流
void AppendAIMessageStream(AIChatData* data, const std::wstring& chunk) {
    if (!data->isStreaming || data->streamingMsgIdx < 0 || 
        data->streamingMsgIdx >= (int)data->messages.size()) {
        return;
    }
    
    data->streamBuffer += chunk;
    
    // 检查是否为Edit模式，如果是则跳过代码块的显示
    bool isEditMode = (wcscmp(g_ModeText, L"Edit") == 0);
    
    // 重新解析消息块
    ChatMessage& msg = data->messages[data->streamingMsgIdx];
    ParseMessageBlocks(data->streamBuffer, msg.blocks, isEditMode);
}

// 完成AI消息
void FinishAIMessage(AIChatData* data) {
    data->isStreaming = false;
    
    // 删除"正在运行中"的提示
    if (data->streamingMsgIdx >= 0 && data->streamingMsgIdx < (int)data->messages.size()) {
        ChatMessage& msg = data->messages[data->streamingMsgIdx];
        // 遍历blocks，删除包含"正在运行中"的块
        for (auto it = msg.blocks.begin(); it != msg.blocks.end(); ) {
            if (it->type == BLOCK_TEXT && it->content.find(L"⏳ 正在运行中") != std::wstring::npos) {
                it = msg.blocks.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    data->streamingMsgIdx = -1;
    data->streamBuffer.clear();
}

// 注册AI聊天窗口类
ATOM RegisterAIChatWindowClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = AIChatWindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = g_CurrentTheme.hBgBrush;  // 使用主题背景色
    wcex.lpszClassName = L"AIChatWindow";
    
    return RegisterClassExW(&wcex);
}

// AI聊天窗口过程
LRESULT CALLBACK AIChatWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    AIChatData* data = (AIChatData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    
    switch (message) {
    case WM_NCHITTEST:
        {
            // 让左边缘5px的鼠标消息穿透到主窗口
            POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            ScreenToClient(hWnd, &pt);
            if (pt.x < 5) {
                return HTTRANSPARENT;
            }
            return HTCLIENT;
        }
    case WM_CREATE:
        {
            data = new AIChatData();
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)data);
            
            // 创建动画定时器（每200ms更新一次）
            SetTimer(hWnd, 1, 200, NULL);
            
            // 创建 Tooltip 控件
            data->hTooltip = CreateWindowExW(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
                                            WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
                                            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                            hWnd, NULL, hInst, NULL);
            
            SetWindowPos(data->hTooltip, HWND_TOPMOST, 0, 0, 0, 0,
                        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
            
            // 添加欢迎消息
            AddUserMessage(data, L"你好!");
            StartAIMessage(data);
            AppendAIMessageStream(data, L"你好!我是AI助手,有什么可以帮你的吗?");
            FinishAIMessage(data);
        }
        return 0;
        
    case WM_ERASEBKGND:
        return 1;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hRealDC = BeginPaint(hWnd, &ps);
            
            RECT rect;
            GetClientRect(hWnd, &rect);
            
            // 双缓冲绘图
            HDC hdc = CreateCompatibleDC(hRealDC);
            HBITMAP hBitmap = CreateCompatibleBitmap(hRealDC, rect.right, rect.bottom);
            HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdc, hBitmap);

            // 背景
            HBRUSH hBgBrush = CreateSolidBrush(g_CurrentTheme.bg);
            FillRect(hdc, &rect, hBgBrush);
            DeleteObject(hBgBrush);
            
            if (!data) {
                SelectObject(hdc, hOldBitmap);
                DeleteObject(hBitmap);
                DeleteDC(hdc);
                EndPaint(hWnd, &ps);
                return 0;
            }
            
            // 设置字体
            HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
            HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
            SetBkMode(hdc, TRANSPARENT);
            
            int yPos = 10 - data->scrollY;
            int padding = 15;
            int bubblePadding = 12;
            int maxWidth = rect.right - 100;
            
            // 绘制每条消息
            for (size_t i = 0; i < data->messages.size(); i++) {
                ChatMessage& msg = data->messages[i];
                
                // 绘制每个消息块
                for (size_t j = 0; j < msg.blocks.size(); j++) {
                    MessageBlock& block = msg.blocks[j];
                    
                    if (block.type == BLOCK_FILE_READ) {
                        // 绘制文件读取信息块：查看 文件名 第X-Y行
                        int blockHeight = 40;  // 增加高度从30到40
                        RECT readRect;
                        readRect.left = padding;
                        readRect.right = rect.right - padding;
                        readRect.top = yPos;
                        readRect.bottom = yPos + blockHeight;
                        
                        // 设置文字
                        SetBkMode(hdc, TRANSPARENT);
                        SetTextColor(hdc, RGB(120, 120, 120));  // 稍微亮一点的灰色
                        HFONT hReadFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,  // 字体从14增加到16
                            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
                        HFONT hOldReadFont = (HFONT)SelectObject(hdc, hReadFont);
                        
                        // 绘制文件图标（灰色小方块）
                        int iconSize = 16;
                        int iconX = readRect.left + 4;
                        int iconY = readRect.top + (blockHeight - iconSize) / 2;
                        HBRUSH hIconBrush = CreateSolidBrush(RGB(100, 100, 100));
                        RECT iconRect = {iconX, iconY, iconX + iconSize, iconY + iconSize};
                        FillRect(hdc, &iconRect, hIconBrush);
                        DeleteObject(hIconBrush);
                        
                        // 构建显示文本："查看 文件名 第X-Y行"
                        std::wstring readText = L"查看 ";
                        readText += block.fileName;
                        readText += L" 第";
                        readText += std::to_wstring(block.readStartLine);
                        readText += L"-";
                        readText += std::to_wstring(block.readEndLine);
                        readText += L"行";
                        
                        // 在图标右侧绘制文字
                        RECT textRect = readRect;
                        textRect.left = iconX + iconSize + 8;
                        DrawTextW(hdc, readText.c_str(), -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                        
                        SelectObject(hdc, hOldReadFont);
                        DeleteObject(hReadFont);
                        
                        yPos = readRect.bottom + 10;
                        
                    } else if (block.type == BLOCK_EDIT_INFO) {
                        // 绘制编辑信息块：类似 VS Code 的编辑摘要卡片
                        int blockHeight = 60;
                        RECT editRect;
                        editRect.left = padding;
                        editRect.right = rect.right - padding;
                        editRect.top = yPos;
                        editRect.bottom = yPos + blockHeight;
                        
                        // 绘制方框背景和边框（更现代的样式）
                        HBRUSH hEditBrush = CreateSolidBrush(RGB(248, 249, 250));  // 浅灰背景
                        HBRUSH hOldEditBrush = (HBRUSH)SelectObject(hdc, hEditBrush);
                        HPEN hEditPen = CreatePen(PS_SOLID, 1, RGB(206, 212, 218));  // 边框颜色
                        HPEN hOldEditPen = (HPEN)SelectObject(hdc, hEditPen);
                        
                        RoundRect(hdc, editRect.left, editRect.top, editRect.right, editRect.bottom, 6, 6);
                        
                        SelectObject(hdc, hOldEditPen);
                        SelectObject(hdc, hOldEditBrush);
                        DeleteObject(hEditPen);
                        DeleteObject(hEditBrush);
                        
                        // 绘制文件类型图标（更大的文档图标）
                        int iconSize = 20;
                        int iconX = editRect.left + 15;
                        int iconY = editRect.top + (blockHeight - iconSize) / 2;
                        
                        // 绘制文档图标外框
                        HPEN hIconPen = CreatePen(PS_SOLID, 2, RGB(108, 117, 125));
                        HPEN hOldIconPen = (HPEN)SelectObject(hdc, hIconPen);
                        HBRUSH hIconBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
                        HBRUSH hOldIconBrush = (HBRUSH)SelectObject(hdc, hIconBrush);
                        
                        Rectangle(hdc, iconX, iconY, iconX + iconSize, iconY + iconSize);
                        
                        // 绘制文档图标内部横线
                        MoveToEx(hdc, iconX + 4, iconY + 6, NULL);
                        LineTo(hdc, iconX + iconSize - 4, iconY + 6);
                        MoveToEx(hdc, iconX + 4, iconY + 10, NULL);
                        LineTo(hdc, iconX + iconSize - 4, iconY + 10);
                        MoveToEx(hdc, iconX + 4, iconY + 14, NULL);
                        LineTo(hdc, iconX + iconSize - 4, iconY + 14);
                        
                        SelectObject(hdc, hOldIconPen);
                        SelectObject(hdc, hOldIconBrush);
                        DeleteObject(hIconPen);
                        
                        // 绘制文本内容
                        SetBkMode(hdc, TRANSPARENT);
                        
                        // 文件名（粗体）
                        HFONT hNameFont = CreateFont(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
                        HFONT hOldNameFont = (HFONT)SelectObject(hdc, hNameFont);
                        
                        SetTextColor(hdc, RGB(33, 37, 41));  // 深色文字
                        RECT nameRect = editRect;
                        nameRect.left = iconX + iconSize + 12;
                        nameRect.right = editRect.right - 15;
                        nameRect.top = editRect.top + 12;
                        nameRect.bottom = editRect.top + 28;
                        DrawTextW(hdc, block.fileName.c_str(), -1, &nameRect, DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS);
                        
                        SelectObject(hdc, hOldNameFont);
                        DeleteObject(hNameFont);
                        
                        // 增删统计（较小字体）
                        HFONT hStatsFont = CreateFont(13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
                        HFONT hOldStatsFont = (HFONT)SelectObject(hdc, hStatsFont);
                        
                        RECT statsRect = editRect;
                        statsRect.left = nameRect.left;
                        statsRect.top = editRect.top + 32;
                        statsRect.bottom = editRect.bottom - 10;
                        
                        // 绘制 "Edited" 标签
                        SetTextColor(hdc, RGB(108, 117, 125));  // 灰色
                        DrawTextW(hdc, L"Edited ", -1, &statsRect, DT_LEFT | DT_SINGLELINE);
                        
                        SIZE editedSize;
                        GetTextExtentPoint32W(hdc, L"Edited ", 7, &editedSize);
                        
                        // 绘制增加部分（绿色）
                        statsRect.left += editedSize.cx;
                        SetTextColor(hdc, RGB(40, 167, 69));  // Bootstrap success 绿色
                        std::wstring addText = L"+";
                        addText += std::to_wstring(block.addedLines);
                        SIZE addSize;
                        GetTextExtentPoint32W(hdc, addText.c_str(), (int)addText.length(), &addSize);
                        DrawTextW(hdc, addText.c_str(), -1, &statsRect, DT_LEFT | DT_SINGLELINE);
                        
                        // 绘制删除部分（红色）
                        statsRect.left += addSize.cx + 8;
                        SetTextColor(hdc, RGB(220, 53, 69));  // Bootstrap danger 红色
                        std::wstring delText = L"-";
                        delText += std::to_wstring(block.deletedLines);
                        DrawTextW(hdc, delText.c_str(), -1, &statsRect, DT_LEFT | DT_SINGLELINE);
                        
                        SelectObject(hdc, hOldStatsFont);
                        DeleteObject(hStatsFont);
                        
                        // 保存矩形区域，用于点击检测
                        block.rect = editRect;
                        
                        yPos = editRect.bottom + 10;
                        
                    } else if (block.type == BLOCK_TEXT) {
                        // 计算文本高度
                        RECT calcRect = {0, 0, maxWidth - bubblePadding * 2, 0};
                        DrawTextW(hdc, block.content.c_str(), -1, &calcRect, DT_WORDBREAK | DT_CALCRECT);
                        int textHeight = calcRect.bottom - calcRect.top;
                        
                        RECT bubbleRect;
                        if (msg.isUser) {
                            // 用户消息：右对齐，带气泡
                            bubbleRect.left = rect.right - maxWidth - padding;
                            bubbleRect.right = rect.right - padding;
                            bubbleRect.top = yPos;
                            bubbleRect.bottom = yPos + textHeight + bubblePadding * 2;
                            
                            // 绘制用户消息气泡 RGB(28, 41, 53)
                            HBRUSH hBrush = CreateSolidBrush(RGB(28, 41, 53));
                            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
                            HPEN hPen = CreatePen(PS_SOLID, 1, RGB(28, 41, 53));
                            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
                            
                            RoundRect(hdc, bubbleRect.left, bubbleRect.top, 
                                     bubbleRect.right, bubbleRect.bottom, 10, 10);
                            
                            SelectObject(hdc, hOldPen);
                            SelectObject(hdc, hOldBrush);
                            DeleteObject(hPen);
                            DeleteObject(hBrush);
                            
                            // 绘制文本
                            RECT textRect = bubbleRect;
                            textRect.left += bubblePadding;
                            textRect.right -= bubblePadding;
                            textRect.top += bubblePadding;
                            textRect.bottom -= bubblePadding;
                            
                            SetTextColor(hdc, RGB(255, 255, 255));
                            DrawTextW(hdc, block.content.c_str(), -1, &textRect, DT_WORDBREAK);
                        } else {
                            // AI消息：左对齐，不使用气泡，直接显示文字
                            bubbleRect.left = padding;
                            bubbleRect.right = padding + maxWidth;
                            bubbleRect.top = yPos;
                            bubbleRect.bottom = yPos + textHeight;
                            
                            // 处理"正在运行中"的动画显示
                            std::wstring displayContent = block.content;
                            if (data->isStreaming && displayContent.find(L"⏳ 正在运行中") != std::wstring::npos) {
                                // 旋转字符数组
                                const wchar_t* spinChars[] = {L"⠋", L"⠙", L"⠹", L"⠸", L"⠼", L"⠴", L"⠦", L"⠧"};
                                std::wstring spinner = spinChars[data->animationFrame];
                                
                                // 替换⏳为旋转字符
                                size_t pos = displayContent.find(L"⏳");
                                if (pos != std::wstring::npos) {
                                    displayContent.replace(pos, 1, spinner);
                                }
                            }
                            
                            // 直接绘制文本，不绘制气泡
                            SetBkMode(hdc, TRANSPARENT);
                            SetTextColor(hdc, g_CurrentTheme.text);
                            DrawTextW(hdc, displayContent.c_str(), -1, &bubbleRect, DT_WORDBREAK);
                        }
                        
                        // 如果是用户消息且是第一个块，在气泡右下角外面显示文件名
                        if (msg.isUser && j == 0) {
                            extern EditorData* g_EditorData;
                            if (g_EditorData) {
                                EditorDocument* doc = g_EditorData->GetActiveDoc();
                                if (doc && !doc->filePath.empty()) {
                                    // 提取文件名
                                    std::wstring filePath = doc->filePath;
                                    size_t lastSlash = filePath.find_last_of(L"\\/");
                                    std::wstring fileName = (lastSlash != std::wstring::npos) ? filePath.substr(lastSlash + 1) : filePath;
                                    
                                    // 设置字体和灰色
                                    SetBkMode(hdc, TRANSPARENT);
                                    SetTextColor(hdc, RGB(120, 120, 120));
                                    HFONT hFileFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,  // 字体从12增加到14
                                        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
                                    HFONT hOldFileFont = (HFONT)SelectObject(hdc, hFileFont);
                                    
                                    // 计算文件名宽度
                                    SIZE textSize;
                                    GetTextExtentPoint32W(hdc, fileName.c_str(), (int)fileName.length(), &textSize);
                                    
                                    // 绘制方框
                                    int iconSize = 14;
                                    int boxPadding = 6;
                                    int boxHeight = 24;  // 方框高度
                                    int boxWidth = iconSize + boxPadding * 2 + textSize.cx + boxPadding;
                                    
                                    RECT boxRect;
                                    boxRect.right = bubbleRect.right;
                                    boxRect.left = bubbleRect.right - boxWidth;
                                    boxRect.top = bubbleRect.bottom + 4;
                                    boxRect.bottom = boxRect.top + boxHeight;
                                    
                                    // 绘制方框边框
                                    HBRUSH hBoxBrush = CreateSolidBrush(g_CurrentTheme.bg);  // 使用背景色填充
                                    HPEN hBoxPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
                                    HBRUSH hOldBoxBrush = (HBRUSH)SelectObject(hdc, hBoxBrush);
                                    HPEN hOldBoxPen = (HPEN)SelectObject(hdc, hBoxPen);
                                    
                                    RoundRect(hdc, boxRect.left, boxRect.top, boxRect.right, boxRect.bottom, 4, 4);
                                    
                                    SelectObject(hdc, hOldBoxPen);
                                    SelectObject(hdc, hOldBoxBrush);
                                    DeleteObject(hBoxPen);
                                    DeleteObject(hBoxBrush);
                                    
                                    // 绘制文件图标（灰色小方块）
                                    int iconX = boxRect.left + boxPadding;
                                    int iconY = boxRect.top + (boxHeight - iconSize) / 2;
                                    RECT iconRect = {iconX, iconY, iconX + iconSize, iconY + iconSize};
                                    HBRUSH hIconBrush = CreateSolidBrush(RGB(100, 100, 100));
                                    FillRect(hdc, &iconRect, hIconBrush);
                                    DeleteObject(hIconBrush);
                                    
                                    // 在图标右侧绘制文件名
                                    RECT fileNameRect;
                                    fileNameRect.left = iconX + iconSize + boxPadding;
                                    fileNameRect.right = boxRect.right - boxPadding;
                                    fileNameRect.top = boxRect.top;
                                    fileNameRect.bottom = boxRect.bottom;
                                    DrawTextW(hdc, fileName.c_str(), -1, &fileNameRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                                    
                                    SelectObject(hdc, hOldFileFont);
                                    DeleteObject(hFileFont);
                                    
                                    // 更新yPos到文件名方框底部
                                    yPos = boxRect.bottom + 10;
                                } else {
                                    // 没有文件，正常更新yPos
                                    yPos = bubbleRect.bottom + 10;
                                }
                            } else {
                                // 没有EditorData，正常更新yPos
                                yPos = bubbleRect.bottom + 10;
                            }
                        } else {
                            // 不是用户消息的第一个块，正常更新yPos
                            yPos = bubbleRect.bottom + 10;
                        }
                        
                    } else if (block.type == BLOCK_CODE) {
                        // 易语言代码块 - 使用类似 YiEditor 的表格渲染
                        
                        // 按钮参数（在代码块上方）
                        int codeBtnSize = 16;  // 代码块按钮图片大小
                        int btnSpacing = 8;  // 按钮间距
                        int btnMarginTop = 20;  // 按钮在代码块上方 20px
                        
                        // 为按钮预留空间
                        yPos += btnMarginTop + 5;  // 留出按钮的空间和间隙
                        
                        int codeStartY = yPos;
                        int codeBlockPadding = 10;
                        int rowHeight = 26;
                        int codeFontSize = 14;
                        
                        // 计算可用宽度（留出内边距）
                        int availableWidth = rect.right - padding * 2 - 20;  // 用于普通文本换行
                        int maxCodeWidth = 0;  // 记录最大宽度
                        
                        // 创建代码字体
                        HFONT hCodeFont = CreateFont(codeFontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                                    GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                                    DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Consolas");
                        HFONT hCodeBoldFont = CreateFont(codeFontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                                        GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                                        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Consolas");
                        HFONT hOldCodeFont = (HFONT)SelectObject(hdc, hCodeFont);

                        // 分割代码为多行
                        std::vector<std::wstring> codeLines;
                        size_t lineStart = 0;
                        for (size_t pos = 0; pos <= block.content.length(); pos++) {
                            if (pos == block.content.length() || block.content[pos] == L'\n') {
                                if (pos > lineStart || pos == block.content.length()) {
                                    std::wstring line = block.content.substr(lineStart, pos - lineStart);
                                    // 移除行尾的 \r
                                    if (!line.empty() && line.back() == L'\r') line.pop_back();
                                    codeLines.push_back(line);
                                }
                                lineStart = pos + 1;
                            }
                        }
                        
                        // 预计算实际高度（包含换行）
                        int totalCodeHeight = 0;
                        for (const auto& line : codeLines) {
                            if (line.find(L"\t") != std::wstring::npos) {
                                // 表格行：固定高度
                                totalCodeHeight += rowHeight;
                            } else {
                                // 计算缩进
                                int indent = 0;
                                size_t pos = 0;
                                while (pos < line.length() && (line[pos] == L' ' || line[pos] == L'\t')) {
                                    if (line[pos] == L'\t') indent += 20;
                                    else indent += 4;
                                    pos++;
                                }

                                // 普通文本：可能换行
                                // 减去缩进宽度，确保计算的高度与绘制时一致
                                int calcWidth = availableWidth - indent;
                                if (calcWidth < 50) calcWidth = 50;

                                RECT calcRect = {0, 0, calcWidth, 0};
                                // 跳过缩进字符进行计算
                                DrawTextW(hdc, line.c_str() + pos, -1, &calcRect, DT_WORDBREAK | DT_CALCRECT);
                                int lineHeight = (std::max)(rowHeight, (int)(calcRect.bottom - calcRect.top));
                                totalCodeHeight += lineHeight;
                            }
                        }
                        
                        // 计算代码块总高度
                        int scrollBarHeight = 14;  // 横向滚动条高度
                        int codeHeight = totalCodeHeight + codeBlockPadding * 2 + scrollBarHeight;
                        
                        // 绘制代码块背景
                        RECT codeRect;
                        codeRect.left = padding;
                        codeRect.right = rect.right - padding;  // 使用实际窗口宽度
                        codeRect.top = codeStartY;
                        codeRect.bottom = codeStartY + codeHeight;
                        
                        // 计算按钮位置（在代码块上方）
                        int insertBtnX = codeRect.right - 10 - codeBtnSize;
                        int copyBtnX = insertBtnX - btnSpacing - codeBtnSize;
                        int undoBtnX = copyBtnX - btnSpacing - codeBtnSize;
                        int btnY = codeRect.top - btnMarginTop;  // 按钮在代码块上方
                        
                        // 绘制代码块背景
                        HBRUSH hCodeBrush = CreateSolidBrush(RGB(250, 250, 250));
                        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hCodeBrush);
                        HPEN hBorderPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
                        HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
                        
                        // 绘制圆角矩形背景和边框
                        RoundRect(hdc, codeRect.left, codeRect.top, codeRect.right, codeRect.bottom, 8, 8);
                        
                        SelectObject(hdc, hOldPen);
                        SelectObject(hdc, hOldBrush);
                        DeleteObject(hBorderPen);
                        DeleteObject(hCodeBrush);
                        
                        // 颜色定义
                        COLORREF clrText = g_CurrentTheme.text;
                        COLORREF clrKeyword = RGB(0, 0, 255);
                        COLORREF clrComment = RGB(0, 128, 0);
                        COLORREF clrGridLine = g_CurrentTheme.grid;
                        COLORREF clrType = RGB(0, 0, 255);
                        COLORREF clrCheck = RGB(139, 0, 0);
                        COLORREF clrRemark = RGB(0, 128, 0);
                        
                        int codeLineY = codeStartY + codeBlockPadding;
                        int codeStartX = codeRect.left + 10;
                        
                        // 设置代码区域的裁剪区域（避免超出边框）
                        RECT codeContentRect = codeRect;
                        codeContentRect.top = codeRect.top + 2;  // 留出上边框
                        codeContentRect.left = codeRect.left + 2;  // 留出左边框
                        codeContentRect.right = codeRect.right - 2;  // 留出右边框
                        codeContentRect.bottom = codeRect.bottom - scrollBarHeight - 2;  // 留出滚动条和下边框
                        HRGN hClipRgn = CreateRectRgn(codeContentRect.left, codeContentRect.top, codeContentRect.right, codeContentRect.bottom);
                        SelectClipRgn(hdc, hClipRgn);
                        
                        // Lambda: 绘制网格线
                        auto DrawGridLine = [&](int x1, int y1, int x2, int y2) {
                            HPEN hPen = CreatePen(PS_SOLID, 1, clrGridLine);
                            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
                            MoveToEx(hdc, x1, y1, NULL);
                            LineTo(hdc, x2, y2);
                            SelectObject(hdc, hOldPen);
                            DeleteObject(hPen);
                        };
                        
                        // Lambda: 绘制表格单元格
                        auto DrawTableCell = [&](int x, int y, int w, int h, const wchar_t* text, COLORREF color, bool isHeader = false, COLORREF headerBg = RGB(220, 220, 220), bool centerAlign = false) {
                            if (isHeader) {
                                RECT rc = {x, y, x + w, y + h};
                                HBRUSH hHeaderBrush = CreateSolidBrush(headerBg);
                                FillRect(hdc, &rc, hHeaderBrush);
                                DeleteObject(hHeaderBrush);
                            }
                            DrawGridLine(x, y, x + w, y);
                            DrawGridLine(x + w, y, x + w, y + h);
                            DrawGridLine(x, y + h, x + w, y + h);
                            DrawGridLine(x, y, x, y + h);
                            
                            SetTextColor(hdc, color);
                            SelectObject(hdc, isHeader ? hCodeBoldFont : hCodeFont);
                            
                            int textY = y + (h - codeFontSize) / 2;
                            if (centerAlign) {
                                SIZE textSize;
                                GetTextExtentPoint32W(hdc, text, lstrlenW(text), &textSize);
                                int textX = x + (w - textSize.cx) / 2;
                                TextOutW(hdc, textX, textY, text, lstrlenW(text));
                            } else {
                                TextOutW(hdc, x + 3, textY, text, lstrlenW(text));
                            }
                        };
                        
                        // 绘制每一行代码
                        for (size_t lineIdx = 0; lineIdx < codeLines.size(); lineIdx++) {
                            const std::wstring& line = codeLines[lineIdx];
                            
                            if (line.find(L"\t") != std::wstring::npos) {
                                // 表格行 - 解析单元格
                                std::vector<std::wstring> cells;
                                size_t start = 0, pos = 0;
                                while ((pos = line.find(L'\t', start)) != std::wstring::npos) {
                                    cells.push_back(line.substr(start, pos - start));
                                    start = pos + 1;
                                }
                                cells.push_back(line.substr(start));
                                
                                // 判断是否为表头
                                bool isHeader = (cells.size() > 0 && 
                                               (cells[0].find(L"名") != std::wstring::npos || 
                                                cells[0].find(L"程") != std::wstring::npos));
                                
                                // 确定列宽
                                std::vector<int> colWidths;
                                COLORREF headerBg = RGB(220, 220, 220);
                                if (cells.size() > 0) {
                                    if (cells[0] == L"子程序名") {
                                        colWidths = {150, 100, 30, 30, 200};
                                        headerBg = RGB(0xE4, 0xED, 0xE6);
                                    } else if (cells[0] == L"参数名") {
                                        colWidths = {150, 100, 30, 30, 200};
                                        headerBg = RGB(0xF0, 0xE3, 0xD9);
                                    } else if (cells[0] == L"变量名") {
                                        colWidths = {100, 80, 30, 30, 200};
                                        headerBg = RGB(0xF0, 0xE3, 0xD9);
                                    } else {
                                        for (size_t i = 0; i < cells.size(); i++) colWidths.push_back(100);
                                    }
                                }
                                
                                // 绘制单元格
                                int cellX = codeStartX - block.scrollX;  // 应用横向滚动偏移
                                int tableRowWidth = 0;  // 记录表格行宽度
                                for (size_t i = 0; i < cells.size() && i < colWidths.size(); i++) {
                                    COLORREF cellColor = clrText;
                                    bool centerAlign = false;
                                    if (!isHeader) {
                                        if (i == 1) cellColor = clrType;  // 类型列
                                        else if (i == 2 || i == 3) { cellColor = clrCheck; centerAlign = true; }  // 复选框列
                                        else if (i == 4) cellColor = clrRemark;  // 备注列
                                    }
                                    DrawTableCell(cellX, codeLineY, colWidths[i], rowHeight, 
                                                cells[i].c_str(), cellColor, isHeader, headerBg, centerAlign);
                                    cellX += colWidths[i];
                                    tableRowWidth += colWidths[i];
                                }
                                // 更新最大宽度
                                if (tableRowWidth > maxCodeWidth) maxCodeWidth = tableRowWidth;
                            } else if (IsFlowControlLine(line)) {
                                // 流程控制语句 - 绘制虚线（向右缩进TAB）
                                int indentX = 20;  // TAB缩进
                                SetTextColor(hdc, clrKeyword);
                                SelectObject(hdc, hCodeFont);
                                TextOutW(hdc, codeStartX + indentX + 10, codeLineY + (rowHeight - codeFontSize) / 2, 
                                        line.c_str(), (int)line.length());
                                
                                // 绘制横向虚线到"如"字左侧
                                int lineX = codeStartX + indentX;
                                int textCenterY = codeLineY + rowHeight / 2;
                                HPEN hDashPen = CreatePen(PS_DOT, 1, RGB(128, 128, 128));
                                HPEN hOldDashPen = (HPEN)SelectObject(hdc, hDashPen);
                                MoveToEx(hdc, lineX, textCenterY, NULL);
                                LineTo(hdc, codeStartX + indentX + 8, textCenterY);
                                
                                // 垂直虚线（查找下一个空行或流程块结束）
                                int endY = codeLineY;
                                for (size_t j = lineIdx + 1; j < codeLines.size(); j++) {
                                    if (codeLines[j].empty() || IsFlowControlLine(codeLines[j])) {
                                        endY = codeLineY + (int)(j - lineIdx) * rowHeight + rowHeight / 2;
                                        break;
                                    }
                                }
                                MoveToEx(hdc, lineX, textCenterY, NULL);
                                LineTo(hdc, lineX, endY);
                                
                                // 向下箭头
                                MoveToEx(hdc, lineX - 3, endY - 3, NULL);
                                LineTo(hdc, lineX, endY);
                                MoveToEx(hdc, lineX + 3, endY - 3, NULL);
                                LineTo(hdc, lineX, endY);
                                
                                SelectObject(hdc, hOldDashPen);
                                DeleteObject(hDashPen);
                            } else if (line.length() > 0 && line[0] == L'\'') {
                                // 注释行（支持换行）
                                SetTextColor(hdc, clrComment);
                                SelectObject(hdc, hCodeFont);
                                
                                RECT textRect = {codeStartX, codeLineY, codeRect.right - 10, codeLineY + rowHeight * 100};
                                int textHeight = DrawTextW(hdc, line.c_str(), -1, &textRect, DT_WORDBREAK);
                                codeLineY += (std::max)(rowHeight, (int)textHeight);
                                continue;  // 跳过后面的 codeLineY += rowHeight
                            } else if (line.find(L"返回") == 0) {
                                // 返回语句
                                SetTextColor(hdc, clrKeyword);
                                SelectObject(hdc, hCodeFont);
                                TextOutW(hdc, codeStartX, codeLineY + (rowHeight - codeFontSize) / 2, 
                                        line.c_str(), (int)line.length());
                            } else if (!line.empty()) {
                                // 普通代码行（处理缩进和换行）
                                int indent = 0;
                                size_t pos = 0;
                                while (pos < line.length() && (line[pos] == L' ' || line[pos] == L'\t')) {
                                    if (line[pos] == L'\t') indent += 20;
                                    else indent += 4;
                                    pos++;
                                }
                                
                                SetTextColor(hdc, clrText);
                                SelectObject(hdc, hCodeFont);
                                
                                // 支持换行
                                RECT textRect = {codeStartX + indent, codeLineY, codeRect.right - 10, codeLineY + rowHeight * 100};
                                int textHeight = DrawTextW(hdc, line.c_str() + pos, -1, &textRect, DT_WORDBREAK);
                                codeLineY += (std::max)(rowHeight, (int)textHeight);
                                continue;  // 跳过后面的 codeLineY += rowHeight
                            }
                            
                            codeLineY += rowHeight;
                        }
                        
                        SelectObject(hdc, hOldCodeFont);
                        DeleteObject(hCodeFont);
                        DeleteObject(hCodeBoldFont);
                        
                        // 移除裁剪区域
                        SelectClipRgn(hdc, NULL);
                        DeleteObject(hClipRgn);
                        
                        // 保存代码块内容宽度
                        block.contentWidth = maxCodeWidth;
                        
                        // 绘制横向滚动条（如果内容超出）
                        int codeBlockWidth = codeRect.right - codeRect.left - 20;  // 减去左右边距
                        if (maxCodeWidth > codeBlockWidth) {
                            int scrollBarY = codeRect.bottom - scrollBarHeight - 5;  // 距离底部 5px
                            int scrollBarX = codeRect.left + 10;
                            int scrollBarWidth = codeBlockWidth;
                            
                            // 绘制滚动条背景
                            RECT scrollBarRect = {scrollBarX, scrollBarY, scrollBarX + scrollBarWidth, scrollBarY + scrollBarHeight};
                            FillRect(hdc, &scrollBarRect, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
                            
                            // 计算滚动条滑块的位置和大小
                            int thumbWidth = (int)((float)scrollBarWidth * scrollBarWidth / maxCodeWidth);
                            if (thumbWidth < 20) thumbWidth = 20;  // 最小滑块宽度
                            int maxScrollX = maxCodeWidth - codeBlockWidth;
                            int thumbX = scrollBarX + (int)((float)block.scrollX * (scrollBarWidth - thumbWidth) / maxScrollX);
                            
                            // 绘制滚动条滑块
                            RECT thumbRect = {thumbX, scrollBarY + 2, thumbX + thumbWidth, scrollBarY + scrollBarHeight - 2};
                            FillRect(hdc, &thumbRect, (HBRUSH)GetStockObject(GRAY_BRUSH));
                            
                            // 保存滚动条区域（用于鼠标事件）
                            block.rect = scrollBarRect;
                        }
                        
                        // 保存按钮区域（用于鼠标事件）
                        block.copyBtnRect = {copyBtnX, btnY, copyBtnX + codeBtnSize, btnY + codeBtnSize};
                        block.insertBtnRect = {insertBtnX, btnY, insertBtnX + codeBtnSize, btnY + codeBtnSize};
                        block.undoBtnRect = {undoBtnX, btnY, undoBtnX + codeBtnSize, btnY + codeBtnSize};
                        
                        // 检查是否需要绘制悬停高亮
                        bool isHoveringCopy = (data->hoveredCodeButton == 1 && 
                                              data->hoveredCodeMsgIdx == i && 
                                              data->hoveredCodeBlockIdx == j);
                        bool isHoveringInsert = (data->hoveredCodeButton == 2 && 
                                                data->hoveredCodeMsgIdx == i && 
                                                data->hoveredCodeBlockIdx == j);
                        bool isHoveringUndo = (data->hoveredCodeButton == 3 && 
                                                data->hoveredCodeMsgIdx == i && 
                                                data->hoveredCodeBlockIdx == j);
                        
                        // 绘制复制按钮高亮背景
                        if (isHoveringCopy) {
                            RECT hoverRect = block.copyBtnRect;
                            InflateRect(&hoverRect, 4, 4);  // 扩大一点
                            HBRUSH hHoverBrush = CreateSolidBrush(RGB(230, 230, 230));
                            HPEN hHoverPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
                            HBRUSH hOldBrush2 = (HBRUSH)SelectObject(hdc, hHoverBrush);
                            HPEN hOldPen2 = (HPEN)SelectObject(hdc, hHoverPen);
                            RoundRect(hdc, hoverRect.left, hoverRect.top, hoverRect.right, hoverRect.bottom, 6, 6);
                            SelectObject(hdc, hOldPen2);
                            SelectObject(hdc, hOldBrush2);
                            DeleteObject(hHoverPen);
                            DeleteObject(hHoverBrush);
                        }
                        
                        // 绘制插入按钮高亮背景
                        if (isHoveringInsert) {
                            RECT hoverRect = block.insertBtnRect;
                            InflateRect(&hoverRect, 4, 4);
                            HBRUSH hHoverBrush = CreateSolidBrush(RGB(230, 230, 230));
                            HPEN hHoverPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
                            HBRUSH hOldBrush2 = (HBRUSH)SelectObject(hdc, hHoverBrush);
                            HPEN hOldPen2 = (HPEN)SelectObject(hdc, hHoverPen);
                            RoundRect(hdc, hoverRect.left, hoverRect.top, hoverRect.right, hoverRect.bottom, 6, 6);
                            SelectObject(hdc, hOldPen2);
                            SelectObject(hdc, hOldBrush2);
                            DeleteObject(hHoverPen);
                            DeleteObject(hHoverBrush);
                        }
                        
                        // 绘制撤销按钮高亮背景
                        if (isHoveringUndo) {
                            RECT hoverRect = block.undoBtnRect;
                            InflateRect(&hoverRect, 4, 4);
                            HBRUSH hHoverBrush = CreateSolidBrush(RGB(230, 230, 230));
                            HPEN hHoverPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
                            HBRUSH hOldBrush2 = (HBRUSH)SelectObject(hdc, hHoverBrush);
                            HPEN hOldPen2 = (HPEN)SelectObject(hdc, hHoverPen);
                            RoundRect(hdc, hoverRect.left, hoverRect.top, hoverRect.right, hoverRect.bottom, 6, 6);
                            SelectObject(hdc, hOldPen2);
                            SelectObject(hdc, hOldBrush2);
                            DeleteObject(hHoverPen);
                            DeleteObject(hHoverBrush);
                        }
                        
                        // 绘制复制按钮图片
                        if (g_hCodeCopyIcon) {
                            HDC hdcMem = CreateCompatibleDC(hdc);
                            HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, g_hCodeCopyIcon);
                            BITMAP bm;
                            GetObject(g_hCodeCopyIcon, sizeof(bm), &bm);
                            
                            BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
                            AlphaBlend(hdc, copyBtnX, btnY, codeBtnSize, codeBtnSize,
                                      hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, bf);
                            
                            SelectObject(hdcMem, hOldBitmap);
                            DeleteDC(hdcMem);
                        }
                        
                        // 绘制插入按钮图片
                        if (g_hCodeInsertIcon) {
                            HDC hdcMem = CreateCompatibleDC(hdc);
                            HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, g_hCodeInsertIcon);
                            BITMAP bm;
                            GetObject(g_hCodeInsertIcon, sizeof(bm), &bm);
                            
                            BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
                            AlphaBlend(hdc, insertBtnX, btnY, codeBtnSize, codeBtnSize,
                                      hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, bf);
                            
                            SelectObject(hdcMem, hOldBitmap);
                            DeleteDC(hdcMem);
                        }
                        
                        // 绘制撤销按钮图片
                        if (g_hUndoIcon) {
                            HDC hdcMem = CreateCompatibleDC(hdc);
                            HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, g_hUndoIcon);
                            BITMAP bm;
                            GetObject(g_hUndoIcon, sizeof(bm), &bm);
                            
                            BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
                            AlphaBlend(hdc, undoBtnX, btnY, codeBtnSize, codeBtnSize,
                                      hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, bf);
                            
                            SelectObject(hdcMem, hOldBitmap);
                            DeleteDC(hdcMem);
                        }
                        
                        yPos = codeRect.bottom + 10;
                    }
                }
                
                // AI消息后绘制分隔线和按钮（只在流式输出完成后）
                if (!msg.isUser) {
                    // 检查是否是正在流式输出的消息
                    bool isCurrentlyStreaming = (data->isStreaming && data->streamingMsgIdx == (int)i);
                    
                    // 只在流式输出完成后绘制虚线和按钮
                    if (!isCurrentlyStreaming) {
                        // 绘制虚线分隔线(===样式)
                        HPEN hLinePen = CreatePen(PS_DOT, 1, RGB(180, 180, 180));
                        HPEN hOldPen = (HPEN)SelectObject(hdc, hLinePen);
                    
                    // 绘制多条短线形成===效果
                    int lineY = yPos;
                    int lineStartX = padding;
                    int lineEndX = rect.right - padding;
                    int lineCount = 1;  // 绘制1条横线
                    int lineSpacing = 2;
                    
                    for (int k = 0; k < lineCount; k++) {
                        MoveToEx(hdc, lineStartX, lineY + k * lineSpacing, NULL);
                        LineTo(hdc, lineEndX, lineY + k * lineSpacing);
                    }
                    
                    SelectObject(hdc, hOldPen);
                    DeleteObject(hLinePen);
                    
                    yPos += lineCount * lineSpacing + 10;
                    
                    // 按钮区域参数
                    int btnSize = 16;  // 按钮图片大小
                    int btnSpacing = 10;  // 按钮间距
                    int btnY = yPos;
                    
                    // 保存按钮区域用于点击检测
                    msg.retryButtonRect.left = padding;
                    msg.retryButtonRect.top = btnY;
                    msg.retryButtonRect.right = padding + btnSize;
                    msg.retryButtonRect.bottom = btnY + btnSize;
                    
                    msg.copyAllButtonRect.left = padding + btnSize + btnSpacing;
                    msg.copyAllButtonRect.top = btnY;
                    msg.copyAllButtonRect.right = padding + btnSize * 2 + btnSpacing;
                    msg.copyAllButtonRect.bottom = btnY + btnSize;
                    
                    msg.undoButtonRect.left = padding + btnSize * 2 + btnSpacing * 2;
                    msg.undoButtonRect.top = btnY;
                    msg.undoButtonRect.right = padding + btnSize * 3 + btnSpacing * 2;
                    msg.undoButtonRect.bottom = btnY + btnSize;
                    
                    // 检查是否需要绘制悬停高亮
                    bool isHoveringRetry = (data->hoveredButton == 1 && data->hoveredMsgIdx == i);
                    bool isHoveringCopy = (data->hoveredButton == 2 && data->hoveredMsgIdx == i);
                    bool isHoveringUndo = (data->hoveredButton == 3 && data->hoveredMsgIdx == i);
                    
                    // 绘制重试按钮高亮背景
                    if (isHoveringRetry) {
                        RECT hoverRect = msg.retryButtonRect;
                        InflateRect(&hoverRect, 4, 4);
                        HBRUSH hHoverBrush = CreateSolidBrush(RGB(230, 230, 230));
                        HPEN hHoverPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
                        HBRUSH hOldBrush2 = (HBRUSH)SelectObject(hdc, hHoverBrush);
                        HPEN hOldPen2 = (HPEN)SelectObject(hdc, hHoverPen);
                        RoundRect(hdc, hoverRect.left, hoverRect.top, hoverRect.right, hoverRect.bottom, 6, 6);
                        SelectObject(hdc, hOldPen2);
                        SelectObject(hdc, hOldBrush2);
                        DeleteObject(hHoverPen);
                        DeleteObject(hHoverBrush);
                    }
                    
                    // 绘制复制按钮高亮背景
                    if (isHoveringCopy) {
                        RECT hoverRect = msg.copyAllButtonRect;
                        InflateRect(&hoverRect, 4, 4);
                        HBRUSH hHoverBrush = CreateSolidBrush(RGB(230, 230, 230));
                        HPEN hHoverPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
                        HBRUSH hOldBrush2 = (HBRUSH)SelectObject(hdc, hHoverBrush);
                        HPEN hOldPen2 = (HPEN)SelectObject(hdc, hHoverPen);
                        RoundRect(hdc, hoverRect.left, hoverRect.top, hoverRect.right, hoverRect.bottom, 6, 6);
                        SelectObject(hdc, hOldPen2);
                        SelectObject(hdc, hOldBrush2);
                        DeleteObject(hHoverPen);
                        DeleteObject(hHoverBrush);
                    }
                    
                    // 绘制撤销按钮高亮背景
                    if (isHoveringUndo) {
                        RECT hoverRect = msg.undoButtonRect;
                        InflateRect(&hoverRect, 4, 4);
                        HBRUSH hHoverBrush = CreateSolidBrush(RGB(230, 230, 230));
                        HPEN hHoverPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
                        HBRUSH hOldBrush2 = (HBRUSH)SelectObject(hdc, hHoverBrush);
                        HPEN hOldPen2 = (HPEN)SelectObject(hdc, hHoverPen);
                        RoundRect(hdc, hoverRect.left, hoverRect.top, hoverRect.right, hoverRect.bottom, 6, 6);
                        SelectObject(hdc, hOldPen2);
                        SelectObject(hdc, hOldBrush2);
                        DeleteObject(hHoverPen);
                        DeleteObject(hHoverBrush);
                    }
                    
                    // 绘制重试按钮图片
                    if (g_hRetryIcon) {
                        HDC hdcMem = CreateCompatibleDC(hdc);
                        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, g_hRetryIcon);
                        BITMAP bm;
                        GetObject(g_hRetryIcon, sizeof(bm), &bm);
                        
                        BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
                        AlphaBlend(hdc, msg.retryButtonRect.left, msg.retryButtonRect.top, btnSize, btnSize,
                                  hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, bf);
                        
                        SelectObject(hdcMem, hOldBitmap);
                        DeleteDC(hdcMem);
                    }
                    
                    // 绘制复制按钮图片
                    if (g_hCopyIcon) {
                        HDC hdcMem = CreateCompatibleDC(hdc);
                        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, g_hCopyIcon);
                        BITMAP bm;
                        GetObject(g_hCopyIcon, sizeof(bm), &bm);
                        
                        BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
                        AlphaBlend(hdc, msg.copyAllButtonRect.left, msg.copyAllButtonRect.top, btnSize, btnSize,
                                  hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, bf);
                        
                        SelectObject(hdcMem, hOldBitmap);
                        DeleteDC(hdcMem);
                    }
                    
                    // 绘制撤销按钮图片
                    if (g_hUndoIcon) {
                        HDC hdcMem = CreateCompatibleDC(hdc);
                        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, g_hUndoIcon);
                        BITMAP bm;
                        GetObject(g_hUndoIcon, sizeof(bm), &bm);
                        
                        BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
                        AlphaBlend(hdc, msg.undoButtonRect.left, msg.undoButtonRect.top, btnSize, btnSize,
                                  hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, bf);
                        
                        SelectObject(hdcMem, hOldBitmap);
                        DeleteDC(hdcMem);
                    }
                    
                    yPos += btnSize + 10;
                    }  // 结束 isCurrentlyStreaming 检查
                }
            }

            // 计算内容高度并更新滚动条（底部留出额外空间）
            yPos += 50;  // 底部留出 50 像素空间，防止最后内容被裁剪
            // yPos 是相对于 (10 - scrollY) 的位置
            // 实际内容高度 = yPos + scrollY - 10 (初始偏移)
            // 但为了简单，我们直接用 yPos + scrollY 近似
            int contentHeight = yPos + data->scrollY;
            data->contentHeight = contentHeight;

            int clientHeight = rect.bottom - rect.top;
            int maxScroll = contentHeight - clientHeight;
            if (maxScroll < 0) maxScroll = 0;
            if (data->scrollY > maxScroll) {
                data->scrollY = maxScroll;
            }

            // 计算自定义滚动条位置
            data->vScrollbarRect.left = rect.right - data->scrollbarWidth;
            data->vScrollbarRect.top = 0;
            data->vScrollbarRect.right = rect.right;
            data->vScrollbarRect.bottom = rect.bottom;
            
            // 计算滚动条滑块
            if (contentHeight > clientHeight) {
                int trackHeight = data->vScrollbarRect.bottom - data->vScrollbarRect.top;
                int thumbHeight = (std::max)(20, (int)((float)clientHeight / contentHeight * trackHeight));
                int thumbTop = data->vScrollbarRect.top + (int)((float)data->scrollY / maxScroll * (trackHeight - thumbHeight));
                
                data->vScrollThumbRect.left = data->vScrollbarRect.left;
                data->vScrollThumbRect.top = thumbTop;
                data->vScrollThumbRect.right = data->vScrollbarRect.right;
                data->vScrollThumbRect.bottom = thumbTop + thumbHeight;
            } else {
                data->vScrollThumbRect = {0, 0, 0, 0};
            }
            
            // 绘制自定义滚动条
            if (data->vScrollThumbRect.bottom > data->vScrollThumbRect.top) {
                // 垂直滚动条背景
                HBRUSH hScrollBgBrush = CreateSolidBrush(g_CurrentTheme.bg);
                FillRect(hdc, &data->vScrollbarRect, hScrollBgBrush);
                DeleteObject(hScrollBgBrush);
                
                // 垂直滚动条滑块
                COLORREF thumbColor = data->vScrollHover ? RGB(90, 90, 90) : RGB(70, 70, 70);
                HBRUSH hThumbBrush = CreateSolidBrush(thumbColor);
                FillRect(hdc, &data->vScrollThumbRect, hThumbBrush);
                DeleteObject(hThumbBrush);
            }
            
            SelectObject(hdc, hOldFont);
            DeleteObject(hFont);

            // 绘制右边框（如果处于悬停或拖动状态）
            if (data->isRightBorderHover || data->isDraggingRightBorder) {
                HBRUSH hBorderBrush = CreateSolidBrush(RGB(0, 122, 204));
                RECT borderRect = { rect.right - 5, 0, rect.right, rect.bottom };
                FillRect(hdc, &borderRect, hBorderBrush);
                DeleteObject(hBorderBrush);
            }

            // 将内存DC内容复制到屏幕
            BitBlt(hRealDC, 0, 0, rect.right, rect.bottom, hdc, 0, 0, SRCCOPY);

            // 清理双缓冲资源
            SelectObject(hdc, hOldBitmap);
            DeleteObject(hBitmap);
            DeleteDC(hdc);

            EndPaint(hWnd, &ps);
        }
        return 0;
    
    case WM_SETCURSOR:
        {
            if (data && (data->isRightBorderHover || data->isDraggingRightBorder)) {
                SetCursor(LoadCursor(NULL, IDC_SIZEWE));
                return TRUE;
            }
        }
        break;
    
    case WM_LBUTTONDOWN:
        {
            if (!data) return 0;
            
            POINT pt;
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            
            // 检查是否点击了右边框
            if (data->isRightBorderHover) {
                data->isDraggingRightBorder = true;
                data->borderDragStartX = pt.x;
                SetCapture(hWnd);
                return 0;
            }
            
            // 检查是否点击了滚动条滑块
            if (PtInRect(&data->vScrollThumbRect, pt)) {
                data->isDraggingVScroll = true;
                data->dragStartY = pt.y;
                data->dragStartScrollY = data->scrollY;
                SetCapture(hWnd);
                return 0;
            }
            
            // 检查是否点击了滚动条轨道（但不是滑块）
            if (PtInRect(&data->vScrollbarRect, pt) && !PtInRect(&data->vScrollThumbRect, pt)) {
                RECT rect;
                GetClientRect(hWnd, &rect);
                int clientHeight = rect.bottom - rect.top;
                
                if (data->contentHeight > clientHeight) {
                    int trackHeight = data->vScrollbarRect.bottom - data->vScrollbarRect.top;
                    int maxScroll = data->contentHeight - clientHeight;
                    int clickY = pt.y - data->vScrollbarRect.top;
                    
                    // 点击在滑块上方，向上翻页
                    if (pt.y < data->vScrollThumbRect.top) {
                        data->scrollY = (std::max)(0, data->scrollY - clientHeight);
                    }
                    // 点击在滑块下方，向下翻页
                    else if (pt.y > data->vScrollThumbRect.bottom) {
                        data->scrollY = (std::min)(maxScroll, data->scrollY + clientHeight);
                    }
                    
                    InvalidateRect(hWnd, nullptr, FALSE);
                }
                return 0;
            }
        }
        return 0;
        
    case WM_LBUTTONUP:
        {
            if (!data) return 0;
            
            // 停止右边框拖动
            if (data->isDraggingRightBorder) {
                data->isDraggingRightBorder = false;
                ReleaseCapture();
                // 通知父窗口重绘
                HWND hParent = GetParent(hWnd);
                if (hParent) {
                    InvalidateRect(hParent, NULL, FALSE);
                }
                return 0;
            }
            
            // 停止滚动条拖动
            if (data->isDraggingVScroll) {
                data->isDraggingVScroll = false;
                ReleaseCapture();
                return 0;
            }
            
            int mouseX = LOWORD(lParam);
            int mouseY = HIWORD(lParam) + data->scrollY;  // 考虑垂直滚动偏移
            
            // 检查是否点击了代码块按钮
            for (size_t i = 0; i < data->messages.size(); i++) {
                ChatMessage& msg = data->messages[i];
                for (size_t j = 0; j < msg.blocks.size(); j++) {
                    MessageBlock& block = msg.blocks[j];
                    if (block.type == BLOCK_CODE) {
                        // 检查复制按钮
                        if (mouseX >= block.copyBtnRect.left && mouseX <= block.copyBtnRect.right &&
                            mouseY >= block.copyBtnRect.top && mouseY <= block.copyBtnRect.bottom) {
                            // 复制代码块内容到剪贴板
                            if (OpenClipboard(hWnd)) {
                                EmptyClipboard();
                                size_t len = (block.content.length() + 1) * sizeof(wchar_t);
                                HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
                                if (hMem) {
                                    wchar_t* pMem = (wchar_t*)GlobalLock(hMem);
                                    wcscpy_s(pMem, block.content.length() + 1, block.content.c_str());
                                    GlobalUnlock(hMem);
                                    SetClipboardData(CF_UNICODETEXT, hMem);
                                }
                                CloseClipboard();
                            }
                            return 0;
                        }
                        // 检查插入按钮
                        if (mouseX >= block.insertBtnRect.left && mouseX <= block.insertBtnRect.right &&
                            mouseY >= block.insertBtnRect.top && mouseY <= block.insertBtnRect.bottom) {
                            // TODO: 实现插入功能（将代码插入到代码编辑器）
                            return 0;
                        }
                        
                        // 检查撤销按钮
                        if (mouseX >= block.undoBtnRect.left && mouseX <= block.undoBtnRect.right &&
                            mouseY >= block.undoBtnRect.top && mouseY <= block.undoBtnRect.bottom) {
                            extern EditorData* g_EditorData;
                            if (g_EditorData) {
                                EditorDocument* doc = g_EditorData->GetActiveDoc();
                                if (doc) {
                                    doc->Undo();
                                    InvalidateRect(g_EditorData->hWnd, NULL, TRUE);
                                }
                            }
                            return 0;
                        }
                    }
                }
            }
            
            // 检查是否点击了代码块的横向滚动条
            for (size_t i = 0; i < data->messages.size(); i++) {
                ChatMessage& msg = data->messages[i];
                for (size_t j = 0; j < msg.blocks.size(); j++) {
                    MessageBlock& block = msg.blocks[j];
                    if (block.type == BLOCK_CODE && block.contentWidth > 0) {
                        // 检查是否点击在滚动条区域
                        if (mouseX >= block.rect.left && mouseX <= block.rect.right &&
                            mouseY >= block.rect.top && mouseY <= block.rect.bottom) {
                            // 计算点击位置对应的滚动值
                            // RECT codeRect;  // 需要重新计算代码块区域，但简单处理：直接使用 block.rect
                            int scrollBarWidth = block.rect.right - block.rect.left;
                            int codeBlockWidth = scrollBarWidth;
                            int maxScrollX = block.contentWidth - codeBlockWidth;
                            
                            int clickX = mouseX - block.rect.left;
                            int thumbWidth = (int)((float)scrollBarWidth * scrollBarWidth / block.contentWidth);
                            if (thumbWidth < 20) thumbWidth = 20;
                            
                            int newScrollX = (int)((float)clickX * maxScrollX / (scrollBarWidth - thumbWidth));
                            if (newScrollX < 0) newScrollX = 0;
                            if (newScrollX > maxScrollX) newScrollX = maxScrollX;
                            
                            block.scrollX = newScrollX;
                            InvalidateRect(hWnd, NULL, TRUE);
                            return 0;
                        }
                    }
                }
            }
            
            mouseY = HIWORD(lParam);  // 恢复原始 Y 坐标用于按钮检测
            
            // 检查是否点击了复制按钮
            for (size_t i = 0; i < data->messages.size(); i++) {
                ChatMessage& msg = data->messages[i];
                
                if (!msg.isUser) {
                    // 检查复制按钮
                    if (mouseX >= msg.copyAllButtonRect.left && mouseX <= msg.copyAllButtonRect.right &&
                        mouseY >= msg.copyAllButtonRect.top && mouseY <= msg.copyAllButtonRect.bottom) {
                        
                        // 提取所有消息块的文本
                        std::wstring allText;
                        for (const auto& block : msg.blocks) {
                            if (!allText.empty()) allText += L"\n";
                            allText += block.content;
                        }
                        
                        // 复制到剪贴板
                        if (OpenClipboard(hWnd)) {
                            EmptyClipboard();
                            size_t len = (allText.length() + 1) * sizeof(wchar_t);
                            HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
                            if (hMem) {
                                wchar_t* pMem = (wchar_t*)GlobalLock(hMem);
                                wcscpy_s(pMem, allText.length() + 1, allText.c_str());
                                GlobalUnlock(hMem);
                                SetClipboardData(CF_UNICODETEXT, hMem);
                            }
                            CloseClipboard();
                        }
                        return 0;
                    }
                    
                    // 检查重试按钮
                    if (mouseX >= msg.retryButtonRect.left && mouseX <= msg.retryButtonRect.right &&
                        mouseY >= msg.retryButtonRect.top && mouseY <= msg.retryButtonRect.bottom) {
                        // TODO: 实现重试功能
                        return 0;
                    }
                    
                    // 检查撤销按钮
                    if (mouseX >= msg.undoButtonRect.left && mouseX <= msg.undoButtonRect.right &&
                        mouseY >= msg.undoButtonRect.top && mouseY <= msg.undoButtonRect.bottom) {
                        // 撤销编辑器操作
                        extern EditorData* g_EditorData;
                        if (g_EditorData) {
                            EditorDocument* doc = g_EditorData->GetActiveDoc();
                            if (doc) {
                                doc->Undo();
                                InvalidateRect(g_EditorData->hWnd, NULL, TRUE);
                            }
                        }
                        return 0;
                    }
                }
            }
        }
        return 0;

    case WM_MOUSEWHEEL:
        {
            if (!data) return 0;
            int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            RECT rc;
            GetClientRect(hWnd, &rc);
            int clientHeight = rc.bottom - rc.top;
            int maxScroll = data->contentHeight - clientHeight;
            if (maxScroll < 0) maxScroll = 0;

            int step = 60;  // 每次滚动的像素
            int newScrollY = data->scrollY + (delta > 0 ? -step : step);
            if (newScrollY < 0) newScrollY = 0;
            if (newScrollY > maxScroll) newScrollY = maxScroll;

            if (newScrollY != data->scrollY) {
                data->scrollY = newScrollY;
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        return 0;

    case WM_MOUSEMOVE:
        {
            if (!data || !data->hTooltip) return 0;
            
            int mouseX = LOWORD(lParam);
            int mouseY = HIWORD(lParam);
            
            // 处理右边框拖动
            if (data->isDraggingRightBorder) {
                // 更新主窗口的g_LeftPanelWidth
                extern int g_LeftPanelWidth;
                extern int g_MinLeftPanelWidth;
                
                // 将鼠标位置转换到主窗口坐标
                POINT screenPt = {mouseX, mouseY};
                ClientToScreen(hWnd, &screenPt);
                HWND hParent = GetParent(hWnd);
                HWND hMainWnd = GetParent(hParent);
                ScreenToClient(hMainWnd, &screenPt);
                
                RECT mainRect;
                GetClientRect(hMainWnd, &mainRect);
                int maxWidth = mainRect.right - 350;
                
                int newWidth = screenPt.x;
                if (newWidth < g_MinLeftPanelWidth) newWidth = g_MinLeftPanelWidth;
                if (newWidth > maxWidth) newWidth = maxWidth;
                
                // 只有宽度真正改变时才更新
                if (newWidth != g_LeftPanelWidth) {
                    g_LeftPanelWidth = newWidth;
                    
                    // 直接触发布局更新，让系统处理重绘
                    SendMessage(hMainWnd, WM_SIZE, 0, MAKELPARAM(mainRect.right, mainRect.bottom));
                    
                    // 强制立即更新显示，避免残影
                    RedrawWindow(hMainWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
                }
                
                return 0;
            }
            
            // 检测是否在右边框区域（最右侧5px）
            RECT rect;
            GetClientRect(hWnd, &rect);
            bool isInRightBorder = (mouseX >= rect.right - 5);
            
            if (isInRightBorder != data->isRightBorderHover && !data->isDraggingRightBorder) {
                data->isRightBorderHover = isInRightBorder;
                // 通知父窗口重绘边框
                HWND hParent = GetParent(hWnd);
                if (hParent) {
                    InvalidateRect(hParent, NULL, FALSE);
                }
            }
            
            // 启用鼠标跟踪以检测鼠标离开
            TRACKMOUSEEVENT tme = {};
            tme.cbSize = sizeof(TRACKMOUSEEVENT);
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = hWnd;
            TrackMouseEvent(&tme);
            
            // 处理滚动条拖动
            if (data->isDraggingVScroll) {
                RECT rect;
                GetClientRect(hWnd, &rect);
                int clientHeight = rect.bottom - rect.top;
                
                if (data->contentHeight > clientHeight) {
                    int trackHeight = data->vScrollbarRect.bottom - data->vScrollbarRect.top;
                    int thumbHeight = data->vScrollThumbRect.bottom - data->vScrollThumbRect.top;
                    int maxScroll = data->contentHeight - clientHeight;
                    
                    int deltaY = mouseY - data->dragStartY;
                    int scrollDelta = (int)((float)deltaY / (trackHeight - thumbHeight) * maxScroll);
                    
                    data->scrollY = data->dragStartScrollY + scrollDelta;
                    if (data->scrollY < 0) data->scrollY = 0;
                    if (data->scrollY > maxScroll) data->scrollY = maxScroll;
                    
                    InvalidateRect(hWnd, nullptr, FALSE);
                }
                return 0;
            }
            
            // 检查滚动条悬停状态
            POINT pt = {mouseX, mouseY};
            bool oldVScrollHover = data->vScrollHover;
            data->vScrollHover = PtInRect(&data->vScrollThumbRect, pt);
            
            if (oldVScrollHover != data->vScrollHover) {
                InvalidateRect(hWnd, &data->vScrollbarRect, FALSE);
            }
            
            int mouseYWithScroll = mouseY + data->scrollY;  // 考虑滚动偏移
            
            // 检查鼠标是否在代码块按钮上
            int newHoveredCodeButton = 0;
            size_t newHoveredCodeMsgIdx = 0;
            size_t newHoveredCodeBlockIdx = 0;
            
            for (size_t i = 0; i < data->messages.size(); i++) {
                ChatMessage& msg = data->messages[i];
                for (size_t j = 0; j < msg.blocks.size(); j++) {
                    MessageBlock& block = msg.blocks[j];
                    if (block.type == BLOCK_CODE) {
                        // 检查复制按钮
                        if (mouseX >= block.copyBtnRect.left && mouseX <= block.copyBtnRect.right &&
                            mouseY >= block.copyBtnRect.top && mouseY <= block.copyBtnRect.bottom) {
                            newHoveredCodeButton = 1;
                            newHoveredCodeMsgIdx = i;
                            newHoveredCodeBlockIdx = j;
                            goto foundCodeButton;
                        }
                        // 检查插入按钮
                        if (mouseX >= block.insertBtnRect.left && mouseX <= block.insertBtnRect.right &&
                            mouseY >= block.insertBtnRect.top && mouseY <= block.insertBtnRect.bottom) {
                            newHoveredCodeButton = 2;
                            newHoveredCodeMsgIdx = i;
                            newHoveredCodeBlockIdx = j;
                            goto foundCodeButton;
                        }
                    }
                }
            }
            
            foundCodeButton:
            // 如果代码块按钮悬停状态改变，重绘窗口
            if (newHoveredCodeButton != data->hoveredCodeButton || 
                newHoveredCodeMsgIdx != data->hoveredCodeMsgIdx ||
                newHoveredCodeBlockIdx != data->hoveredCodeBlockIdx) {
                
                // 只重绘受影响的区域，减少闪烁
                data->hoveredCodeButton = newHoveredCodeButton;
                data->hoveredCodeMsgIdx = newHoveredCodeMsgIdx;
                data->hoveredCodeBlockIdx = newHoveredCodeBlockIdx;
                InvalidateRect(hWnd, NULL, FALSE);  // 不擦除背景，减少闪烁
            }
            
            // 检查鼠标是否在任何按钮上
            int newHoveredButton = 0;
            size_t newHoveredMsgIdx = 0;
            
            for (size_t i = 0; i < data->messages.size(); i++) {
                ChatMessage& msg = data->messages[i];
                if (msg.isUser) continue;
                
                // 检查重试按钮
                if (mouseX >= msg.retryButtonRect.left && mouseX <= msg.retryButtonRect.right &&
                    mouseY >= msg.retryButtonRect.top && mouseY <= msg.retryButtonRect.bottom) {
                    newHoveredButton = 1;
                    newHoveredMsgIdx = i;
                    break;
                }
                
                // 检查复制按钮
                if (mouseX >= msg.copyAllButtonRect.left && mouseX <= msg.copyAllButtonRect.right &&
                    mouseY >= msg.copyAllButtonRect.top && mouseY <= msg.copyAllButtonRect.bottom) {
                    newHoveredButton = 2;
                    newHoveredMsgIdx = i;
                    break;
                }
                
                // 检查撤销按钮
                if (mouseX >= msg.undoButtonRect.left && mouseX <= msg.undoButtonRect.right &&
                    mouseY >= msg.undoButtonRect.top && mouseY <= msg.undoButtonRect.bottom) {
                    newHoveredButton = 3;
                    newHoveredMsgIdx = i;
                    break;
                }
            }
            
            // 如果悬停状态改变，更新 Tooltip
            if (newHoveredButton != data->hoveredButton || newHoveredMsgIdx != data->hoveredMsgIdx) {
                data->hoveredButton = newHoveredButton;
                data->hoveredMsgIdx = newHoveredMsgIdx;
                
                // 重绘窗口以显示/隐藏按钮背景
                InvalidateRect(hWnd, NULL, FALSE);

                // 移除旧的 Tooltip
                TOOLINFO ti = {};
                ti.cbSize = sizeof(TOOLINFO);
                ti.hwnd = hWnd;
                ti.uId = 1;
                SendMessage(data->hTooltip, TTM_DELTOOL, 0, (LPARAM)&ti);
                
                // 如果有悬停的按钮，添加新 Tooltip
                if (newHoveredButton > 0 && newHoveredMsgIdx < data->messages.size()) {
                    ChatMessage& msg = data->messages[newHoveredMsgIdx];
                    RECT btnRect = {0, 0, 0, 0};
                    const wchar_t* tipText = L"";
                    
                    if (newHoveredButton == 1) {
                        btnRect = msg.retryButtonRect;
                        tipText = L"重试";
                    } else if (newHoveredButton == 2) {
                        btnRect = msg.copyAllButtonRect;
                        tipText = L"复制";
                    } else if (newHoveredButton == 3) {
                        btnRect = msg.undoButtonRect;
                        tipText = L"撤销";
                    }
                    
                    ZeroMemory(&ti, sizeof(TOOLINFO));
                    ti.cbSize = sizeof(TOOLINFO);
                    ti.uFlags = TTF_SUBCLASS;
                    ti.hwnd = hWnd;
                    ti.uId = 1;
                    ti.rect = btnRect;
                    ti.lpszText = const_cast<wchar_t*>(tipText);
                    SendMessage(data->hTooltip, TTM_ADDTOOL, 0, (LPARAM)&ti);
                    
                    // 立即显示 Tooltip
                    SendMessage(data->hTooltip, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);
                    SendMessage(data->hTooltip, TTM_TRACKPOSITION, 0, MAKELPARAM(mouseX + 10, mouseY + 10));
                }
            } else if (newHoveredButton > 0) {
                // 更新 Tooltip 位置
                SendMessage(data->hTooltip, TTM_TRACKPOSITION, 0, MAKELPARAM(mouseX + 10, mouseY + 10));
            }
        }
        return 0;
        
    case WM_AI_STREAM_RESULT:
        {
            if (wParam == 1) { // Chunk
                std::wstring* pChunk = (std::wstring*)lParam;
                if (pChunk && data) {
                    wchar_t logMsg[256];
                    swprintf_s(logMsg, L"[WM_AI_STREAM_RESULT] Chunk received, length: %d, total: %d", 
                              (int)pChunk->length(), (int)(data->streamBuffer.length() + pChunk->length()));
                    WriteDebugLog(logMsg);
                    
                    if (data->streamBuffer == L"正在思考...") {
                        data->streamBuffer.clear();
                        // 重新解析当前消息（清除"正在思考..."）
                        if (data->streamingMsgIdx >= 0 && data->streamingMsgIdx < (int)data->messages.size()) {
                            ChatMessage& msg = data->messages[data->streamingMsgIdx];
                            msg.blocks.clear();
                        }
                    }
                    AppendAIMessageStream(data, *pChunk);
                    delete pChunk;
                    InvalidateRect(hWnd, NULL, TRUE);
                } else if (pChunk) {
                    delete pChunk;
                }
            } else if (wParam == 2) { // Finish
                if (data) {
                    // 保存当前的streamingMsgIdx，因为FinishAIMessage会将其重置为-1
                    int currentMsgIdx = data->streamingMsgIdx;
                    
                    WriteDebugLog(L"[WM_AI_STREAM_RESULT] Stream finished");
                    wchar_t totalLengthMsg[256];
                    swprintf_s(totalLengthMsg, L"[WM_AI_STREAM_RESULT] Total response length: %d characters", 
                              (int)data->streamBuffer.length());
                    WriteDebugLog(totalLengthMsg);
                    
                    std::wstring debugMsg = L"[WM_AI_STREAM_RESULT] Mode: ";
                    debugMsg += g_ModeText;
                    WriteDebugLog(debugMsg.c_str());
                    
                    // 记录完整响应内容（前500字符）
                    std::wstring responsePreview = data->streamBuffer.substr(0, (std::min)(500, (int)data->streamBuffer.length()));
                    WriteDebugLog(L"[WM_AI_STREAM_RESULT] Response preview:");
                    WriteDebugLog(responsePreview.c_str());
                    
                    // 保存完整响应到临时变量，因为FinishAIMessage会清空streamBuffer
                    std::wstring fullResponse = data->streamBuffer;
                    
                    // 先完成消息处理（会清空streamBuffer）
                    FinishAIMessage(data);
                    
                    // 使用临时变量保存日志和应用代码
                    SaveAIResponseLog(fullResponse);
                    WriteDebugLog(L"[WM_AI_STREAM_RESULT] Response saved to log file");
                    
                    // 如果模式是Edit或Agent，尝试应用代码修改
                    if (wcscmp(g_ModeText, L"Edit") == 0 || wcscmp(g_ModeText, L"Agent") == 0) {
                        WriteDebugLog(L"[WM_AI_STREAM_RESULT] Calling ApplyAICodeChanges");
                        // 传递保存的消息索引给ApplyAICodeChanges
                        ApplyAICodeChanges(fullResponse, currentMsgIdx);
                    } else {
                        std::wstring modeLog = L"[WM_AI_STREAM_RESULT] Not in Edit/Agent mode, current mode: ";
                        modeLog += g_ModeText;
                        WriteDebugLog(modeLog.c_str());
                    }
                    
                    InvalidateRect(hWnd, NULL, TRUE);
                    
                    // 重绘发送按钮，切换回发送图标
                    InvalidateRect(hChatSendBtn, NULL, FALSE);
                }
            } else {
                // 兼容旧代码（如果有）
                std::wstring* pReply = (std::wstring*)lParam;
                if (pReply) delete pReply;
            }
        }
        return 0;
        
    case WM_TIMER:
        if (wParam == 1 && data) {
            // 更新动画帧
            data->animationFrame = (data->animationFrame + 1) % 8;
            
            // 如果正在流式输出，刷新显示以更新动画
            if (data->isStreaming) {
                InvalidateRect(hWnd, NULL, FALSE);
            }
        }
        return 0;
        
    case WM_MOUSELEAVE:
        {
            if (data && !data->isDraggingRightBorder && data->isRightBorderHover) {
                data->isRightBorderHover = false;
                HWND hParent = GetParent(hWnd);
                if (hParent) {
                    InvalidateRect(hParent, NULL, FALSE);
                }
            }
        }
        return 0;
        
    case WM_DESTROY:
        if (data) {
            // 销毁定时器
            KillTimer(hWnd, 1);
            
            // 销毁 Tooltip
            if (data->hTooltip) {
                DestroyWindow(data->hTooltip);
                data->hTooltip = nullptr;
            }
            delete data;
            SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
        }
        return 0;
    }
    
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// 输入框子类化
LRESULT CALLBACK ChatInputSubclassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_KEYDOWN) {
        if (wParam == VK_RETURN) {
            // SHIFT+回车：换行（让默认处理）
            if (GetKeyState(VK_SHIFT) & 0x8000) {
                return CallWindowProc(g_OriginalEditProc, hWnd, message, wParam, lParam);
            }
            // 单独回车：发送消息
            else {
                PostMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(2003, BN_CLICKED), (LPARAM)hChatSendBtn);
                return 0;
            }
        }
    } else if (message == WM_CHAR || message == WM_KEYUP || message == WM_PASTE || message == WM_CUT) {
        // 延迟检查输入框内容，以便文本已经更新
        PostMessage(GetParent(hWnd), WM_USER + 100, 0, 0);
    }
    return CallWindowProc(g_OriginalEditProc, hWnd, message, wParam, lParam);
}

// 主窗口AI聊天面板过程
LRESULT CALLBACK AIChatWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_NCHITTEST:
        {
            // 让左边缘5px的鼠标消息穿透到主窗口，用于调整窗口大小
            POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            ScreenToClient(hWnd, &pt);
            if (pt.x < 5) {
                return HTTRANSPARENT;
            }
            return HTCLIENT;
        }
    case WM_CREATE:
        {
            hChatControlFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
            
            // 注册并创建AI聊天窗口
            RegisterAIChatWindowClass(hInst);
            RegisterCustomMenuClass(hInst);
            hChatWindow = CreateWindowW(L"AIChatWindow", nullptr,
                WS_CHILD | WS_VISIBLE,  // 移除系统滚动条，使用自定义滚动条
                0, 0, 0, 0, hWnd, (HMENU)2001, hInst, nullptr);
            
            // 创建输入框
            hChatInput = CreateWindowW(L"EDIT", L"",
                WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
                0, 0, 0, 0, hWnd, (HMENU)2002, hInst, nullptr);
            SendMessageW(hChatInput, WM_SETFONT, (WPARAM)hChatControlFont, TRUE);
            
            // 子类化输入框以处理焦点事件
            g_OriginalInputProc = (WNDPROC)SetWindowLongPtr(hChatInput, GWLP_WNDPROC, (LONG_PTR)InputSubclassProc);
            
            // 初始化输入框边框颜色为编辑器边框颜色
            g_InputBorderColor = g_CurrentTheme.grid;
            
            // 不再创建combo box，改为自定义绘制
            // 加载保存的设置
            LoadChatSettings();
            
            // 初始化模型文本
            if (!g_AIModels.empty() && g_CurrentModelIndex >= 0 && g_CurrentModelIndex < (int)g_AIModels.size()) {
                g_ModelText = g_AIModels[g_CurrentModelIndex].name;
            }
            
            // 创建发送按钮（初始禁用）
            hChatSendBtn = CreateWindowW(L"BUTTON", L"",
                WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                0, 0, 0, 0, hWnd, (HMENU)2003, hInst, nullptr);
            EnableWindow(hChatSendBtn, FALSE);
            
            g_hSendIcon = LoadPNGImage(L"img/KHCFDC_发送.png");
            g_hPauseIcon = LoadPNGImage(L"img/暂停.png");
            g_hRetryIcon = LoadPNGImage(L"img/重试2.png");
            g_hCopyIcon = LoadPNGImage(L"img/复制.png");
            g_hUndoIcon = LoadPNGImage(L"img/撤销.png");
            g_hCodeCopyIcon = LoadPNGImage(L"img/复制.png");   // 代码块复制按钮
            g_hCodeInsertIcon = LoadPNGImage(L"img/插入2.png"); // 代码块插入按钮
            
            g_OriginalEditProc = (WNDPROC)SetWindowLongPtr(hChatInput, GWLP_WNDPROC, (LONG_PTR)ChatInputSubclassProc);
        }
        return 0;
        
    case WM_SIZE:
        {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            int statusBarHeight = 40;  // 状态栏高度
            int fileNameHeight = 22;  // 文件名高度
            int inputHeight = 60;      // 输入框高度
            int buttonHeight = 32;     // 按钮区高度
            int padding = 8;
            int controlAreaHeight = statusBarHeight + fileNameHeight + 4 + inputHeight + 6 + buttonHeight + padding;
            
            if (hChatWindow) {
                // 聊天窗口占据从顶部到控制区
                MoveWindow(hChatWindow, 0, 0, width, height - controlAreaHeight, TRUE);
            }
            
            int controlAreaTop = height - controlAreaHeight;
            
            if (hChatInput) {
                // 输入框在状态栏+文件名下方
                int inputTop = controlAreaTop + statusBarHeight + fileNameHeight + 4;
                MoveWindow(hChatInput, 
                    padding + 2, 
                    inputTop,
                    width - 2 * padding - 4, 
                    inputHeight, 
                    TRUE);
            }
            
            int btnAreaTop = controlAreaTop + statusBarHeight + fileNameHeight + 4 + inputHeight + 6;
            
            // 计算模式选择区域
            g_ModeRect.left = padding + 6;
            g_ModeRect.top = btnAreaTop;
            g_ModeRect.right = g_ModeRect.left + 75;
            g_ModeRect.bottom = btnAreaTop + buttonHeight;
            
            // 计算模型选择区域
            g_ModelRect.left = g_ModeRect.right + 6;
            g_ModelRect.top = btnAreaTop;
            g_ModelRect.right = g_ModelRect.left + 145;
            g_ModelRect.bottom = btnAreaTop + buttonHeight;
            
            if (hChatSendBtn) {
                int x = width - 16 - padding - 12;
                int y = btnAreaTop + (buttonHeight - 16) / 2;
                MoveWindow(hChatSendBtn, x, y, 16, 16, TRUE);
            }
        }
        return 0;
        
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            int wmEvent = HIWORD(wParam);
            
            if (wmId == 2003) {
                WriteDebugLog(L"[WM_COMMAND] Send button clicked");
                
                // 检查是否正在流式输出
                AIChatData* data = (AIChatData*)GetWindowLongPtr(hChatWindow, GWLP_USERDATA);
                if (data && data->isStreaming) {
                    // 点击停止按钮
                    WriteDebugLog(L"[WM_COMMAND] Stop button clicked, stopping AI response");
                    g_StopRequested = true;
                    data->isStreaming = false;
                    
                    // 完成当前消息
                    FinishAIMessage(data);
                    
                    // 重绘发送按钮，切换回发送图标
                    InvalidateRect(hChatSendBtn, NULL, FALSE);
                    InvalidateRect(hChatWindow, NULL, TRUE);
                    return 0;
                }
                
                // 检查按钮是否启用
                if (!IsWindowEnabled(hChatSendBtn)) {
                    WriteDebugLog(L"[WM_COMMAND] Send button is disabled, ignoring click");
                    return 0;
                }
                int len = GetWindowTextLengthW(hChatInput);
                WriteDebugLog((L"[WM_COMMAND] Input text length: " + std::to_wstring(len)).c_str());
                
                if (len > 0 && hChatWindow) {
                    std::wstring userMsg(len + 1, 0);
                    GetWindowTextW(hChatInput, &userMsg[0], len + 1);
                    userMsg.resize(len);
                    
                    // 获取聊天窗口数据
                    AIChatData* data = (AIChatData*)GetWindowLongPtr(hChatWindow, GWLP_USERDATA);
                    if (data) {
                        AddUserMessage(data, userMsg);
                        InvalidateRect(hChatWindow, NULL, TRUE);
                    }
                    
                    SetWindowTextW(hChatInput, L"");
                    
                    // 根据模式文本设置mode
                    std::wstring mode;
                    if (wcscmp(g_ModeText, L"Ask") == 0) {
                        mode = L"ASK";
                    } else if (wcscmp(g_ModeText, L"Edit") == 0) {
                        mode = L"EDIT";
                    } else {
                        mode = L"AGENT";
                    }
                    
                    WriteDebugLog((L"[WM_COMMAND] Mode: " + mode).c_str());
                    WriteDebugLog((L"[WM_COMMAND] Model index: " + std::to_wstring(g_CurrentModelIndex)).c_str());
                    
                    if (g_CurrentModelIndex >= 0 && g_CurrentModelIndex < (int)g_AIModels.size() && data) {
                        StartAIMessage(data);
                        AppendAIMessageStream(data, L"⏳ 正在运行中...\n\n");  // 添加换行，后续流式输出会在下方显示
                        InvalidateRect(hChatWindow, NULL, TRUE);
                        
                        // 确保按钮在流式输出期间保持启用（作为停止按钮）
                        EnableWindow(hChatSendBtn, TRUE);
                        
                        // 重绘发送按钮，切换为暂停图标
                        InvalidateRect(hChatSendBtn, NULL, FALSE);

                        AIRequestParam* param = new AIRequestParam();
                        param->hChatWindow = hChatWindow;
                        param->userMsg = userMsg;
                        param->modelIndex = g_CurrentModelIndex;
                        param->mode = mode;

                        WriteDebugLog(L"[WM_COMMAND] Creating AI request thread...");
                        HANDLE hThread = CreateThread(NULL, 0, AIRequestThreadProc, param, 0, NULL);
                        if (hThread) {
                            WriteDebugLog(L"[WM_COMMAND] Thread created successfully");
                            CloseHandle(hThread);
                        } else {
                            WriteDebugLog(L"[WM_COMMAND] ERROR: Failed to create thread");
                        }
                    }
                }
            }

        }
        return 0;
    
    case WM_LBUTTONUP:
        return 0;
    
    case WM_MOUSELEAVE:
        return 0;
    
    case WM_SETCURSOR:
        {
            // 获取鼠标位置
            POINT pt;
            GetCursorPos(&pt);
            ScreenToClient(hWnd, &pt);
            
            // 检查是否在模式选择区域
            extern RECT g_ModeRect;
            if (PtInRect(&g_ModeRect, pt)) {
                SetCursor(LoadCursor(NULL, IDC_ARROW));
                return TRUE;
            }
            
            // 检查是否在模型选择区域
            extern RECT g_ModelRect;
            if (PtInRect(&g_ModelRect, pt)) {
                SetCursor(LoadCursor(NULL, IDC_ARROW));
                return TRUE;
            }
            
            // 检查是否在发送按钮区域
            if (hChatSendBtn) {
                RECT btnRect;
                GetWindowRect(hChatSendBtn, &btnRect);
                POINT screenPt;
                GetCursorPos(&screenPt);
                if (PtInRect(&btnRect, screenPt)) {
                    SetCursor(LoadCursor(NULL, IDC_ARROW));
                    return TRUE;
                }
            }
        }
        break;
    
    case WM_USER + 100:
        // 检查输入框内容并更新按钮状态
        if (hChatInput && hChatSendBtn) {
            // 如果正在流式输出，按钮应该保持启用（作为停止按钮）
            AIChatData* data = (AIChatData*)GetWindowLongPtr(hChatWindow, GWLP_USERDATA);
            if (data && data->isStreaming) {
                EnableWindow(hChatSendBtn, TRUE);
            } else {
                int len = GetWindowTextLengthW(hChatInput);
                EnableWindow(hChatSendBtn, len > 0);
            }
            InvalidateRect(hChatSendBtn, NULL, TRUE);
        }
        return 0;
        
    case WM_CTLCOLOREDIT:
        {
            // 输入框使用特定背景色 RGB(49, 49, 49)
            HDC hdcEdit = (HDC)wParam;
            SetTextColor(hdcEdit, g_CurrentTheme.text);
            SetBkColor(hdcEdit, RGB(49, 49, 49));
            static HBRUSH hInputBrush = CreateSolidBrush(RGB(49, 49, 49));
            return (LRESULT)hInputBrush;
        }
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORLISTBOX:
        {
            HDC hdcEdit = (HDC)wParam;
            SetTextColor(hdcEdit, g_CurrentTheme.text);
            SetBkColor(hdcEdit, g_CurrentTheme.bg);
            return (LRESULT)g_CurrentTheme.hBgBrush;
        }
        
    case WM_DRAWITEM:
        {
            LPDRAWITEMSTRUCT lpDis = (LPDRAWITEMSTRUCT)lParam;
            
            if (lpDis->CtlID == 2003) {  // 发送按钮
                HDC hdc = lpDis->hDC;
                RECT rect = lpDis->rcItem;
                
                // 填充背景为输入框背景色（RGB(49, 49, 49)）
                HBRUSH hBgBrush = CreateSolidBrush(RGB(49, 49, 49));
                FillRect(hdc, &rect, hBgBrush);
                DeleteObject(hBgBrush);

                // 检查按钮是否禁用
                bool isEnabled = IsWindowEnabled(hChatSendBtn);
                
                // 检查是否悬停，绘制圆角矩形背景
                AIChatData* data = (AIChatData*)GetWindowLongPtr(hChatWindow, GWLP_USERDATA);
                if (data && data->isSendButtonHovered && isEnabled) {
                    // 绘制扩展的圆角矩形
                    RECT hoverRect = rect;
                    InflateRect(&hoverRect, 2, 2);
                    HBRUSH hHoverBrush = CreateSolidBrush(RGB(70, 70, 70));
                    HPEN hHoverPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
                    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hHoverBrush);
                    HPEN hOldPen = (HPEN)SelectObject(hdc, hHoverPen);
                    RoundRect(hdc, hoverRect.left, hoverRect.top, hoverRect.right, hoverRect.bottom, 5, 5);
                    SelectObject(hdc, hOldPen);
                    SelectObject(hdc, hOldBrush);
                    DeleteObject(hHoverPen);
                    DeleteObject(hHoverBrush);
                }
                
                // 根据 isStreaming 状态切换图标：正在返回时显示暂停图标，否则显示发送图标
                HBITMAP hIcon = (data && data->isStreaming) ? g_hPauseIcon : g_hSendIcon;
                
                if (hIcon) {
                    HDC hdcMem = CreateCompatibleDC(hdc);
                    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hIcon);
                    BITMAP bm;
                    GetObject(hIcon, sizeof(bm), &bm);
                    
                    int iconSize = (std::min)(rect.right - rect.left, rect.bottom - rect.top);
                    int iconX = rect.left + (rect.right - rect.left - iconSize) / 2;
                    int iconY = rect.top + (rect.bottom - rect.top - iconSize) / 2;
                    
                    // 根据按钮启用状态调整透明度
                    BYTE alpha = isEnabled ? 255 : 100;
                    BLENDFUNCTION bf = { AC_SRC_OVER, 0, alpha, AC_SRC_ALPHA };
                    AlphaBlend(hdc, iconX, iconY, iconSize, iconSize,
                              hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, bf);
                            
                    SelectObject(hdcMem, hOldBitmap);
                    DeleteDC(hdcMem);
                }
                return TRUE;
            }
        }
        return 0;
    
    case WM_LBUTTONDOWN:
        {
            POINT pt;
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            
            // 检测是否点击了状态栏（但不是接受和撤销按钮）
            if (PtInRect(&g_StatusBarRect, pt) && !g_PendingEdits.empty() &&
                !PtInRect(&g_AcceptBtnRect, pt) && !PtInRect(&g_UndoBtnRect, pt)) {
                // 切换文件列表展开状态
                g_FileListExpanded = !g_FileListExpanded;
                InvalidateRect(hWnd, nullptr, TRUE);
                return 0;
            }
            
            // 检测是否点击了接受按钮
            if (PtInRect(&g_AcceptBtnRect, pt) && !g_PendingEdits.empty()) {
                // 接受所有更改 - 保存所有文件
                extern EditorData* g_EditorData;
                extern bool SaveFile(const std::wstring& filePath, EditorDocument* doc);
                extern HWND hEditorWnd;
                
                for (auto& editInfo : g_PendingEdits) {
                    // 找到对应的文档
                    for (size_t i = 0; i < g_EditorData->documents.size(); i++) {
                        EditorDocument* doc = g_EditorData->documents[i];
                        if (doc->filePath == editInfo.filePath) {
                            // 应用最终修改（删除DIFF_DELETED行）
                            std::vector<std::wstring> finalLines;
                            for (size_t j = 0; j < doc->lines.size(); j++) {
                                if (j >= doc->diffTypes.size() || doc->diffTypes[j] != DIFF_DELETED) {
                                    finalLines.push_back(doc->lines[j]);
                                }
                            }
                            doc->lines = finalLines;
                            doc->diffTypes.clear();
                            doc->showDiff = false;
                            doc->modified = true;  // 标记为已修改以便SaveFile保存
                            
                            // 使用SaveFile函数保存
                            SaveFile(doc->filePath, doc);
                            
                            // 刷新编辑器显示
                            if (hEditorWnd) {
                                InvalidateRect(hEditorWnd, NULL, TRUE);
                            }
                            break;
                        }
                    }
                }
                
                // 清空待处理列表
                g_PendingEdits.clear();
                InvalidateRect(hWnd, nullptr, TRUE);
                
                // 刷新编辑器
                extern HWND hEditorWnd;
                if (hEditorWnd) {
                    InvalidateRect(hEditorWnd, nullptr, TRUE);
                }
                
                return 0;
            }
            
            // 检测是否点击了撤销按钮
            if (PtInRect(&g_UndoBtnRect, pt) && !g_PendingEdits.empty()) {
                // 撤销所有更改 - 恢复原始内容
                extern EditorData* g_EditorData;
                for (auto& editInfo : g_PendingEdits) {
                    // 找到对应的文档
                    for (size_t i = 0; i < g_EditorData->documents.size(); i++) {
                        EditorDocument* doc = g_EditorData->documents[i];
                        if (doc->filePath == editInfo.filePath) {
                            // 恢复原始内容
                            doc->lines = doc->originalLines;
                            doc->diffTypes.clear();
                            doc->showDiff = false;
                            doc->modified = false;
                            ExplorerSetFileModified(doc->filePath, false);
                            break;
                        }
                    }
                }
                
                // 清空待处理列表
                g_PendingEdits.clear();
                InvalidateRect(hWnd, nullptr, TRUE);
                
                // 刷新编辑器
                extern HWND hEditorWnd;
                if (hEditorWnd) {
                    InvalidateRect(hEditorWnd, nullptr, TRUE);
                }
                
                return 0;
            }
            
            // 检测是否点击了模式选择区域
            if (PtInRect(&g_ModeRect, pt)) {
                // 准备菜单项数据
                std::vector<MenuItemData> items;
                items.push_back({L"Ask", false, g_CurrentChatMode == 0});
                items.push_back({L"Edit", false, g_CurrentChatMode == 1});
                items.push_back({L"Agent", false, g_CurrentChatMode == 2});
                
                // 显示自定义菜单
                POINT screenPt = {g_ModeRect.left, g_ModeRect.bottom};
                ClientToScreen(hWnd, &screenPt);
                int cmd = ShowCustomMenu(hWnd, screenPt.x, screenPt.y, items);
                
                if (cmd > 0) {
                    g_CurrentChatMode = cmd - 1;
                    const wchar_t* modes[] = {L"Ask", L"Edit", L"Agent"};
                    g_ModeText = modes[g_CurrentChatMode];
                    SaveChatSettings();
                    InvalidateRect(hWnd, &g_ModeRect, FALSE);
                }
                
                return 0;
            }
            
            // 检测是否点击了模型选择区域
            if (PtInRect(&g_ModelRect, pt)) {
                // 准备菜单项数据
                std::vector<MenuItemData> items;
                
                for (const auto& model : g_AIModels) {
                    items.push_back({model.name, false, (int)items.size() == g_CurrentModelIndex});
                }
                
                // 添加分隔符
                items.push_back({L"", true, false});
                
                // 添加管理模型项
                items.push_back({L"--- 管理模型 ---", false, false});
                
                // 显示自定义菜单
                POINT screenPt = {g_ModelRect.left, g_ModelRect.bottom};
                ClientToScreen(hWnd, &screenPt);
                int cmd = ShowCustomMenu(hWnd, screenPt.x, screenPt.y, items);
                
                if (cmd > 0) {
                    if (cmd == (int)items.size()) {
                        // 最后一项：管理模型
                        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, ModelSettingsDlg);
                    } else if (cmd <= (int)g_AIModels.size()) {
                        // 选择模型
                        g_CurrentModelIndex = cmd - 1;
                        g_ModelText = g_AIModels[g_CurrentModelIndex].name;
                        SaveChatSettings();
                        InvalidateRect(hWnd, &g_ModelRect, FALSE);
                    }
                }
                
                return 0;
            }
        }
        break;
        
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            RECT rect;
            GetClientRect(hWnd, &rect);
            
            int statusBarHeight = 40;
            int fileNameHeight = 22;
            int inputHeight = 60;
            int buttonHeight = 32;
            int padding = 8;
            int controlAreaHeight = statusBarHeight + fileNameHeight + 4 + inputHeight + 6 + buttonHeight + padding;
            int controlAreaTop = rect.bottom - controlAreaHeight;
            
            // 绘制聊天区域背景
            RECT chatAreaRect = {0, 0, rect.right, controlAreaTop};
            FillRect(hdc, &chatAreaRect, g_CurrentTheme.hBgBrush);
            
            // 绘制控制区域背景
            RECT controlRect = {0, controlAreaTop, rect.right, rect.bottom};
            FillRect(hdc, &controlRect, g_CurrentTheme.hBgBrush);
            
            // 如果有待处理的编辑，在控制区顶部显示状态方框
            if (!g_PendingEdits.empty()) {
                RECT statusOuterRect;
                statusOuterRect.left = padding;
                statusOuterRect.top = controlAreaTop + 2;
                statusOuterRect.right = rect.right - padding;
                statusOuterRect.bottom = controlAreaTop + statusBarHeight - 2;
                
                // 保存状态栏矩形用于点击检测
                g_StatusBarRect = statusOuterRect;
                
                // 绘制状态方框边框
                HPEN hStatusPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
                HPEN hOldStatusPen = (HPEN)SelectObject(hdc, hStatusPen);
                HBRUSH hStatusBrush = CreateSolidBrush(g_CurrentTheme.bg);  // 使用编辑器背景色
                HBRUSH hOldStatusBrush = (HBRUSH)SelectObject(hdc, hStatusBrush);
                
                RoundRect(hdc, statusOuterRect.left, statusOuterRect.top, 
                         statusOuterRect.right, statusOuterRect.bottom, 6, 6);
                
                SelectObject(hdc, hOldStatusPen);
                SelectObject(hdc, hOldStatusBrush);
                DeleteObject(hStatusPen);
                DeleteObject(hStatusBrush);
                
                // 设置字体和颜色
                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, RGB(200, 200, 200));
                HFONT hStatusFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                    CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
                HFONT hOldStatusFont = (HFONT)SelectObject(hdc, hStatusFont);
                
                // 显示文本："已更改 X 个文件"
                std::wstring statusText = L"已更改 ";
                statusText += std::to_wstring(g_PendingEdits.size());
                statusText += L" 个文件";
                
                RECT textRect = statusOuterRect;
                textRect.left += 12;
                DrawTextW(hdc, statusText.c_str(), -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                
                // 计算文本宽度
                SIZE textSize;
                GetTextExtentPoint32W(hdc, statusText.c_str(), (int)statusText.length(), &textSize);
                
                // 绘制"接受"按钮（圆角矩形）
                g_AcceptBtnRect.right = statusOuterRect.right - 12;
                g_AcceptBtnRect.left = g_AcceptBtnRect.right - 60;
                g_AcceptBtnRect.top = statusOuterRect.top + 6;
                g_AcceptBtnRect.bottom = statusOuterRect.bottom - 6;
                
                HBRUSH hAcceptBrush = CreateSolidBrush(RGB(0, 120, 215));
                HPEN hAcceptPen = CreatePen(PS_SOLID, 1, RGB(0, 120, 215));
                HBRUSH hOldAcceptBrush = (HBRUSH)SelectObject(hdc, hAcceptBrush);
                HPEN hOldAcceptPen = (HPEN)SelectObject(hdc, hAcceptPen);
                
                RoundRect(hdc, g_AcceptBtnRect.left, g_AcceptBtnRect.top, g_AcceptBtnRect.right, g_AcceptBtnRect.bottom, 4, 4);
                
                SelectObject(hdc, hOldAcceptPen);
                SelectObject(hdc, hOldAcceptBrush);
                DeleteObject(hAcceptPen);
                DeleteObject(hAcceptBrush);
                
                SetTextColor(hdc, RGB(255, 255, 255));
                DrawTextW(hdc, L"接受", -1, &g_AcceptBtnRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                
                // 绘制"撤销"按钮（圆角矩形）
                g_UndoBtnRect.right = g_AcceptBtnRect.left - 8;
                g_UndoBtnRect.left = g_UndoBtnRect.right - 60;
                g_UndoBtnRect.top = statusOuterRect.top + 6;
                g_UndoBtnRect.bottom = statusOuterRect.bottom - 6;
                
                HBRUSH hUndoBrush = CreateSolidBrush(RGB(80, 80, 80));
                HPEN hUndoPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
                HBRUSH hOldUndoBrush = (HBRUSH)SelectObject(hdc, hUndoBrush);
                HPEN hOldUndoPen = (HPEN)SelectObject(hdc, hUndoPen);
                
                RoundRect(hdc, g_UndoBtnRect.left, g_UndoBtnRect.top, g_UndoBtnRect.right, g_UndoBtnRect.bottom, 4, 4);
                
                SelectObject(hdc, hOldUndoPen);
                SelectObject(hdc, hOldUndoBrush);
                DeleteObject(hUndoPen);
                DeleteObject(hUndoBrush);
                
                SetTextColor(hdc, RGB(200, 200, 200));
                DrawTextW(hdc, L"撤销", -1, &g_UndoBtnRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                
                SelectObject(hdc, hOldStatusFont);
                DeleteObject(hStatusFont);
                
                // 如果展开状态，绘制文件列表
                if (g_FileListExpanded) {
                    int listTop = statusOuterRect.bottom + 4;
                    int itemHeight = 24;
                    int listHeight = (int)g_PendingEdits.size() * itemHeight + 8;
                    
                    RECT listRect;
                    listRect.left = padding;
                    listRect.top = listTop;
                    listRect.right = rect.right - padding;
                    listRect.bottom = listTop + listHeight;
                    
                    // 绘制列表背景
                    HBRUSH hListBrush = CreateSolidBrush(g_CurrentTheme.bg);
                    HPEN hListPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
                    HBRUSH hOldListBrush = (HBRUSH)SelectObject(hdc, hListBrush);
                    HPEN hOldListPen = (HPEN)SelectObject(hdc, hListPen);
                    
                    RoundRect(hdc, listRect.left, listRect.top, listRect.right, listRect.bottom, 6, 6);
                    
                    SelectObject(hdc, hOldListPen);
                    SelectObject(hdc, hOldListBrush);
                    DeleteObject(hListPen);
                    DeleteObject(hListBrush);
                    
                    // 绘制文件列表
                    HFONT hFileFont = CreateFont(13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
                    HFONT hOldFileFont = (HFONT)SelectObject(hdc, hFileFont);
                    
                    int yPos = listRect.top + 4;
                    for (const auto& editInfo : g_PendingEdits) {
                        RECT itemRect = {listRect.left + 8, yPos, listRect.right - 8, yPos + itemHeight};
                        
                        // 提取文件名
                        std::wstring fileName = editInfo.fileName;
                        
                        // 绘制文件名和统计信息
                        std::wstring itemText = fileName + L"  (+" + std::to_wstring(editInfo.addedLines) + 
                                               L" -" + std::to_wstring(editInfo.deletedLines) + L")";
                        
                        SetTextColor(hdc, RGB(180, 180, 180));
                        DrawTextW(hdc, itemText.c_str(), -1, &itemRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                        
                        yPos += itemHeight;
                    }
                    
                    SelectObject(hdc, hOldFileFont);
                    DeleteObject(hFileFont);
                }
            }
            
            // 绘制包裹输入框和按钮的圆角矩形边框（先绘制边框，文件名会在边框内部）
            RECT borderRect;
            borderRect.left = padding;
            borderRect.top = controlAreaTop + statusBarHeight + 2;  // 紧接状态栏下方
            borderRect.right = rect.right - padding;
            borderRect.bottom = rect.bottom - padding;
            
            // 先填充边框内部区域为 RGB(49, 49, 49)
            HBRUSH hInnerBrush = CreateSolidBrush(RGB(49, 49, 49));
            RECT innerRect = borderRect;
            InflateRect(&innerRect, -1, -1);
            FillRect(hdc, &innerRect, hInnerBrush);
            DeleteObject(hInnerBrush);
            
            // 绘制圆角矩形边框
            HPEN hBorderPen = CreatePen(PS_SOLID, 1, g_InputBorderColor);
            HPEN hOldBorderPen = (HPEN)SelectObject(hdc, hBorderPen);
            HBRUSH hNullBrush2 = (HBRUSH)GetStockObject(NULL_BRUSH);
            HBRUSH hOldNullBrush2 = (HBRUSH)SelectObject(hdc, hNullBrush2);
            
            RoundRect(hdc, borderRect.left, borderRect.top, borderRect.right, borderRect.bottom, 8, 8);
            
            SelectObject(hdc, hOldBorderPen);
            SelectObject(hdc, hOldNullBrush2);
            DeleteObject(hBorderPen);
            
            // 在边框内部顶部绘制文件名
            extern EditorData* g_EditorData;
            if (g_EditorData) {
                EditorDocument* doc = g_EditorData->GetActiveDoc();
                if (doc && !doc->filePath.empty()) {
                    // 提取文件名
                    std::wstring filePath = doc->filePath;
                    size_t lastSlash = filePath.find_last_of(L"\\/");
                    std::wstring fileName = (lastSlash != std::wstring::npos) ? filePath.substr(lastSlash + 1) : filePath;
                    
                    // 设置字体和颜色
                    SetBkMode(hdc, TRANSPARENT);
                    SetTextColor(hdc, RGB(150, 150, 150));  // 灰色文字
                    HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
                    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
                    
                    // 计算文件名宽度
                    SIZE textSize;
                    GetTextExtentPoint32W(hdc, fileName.c_str(), (int)fileName.length(), &textSize);
                    
                    // 文件名框在边框内部
                    int iconSize = 16;
                    int frameLeft = borderRect.left + 6;
                    int frameTop = borderRect.top + 4;
                    int frameRight = frameLeft + iconSize + 10 + textSize.cx + 8;
                    int frameBottom = frameTop + fileNameHeight - 2;
                    
                    HPEN hSolidPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
                    HPEN hOldSolidPen = (HPEN)SelectObject(hdc, hSolidPen);
                    HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
                    HBRUSH hOldNullBrush = (HBRUSH)SelectObject(hdc, hNullBrush);
                    
                    Rectangle(hdc, frameLeft, frameTop, frameRight, frameBottom);
                    
                    SelectObject(hdc, hOldSolidPen);
                    SelectObject(hdc, hOldNullBrush);
                    DeleteObject(hSolidPen);
                    
                    // 绘制文件图标占位（灰色小方块）
                    int iconX = frameLeft + 4;
                    int iconY = frameTop + (frameBottom - frameTop - iconSize) / 2;
                    
                    HBRUSH hIconBrush = CreateSolidBrush(RGB(80, 80, 80));
                    RECT iconRect = {iconX, iconY, iconX + iconSize, iconY + iconSize};
                    FillRect(hdc, &iconRect, hIconBrush);
                    DeleteObject(hIconBrush);
                    
                    // 在图标右侧绘制文件名
                    RECT fileNameRect = {iconX + iconSize + 6, frameTop + 2, frameRight - 4, frameBottom - 2};
                    DrawTextW(hdc, fileName.c_str(), -1, &fileNameRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                    
                    SelectObject(hdc, hOldFont);
                    DeleteObject(hFont);
                }
            }
            
            // 绘制模式选择区域
            if (!IsRectEmpty(&g_ModeRect)) {
                // 绘制文字
                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, g_CurrentTheme.text);
                HFONT hOldFont = (HFONT)SelectObject(hdc, hChatControlFont);
                
                RECT textRect = g_ModeRect;
                textRect.right -= 20;  // 为下拉箭头留空间
                DrawTextW(hdc, g_ModeText, -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                
                // 绘制V字形下拉箭头
                int arrowX = g_ModeRect.right - 15;
                int arrowY = (g_ModeRect.top + g_ModeRect.bottom) / 2;
                HPEN hArrowPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.text);
                HPEN hOldArrowPen = (HPEN)SelectObject(hdc, hArrowPen);
                
                MoveToEx(hdc, arrowX - 4, arrowY - 2, nullptr);
                LineTo(hdc, arrowX, arrowY + 2);
                LineTo(hdc, arrowX + 4, arrowY - 2);
                
                SelectObject(hdc, hOldArrowPen);
                DeleteObject(hArrowPen);
                
                SelectObject(hdc, hOldFont);
            }
            
            // 绘制模型选择区域
            if (!IsRectEmpty(&g_ModelRect)) {
                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, g_CurrentTheme.text);
                HFONT hOldFont = (HFONT)SelectObject(hdc, hChatControlFont);
                
                RECT textRect = g_ModelRect;
                textRect.right -= 20;  // 为下拉箭头留空间
                DrawTextW(hdc, g_ModelText.c_str(), -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                
                // 绘制V字形下拉箭头
                int arrowX = g_ModelRect.right - 15;
                int arrowY = (g_ModelRect.top + g_ModelRect.bottom) / 2;
                HPEN hArrowPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.text);
                HPEN hOldArrowPen = (HPEN)SelectObject(hdc, hArrowPen);
                
                MoveToEx(hdc, arrowX - 4, arrowY - 2, nullptr);
                LineTo(hdc, arrowX, arrowY + 2);
                LineTo(hdc, arrowX + 4, arrowY - 2);
                
                SelectObject(hdc, hOldArrowPen);
                DeleteObject(hArrowPen);
                
                SelectObject(hdc, hOldFont);
            }
            
            // 绘制右边框（整个面板的右侧）
            AIChatData* chatData = (AIChatData*)GetWindowLongPtr(hChatWindow, GWLP_USERDATA);
            if (chatData && (chatData->isRightBorderHover || chatData->isDraggingRightBorder)) {
                // 只有在悬停或拖动时才显示5px蓝色边框
                HBRUSH hBorderBrush = CreateSolidBrush(RGB(0, 122, 204));
                RECT borderRect = { rect.right - 5, 0, rect.right, rect.bottom };
                FillRect(hdc, &borderRect, hBorderBrush);
                DeleteObject(hBorderBrush);
            }
            
            EndPaint(hWnd, &ps);
        }
        return 0;
        
    case WM_MOUSEMOVE:
        {
            POINT pt;
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            
            AIChatData* data = (AIChatData*)GetWindowLongPtr(hChatWindow, GWLP_USERDATA);
            if (!data) return 0;
            
            // 检查鼠标是否在发送按钮上
            RECT btnRect;
            GetWindowRect(hChatSendBtn, &btnRect);
            ScreenToClient(hWnd, (LPPOINT)&btnRect);
            ScreenToClient(hWnd, ((LPPOINT)&btnRect) + 1);
            
            bool isHovered = PtInRect(&btnRect, pt);
            
            if (data->isSendButtonHovered != isHovered) {
                data->isSendButtonHovered = isHovered;
                InvalidateRect(hChatSendBtn, NULL, FALSE);
            }
        }
        return 0;
        
    case WM_ERASEBKGND:
        return 1;
        
    case WM_DESTROY:
        if (g_hSendIcon) {
            DeleteObject(g_hSendIcon);
            g_hSendIcon = nullptr;
        }
        if (g_hPauseIcon) {
            DeleteObject(g_hPauseIcon);
            g_hPauseIcon = nullptr;
        }
        if (g_hRetryIcon) {
            DeleteObject(g_hRetryIcon);
            g_hRetryIcon = nullptr;
        }
        if (g_hCopyIcon) {
            DeleteObject(g_hCopyIcon);
            g_hCopyIcon = nullptr;
        }
        if (g_hUndoIcon) {
            DeleteObject(g_hUndoIcon);
            g_hUndoIcon = nullptr;
        }
        if (g_hCodeCopyIcon) {
            DeleteObject(g_hCodeCopyIcon);
            g_hCodeCopyIcon = nullptr;
        }
        if (g_hCodeInsertIcon) {
            DeleteObject(g_hCodeInsertIcon);
            g_hCodeInsertIcon = nullptr;
        }
        break;
        
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
