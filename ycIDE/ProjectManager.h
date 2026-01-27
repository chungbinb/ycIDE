#pragma once
#include <windows.h>
#include <string>
#include <vector>

// 项目类型（输出类型）
enum class ProjectOutputType {
    Console,        // 控制台程序
    WindowsApp,     // 窗口程序
    DynamicLibrary  // 动态链接库 (DLL)
};

// 项目文件类型
enum ProjectFileType {
    PROJECT_FILE_EYC,       // 源代码文件 (.eyc)
    PROJECT_FILE_ELL,       // DLL声明文件 (.ell)
    PROJECT_FILE_EC,        // 模块文件 (.ec)
    PROJECT_FILE_EFW,       // 窗口文件 (.efw)
    PROJECT_FILE_ECS,       // 常量数据文件 (.ecs)
    PROJECT_FILE_EDT,       // 自定义数据类型文件 (.edt)
    PROJECT_FILE_EGV,       // 全局变量文件 (.egv)
    PROJECT_FILE_RES_IMAGE, // 图片资源 (.png, .jpg, .bmp, .ico, .gif)
    PROJECT_FILE_RES_AUDIO, // 音频资源 (.wav, .mp3)
    PROJECT_FILE_RES_DATA,  // 数据资源 (.txt, .dat, .bin)
    PROJECT_FILE_OTHER      // 其他文件
};

// 项目文件项
struct ProjectFileItem {
    std::wstring filePath;      // 文件路径（相对于项目文件）
    std::wstring fileName;      // 文件名
    ProjectFileType fileType;   // 文件类型
    bool isMainFile;            // 是否是主文件
    
    ProjectFileItem() : fileType(PROJECT_FILE_OTHER), isMainFile(false) {}
};

// 项目信息
struct ProjectInfo {
    std::wstring projectName;           // 项目名称
    std::wstring projectPath;           // 项目文件路径（.epp）
    std::wstring projectDirectory;      // 项目所在目录
    ProjectOutputType outputType;       // 项目输出类型
    int version;                        // 项目文件版本
    
    std::vector<ProjectFileItem> files; // 项目包含的文件列表
    
    ProjectInfo() : outputType(ProjectOutputType::Console), version(1) {}
};

// 项目管理器
class ProjectManager {
public:
    static ProjectManager& GetInstance();
    
    // 创建新项目
    bool CreateProject(const std::wstring& projectPath, const std::wstring& projectName,
                      ProjectOutputType outputType = ProjectOutputType::Console);
    
    // 打开项目
    bool OpenProject(const std::wstring& projectPath);
    
    // 保存项目
    bool SaveProject();
    
    // 关闭项目
    void CloseProject();
    
    // 添加文件到项目
    bool AddFileToProject(const std::wstring& filePath);
    
    // 从项目移除文件
    bool RemoveFileFromProject(const std::wstring& filePath);
    
    // 获取当前项目信息
    const ProjectInfo* GetCurrentProject() const { return currentProject_; }
    
    // 是否有打开的项目
    bool HasOpenProject() const { return currentProject_ != nullptr; }
    
    // 获取项目中的所有文件
    const std::vector<ProjectFileItem>& GetProjectFiles() const;
    
    // 设置主文件
    bool SetMainFile(const std::wstring& filePath);
    
    // 获取主文件
    std::wstring GetMainFile() const;
    
    // 获取相对路径
    std::wstring GetRelativePath(const std::wstring& fullPath) const;
    
    // 获取绝对路径
    std::wstring GetAbsolutePath(const std::wstring& relativePath) const;
    
private:
    ProjectManager() : currentProject_(nullptr) {}
    ~ProjectManager();
    ProjectManager(const ProjectManager&) = delete;
    ProjectManager& operator=(const ProjectManager&) = delete;
    
    ProjectInfo* currentProject_;
    
    // 解析项目文件
    bool ParseProjectFile(const std::wstring& content, ProjectInfo* project);
    
    // 生成项目文件内容
    std::wstring GenerateProjectFile(const ProjectInfo* project);
};

// 显示创建项目对话框
bool ShowCreateProjectDialog(HWND hWnd, std::wstring& projectPath, std::wstring& projectName);

// 显示打开项目对话框
bool ShowOpenProjectDialog(HWND hWnd, std::wstring& projectPath);
