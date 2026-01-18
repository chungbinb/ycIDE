#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网筛选器).取读写数据, 命令说明: "仅当“取事件类型()”返回值为 #筛选器事件.读取数据 或 #筛选器事件.发送数据 的情况下才能调用此方法。执行成功返回真，否则返回假"
// 参数<1>: [&数据缓冲区地址 SDT_INT], 参数说明: NULL
// 参数<2>: [&数据长度 SDT_INT], 参数说明: NULL
// 参数<3>: [&缓冲区长度 SDT_INT], 参数说明: NULL
ISAPI_EXTERN_C void isapi_FilterGetRawData_72_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PINT     arg1 = pArgInf[1].m_pInt;
    PINT     arg2 = pArgInf[2].m_pInt;
    PINT     arg3 = pArgInf[3].m_pInt;

}

