#include "..\include_dp1_header.h"

// 调用格式: SDT_BIN 解密数据, 命令说明: "解密一段加密后的字节集数据，返回解密后的结果字节集。注意本命令并不对密码文本进行校验，如果密码提供错误，将返回错误的结果。如果失败，返回空字节集"
// 参数<1>: 字节集数据 SDT_BIN, 参数说明: "为命令提供所需的字节集数据"
// 参数<2>: 密码文本 SDT_TEXT, 参数说明: NULL
// 参数<3>: [加密算法 SDT_INT], 参数说明: "指定具体使用的加密算法，可以为以下常量值之一：1: #DES算法； 2: #RC4算法。加密和解密必须使用相同的算法，有关算法的具体说明请参阅有关文献。如果本参数被省略，则默认值为1，即DES算法"
DP1_EXTERN_C void dp1_Decrypt_4_dp1(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[0].m_pBin;
    LPSTR    arg2 = pArgInf[1].m_pText;
    INT      arg3 = pArgInf[2].m_int;

}

