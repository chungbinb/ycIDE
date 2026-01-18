#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置视口, 命令说明: "设置设备中可以显示的范围。"
// 参数<1>: 左下角横坐标 SDT_INT, 参数说明: "必须大于0。如果需要全部显示就设为0"
// 参数<2>: 左下角竖坐标 SDT_INT, 参数说明: "必须大于0。如果需要全部显示就设为0"
// 参数<3>: 宽度 SDT_INT, 参数说明: "必须大于0。如果需要全部显示就设为组件的完整宽度"
// 参数<4>: 高度 SDT_INT, 参数说明: "必须大于0。如果需要全部显示就设为组件的完整高度"
OPENGL_EXTERN_C void opengl_glViewport_15_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;
    INT      arg4 = pArgInf[3].m_int;

}

