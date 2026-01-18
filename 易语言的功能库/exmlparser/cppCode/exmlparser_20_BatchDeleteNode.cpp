#include "..\include_exmlparser_header.h"

// 调用格式: SDT_BOOL (XML树).批量删除节点, 命令说明: "该方法要删除节点的路径参数可以支持通配符（*,?），该方法会删除所有符合路径条件的节点和该节点的子节点，该方法不允许删除根节点。注意索引形式的路径参数不支持通配符。该方法执行成功返回真，执行失败返回假"
// 参数<1>: 节点全路径 SDT_TEXT, 参数说明: "本参数为节点在解析树中的全路径，格式有两种形式：一种为通过“/”字符把节点名称串联起来，例如“根节点/子节点/叶子节点”；另一种是采用索引的形式，索引的最小值为1，表示它是它父节点的第几个子节点，然后在索引前面加上@符号，例如“@1/@2/@1”。这两种形式可以混合使用，例如“根节点/@1/@2”。注意该参数支持通配符（*, ?）——仅在路径中使用节点名称时支持通配符，使用索引时不支持通配符。"
EXMLPARSER_EXTERN_C void exmlparser_BatchDeleteNode_20_exmlparser(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

