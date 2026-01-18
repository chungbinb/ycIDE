#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_BOOL (位图).置某点颜色索引, 命令说明: "参看“取某点颜色索引”。成功返回真，否则返回假。如果该位图不使用颜色表或参数3指定的颜色索引超出了该位图颜色表的最大索引，本命令调用失败。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。"
// 参数<1>: 横坐标 SDT_INT, 参数说明: "以像素为单位。如果该参数指定的坐标超出了位图的实际范围，本命令将失败"
// 参数<2>: 纵坐标 SDT_INT, 参数说明: "以像素为单位。如果该参数指定的坐标超出了位图的实际范围，本命令将失败"
// 参数<3>: 欲置入的值 SDT_INT, 参数说明: NULL
BMPOPERATE_EXTERN_C void bmpoperate_SetPixelColorIndex_18_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;

}

