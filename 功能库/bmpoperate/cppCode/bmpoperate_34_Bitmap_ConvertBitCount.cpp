#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_BIN 位图_转换位深度, 命令说明: NULL
// 参数<1>: DC句柄 SDT_INT, 参数说明: NULL
// 参数<2>: 原始位图数据 SDT_BIN, 参数说明: "如果本参数指定的不是有效的位图数据，将转换失败甚至造成程序崩溃"
// 参数<3>: 目的位深度 SDT_BYTE, 参数说明: "参看“位图”对象的相关方法"
BMPOPERATE_EXTERN_C void bmpoperate_Bitmap_ConvertBitCount_34_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPBYTE   arg2 = pArgInf[1].m_pBin;
    BYTE     arg3 = pArgInf[2].m_byte;

}

