#include "..\include_commobj_header.h"

// 调用格式: SDT_BOOL (快速字节集对象).置可用内存, 命令说明: "设置本对象内部使用的最小可用内存和最大可用内存。使用本方法可以优化内存使用，但因为内部已经有适当的初始值，普通用户不必调用本方法。"
// 参数<1>: [最小可用内存 SDT_INT], 参数说明: "单位为字节。如果本参数被省略，则保持现有“最小可用内存”数值不变。其内部初始值为10240字节，约10K。"
// 参数<2>: [最大可用内存 SDT_INT], 参数说明: "单位为字节。如果本参数被省略，则保持现有“最大可用内存”数值不变。其内部初始值为2147483647字节，约2G。"
COMMOBJ_EXTERN_C void commobj_SetMemLength_132_commobj(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;

}

