#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).设置材质, 命令说明: "设置以后渲染用的材质,渲染过一个物体之后可以再次调用此方法设置另外一种材质继续渲染下一个物体。"
// 参数<1>: 材质 MAKELONG(0x0C, 0), 参数说明: "本参数表示要设定的材质。"
OGRELIB_EXTERN_C void ogrelib_SetMaterial_147_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

