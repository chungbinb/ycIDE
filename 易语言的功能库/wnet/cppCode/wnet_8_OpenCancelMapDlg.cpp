#include "..\include_wnet_header.h"

// 调用格式: _SDT_NULL (局域网操作).打开中断对话框, 命令说明: "打开系统“中断网络驱动器”对话框"
// 参数<1>: [父窗口句柄 SDT_INT], 参数说明: "本参数指定打开的对话框的父窗口"
WNET_EXTERN_C void wnet_OpenCancelMapDlg_8_wnet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

