#include "..\include_commobj_header.h"

// 调用格式: SDT_BOOL (快速字节集对象).从文件读, 命令说明: "从指定的文件中读取其所有数据到本对象中。"
// 参数<1>: 文件名 SDT_TEXT, 参数说明: "请提供包含完整路径的文本名称。"
COMMOBJ_EXTERN_C void commobj_ReadFromFile_126_commobj(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

