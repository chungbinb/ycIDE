#include "..\include_shell_header.h"

// 调用格式: SDT_TEXT 查询快捷方式, 命令说明: "如果文件名不包含后缀，将自动加上“.lnk”后缀名"
// 参数<1>: 快捷方式文件名 SDT_TEXT, 参数说明: NULL
SHELL_EXTERN_C void shell_GetShortCut_1_shell(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;

}

