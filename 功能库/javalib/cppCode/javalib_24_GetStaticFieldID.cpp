#include "..\include_javalib_header.h"

// 调用格式: SDT_INT (Java本地接口).取静态成员标志符, 命令说明: "返回类的静态成员(static Field)的成员标志符，该成员由其名称及签名指定。失败返回0。本方法可自动将未初始化的类初始化。调用“取静态成员()”“置静态成员()”系列方法时都将用到本方法返回的静态成员标志符。"
// 参数<1>: 类标志符 SDT_INT, 参数说明: "指定欲取其成员(Field)标志符的类。该标志符不应为0。"
// 参数<2>: 成员名称 SDT_TEXT, 参数说明: "欲取其标志符的成员名称。"
// 参数<3>: 成员签名 SDT_TEXT, 参数说明: "欲取其标志符的成员签名。使用Java中的“类型签名(Type Signature)”协助前一参数“成员名称”唯一标识欲访问的成员。关于“类型签名”的更多信息，请点击易语言[工具]菜单“JavaLib: 查询Java类的类型签名”；或参考相关文档。"
JAVALIB_EXTERN_C void javalib_GetStaticFieldID_24_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    LPSTR    arg2 = pArgInf[2].m_pText;
    LPSTR    arg3 = pArgInf[3].m_pText;

}

