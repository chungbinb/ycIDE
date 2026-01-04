#include "..\include_esslayer_header.h"

// 调用格式: _SDT_NULL (保密客户端).置加密方式, 命令说明: "设置数据网络传输过程中的加密方式，如果没有经过该项设置默认的加密方式为des方法"
// 参数<1>: 加密方式 SDT_INT, 参数说明: "该值为0代表采用rc4方法加密，该值为1代表采用des方法加密"
ESSLAYER_EXTERN_C void esslayer_SetEncryptStyle_16_esslayer(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

