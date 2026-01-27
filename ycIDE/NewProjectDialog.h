#pragma once
#include <windows.h>
#include <string>

// 项目类型
enum class ProjectType {
    Console,        // 控制台程序
    WindowsApp,     // 窗口程序
    DynamicLibrary  // 动态链接库 (DLL)
};

// 新建项目对话框结果
struct NewProjectResult {
    bool success;               // 是否成功创建
    ProjectType type;           // 项目类型
    std::wstring projectName;   // 项目名称
    std::wstring projectPath;   // 项目路径 (.epp文件完整路径)
    std::wstring projectDir;    // 项目所在目录
    
    NewProjectResult() : success(false), type(ProjectType::Console) {}
};

// 新建项目对话框类
class NewProjectDialog {
public:
    // 显示新建项目对话框
    // 返回: 用户选择的项目配置，如果取消则 success 为 false
    static NewProjectResult Show(HWND hParent);

private:
    // 对话框过程
    static INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    
    // 初始化对话框
    static void OnInitDialog(HWND hDlg);
    
    // 处理命令
    static void OnCommand(HWND hDlg, WPARAM wParam, LPARAM lParam);
    
    // 绘制项目类型项
    static void OnDrawItem(HWND hDlg, LPDRAWITEMSTRUCT lpDIS);
    
    // 浏览文件夹
    static void BrowseFolder(HWND hDlg);
    
    // 验证输入
    static bool ValidateInput(HWND hDlg, NewProjectResult& result);
    
    // 获取选中的项目类型
    static ProjectType GetSelectedProjectType(HWND hDlg);
    
    // 当前对话框结果（静态存储用于对话框过程）
    static NewProjectResult s_result;
};

// 获取项目类型的显示名称
const wchar_t* GetProjectTypeName(ProjectType type);

// 获取项目类型的描述
const wchar_t* GetProjectTypeDescription(ProjectType type);
