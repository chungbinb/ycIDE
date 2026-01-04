#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x04, 0) (变换矩阵).建平面反射矩阵, 命令说明: "从平面方程创建一个平面反射矩阵"
// 参数<1>: 平面 MAKELONG(0x05, 0), 参数说明: "指定平面"
OGRELIB_EXTERN_C void ogrelib_D3DXMatrixReflect_321_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

