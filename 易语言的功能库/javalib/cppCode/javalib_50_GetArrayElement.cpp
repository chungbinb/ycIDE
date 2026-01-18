#include "..\include_javalib_header.h"

// 调用格式: SDT_BOOL (Java本地接口).取数组成员, 命令说明: "取出指定数组中指定成员并存放到本方法的第三个参数中。成功返回“真”，失败返回“假”。"
// 参数<1>: 数组标志符 SDT_INT, 参数说明: "指定要取其数据成员的Java数组。为“创建数组()”方法的返回值，注意不要混用不同数据类型的数组。"
// 参数<2>: 成员索引 SDT_INT, 参数说明: "指定欲取其值的数组成员的索引。索引值从0开始。"
// 参数<3>: &取出的值 _SDT_ALL, 参数说明: "存放从数组中取出的值。注意：要保证本参数的数据类型与数组所能容纳的数据类型一致。"
JAVALIB_EXTERN_C void javalib_GetArrayElement_50_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    PVOID    arg3 = pArgInf[3].m_pByte;

}

