#include "..\include_ecalc_header.h"

// 调用格式: SDT_BOOL (大数).导入数字, 命令说明: "导入当前易语言支持的所有格式的数字为当前数据类型的数据，以前的数据将被舍弃，成功返回真，失败返回假，注意：如果您导入的数据是小数有可能产生误差"
// 参数<1>: 数字 _SDT_ALL, 参数说明: "该参数可以是易语言兼容的所有数字格式数据，例如:字节型，短整数型，整数型，长整数型，小数型，双精度小数型"
ECALC_EXTERN_C void ecalc_ImportNum_69_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[1].m_pByte;

}

