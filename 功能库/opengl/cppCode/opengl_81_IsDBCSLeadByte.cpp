#include "..\include_opengl_header.h"

// 调用格式: SDT_BOOL 是否为双字节字符, 命令说明: "如果为双字节返回真，否则返回假。如果指定位置超出文本长度，返回0。"
// 参数<1>: 欲检查文本 SDT_TEXT, 参数说明: NULL
// 参数<2>: 欲检查的字符位置 SDT_INT, 参数说明: "0为首位置，1为第2个位置，如此类推，必须是大于0。"
OPENGL_EXTERN_C void opengl_IsDBCSLeadByte_81_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    INT      arg2 = pArgInf[1].m_int;

}

