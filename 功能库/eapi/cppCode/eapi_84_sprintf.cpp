#include "..\include_eapi_header.h"

// 调用格式: _SDT_ALL 格式化字符串, 命令说明: "将一个格式化的字符串输出到一个目的字符串中。给定unicode会返回unicode，此命令与c中sprintf相同，请遵守格式化规则，不把ansi和unicode混用"
// 参数<1>: 格式化字符串 _SDT_ALL, 参数说明: "设置自动运行时的程序名称。"
// 参数<2>: 格式化字符串 _SDT_ALL, 参数说明: "设置自动运行时的程序名称。"
// 变长参数, 命令参数表中最后一个参数可以被重复添加
EAPI_EXTERN_C void eapi_sprintf_84_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[0].m_pByte;
    PVOID    arg2 = pArgInf[1].m_pByte;

}

