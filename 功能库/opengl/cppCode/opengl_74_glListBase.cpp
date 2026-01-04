#include "..\include_opengl_header.h"

// 调用格式: SDT_BOOL 设置偏移列表数, 命令说明: "为“调用多显示列表”设定一个起始列表号。成功返回真，失败返回假"
// 参数<1>: 偏移列表数 SDT_INT, 参数说明: "必须是大于0的整数"
OPENGL_EXTERN_C void opengl_glListBase_74_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

