#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 画曲面, 命令说明: "先使用“设置曲面控制点”命令生成曲面，然后才能画出曲面。U向显示范围在“曲面U向起点”和“曲面U向起点”之间。V向显示范围在“曲面V向起点”和“曲面V向起点”之间。必须启用“#曲面坐标”才有效，不使用的时候需要关闭。如果需要使用法向量，必须启用“#自动法向量”才有效，不使用的时候需要关闭。"
// 参数<1>: 画出模式 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t6912、#点，将画出的多边形显示为点；\r\n\t6913、#线，将画出的多边形显示为线段；\r\n\t6914、#面，将画出的多边形显示为填充颜色的多边形"
// 参数<2>: 曲面U向起点 SDT_INT, 参数说明: "参数值应该介于 0 和“设置曲面等分网格”命令中的“U向等分数”之间。"
// 参数<3>: 曲面U向终点 SDT_INT, 参数说明: "参数值应该介于 0 和“设置曲面等分网格”命令中的“U向等分数”之间。"
// 参数<4>: 曲面V向起点 SDT_INT, 参数说明: "参数值应该介于 0 和“设置曲面等分网格”命令中的“V向等分数”之间。"
// 参数<5>: 曲面V向终点 SDT_INT, 参数说明: "参数值应该介于 0 和“设置曲面等分网格”命令中的“V向等分数”之间。"
OPENGL_EXTERN_C void opengl_glEvalMesh2_115_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;
    INT      arg4 = pArgInf[3].m_int;
    INT      arg5 = pArgInf[4].m_int;

}

