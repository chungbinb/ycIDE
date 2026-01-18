#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_BIN (位图).取像素点阵, 命令说明: "取得该位图中代表实际像素点阵的数据，该数据的排列格式取决于该位图的位深度和实际宽度，具体请参阅相关资料。失败返回空字节集。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。"
// 无参数
BMPOPERATE_EXTERN_C void bmpoperate_GetBits_12_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}

