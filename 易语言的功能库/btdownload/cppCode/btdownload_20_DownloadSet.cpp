#include "..\include_btdownload_header.h"

// 调用格式: SDT_BOOL 下载设置, 命令说明: "进行一些基本的全局的参数设置。"
// 参数<1>: 下载设置信息 MAKELONG(0x07, 0), 参数说明: "本参数表示下载的参数信息。"
BTDOWNLOAD_EXTERN_C void btdownload_DownloadSet_20_btdownload(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pCompoundData;

}

