#include "..\include_exmlparser_header.h"

// 调用格式: SDT_BOOL (XML树).取根节点名, 命令说明: "取得当前解析树根节点的名称。该方法执行成功返回真，执行失败返回假"
// 参数<1>: &根节点名称 SDT_TEXT, 参数说明: "本参数作为“取根节点名”命令的填充参数。“取根节点名”命令执行完毕后，该参数的内容为当前解析树根节点的名称。如果命令执行失败，该参数的值无意义"
EXMLPARSER_EXTERN_C void exmlparser_GetRootNodeCaption_4_exmlparser(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR*   arg1 = pArgInf[1].m_ppText;

}

