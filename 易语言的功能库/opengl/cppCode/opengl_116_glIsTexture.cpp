#include "..\include_opengl_header.h"

// 调用格式: SDT_BOOL 是否存在纹理号, 命令说明: "如果该纹理号已经存在就返回真，否则返回假。"
// 参数<1>: 纹理 SDT_INT, 参数说明: "需要进行判断的纹理号。"
OPENGL_EXTERN_C void opengl_glIsTexture_116_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

