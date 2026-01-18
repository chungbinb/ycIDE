#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).重置, 命令说明: "重置交换链的类型、尺寸和格式。本方法是当设备丢失时唯一有效的方法，并且是应用程序可用来把设备从丢失状态恢复到操作状态的唯一方法。"
// 参数<1>: [设备参数 MAKELONG(0x1B, 0)], 参数说明: "设备描述参数。"
OGRELIB_EXTERN_C void ogrelib_Reset_204_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

