#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置材质, 命令说明: NULL
// 参数<1>: 应用面 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n   1028、#正面； 1029、#背面； 1030、#正面和背面；\r\n"
// 参数<2>: 参数名称 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t4608、#环境光，参数值为范围在[0，1]内的表示红绿蓝混合通道颜色的四维数组，多余部分无效；\r\n\t4609、#漫反射光，参数值为范围在[0，1]内的表示红绿蓝混合通道颜色的四维数组，多余部分无效；\r\n\t4610、#镜面光，参数值为范围在[0，1]内的表示红绿蓝混合通道颜色的四维数组，多余部分无效；\r\n\t5632、#散射光，参数值为范围在[0，1]内的表示红绿蓝混合通道颜色的四维数组，多余部分无效；\r\n\t5633、#亮光，参数值为范围在[0，128]内的非数组数值；\r\n\t5634、#环境光和漫反射光，参数值为范围在[0，1]内的表示红绿蓝混合通道颜色的四维数组，多余部分无效；\r\n\t5635、#颜色索引，参数值为非数组数值"
// 参数<3>: 参数值 数组/非数组 SDT_FLOAT, 参数说明: NULL
OPENGL_EXTERN_C void opengl_glMaterial_54_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    void*    arg3 = pArgInf[2].m_pAryData;

}

