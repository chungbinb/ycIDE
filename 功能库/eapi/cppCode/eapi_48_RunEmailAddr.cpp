#include "..\include_eapi_header.h"

// 调用格式: _SDT_NULL 撰写邮件, 命令说明: "调用系统默认邮箱工具并置目标邮箱为参数"
// 参数<1>: Email地址 SDT_TEXT, 参数说明: "目标Email地址"
EAPI_EXTERN_C void eapi_RunEmailAddr_48_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;

}

