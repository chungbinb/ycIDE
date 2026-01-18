#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 取当前鼠标处颜色值, 命令说明: "取当前鼠标所在位置的RGB值。"
// 参数<1>: &R SDT_INT, 参数说明: "要取得的R值"
// 参数<2>: &G SDT_INT, 参数说明: "要取得的G值"
// 参数<3>: &B SDT_INT, 参数说明: "要取得的B值"
EAPI_EXTERN_C void eapi_GetPointRGB_71_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PINT     arg1 = pArgInf[0].m_pInt;
    PINT     arg2 = pArgInf[1].m_pInt;
    PINT     arg3 = pArgInf[2].m_pInt;

}

