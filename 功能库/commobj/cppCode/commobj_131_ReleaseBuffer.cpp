#include "..\include_commobj_header.h"

// 调用格式: SDT_BOOL (快速字节集对象).释放缓冲区, 命令说明: "告知对象“最新的、经外界修改过的”数据长度。另请参考“取缓冲区()”中的说明。"
// 参数<1>: 数据长度 SDT_INT, 参数说明: "经外界修改后的数据长度。"
COMMOBJ_EXTERN_C void commobj_ReleaseBuffer_131_commobj(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

