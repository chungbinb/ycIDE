#include "..\include_ewizard_header.h"

// 调用格式: _SDT_NULL 置作者信息, 命令说明: "设置程序与作者相关的信息"
// 参数<1>: [作者名称 SDT_TEXT], 参数说明: "如果本参数被省略，则不改动此项"
// 参数<2>: [邮政编码 SDT_TEXT], 参数说明: "如果本参数被省略，则不改动此项"
// 参数<3>: [联系地址 SDT_TEXT], 参数说明: "如果本参数被省略，则不改动此项"
// 参数<4>: [电话 SDT_TEXT], 参数说明: "如果本参数被省略，则不改动此项"
// 参数<5>: [传真 SDT_TEXT], 参数说明: "如果本参数被省略，则不改动此项"
// 参数<6>: [电子信箱 SDT_TEXT], 参数说明: "如果本参数被省略，则不改动此项"
// 参数<7>: [主页地址 SDT_TEXT], 参数说明: "如果本参数被省略，则不改动此项"
// 参数<8>: [其它信息 SDT_TEXT], 参数说明: "如果本参数被省略，则不改动此项"
EWIZARD_EXTERN_C void ewizard_SetAuthorInfo_9_ewizard(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;
    LPSTR    arg4 = pArgInf[3].m_pText;
    LPSTR    arg5 = pArgInf[4].m_pText;
    LPSTR    arg6 = pArgInf[5].m_pText;
    LPSTR    arg7 = pArgInf[6].m_pText;
    LPSTR    arg8 = pArgInf[7].m_pText;

}

