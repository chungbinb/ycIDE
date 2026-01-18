#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (模型).画子模型, 命令说明: "根据属性表标识符画本模型的子模型。"
// 参数<1>: 标识符 SDT_INT, 参数说明: "属性表标识符，用于标识哪个面被画。"
OGRELIB_EXTERN_C void ogrelib_DrawSubset_360_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

