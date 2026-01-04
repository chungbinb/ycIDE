#include "..\include_commobj_header.h"

// 调用格式: SDT_BIN (快速字节集对象).取字节集, 命令说明: "取本对象中的全部或部分字节集。"
// 参数<1>: [起始位置 SDT_INT], 参数说明: "位置索引可以为正值，1表示第一个字节，2表示第二个字节，依次类推；位置索引也可以为负值，-1表示最后一个字节，-2表示倒数第二个字节，依次类推。如果本参数被省略，默认为1。"
// 参数<2>: [字节数 SDT_INT], 参数说明: "如果本参数被省略，默认为从“起始位置”到字节集结尾的字节数。"
COMMOBJ_EXTERN_C void commobj_GetData_84_commobj(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;

}

