#include "..\include_wnet_header.h"

// 调用格式: SDT_BOOL (局域网操作).断开映射, 命令说明: "断开一个映射到本地驱动器的共享资源。成功返回“真”，失败返回“假”"
// 参数<1>: 欲断开的映射驱动器 SDT_TEXT, 参数说明: "本参数指定欲断开连接的映射驱动器（或打印机）名称。比如“X:”或“LPT1”"
// 参数<2>: 是否永久断开 SDT_BOOL, 参数说明: "本参数指定是否永久断开该映射驱动器。永久断开的映射驱动器在下次登录时不会自动重新连接"
// 参数<3>: 是否强制断开 SDT_BOOL, 参数说明: "如果本参数为“真”，即使本映射驱动器正在使用，也立即强制断开"
WNET_EXTERN_C void wnet_CancelResMap_6_wnet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    BOOL     arg2 = pArgInf[2].m_bool;
    BOOL     arg3 = pArgInf[3].m_bool;

}

