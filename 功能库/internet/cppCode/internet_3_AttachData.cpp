#include "..\include_internet_header.h"

// 调用格式: SDT_BOOL 添加附件数据, 命令说明: "添加附件数据到即将发送的邮件中，成功返回真，失败返回假"
// 参数<1>: 欲添加的附件数据 SDT_BIN, 参数说明: NULL
// 参数<2>: 附件标题 SDT_TEXT, 参数说明: NULL
INTERNET_EXTERN_C void internet_AttachData_3_internet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[0].m_pBin;
    LPSTR    arg2 = pArgInf[1].m_pText;

}

