#include "..\include_opengl_header.h"

// 调用格式: SDT_INT 选择像素格式, 命令说明: "为指定的设备选择一个匹配的像素格式。成功返回像素格式号，失败返回0。"
// 参数<1>: 设备句柄 SDT_INT, 参数说明: "需要取得像素格式号的设备句柄。"
// 参数<2>: 像素格式 MAKELONG(0x01, 0), 参数说明: "为指定的设备选择的像素格式。"
OPENGL_EXTERN_C void opengl_ChoosePixelFormat_0_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    void*    arg2 = pArgInf[1].m_pCompoundData;

}

