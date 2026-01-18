#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).拉伸块复制, 命令说明: "将源页面指定区域的像素块拉伸后复制到目的页面的指定区域。该命令一般在改变视频流的纵横比之后使用。"
// 参数<1>: 源页面 MAKELONG(0x08, 0), 参数说明: NULL
// 参数<2>: [源矩形区域 MAKELONG(0x01, 0)], 参数说明: "源矩形区域。为空则复制整个页面。"
// 参数<3>: 目的页面 MAKELONG(0x08, 0), 参数说明: NULL
// 参数<4>: [目的矩形区域 MAKELONG(0x01, 0)], 参数说明: "目的矩形区域。为空则复制整个页面。"
// 参数<5>: 过滤器类型 SDT_INT, 参数说明: "为“纹理过滤类型”常量中的任一值。"
OGRELIB_EXTERN_C void ogrelib_StretchRect_189_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    void*    arg2 = pArgInf[2].m_pCompoundData;
    void*    arg3 = pArgInf[3].m_pCompoundData;
    void*    arg4 = pArgInf[4].m_pCompoundData;
    INT      arg5 = pArgInf[5].m_int;

}

