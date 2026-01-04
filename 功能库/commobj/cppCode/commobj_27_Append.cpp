#include "..\include_commobj_header.h"

// 调用格式: SDT_BOOL (快速文本对象).添加, 命令说明: "将参数转换成文本并添加到本对象文本的末尾。"
// 参数<1>: 欲添加的数据 数组/非数组 _SDT_ALL, 参数说明: "参数类型支持各种基本数据类型及其数组数据。"
// 变长参数, 命令参数表中最后一个参数可以被重复添加
COMMOBJ_EXTERN_C void commobj_Append_27_commobj(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;

}

