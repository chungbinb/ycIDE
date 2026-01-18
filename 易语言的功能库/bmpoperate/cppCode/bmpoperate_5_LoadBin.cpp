#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_BOOL (位图).载入数据, 命令说明: "从字节集或资源载入位图数据，成功返回真，失败返回假。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。"
// 参数<1>: 位图文件数据 SDT_BIN, 参数说明: NULL
BMPOPERATE_EXTERN_C void bmpoperate_LoadBin_5_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[1].m_pBin;

}

