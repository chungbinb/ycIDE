#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网筛选器).取拒绝访问信息, 命令说明: "获取拒绝访问信息。仅当“取事件类型()”返回值为 #筛选器事件.拒绝访问 的情况下才能调用本方法。"
// 参数<1>: [&URL SDT_TEXT], 参数说明: NULL
// 参数<2>: [&服务器文件 SDT_TEXT], 参数说明: NULL
// 参数<3>: [&拒绝理由 SDT_INT], 参数说明: "应为枚举类型常量 #筛选器拒绝理由.xxx 成员之一"
ISAPI_EXTERN_C void isapi_FilterGetAccessDenied_82_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR*   arg1 = pArgInf[1].m_ppText;
    LPSTR*   arg2 = pArgInf[2].m_ppText;
    PINT     arg3 = pArgInf[3].m_pInt;

}

