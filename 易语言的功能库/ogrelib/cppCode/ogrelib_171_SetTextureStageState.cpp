#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).置纹理层状态, 命令说明: "设置纹理层状态值"
// 参数<1>: 索引 SDT_INT, 参数说明: "本参数表示纹理层索引。值可以是0至7。D3D最少允许有8层纹理。"
// 参数<2>: 类型 MAKELONG(0x30, 0), 参数说明: "本参数表示纹理层类型, 值可以是“纹理层状态类型”常量数据类型中的值。"
// 参数<3>: 值 SDT_INT, 参数说明: "本参数表示纹理层状态类型的值。意义根据纹理层状态类型的不同而不同。"
OGRELIB_EXTERN_C void ogrelib_SetTextureStageState_171_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void*    arg2 = pArgInf[2].m_pCompoundData;
    INT      arg3 = pArgInf[3].m_int;

}

