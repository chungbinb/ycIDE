#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (二叉树).取当前节点, 命令说明: "取二叉树的当前节点。成功返回真，失败返回假"
// 参数<1>: &节点值 MAKELONG(0x01, 0), 参数说明: "“取当前节点”方法的填充参数，该方法执行完毕，该参数的内容为二叉树的当前节点值"
EDATASTRUCTURE_EXTERN_C void edatastructure_BiTreeGetCurNode_64_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[1].m_ppCompoundData;

}

