#include "..\include_btdownload_header.h"

// 调用格式: SDT_BOOL (BT下载).添加服务器, 命令说明: "手动添加服务器(Tracker)。注：有些BT的Tracker服务器有备用的，并不在发布文件中，所以要有此方法。"
// 参数<1>: 地址 SDT_TEXT, 参数说明: "本参数表示备用服务器(Tracker)的IP地址。"
// 参数<2>: 端口 SDT_INT, 参数说明: "本参数表示要连接的端口。"
BTDOWNLOAD_EXTERN_C void btdownload_AddTreckerServer_26_btdownload(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    INT      arg2 = pArgInf[2].m_int;

}

