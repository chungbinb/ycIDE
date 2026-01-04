#include "..\include_iconv_header.h"

// 调用格式: SDT_INT 编码转换_打开, 命令说明: "初始化编码转换环境，返回一个句柄，供后续“编码转换_转换()”命令使用。如果本命令返回值为-1，表示执行失败。本命令返回的有效句柄必须使用“编码转换_关闭()”命令关闭。"
// 参数<1>: 转换前的数据编码 SDT_TEXT, 参数说明: "可以使用本库中定义的以“编码_”开头的编码常量。编码名称不区分字母大小写。注意：如果指定的编码与“被转换数据”的实际编码不符，很可能导致编码转换失败。"
// 参数<2>: 转换后的数据编码 SDT_TEXT, 参数说明: "可以使用本库中定义的以“编码_”开头的编码常量。编码名称不区分字母大小写。"
ICONV_EXTERN_C void iconv_iconv_open_1_iconv(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR    arg2 = pArgInf[1].m_pText;

}

