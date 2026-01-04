#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 操作累积缓存, 命令说明: "对裁剪区域里的缓存颜色进行累积缓存操作。"
// 参数<1>: 操作方式 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t256、#获取缓存，获取红绿蓝以及混合通道的数据，并加入到累积缓存里；\r\n\t257、#载入缓存，获取红绿蓝以及混合通道的数据，并加入到累积缓存里，但是不包括缓存现有的数据；\r\n\t258、#返回缓存，将累积缓存里的数据返回到颜色缓存中去；\r\n\t259、#缩放缓存，按照操作值来缩放累积缓存里的数据；\r\n\t260、#加入缓存，增加累积缓存里每个红绿蓝以及混合通道的数据"
// 参数<2>: 操作值 SDT_FLOAT, 参数说明: "范围在[-1，1]内的数值。"
OPENGL_EXTERN_C void opengl_glAccum_41_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    FLOAT    arg2 = pArgInf[1].m_float;

}

