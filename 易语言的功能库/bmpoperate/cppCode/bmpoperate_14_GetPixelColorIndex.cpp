#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_INT (位图).取某点颜色索引, 命令说明: "取得该位图中某点的颜色在颜色表中的索引，以位图的左上角为坐标原点。失败返回-1。对使用颜色表的位图调用本命令比调用“取某点颜色”执行速度更快。对不使用颜色表的位图调用该方法将导致调用失败。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。"
// 参数<1>: 横坐标 SDT_INT, 参数说明: "以像素为单位。如果该参数指定的坐标超出了位图的实际范围，本命令将失败"
// 参数<2>: 纵坐标 SDT_INT, 参数说明: "以像素为单位。如果该参数指定的坐标超出了位图的实际范围，本命令将失败"
BMPOPERATE_EXTERN_C void bmpoperate_GetPixelColorIndex_14_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;

}

