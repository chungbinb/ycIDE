#include "..\include_edatastructure_header.h"

// 本命令被隐藏, 原始名字 = "拷贝构造函数", 本命令为复制构造函数
// 调用格式: _SDT_NULL (链表).拷贝构造函数, 命令说明: "拷贝构造。该命令在系统调用“=”时，系统会自动调用该方法"
// 参数<1>: 源链表 MAKELONG(0x02, 0), 参数说明: "“拷贝构造”方法中欲拷贝的源链表"
EDATASTRUCTURE_EXTERN_C void edatastructure_ListCopy_15_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

