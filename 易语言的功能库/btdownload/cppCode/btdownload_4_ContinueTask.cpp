#include "..\include_btdownload_header.h"

// 调用格式: SDT_BOOL (BT下载).继续本任务, 命令说明: "继续下载一个暂停的任务。如果发现下载的文件丢失或数据有误，可在“重新检查完整性”后指定新的任务内容。"
// 参数<1>: [任务内容 SDT_BIN], 参数说明: "如果提供本参数，则更新当前任务内容。如果被省略，保持当前任务内容不变。"
BTDOWNLOAD_EXTERN_C void btdownload_ContinueTask_4_btdownload(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[1].m_pBin;

}

