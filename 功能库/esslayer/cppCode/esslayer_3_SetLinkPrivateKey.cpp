#include "..\include_esslayer_header.h"

// 调用格式: SDT_BOOL (保密服务器).置连接私钥, 命令说明: "设置服务器连接的过程中使用的私钥和公共模数，公钥和公共模数是公开的。服务程序一旦定下来私钥和公钥，并把公钥公开后，这项连接密钥将不再改变，否则客户使用服务程序公开给他的公钥将不能再连接到服务程序上。加解密采用非对称的RSA加解密方法。该项设置服务器端的私钥和公共模数，客户端掌握公钥和公共模数，这样客户端和服务器端在连接成功后立刻使用该密钥进行验证，以确认通讯的双方是否可以相互信任，如果验证不成功，立刻断开连接。客户端掌握的公钥和公共模数和服务器的私钥和公共模数是相匹配的。设置成功返回真，否则返回假"
// 参数<1>: 私钥文本 SDT_TEXT, 参数说明: "为十六进制数值文本，为了生成符合要求的随机密钥，可以下载使用 http://dywt.com.cn/RSATool2v14.rar 第三方工具软件（内有操作说明）"
// 参数<2>: 公共模数 SDT_TEXT, 参数说明: "为十六进制数值文本"
ESSLAYER_EXTERN_C void esslayer_SetLinkPrivateKey_3_esslayer(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;

}

