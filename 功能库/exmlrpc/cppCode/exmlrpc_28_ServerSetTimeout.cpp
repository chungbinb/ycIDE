#include "..\include_exmlrpc_header.h"

// 本命令被隐藏, 原始名字 = "置超时时间"
// 调用格式: SDT_BOOL (远程服务).置超时时间, 命令说明: "设置网络超时时间。不同的网络环境下的网络传输的情况是不同的，而且局域网和互联网的差别是相当大的，所以在不同的网络环境下要设置相应的超时时间。这里指的超时时间是指服务器和客户端一次交互的超时时间，一般的收发数据服务器要和客户端进行多次交互才能完成，每次最多能够收发10K左右的字节，每次交互都会检查是否超时。这个设置非常重要，如果设置的不合理，会影响服务器的性能，默认情况下服务器的超时时间为300毫秒"
// 无参数
EXMLRPC_EXTERN_C void exmlrpc_ServerSetTimeout_28_exmlrpc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}

