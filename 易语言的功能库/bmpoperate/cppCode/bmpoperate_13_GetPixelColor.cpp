#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_INT (位图).取某点颜色, 命令说明: "取得该位图中某点的实际颜色值，以位图的左上角为坐标原点。注意，对位深度为16的位图，取得的颜色可能有少许误差，这是由16位位图的特殊内部结构决定的。如果指定的像素点坐标越界，将导致调用失败，返回-1。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。"
// 参数<1>: 横坐标 SDT_INT, 参数说明: "以像素为单位。如果该参数指定的坐标超出了位图的实际范围，本命令将失败"
// 参数<2>: 纵坐标 SDT_INT, 参数说明: "以像素为单位。如果该参数指定的坐标超出了位图的实际范围，本命令将失败"
BMPOPERATE_EXTERN_C void bmpoperate_GetPixelColor_13_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;

}

