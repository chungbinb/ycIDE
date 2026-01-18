#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).创建像素着色器, 命令说明: "创建一个像素着色器。"
// 参数<1>: 像素着色器文件 SDT_TEXT, 参数说明: "指定顶点着色器文件。"
// 参数<2>: 像素着色器 MAKELONG(0x27, 0), 参数说明: "要创建的像素着色器。"
OGRELIB_EXTERN_C void ogrelib_CreatePixelShader_215_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

