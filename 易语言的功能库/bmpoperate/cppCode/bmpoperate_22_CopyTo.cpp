#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_BOOL (位图).复制到, 命令说明: "将本位图的某一矩形区域复制到目的位图，成功返回真，失败返回假。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。"
// 参数<1>: 欲复制区域的左边 SDT_INT, 参数说明: NULL
// 参数<2>: 欲复制区域的顶边 SDT_INT, 参数说明: NULL
// 参数<3>: [欲复制区域的宽度 SDT_INT], 参数说明: "如果本参数省略，默认值为源位图宽度"
// 参数<4>: [欲复制区域的高度 SDT_INT], 参数说明: "如果本参数省略，默认值为源位图高度"
// 参数<5>: [复制到的目的位图 MAKELONG(0x01, 0)], 参数说明: "如果本参数省略，则复制到源位图对象。注意：如果是复制到自身，且欲复制区域和复制到的区域有重叠部分，可能出现重叠复制。"
// 参数<6>: 欲复制到位置左边 SDT_INT, 参数说明: "注意：复制到的矩形不能超出目的位图的区域，否则本命令调用失败"
// 参数<7>: 欲复制到位置顶边 SDT_INT, 参数说明: "注意：复制到的矩形不能超出目的位图的区域，否则本命令调用失败"
// 参数<8>: [透明色 SDT_INT], 参数说明: "透明色为源位图在复制过程中将被忽略的颜色。如果本参数省略，不使用透明色。"
BMPOPERATE_EXTERN_C void bmpoperate_CopyTo_22_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;
    INT      arg4 = pArgInf[4].m_int;
    void*    arg5 = pArgInf[5].m_pCompoundData;
    INT      arg6 = pArgInf[6].m_int;
    INT      arg7 = pArgInf[7].m_int;
    INT      arg8 = pArgInf[8].m_int;

}

