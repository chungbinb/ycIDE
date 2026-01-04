#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).创建顶点着色器声明, 命令说明: "从设备和顶点元素创建顶点着色器声明。"
// 参数<1>: 顶点成员 MAKELONG(0x16, 0), 参数说明: NULL
// 参数<2>: 顶点着色器声明 MAKELONG(0x17, 0), 参数说明: "要创建的顶点着色器声明。"
OGRELIB_EXTERN_C void ogrelib_CreateVertexDeclaration_201_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

