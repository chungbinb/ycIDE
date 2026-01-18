#include "..\include_portio_header.h"

// 调用格式: _SDT_NULL 置ECP模式, 命令说明: "如果指定并口类型为ECP，则可以使用本命令设置其具体的ECP模式。注意如果没有安装port95nt.exe，本命令将失败。请在运行本命令的目的机器上下载安装第三方免费软件 http://www.dywt.com.cn/port95nt.exe "
// 参数<1>: 并口端口号 SDT_INT, 参数说明: "本参数提供ECP类型并口的端口号"
// 参数<2>: ECP模式 SDT_INT, 参数说明: "本参数可以为以下值之一:  1: SPP;  2: PS2;  3: FastCentronics;  4: ECP;  5: EPP;  6: Test;  7: Configuration"
PORTIO_EXTERN_C void portio_SetECPMode_3_portio(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;

}

