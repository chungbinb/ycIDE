#include "LibraryParser.h"
#include "PinyinHelper.h"
#include "FneParser.h"
#include <sstream>
#include <algorithm>
#include <fstream>
#include <Windows.h>
#include <ShlObj.h>

LibraryParser& LibraryParser::GetInstance() {
    static LibraryParser instance;
    return instance;
}

// 加载 .fne 支持库文件
bool LibraryParser::LoadFneLibrary(const std::wstring& fnePath) {
    FneParser fneParser;
    
    if (!fneParser.LoadFneFile(fnePath)) {
        lastError_ = L"无法加载 FNE 文件: " + fnePath;
        return false;
    }
    
    std::wstring libraryName = fneParser.GetLibraryName();
    const auto& fneCommands = fneParser.GetCommands();
    
    // 调试输出到文件
    CreateDirectoryW(L"logs", NULL);
    std::wofstream debugFile(L"logs\\library_load_debug.txt", std::ios::app);
    debugFile.imbue(std::locale(""));
    debugFile << L"========================================" << std::endl;
    debugFile << L"加载支持库: " << libraryName << std::endl;
    debugFile << L"命令数: " << fneCommands.size() << std::endl;
    
    // 将 FNE 命令转换为 LibraryCommand 格式
    int flowControlCount = 0;
    for (size_t idx = 0; idx < fneCommands.size(); idx++) {
        const auto& fneCmd = fneCommands[idx];
        LibraryCommand cmd;
        cmd.chineseName = fneCmd.name;
        cmd.returnType = fneCmd.returnType;
        cmd.description = fneCmd.description;
        cmd.library = libraryName;
        cmd.category = fneCmd.category;  // 使用从 FNE 读取的真实类别
        
        // 生成拼音和首字母
        cmd.pinyin = PinyinHelper::GetStringPinyins(cmd.chineseName);
        cmd.initials = PinyinHelper::GetStringInitialsList(cmd.chineseName);
        
        // 根据类别判断是否是流程控制命令（需要流程线）
        cmd.needsFlowLine = false;
        cmd.needsBrackets = false;
        
        if (cmd.category == L"流程控制") {
            cmd.needsFlowLine = true;
            cmd.needsBrackets = true;
            flowControlCount++;
            // 调试输出流程控制命令
            if (flowControlCount <= 15) {
                debugFile << L"  流程控制命令: " << cmd.chineseName << L" (类别: " << cmd.category << L")" << std::endl;
            }
        }
        
        // 输出前几个命令的详细信息
        if (idx < 10) {
            debugFile << L"  命令[" << idx << L"]: " << cmd.chineseName << L", 类别: [" << cmd.category << L"]" << std::endl;
        }
        
        // 解析参数：FNE 的参数格式是 "参数名 : 参数类型 [标志]"
        for (const auto& paramStr : fneCmd.parameters) {
            LibraryParameter param;
            
            size_t colonPos = paramStr.find(L" : ");
            if (colonPos != std::wstring::npos) {
                param.name = paramStr.substr(0, colonPos);
                std::wstring typeAndFlags = paramStr.substr(colonPos + 3);
                
                // 检查是否可选
                param.optional = (typeAndFlags.find(L"[可选]") != std::wstring::npos);
                
                // 提取类型
                size_t bracketPos = typeAndFlags.find(L" [");
                if (bracketPos != std::wstring::npos) {
                    param.type = typeAndFlags.substr(0, bracketPos);
                } else {
                    param.type = typeAndFlags;
                }
            } else {
                // 没有冒号，整个就是类型
                param.type = paramStr;
                param.name = L"";
            }
            
            param.description = L"";
            cmd.parameters.push_back(param);
        }
        
        commands.push_back(cmd);
        nameIndex[cmd.chineseName] = commands.size() - 1;
    }
    
    debugFile << L"流程控制命令总数: " << flowControlCount << std::endl;
    
    // 解析数据类型
    const auto& fneDataTypes = fneParser.GetDataTypes();
    debugFile << L"数据类型数: " << fneDataTypes.size() << std::endl;
    
    for (const auto& fneDt : fneDataTypes) {
        LibraryDataType dt;
        dt.name = fneDt.name;
        dt.englishName = fneDt.englishName;
        dt.description = fneDt.description;
        dt.library = libraryName;
        
        dataTypes.push_back(dt);
        
        // 调试输出前几个数据类型
        if (dataTypes.size() <= 10) {
            debugFile << L"  数据类型: " << dt.name << std::endl;
        }
    }
    
    // 解析窗口组件
    const auto& fneWindowUnits = fneParser.GetWindowUnits();
    debugFile << L"窗口组件数: " << fneWindowUnits.size() << std::endl;
    
    for (const auto& fneUnit : fneWindowUnits) {
        WindowUnitInfo unit;
        unit.name = fneUnit.name;
        unit.englishName = fneUnit.englishName;
        unit.description = fneUnit.description;
        unit.libraryName = libraryName;
        unit.isContainer = fneUnit.isContainer;
        unit.properties = fneUnit.properties;
        unit.events = fneUnit.events;
        
        // 根据组件类型确定分类
        if (unit.isContainer) {
            unit.category = L"容器控件";
        } else if (unit.name == L"窗口" || unit.name == L"窗体") {
            unit.category = L"窗口";
        } else {
            unit.category = L"常用控件";
        }
        
        windowUnits.push_back(unit);
        unitIndex[unit.name] = windowUnits.size() - 1;
        
        // 调试输出窗口组件
        debugFile << L"  窗口组件: " << unit.name 
                  << L", 属性数: " << unit.properties.size()
                  << L", 事件数: " << unit.events.size() << std::endl;
    }
    
    debugFile << L"========================================" << std::endl << std::endl;
    debugFile.close();
    
    return true;
}

