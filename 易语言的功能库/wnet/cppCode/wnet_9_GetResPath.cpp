#include "..\include_wnet_header.h"

// 调用格式: SDT_TEXT (局域网操作).取对应资源, 命令说明: "取得指定的网络驱动器所对应的共享资源路径。失败返回空文本"
// 参数<1>: 网络驱动器名称 SDT_TEXT, 参数说明: "本参数指定欲取其对应的资源路径的网络驱动器，比如“X:”"
WNET_EXTERN_C void wnet_GetResPath_9_wnet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

