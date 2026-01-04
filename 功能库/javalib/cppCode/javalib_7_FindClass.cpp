#include "..\include_javalib_header.h"

// 调用格式: SDT_INT (Java本地接口).加载类, 命令说明: "根据类名称加载指定的Java类。它将搜索由classpath环境变量中指定的目录和jar文件，以及调用“Java虚拟机.创建()”方法时指定的“用户类路径”和“本地库路径”。返回加载后的Java类标志符。如果出错则返回0。"
// 参数<1>: 要加载的类名称 SDT_TEXT, 参数说明: "必须使用完整类名称，可用字符.或/分隔，如：“java.lang.String”“java/lang/String”等。"
JAVALIB_EXTERN_C void javalib_FindClass_7_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

