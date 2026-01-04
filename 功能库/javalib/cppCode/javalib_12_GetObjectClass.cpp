#include "..\include_javalib_header.h"

// 调用格式: SDT_INT (Java本地接口).取对象类, 命令说明: "返回“对象所属类”的类标志符，失败返回0。"
// 参数<1>: 对象标志符 SDT_INT, 参数说明: "指定欲取其类的对象。该标志符不应为0。"
JAVALIB_EXTERN_C void javalib_GetObjectClass_12_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

