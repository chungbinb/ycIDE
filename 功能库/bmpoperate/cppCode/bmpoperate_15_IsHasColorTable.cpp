#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_BOOL (位图).是否使用颜色表, 命令说明: "位深度小于16的位图使用颜色表，否则不使用。调用本命令后可能产生三种情况：\r\n\t1. 位图尚未创建/载入（即位图为空）。返回假，且设置“错误信息”为非空文本，随后调用“取错误信息”将取回该错误说明信息。\r\n\t2. 位图已经创建，该位图使用颜色表。返回真。\r\n\t3. 位图已经创建，该位图不使用颜色表。返回假。如果随后调用“取错误信息”将返回空文本。"
// 无参数
BMPOPERATE_EXTERN_C void bmpoperate_IsHasColorTable_15_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}

