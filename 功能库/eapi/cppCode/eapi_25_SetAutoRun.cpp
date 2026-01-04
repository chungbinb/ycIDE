#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 设置自动运行, 命令说明: "设置程序在开机时自动运行。本命令为初级命令"
// 参数<1>: 程序类名 SDT_TEXT, 参数说明: "程序类名、置于启动组或者注册表中的名称"
// 参数<2>: 程序路径 SDT_TEXT, 参数说明: "要设置为自动运行的程序文件名（包含绝对路径）。"
// 参数<3>: [位置 SDT_INT], 参数说明: "可以被省略。为以下常量值之一：0、#接口常量.到注册表；1、#接口常量.到启动组。"
EAPI_EXTERN_C void eapi_SetAutoRun_25_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR    arg2 = pArgInf[1].m_pText;
    INT      arg3 = pArgInf[2].m_int;

}

