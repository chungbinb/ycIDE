#include "..\include_internet_header.h"

// 调用格式: SDT_BOOL 连接FTP服务器, 命令说明: "连接到互联网上指定地址处的FTP服务器。在进行所有FTP操作之前必须首先成功连接FTP服务器。成功返回真，失败返回假"
// 参数<1>: FTP服务器地址 SDT_TEXT, 参数说明: "本参数指定FTP服务器在互联网上的地址"
// 参数<2>: [用户名 SDT_TEXT], 参数说明: "本参数提供用作通过FTP服务器用户验证的用户名称，如果被省略，默认值为匿名用户名“anonymous”"
// 参数<3>: [密码 SDT_TEXT], 参数说明: "本参数提供用作通过FTP服务器用户验证的密码，如果被省略，默认值为空文本"
// 参数<4>: [端口号 SDT_INT], 参数说明: "本参数提供欲连接到FTP服务器上的端口号，如果被省略，默认值为标准端口 21 "
// 参数<5>: [是否使用被动语义 SDT_BOOL], 参数说明: "如果本参数被省略，默认值为假"
INTERNET_EXTERN_C void internet_ConnectFTPServer_8_internet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;
    INT      arg4 = pArgInf[3].m_int;
    BOOL     arg5 = pArgInf[4].m_bool;

}

