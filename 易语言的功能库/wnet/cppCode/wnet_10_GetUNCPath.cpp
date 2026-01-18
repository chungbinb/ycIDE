#include "..\include_wnet_header.h"

// 调用格式: SDT_BOOL (局域网操作).取资源路径, 命令说明: "取得指定的网络驱动器的路径所对应的局域网资源路径、连接名称和相对路径。比如将“\\\\Prog5\\共享测试”映射到了驱动器“X:”，那么“X:\\易语言教程.doc”所对应的资源路径为“\\\\Prog5\\共享测试\\易语言教程.doc”，连接名称为“\\\\Prog5\\共享测试”，相对路径为“\\易语言教程.doc”"
// 参数<1>: 网络驱动器路径 SDT_TEXT, 参数说明: NULL
// 参数<2>: &取得的资源路径 SDT_TEXT, 参数说明: NULL
// 参数<3>: [&取得的连接名称 SDT_TEXT], 参数说明: NULL
// 参数<4>: [&取得的相对路径 SDT_TEXT], 参数说明: NULL
WNET_EXTERN_C void wnet_GetUNCPath_10_wnet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR*   arg2 = pArgInf[2].m_ppText;
    LPSTR*   arg3 = pArgInf[3].m_ppText;
    LPSTR*   arg4 = pArgInf[4].m_ppText;

}

