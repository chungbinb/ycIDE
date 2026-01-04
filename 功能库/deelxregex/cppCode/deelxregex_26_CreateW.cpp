#include "..\include_deelxregex_header.h"

// 调用格式: SDT_BOOL (正则表达式DEELX).创建W, 命令说明: "根据正则表达式语法，对正则表达式文本进行编译。"
// 参数<1>: 正则表达式文本 SDT_BIN, 参数说明: "Unicode格式的正则表达式文本，比如“易语言5\\.0(模块|支持库)?”"
// 参数<2>: [匹配模式 SDT_INT], 参数说明: "支持的匹配模式有：单行模式、多行模式、全局模式、忽略大小写、从右向左、扩展模式 这 6 种模式以及它们的组合。\r\n可以用如下常量（#正则常量.单行模式；#正则常量.多行模式；#正则常量.全局模式；#正则常量.忽略大小写；#正则常量.从右向左；#正则常量.扩展模式）可以查看支持库常量说明"
DEELXREGEX_EXTERN_C void deelxregex_CreateW_26_deelxregex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[1].m_pBin;
    INT      arg2 = pArgInf[2].m_int;

}

