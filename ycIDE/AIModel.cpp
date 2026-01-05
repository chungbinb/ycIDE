#include "framework.h"
#include "AIModel.h"
#include <winhttp.h>
#include <wincrypt.h>
#include <ctime>
#include <algorithm>
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "crypt32.lib")

// 外部声明调试日志函数
extern void WriteDebugLog(const wchar_t* message);

std::vector<AIModel> g_AIModels = {
    AIModel(L"DeepSeek", L"DeepSeek", L"https://api.deepseek.com/chat/completions"),
};

// JSON字符串转义
static std::wstring JsonEscape(const std::wstring& input) {
    std::wstring output;
    output.reserve(input.size());

    for (wchar_t ch : input) {
        switch (ch) {
        case L'"':
            output += L"\\\"";
            break;
        case L'\\':
            output += L"\\\\";
            break;
        case L'\b':
            output += L"\\b";
            break;
        case L'\f':
            output += L"\\f";
            break;
        case L'\n':
            output += L"\\n";
            break;
        case L'\r':
            output += L"\\r";
            break;
        case L'\t':
            output += L"\\t";
            break;
        default:
            if (ch >= 0 && ch < 0x20) {
                wchar_t buf[7];
                swprintf(buf, 7, L"\\u%04X", (unsigned int)ch);
                output += buf;
            } else {
                output += ch;
            }
            break;
        }
    }

    return output;
}

// 从标准OpenAI/DeepSeek JSON响应中提取assistant的content
static std::wstring ExtractContentFromResponse(const std::wstring& response) {
    // 优先从choices[0].message.content中提取
    size_t msgPos = response.find(L"\"message\"");
    if (msgPos == std::wstring::npos) {
        msgPos = 0;  // 找不到就从头开始兜底
    }

    size_t contentPos = response.find(L"\"content\"", msgPos);
    if (contentPos == std::wstring::npos) {
        return L"";
    }

    size_t colonPos = response.find(L":", contentPos);
    if (colonPos == std::wstring::npos) {
        return L"";
    }

    size_t i = colonPos + 1;
    // 跳过空白
    while (i < response.size() && iswspace(response[i])) {
        ++i;
    }

    if (i >= response.size() || response[i] != L'"') {
        // 不是字符串(例如是数组/对象)，简单兜底：返回空
        return L"";
    }

    // 解析JSON字符串，处理转义字符，避免遇到内部的 \" 提前截断
    ++i; // 跳过开头的引号
    std::wstring result;
    bool escape = false;
    for (; i < response.size(); ++i) {
        wchar_t ch = response[i];
        if (escape) {
            // 简单按字符追加，常见转义(如\n)在显示时问题不大
            result.push_back(ch);
            escape = false;
        } else if (ch == L'\\') {
            escape = true;
        } else if (ch == L'"') {
            // 字符串结束
            break;
        } else {
            result.push_back(ch);
        }
    }
    return result;
}

// 从流式响应块中提取content
static std::wstring ExtractContentFromChunk(const std::wstring& chunk) {
    // 查找 "delta"
    size_t deltaPos = chunk.find(L"\"delta\"");
    if (deltaPos == std::wstring::npos) return L"";

    // 查找 "content"
    size_t contentPos = chunk.find(L"\"content\"", deltaPos);
    if (contentPos == std::wstring::npos) return L"";

    // 查找冒号
    size_t colonPos = chunk.find(L":", contentPos);
    if (colonPos == std::wstring::npos) return L"";

    size_t i = colonPos + 1;
    while (i < chunk.size() && iswspace(chunk[i])) ++i;

    if (i >= chunk.size() || chunk[i] != L'"') return L"";

    ++i; // 跳过引号
    std::wstring result;
    bool escape = false;
    for (; i < chunk.size(); ++i) {
        wchar_t ch = chunk[i];
        if (escape) {
            if (ch == L'n') result.push_back(L'\n');
            else if (ch == L't') result.push_back(L'\t');
            else if (ch == L'r') result.push_back(L'\r');
            else if (ch == L'\\') result.push_back(L'\\');
            else if (ch == L'"') result.push_back(L'"');
            else result.push_back(ch);
            escape = false;
        } else if (ch == L'\\') {
            escape = true;
        } else if (ch == L'"') {
            break;
        } else {
            result.push_back(ch);
        }
    }
    return result;
}

