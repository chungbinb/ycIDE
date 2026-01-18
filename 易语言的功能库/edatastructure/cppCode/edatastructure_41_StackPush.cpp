#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (栈).压入, 命令说明: "向栈中压入一个节点。该方法执行成功返回真，执行失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息"
// 参数<1>: 节点值 MAKELONG(0x01, 0), 参数说明: "“压入”方法中欲压到栈中节点"
EDATASTRUCTURE_EXTERN_C void edatastructure_StackPush_41_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

