#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_BOOL (位图).取指针, 命令说明: "取得该位图数据中关键段的指针，成功返回真，否则返回假。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。\r\n    本命令适用于一些特殊的需要直接操作内存指针场合，一般用户不推荐使用！！如果需要修改这些指针所指向的数据，请务必谨慎，以免造成程序崩溃等意外结果"
// 参数<1>: &位图信息头指针 SDT_INT, 参数说明: "该参数接收一个指向“位图信息头(BITMAPINFOHEADER)”结构的指针，也是指向BITMAPINFO结构的指针"
// 参数<2>: &像素点阵数据指针 SDT_INT, 参数说明: "该参数接收一个指向位图实际像素点阵数据的指针。"
BMPOPERATE_EXTERN_C void bmpoperate_GetPointers_23_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PINT     arg1 = pArgInf[1].m_pInt;
    PINT     arg2 = pArgInf[2].m_pInt;

}

