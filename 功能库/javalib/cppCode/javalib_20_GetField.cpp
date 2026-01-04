#include "..\include_javalib_header.h"

// 调用格式: SDT_BOOL (Java本地接口).取成员, 命令说明: "取对象成员的值，结果存到第三个参数“结果”中。成功返回“真”，失败返回“假”。"
// 参数<1>: 对象标志符 SDT_INT, 参数说明: "指定欲取其成员(Field)的类。该标志符不应为0。"
// 参数<2>: 成员标志符 SDT_INT, 参数说明: "欲取其值的成员。该标志符不应为0。"
// 参数<3>: &结果 _SDT_ALL, 参数说明: "存放取出的成员值。注意：要提供与成员的数据类型相一致的变量。"
JAVALIB_EXTERN_C void javalib_GetField_20_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    PVOID    arg3 = pArgInf[3].m_pByte;

}

