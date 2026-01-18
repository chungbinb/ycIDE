#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 生成纹理坐标, 命令说明: "需要生成纹理号"
// 参数<1>: 坐标量 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t8192、#S分量； \r\n\t8193、#T分量；\r\n"
// 参数<2>: 映射值 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t9472、#纹理生成模式，坐标值可以为以下常量：\r\n\t\t9216、#观测线性插值； \r\n\t\t9217、#模型线性插值； \r\n\t\t9218、#纹理贴图形状；\r\n\t9473、#模型平面，四维数组，为生成纹理坐标的方程指定四个相应的参数，多余部分无效；\r\n\t9474、#观测平面，四维数组，为生成纹理坐标的方程指定四个相应的参数，多余部分无效；"
// 参数<3>: 坐标值 数组/非数组 SDT_DOUBLE, 参数说明: NULL
OPENGL_EXTERN_C void opengl_glTexGen_66_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    void*    arg3 = pArgInf[2].m_pAryData;

}

