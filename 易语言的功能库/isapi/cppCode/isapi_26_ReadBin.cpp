#include "..\include_isapi_header.h"

// 调用格式: SDT_BIN (互联网扩展).读取字节集, 命令说明: "读取客户端用投递(POST)方法传入的内容，并以字节集的方式返回。"
// 参数<1>: 大小 SDT_INT, 参数说明: "本参数表示期望返回字节集的最大长度。如果实际数据比本长度大，就按本长度返回；如果实际数据比本长度小，就按实际长度返回。"
ISAPI_EXTERN_C void isapi_ReadBin_26_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

