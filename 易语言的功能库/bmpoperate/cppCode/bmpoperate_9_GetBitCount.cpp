#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_SHORT (位图).取位深度, 命令说明: "取得该位图中颜色的位深度。失败返回-1。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。有效的返回值及其含义如下：\r\n\t1：  单色；\r\n\t4：  4位色（16色）；\r\n\t8：  8位色（256色）；\r\n\t16： 16位色（65536色）；\r\n\t24： 24位色（16777216色）；\r\n\t32： 32位色（4294967296色）。\r\n\t"
// 无参数
BMPOPERATE_EXTERN_C void bmpoperate_GetBitCount_9_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}

