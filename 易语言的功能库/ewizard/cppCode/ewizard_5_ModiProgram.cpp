#include "..\include_ewizard_header.h"

// 调用格式: _SDT_NULL 修改程序, 命令说明: NULL
// 参数<1>: 欲修改项类型 MAKELONG(0x01, 0), 参数说明: NULL
// 参数<2>: 欲修改项名称 SDT_TEXT, 参数说明: NULL
// 参数<3>: 欲修改属性 MAKELONG(0x02, 0), 参数说明: "合法的修改属性与与修改项类型相关，具体如下：\r\n程序集： 名称、备注、公开\r\n子程序： 名称、备注、数据类型、公开、收缩\r\n全局变量、程序集变量、局部变量、子程序参数、数据类型成员、DLL命令参数： 名称、备注、数据类型、数组类型、静态、参考、可空、公开\r\n数据类型： 名称、备注、公开\r\nDLL命令： 名称、备注、公开、DLL库文件名、DLL库命令名、数据类型\r\n窗口： 名称、备注\r\n窗口组件： 名称、备注\r\n常量或资源： 名称、备注、公开、常量或资源值\r\n"
// 参数<4>: 修改值 _SDT_ALL, 参数说明: "修改值的数据类型必须与指定欲修改属性的数据类型一致，修改值具体所需要的数据类型请见“程序项属性”中各成员常量的解释"
EWIZARD_EXTERN_C void ewizard_ModiProgram_5_ewizard(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pCompoundData;
    LPSTR    arg2 = pArgInf[1].m_pText;
    void*    arg3 = pArgInf[2].m_pCompoundData;
    PVOID    arg4 = pArgInf[3].m_pByte;

}

