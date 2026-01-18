#include "..\include_dp1_header.h"

// 调用格式: SDT_BIN 解压数据, 命令说明: "解压一个内含被压缩数据的字节集，返回被解压后的结果字节集。如果压缩失败，返回空字节集。注意解压后的数据也有可能是空字节集"
// 参数<1>: 字节集数据 SDT_BIN, 参数说明: "为命令提供所需的字节集数据"
DP1_EXTERN_C void dp1_Uncompress_1_dp1(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[0].m_pBin;

}

