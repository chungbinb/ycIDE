#include "..\include_commobj_header.h"

// 调用格式: SDT_INT (快速字节集对象).倒找字节集, 命令说明: "在本对象数据中从“起始寻找位置”开始向数据开头方向寻找指定数据第一次出现时的位置索引（相对数据开头），未找到则返回-1。"
// 参数<1>: 欲寻找的数据 _SDT_ALL, 参数说明: "参数类型支持各种基本数据类型。"
// 参数<2>: [起始寻找位置 SDT_INT], 参数说明: "位置索引可以为正值，1表示第一个字节，2表示第二个字节，依次类推；位置索引也可以为负值，-1表示最后一个字节，-2表示倒数第二个字节，依次类推。默认为-1，即从数据结尾开始向前寻找。"
COMMOBJ_EXTERN_C void commobj_FindReverse_95_commobj(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[1].m_pByte;
    INT      arg2 = pArgInf[2].m_int;

}

