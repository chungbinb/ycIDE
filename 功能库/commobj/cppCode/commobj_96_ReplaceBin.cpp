#include "..\include_commobj_header.h"

// 调用格式: SDT_BOOL (快速字节集对象).替换字节集, 命令说明: "将本对象数据中从指定位置开始指定长度的数据替换为新的数据。"
// 参数<1>: 起始替换位置 SDT_INT, 参数说明: "位置索引可以为正值，1表示第一个字节，2表示第二个字节，依次类推；位置索引也可以为负值，-1表示最后一个字节，-2表示倒数第二个字节，依次类推。"
// 参数<2>: 替换长度 SDT_INT, 参数说明: NULL
// 参数<3>: [替换为字节集 _SDT_ALL], 参数说明: "参数类型支持各种基本数据类型。如果本参数被省略，则删除字节集中的指定部分。"
COMMOBJ_EXTERN_C void commobj_ReplaceBin_96_commobj(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    PVOID    arg3 = pArgInf[3].m_pByte;

}

