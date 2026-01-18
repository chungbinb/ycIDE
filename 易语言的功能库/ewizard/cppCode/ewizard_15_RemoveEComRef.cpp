#include "..\include_ewizard_header.h"

// 调用格式: _SDT_NULL 删除模块引用, 命令说明: "删除程序中对指定易模块文件的引用"
// 参数<1>: 模块文件名 SDT_TEXT, 参数说明: "本参数提供欲删除引用的不带路径易模块文件名"
EWIZARD_EXTERN_C void ewizard_RemoveEComRef_15_ewizard(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;

}

