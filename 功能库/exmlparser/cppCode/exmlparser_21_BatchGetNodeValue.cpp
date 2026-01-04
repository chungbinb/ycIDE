#include "..\include_exmlparser_header.h"

// 调用格式: SDT_BOOL (XML树).批量取节点值, 命令说明: "该方法要取值节点的路径参数可以支持通配符（*,?），该方法会取所有符合路径条件的节点的值。注意索引形式的路径参数不支持通配符。该方法执行成功返回真，执行失败返回假"
// 参数<1>: 节点全路径 SDT_TEXT, 参数说明: "本参数为节点在解析树中的全路径，格式有两种形式：一种为通过“/”字符把节点名称串联起来，例如“根节点/子节点/叶子节点”；另一种是采用索引的形式，索引的最小值为1，表示它是它父节点的第几个子节点，然后在索引前面加上@符号，例如“@1/@2/@1”。这两种形式可以混合使用，例如“根节点/@1/@2”。注意该参数支持通配符（*, ?）——仅在路径中使用节点名称时支持通配符，使用索引时不支持通配符。"
// 参数<2>: &批量节点值 数组 SDT_TEXT, 参数说明: "本参数作为“批量取节点值”命令的填充参数。“批量取节点值”命令执行完毕后，该参数内容为节点值的数组。如果命令执行失败，该参数的值无意义"
EXMLPARSER_EXTERN_C void exmlparser_BatchGetNodeValue_21_exmlparser(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    void**   arg2 = pArgInf[2].m_ppAryData;

}

