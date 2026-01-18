#include "..\include_dp1_header.h"

// 调用格式: SDT_BOOL 签名验证, 命令说明: "使用指定公钥和签署结果文本对被签署字节集数据进行验证，如验证通过则说明该字节集数据必定是此公钥拥有者（即签署者）所认可的，且签署后未经任何篡改。验证通过返回真，否则返回假"
// 参数<1>: 待验证被签署字节集 SDT_BIN, 参数说明: NULL
// 参数<2>: 签署结果文本 SDT_TEXT, 参数说明: "必须为“数字签名”命令所返回的结果"
// 参数<3>: 签署者公钥文本 SDT_TEXT, 参数说明: "为十六进制数值文本"
// 参数<4>: 签署者公共模数 SDT_TEXT, 参数说明: "为十六进制数值文本"
DP1_EXTERN_C void dp1_RSACheck_6_dp1(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[0].m_pBin;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;
    LPSTR    arg4 = pArgInf[3].m_pText;

}

