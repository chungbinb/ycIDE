#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).置曲面表面模式, 命令说明: "开启或关闭曲面表面模式（N-patches）。"
// 参数<1>: [曲面段个数 SDT_FLOAT], 参数说明: "指定使用多少段来生成曲面。如果段个数小于1.0，关闭曲面表面模式；反之则开启曲面表面模式。默认值为0.0。"
OGRELIB_EXTERN_C void ogrelib_SetNPatchMode_166_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    FLOAT    arg1 = pArgInf[1].m_float;

}

