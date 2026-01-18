#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x02, 0) (三维向量).缩放向量, 命令说明: "对向量进行按比例缩放"
// 参数<1>: 比例因子 SDT_FLOAT, 参数说明: "对向量进行缩放的比例因子"
OGRELIB_EXTERN_C void ogrelib_D3DXVec3Scale_295_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    FLOAT    arg1 = pArgInf[1].m_float;

}