std::wstring CallAIAPI(const std::wstring& prompt, const AIModel& model, const std::wstring& mode, StreamCallback callback, void* callbackParam) {
    WriteDebugLog(L"[CallAIAPI] Function called");
    
    if (model.apiKey.empty()) {
        WriteDebugLog(L"[CallAIAPI] ERROR: API Key is empty");
        return L"[Error] Please config API Key first";
    }
    
    wchar_t logMsg[512];
    swprintf_s(logMsg, L"[CallAIAPI] Model: %s, Provider: %s, Mode: %s", 
              model.name.c_str(), model.provider.c_str(), mode.c_str());
    WriteDebugLog(logMsg);

    std::wstring url = model.apiUrl;
    std::wstring host;
    std::wstring path;
    bool useHttps = (url.find(L"https://") == 0);

    size_t hostStart = useHttps ? 8 : 7;
    size_t pathStart = url.find(L'/', hostStart);
    if (pathStart != std::wstring::npos) {
        host = url.substr(hostStart, pathStart - hostStart);
        path = url.substr(pathStart);
    }
    else {
        host = url.substr(hostStart);
        path = L"/";
    }

    bool isStreaming = (callback != nullptr);
    std::wstring streamVal = isStreaming ? L"true" : L"false";

    std::wstring jsonBody;
    if (model.provider == L"OpenAI") {
        std::wstring modelName = (model.name.find(L"GPT-4") != std::wstring::npos) ? L"gpt-4" : L"gpt-3.5-turbo";
        jsonBody = L"{\"model\":\"" + modelName + L"\",\"messages\":[{\"role\":\"user\",\"content\":\"" + JsonEscape(prompt) + L"\"}],\"stream\":" + streamVal + L"}";
    }
    else if (model.provider == L"DeepSeek") {
        std::wstring modelName = (mode == L"思考") ? L"deepseek-reasoner" : L"deepseek-chat";
        jsonBody = L"{\"model\":\"" + modelName + L"\",\"messages\":[{\"role\":\"system\",\"content\":\"You are a helpful assistant.\"},{\"role\":\"user\",\"content\":\"" + JsonEscape(prompt) + L"\"}],\"stream\":" + streamVal + L"}";
    }
    else if (model.provider == L"Anthropic") {
        // Claude API format is different, simplified here
        jsonBody = L"{\"model\":\"claude-3-sonnet-20240229\",\"max_tokens\":1024,\"messages\":[{\"role\":\"user\",\"content\":\"" + JsonEscape(prompt) + L"\"}],\"stream\":" + streamVal + L"}";
    }
    else {
        jsonBody = L"{\"model\":\"default\",\"messages\":[{\"role\":\"user\",\"content\":\"" + JsonEscape(prompt) + L"\"}],\"stream\":" + streamVal + L"}";
    }

    int len = WideCharToMultiByte(CP_UTF8, 0, jsonBody.c_str(), -1, NULL, 0, NULL, NULL);
    std::string utf8Body(len - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, jsonBody.c_str(), -1, &utf8Body[0], len, NULL, NULL);

    HINTERNET hSession = WinHttpOpen(L"ycIDE/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) {
        WriteDebugLog(L"[CallAIAPI] ERROR: WinHttpOpen failed");
        return L"[Error] Cannot init HTTP request";
    }

    WriteDebugLog(L"[CallAIAPI] Connecting to server...");
    HINTERNET hConnect = WinHttpConnect(hSession, host.c_str(), useHttps ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT, 0);
    if (!hConnect) {
        WriteDebugLog(L"[CallAIAPI] ERROR: WinHttpConnect failed");
        WinHttpCloseHandle(hSession);
        return L"[Error] Cannot connect to server";
    }

    WriteDebugLog(L"[CallAIAPI] Opening request...");
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", path.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, useHttps ? WINHTTP_FLAG_SECURE : 0);
    if (!hRequest) {
        WriteDebugLog(L"[CallAIAPI] ERROR: WinHttpOpenRequest failed");
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return L"[Error] Cannot create request";
    }

    std::wstring headers = L"Content-Type: application/json\r\nAuthorization: Bearer " + model.apiKey + L"\r\n";

    WriteDebugLog(L"[CallAIAPI] Sending request...");
    BOOL bResults = WinHttpSendRequest(hRequest, headers.c_str(), -1, (LPVOID)utf8Body.c_str(), (DWORD)utf8Body.length(), (DWORD)utf8Body.length(), 0);
    if (!bResults) {
        DWORD error = GetLastError();
        wchar_t errMsg[256];
        swprintf_s(errMsg, L"[CallAIAPI] ERROR: WinHttpSendRequest failed, error code: %d", error);
        WriteDebugLog(errMsg);
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return L"[Error] Send request failed";
    }

    WriteDebugLog(L"[CallAIAPI] Receiving response...");
    bResults = WinHttpReceiveResponse(hRequest, NULL);
    if (!bResults) {
        DWORD error = GetLastError();
        wchar_t errMsg[256];
        swprintf_s(errMsg, L"[CallAIAPI] ERROR: WinHttpReceiveResponse failed, error code: %d", error);
        WriteDebugLog(errMsg);
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return L"[Error] Receive response failed";
    }
    
    WriteDebugLog(L"[CallAIAPI] Reading response data...");

    std::string responseData;
    std::string streamBuffer;
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;

    do {
        dwSize = 0;
        if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) break;
        
        if (dwSize > 0) {
            wchar_t sizeMsg[128];
            swprintf_s(sizeMsg, L"[CallAIAPI] Data available: %d bytes", dwSize);
            WriteDebugLog(sizeMsg);
        }
        
        pszOutBuffer = new char[dwSize + 1];
        ZeroMemory(pszOutBuffer, dwSize + 1);
        
        if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
            delete[] pszOutBuffer;
            break;
        }

        if (isStreaming) {
            streamBuffer.append(pszOutBuffer, dwDownloaded);
            
            // 处理 SSE 行
            size_t pos = 0;
            while (true) {
                size_t nextLine = streamBuffer.find('\n', pos);
                if (nextLine == std::string::npos) break;
                
                std::string line = streamBuffer.substr(pos, nextLine - pos);
                // 如果存在 \r 则移除
                if (!line.empty() && line.back() == '\r') line.pop_back();
                
                if (line.find("data: ") == 0) {
                    std::string data = line.substr(6);
                    if (data == "[DONE]") {
                        // 流结束
                        WriteDebugLog(L"[CallAIAPI] Stream DONE signal received");
                    } else {
                        // 解析 JSON
                        int wlen = MultiByteToWideChar(CP_UTF8, 0, data.c_str(), -1, NULL, 0);
                        if (wlen > 0) {
                            std::wstring wdata(wlen - 1, 0);
                            MultiByteToWideChar(CP_UTF8, 0, data.c_str(), -1, &wdata[0], wlen);
                            
                            // 记录原始JSON数据到日志
                            if (wdata.length() <= 200) {
                                std::wstring logMsg = L"[CallAIAPI] Raw JSON: " + wdata;
                                WriteDebugLog(logMsg.c_str());
                            } else {
                                std::wstring preview = wdata.substr(0, 200) + L"...";
                                std::wstring logMsg = L"[CallAIAPI] Raw JSON (truncated): " + preview;
                                WriteDebugLog(logMsg.c_str());
                            }
                            
                            std::wstring content = ExtractContentFromChunk(wdata);
                            if (!content.empty() && callback) {
                                wchar_t contentMsg[256];
                                int previewLen = (std::min)((int)content.length(), 50);
                                swprintf_s(contentMsg, L"[CallAIAPI] Extracted content (%d chars): %.*s%s", 
                                          (int)content.length(), 
                                          previewLen, 
                                          content.c_str(),
                                          content.length() > 50 ? L"..." : L"");
                                WriteDebugLog(contentMsg);
                                
                                if (!callback(content, callbackParam)) {
                                    // 回调返回 false，停止流式传输
                                    WriteDebugLog(L"[CallAIAPI] Callback returned false, stopping stream");
                                    delete[] pszOutBuffer;
                                    WinHttpCloseHandle(hRequest);
                                    WinHttpCloseHandle(hConnect);
                                    WinHttpCloseHandle(hSession);
                                    return L""; 
                                }
                            }
                        }
                    }
                }
                
                pos = nextLine + 1;
            }
            streamBuffer = streamBuffer.substr(pos);
        } else {
            responseData.append(pszOutBuffer, dwDownloaded);
        }
        
        delete[] pszOutBuffer;
    } while (dwSize > 0);

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    if (isStreaming) {
        WriteDebugLog(L"[CallAIAPI] Streaming completed");
        return L""; // Streaming handled via callback
    }
    
    wchar_t resMsg[256];
    swprintf_s(resMsg, L"[CallAIAPI] Non-streaming response length: %d bytes", (int)responseData.length());
    WriteDebugLog(resMsg);
    
    // 记录非流式响应数据到日志文件
    std::wstring logFileName = L"logs\\ai_response_";
    wchar_t timeStr[64];
    time_t now = std::time(0);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    wcsftime(timeStr, 64, L"%Y%m%d_%H%M%S", &timeinfo);
    logFileName += timeStr;
    logFileName += L".txt";
    
    FILE* logFile = nullptr;
    _wfopen_s(&logFile, logFileName.c_str(), L"w, ccs=UTF-8");
    if (logFile) {
        int wlen = MultiByteToWideChar(CP_UTF8, 0, responseData.c_str(), -1, NULL, 0);
        std::wstring wresponse(wlen - 1, 0);
        MultiByteToWideChar(CP_UTF8, 0, responseData.c_str(), -1, &wresponse[0], wlen);
        fwprintf(logFile, L"%s", wresponse.c_str());
        fclose(logFile);
        
        std::wstring saveMsg = L"[CallAIAPI] Response saved to: " + logFileName;
        WriteDebugLog(saveMsg.c_str());
    }

    len = MultiByteToWideChar(CP_UTF8, 0, responseData.c_str(), -1, NULL, 0);
    std::wstring response(len - 1, 0);
    MultiByteToWideChar(CP_UTF8, 0, responseData.c_str(), -1, &response[0], len);

    // 优先尝试从JSON中提取assistant的content
    std::wstring content = ExtractContentFromResponse(response);
    if (!content.empty()) {
        return content;
    }

    if (response.empty()) {
        return L"[Error] API return empty response";
    }

    return L"[Raw Response] " + response.substr(0, 500);
}

