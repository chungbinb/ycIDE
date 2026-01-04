#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).创建新状态块, 命令说明: "创建一个新的状态块，它包含全部的设备状态或是仅与顶点或像素处理相关的设备状态。如果该方法失败，很可能是显示模式改变了。为了从此类失败中恢复，应用程序应该重新创建表面，然后重新创建状态块。"
// 参数<1>: 状态信息类型 SDT_INT, 参数说明: "要在新的状态块中记录的状态信息的类型。为以下常量值之一：1、#设备常量.顶点和像素状态；2、#设备常量.像素状态；3、#设备常量.顶点状态。"
// 参数<2>: 状态块句柄 MAKELONG(0x20, 0), 参数说明: NULL
OGRELIB_EXTERN_C void ogrelib_CreateStateBlock_192_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

