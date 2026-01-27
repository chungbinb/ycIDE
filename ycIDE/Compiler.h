#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <functional>

// 前向声明
struct ProjectInfo;

// 编译消息类型
enum class CompileMessageType {
    Info,       // 普通信息
    Warning,    // 警告
    Error,      // 错误
    Success     // 成功
};

// 编译消息
struct CompileMessage {
    CompileMessageType type;
    std::wstring message;
    std::wstring file;      // 相关文件（可选）
    int line;               // 行号（可选，-1表示无）
    
    CompileMessage() : type(CompileMessageType::Info), line(-1) {}
    CompileMessage(CompileMessageType t, const std::wstring& msg) 
        : type(t), message(msg), line(-1) {}
    CompileMessage(CompileMessageType t, const std::wstring& msg, const std::wstring& f, int l)
        : type(t), message(msg), file(f), line(l) {}
};

// 编译选项
struct CompileOptions {
    bool debug;                     // 是否调试模式
    bool optimizeSize;              // 优化大小
    bool optimizeSpeed;             // 优化速度
    std::wstring outputPath;        // 输出路径
    std::wstring outputName;        // 输出文件名
    std::vector<std::wstring> includePaths;  // 包含路径
    std::vector<std::wstring> libraryPaths;  // 库路径
    
    CompileOptions() : debug(true), optimizeSize(false), optimizeSpeed(false) {}
};

// 编译结果
struct CompileResult {
    bool success;                   // 是否成功
    std::wstring outputFile;        // 输出文件路径
    std::vector<CompileMessage> messages;  // 编译消息
    int errorCount;                 // 错误数量
    int warningCount;               // 警告数量
    DWORD elapsedMs;               // 编译耗时（毫秒）
    
    CompileResult() : success(false), errorCount(0), warningCount(0), elapsedMs(0) {}
};

// 编译器回调函数类型
typedef std::function<void(const CompileMessage&)> CompileCallback;

// 编译器类
class Compiler {
public:
    static Compiler& GetInstance();
    
    // 编译项目
    CompileResult CompileProject(const CompileOptions& options);
    
    // 运行已编译的程序
    bool RunExecutable(const std::wstring& exePath, HANDLE* hProcess = nullptr);
    
    // 停止正在运行的程序
    bool StopExecutable();
    
    // 检查是否正在编译
    bool IsCompiling() const { return m_isCompiling; }
    
    // 检查是否有程序正在运行
    bool IsRunning() const { return m_hRunningProcess != NULL; }
    
    // 设置编译消息回调
    void SetCallback(CompileCallback callback) { m_callback = callback; }
    
    // 获取最后的编译结果
    const CompileResult& GetLastResult() const { return m_lastResult; }
    
    // 获取临时目录
    std::wstring GetTempDirectory() const;
    
    // 获取输出目录
    std::wstring GetOutputDirectory() const;
    
    // 获取程序所在目录
    std::wstring GetAppDirectory() const;
    
private:
    Compiler();
    ~Compiler();
    Compiler(const Compiler&) = delete;
    Compiler& operator=(const Compiler&) = delete;
    
    bool m_isCompiling;
    HANDLE m_hRunningProcess;
    CompileCallback m_callback;
    CompileResult m_lastResult;
    
    // 发送编译消息
    void SendMessage(CompileMessageType type, const std::wstring& message);
    void SendMessage(CompileMessageType type, const std::wstring& message, 
                    const std::wstring& file, int line);
    
    // 语法检查
    bool CheckProjectSyntax(const struct ProjectInfo* project);
    bool CheckFileSyntax(const std::wstring& filePath);
    
    // 读取源文件内容
    std::wstring ReadSourceFile(const std::wstring& filePath);
    
    // 生成C++代码 (用于 MinGW/MSVC)
    bool GenerateCppCode(const std::wstring& outputDir);
    
    // 生成纯C代码 (用于 TCC)
    bool GenerateCCode(const std::wstring& outputDir);
    
    // 调用C++编译器
    bool InvokeCppCompiler(const std::wstring& cppFile, const std::wstring& outputExe,
                          const CompileOptions& options);
    
    // 调用TCC编译器
    bool InvokeTccCompiler(const std::wstring& cFile, const std::wstring& outputExe,
                          const CompileOptions& options);
    
    // 查找C++编译器路径
    std::wstring FindCppCompiler();
    
    // 查找TCC编译器路径 (返回空表示未找到)
    std::wstring FindTccCompiler(bool target32bit = false);
    
    // 创建进程并捕获输出
    bool CreateCompilerProcess(const std::wstring& cmdLine, std::wstring& output, 
                              const std::wstring& workDir = L"");
};
