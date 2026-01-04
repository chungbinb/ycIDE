#include "..\include_exmlparser_header.h"

// 调用格式: SDT_BOOL (XML树).导入, 命令说明: "从XML文件或字节集中导入数据，并根据该数据重建XML树（如果XML树已经创建，则自动释放当前XML树，然后执行导入操作）。该方法执行成功返回真，执行失败返回假。本方法将对导入的数据进行必要的语法检查，如果发现非法的数据格式，将直接返回假。"
// 参数<1>: XML文件名或字节集 _SDT_ALL, 参数说明: "指定欲导入的XML文件名称或欲导入的XML字节集数据。如果参数类型为“文本型”，则从文件导入（文件名可以是绝对路径名，也可以是相对路径名）；如果参数类型为“字节集型”，则从字节集导入；其它类型不予处理。"
EXMLPARSER_EXTERN_C void exmlparser_LoadFrom_1_exmlparser(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[1].m_pByte;

}

