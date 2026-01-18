#include "..\include_opengl_header.h"

// 调用格式: SDT_BOOL 像素缩放, 命令说明: "设置接下来的像素操作的缩放比例。成功返回真，失败返回假"
// 参数<1>: 横向位置缩放 SDT_FLOAT, 参数说明: "大于1表示放大，小于1表示缩小，1表示不进行缩放，必须大于0"
// 参数<2>: 竖向位置缩放 SDT_FLOAT, 参数说明: "大于1表示放大，小于1表示缩小，1表示不进行缩放，必须大于0"
OPENGL_EXTERN_C void opengl_glPixelZoom_80_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    FLOAT    arg1 = pArgInf[0].m_float;
    FLOAT    arg2 = pArgInf[1].m_float;

}

