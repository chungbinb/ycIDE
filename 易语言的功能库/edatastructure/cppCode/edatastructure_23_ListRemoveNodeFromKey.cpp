#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (链表).删除节点, 命令说明: "删除链表中指定键值的节点。该方法执行成功返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息"
// 参数<1>: 键值 SDT_INT, 参数说明: "“删除节点”方法中欲删除节点数据的键值"
EDATASTRUCTURE_EXTERN_C void edatastructure_ListRemoveNodeFromKey_23_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

