#include "..\include_pop3_header.h"

// 调用格式: SDT_BOOL 获取邮件信息, 命令说明: "获取目前服务器上邮件的个数和大小。"
// 参数<1>: &个数 SDT_INT, 参数说明: "返回邮件的个数。"
// 参数<2>: &大小 SDT_INT, 参数说明: "返回邮件的总的大小，单位为字节。"
POP3_EXTERN_C void pop3_GetMailInfo_12_pop3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PINT     arg1 = pArgInf[0].m_pInt;
    PINT     arg2 = pArgInf[1].m_pInt;

}

