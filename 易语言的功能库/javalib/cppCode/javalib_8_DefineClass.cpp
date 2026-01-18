#include "..\include_javalib_header.h"

// 调用格式: SDT_INT (Java本地接口).字节集加载类, 命令说明: "从包含原始类数据(.class文件内容)的字节集中加载类，返回加载后的Java类标志符。如果出错则返回0。JNI不允许重复加载同一个类。"
// 参数<1>: 类名称 SDT_TEXT, 参数说明: "使用带包名称的类时，可用字符.或/分隔，如：“java.lang.String”“java/lang/String”等。"
// 参数<2>: 类加载器 SDT_INT, 参数说明: "分派给所定义的类的类加载器（的对象标志符）。注：如果本参数为0，默认使用系统类加载器java.lang.ClassLoader.getSystemClassLoader()。"
// 参数<3>: 类文件数据 SDT_BIN, 参数说明: "包含 .class 文件数据的字节集数据。"
JAVALIB_EXTERN_C void javalib_DefineClass_8_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    INT      arg2 = pArgInf[2].m_int;
    LPBYTE   arg3 = pArgInf[3].m_pBin;

}

