#include "..\include_wnet_header.h"

// 调用格式: SDT_INT (局域网操作).取共享资源, 命令说明: "取出当前局域网中指定类型的共享资源（包括共享目录和共享文件夹）的路径。返回取得的资源个数"
// 参数<1>: 欲取的资源类型 MAKELONG(0x02, 0), 参数说明: "本参数指定欲取出的资源的类型。该参数可以为“资源类型”枚举常量集合中的一个常量值"
// 参数<2>: &取得的资源 数组 SDT_TEXT, 参数说明: "命令执行完毕后本变量数组中依次存放取得的资源的路径"
// 参数<3>: [&取得的资源类型 数组 SDT_INT], 参数说明: "命令执行完毕后本变量数组中依次存放取得的资源的类型，并与取得的资源路径一一对应。该数组成员值可以为以下常量之一：“#资源类型.共享文件夹”或“#资源类型.共享打印机”"
WNET_EXTERN_C void wnet_GetSharedRes_2_wnet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    void**   arg2 = pArgInf[2].m_ppAryData;
    void**   arg3 = pArgInf[3].m_ppAryData;

}

