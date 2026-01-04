#include "..\include_opengl_header.h"

// 调用格式: SDT_INT 显示光标, 命令说明: "指定当前应用程序是否显示光标，也可以用来判断当前的光标是否显示，返回值大于0表示正在显示，返回值小于0表示正在隐藏。"
// 参数<1>: 是否显示 SDT_BOOL, 参数说明: "真表示显示，假表示隐藏光标"
OPENGL_EXTERN_C void opengl_ShowCursor_6_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    BOOL     arg1 = pArgInf[0].m_bool;

}