// 简单加密函数(使用Windows DPAPI)
std::wstring EncryptString(const std::wstring& plainText) {
    if (plainText.empty()) return L"";
    
    DATA_BLOB dataIn;
    DATA_BLOB dataOut;
    
    dataIn.pbData = (BYTE*)plainText.c_str();
    dataIn.cbData = (DWORD)((plainText.length() + 1) * sizeof(wchar_t));
    
    if (!CryptProtectData(&dataIn, L"ycIDE", NULL, NULL, NULL, 0, &dataOut)) {
        return plainText;  // 加密失败,返回原文
    }
    
    // 转换为Base64
    DWORD base64Len = 0;
    CryptBinaryToStringW(dataOut.pbData, dataOut.cbData, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, NULL, &base64Len);
    
    std::wstring base64(base64Len, 0);
    CryptBinaryToStringW(dataOut.pbData, dataOut.cbData, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, &base64[0], &base64Len);
    
    LocalFree(dataOut.pbData);
    
    // 移除末尾的\0
    if (!base64.empty() && base64[base64.length() - 1] == 0) {
        base64.resize(base64.length() - 1);
    }
    
    return base64;
}

// 简单解密函数(使用Windows DPAPI)
std::wstring DecryptString(const std::wstring& encrypted) {
    if (encrypted.empty()) return L"";
    
    // 从CBase64转换
    DWORD binaryLen = 0;
    if (!CryptStringToBinaryW(encrypted.c_str(), 0, CRYPT_STRING_BASE64, NULL, &binaryLen, NULL, NULL)) {
        return encrypted;  // 解密失败,返回原文
    }
    
    std::vector<BYTE> binary(binaryLen);
    CryptStringToBinaryW(encrypted.c_str(), 0, CRYPT_STRING_BASE64, &binary[0], &binaryLen, NULL, NULL);
    
    DATA_BLOB dataIn;
    DATA_BLOB dataOut;
    
    dataIn.pbData = &binary[0];
    dataIn.cbData = binaryLen;
    
    if (!CryptUnprotectData(&dataIn, NULL, NULL, NULL, NULL, 0, &dataOut)) {
        return encrypted;  // 解密失败,返回原文
    }
    
    std::wstring plainText((wchar_t*)dataOut.pbData);
    LocalFree(dataOut.pbData);
    
    return plainText;
}

