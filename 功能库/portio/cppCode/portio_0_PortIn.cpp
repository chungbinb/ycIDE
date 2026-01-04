#include "..\include_portio_header.h"

// 调用格式: SDT_BYTE 端口读入, 命令说明: "从指定端口读入并返回一个字节数据，注意如果没有安装port95nt.exe，本命令将失败并始终返回0。请在运行本命令的目的机器上下载安装第三方免费软件 http://www.dywt.com.cn/port95nt.exe "
// 参数<1>: 端口号 SDT_INT, 参数说明: "本参数提供欲进行读写端口的端口号"
PORTIO_EXTERN_C void portio_PortIn_0_portio(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

