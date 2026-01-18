#include "..\include_dp1_header.h"

// 调用格式: SDT_TEXT 取数据摘要, 命令说明: "返回一段字节集数据的MD5数据摘要编码文本。不同数据的MD5码都不一样，因此本命令可以用作保证重要数据不会被篡改"
// 参数<1>: 字节集数据 SDT_BIN, 参数说明: "为命令提供所需的字节集数据"
DP1_EXTERN_C void dp1_GetMD5_2_dp1(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[0].m_pBin;

}

