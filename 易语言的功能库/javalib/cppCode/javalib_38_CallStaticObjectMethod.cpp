#include "..\include_javalib_header.h"

// 调用格式: SDT_BOOL (Java本地接口).静态对象方法, 命令说明: "调用类的具有对象型返回值的静态方法。调用成功返回“真”，失败返回“假”。"
// 参数<1>: 类标志符 SDT_INT, 参数说明: "指定欲调用其静态方法的类。可为“加载类()”等方法的返回值。"
// 参数<2>: 方法标志符 SDT_INT, 参数说明: "指定欲调用的静态方法。应为“取静态方法标志符()”方法的返回值。"
// 参数<3>: &返回结果 SDT_INT, 参数说明: "存放静态方法调用后的返回值——对象标志符。"
// 参数<4>: [参数值 _SDT_ALL], 参数说明: "对应于被调用静态方法的参数值，注意传入的类型要与该方法实际需要的类型一致。本参数可重复添加。"
// 变长参数, 命令参数表中最后一个参数可以被重复添加
JAVALIB_EXTERN_C void javalib_CallStaticObjectMethod_38_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    PINT     arg3 = pArgInf[3].m_pInt;
    PVOID    arg4 = pArgInf[4].m_pByte;

}

