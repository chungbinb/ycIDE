#include "..\include_einterprocess_header.h"

// 调用格式: SDT_BOOL (邮槽客户机).写数据, 命令说明: "只能在客户机端向邮槽中写入数据。注意写入的数据长度有限制，最大的长度为424字节，如果通讯数据的长度太大，最好选用“命名管道”。写入成功返回真，否则返回假"
// 参数<1>: 数据 SDT_BIN, 参数说明: "欲向邮槽中写入的数据，数据长度必须小于424字节，否则出错"
EINTERPROCESS_EXTERN_C void einterprocess_wDataToClient_15_einterprocess(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[1].m_pBin;

}

