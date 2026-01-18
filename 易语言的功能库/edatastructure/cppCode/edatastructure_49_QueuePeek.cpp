#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (队列).取队列首节点, 命令说明: "取队列的首节点，与“弹出”方法不同的是对列的首节点并不弹出，仍保留在对列中。该方法执行成功返回真，执行失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息"
// 参数<1>: &节点值 MAKELONG(0x01, 0), 参数说明: "“取队列首节点”方法的填充参数，该方法执行完毕，该参数的内容为队列首节点值。如果方法执行失败，该参数内容无意义"
EDATASTRUCTURE_EXTERN_C void edatastructure_QueuePeek_49_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[1].m_ppCompoundData;

}

