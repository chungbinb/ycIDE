#include "EllEditor.h"
#include "Theme.h"
#include <sstream>
#include <fstream>
#include <algorithm>

extern HINSTANCE hInst;
extern AppTheme g_CurrentTheme;

// EllDocument 实现
EllDocument::EllDocument() 
    : cursorLine(0), cursorCol(0), scrollY(0), scrollX(0), 
      modified(false), hFileLock(INVALID_HANDLE_VALUE),
      hasSelection(false), selStartLine(0), selStartCol(0), 
      selEndLine(0), selEndCol(0), version(2) {
    fileName = L"未命名.ell";
}

EllDocument::~EllDocument() {
    if (hFileLock != INVALID_HANDLE_VALUE) {
        CloseHandle(hFileLock);
        hFileLock = INVALID_HANDLE_VALUE;
    }
}

void EllDocument::ParseContent() {
    commands.clear();
    allParams.clear();
    
    // 解析.ell文件内容
    DllCommand* currentCommand = nullptr;
    
    for (size_t i = 0; i < lines.size(); i++) {
        const std::wstring& line = lines[i];
        
        // 跳过空行
        if (line.empty()) continue;
        
        // 解析版本号
        if (line.find(L".版本") == 0) {
            size_t spacePos = line.find(L' ');
            if (spacePos != std::wstring::npos) {
                version = std::stoi(line.substr(spacePos + 1));
            }
            continue;
        }
        
        // 解析DLL命令
        if (line.find(L".DLL命令") == 0) {
            // 格式: .DLL命令 命令名, 返回类型, "DLL文件名", "DLL中命令名", 公开/私有, 备注
            commands.push_back(DllCommand());
            currentCommand = &commands.back();
            
            size_t pos = line.find(L' ');
            if (pos != std::wstring::npos) {
                std::wstring params = line.substr(pos + 1);
                
                // 按逗号分割参数
                std::vector<std::wstring> parts;
                std::wstring current;
                bool inQuote = false;
                
                for (wchar_t ch : params) {
                    if (ch == L'\"') {
                        inQuote = !inQuote;
                    } else if (ch == L',' && !inQuote) {
                        // 去除首尾空格
                        size_t start = current.find_first_not_of(L" \t");
                        size_t end = current.find_last_not_of(L" \t");
                        if (start != std::wstring::npos) {
                            parts.push_back(current.substr(start, end - start + 1));
                        } else {
                            parts.push_back(L"");
                        }
                        current.clear();
                    } else {
                        current += ch;
                    }
                }
                // 添加最后一个部分
                size_t start = current.find_first_not_of(L" \t");
                size_t end = current.find_last_not_of(L" \t");
                if (start != std::wstring::npos) {
                    parts.push_back(current.substr(start, end - start + 1));
                } else {
                    parts.push_back(L"");
                }
                
                // 解析各字段
                if (parts.size() >= 1) currentCommand->commandName = parts[0];
                if (parts.size() >= 2) currentCommand->returnType = parts[1];
                if (parts.size() >= 3) {
                    // 去除引号
                    currentCommand->dllFileName = parts[2];
                    if (currentCommand->dllFileName.size() >= 2 && 
                        currentCommand->dllFileName.front() == L'\"' && 
                        currentCommand->dllFileName.back() == L'\"') {
                        currentCommand->dllFileName = currentCommand->dllFileName.substr(1, currentCommand->dllFileName.size() - 2);
                    }
                }
                if (parts.size() >= 4) {
                    // 去除引号
                    currentCommand->dllCommandName = parts[3];
                    if (currentCommand->dllCommandName.size() >= 2 && 
                        currentCommand->dllCommandName.front() == L'\"' && 
                        currentCommand->dllCommandName.back() == L'\"') {
                        currentCommand->dllCommandName = currentCommand->dllCommandName.substr(1, currentCommand->dllCommandName.size() - 2);
                    }
                }
                if (parts.size() >= 5) {
                    currentCommand->isPublic = (parts[4] == L"公开");
                }
                if (parts.size() >= 6) {
                    currentCommand->comment = parts[5];
                }
            }
            continue;
        }
        
        // 解析参数
        if (line.find(L"    .参数") == 0 && currentCommand) {
            // 格式: .参数 参数名, 类型, [参考], [数组], [可空], [备注]
            DllParameter param;
            param.byRef = false;
            param.isArray = false;
            param.isOptional = false;
            
            // "    .参数 " = 4空格 + "." + "参数" + " " = 8个字符
            if (line.length() > 8) {
                std::wstring paramStr = line.substr(8);  // 直接跳过"    .参数 "
                
                // 按逗号分割
                std::vector<std::wstring> parts;
                std::wstring current;
                
                for (wchar_t ch : paramStr) {
                    if (ch == L',') {
                        size_t start = current.find_first_not_of(L" \t");
                        size_t end = current.find_last_not_of(L" \t");
                        if (start != std::wstring::npos) {
                            parts.push_back(current.substr(start, end - start + 1));
                        } else {
                            parts.push_back(L"");
                        }
                        current.clear();
                    } else {
                        current += ch;
                    }
                }
                // 添加最后一个部分
                size_t start = current.find_first_not_of(L" \t");
                size_t end = current.find_last_not_of(L" \t");
                if (start != std::wstring::npos) {
                    parts.push_back(current.substr(start, end - start + 1));
                } else {
                    parts.push_back(L"");
                }
                
                // 解析字段（格式：参数名, 类型）
                if (parts.size() >= 1) param.paramName = parts[0];
                if (parts.size() >= 2) param.paramType = parts[1];
                
                wchar_t debugMsg[512];
                swprintf_s(debugMsg, L"[ParseParam] parts.size()=%d, paramName=%s, paramType=%s\n", 
                    (int)parts.size(), param.paramName.c_str(), param.paramType.c_str());
                OutputDebugStringW(debugMsg);
                
                // 处理可选标记
                for (size_t j = 2; j < parts.size(); j++) {
                    if (parts[j] == L"参考") param.byRef = true;
                    else if (parts[j] == L"数组") param.isArray = true;
                    else if (parts[j] == L"可空") param.isOptional = true;
                    else if (!parts[j].empty()) param.comment = parts[j];
                }
            }
            
            currentCommand->paramLines.push_back((int)allParams.size());
            allParams.push_back(param);
            continue;
        }
    }
}

