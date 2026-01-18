#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (二叉树).插入节点, 命令说明: "向二叉树中插入节点。成功返回真，失败返回假"
// 参数<1>: 节点值 MAKELONG(0x01, 0), 参数说明: "“插入节点”方法中欲插入到二叉树中的节点值"
// 参数<2>: [键值 SDT_INT], 参数说明: "“插入节点”方法中欲插入到二叉树中的节点的键值。该键值不能为负数。该参数可以省略，如果省略，则新插入节点的键值为二叉树中的最大键值加一"
EDATASTRUCTURE_EXTERN_C void edatastructure_BiTreeInsert_66_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    INT      arg2 = pArgInf[2].m_int;

}

