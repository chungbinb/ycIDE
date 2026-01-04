#include "..\include_wnet_header.h"

// 调用格式: SDT_INT (局域网操作).取所有主机名, 命令说明: "取出当前局域网中所有的主机名称。返回取得的主机个数"
// 参数<1>: &取得的主机名 数组 SDT_TEXT, 参数说明: "命令执行完毕后本变量数组中依次存放取得的主机名"
WNET_EXTERN_C void wnet_GetServerName_3_wnet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[1].m_ppAryData;

}

