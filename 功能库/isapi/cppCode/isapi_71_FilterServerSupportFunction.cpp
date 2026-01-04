#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网筛选器).调用扩展函数, 命令说明: "执行成功返回真，否则返回假"
// 参数<1>: 欲调用的函数 SDT_INT, 参数说明: "可以为枚举类型常量 #筛选器扩展函数 的成员之一"
// 参数<2>: 参数1 SDT_INT, 参数说明: "视调用的函数不同，此参数含义也不相同"
// 参数<3>: 参数2 SDT_INT, 参数说明: "视调用的函数不同，此参数含义也不相同"
// 参数<4>: 参数3 SDT_INT, 参数说明: "视调用的函数不同，此参数含义也不相同"
ISAPI_EXTERN_C void isapi_FilterServerSupportFunction_71_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;
    INT      arg4 = pArgInf[4].m_int;

}

