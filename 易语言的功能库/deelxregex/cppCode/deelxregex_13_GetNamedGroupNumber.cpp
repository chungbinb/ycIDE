#include "..\include_deelxregex_header.h"

// 调用格式: SDT_INT (正则表达式DEELX).获取命名分组编号, 命令说明: "通过命名分组名，返回命名分组编号。"
// 参数<1>: 命名分组名 SDT_TEXT, 参数说明: "命名分组名"
DEELXREGEX_EXTERN_C void deelxregex_GetNamedGroupNumber_13_deelxregex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

