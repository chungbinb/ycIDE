#include "..\include_commobj_header.h"

// 调用格式: SDT_INT (快速字节集对象).替换子字节集, 命令说明: "在本对象数据中查找“被替换数据”，并将其替换为“替换为数据”。返回实际替换次数。"
// 参数<1>: 被替换数据 _SDT_ALL, 参数说明: "参数类型支持各种基本数据类型。"
// 参数<2>: [替换为数据 _SDT_ALL], 参数说明: "参数类型支持各种基本数据类型。如果本参数被省略，则表示将删除被替换数据。"
// 参数<3>: [起始替换位置 SDT_INT], 参数说明: "位置索引可以为正值，1表示第一个字节，2表示第二个字节，依次类推；位置索引也可以为负值，-1表示最后一个字节，-2表示倒数第二个字节，依次类推。如果本参数被省略，默认为1。"
// 参数<4>: [最大替换次数 SDT_INT], 参数说明: "如果本参数被省略，或值为-1，表示不限制替换次数。"
COMMOBJ_EXTERN_C void commobj_ReplaceSubBin_97_commobj(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[1].m_pByte;
    PVOID    arg2 = pArgInf[2].m_pByte;
    INT      arg3 = pArgInf[3].m_int;
    INT      arg4 = pArgInf[4].m_int;

}