std::wstring EllDocument::GenerateContent() {
    std::wstring content;
    
    // 生成版本号
    content += L".版本 " + std::to_wstring(version) + L"\n";
    
    // 生成DLL命令
    for (const auto& cmd : commands) {
        content += L"\n.DLL命令 " + cmd.commandName + L", ";
        content += cmd.returnType + L", ";
        content += L"\"" + cmd.dllFileName + L"\", ";
        content += L"\"" + cmd.dllCommandName + L"\", ";
        content += (cmd.isPublic ? L"公开" : L"私有");
        if (!cmd.comment.empty()) {
            content += L", " + cmd.comment;
        }
        content += L"\n";
        
        // 生成参数
        for (int paramIdx : cmd.paramLines) {
            if (paramIdx < (int)allParams.size()) {
                const auto& param = allParams[paramIdx];
                content += L"    .参数 " + param.paramName + L", " + param.paramType;
                if (param.byRef) content += L", 参考";
                if (param.isArray) content += L", 数组";
                if (param.isOptional) content += L", 可空";
                if (!param.comment.empty()) content += L", " + param.comment;
                content += L"\n";
            }
        }
    }
    
    return content;
}

// EllEditorData 实现
EllEditorData::EllEditorData(HWND hwnd) 
    : hWnd(hwnd), activeDocIndex(-1), fontSize(16), rowHeight(24), tabHeight(30),
      scrollbarWidth(16), vScrollHover(false), hScrollHover(false),
      isDraggingVScroll(false), isDraggingHScroll(false),
      dragStartY(0), dragStartX(0), dragStartScrollY(0), dragStartScrollX(0),
      showWelcomePage(true) {
}