// 保存模型配置到文件
void SaveModelsConfig() {
    wchar_t configPath[MAX_PATH];
    GetModuleFileNameW(NULL, configPath, MAX_PATH);
    std::wstring path(configPath);
    size_t pos = path.find_last_of(L"\\");
    if (pos != std::wstring::npos) {
        path = path.substr(0, pos + 1) + L"models.ini";
    }
    
    // 先删除旧文件
    DeleteFileW(path.c_str());
    
    // 写入模型数量
    WritePrivateProfileStringW(L"Config", L"Count", std::to_wstring(g_AIModels.size()).c_str(), path.c_str());
    
    // 写入每个模型的配置
    for (size_t i = 0; i < g_AIModels.size(); i++) {
        std::wstring section = L"Model" + std::to_wstring(i);
        WritePrivateProfileStringW(section.c_str(), L"Name", g_AIModels[i].name.c_str(), path.c_str());
        WritePrivateProfileStringW(section.c_str(), L"Provider", g_AIModels[i].provider.c_str(), path.c_str());
        WritePrivateProfileStringW(section.c_str(), L"ApiUrl", g_AIModels[i].apiUrl.c_str(), path.c_str());
        
        // 加密API Key后保存
        std::wstring encryptedKey = EncryptString(g_AIModels[i].apiKey);
        WritePrivateProfileStringW(section.c_str(), L"ApiKey", encryptedKey.c_str(), path.c_str());
    }
}

