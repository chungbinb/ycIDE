#include "..\include_xplib_header.h"

// 调用格式: SDT_BOOL XP风格, 命令说明: "设置当前应用程序的所有窗口组件风格为XP界面风格，成功返回真，失败返回假"
// 参数<1>: 风格类型 SDT_INT, 参数说明: "本参数可以为以下常量值之一: 0.#无风格;  1.#蓝色风格;  2.#绿色风格;  3.#银色风格"
XPLIB_EXTERN_C void xplib_SetXP_0_xplib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

