#include "..\include_einterprocess_header.h"

// 调用格式: SDT_INT 写命名管道, 命令说明: "向命名管道中写入数据，返回实际写入到命名管道中的字节数"
// 参数<1>: 命名管道句柄 SDT_INT, 参数说明: "欲写入的命名管道句柄"
// 参数<2>: 数据 SDT_BIN, 参数说明: "欲向命名管道中写入的数据"
EINTERPROCESS_EXTERN_C void einterprocess_WriteNamedPipe_4_einterprocess(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPBYTE   arg2 = pArgInf[1].m_pBin;

}

