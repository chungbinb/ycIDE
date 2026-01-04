#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (树).插入节点, 命令说明: "向树中插入节点，插入位置为当前节点的子节点链表的末端。成功返回真，失败返回假。如果树是一个空的树，则把插入的节点作为树的根节点"
// 参数<1>: 节点值 MAKELONG(0x01, 0), 参数说明: "“插入节点”方法中欲插入到树中的节点值"
EDATASTRUCTURE_EXTERN_C void edatastructure_TreeInsert_83_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

