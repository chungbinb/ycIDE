#include "..\include_javalib_header.h"

// 调用格式: SDT_INT (Java本地接口).取静态方法标志符, 命令说明: "取指定类的静态方法(static Method)的方法标志符，该方法由其名称和签名指定。失败返回0。本方法可自动将未初始化的类初始化。调用“静态[对象/空]方法()”系列方法时都将用到本方法返回的静态方法标志符。"
// 参数<1>: 类标志符 SDT_INT, 参数说明: "“欲取其静态方法的”类的标志符，可为“查询类()”的返回值。"
// 参数<2>: 静态方法名称 SDT_TEXT, 参数说明: "欲调用的静态方法名称"
// 参数<3>: 静态方法签名 SDT_TEXT, 参数说明: "欲调用的方法签名。使用Java中的“类型签名(Type Signature)”协助前一参数“静态方法名称”唯一标识欲调用的类方法——Java类方法可能被重载，仅“方法名称”不能唯一标识之。关于“类型签名”的更多信息，请点击易语言[工具]菜单“JavaLib: 查询Java类的类型签名”；或参考相关文档。"
JAVALIB_EXTERN_C void javalib_GetStaticMethodID_36_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    LPSTR    arg2 = pArgInf[2].m_pText;
    LPSTR    arg3 = pArgInf[3].m_pText;

}

