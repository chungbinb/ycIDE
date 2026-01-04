#include "..\include_ewizard_header.h"

// 调用格式: _SDT_NULL 置组件属性, 命令说明: NULL
// 参数<1>: 组件属性名 SDT_TEXT, 参数说明: "组件属性名以“窗口名.组件名.属性名”或“窗口名.属性名”格式表示，注意窗口组件包括菜单项"
// 参数<2>: 属性值 _SDT_ALL, 参数说明: "属性值的数据类型必须与指定组件属性的数据类型一致"
EWIZARD_EXTERN_C void ewizard_SetCtrlProperty_6_ewizard(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    PVOID    arg2 = pArgInf[1].m_pByte;

}

