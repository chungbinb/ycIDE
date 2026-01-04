#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 画曲线, 命令说明: "先使用“设置曲线控制点”命令生成曲线，然后才能画出曲线。显示范围在“曲线起点”和“曲线终点”之间。必须启用“#曲线坐标”才有效，不使用的时候需要关闭。如果需要使用法向量，必须启用“#自动法向量”才有效，不使用的时候需要关闭。"
// 参数<1>: 画出模式 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t6912、#点，将画出的多边形显示为点；\r\n\t6913、#线，将画出的多边形显示为线段；\r\n"
// 参数<2>: 曲线起点 SDT_INT, 参数说明: "参数值应该介于 0 和“设置曲线等分点”命令中的“U向等分数”之间。"
// 参数<3>: 曲线终点 SDT_INT, 参数说明: "参数值应该介于 0 和“设置曲线等分点”命令中的“U向等分数”之间。"
OPENGL_EXTERN_C void opengl_glEvalMesh1_111_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;

}

