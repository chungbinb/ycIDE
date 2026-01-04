#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 选择矩阵模式, 命令说明: "选择接下来将要进行操作的坐标矩阵。"
// 参数<1>: 矩阵模式 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t5888、#观察矩阵，用来建模的坐标矩阵；\r\n\t5889、#投影矩阵，用来进行投影变换的坐标矩阵；\r\n\t5890、#纹理矩阵，用来改变纹理坐标的坐标矩阵。"
OPENGL_EXTERN_C void opengl_glMatrixMode_9_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

