#include "..\include_btdownload_header.h"

// 调用格式: MAKELONG(0x05, 0) 分析发布文件, 命令说明: "分析发布文件(Torrent文件)，取到其中的信息。"
// 参数<1>: 发布文件名 SDT_TEXT, 参数说明: "本参数表示指定发布文件名(Torrent)。"
BTDOWNLOAD_EXTERN_C void btdownload_AnalyseTorrentFile_13_btdownload(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;

}

