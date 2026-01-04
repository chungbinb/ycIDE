#include "..\include_commobj_header.h"

// 调用格式: SDT_BOOL (快速文本对象).写到文件, 命令说明: "将本对象中的所有数据写入指定的文件中。如果该文件不存在，则自动创建；如果该文件已经存在，其原有内容将被直接覆盖！"
// 参数<1>: 文件名 SDT_TEXT, 参数说明: "请提供包含完整路径的文本名称。"
COMMOBJ_EXTERN_C void commobj_WriteToFile_67_commobj(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

