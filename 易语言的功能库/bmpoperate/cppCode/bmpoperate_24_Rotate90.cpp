#include "..\include_bmpoperate_header.h"

// 调用格式: MAKELONG(0x01, 0) (位图).旋转90度, 命令说明: "将该位图顺时针或逆时针旋转90度，并返回旋转后的位图，失败返回空位图。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。"
// 参数<1>: 是否顺时针旋转 SDT_BOOL, 参数说明: "真，顺时针方向旋转；假，逆时针"
BMPOPERATE_EXTERN_C void bmpoperate_Rotate90_24_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    BOOL     arg1 = pArgInf[1].m_bool;

}

