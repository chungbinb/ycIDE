#include "..\include_commobj_header.h"

// 调用格式: SDT_BOOL (快速文本对象).置内存增量, 命令说明: "设置本对象内部使用的内存增量值。所谓“内存增量”，即每次需要申请内存时在所需内存的基础上增加的内存数量（多申请出来的内存可供后续操作使用，从而减少了申请内存的次数，提高了执行效率）。正确使用本方法可以优化内存使用并提高执行效率，但因为内部已经有适当的初始值，普通用户不必调用本方法。"
// 参数<1>: 内存增量 SDT_INT, 参数说明: "单位为字节。如果此值过小，可能导致数据操作效率降低；如果此值过大，可能导致消耗较多内存。内部初始值为10240字节，约10K。"
COMMOBJ_EXTERN_C void commobj_SetMemGrowSize_73_commobj(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

