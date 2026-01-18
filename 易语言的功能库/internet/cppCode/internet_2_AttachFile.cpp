#include "..\include_internet_header.h"

// 调用格式: SDT_BOOL 添加附件文件, 命令说明: "添加指定的文件附件到即将发送的邮件中，成功返回真，失败返回假"
// 参数<1>: 欲添加的附件文件 SDT_TEXT, 参数说明: NULL
INTERNET_EXTERN_C void internet_AttachFile_2_internet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;

}

