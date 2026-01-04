// ModuleParser 使用示例
// 展示如何在 ycIDE 中加载和使用 .ec 模块

#include "ModuleParser.h"
#include "LibraryParser.h"
#include <iostream>

// 示例1：基本使用
void Example_BasicUsage() {
    // 获取 LibraryParser 实例
    LibraryParser& libParser = LibraryParser::GetInstance();
    
    // 加载未加密的模块
    bool success = libParser.LoadModuleFile(L"D:\\易语言\\模块\\精易模块.ec");
    
    if (success) {
        std::wcout << L"模块加载成功！" << std::endl;
        
        // 获取所有命令（包括模块中的命令）
        const auto& commands = libParser.GetCommands();
        std::wcout << L"总命令数: " << commands.size() << std::endl;
        
        // 查找特定命令
        const LibraryCommand* cmd = libParser.FindCommand(L"文件_读入");
        if (cmd) {
            std::wcout << L"找到命令: " << cmd->chineseName << std::endl;
            std::wcout << L"说明: " << cmd->description << std::endl;
            std::wcout << L"返回值: " << cmd->returnType << std::endl;
        }
    } else {
        std::wcout << L"加载失败: " << libParser.GetLastError() << std::endl;
    }
}

// 示例2：加载加密模块
void Example_LoadEncryptedModule() {
    LibraryParser& libParser = LibraryParser::GetInstance();
    
    // 加载加密模块（需要提供密码）
    bool success = libParser.LoadModuleFile(
        L"D:\\易语言\\模块\\加密模块.ec", 
        L"密码123"
    );
    
    if (!success) {
        // 当前版本暂不支持加密模块，会返回提示信息
        std::wcout << libParser.GetLastError() << std::endl;
    }
}

// 示例3：直接使用 ModuleParser
void Example_DirectModuleParser() {
    ModuleParser parser;
    
    // 初始化（需要提供易语言安装目录）
    if (!parser.Initialize(L"C:\\Program Files (x86)\\e\\")) {
        std::wcout << L"初始化失败: " << parser.GetLastError() << std::endl;
        return;
    }
    
    // 加载模块
    if (!parser.LoadModule(L"D:\\易语言\\模块\\测试模块.ec")) {
        std::wcout << L"加载失败: " << parser.GetLastError() << std::endl;
        return;
    }
    
    // 获取解析结果
    const auto& methods = parser.GetMethods();
    const auto& dllCmds = parser.GetDllCommands();
    const auto& dataTypes = parser.GetDataTypes();
    
    std::wcout << L"子程序数: " << methods.size() << std::endl;
    std::wcout << L"DLL命令数: " << dllCmds.size() << std::endl;
    std::wcout << L"数据类型数: " << dataTypes.size() << std::endl;
    
    // 转换为 LibraryCommand 格式
    std::vector<LibraryCommand> commands = parser.ConvertToLibraryCommands(L"测试模块");
    
    // 遍历所有命令
    for (const auto& cmd : commands) {
        std::wcout << L"命令: " << cmd.chineseName << std::endl;
        std::wcout << L"  类别: " << cmd.category << std::endl;
        std::wcout << L"  返回值: " << cmd.returnType << std::endl;
        
        // 显示参数
        for (const auto& param : cmd.parameters) {
            std::wcout << L"  参数: " << param.name 
                      << L" (" << param.type << L")" << std::endl;
        }
    }
}

// 示例4：批量加载多个模块
void Example_BatchLoad() {
    LibraryParser& libParser = LibraryParser::GetInstance();
    
    std::vector<std::wstring> moduleFiles = {
        L"D:\\易语言\\模块\\精易模块.ec",
        L"D:\\易语言\\模块\\扩展界面支持库.ec",
        L"D:\\易语言\\模块\\网络操作.ec"
    };
    
    int successCount = 0;
    for (const auto& file : moduleFiles) {
        if (libParser.LoadModuleFile(file)) {
            successCount++;
            std::wcout << L"✓ " << file << std::endl;
        } else {
            std::wcout << L"✗ " << file 
                      << L" - " << libParser.GetLastError() << std::endl;
        }
    }
    
    std::wcout << L"\n成功加载 " << successCount 
              << L" / " << moduleFiles.size() << L" 个模块" << std::endl;
}

// 示例5：获取模块信息
void Example_GetModuleInfo() {
    ModuleParser parser;
    
    if (!parser.Initialize(L"C:\\Program Files (x86)\\e\\")) {
        return;
    }
    
    if (parser.LoadModule(L"D:\\易语言\\模块\\精易模块.ec")) {
        // 获取模块信息
        const auto& info = parser.GetModuleInfo();
        
        std::wcout << L"模块名称: " << info.projectName << std::endl;
        std::wcout << L"作者: " << info.author << std::endl;
        std::wcout << L"版本: " << info.version << std::endl;
        std::wcout << L"说明: " << info.remark << std::endl;
        std::wcout << L"版权: " << info.copyright << std::endl;
    }
}

int main() {
    // 设置控制台输出为UTF-8
    _setmode(_fileno(stdout), _O_U16TEXT);
    
    std::wcout << L"=== ModuleParser 使用示例 ===" << std::endl << std::endl;
    
    // 运行示例
    Example_BasicUsage();
    
    return 0;
}
