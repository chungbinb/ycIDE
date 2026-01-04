#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (二叉树).删除节点, 命令说明: "删除二叉树中指定键值的节点。如果参数“删除标志”为假，仅删除一个节点的情况下分为以下几种不同的情形和执行结果：一，欲删除节点没有子节点，直接删除节点。二，欲删除节点仅有左子节点，删除节点后，把删除节点的左子节点连接到删除节点的双亲上。三，欲删除节点仅有右子节点，删除节点后，把删除节点的右子节点连接到删除节点的双亲上。四，欲删除节点有左子节点和右子节点，删除节点后，把删除节点的左子树上键值最大的节点（也是最右边的节点）连接到删除节点的双亲上，也就是提升到删除节点的位置，然后把它的左子树连接到它以前位置的双亲上。成功删除后，二叉树的当前节点值恢复为根节点值。该方法成功返回真，失败返回假"
// 参数<1>: 键值 SDT_INT, 参数说明: "“删除节点”方法中欲删除节点的键值"
// 参数<2>: [删除标记 SDT_BOOL], 参数说明: "该标记为真时表示删除节点及其所有的子节点，为假时表示仅删除单个节点。该参数可以省略，默认为假"
EDATASTRUCTURE_EXTERN_C void edatastructure_BiTreeDelete_69_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    BOOL     arg2 = pArgInf[2].m_bool;

}

