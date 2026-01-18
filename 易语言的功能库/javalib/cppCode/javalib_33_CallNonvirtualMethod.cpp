#include "..\include_javalib_header.h"

// 调用格式: SDT_BOOL (Java本地接口).非虚拟方法, 命令说明: "根据指定的类和方法标志符调用某对象的具有基本数据类型返回值的实例（非静态）方法。调用成功返回“真”，失败返回“假”。"
// 参数<1>: 对象标志符 SDT_INT, 参数说明: "指定欲调用其方法的对象。"
// 参数<2>: 类标志符 SDT_INT, 参数说明: "指定欲调用其方法的类。可为“加载类()”等方法的返回值。"
// 参数<3>: 方法标志符 SDT_INT, 参数说明: "指定欲调用的方法。应为“取方法标志符()”方法的返回值。"
// 参数<4>: &返回结果 _SDT_ALL, 参数说明: "存放方法调用后的返回值。注意，提供的变量的数据类型要与方法的实际返回值数据类型一致。"
// 参数<5>: [参数值 _SDT_ALL], 参数说明: "对应于被调用方法的参数值，注意传入的类型要与该方法实际需要的类型一致。本参数可重复添加。"
// 变长参数, 命令参数表中最后一个参数可以被重复添加
JAVALIB_EXTERN_C void javalib_CallNonvirtualMethod_33_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;
    PVOID    arg4 = pArgInf[4].m_pByte;
    PVOID    arg5 = pArgInf[5].m_pByte;

}

