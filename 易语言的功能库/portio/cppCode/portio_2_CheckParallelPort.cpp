#include "..\include_portio_header.h"

// 调用格式: SDT_INT []查询并口, 命令说明: "返回当前计算机上所有并口的配置情况。返回数据是一个整数数组，其中每三个成员描述一个并口的信息。第一个成员记录此并口的端口访问地址，第二个记录该并口的类型，为以下整数值之一： 0: 未定义;  1: SPP;  2: PS2;  3: EPP;  4: ECP 。如果并口类型为ECP，第三个成员记录具体的ECP模式，为以下整数值之一： 0: 未定义;  1: SPP;  2: PS2;  3: FastCentronics;  4: ECP;  5: EPP;  6: Test;  7: Configuration 。如果并口类型不为ECP，则第三个成员始终为0。注意如果没有安装port95nt.exe，本命令将失败并返回空整数数组。请在运行本命令的目的机器上下载安装第三方免费软件 http://www.dywt.com.cn/port95nt.exe "
// 无参数
PORTIO_EXTERN_C void portio_CheckParallelPort_2_portio(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}

