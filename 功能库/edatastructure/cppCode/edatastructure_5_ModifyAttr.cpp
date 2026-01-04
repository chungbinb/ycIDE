#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (节点).修改属性, 命令说明: "修改节点的属性值。成功修改返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息"
// 参数<1>: 属性名称 SDT_TEXT, 参数说明: "“修改属性”方法中欲修改的属性名称"
// 参数<2>: 新属性值 _SDT_ALL, 参数说明: "“修改属性”方法中欲修改的新的属性值"
EDATASTRUCTURE_EXTERN_C void edatastructure_ModifyAttr_5_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    PVOID    arg2 = pArgInf[2].m_pByte;

}

