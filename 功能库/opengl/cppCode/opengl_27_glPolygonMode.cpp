#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置多边形模式, 命令说明: "当绘出图形为封闭的多边形时才起作用。默认设置正面和背面都为面。"
// 参数<1>: 应用面 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t1028、#正面；\r\n\t1029、#背面；\r\n\t1032、#正面和背面"
// 参数<2>: 模式 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t6912、#点，将画出的多边形显示为点；\r\n\t6913、#线，将画出的多边形显示为线段；\r\n\t6914、#面，将画出的多边形显示为填充颜色的多边形"
OPENGL_EXTERN_C void opengl_glPolygonMode_27_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;

}

