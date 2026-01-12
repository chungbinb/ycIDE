#include <windows.h>
#include <iostream>
#include <string>
#include "../ycIDE/FneParser.h"
#include "../ycIDE/LibraryParser.h"

int main() {
    // 测试加载 krnln.fne
    std::wstring fnePath = L"d:\\chungbin\\ycide\\功能库\\krnln\\x64\\Debug\\krnln.fne";
    
    std::wcout.imbue(std::locale(""));
    
    std::wcout << L"开始测试 FNE 类别加载..." << std::endl;
    std::wcout << L"FNE 文件路径: " << fnePath << std::endl << std::endl;
    
    // 测试 LibraryParser
    LibraryParser& parser = LibraryParser::GetInstance();
    if (parser.LoadFneLibrary(fnePath)) {
        std::wcout << L"✓ FNE 文件加载成功！" << std::endl;
        std::wcout << L"总命令数: " << parser.GetCommandCount() << std::endl << std::endl;
        
        // 测试几个流程控制命令
        std::wstring testCommands[] = {L"如果", L"如果真", L"判断", L"计次循环首", L"变量循环首", L"相加", L"信息框"};
        
        std::wcout << L"测试命令类别和属性:" << std::endl;
        std::wcout << L"----------------------------------------" << std::endl;
        for (const auto& cmdName : testCommands) {
            const LibraryCommand* cmd = parser.FindCommand(cmdName);
            if (cmd) {
                std::wcout << L"命令: " << cmd->chineseName << std::endl;
                std::wcout << L"  类别: " << cmd->category << std::endl;
                std::wcout << L"  返回值: " << cmd->returnType << std::endl;
                std::wcout << L"  需要流程线: " << (cmd->needsFlowLine ? L"是" : L"否") << std::endl;
                std::wcout << L"  需要括号: " << (cmd->needsBrackets ? L"是" : L"否") << std::endl;
                std::wcout << std::endl;
            } else {
                std::wcout << L"✗ 未找到命令: " << cmdName << std::endl << std::endl;
            }
        }
    } else {
        std::wcerr << L"✗ 加载 FNE 文件失败" << std::endl;
        std::wcerr << L"错误: " << parser.GetLastError() << std::endl;
    }
    
    system("pause");
    return 0;
}
