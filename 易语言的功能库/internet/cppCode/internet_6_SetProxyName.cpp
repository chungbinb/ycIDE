#include "..\include_internet_header.h"

// 调用格式: _SDT_NULL 置代理服务器, 命令说明: "设置本库此后的HTTP和FTP新建连接所使用的代理服务器信息。对FTP操作而言，目前不支持代理服务器验证，将忽略用户名和密码参数。"
// 参数<1>: [代理服务器名称 SDT_TEXT], 参数说明: "如果要取消代理服务器设置，请提供空文本。如果本参数被省略，默认值为空文本"
// 参数<2>: [用户名 SDT_TEXT], 参数说明: NULL
// 参数<3>: [密码 SDT_TEXT], 参数说明: NULL
INTERNET_EXTERN_C void internet_SetProxyName_6_internet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;

}

