#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_INT (位图).创建兼容位图, 命令说明: "根据指定窗口的设备环境（DC）创建出一个兼容位图（设备相关位图），并返回其位图句柄（HBITMAP），失败返回0。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。\r\n    本命令通常用于和API交互，一般用户不推荐使用。注意：该命令成功创建的位图句柄在不再需要时☆必须☆调用 DeleteObject API函数删除之！！否则可能造成GDI资源泄露，产生意外错误。\r\n    该API在易语言中的声明如下：\r\n\t.DLL命令 删除对象, 逻辑型, , \"DeleteObject\"\r\n\t.参数 GDI对象句柄, 整数型\r\n    直接把以上两行复制粘贴到易语言DLL命令区即可使用该API"
// 参数<1>: [窗口句柄 SDT_INT], 参数说明: "所创建出来的位图将和该窗口的设备环境（DC）兼容。如果本参数省略，默认使用桌面窗口句柄"
BMPOPERATE_EXTERN_C void bmpoperate_CreateCompitableBitmap_26_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

