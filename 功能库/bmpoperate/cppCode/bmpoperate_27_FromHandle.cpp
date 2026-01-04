#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_BOOL (位图).从句柄创建, 命令说明: "从一个位图句柄（HBITMAP）创建出设备无关位图，该位图原有数据会被自动释放。成功返回真，失败返回假。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。注意：本方法创建出来的位图的颜色质量可能会受到当前屏幕显示模式的影响"
// 参数<1>: 位图句柄 SDT_INT, 参数说明: "一个位图句柄（HBITMAP）"
BMPOPERATE_EXTERN_C void bmpoperate_FromHandle_27_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

