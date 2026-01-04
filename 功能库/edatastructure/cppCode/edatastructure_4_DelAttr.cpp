#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (节点).删除属性, 命令说明: "从节点中删除属性。成功删除返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息"
// 参数<1>: 属性名称 SDT_TEXT, 参数说明: "“删除属性”方法中欲删除的属性名称"
EDATASTRUCTURE_EXTERN_C void edatastructure_DelAttr_4_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

