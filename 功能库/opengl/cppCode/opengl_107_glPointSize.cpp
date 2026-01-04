#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置点大小, 命令说明: "为所画的点设置点的大小。通过启用和停用“光滑点”功能，来控制点的放样效果。"
// 参数<1>: 大小 SDT_FLOAT, 参数说明: "以像素为单位的正数，负数自动取正，零还原为1。默认为1个像素大小。"
OPENGL_EXTERN_C void opengl_glPointSize_107_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    FLOAT    arg1 = pArgInf[0].m_float;

}

