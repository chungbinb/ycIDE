#include "..\include_exmlparser_header.h"

// 调用格式: SDT_BOOL (XML树).导出到文件, 命令说明: "保存当前XML树的全部数据到指定的文件。该方法执行成功返回真，执行失败返回假。注：可使用“取XML数据()”获取相同的字节集数据而不必保存到文件。"
// 参数<1>: XML文件名 SDT_TEXT, 参数说明: "欲导出的XML文件名称。该文件名可以是绝对路径名，也可以是相对路径名。注意：如果指定的文件已经存在，原有文件内容将被覆盖。"
// 参数<2>: [字符集 SDT_TEXT], 参数说明: "指定欲使用的XML字符集属性，默认为“gb18030”。注意，本参数仅用于对“<?xml ... ?>”节点的“encoding”属性赋值，不据此进行编码转换。"
// 参数<3>: [换行文本 SDT_TEXT], 参数说明: "用于格式化XML文本，默认为 #换行符。注意，如果此参数不为空文本，导出又导入后，会对有子节点的节点值产生影响。"
// 参数<4>: [缩进文本 SDT_TEXT], 参数说明: "用于格式化XML文本，默认为TAB键字符文本。注意，如果此参数不为空文本，导出又导入后，会对有子节点的节点值产生影响。"
EXMLPARSER_EXTERN_C void exmlparser_WriteToFile_2_exmlparser(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;
    LPSTR    arg3 = pArgInf[3].m_pText;
    LPSTR    arg4 = pArgInf[4].m_pText;

}

