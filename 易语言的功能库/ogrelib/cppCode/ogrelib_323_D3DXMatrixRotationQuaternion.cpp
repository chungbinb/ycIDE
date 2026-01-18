#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x04, 0) (变换矩阵).建四元数旋转矩阵, 命令说明: "从一个四元数创建一个旋转矩阵"
// 参数<1>: 轴向量 MAKELONG(0x02, 0), 参数说明: "指向任意轴向量"
OGRELIB_EXTERN_C void ogrelib_D3DXMatrixRotationQuaternion_323_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

