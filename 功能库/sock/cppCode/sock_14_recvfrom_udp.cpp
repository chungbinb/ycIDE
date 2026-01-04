#include "..\include_sock_header.h"

// 调用格式: SDT_BIN (网络数据报).接收, 命令说明: "接收来自网络数据报的数据，每次接受的最大长度为20480个字节。如果出错，将向第三个参数写入假，并返回空字节集。"
// 参数<1>: 等待时间 SDT_INT, 参数说明: "等待连接的时间范围 0 无限等待，-1 不等待 其它 具体等待时间（毫秒）"
// 参数<2>: &对方信息 MAKELONG(0x04, 0), 参数说明: "获得对方的IP及端口信息"
// 参数<3>: [&是否成功 SDT_BOOL], 参数说明: "如果提供本参数，在接收数据成功的情况下，其值将被改写为真；在接收数据失败的情况下，其值将被改写为假。"
SOCK_EXTERN_C void sock_recvfrom_udp_14_sock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void**   arg2 = pArgInf[2].m_ppCompoundData;
    BOOL*    arg3 = pArgInf[3].m_pBool;

}

