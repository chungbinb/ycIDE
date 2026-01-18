#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网扩展).写出字节集, 命令说明: NULL
// 参数<1>: 数据 SDT_BIN, 参数说明: NULL
ISAPI_EXTERN_C void isapi_WriteBinary_29_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[1].m_pBin;

}

