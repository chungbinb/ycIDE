#include "..\include_exmlparser_header.h"

// 调用格式: SDT_BOOL (XML树).取属性值, 命令说明: "取得参数节点的特定属性的值。该方法执行成功返回真，执行失败返回假"
// 参数<1>: 节点全路径 SDT_TEXT, 参数说明: "本参数为节点在解析树中的全路径，格式有两种形式：一种为通过“/”字符把节点名称串联起来，例如“根节点/子节点/叶子节点”；另一种是采用索引的形式，索引的最小值为1，表示它是它父节点的第几个子节点，然后在索引前面加上@符号，例如“@1/@2/@1”。这两种形式可以混合使用，例如“根节点/@1/@2”。"
// 参数<2>: 属性名 SDT_TEXT, 参数说明: "本参数提供“取属性值”命令所需要的节点属性的名称。该参数也可以是属性的索引，例如“@1”表示节点的第一个属性"
// 参数<3>: &属性值 SDT_TEXT, 参数说明: "本参数作为“取属性值”命令的填充参数。“取属性值”命令执行完毕后，该参数内容为节点属性的值。如果命令执行失败，该参数的值无意义"
EXMLPARSER_EXTERN_C void exmlparser_GetNodeAttrValue_10_exmlparser(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;
    LPSTR*   arg3 = pArgInf[3].m_ppText;

}

