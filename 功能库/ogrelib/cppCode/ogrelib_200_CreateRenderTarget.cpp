#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).创建渲染目标页面, 命令说明: NULL
// 参数<1>: 页面宽度 SDT_INT, 参数说明: "渲染目标页面宽度（以像素为单位）。"
// 参数<2>: 页面高度 SDT_INT, 参数说明: "渲染目标页面高度（以像素为单位）。"
// 参数<3>: 格式 MAKELONG(0x35, 0), 参数说明: "渲染目标页面的格式。为“页面格式”常量值之一。"
// 参数<4>: 多重采样类型 MAKELONG(0x38, 0), 参数说明: "为“多重采样类型”常量类型中的任一值。"
// 参数<5>: 多重采样质量等级 SDT_INT, 参数说明: NULL
// 参数<6>: 可锁定 SDT_BOOL, 参数说明: "0、渲染目标页面不可锁定；1、渲染目标页面可锁定。"
// 参数<7>: 页面 MAKELONG(0x08, 0), 参数说明: "要创建的渲染目标页面。"
OGRELIB_EXTERN_C void ogrelib_CreateRenderTarget_200_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    void*    arg3 = pArgInf[3].m_pCompoundData;
    void*    arg4 = pArgInf[4].m_pCompoundData;
    INT      arg5 = pArgInf[5].m_int;
    BOOL     arg6 = pArgInf[6].m_bool;
    void*    arg7 = pArgInf[7].m_pCompoundData;

}

