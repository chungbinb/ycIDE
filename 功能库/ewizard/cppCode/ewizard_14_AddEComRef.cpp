#include "..\include_ewizard_header.h"

// 调用格式: _SDT_NULL 添加模块引用, 命令说明: "增加对指定易模块文件的引用到程序中"
// 参数<1>: 模块文件路径名 SDT_TEXT, 参数说明: "本参数提供欲添加引用的模块文件路径名。如果以“$”字符开始，表示后面是基于易语言系统模块目录的相对路径。如：$classlib\\d3d.ec，代表在易语言系统模块目录下classlib子目录中的d3d.ec模块文件"
EWIZARD_EXTERN_C void ewizard_AddEComRef_14_ewizard(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;

}

