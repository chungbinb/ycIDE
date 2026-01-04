#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 校验立方体纹理, 命令说明: "校验立方体纹理参数。"
// 参数<1>: 设备 MAKELONG(0x09, 0), 参数说明: "指定与立方体纹理相关联的设备。"
// 参数<2>: &尺寸 SDT_INT, 参数说明: "指定宽度和高度（以像素为单位）。并返回校正后的尺寸。"
// 参数<3>: &渐进映射级数 SDT_INT, 参数说明: "指定渐进映射级数。并返回校正后的渐进映射级数。"
// 参数<4>: 用途 SDT_INT, 参数说明: "为以下两个值之一：0、1。如果为1，值相当于#资源用途.渲染目标。"
// 参数<5>: &像素格式 MAKELONG(0x35, 0), 参数说明: "要取得的像素格式。并返回校正后的像素格式。"
// 参数<6>: 内存池类型 MAKELONG(0x39, 0), 参数说明: "指定纹理的放置位置。为“内存池类型”常量值之一。"
OGRELIB_EXTERN_C void ogrelib_D3DXCheckCubeTextureRequirements_245_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pCompoundData;
    PINT     arg2 = pArgInf[1].m_pInt;
    PINT     arg3 = pArgInf[2].m_pInt;
    INT      arg4 = pArgInf[3].m_int;
    void**   arg5 = pArgInf[4].m_ppCompoundData;
    void*    arg6 = pArgInf[5].m_pCompoundData;

}

