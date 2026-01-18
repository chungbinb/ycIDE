#include "..\include_bmpoperate_header.h"

// 调用格式: MAKELONG(0x01, 0) (位图).旋转, 命令说明: "把一个位图旋转一个角度，并返回旋转后的位图。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。注意：对一个位图进行旋转后会有少许失真。如果本旋转算法的精度无法满足要求，请根据实际情况用“取某点颜色/索引”、“置某点颜色/索引”结合相应的的旋转算法进行操作"
// 参数<1>: 旋转角度 SDT_FLOAT, 参数说明: "正值代表正时针旋转，负值代表逆时针旋转"
// 参数<2>: 背景填充颜色 SDT_INT, 参数说明: "如果位图经过旋转后尺寸变大，新增的部分将用该颜色填充"
BMPOPERATE_EXTERN_C void bmpoperate_Rotate_28_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    FLOAT    arg1 = pArgInf[1].m_float;
    INT      arg2 = pArgInf[2].m_int;

}

