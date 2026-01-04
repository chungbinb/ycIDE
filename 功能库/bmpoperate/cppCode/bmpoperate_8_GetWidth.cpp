#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_INT (位图).取高度, 命令说明: "取得该位图的实际高度，如果该位图的颜色点阵为正向（top-down）存储，高度为负值（不常见）。失败返回-1。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。"
// 无参数
BMPOPERATE_EXTERN_C void bmpoperate_GetWidth_8_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}

