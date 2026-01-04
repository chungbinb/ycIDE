#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).置顶点处理模式, 命令说明: "设置顶点处理模式，可以设为软件顶点处理和硬件顶点处理两种模式。"
// 参数<1>: 顶点处理模式 SDT_INT, 参数说明: "要设置的顶点处理模式。为以下两个值之一：32、#设备常量.软件顶点处理；64、#设备常量.硬件顶点处理。"
OGRELIB_EXTERN_C void ogrelib_SetVertexProcessing_170_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

