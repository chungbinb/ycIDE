#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网筛选器).向客户端写数据, 命令说明: NULL
// 参数<1>: 欲写出的数据 SDT_BIN, 参数说明: NULL
ISAPI_EXTERN_C void isapi_FilterWriteClient_69_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[1].m_pBin;

}

