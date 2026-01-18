#include "..\include_isapi_header.h"

// 调用格式: SDT_TEXT (互联网筛选器).URL映射信息, 命令说明: "获取或修改URL映射信息，返回URL映射到的服务器文件名。仅当“取事件类型()”返回值为 #筛选器事件.映射URL 的情况下才能调用本方法。"
// 参数<1>: URL SDT_TEXT, 参数说明: NULL
// 参数<2>: [服务器文件 SDT_TEXT], 参数说明: "如果提供了此参数，表示修改URL映射结果，要求参数是服务器中的一个完整路径文件名。如果此参数被省略，表示不修改URL映射结果。"
ISAPI_EXTERN_C void isapi_FilterURLMap_79_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;

}

