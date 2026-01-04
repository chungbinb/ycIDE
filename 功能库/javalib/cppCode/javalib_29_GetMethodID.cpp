#include "..\include_javalib_header.h"

// 调用格式: SDT_INT (Java本地接口).取方法标志符, 命令说明: "返回类或接口实例方法（非静态方法）的方法标志符，该方法由其名称及签名指定。失败返回0。方法可在参数“类标志符”所指定的类的超类中定义。本方法可自动将未初始化的类初始化。要获得构造函数的方法标志符，应将<init>作为方法名，同时将void (V)作为返回类型。调用“[非虚拟][对象/空]方法()”系列方法时都将用到本方法返回的方法标志符。"
// 参数<1>: 类标志符 SDT_INT, 参数说明: "指定欲取其方法的类。可为“加载类()”等方法的返回值。"
// 参数<2>: 方法名称 SDT_TEXT, 参数说明: "欲调用的方法名称"
// 参数<3>: 方法签名 SDT_TEXT, 参数说明: "欲调用的方法签名。使用Java中的“类型签名(Type Signature)”协助前一参数“方法名称”唯一标识欲调用的类方法——Java类方法可能被重载，仅“方法名称”不能唯一标识之。关于“类型签名”的更多信息，请点击易语言[工具]菜单“JavaLib: 查询Java类的类型签名”；或参考相关文档。"
JAVALIB_EXTERN_C void javalib_GetMethodID_29_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    LPSTR    arg2 = pArgInf[2].m_pText;
    LPSTR    arg3 = pArgInf[3].m_pText;

}

