#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x04, 0) (变换矩阵).建仿射变换矩阵, 命令说明: "创建一个仿射变换变换矩阵。本函数是用下面的公式来计算一个仿射变换矩阵：Mout = Ms * (Mrc)-1 * Mr * Mrc * Mt，其中:Mout = 输出矩阵，Ms = 缩放矩阵，Mrc = 旋转矩阵中心，Mr = 旋转矩阵，Mt = 平移矩阵"
// 参数<1>: 缩放因子 SDT_FLOAT, 参数说明: NULL
// 参数<2>: [旋转中心向量 MAKELONG(0x02, 0)], 参数说明: "如果该项为空，就用一个单位矩阵代替Mrc"
// 参数<3>: [旋转矩阵 MAKELONG(0x06, 0)], 参数说明: "如果该项为空，就用单位矩阵Mr代替"
// 参数<4>: [平移向量 MAKELONG(0x02, 0)], 参数说明: "如果该项为空，就用单位矩阵Mt代替"
OGRELIB_EXTERN_C void ogrelib_D3DXMatrixAffineTransformation_310_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    FLOAT    arg1 = pArgInf[1].m_float;
    void*    arg2 = pArgInf[2].m_pCompoundData;
    void*    arg3 = pArgInf[3].m_pCompoundData;
    void*    arg4 = pArgInf[4].m_pCompoundData;

}

