#include "..\include_ecalc_header.h"

// 调用格式: SDT_BOOL (大数).导入文本, 命令说明: "导入存储数字的文本型的数据为本对象的当前数据，该文本型必须为存储合法数字格式的文本，该数字可以是任何长度和精度，本对象以前存储的数据将被舍弃，成功返回真，失败返回假"
// 参数<1>: 数字文本 SDT_TEXT, 参数说明: "合法的文本格式的数字数据，可以是任意精度，该数字将被转换成本数据类型特有的数字储存格式，并参与计算"
ECALC_EXTERN_C void ecalc_ImportString_67_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

