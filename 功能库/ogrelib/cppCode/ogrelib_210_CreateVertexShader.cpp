#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).创建顶点着色器, 命令说明: "创建一个顶点着色器。"
// 参数<1>: 顶点着色器文件 SDT_TEXT, 参数说明: "指定顶点着色器文件。"
// 参数<2>: 顶点着色器 MAKELONG(0x15, 0), 参数说明: "要创建的顶点着色器。"
OGRELIB_EXTERN_C void ogrelib_CreateVertexShader_210_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

