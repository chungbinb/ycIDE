#include "..\include_javalib_header.h"

// 调用格式: SDT_INT (Java本地接口).创建对象数组, 命令说明: "创建一个可以容纳Java对象(object)的数组。返回创建后的数组标志符，失败返回0。"
// 参数<1>: 数组大小 SDT_INT, 参数说明: NULL
// 参数<2>: 类标志符 SDT_INT, 参数说明: "指定该数组用于存放何种“类(class)”。"
// 参数<3>: 初始对象标志符 SDT_INT, 参数说明: "指定该数组所有成员的初始值。"
JAVALIB_EXTERN_C void javalib_NewObjectArray_52_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;

}

