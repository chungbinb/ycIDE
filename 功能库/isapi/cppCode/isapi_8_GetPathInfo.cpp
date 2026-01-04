#include "..\include_isapi_header.h"

// 调用格式: SDT_TEXT (互联网扩展).获取路径信息, 命令说明: "获取路径信息。如客户端传入 xxx.dll/111.txt。0: 相对路径为 /111.txt，1: 绝对路径为 c:\\wwwroot\\interput\\111.txt, c:\\wwwroot\\interput为服务器所在目录"
// 参数<1>: 路径类型 SDT_INT, 参数说明: "路径类型: 0为相对路径，1为绝对路径"
ISAPI_EXTERN_C void isapi_GetPathInfo_8_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

