#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (链表).取当前节点键值, 命令说明: "取链表中当前节点的键值。该方法执行成功返回真，执行失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息"
// 参数<1>: &键值 SDT_INT, 参数说明: "“取当前节点键值”方法的填充参数。方法成功执行，该参数的内容为链表中当前节点的键值"
EDATASTRUCTURE_EXTERN_C void edatastructure_ListGetCurNodeKey_31_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PINT     arg1 = pArgInf[1].m_pInt;

}

