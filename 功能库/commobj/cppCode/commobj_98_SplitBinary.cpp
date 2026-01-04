#include "..\include_commobj_header.h"

// 调用格式: SDT_BIN (快速字节集对象).[]分割字节集, 命令说明: NULL
// 参数<1>: [用作分隔的字节集 SDT_BIN], 参数说明: "如果被省略，则默认使用字节 0 作为分隔符。如果是一个长度为零的文本，则返回的数组仅包含一个成员，其内容是本对象中的文本。"
// 参数<2>: [要返回的子文本数目 SDT_INT], 参数说明: "如果为-1，表示不限制替换次数。如果本参数被省略，默认为-1。"
COMMOBJ_EXTERN_C void commobj_SplitBinary_98_commobj(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[1].m_pBin;
    INT      arg2 = pArgInf[2].m_int;

}

