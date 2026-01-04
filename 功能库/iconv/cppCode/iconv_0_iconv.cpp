#include "..\include_iconv_header.h"

// 调用格式: SDT_BIN 编码转换, 命令说明: "将指定的数据从一种编码转换为另一种编码，返回转换后的数据。如果执行失败，将返回空字节集，同时设置参数“执行结果”为“假”。本命令相当于“编码转换_打开()”“编码转换_转换()”“编码转换_关闭()”三个命令的组合使用。"
// 参数<1>: 被转换数据 SDT_BIN, 参数说明: NULL
// 参数<2>: 转换前的数据编码 SDT_TEXT, 参数说明: "可以使用本库中定义的以“编码_”开头的编码常量。编码名称不区分字母大小写。注意：如果指定的编码与“被转换数据”的实际编码不符，很可能导致编码转换失败。"
// 参数<3>: 转换后的数据编码 SDT_TEXT, 参数说明: "可以使用本库中定义的以“编码_”开头的编码常量。编码名称不区分字母大小写。"
// 参数<4>: [&执行结果 SDT_BOOL], 参数说明: "如果提供本参数，其中将被写入本命令的执行结果——执行成功时为真，执行失败时为假。"
ICONV_EXTERN_C void iconv_iconv_0_iconv(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[0].m_pBin;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;
    BOOL*    arg4 = pArgInf[3].m_pBool;

}

