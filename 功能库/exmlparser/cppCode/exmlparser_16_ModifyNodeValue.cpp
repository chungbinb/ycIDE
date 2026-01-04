#include "..\include_exmlparser_header.h"

// 调用格式: SDT_BOOL (XML树).修改节点值, 命令说明: "该方法会修改指定节点的值。该方法执行成功返回真，执行失败返回假"
// 参数<1>: 节点全路径 SDT_TEXT, 参数说明: "本参数为节点在解析树中的全路径，格式有两种形式：一种为通过“/”字符把节点名称串联起来，例如“根节点/子节点/叶子节点”；另一种是采用索引的形式，索引的最小值为1，表示它是它父节点的第几个子节点，然后在索引前面加上@符号，例如“@1/@2/@1”。这两种形式可以混合使用，例如“根节点/@1/@2”。"
// 参数<2>: 新节点值 SDT_TEXT, 参数说明: "本参数提供“修改节点值”命令欲修改节点的新值"
// 参数<3>: [使用CDATA SDT_BOOL], 参数说明: "如果本参数为真，则节点值文本将被“<![CDATA[”和“]]>”包围，这意味着该文本可以包含任意字符（以不影响CDATA解析为限）。如果被省略，默认为假。"
EXMLPARSER_EXTERN_C void exmlparser_ModifyNodeValue_16_exmlparser(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;
    BOOL     arg3 = pArgInf[3].m_bool;

}