EllEditorData::~EllEditorData() {
    for (auto doc : documents) {
        delete doc;
    }
}

EllDocument* EllEditorData::GetActiveDoc() {
    if (activeDocIndex >= 0 && activeDocIndex < (int)documents.size()) {
        return documents[activeDocIndex];
    }
    return nullptr;
}

void EllEditorData::AddDocument(const std::wstring& filePath) {
    EllDocument* doc = new EllDocument();
    
    if (!filePath.empty()) {
        LoadEllFile(filePath, doc);
    }
    
    documents.push_back(doc);
    activeDocIndex = (int)documents.size() - 1;
    showWelcomePage = false;
}

void EllEditorData::CloseDocument(int index) {
    if (index >= 0 && index < (int)documents.size()) {
        delete documents[index];
        documents.erase(documents.begin() + index);
        
        if (activeDocIndex >= (int)documents.size()) {
            activeDocIndex = (int)documents.size() - 1;
        }
        
        if (documents.empty()) {
            showWelcomePage = true;
        }
    }
}

void EllEditorData::SetActiveDocument(int index) {
    if (index >= 0 && index < (int)documents.size()) {
        activeDocIndex = index;
    }
}

// 窗口类注册
bool RegisterEllEditorClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = EllEditorWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"EllEditorWindow";
    
    return RegisterClassExW(&wcex) != 0;
}

// 创建编辑器窗口
HWND CreateEllEditorWindow(HINSTANCE hInstance, HWND hParent) {
    return CreateWindowW(L"EllEditorWindow", L"DLL声明编辑器",
                        WS_CHILD | WS_VISIBLE,
                        0, 0, 800, 600,
                        hParent, nullptr, hInstance, nullptr);
}

// 加载.ell文件
bool LoadEllFile(const std::wstring& path, EllDocument* doc) {
    wchar_t debugMsg[512];
    swprintf_s(debugMsg, L"[LoadEllFile] 开始加载文件: %s\n", path.c_str());
    OutputDebugStringW(debugMsg);
    
    HANDLE hFile = CreateFileW(path.c_str(), GENERIC_READ | GENERIC_WRITE,
                               0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if (hFile == INVALID_HANDLE_VALUE) {
        hFile = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ,
                           NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) return false;
    }
    
    DWORD fileSize = GetFileSize(hFile, NULL);
    std::string content;
    if (fileSize > 0) {
        content.resize(fileSize);
        DWORD bytesRead;
        ReadFile(hFile, &content[0], fileSize, &bytesRead, NULL);
    }
    
    doc->hFileLock = hFile;
    
    // UTF-8 转 Unicode
    std::wstring wContent;
    if (!content.empty()) {
        int len = MultiByteToWideChar(CP_UTF8, 0, content.c_str(), -1, NULL, 0);
        if (len > 0) {
            wContent.resize(len - 1);
            MultiByteToWideChar(CP_UTF8, 0, content.c_str(), -1, &wContent[0], len);
        }
    }
    
    // 分割为行
    doc->lines.clear();
    std::wstringstream ss(wContent);
    std::wstring line;
    while (std::getline(ss, line, L'\n')) {
        if (!line.empty() && line.back() == L'\r') line.pop_back();
        doc->lines.push_back(line);
    }
    
    if (doc->lines.empty()) doc->lines.push_back(L"");
    
    doc->filePath = path;
    size_t lastSlash = path.find_last_of(L"\\/");
    doc->fileName = (lastSlash != std::wstring::npos) ? path.substr(lastSlash + 1) : path;
    doc->modified = false;
    
    // 解析内容
    doc->ParseContent();
    
    swprintf_s(debugMsg, L"[LoadEllFile] 文件加载成功，命令数量: %d, 参数数量: %d\n", 
        (int)doc->commands.size(), (int)doc->allParams.size());
    OutputDebugStringW(debugMsg);
    
    // 输出第一个命令的详细信息
    if (!doc->commands.empty()) {
        const auto& cmd = doc->commands[0];
        swprintf_s(debugMsg, L"[LoadEllFile] 第一个命令: %s, 返回类型: %s, DLL: %s, 公开: %d\n",
            cmd.commandName.c_str(), cmd.returnType.c_str(), cmd.dllFileName.c_str(), cmd.isPublic);
        OutputDebugStringW(debugMsg);
    }
    
    return true;
}

