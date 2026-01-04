#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).结束记录状态块, 命令说明: "当应用程序完成对状态块的记录后，应该调用本方法通知系统以结束记录。如果该方法失败，很可能是因为显示模式改变了。只要重新创建表面并再次记录状态块就可以达到这个目的。"
// 参数<1>: 状态块句柄 MAKELONG(0x20, 0), 参数说明: NULL
OGRELIB_EXTERN_C void ogrelib_EndStateBlock_194_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

