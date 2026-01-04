#include "..\include_opengl_header.h"

// 调用格式: SDT_BOOL 置像素格式, 命令说明: "使用像素格式号为指定的设备设置像素格式。成功返回真，失败返回假。"
// 参数<1>: 设备句柄 SDT_INT, 参数说明: "需要设置像素格式的设备句柄。"
// 参数<2>: 像素格式号 SDT_INT, 参数说明: "与指定的设备相匹配的像素格式号"
// 参数<3>: 像素格式 MAKELONG(0x01, 0), 参数说明: "为指定的设备选择的像素格式。"
OPENGL_EXTERN_C void opengl_SetPixelFormat_1_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    void*    arg3 = pArgInf[2].m_pCompoundData;

}

