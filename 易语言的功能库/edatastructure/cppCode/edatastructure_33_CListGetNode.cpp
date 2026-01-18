#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (链表).取节点, 命令说明: "根据参数键值取链表中的节点。该方法执行成功返回真，执行失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息"
// 参数<1>: &节点值 MAKELONG(0x01, 0), 参数说明: "“取节点”方法的填充参数。方法成功执行，该参数的内容为链表中的节点数据。如果方法执行失败，该参数内容无意义"
// 参数<2>: 键值 SDT_INT, 参数说明: "“取节点”方法中欲取得节点数据的键值"
EDATASTRUCTURE_EXTERN_C void edatastructure_CListGetNode_33_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[1].m_ppCompoundData;
    INT      arg2 = pArgInf[2].m_int;

}

