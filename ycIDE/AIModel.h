#pragma once
#include <string>
#include <vector>

struct AIModel {
    std::wstring name;
    std::wstring provider;
    std::wstring apiUrl;
    std::wstring apiKey;
    AIModel(const std::wstring& n, const std::wstring& p, const std::wstring& url)
        : name(n), provider(p), apiUrl(url), apiKey(L"") {}
};

extern std::vector<AIModel> g_AIModels;

// 回调函数类型：返回 true 继续，返回 false 中止
typedef bool (*StreamCallback)(const std::wstring& chunk, void* param);

std::wstring CallAIAPI(const std::wstring& prompt, const AIModel& model, const std::wstring& mode, StreamCallback callback = nullptr, void* callbackParam = nullptr);

// 配置文件保存和加载
void SaveModelsConfig();
void LoadModelsConfig();
