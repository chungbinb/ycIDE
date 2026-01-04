#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_BOOL (位图).置像素点阵, 命令说明: "参看“取像素点阵”。成功返回真，否则返回假。如果指定数据的格式错误，本命令调用失败。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。\r\n    注意，操作位图的像素点阵需要对位图格式有一定的了解，错误的操作可能会破坏位图数据，请参阅相关资料"
// 参数<1>: 欲置入的点阵数据 SDT_BIN, 参数说明: NULL
BMPOPERATE_EXTERN_C void bmpoperate_SetBits_20_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[1].m_pBin;

}

