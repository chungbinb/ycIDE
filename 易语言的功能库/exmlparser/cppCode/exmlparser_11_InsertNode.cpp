#include "..\include_exmlparser_header.h"

// 调用格式: SDT_BOOL (XML树).插入节点, 命令说明: "新建一个节点，并把该参数节点作为该新节点的父节点，然后插入到XML树中。该方法执行成功返回真，执行失败返回假"
// 参数<1>: 父节点全路径 SDT_TEXT, 参数说明: "本参数为要插入节点的父节点在解析树中的全路径，格式有两种形式，一种为通过“/”字符把节点名称串联起来，例如“根节点/子节点/叶子节点”，另一种是采用索引的形式，索引的最小值为1，表示它是它父节点的第几个子节点，然后在索引前面加上@符号，例如“@1/@2/@1”，另外这两种形式可以混合使用，例如“根节点/@1/@2”"
// 参数<2>: 新节点名 SDT_TEXT, 参数说明: "本参数提供“插入节点”命令所需要的新插入节点的名称"
// 参数<3>: 新节点值 SDT_TEXT, 参数说明: "本参数作为“插入节点”命令所需要的新插入节点的值"
// 参数<4>: [使用CDATA SDT_BOOL], 参数说明: "如果本参数为真，则节点值文本将被“<![CDATA[”和“]]>”包围，这意味着该文本可以包含任意字符（以不影响CDATA解析为限）。如果被省略，默认为假。"
EXMLPARSER_EXTERN_C void exmlparser_InsertNode_11_exmlparser(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;
    LPSTR    arg3 = pArgInf[3].m_pText;
    BOOL     arg4 = pArgInf[4].m_bool;

}

