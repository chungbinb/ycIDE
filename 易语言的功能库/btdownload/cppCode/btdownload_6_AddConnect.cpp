#include "..\include_btdownload_header.h"

// 调用格式: SDT_BOOL (BT下载).增加连接, 命令说明: "增加一个连接。"
// 参数<1>: 地址 SDT_TEXT, 参数说明: "本参数表示要连接的IP地址，如果本参数为空则自动选择。"
// 参数<2>: 端口号 SDT_INT, 参数说明: "本参数表示端口号。"
BTDOWNLOAD_EXTERN_C void btdownload_AddConnect_6_btdownload(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    INT      arg2 = pArgInf[2].m_int;

}

