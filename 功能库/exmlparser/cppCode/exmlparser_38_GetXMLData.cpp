#include "..\include_exmlparser_header.h"

// 调用格式: SDT_BIN (XML树).取XML数据, 命令说明: "取当前XML树的全部数据，并转换为字节集后返回。如果执行失败，返回值为空字节集。本方法所返回的数据与“导出到文件()”所写入文件的数据完全等同。"
// 参数<1>: [字符集 SDT_TEXT], 参数说明: "指定欲使用的XML字符集属性，默认为“gb18030”。注意，本参数仅用于对“<?xml ... ?>”节点的“encoding”属性赋值，不据此进行编码转换。"
// 参数<2>: [&执行结果 SDT_BOOL], 参数说明: "如果提供本参数，其中将存放本方法的执行结果——成功为“真”，失败为“假”。"
// 参数<3>: [换行文本 SDT_TEXT], 参数说明: "用于格式化XML文本，默认为 #换行符。注意，如果此参数不为空文本，导出又导入后，会对有子节点的节点值产生影响。"
// 参数<4>: [缩进文本 SDT_TEXT], 参数说明: "用于格式化XML文本，默认为TAB键字符文本。注意，如果此参数不为空文本，导出又导入后，会对有子节点的节点值产生影响。"
EXMLPARSER_EXTERN_C void exmlparser_GetXMLData_38_exmlparser(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    BOOL*    arg2 = pArgInf[2].m_pBool;
    LPSTR    arg3 = pArgInf[3].m_pText;
    LPSTR    arg4 = pArgInf[4].m_pText;

}

