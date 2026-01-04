#include "..\include_pop3_header.h"

// 调用格式: SDT_BOOL 接收邮件序号, 命令说明: "此序号对于每个服务器是唯一的，可用来比较邮件是否接收到本地。"
// 参数<1>: 第几封 SDT_INT, 参数说明: "指定是第几封邮件,如果此值为-1就表示获取每一封的序号。"
// 参数<2>: &序号 数组/非数组 SDT_TEXT, 参数说明: "返回一封或多封邮件的序号。"
POP3_EXTERN_C void pop3_GetMailNo_29_pop3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    void**   arg2 = pArgInf[1].m_ppAryData;

}

