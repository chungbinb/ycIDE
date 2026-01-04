#include "..\include_opengl_header.h"

// 调用格式: SDT_BOOL 设置线宽, 命令说明: "为所画的线段设置线条宽度。如果操作系统支持指定的宽度大小，返回真，否则返回假。通过启用和停用“光滑线”功能，来控制线段的放样效果。"
// 参数<1>: 宽度 SDT_FLOAT, 参数说明: "以像素为单位的正数，负数自动取正，零还原为1。默认为1个像素大小。目前WINDOWS支持的宽度大小在0.5和10.0之间，间隔为0.125，最后使用的实际宽度为最接近这个值的宽度大小。"
OPENGL_EXTERN_C void opengl_glLineWidth_106_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    FLOAT    arg1 = pArgInf[0].m_float;

}

