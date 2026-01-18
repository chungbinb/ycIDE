#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).检测设备页面格式, 命令说明: "检测是否可在设备上使用指定的页面格式。"
// 参数<1>: [适配器序号 SDT_INT], 参数说明: "指定要检测的适配器的序号。默认值为主显示适配器。"
// 参数<2>: 设备类型 SDT_INT, 参数说明: "指定设备的类型，为以下常量值之一：1、#设备常量.硬件加速；2、#设备常量.软件加速；3、#设备常量.第三方设备插件。"
// 参数<3>: 显示格式 MAKELONG(0x35, 0), 参数说明: "要检测的适配器的显示格式。"
// 参数<4>: 用途 MAKELONG(0x37, 0), 参数说明: "为“资源用途”常量值之一。"
// 参数<5>: 资源类型 MAKELONG(0x36, 0), 参数说明: "为“资源类型”常量值之一。"
// 参数<6>: 检测格式 MAKELONG(0x35, 0), 参数说明: "要检测的设备页面格式。"
OGRELIB_EXTERN_C void ogrelib_CheckDeviceFormat_224_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    void*    arg3 = pArgInf[3].m_pCompoundData;
    void*    arg4 = pArgInf[4].m_pCompoundData;
    void*    arg5 = pArgInf[5].m_pCompoundData;
    void*    arg6 = pArgInf[6].m_pCompoundData;

}

