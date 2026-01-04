#include "..\include_bmpoperate_header.h"

// 调用格式: MAKELONG(0x01, 0) (位图).转换位深度, 命令说明: "转换本位图对象的颜色位深度，并返回转换后的对象。失败返回空位图对象。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。"
// 参数<1>: 欲转换为的位深度 SDT_SHORT, 参数说明: "本参数的有效值请参看本数据类型的“取位深度”命令"
BMPOPERATE_EXTERN_C void bmpoperate_SetBitCount_21_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    SHORT    arg1 = pArgInf[1].m_short;

}

