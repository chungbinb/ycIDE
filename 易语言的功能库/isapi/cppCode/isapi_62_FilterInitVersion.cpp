#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网筛选器).初始化版本信息, 命令说明: "在筛选器DLL的公开导出函数 GetFilterVersion() 中必须调用本方法"
// 参数<1>: 版本信息 SDT_INT, 参数说明: "必须是 GetFilterVersion() 函数的参数"
// 参数<2>: 筛选器标记 SDT_INT, 参数说明: "此标记用于指定本筛选器欲处理的事件类型、端口、优先级等。仅当相应条件（事件/端口）满足时IIS服务器才会调用 HttpFilterProc() 函数。参数值可以是枚举类型常量 #筛选器事件 中的一个或多个，可以是 #筛选器端口 中的一个，也可以是 #筛选器优先级 中的一个，或其数值之和。例如：#筛选器事件.预处理头 + #筛选器事件.结束请求 + #筛选器优先级.默认 + #筛选器端口.非安全端口"
// 参数<3>: 筛选器描述 SDT_TEXT, 参数说明: "可以是任意文本，但长度不得超过256字节（128个汉字）。此文本可被IIS服务器获取并显示。"
ISAPI_EXTERN_C void isapi_FilterInitVersion_62_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    LPSTR    arg3 = pArgInf[3].m_pText;

}

