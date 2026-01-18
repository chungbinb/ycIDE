#include "..\include_commobj_header.h"

// 调用格式: SDT_INT (快速字节集对象).取缓冲区, 命令说明: "返回指定大小的缓冲区内存首地址，该地址同时也是内部数据指针。如果不能获取足够大小的缓冲区，将返回0。可以向该缓冲区直接写入数据，注意写入数据时请不要写到缓冲区之外。如果改变了缓冲区中数据的长度，必须及时调用“释放缓冲区()”。"
// 参数<1>: [缓冲区大小 SDT_INT], 参数说明: "应大于零并小于“最大可用内存”。如果本参数被省略，默认为当前数据长度。"
COMMOBJ_EXTERN_C void commobj_GetBuffer_130_commobj(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

