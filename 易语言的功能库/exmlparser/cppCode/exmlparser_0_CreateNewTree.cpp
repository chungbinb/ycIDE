#include "..\include_exmlparser_header.h"

// 调用格式: SDT_BOOL (XML树).创建, 命令说明: "在内存中创建一个新的XML的解析树。如果XML树已经创建，则自动释放当前XML树，然后执行创建操作。该方法执行成功返回真，执行失败返回假"
// 参数<1>: 根节点名 SDT_TEXT, 参数说明: "本参数提供所要新建解析树根节点的名称"
EXMLPARSER_EXTERN_C void exmlparser_CreateNewTree_0_exmlparser(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

