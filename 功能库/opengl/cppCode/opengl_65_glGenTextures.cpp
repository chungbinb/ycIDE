#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 生成纹理号, 命令说明: NULL
// 参数<1>: 纹理数 SDT_INT, 参数说明: "需要生成纹理号"
// 参数<2>: &存放纹理号的变量 数组/非数组 SDT_INT, 参数说明: "用来存放纹理号的变量，如果提供的变量为数组，那么纹理号将被依次存放在数组中。超出数组成员数的部分将无法生成。"
OPENGL_EXTERN_C void opengl_glGenTextures_65_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    void**   arg2 = pArgInf[1].m_ppAryData;

}

