#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (链表).加入节点, 命令说明: "在链表中加入新的节点。加入链表中自动根据键值参数进行从小到大的排序。键值参数可以省略，如果省略，默认键值为链表中最大的键值加一。该方法执行成功返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息"
// 参数<1>: 节点值 MAKELONG(0x01, 0), 参数说明: "“加入节点”方法中欲加入到链表中的节点数据"
// 参数<2>: [键值 SDT_INT], 参数说明: "“加入节点”方法中欲加入到链表中的节点数据的键值。这个值也是节点在链表中的唯一标识，节点在链表中存在的过程中键值是不允许改变的。该键值必须大于等于0"
EDATASTRUCTURE_EXTERN_C void edatastructure_ListAddNode_18_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    INT      arg2 = pArgInf[2].m_int;

}

