#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (节点).取数值, 命令说明: "取出节点的数值型属性值。成功返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息"
// 参数<1>: 属性名称 SDT_TEXT, 参数说明: "“取数值”方法中欲取值的属性名称"
// 参数<2>: &属性值 _SDT_ALL, 参数说明: "“取数值”方法的填充参数。该参数必须为数值型，否则“取数值”方法返回假。方法执行完毕后，该参数的内容为属性的数值，如果方法执行失败，该参数内容无意义"
EDATASTRUCTURE_EXTERN_C void edatastructure_GetDoubleValue_8_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    PVOID    arg2 = pArgInf[2].m_pByte;

}

