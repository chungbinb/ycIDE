#include "..\include_ogrelib_header.h"

// 调用格式: SDT_INT (索引缓冲).置优先级, 命令说明: "设置本资源的优先级。关于优先级请看“取优先级”方法说明。"
// 参数<1>: 优先级 SDT_INT, 参数说明: "新的优先级。"
OGRELIB_EXTERN_C void ogrelib_SetPriority_57_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

