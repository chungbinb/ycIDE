#include "..\include_einterprocess_header.h"

// 调用格式: SDT_BOOL (邮槽服务器).读数据, 命令说明: "只能在服务器端从邮槽中取出数据。读取成功返回真，否则返回假"
// 参数<1>: &数据 SDT_BIN, 参数说明: "从向邮槽中读出的数据"
EINTERPROCESS_EXTERN_C void einterprocess_rDataFromServer_10_einterprocess(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE*  arg1 = pArgInf[1].m_ppBin;

}

