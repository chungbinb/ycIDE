#include "..\include_downlib_header.h"

// 调用格式: SDT_BOOL 网络通讯设置, 命令说明: "初始化下载支持库，进行基本设置."
// 参数<1>: 写入文件大小 SDT_INT, 参数说明: "本参数指定每次写入文件的指定大小，默认为20480, 注意:如果待下载文件的总长度小于此值，那么将不能使用多个线程下载"
// 参数<2>: 重连时间 SDT_INT, 参数说明: "出错时连接服务器间隔时间，默认为0"
// 参数<3>: 重连次数 SDT_INT, 参数说明: "出错时重复连接服务器次数，默认为0"
DOWNLIB_EXTERN_C void downlib_InitDownLib_0_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;

}

