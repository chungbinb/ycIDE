#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_INT (位图).[]取颜色表, 命令说明: "取得该位图使用的颜色表数据（注意，位深度大于等于16的位图不使用颜色表）。成功返回一个包含颜色表数据的数组，失败返回空数组（成员数为0）。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。"
// 无参数
BMPOPERATE_EXTERN_C void bmpoperate_GetColorTable_11_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}

