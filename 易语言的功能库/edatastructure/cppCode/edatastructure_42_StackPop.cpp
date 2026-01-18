#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (栈).弹出, 命令说明: "从栈中弹出一个节点。该方法执行成功返回真，执行失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息"
// 参数<1>: &节点值 MAKELONG(0x01, 0), 参数说明: "“弹出”方法的填充参数，该方法执行完毕，该参数的内容为从栈顶弹出的节点值。如果方法执行失败，该参数内容无意义"
EDATASTRUCTURE_EXTERN_C void edatastructure_StackPop_42_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[1].m_ppCompoundData;

}

