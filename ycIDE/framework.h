// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
#define NOMINMAX                        // 防止 Windows.h 定义 min/max 宏，避免与标准库冲突
// Windows 头文件
#include <windows.h>
#include <objidl.h>  // 为GDI+提供IStream等类型
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