// 从文件加载模型配置
void LoadModelsConfig() {
    wchar_t configPath[MAX_PATH];
    GetModuleFileNameW(NULL, configPath, MAX_PATH);
    std::wstring path(configPath);
    size_t pos = path.find_last_of(L"\\");
    if (pos != std::wstring::npos) {
        path = path.substr(0, pos + 1) + L"models.ini";
    }
    
    // 检查文件是否存在
    DWORD attr = GetFileAttributesW(path.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES) {
        return;  // 文件不存在,使用默认配置
    }
    
    // 读取模型数量
    int count = GetPrivateProfileIntW(L"Config", L"Count", 0, path.c_str());
    if (count == 0) {
        return;  // 没有配置,使用默认
    }
    
    // 清空默认模型
    g_AIModels.clear();
    
    // 读取每个模型
    wchar_t buffer[1024];
    for (int i = 0; i < count; i++) {
        std::wstring section = L"Model" + std::to_wstring(i);
        
        GetPrivateProfileStringW(section.c_str(), L"Name", L"", buffer, 1024, path.c_str());
        std::wstring name = buffer;
        
        GetPrivateProfileStringW(section.c_str(), L"Provider", L"", buffer, 1024, path.c_str());
        std::wstring provider = buffer;
        
        GetPrivateProfileStringW(section.c_str(), L"ApiUrl", L"", buffer, 1024, path.c_str());
        std::wstring apiUrl = buffer;
        
        GetPrivateProfileStringW(section.c_str(), L"ApiKey", L"", buffer, 1024, path.c_str());
        std::wstring encryptedKey = buffer;
        
        // 解密API Key
        std::wstring apiKey = DecryptString(encryptedKey);
        
        AIModel model(name, provider, apiUrl);
        model.apiKey = apiKey;
        g_AIModels.push_back(model);
    }
}
