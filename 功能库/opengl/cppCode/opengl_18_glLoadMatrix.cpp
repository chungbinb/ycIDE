#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 载入矩阵, 命令说明: "把当前坐标模式变换成指定的矩阵。"
// 参数<1>: 指定矩阵 数组 SDT_DOUBLE, 参数说明: "矩阵为一个拥有16个成员的数组，多余部分无效。"
OPENGL_EXTERN_C void opengl_glLoadMatrix_18_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pAryData;

}

