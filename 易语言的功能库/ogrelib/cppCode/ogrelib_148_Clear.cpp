#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).清除后台缓冲, 命令说明: "用指定的颜色清除后台缓冲。"
// 参数<1>: 清除标志 SDT_INT, 参数说明: "指定要清除的页面，可以为以下常量值之一或之和：1、#设备常量.渲染目标页面；2、#设备常量.深度缓冲区；4、#设备常量.复制缓冲页面。"
// 参数<2>: 渲染目标页面颜色 SDT_INT, 参数说明: "指定渲染目标页面颜色。"
// 参数<3>: [深度缓冲值 SDT_FLOAT], 参数说明: "深度缓冲值，参数取值范围为：0.0 - 1.0。0.0表示离观察者较近，1.0表示离观察者较远。"
// 参数<4>: [复制缓冲 SDT_INT], 参数说明: NULL
// 参数<5>: [矩形 MAKELONG(0x01, 0)], 参数说明: "指定要清除的矩形区域。"
OGRELIB_EXTERN_C void ogrelib_Clear_148_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    FLOAT    arg3 = pArgInf[3].m_float;
    INT      arg4 = pArgInf[4].m_int;
    void*    arg5 = pArgInf[5].m_pCompoundData;

}

