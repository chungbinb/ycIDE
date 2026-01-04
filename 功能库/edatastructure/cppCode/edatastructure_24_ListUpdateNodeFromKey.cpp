#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (链表).修改节点, 命令说明: "使用参数节点替换链表中指定键值的节点。该方法执行成功返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息"
// 参数<1>: 节点值 MAKELONG(0x01, 0), 参数说明: "“修改节点”方法中链表节点被更改后的新节点数据"
// 参数<2>: 键值 SDT_INT, 参数说明: "“修改节点”方法中欲更改的节点数据的键值"
EDATASTRUCTURE_EXTERN_C void edatastructure_ListUpdateNodeFromKey_24_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    INT      arg2 = pArgInf[2].m_int;

}

