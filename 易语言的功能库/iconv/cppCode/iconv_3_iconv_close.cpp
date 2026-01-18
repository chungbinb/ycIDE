#include "..\include_iconv_header.h"

// 调用格式: SDT_BOOL 编码转换_关闭, 命令说明: "关闭由“编码转换_打开()”命令所返回的转换句柄，释放相关资源。"
// 参数<1>: 转换句柄 SDT_INT, 参数说明: "必须是“编码转换_打开()”命令所返回的句柄。"
ICONV_EXTERN_C void iconv_iconv_close_3_iconv(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

