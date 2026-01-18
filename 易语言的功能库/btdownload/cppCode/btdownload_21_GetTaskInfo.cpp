#include "..\include_btdownload_header.h"

// 调用格式: SDT_BOOL (BT下载).取任务内容, 命令说明: "获得任务内容，在指定的时间(一般是停止任务之前)执行本命令来取得任务信息,在“增加新任务”方法的参数中传进去，可以避免每次都效验文件。"
// 参数<1>: &任务内容 SDT_BIN, 参数说明: "本参数表示要保存的任务内容。"
BTDOWNLOAD_EXTERN_C void btdownload_GetTaskInfo_21_btdownload(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE*  arg1 = pArgInf[1].m_ppBin;

}

