#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x22, 0) (设备).[]取适配器模式列表, 命令说明: "取得可用的适配器模式列表。成功返回模式列表数组，失败返回空。"
// 参数<1>: 适配器序号 SDT_INT, 参数说明: "指定要取得模式列表的适配器的序号。0表示主显示适配器。"
// 参数<2>: 像素格式 MAKELONG(0x35, 0), 参数说明: "允许的像素格式。有效的格式有以下几个：21、#页面格式.透8红8绿8蓝8；22、#页面格式.保8红8绿8蓝8；23、#页面格式.红5绿6蓝5；24、#页面格式.保1红5绿5蓝5；25、#页面格式.透1红5绿5蓝5；35、#页面格式.透2红10绿10蓝10。"
OGRELIB_EXTERN_C void ogrelib_EnumAdapterModes_221_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

