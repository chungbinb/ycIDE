#include "..\include_regex_header.h"

// 本命令被隐藏, 原始名字 = "复制构造函数", 本命令为复制构造函数
// 调用格式: _SDT_NULL (正则表达式).复制构造函数, 命令说明: "“正则表达式”类型的复制构造函数。隐藏"
// 参数<1>: 源对象 MAKELONG(0x01, 0), 参数说明: NULL
REGEX_EXTERN_C void regex_CopyConstructor_13_regex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

