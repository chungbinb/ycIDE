#include "..\include_javalib_header.h"

// 调用格式: SDT_BOOL (Java虚拟机).创建, 命令说明: "创建JAVA虚拟机，并同时设置“用户类路径(classpath)”和“本地库路径(librarypath)”。如果不需要特别指定“用户类路径”或“本地库路径”，不必显式调用本方法，“Java虚拟机”会在必要的时机（进入对象作用域时）自动创建。成功返回“真”，失败返回“假”。"
// 参数<1>: [用户类路径 SDT_TEXT], 参数说明: "指定java.class.path属性，各路径间以半角分号;分隔，如“c:\\myclasses;d:\\xx.jar”。"
// 参数<2>: [本地库路径 SDT_TEXT], 参数说明: "指定java.library.path属性，各路径间以半角分号;分隔，如“c:\\myclasses”。此类目录下一般存放实现了JNI接口的dll或so文件。"
JAVALIB_EXTERN_C void javalib_Create_1_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;

}

