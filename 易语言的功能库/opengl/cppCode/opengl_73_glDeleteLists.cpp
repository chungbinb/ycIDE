#include "..\include_opengl_header.h"

// 调用格式: SDT_BOOL 删除列表列, 命令说明: "删除一列连续的列表。成功返回真，失败返回假"
// 参数<1>: 起始列表号 SDT_INT, 参数说明: "必须是大于0的整数"
// 参数<2>: 列表数 SDT_INT, 参数说明: "必须是大于0的整数"
OPENGL_EXTERN_C void opengl_glDeleteLists_73_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;

}

