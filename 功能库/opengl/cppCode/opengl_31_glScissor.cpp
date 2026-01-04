#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 矩形剪切, 命令说明: "定义一个矩形剪切框。裁剪框内的内容被保留下来，其余被减掉。必须启用“#裁剪测试”才有效，不使用的时候需要关闭。"
// 参数<1>: 左下角横坐标 SDT_INT, 参数说明: "相对于显示设备的左下角，以像素为单位"
// 参数<2>: 左下角纵坐标 SDT_INT, 参数说明: "相对于显示设备的左下角，以像素为单位"
// 参数<3>: 宽度 SDT_INT, 参数说明: "以像素为单位"
// 参数<4>: 高度 SDT_INT, 参数说明: "以像素为单位"
OPENGL_EXTERN_C void opengl_glScissor_31_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;
    INT      arg4 = pArgInf[3].m_int;

}

