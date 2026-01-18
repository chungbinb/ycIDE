#include "..\include_iconv_header.h"

// 调用格式: SDT_BIN 编码转换_转换, 命令说明: "对指定数据进行编码转换，返回转换后的数据。对同一个“转换句柄”，可多次执行本命令，直到该句柄被关闭。如果执行失败，将返回空字节集，同时设置参数“执行结果”为“假”。"
// 参数<1>: 转换句柄 SDT_INT, 参数说明: "必须是“编码转换_打开()”命令所返回的句柄。"
// 参数<2>: 被转换数据 SDT_BIN, 参数说明: NULL
// 参数<3>: [&执行结果 SDT_BOOL], 参数说明: "如果提供本参数，其中将被写入本命令的执行结果——执行成功时为真，执行失败时为假。"
ICONV_EXTERN_C void iconv_iconv_iconv_2_iconv(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPBYTE   arg2 = pArgInf[1].m_pBin;
    BOOL*    arg3 = pArgInf[2].m_pBool;

}

