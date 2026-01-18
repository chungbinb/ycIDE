#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置正面, 命令说明: "为填充颜色的多边形指定正面的定义方向。当四个手指并拢依次绕着“设置点”命令所定义点的方向握紧时，大拇指所指的方向就是正面的方向。反面即为背面。必须启用“#精选面”才有效。默认为#逆时针。"
// 参数<1>: 模式 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t2304、#顺时针；\r\n\t2305、#逆时针"
OPENGL_EXTERN_C void opengl_glFrontFace_25_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

