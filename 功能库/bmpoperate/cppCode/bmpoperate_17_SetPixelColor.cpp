#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_BOOL (位图).置某点颜色, 命令说明: "参看“取某点颜色”。成功返回真，否则返回假。注意，如果参数3所指定的颜色值不被当前位图格式所支持，系统会自动选择最接近的有效颜色"
// 参数<1>: 横坐标 SDT_INT, 参数说明: "以像素为单位。如果该参数指定的坐标超出了位图的实际范围，本命令将失败"
// 参数<2>: 纵坐标 SDT_INT, 参数说明: "以像素为单位。如果该参数指定的坐标超出了位图的实际范围，本命令将失败"
// 参数<3>: 欲置入的值 SDT_INT, 参数说明: NULL
BMPOPERATE_EXTERN_C void bmpoperate_SetPixelColor_17_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;

}

