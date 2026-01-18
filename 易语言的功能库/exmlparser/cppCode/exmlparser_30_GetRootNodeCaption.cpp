#include "..\include_exmlparser_header.h"

// 调用格式: SDT_TEXT (XML树).取根节点名文本, 命令说明: "取得当前解析树根节点的名称文本。"
// 参数<1>: [&执行结果 SDT_BOOL], 参数说明: "如果提供本参数，其中将存放本方法的执行结果——成功为“真”，失败为“假”。"
EXMLPARSER_EXTERN_C void exmlparser_GetRootNodeCaption_30_exmlparser(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    BOOL*    arg1 = pArgInf[1].m_pBool;

}

