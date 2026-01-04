#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).置源数据流, 命令说明: "使顶点缓冲区绑定一个设备数据流。渲染顶点缓冲中的顶点数据需要几个步骤。首先，你需要设置源数据流。"
// 参数<1>: 数据流 SDT_INT, 参数说明: NULL
// 参数<2>: 绑定的顶点缓冲 MAKELONG(0x13, 0), 参数说明: "将被绑定到数据源上的顶点缓冲。"
// 参数<3>: 顶点数据大小 SDT_INT, 参数说明: "每一个顶点数据的大小，以字节为单位。"
// 参数<4>: 流组件大小 SDT_INT, 参数说明: "流组件的大小，以字节为单位。"
OGRELIB_EXTERN_C void ogrelib_SetStreamSource_155_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void*    arg2 = pArgInf[2].m_pCompoundData;
    INT      arg3 = pArgInf[3].m_int;
    INT      arg4 = pArgInf[4].m_int;

}

