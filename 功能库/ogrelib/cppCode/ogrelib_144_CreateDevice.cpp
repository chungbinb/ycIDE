#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).创建设备, 命令说明: "创建设备对象，几乎所有的3D绘图功能都要通过它实现。"
// 参数<1>: 设备参数 MAKELONG(0x1B, 0), 参数说明: "设备初始化参数。"
// 参数<2>: 窗口句柄 SDT_INT, 参数说明: "设备窗口句柄。"
// 参数<3>: 设备类型 SDT_INT, 参数说明: "指定设备的类型，为以下常量值之一：1、#设备常量.硬件加速；2、#设备常量.软件加速；3、#设备常量.第三方设备插件。"
// 参数<4>: 设备创建行为 SDT_INT, 参数说明: "控制设备创建行为，为以下常量值之一或之和：2、#设备常量.FPU保护；4、#设备常量.采用多线程；16、#设备常量.纯设备；32、#设备常量.软件顶点运算；64、#设备常量.硬件顶点运算；128、#设备常量.混合顶点运算；256、#设备常量.代替显卡驱动程序管理；512、#设备常量.驱动所有主适配器；"
OGRELIB_EXTERN_C void ogrelib_CreateDevice_144_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;
    INT      arg4 = pArgInf[4].m_int;

}

