#include "..\include_ecalc_header.h"

// 调用格式: SDT_BYTE (大数).取字符, 命令说明: "获取本对象所表示的数字文本在任意位置的数字字符，指定的序号参数必须大于等于1且小于等于字符串长度"
// 参数<1>: 字符位置序号 SDT_INT, 参数说明: "要获取的字符在整个数字文本中的位置，序号从\"1\"开始。如果本参数指定的序号在数字文本中不存在，那么本方法将出错"
ECALC_EXTERN_C void ecalc_GetChar_74_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

