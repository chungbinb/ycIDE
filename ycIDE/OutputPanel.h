#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <vector>
#include <functional>

using namespace Gdiplus;

// 输出面板标签类型
enum class OutputTabType {
    Output,  // 输出
    Hint     // 提示
};

// 命令提示信息
struct CommandHint {
    std::wstring commandName;      // 命令名称
    std::wstring description;      // 命令描述
    std::wstring syntax;           // 语法格式
    std::vector<std::wstring> parameters;  // 参数列表
    std::wstring returnValue;      // 返回值说明
    std::wstring example;          // 示例代码
};

class OutputPanel
{
public:
    OutputPanel(HWND hWnd);
    ~OutputPanel();
    
    // === 窗口消息处理 ===
    void OnPaint(HDC hdc);
    void OnSize(int width, int height);
    void OnLButtonDown(int x, int y);
    
    // === 标签操作 ===
    void SetActiveTab(OutputTabType tab);
    OutputTabType GetActiveTab() const { return m_activeTab; }
    
    // === 输出操作 ===
    void AppendOutput(const std::wstring& text, bool autoSwitch = true);  // autoSwitch: 编译/运行时自动切换到输出标签
    void ClearOutput();
    std::wstring GetOutput() const { return m_outputText; }
    
    // === 提示操作 ===
    void SetHint(const CommandHint& hint);
    void ClearHint();
    
    // === 编辑框访问 ===
    HWND GetOutputEdit() const { return m_hOutputEdit; }
    HWND GetHintEdit() const { return m_hHintEdit; }
    
    // === 当前命令信息 ===
    void SetCurrentCommand(const std::wstring& commandName, int lineNumber);
    
private:
    HWND m_hWnd;
    HWND m_hOutputEdit;   // 输出内容编辑框
    HWND m_hHintEdit;     // 提示内容编辑框
    
    OutputTabType m_activeTab;
    int m_tabHeight;
    int m_hoveredTab;
    
    std::wstring m_outputText;
    CommandHint m_currentHint;
    
    // GDI+
    ULONG_PTR m_gdiplusToken;
    
    // 内部方法
    void DrawTabs(Graphics& g, const RECT& rect);
    void UpdateEditVisibility();
    int HitTestTab(int x, int y);
    void UpdateHintDisplay();
};
