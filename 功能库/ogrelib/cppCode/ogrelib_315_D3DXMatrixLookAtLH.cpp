#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x04, 0) (变换矩阵).建观察矩阵, 命令说明: "创建一个观察矩阵"
// 参数<1>: 眼睛位置 MAKELONG(0x02, 0), 参数说明: "眼睛所有在位置向量"
// 参数<2>: 摄像机位置 MAKELONG(0x02, 0), 参数说明: "摄像机观察目标位置向量"
// 参数<3>: 上方向向量 MAKELONG(0x02, 0), 参数说明: "世界坐标系向上方向向量。通常用[0, 1, 0]向量"
OGRELIB_EXTERN_C void ogrelib_D3DXMatrixLookAtLH_315_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    void*    arg2 = pArgInf[2].m_pCompoundData;
    void*    arg3 = pArgInf[3].m_pCompoundData;

}

