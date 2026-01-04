#include "..\include_opengl_header.h"

// 调用格式: SDT_INT 生成列表号, 命令说明: "按次序自动生成一串连续的空列表号。返回其中的第一个列表号。失败返回-1"
// 参数<1>: 列表长度 SDT_INT, 参数说明: "必须是大于0的整数"
OPENGL_EXTERN_C void opengl_glGenLists_70_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

