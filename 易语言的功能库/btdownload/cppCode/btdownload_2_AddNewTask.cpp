#include "..\include_btdownload_header.h"

// 调用格式: SDT_BOOL (BT下载).增加新任务, 命令说明: "增加一个下载任务。注：本函数返回真并不是已经真正开始下载了，要通过“其它日志”中的“下载已全部停止”的参数三来判断。"
// 参数<1>: 任务信息 MAKELONG(0x02, 0), 参数说明: "本参数表示下载任务的相关信息。"
BTDOWNLOAD_EXTERN_C void btdownload_AddNewTask_2_btdownload(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

