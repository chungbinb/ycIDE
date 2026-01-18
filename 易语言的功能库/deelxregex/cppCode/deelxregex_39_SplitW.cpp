#include "..\include_deelxregex_header.h"

// 调用格式: SDT_BIN (正则表达式DEELX).[]分割W, 命令说明: "使用指定的正则表达式将指定文本进行分割。返回分割后的一维字节集数组。如果失败则返回一个空数组，即没有任何成员的数组。"
// 参数<1>: 待分割文本 SDT_BIN, 参数说明: "Unicode格式文本"
// 参数<2>: [要返回的子文本数目 SDT_INT], 参数说明: "如果被省略或等于0，则默认返回所有的子文本"
DEELXREGEX_EXTERN_C void deelxregex_SplitW_39_deelxregex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[1].m_pBin;
    INT      arg2 = pArgInf[2].m_int;

}

