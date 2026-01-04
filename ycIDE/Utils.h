#pragma once
#include <string>
#include <vector>
#include <windows.h>

struct EditorDocument;

bool SaveFile(const std::wstring& path, EditorDocument* data);
bool LoadFile(const std::wstring& path, EditorDocument* data);
bool ShowSaveDialog(HWND hWnd, std::wstring& path);
bool ShowOpenDialog(HWND hWnd, std::wstring& path);

// 格式转换函数
std::wstring ConvertEPLToInternal(const std::wstring& eplCode);
std::wstring ConvertInternalToEPL(const std::vector<std::wstring>& lines);
std::vector<std::wstring> SplitString(const std::wstring& str, const std::wstring& delimiter);

// 检查一行是否是需要绘制流程线的流程控制命令
bool IsFlowControlLine(const std::wstring& line);

// 拼音转换工具
std::wstring ChineseToPinyin(const std::wstring& chinese);
std::wstring ChineseToPinyinInitials(const std::wstring& chinese);
bool FuzzyMatch(const std::wstring& input, const std::wstring& target);
