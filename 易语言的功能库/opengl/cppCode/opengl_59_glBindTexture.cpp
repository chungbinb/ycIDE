#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 绑定纹理号, 命令说明: NULL
// 参数<1>: 纹理维数 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t3552、#一维纹理；\r\n\t3553、#二维纹理"
// 参数<2>: 纹理号 SDT_INT, 参数说明: NULL
OPENGL_EXTERN_C void opengl_glBindTexture_59_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;

}

