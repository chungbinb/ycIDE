#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x04, 0) (变换矩阵).建Z轴旋转矩阵, 命令说明: "创建绕着Z轴旋转矩阵"
// 参数<1>: 转角 SDT_FLOAT, 参数说明: "绕着Z轴旋转的角度（单位是弧度）"
OGRELIB_EXTERN_C void ogrelib_D3DXMatrixRotationZ_326_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    FLOAT    arg1 = pArgInf[1].m_float;

}

