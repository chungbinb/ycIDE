#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置画面质量, 命令说明: "在权衡运行速度和画面质量之后，为显示效果的质量和画面的细节处理设置等级。"
// 参数<1>: 目标 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t3152、#透视修正质量，控制颜色和纹理的显示效果；\r\n\t3153、#点质量，控制点的显示质量；\r\n\t3154、#线质量，控制线的显示质量；\r\n\t3155、#多边形质量，控制多边形的显示质量；\r\n\t3156、#雾质量，控制雾的显示质量；\r\n"
// 参数<2>: 效果 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t4352、#不考虑；不做特殊要求，让OPenGL自己选择合适的方式进行显示；\r\n\t4353、#快速；使用最快的显示速度，但是显示效果会有所下降；\r\n\t4354、#最佳；使用最好的显示效果，但是显示速度会有所下降。当“目标”值为#雾质量时不能使用"
OPENGL_EXTERN_C void opengl_glHint_32_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;

}

