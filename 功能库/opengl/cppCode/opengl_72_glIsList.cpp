#include "..\include_opengl_header.h"

// 调用格式: SDT_BOOL 是否存在列表, 命令说明: "如果该列表号存在已经编译的列表就返回真，否则返回假。"
// 参数<1>: 列表号 SDT_INT, 参数说明: "可以直接指定列表号或者使用由“生成列表号”取得的返回值，必须是大于0的整数。"
OPENGL_EXTERN_C void opengl_glIsList_72_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

