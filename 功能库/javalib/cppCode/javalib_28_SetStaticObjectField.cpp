#include "..\include_javalib_header.h"

// 调用格式: SDT_BOOL (Java本地接口).置静态对象成员, 命令说明: "设置对象静态成员的值。成功返回“真”，失败返回“假”。"
// 参数<1>: 对象标志符 SDT_INT, 参数说明: "指定欲取其成员(Field)的类。该标志符不应为0。"
// 参数<2>: 成员标志符 SDT_INT, 参数说明: "欲取其值的成员。该标志符不应为0。"
// 参数<3>: 对象标志符 SDT_INT, 参数说明: "指定欲赋于该成员的对象。"
JAVALIB_EXTERN_C void javalib_SetStaticObjectField_28_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;

}

