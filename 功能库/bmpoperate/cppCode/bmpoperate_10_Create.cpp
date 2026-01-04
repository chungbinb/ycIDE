#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_BOOL (位图).创建, 命令说明: "创建一个指定大小、位深度的位图，本对象内原有数据会被自动释放。成功返回真，失败返回假。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。"
// 参数<1>: 宽度 SDT_INT, 参数说明: "位图的实际宽度"
// 参数<2>: 高度 SDT_INT, 参数说明: "位图的实际高度"
// 参数<3>: 位深度 SDT_SHORT, 参数说明: "位图的颜色位深度。参看“取位深度”命令"
// 参数<4>: [初始颜色 SDT_INT], 参数说明: "位图的初始颜色。如果本参数被省略，则新建的位图内包含的颜色数据不会被初始化，这样可以加快创建的速度，但是在使用本位图前可能需要手动进行必要的初始化。"
BMPOPERATE_EXTERN_C void bmpoperate_Create_10_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    SHORT    arg3 = pArgInf[3].m_short;
    INT      arg4 = pArgInf[4].m_int;

}

