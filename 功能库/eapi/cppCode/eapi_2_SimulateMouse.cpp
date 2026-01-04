#include "..\include_eapi_header.h"

// 调用格式: _SDT_NULL 模拟鼠标点击, 命令说明: "模拟鼠标点击。"
// 参数<1>: X坐标 SDT_INT, 参数说明: NULL
// 参数<2>: Y坐标 SDT_INT, 参数说明: NULL
// 参数<3>: [点击类型 SDT_INT], 参数说明: "可以被省略。模拟鼠标点击的类型，可以为以下常量值之一：0、#接口常量.左键单击；1、#接口常量.左键双击；2、#接口常量.右键单击；3、#接口常量.右键双击。"
EAPI_EXTERN_C void eapi_SimulateMouse_2_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;

}

