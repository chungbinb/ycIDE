#include "..\include_javalib_header.h"

// 调用格式: SDT_INT (Java本地接口).创建对象, 命令说明: "创建Java对象。返回创建后的对象标志符，失败返回0。"
// 参数<1>: 类标志符 SDT_INT, 参数说明: "指定欲创建其对象的Java类。该标志符可通过调用“加载类()”等方法获得。"
// 参数<2>: 构造函数的方法标志符 SDT_INT, 参数说明: "指定应调用的构造函数方法。该标志符必须通过调用“取方法标志符()”方法获得，且调用时的方法名必须为<init>，而返回类型必须为void (V)。"
// 参数<3>: [参数值 SDT_INT], 参数说明: "对应于被调用的Java类构造函数的参数值，注意传入的类型要与实际需要的类型一致。本参数可重复添加。"
// 变长参数, 命令参数表中最后一个参数可以被重复添加
JAVALIB_EXTERN_C void javalib_NewObject_11_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;

}

