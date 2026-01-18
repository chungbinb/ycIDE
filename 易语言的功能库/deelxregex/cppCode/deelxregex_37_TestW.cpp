#include "..\include_deelxregex_header.h"

// 调用格式: SDT_BOOL (正则表达式DEELX).测试匹配W, 命令说明: "测试表达式与匹配文本是否完全匹配，成功返回真，失败返回假。该方法常用于判断用户输入数据的合法性，比如检验Email的合法性。"
// 参数<1>: [用来匹配的文本 SDT_BIN], 参数说明: "Unicode格式，进行匹配的字符串"
DEELXREGEX_EXTERN_C void deelxregex_TestW_37_deelxregex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[1].m_pBin;

}

