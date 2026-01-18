#include "..\include_commobj_header.h"

// 调用格式: SDT_BOOL (快速字节集对象).删除字节, 命令说明: "删除字节集中从指定字节开始的指定个数的字节（包括起始删除位置处的字节）"
// 参数<1>: 起始删除位置 SDT_INT, 参数说明: "位置索引可以为正值，1表示第一个字节，2表示第二个字节，依次类推；位置索引也可以为负值，-1表示最后一个字节，-2表示倒数第二个字节，依次类推。"
// 参数<2>: 欲删除的字节数 SDT_INT, 参数说明: NULL
COMMOBJ_EXTERN_C void commobj_DeleteBytes_89_commobj(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;

}

