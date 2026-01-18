#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x04, 0) (变换矩阵).建绕任意轴旋转矩阵, 命令说明: "创建一个可以绕着任意轴旋转的旋转矩阵"
// 参数<1>: 轴向量 MAKELONG(0x02, 0), 参数说明: "指向任意轴向量"
// 参数<2>: 转角 SDT_FLOAT, 参数说明: "绕着任意轴旋转的角度（单位是弧度）"
OGRELIB_EXTERN_C void ogrelib_D3DXMatrixRotationAxis_322_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    FLOAT    arg2 = pArgInf[2].m_float;

}

