#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (表).删除键值对, 命令说明: "删除对象中指定的键及其值。成功返回真，失败返回假。"
// 参数<1>: 键 SDT_BIN, 参数说明: NULL
EDATASTRUCTURE_EXTERN_C void edatastructure_DelKeyValue_95_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[1].m_pBin;

}

