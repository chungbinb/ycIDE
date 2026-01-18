#include "..\include_javalib_header.h"

// 调用格式: SDT_BOOL (Java本地接口).对象方法, 命令说明: "调用对象的具有对象型返回值的方法。调用成功返回“真”，失败返回“假”。"
// 参数<1>: 对象标志符 SDT_INT, 参数说明: "指定欲调用其方法的对象。"
// 参数<2>: 方法标志符 SDT_INT, 参数说明: "指定欲调用的方法。应为“取方法标志符()”方法的返回值。"
// 参数<3>: &返回结果 SDT_INT, 参数说明: "存放方法调用后的返回值——对象标志符。"
// 参数<4>: [参数值 _SDT_ALL], 参数说明: "对应于被调用方法的参数值，注意传入的类型要与该方法实际需要的类型一致。本参数可重复添加。"
// 变长参数, 命令参数表中最后一个参数可以被重复添加
JAVALIB_EXTERN_C void javalib_CallObjectMethod_31_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    PINT     arg3 = pArgInf[3].m_pInt;
    PVOID    arg4 = pArgInf[4].m_pByte;

}

