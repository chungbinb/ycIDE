#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 创建空立体纹理, 命令说明: "创建一个空立体纹理。"
// 参数<1>: 设备 MAKELONG(0x09, 0), 参数说明: "指定与立体纹理相关联的设备。"
// 参数<2>: 宽度 SDT_INT, 参数说明: "立体纹理的宽度（以像素为单位）。不能为0。"
// 参数<3>: 高度 SDT_INT, 参数说明: "立体纹理的高度（以像素为单位）。不能为0。"
// 参数<4>: 深度 SDT_INT, 参数说明: "立体纹理的深度（以像素为单位）。不能为0。"
// 参数<5>: 渐进级数 SDT_INT, 参数说明: "指定渐进级数。如果参数值为0或-1，将创建一个完整的渐进映射链。"
// 参数<6>: 用途 MAKELONG(0x37, 0), 参数说明: "为“资源用途”常量值之一。"
// 参数<7>: 像素格式 MAKELONG(0x35, 0), 参数说明: "要取得的像素格式。"
// 参数<8>: 内存池类型 MAKELONG(0x39, 0), 参数说明: "指定纹理的放置位置。为“内存池类型”常量值之一。"
// 参数<9>: &立体纹理 MAKELONG(0x11, 0), 参数说明: "要创建的立体纹理对象。"
OGRELIB_EXTERN_C void ogrelib_D3DXCreateVolumeTexture_259_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pCompoundData;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;
    INT      arg4 = pArgInf[3].m_int;
    INT      arg5 = pArgInf[4].m_int;
    void*    arg6 = pArgInf[5].m_pCompoundData;
    void*    arg7 = pArgInf[6].m_pCompoundData;
    void*    arg8 = pArgInf[7].m_pCompoundData;
    void**   arg9 = pArgInf[8].m_ppCompoundData;

}

