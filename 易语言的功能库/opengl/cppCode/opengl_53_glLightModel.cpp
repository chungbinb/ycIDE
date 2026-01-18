#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置光照模式, 命令说明: NULL
// 参数<1>: 参数名称 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t2897、#光源观察点模式，顶点的高光强度不仅取决于顶点法向量，而且取决于顶点到光源的方向以及顶点到视点的方向.1表示设置为局部视点,需要计算视点到每个顶点的方向,会影响运行速度。0表示设置为无穷远视点,不论观察坐标如何改变，光的反射方向始终与纵向轴平行。默认为0；\r\n\t2898、#双面光模式，如果参数值为0表示使用单面光，只有正面接受光照效果。否则正面和背面都接受光照效果，背面的法向为正面法向的反方向。默认为0；\r\n\t2899、#全局光模式，参数值为范围在[0，1]内的表示红绿蓝混合通道颜色的四维数组，默认为（0.2, 0.2, 0.2, 1.0），多余部分无效；\r\n"
// 参数<2>: 参数值 数组/非数组 SDT_FLOAT, 参数说明: NULL
OPENGL_EXTERN_C void opengl_glLightModel_53_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    void*    arg2 = pArgInf[1].m_pAryData;

}

