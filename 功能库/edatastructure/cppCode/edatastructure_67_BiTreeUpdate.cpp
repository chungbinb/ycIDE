#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (二叉树).修改节点, 命令说明: "修改二叉树中指定键值的节点。成功返回真，失败返回假"
// 参数<1>: 节点值 MAKELONG(0x01, 0), 参数说明: "“修改节点”方法中修改后的节点值"
// 参数<2>: 键值 SDT_INT, 参数说明: "“修改节点”方法中欲修改节点的键值"
EDATASTRUCTURE_EXTERN_C void edatastructure_BiTreeUpdate_67_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    INT      arg2 = pArgInf[2].m_int;

}

