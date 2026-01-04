#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).检测设备类型, 命令说明: "检测指定适配器是否支持指定的设备类型。"
// 参数<1>: [适配器序号 SDT_INT], 参数说明: "指定要检测的适配器的序号。默认值为主显示适配器。"
// 参数<2>: 设备类型 SDT_INT, 参数说明: "指定设备的类型，为以下常量值之一：1、#设备常量.硬件加速；2、#设备常量.软件加速；3、#设备常量.第三方设备插件。"
// 参数<3>: 显示格式 MAKELONG(0x35, 0), 参数说明: "要检测的适配器的显示格式。"
// 参数<4>: 后台缓冲格式 MAKELONG(0x35, 0), 参数说明: "应该注意到，在窗口模式，后台缓冲的格式不一定要匹配于显示格式，只要你的硬件支持适当的颜色转换就可以了。不论你的硬件是否支持这种功能，本方法都会返回适当的结果。"
// 参数<5>: 窗口模式 SDT_BOOL, 参数说明: "是否窗口模式或全屏模式。真为窗口模式，假为全屏模式。"
OGRELIB_EXTERN_C void ogrelib_CheckDeviceType_223_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    void*    arg3 = pArgInf[3].m_pCompoundData;
    void*    arg4 = pArgInf[4].m_pCompoundData;
    BOOL     arg5 = pArgInf[5].m_bool;

}

