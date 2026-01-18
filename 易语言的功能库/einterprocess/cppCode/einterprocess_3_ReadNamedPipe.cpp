#include "..\include_einterprocess_header.h"

// 调用格式: SDT_BOOL 读命名管道, 命令说明: "从命名管道中读取数据。成功返回真，失败返回假"
// 参数<1>: 命名管道句柄 SDT_INT, 参数说明: "欲读取的命名管道句柄"
// 参数<2>: &数据 SDT_BIN, 参数说明: "从命名管道中读取出的数据"
EINTERPROCESS_EXTERN_C void einterprocess_ReadNamedPipe_3_einterprocess(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPBYTE*  arg2 = pArgInf[1].m_ppBin;

}

