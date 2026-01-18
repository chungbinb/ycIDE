#include "..\include_commobj_header.h"

// 调用格式: SDT_BOOL (快速字节集对象).插入, 命令说明: "在指定的插入位置之前插入指定的字节集数据"
// 参数<1>: 插入位置 SDT_INT, 参数说明: "位置索引可以为正值，1表示第一个字节，2表示第二个字节，依次类推；位置索引也可以为负值，-1表示最后一个字节，-2表示倒数第二个字节，依次类推。"
// 参数<2>: 欲插入的数据 数组/非数组 _SDT_ALL, 参数说明: "参数类型支持各种基本数据类型及其数组数据。"
// 变长参数, 命令参数表中最后一个参数可以被重复添加
COMMOBJ_EXTERN_C void commobj_Insert_88_commobj(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void*    arg2 = pArgInf[2].m_pAryData;

}

