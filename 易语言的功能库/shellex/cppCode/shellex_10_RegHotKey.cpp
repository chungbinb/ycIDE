#include "..\include_shellex_header.h"

// 调用格式: SDT_INT 注册热键, 命令说明: "注册系统热键，返回一个热键标识，失败返回0。"
// 参数<1>: 窗口句柄 SDT_INT, 参数说明: "窗口句柄"
// 参数<2>: 标签句柄 SDT_INT, 参数说明: "使用标签的反馈事件来接受热键，反馈事件中的第一个参数为热键标识，第二个参数无效"
// 参数<3>: 功能键 SDT_INT, 参数说明: "可以为：0-无功能键；1-CTRL键状态；2-SHIFT键状态；4-ALT键状态或各键状态值之和"
// 参数<4>: 主热键 SDT_INT, 参数说明: "键代码，可以使用易语言中的键代码常量"
SHELLEX_EXTERN_C void shellex_RegHotKey_10_shellex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;
    INT      arg4 = pArgInf[3].m_int;

}

