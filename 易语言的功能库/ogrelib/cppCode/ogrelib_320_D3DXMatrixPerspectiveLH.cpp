#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x04, 0) (变换矩阵).建透视投影矩阵扩展, 命令说明: "创建一个左手坐标系的透视投影矩阵"
// 参数<1>: 前截面宽度 SDT_FLOAT, 参数说明: "前观察面的宽度"
// 参数<2>: 前截面高度 SDT_FLOAT, 参数说明: "前观察面的高度"
// 参数<3>: 前裁剪面位置 SDT_FLOAT, 参数说明: "前裁剪面到原点的距离"
// 参数<4>: 后裁剪面位置 SDT_FLOAT, 参数说明: "后裁剪面到原点的距离"
OGRELIB_EXTERN_C void ogrelib_D3DXMatrixPerspectiveLH_320_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    FLOAT    arg1 = pArgInf[1].m_float;
    FLOAT    arg2 = pArgInf[2].m_float;
    FLOAT    arg3 = pArgInf[3].m_float;
    FLOAT    arg4 = pArgInf[4].m_float;

}

