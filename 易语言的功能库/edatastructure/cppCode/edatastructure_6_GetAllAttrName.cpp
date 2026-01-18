#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (节点).取全部属性名, 命令说明: "取出节点的全部属性名称。成功返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息"
// 参数<1>: &属性名称数组 数组 SDT_TEXT, 参数说明: "“取全部属性名”方法的填充参数。方法执行完毕后，该参数的内容为全部属性名称的数组。如果方法执行失败，该参数内容无意义"
EDATASTRUCTURE_EXTERN_C void edatastructure_GetAllAttrName_6_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[1].m_ppAryData;

}