// 保存.ell文件
bool SaveEllFile(const std::wstring& path, EllDocument* doc) {
    if (doc->hFileLock != INVALID_HANDLE_VALUE) {
        CloseHandle(doc->hFileLock);
        doc->hFileLock = INVALID_HANDLE_VALUE;
    }
    
    std::wstring content = doc->GenerateContent();
    
    // Unicode 转 UTF-8
    int len = WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, NULL, 0, NULL, NULL);
    if (len <= 0) return false;
    
    std::string utf8Content(len - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, &utf8Content[0], len, NULL, NULL);
    
    // 写入文件
    HANDLE hFile = CreateFileW(path.c_str(), GENERIC_WRITE, 0, NULL,
                               CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return false;
    
    DWORD bytesWritten;
    bool result = WriteFile(hFile, utf8Content.c_str(), (DWORD)utf8Content.size(), &bytesWritten, NULL);
    CloseHandle(hFile);
    
    if (result) {
        doc->modified = false;
        doc->filePath = path;
        
        // 重新锁定
        doc->hFileLock = CreateFileW(path.c_str(), GENERIC_READ | GENERIC_WRITE,
                                     0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    }
    
    return result;
}

// 添加DLL命令
void AddDllCommand(EllDocument* doc) {
    // TODO: 实现添加命令对话框
}

// 删除DLL命令
void DeleteDllCommand(EllDocument* doc, int commandIndex) {
    // TODO: 实现删除命令
}

// 窗口过程 - 基础框架
LRESULT CALLBACK EllEditorWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    EllEditorData* data = (EllEditorData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    
    switch (message) {
    case WM_CREATE:
        {
            data = new EllEditorData(hWnd);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)data);
            
            wchar_t debugMsg[256];
            swprintf_s(debugMsg, L"[EllEditor] WM_CREATE - EllEditorData已创建: %p\n", data);
            OutputDebugStringW(debugMsg);
        }
        return 0;
        
    case WM_DESTROY:
        if (data) {
            delete data;
            SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
        }
        return 0;
        
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            if (!data || !data->GetActiveDoc()) {
                RECT rect;
                GetClientRect(hWnd, &rect);
                FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
                SetTextColor(hdc, RGB(100, 100, 100));
                DrawTextW(hdc, L"DLL声明编辑器", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                EndPaint(hWnd, &ps);
                return 0;
            }
            
            EllDocument* doc = data->GetActiveDoc();
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            
            // 使用双缓冲
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);
            
            // 填充背景
            FillRect(memDC, &clientRect, CreateSolidBrush(g_CurrentTheme.editorBg));
            
            // 绘制参数
            const int ROW_HEIGHT = 30;
            const int HEADER_HEIGHT = 28;
            const int LEFT_MARGIN = 50;
            const int COL_PADDING = 10;
            
            // 列宽定义
            const int CMD_NAME_WIDTH = 200;
            const int RETURN_TYPE_WIDTH = 100;  // 从150缩小到100
            const int PUBLIC_WIDTH = 30;         // 从80缩小到30
            const int BASE_REMARK_WIDTH = 200;   // 备注基础宽度，从300缩小到200
            
            // 计算备注列的实际宽度（根据内容动态扩展）
            int remarkWidth = BASE_REMARK_WIDTH;
            for (const auto& cmd : doc->commands) {
                if (!cmd.comment.empty()) {
                    // 估算文本宽度（每个字符约10像素）
                    int textWidth = (int)cmd.comment.length() * 10 + COL_PADDING * 2;
                    if (textWidth > remarkWidth) {
                        remarkWidth = textWidth;
                    }
                }
            }
            
            const int REMARK_WIDTH = remarkWidth;
            const int LIB_FILE_WIDTH = CMD_NAME_WIDTH + RETURN_TYPE_WIDTH + PUBLIC_WIDTH + REMARK_WIDTH;
            
            const int PARAM_NAME_WIDTH = 150;
            const int PARAM_TYPE_WIDTH = 120;
            const int BY_REF_WIDTH = 60;
            const int IS_ARRAY_WIDTH = 60;
            const int PARAM_REMARK_WIDTH = 300;
            
            int currentY = 10 - doc->scrollY;
            
            // 设置字体
            HFONT hFont = CreateFontW(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft YaHei");
            HFONT hHeaderFont = CreateFontW(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft YaHei");
            HFONT oldFont = (HFONT)SelectObject(memDC, hFont);
            
            HPEN gridPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.grid);
            HPEN oldPen = (HPEN)SelectObject(memDC, gridPen);
            
            SetBkMode(memDC, TRANSPARENT);
            
            // 绘制每个DLL命令
            for (size_t cmdIdx = 0; cmdIdx < doc->commands.size(); cmdIdx++) {
                const DllCommand& cmd = doc->commands[cmdIdx];
                
                if (cmdIdx > 0) {
                    currentY += 20; // 命令之间的间隔
                }
                
                // 第1行：表头 - DLL命令名、返回值类型、公开、备注
                SelectObject(memDC, hHeaderFont);
                RECT headerRect = {LEFT_MARGIN, currentY, LEFT_MARGIN + LIB_FILE_WIDTH, currentY + HEADER_HEIGHT};
                FillRect(memDC, &headerRect, CreateSolidBrush(g_CurrentTheme.tableHeaderBg));
                
                SetTextColor(memDC, g_CurrentTheme.text);
                int x = LEFT_MARGIN;
                
                // 绘制表头单元格
                RECT cellRect = {x, currentY, x + CMD_NAME_WIDTH, currentY + HEADER_HEIGHT};
                DrawTextW(memDC, L"DLL命令名", -1, &cellRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                MoveToEx(memDC, x + CMD_NAME_WIDTH, currentY, NULL);
                LineTo(memDC, x + CMD_NAME_WIDTH, currentY + HEADER_HEIGHT);
                x += CMD_NAME_WIDTH;
                
                cellRect = {x, currentY, x + RETURN_TYPE_WIDTH, currentY + HEADER_HEIGHT};
                DrawTextW(memDC, L"返回值类型", -1, &cellRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                MoveToEx(memDC, x + RETURN_TYPE_WIDTH, currentY, NULL);
                LineTo(memDC, x + RETURN_TYPE_WIDTH, currentY + HEADER_HEIGHT);
                x += RETURN_TYPE_WIDTH;
                
                cellRect = {x, currentY, x + PUBLIC_WIDTH, currentY + HEADER_HEIGHT};
                DrawTextW(memDC, L"公开", -1, &cellRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                MoveToEx(memDC, x + PUBLIC_WIDTH, currentY, NULL);
                LineTo(memDC, x + PUBLIC_WIDTH, currentY + HEADER_HEIGHT);
                x += PUBLIC_WIDTH;
                
                cellRect = {x, currentY, x + REMARK_WIDTH, currentY + HEADER_HEIGHT};
                DrawTextW(memDC, L"备注", -1, &cellRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                
                // 表头下边框
                MoveToEx(memDC, LEFT_MARGIN, currentY + HEADER_HEIGHT, NULL);
                LineTo(memDC, LEFT_MARGIN + LIB_FILE_WIDTH, currentY + HEADER_HEIGHT);
                currentY += HEADER_HEIGHT;
                
                // 第2行：数据 - 命令信息
                SelectObject(memDC, hFont);
                x = LEFT_MARGIN;
                
                cellRect = {x + COL_PADDING, currentY, x + CMD_NAME_WIDTH, currentY + ROW_HEIGHT};
                DrawTextW(memDC, cmd.commandName.c_str(), -1, &cellRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                MoveToEx(memDC, x + CMD_NAME_WIDTH, currentY, NULL);
                LineTo(memDC, x + CMD_NAME_WIDTH, currentY + ROW_HEIGHT);
                x += CMD_NAME_WIDTH;
                
                cellRect = {x + COL_PADDING, currentY, x + RETURN_TYPE_WIDTH, currentY + ROW_HEIGHT};
                DrawTextW(memDC, cmd.returnType.c_str(), -1, &cellRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                MoveToEx(memDC, x + RETURN_TYPE_WIDTH, currentY, NULL);
                LineTo(memDC, x + RETURN_TYPE_WIDTH, currentY + ROW_HEIGHT);
                x += RETURN_TYPE_WIDTH;
                
                cellRect = {x, currentY, x + PUBLIC_WIDTH, currentY + ROW_HEIGHT};
                DrawTextW(memDC, cmd.isPublic ? L"✓" : L"", -1, &cellRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                MoveToEx(memDC, x + PUBLIC_WIDTH, currentY, NULL);
                LineTo(memDC, x + PUBLIC_WIDTH, currentY + ROW_HEIGHT);
                x += PUBLIC_WIDTH;
                
                cellRect = {x + COL_PADDING, currentY, x + REMARK_WIDTH, currentY + ROW_HEIGHT};
                DrawTextW(memDC, cmd.comment.c_str(), -1, &cellRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                
                MoveToEx(memDC, LEFT_MARGIN, currentY + ROW_HEIGHT, NULL);
                LineTo(memDC, LEFT_MARGIN + LIB_FILE_WIDTH, currentY + ROW_HEIGHT);
                currentY += ROW_HEIGHT;
                
                // 第3行：表头 - 库文件名
                SelectObject(memDC, hHeaderFont);
                headerRect = {LEFT_MARGIN, currentY, LEFT_MARGIN + LIB_FILE_WIDTH, currentY + HEADER_HEIGHT};
                FillRect(memDC, &headerRect, CreateSolidBrush(g_CurrentTheme.tableHeaderBg));
                DrawTextW(memDC, L"库文件名", -1, &headerRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                MoveToEx(memDC, LEFT_MARGIN, currentY + HEADER_HEIGHT, NULL);
                LineTo(memDC, LEFT_MARGIN + LIB_FILE_WIDTH, currentY + HEADER_HEIGHT);
                currentY += HEADER_HEIGHT;
                
                // 第4行：数据 - 库文件名
                SelectObject(memDC, hFont);
                cellRect = {LEFT_MARGIN + COL_PADDING, currentY, LEFT_MARGIN + LIB_FILE_WIDTH, currentY + ROW_HEIGHT};
                DrawTextW(memDC, cmd.dllFileName.c_str(), -1, &cellRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                MoveToEx(memDC, LEFT_MARGIN, currentY + ROW_HEIGHT, NULL);
                LineTo(memDC, LEFT_MARGIN + LIB_FILE_WIDTH, currentY + ROW_HEIGHT);
                currentY += ROW_HEIGHT;
                
                // 第5行：表头 - 在库中对应的命令名
                SelectObject(memDC, hHeaderFont);
                headerRect = {LEFT_MARGIN, currentY, LEFT_MARGIN + LIB_FILE_WIDTH, currentY + HEADER_HEIGHT};
                FillRect(memDC, &headerRect, CreateSolidBrush(g_CurrentTheme.tableHeaderBg));
                DrawTextW(memDC, L"在库中对应的命令名", -1, &headerRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                MoveToEx(memDC, LEFT_MARGIN, currentY + HEADER_HEIGHT, NULL);
                LineTo(memDC, LEFT_MARGIN + LIB_FILE_WIDTH, currentY + HEADER_HEIGHT);
                currentY += HEADER_HEIGHT;
                
                // 第6行：数据 - 在库中对应的命令名
                SelectObject(memDC, hFont);
                cellRect = {LEFT_MARGIN + COL_PADDING, currentY, LEFT_MARGIN + LIB_FILE_WIDTH, currentY + ROW_HEIGHT};
                DrawTextW(memDC, cmd.dllCommandName.c_str(), -1, &cellRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                MoveToEx(memDC, LEFT_MARGIN, currentY + ROW_HEIGHT, NULL);
                LineTo(memDC, LEFT_MARGIN + LIB_FILE_WIDTH, currentY + ROW_HEIGHT);
                currentY += ROW_HEIGHT;
                
                // 第7行：表头 - 参数表头
                SelectObject(memDC, hHeaderFont);
                headerRect = {LEFT_MARGIN, currentY, LEFT_MARGIN + LIB_FILE_WIDTH, currentY + HEADER_HEIGHT};
                FillRect(memDC, &headerRect, CreateSolidBrush(g_CurrentTheme.tableHeaderBg));
                
                x = LEFT_MARGIN;
                cellRect = {x, currentY, x + PARAM_NAME_WIDTH, currentY + HEADER_HEIGHT};
                DrawTextW(memDC, L"参数名", -1, &cellRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                MoveToEx(memDC, x + PARAM_NAME_WIDTH, currentY, NULL);
                LineTo(memDC, x + PARAM_NAME_WIDTH, currentY + HEADER_HEIGHT);
                x += PARAM_NAME_WIDTH;
                
                cellRect = {x, currentY, x + PARAM_TYPE_WIDTH, currentY + HEADER_HEIGHT};
                DrawTextW(memDC, L"类型", -1, &cellRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                MoveToEx(memDC, x + PARAM_TYPE_WIDTH, currentY, NULL);
                LineTo(memDC, x + PARAM_TYPE_WIDTH, currentY + HEADER_HEIGHT);
                x += PARAM_TYPE_WIDTH;
                
                cellRect = {x, currentY, x + BY_REF_WIDTH, currentY + HEADER_HEIGHT};
                DrawTextW(memDC, L"传址", -1, &cellRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                MoveToEx(memDC, x + BY_REF_WIDTH, currentY, NULL);
                LineTo(memDC, x + BY_REF_WIDTH, currentY + HEADER_HEIGHT);
                x += BY_REF_WIDTH;
                
                cellRect = {x, currentY, x + IS_ARRAY_WIDTH, currentY + HEADER_HEIGHT};
                DrawTextW(memDC, L"数组", -1, &cellRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                MoveToEx(memDC, x + IS_ARRAY_WIDTH, currentY, NULL);
                LineTo(memDC, x + IS_ARRAY_WIDTH, currentY + HEADER_HEIGHT);
                x += IS_ARRAY_WIDTH;
                
                int paramRemarkActualWidth = LIB_FILE_WIDTH - PARAM_NAME_WIDTH - PARAM_TYPE_WIDTH - BY_REF_WIDTH - IS_ARRAY_WIDTH;
                cellRect = {x, currentY, x + paramRemarkActualWidth, currentY + HEADER_HEIGHT};
                DrawTextW(memDC, L"备注", -1, &cellRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                
                MoveToEx(memDC, LEFT_MARGIN, currentY + HEADER_HEIGHT, NULL);
                LineTo(memDC, LEFT_MARGIN + LIB_FILE_WIDTH, currentY + HEADER_HEIGHT);
                currentY += HEADER_HEIGHT;
                
                // 第8+行：参数数据行
                SelectObject(memDC, hFont);
                for (int paramLineIdx : cmd.paramLines) {
                    if (paramLineIdx >= 0 && paramLineIdx < (int)doc->allParams.size()) {
                        const DllParameter& param = doc->allParams[paramLineIdx];
                        
                        x = LEFT_MARGIN;
                        cellRect = {x + COL_PADDING, currentY, x + PARAM_NAME_WIDTH, currentY + ROW_HEIGHT};
                        DrawTextW(memDC, param.paramName.c_str(), -1, &cellRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                        MoveToEx(memDC, x + PARAM_NAME_WIDTH, currentY, NULL);
                        LineTo(memDC, x + PARAM_NAME_WIDTH, currentY + ROW_HEIGHT);
                        x += PARAM_NAME_WIDTH;
                        
                        cellRect = {x + COL_PADDING, currentY, x + PARAM_TYPE_WIDTH, currentY + ROW_HEIGHT};
                        DrawTextW(memDC, param.paramType.c_str(), -1, &cellRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                        MoveToEx(memDC, x + PARAM_TYPE_WIDTH, currentY, NULL);
                        LineTo(memDC, x + PARAM_TYPE_WIDTH, currentY + ROW_HEIGHT);
                        x += PARAM_TYPE_WIDTH;
                        
                        cellRect = {x, currentY, x + BY_REF_WIDTH, currentY + ROW_HEIGHT};
                        DrawTextW(memDC, param.byRef ? L"是" : L"", -1, &cellRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                        MoveToEx(memDC, x + BY_REF_WIDTH, currentY, NULL);
                        LineTo(memDC, x + BY_REF_WIDTH, currentY + ROW_HEIGHT);
                        x += BY_REF_WIDTH;
                        
                        cellRect = {x, currentY, x + IS_ARRAY_WIDTH, currentY + ROW_HEIGHT};
                        DrawTextW(memDC, param.isArray ? L"是" : L"", -1, &cellRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                        MoveToEx(memDC, x + IS_ARRAY_WIDTH, currentY, NULL);
                        LineTo(memDC, x + IS_ARRAY_WIDTH, currentY + ROW_HEIGHT);
                        x += IS_ARRAY_WIDTH;
                        
                        cellRect = {x + COL_PADDING, currentY, x + paramRemarkActualWidth, currentY + ROW_HEIGHT};
                        DrawTextW(memDC, param.comment.c_str(), -1, &cellRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                        
                        MoveToEx(memDC, LEFT_MARGIN, currentY + ROW_HEIGHT, NULL);
                        LineTo(memDC, LEFT_MARGIN + LIB_FILE_WIDTH, currentY + ROW_HEIGHT);
                        currentY += ROW_HEIGHT;
                    }
                }
                
                // 绘制左右边框
                int paramCount = (int)cmd.paramLines.size();
                MoveToEx(memDC, LEFT_MARGIN, currentY - (HEADER_HEIGHT * 4 + ROW_HEIGHT * (3 + paramCount)), NULL);
                LineTo(memDC, LEFT_MARGIN, currentY);
                MoveToEx(memDC, LEFT_MARGIN + LIB_FILE_WIDTH, currentY - (HEADER_HEIGHT * 4 + ROW_HEIGHT * (3 + paramCount)), NULL);
                LineTo(memDC, LEFT_MARGIN + LIB_FILE_WIDTH, currentY);
            }
            
            // 清理
            SelectObject(memDC, oldFont);
            SelectObject(memDC, oldPen);
            DeleteObject(hFont);
            DeleteObject(hHeaderFont);
            DeleteObject(gridPen);
            
            // 复制到屏幕
            BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, memDC, 0, 0, SRCCOPY);
            
            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);
            
            EndPaint(hWnd, &ps);
        }
        return 0;
        
    case WM_SIZE:
        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
        
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    
    return 0;
}
