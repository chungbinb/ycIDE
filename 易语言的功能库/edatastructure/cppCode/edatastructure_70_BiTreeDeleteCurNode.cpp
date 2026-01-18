#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (二叉树).删除当前节点, 命令说明: "删除二叉树中当前节点。删除原则和“删除节点”方法相同。成功删除后，二叉树的当前节点值恢复为根节点值。该方法成功返回真，失败返回假"
// 参数<1>: [删除标记 SDT_BOOL], 参数说明: "该标记为真时表示删除节点及其所有的子节点，为假时表示仅删除单个节点。该参数可以省略，默认为假"
EDATASTRUCTURE_EXTERN_C void edatastructure_BiTreeDeleteCurNode_70_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    BOOL     arg1 = pArgInf[1].m_bool;

}

