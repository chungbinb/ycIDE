#include "..\include_btdownload_header.h"

// 调用格式: SDT_BOOL 测试代理服务器, 命令说明: "测试代理服务器是否能成功连上。注：调用此方法要先调用“下载设置”命令设置代理服务器相关内容。"
// 参数<1>: 地址 SDT_TEXT, 参数说明: "本参数表示要连接的IP地址，如www.dywt.com.cn。"
// 参数<2>: 端口 SDT_INT, 参数说明: "本参数表示要连接的端口。"
BTDOWNLOAD_EXTERN_C void btdownload_TestProxyServer_25_btdownload(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    INT      arg2 = pArgInf[1].m_int;

}

