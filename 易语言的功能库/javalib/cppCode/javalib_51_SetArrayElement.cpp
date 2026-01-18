#include "..\include_javalib_header.h"

// 调用格式: SDT_BOOL (Java本地接口).置数组成员, 命令说明: "成功返回“真”，失败返回“假”。"
// 参数<1>: 数组标志符 SDT_INT, 参数说明: "指定要对其数据成员赋值的Java数组。可为“创建数组()”方法的返回值，注意不要混用不同基本数据类型的数组。"
// 参数<2>: 成员索引 SDT_INT, 参数说明: "指定欲对其赋值的数组成员的索引。索引值从0开始。"
// 参数<3>: 值 _SDT_ALL, 参数说明: "欲赋给指定数组成员的值。注意：要保证本参数的数据类型与数组所能容纳的数据类型一致。"
JAVALIB_EXTERN_C void javalib_SetArrayElement_51_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    PVOID    arg3 = pArgInf[3].m_pByte;

}

