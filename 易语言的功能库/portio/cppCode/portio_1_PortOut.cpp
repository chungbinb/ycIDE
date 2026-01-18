#include "..\include_portio_header.h"

// 调用格式: _SDT_NULL 端口写出, 命令说明: "向指定端口写出一个字节数据，注意如果没有安装port95nt.exe，本命令将失败。请在运行本命令的目的机器上下载安装第三方免费软件 http://www.dywt.com.cn/port95nt.exe "
// 参数<1>: 端口号 SDT_INT, 参数说明: "本参数提供欲进行读写端口的端口号"
// 参数<2>: 欲写出字节 SDT_BYTE, 参数说明: "本参数提供欲写出到指定端口的字节数据"
PORTIO_EXTERN_C void portio_PortOut_1_portio(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    BYTE     arg2 = pArgInf[1].m_byte;

}

