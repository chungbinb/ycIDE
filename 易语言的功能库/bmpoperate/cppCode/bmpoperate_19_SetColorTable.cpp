#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_BOOL (位图).置颜色表, 命令说明: "参看“取颜色表”。成功返回真，否则返回假。如果该位图不使用颜色表或指定的颜色表格式错误，本命令调用失败。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。\r\n    注意，操作位图的颜色表需要对位图格式有一定的了解，错误的操作可能会破坏位图数据，请参阅相关资料"
// 参数<1>: 欲置入的颜色表数据 数组 SDT_INT, 参数说明: NULL
BMPOPERATE_EXTERN_C void bmpoperate_SetColorTable_19_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;

}

