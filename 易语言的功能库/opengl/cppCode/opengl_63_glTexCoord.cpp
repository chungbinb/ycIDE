#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置纹理映射坐标, 命令说明: NULL
// 参数<1>: 横向位置 SDT_DOUBLE, 参数说明: "纹理图片的左边为0，右边为1，超出[0，1]范围的值，纹理图片将被平移相应的距离。"
// 参数<2>: 竖向位置 SDT_DOUBLE, 参数说明: "纹理图片的底边为0，顶边为1，超出[0，1]范围的值，纹理图片将被平移相应的距离。如果为空，表示定义的是一维纹理的坐标"
OPENGL_EXTERN_C void opengl_glTexCoord_63_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[0].m_double;
    double   arg2 = pArgInf[1].m_double;

}

