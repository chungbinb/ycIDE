#include "..\include_commobj_header.h"

// 调用格式: SDT_BYTE (快速字节集对象).取字节, 命令说明: "取指定位置处的字节值。"
// 参数<1>: 位置索引 SDT_INT, 参数说明: "位置索引可以为正值，1表示第一个字节，2表示第二个字节，依次类推；位置索引也可以为负值，-1表示最后一个字节，-2表示倒数第二个字节，依次类推。"
COMMOBJ_EXTERN_C void commobj_GetByte_90_commobj(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

