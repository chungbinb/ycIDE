#include "..\include_btdownload_header.h"

// 调用格式: SDT_BOOL (BT下载).限制上传速度, 命令说明: "限制上传速度。"
// 参数<1>: 上传速度 SDT_INT, 参数说明: "本参数表示要限制的速度，单位为字节数/每秒。如 1024/s,表示1k/s。"
BTDOWNLOAD_EXTERN_C void btdownload_SetUploadRate_19_btdownload(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

