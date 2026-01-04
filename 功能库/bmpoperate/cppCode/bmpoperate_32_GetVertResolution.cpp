#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_BOOL (位图).置纵向分辨率, 命令说明: "设置本位图的纵向分辨率。分辨率一般在打印时用到。单位为“像素/米”"
// 参数<1>: 分辨率 SDT_INT, 参数说明: "单位为“像素/米”"
BMPOPERATE_EXTERN_C void bmpoperate_GetVertResolution_32_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

