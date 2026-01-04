#include "..\include_javalib_header.h"

// 本命令被隐藏, 原始名字 = "创建UTF字符串"
// 调用格式: SDT_INT (Java本地接口).创建UTF字符串, 命令说明: "使用易语言文本(UTF-8格式)构造新的java.lang.String对象，返回创建后的字符串对象标志符。失败返回0。"
// 参数<1>: 文本 SDT_TEXT, 参数说明: "将被转换到Java字符串的易语言文本(UTF-8格式)"
JAVALIB_EXTERN_C void javalib_NewStringUTF_44_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

