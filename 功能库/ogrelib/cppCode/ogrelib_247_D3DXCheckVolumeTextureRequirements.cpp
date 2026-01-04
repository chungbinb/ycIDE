#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 校验立方纹理, 命令说明: "校验立方纹理参数。"
// 参数<1>: 设备 MAKELONG(0x09, 0), 参数说明: "指定与立体纹理相关联的设备。"
// 参数<2>: &宽度 SDT_INT, 参数说明: "指定宽度（以像素为单位）。并返回校正后的尺寸。"
// 参数<3>: &高度 SDT_INT, 参数说明: "指定高度（以像素为单位）。并返回校正后的尺寸。"
// 参数<4>: &深度 SDT_INT, 参数说明: "指定深度（以像素为单位）。并返回校正后的尺寸。"
// 参数<5>: &渐进映射级数 SDT_INT, 参数说明: "指定渐进映射级数。返回校正后的渐进映射级数。"
// 参数<6>: 用途 SDT_INT, 参数说明: "没用，置为0。"
// 参数<7>: &像素格式 MAKELONG(0x35, 0), 参数说明: "要取得的像素格式。并返回校正后的像素格式。"
// 参数<8>: 内存池类型 MAKELONG(0x39, 0), 参数说明: "指定纹理的放置位置。为“内存池类型”常量值之一。"
OGRELIB_EXTERN_C void ogrelib_D3DXCheckVolumeTextureRequirements_247_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pCompoundData;
    PINT     arg2 = pArgInf[1].m_pInt;
    PINT     arg3 = pArgInf[2].m_pInt;
    PINT     arg4 = pArgInf[3].m_pInt;
    PINT     arg5 = pArgInf[4].m_pInt;
    INT      arg6 = pArgInf[5].m_int;
    void**   arg7 = pArgInf[6].m_ppCompoundData;
    void*    arg8 = pArgInf[7].m_pCompoundData;

}

