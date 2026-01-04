#pragma once


#include <Windows.h>
#include <string>
#include <vector>
#include <map>

#include "../ECodeParser/efs.h"
#include "../elib/lib2.h"


#define EVENT_INFO "info"
#define EVENT_LIB "lib"
#define EVENT_EC "ec"
#define EVENT_WINDOW "window"
#define EVENT_CLASS "class"
#define EVENT_ASSEMBLY "assembly"
#define EVENT_METHOD "method"
#define EVENT_STRUCT "struct"
#define EVENT_DLL "dll"
#define EVENT_GLOBAL "global"
#define EVENT_CONST "const"
#define EVENT_RESOURCE "resource"
#define EVENT_END "end"

typedef void(__stdcall * EVENT)(void* obj);

class ECode;

