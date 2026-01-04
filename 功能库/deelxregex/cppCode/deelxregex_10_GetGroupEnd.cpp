#include "..\include_deelxregex_header.h"

// 调用格式: SDT_INT (搜索结果DEELX).获取分组结束位置, 命令说明: "返回指定分组捕获的字符串的结束位置。如果指定分组未捕获，则返回负值。注意：本支持库内部采用Unicode编码，返回的位置也是相对于Unicode字符串的。"
// 参数<1>: 分组编号 SDT_INT, 参数说明: "分组编号"
DEELXREGEX_EXTERN_C void deelxregex_GetGroupEnd_10_deelxregex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

