#include "..\include_opengl_header.h"

// 调用格式: SDT_BOOL 到屏幕坐标, 命令说明: "将OPenGL的空间坐标转换到组件的屏幕坐标。成功返回真，否则返回假。"
// 参数<1>: 空间坐标横向位置 SDT_DOUBLE, 参数说明: "OPenGL的空间坐标系中的横向位置"
// 参数<2>: 空间坐标竖向位置 SDT_DOUBLE, 参数说明: "OPenGL的空间坐标系中的竖向位置"
// 参数<3>: 空间坐标纵向位置 SDT_DOUBLE, 参数说明: "OPenGL的空间坐标系中的纵向位置"
// 参数<4>: &存放屏幕坐标横向位置的变量 SDT_DOUBLE, 参数说明: "窗口组件中的横向位置"
// 参数<5>: &存放屏幕坐标纵向位置的变量 SDT_DOUBLE, 参数说明: "窗口组件中的纵向位置"
// 参数<6>: &存放屏幕深度的变量 SDT_DOUBLE, 参数说明: "0表示与投影坐标系中的最近的平面,1表示最远的平面。"
OPENGL_EXTERN_C void opengl_gluProject_123_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[0].m_double;
    double   arg2 = pArgInf[1].m_double;
    double   arg3 = pArgInf[2].m_double;
    double*  arg4 = pArgInf[3].m_pDouble;
    double*  arg5 = pArgInf[4].m_pDouble;
    double*  arg6 = pArgInf[5].m_pDouble;

}

