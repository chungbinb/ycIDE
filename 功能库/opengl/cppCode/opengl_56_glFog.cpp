#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置雾, 命令说明: "必须启用“#雾”后才有效。"
// 参数<1>: 参数名称 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t2913、#雾索引，参数值为非数组数值；\r\n\t2914、#雾密度，参数值为范围在[0，+∞)内的非数组数值；\r\n\t2915、#雾起点，参数值为非数组数值；\r\n\t2916、#雾终点，参数值为非数组数值；\r\n\t2917、#雾模式，参数值可以为以下常量：\r\n\t\t2048、#雾模式1；\r\n\t\t2049、#雾模式2；\r\n\t\t9729、#线性\r\n\t2918、#雾颜色，参数值为范围在[0，1]内的四维数组，多余部分无效"
// 参数<2>: 参数值 数组/非数组 SDT_FLOAT, 参数说明: NULL
OPENGL_EXTERN_C void opengl_glFog_56_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    void*    arg2 = pArgInf[1].m_pAryData;

}

