#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网筛选器).置认证信息, 命令说明: "修改客户端提交的用户名密码认证信息。仅当“取事件类型()”返回值为 #筛选器事件.认证 的情况下才能调用本方法。"
// 参数<1>: [用户名 SDT_TEXT], 参数说明: "如果文本太长可能会被截断。省略此参数表示不修改用户名。"
// 参数<2>: [密码 SDT_TEXT], 参数说明: "如果文本太长可能会被截断。省略此参数表示不修改密码。"
ISAPI_EXTERN_C void isapi_FilterSetAuthInfo_78_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;

}

