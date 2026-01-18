#include "..\include_downlib_header.h"

// 调用格式: SDT_INT (FTP辅助对象).执行FTP命令, 命令说明: "执行一条FTP命令, 返回服务器返回的信息代码。如果执行失败返回0。 本命令只有通过“连接FTP服务器”命令连接到FTP服务器以后才能使用。"
// 参数<1>: FTP命令 SDT_TEXT, 参数说明: "欲执行的FTP命令"
// 参数<2>: [&返回命令数据 SDT_TEXT], 参数说明: "执行FTP命令后服务器返回的命令文本，最大支持返回51200字节。"
DOWNLIB_EXTERN_C void downlib_FtpRunCmd_64_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR*   arg2 = pArgInf[2].m_ppText;

}

