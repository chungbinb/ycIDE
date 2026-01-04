#include "..\include_wnet_header.h"

// 调用格式: SDT_INT64 (局域网操作).取连接速度, 命令说明: "取出与局域网内指定的共享资源（共享目录或共享打印机等）之间的连接速度。以“100比特/秒（bit/s）”为单位。如果该命令执行出错，返回-1；如果无法取出连接速度，返回0。注意，该命令取出的速度为大概值，可能有误差，仅供参考"
// 参数<1>: 欲测试的资源路径 SDT_TEXT, 参数说明: "可以为局域网中的共享文件夹路径，比如“\\\\Prog5\\共享测试”，也可以为映射到本地的网络驱动器名称，比如“Y:”"
WNET_EXTERN_C void wnet_GetConnectionSpeed_4_wnet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