const LibraryCommand* LibraryParser::FindCommand(const std::wstring& name) const {
    auto it = nameIndex.find(name);
    if (it != nameIndex.end() && it->second < commands.size()) {
        return &commands[it->second];
    }
    return nullptr;
}

const WindowUnitInfo* LibraryParser::FindWindowUnit(const std::wstring& name) const {
    auto it = unitIndex.find(name);
    if (it != unitIndex.end() && it->second < windowUnits.size()) {
        return &windowUnits[it->second];
    }
    return nullptr;
}

std::vector<std::wstring> LibraryParser::GetCompletions(const std::wstring& input) const {
    std::vector<std::wstring> results;
    
    if (input.empty()) {
        return results;
    }
    
    std::wstring lowerInput = input;
    std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::towlower);
    
    for (const auto& cmd : commands) {
        bool matched = false;
        
        // 匹配中文名
        if (cmd.chineseName.find(input) == 0) {
            matched = true;
        }
        
        // 匹配英文名
        if (!matched && !cmd.englishName.empty()) {
            std::wstring lowerEnglish = cmd.englishName;
            std::transform(lowerEnglish.begin(), lowerEnglish.end(), 
                         lowerEnglish.begin(), ::towlower);
            if (lowerEnglish.find(lowerInput) == 0) {
                matched = true;
            }
        }
        
        if (matched) {
            results.push_back(cmd.chineseName);
        }
    }
    
    return results;
}

// 从 FNE 解析器添加命令（用于 LibraryConfig 动态加载其他支持库）
void LibraryParser::AddCommandFromFne(const std::wstring& cmdName, const std::wstring& description,
                                      const std::wstring& returnType, const std::vector<std::wstring>& params,
                                      const std::wstring& libraryName) {
    LibraryCommand cmd;
    cmd.chineseName = cmdName;
    cmd.returnType = returnType;
    cmd.description = description;
    cmd.library = libraryName;
    cmd.category = libraryName;  // 这里暂时使用库名作为类别
    
    // 生成拼音和首字母
    cmd.pinyin = PinyinHelper::GetStringPinyins(cmd.chineseName);
    cmd.initials = PinyinHelper::GetStringInitialsList(cmd.chineseName);
    
    // 默认不是流程控制命令（如果需要，应该从 FNE 文件中读取类别信息）
    cmd.needsFlowLine = false;
    cmd.needsBrackets = false;
    
    // 解析参数
    for (const auto& paramStr : params) {
        LibraryParameter param;
        
        // 参数格式: "参数名 : 参数类型 [可选]"
        size_t colonPos = paramStr.find(L" : ");
        if (colonPos != std::wstring::npos) {
            param.name = paramStr.substr(0, colonPos);
            std::wstring typeAndFlags = paramStr.substr(colonPos + 3);
            
            // 检查是否可选
            param.optional = (typeAndFlags.find(L"[可选]") != std::wstring::npos ||
                            typeAndFlags.find(L"[变量地址]") != std::wstring::npos);
            
            // 提取类型
            size_t bracketPos = typeAndFlags.find(L" [");
            if (bracketPos != std::wstring::npos) {
                param.type = typeAndFlags.substr(0, bracketPos);
            } else {
                param.type = typeAndFlags;
            }
        } else {
            param.name = paramStr;
            param.type = L"";
        }
        
        cmd.parameters.push_back(param);
    }
    
    commands.push_back(cmd);
    nameIndex[cmd.chineseName] = commands.size() - 1;
}

// 获取所有数据类型名称（包括基础类型和支持库自定义类型）
std::vector<std::wstring> LibraryParser::GetAllDataTypeNames() const {
    std::vector<std::wstring> types;
    
    // 基础数据类型（按照易语言习惯排序）
    types.push_back(L"整数型");
    types.push_back(L"文本型");
    types.push_back(L"小数型");
    types.push_back(L"逻辑型");
    types.push_back(L"字节型");
    types.push_back(L"短整数型");
    types.push_back(L"长整数型");
    types.push_back(L"双精度小数型");
    types.push_back(L"日期时间型");
    types.push_back(L"字节集");
    types.push_back(L"子程序指针");
    
    // 添加支持库自定义数据类型
    for (const auto& dt : dataTypes) {
        // 避免重复
        bool exists = false;
        for (const auto& t : types) {
            if (t == dt.name) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            types.push_back(dt.name);
        }
    }
    
    return types;
}

// 加载.ec模块文件
// 模块加载功能已移除
/*
bool LibraryParser::LoadModuleFile(const std::wstring& filePath, const std::wstring& password) {
    ... 已删除 ...
}
*/

