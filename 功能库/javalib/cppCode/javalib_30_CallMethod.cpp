#include "..\include_javalib_header.h"

// 调用格式: SDT_BOOL (Java本地接口).方法, 命令说明: "调用对象的具有基本数据类型返回值方法。调用成功返回“真”，失败返回“假”。"
// 参数<1>: 对象标志符 SDT_INT, 参数说明: "指定欲调用其方法的对象。"
// 参数<2>: 方法标志符 SDT_INT, 参数说明: "指定欲调用的方法。应为“取方法标志符()”方法的返回值。"
// 参数<3>: &返回结果 _SDT_ALL, 参数说明: "存放方法调用后的返回值。注意，提供的变量的数据类型要与方法的实际返回值数据类型一致。"
// 参数<4>: [参数值 _SDT_ALL], 参数说明: "对应于被调用方法的参数值，注意传入的类型要与该方法实际需要的类型一致。本参数可重复添加。"
// 变长参数, 命令参数表中最后一个参数可以被重复添加
JAVALIB_EXTERN_C void javalib_CallMethod_30_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    PVOID    arg3 = pArgInf[3].m_pByte;
    PVOID    arg4 = pArgInf[4].m_pByte;

}

