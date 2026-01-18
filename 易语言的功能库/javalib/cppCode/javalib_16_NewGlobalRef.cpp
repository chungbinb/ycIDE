#include "..\include_javalib_header.h"

// 调用格式: SDT_INT (Java本地接口).创建全局引用, 命令说明: "创建指定对象的新全局引用。返回创建后的全局引用对象标志符，失败返回0。全局引用需要通过调用“销毁全局引用()”方法来销毁。"
// 参数<1>: 被引用的对象标志符 SDT_INT, 参数说明: "该参数所指定的对象既可以是全局引用，也可以是局部引用。"
JAVALIB_EXTERN_C void javalib_NewGlobalRef_16_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

