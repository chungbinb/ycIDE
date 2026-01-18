#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 设置屏幕分辨率, 命令说明: NULL
// 参数<1>: 宽度 SDT_INT, 参数说明: "屏幕显示区域的宽度，单位为像素点。"
// 参数<2>: 高度 SDT_INT, 参数说明: "屏幕显示区域的高度，单位为像素点。"
// 参数<3>: [位数 SDT_INT], 参数说明: "每像素的位数。"
// 参数<4>: [刷新频率 SDT_INT], 参数说明: "屏幕的刷新频率。"
EAPI_EXTERN_C void eapi_SetResolveRatio_69_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;
    INT      arg4 = pArgInf[3].m_int;

}

