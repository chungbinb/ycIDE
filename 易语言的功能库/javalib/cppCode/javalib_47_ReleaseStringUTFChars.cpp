#include "..\include_javalib_header.h"

// 本命令被隐藏, 原始名字 = "释放UTF字符串"
// 调用格式: SDT_BOOL (Java本地接口).释放UTF字符串, 命令说明: "释放由“取UTF字符串文本()”方法所返回的文本。成功返回“真”，失败返回“假”。注：内存释放由JAVA虚拟机负责，用户不必介入，故通常忽略本方法的返回值。"
// 参数<1>: 字符串标志符 SDT_INT, 参数说明: "指定欲释放其字符串文本的Java字符串对象"
// 参数<2>: 欲释放的文本 SDT_TEXT, 参数说明: "该文本应是“取UTF字符串文本()”方法的返回值。"
JAVALIB_EXTERN_C void javalib_ReleaseStringUTFChars_47_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    LPSTR    arg2 = pArgInf[2].m_pText;

}

