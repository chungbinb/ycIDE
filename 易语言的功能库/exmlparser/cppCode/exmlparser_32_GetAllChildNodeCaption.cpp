#include "..\include_exmlparser_header.h"

// 调用格式: SDT_TEXT (XML树).[]取所有子节点名, 命令说明: "取得由参数节点的所有子节点名称组成的文本数组。"
// 参数<1>: 节点全路径 SDT_TEXT, 参数说明: "本参数为节点在解析树中的全路径，格式有两种形式：一种为通过“/”字符把节点名称串联起来，例如“根节点/子节点/叶子节点”；另一种是采用索引的形式，索引的最小值为1，表示它是它父节点的第几个子节点，然后在索引前面加上@符号，例如“@1/@2/@1”。这两种形式可以混合使用，例如“根节点/@1/@2”。"
// 参数<2>: [&执行结果 SDT_BOOL], 参数说明: "如果提供本参数，其中将存放本方法的执行结果——成功为“真”，失败为“假”。"
EXMLPARSER_EXTERN_C void exmlparser_GetAllChildNodeCaption_32_exmlparser(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    BOOL*    arg2 = pArgInf[2].m_pBool;

}

