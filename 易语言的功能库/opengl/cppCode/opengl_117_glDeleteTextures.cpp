#include "..\include_opengl_header.h"

// 调用格式: SDT_INT 删除纹理号, 命令说明: "删除已经创建的纹理号，与纹理号相应的纹理将无法使用。返回实际删除的纹理号的数量"
// 参数<1>: [需要删除的纹理号数量 SDT_INT], 参数说明: "默认删除纹理数组中所有的纹理号，否则删除数组中指定的前几项。"
// 参数<2>: 纹理号数组 数组/非数组 SDT_INT, 参数说明: "保存了纹理号的数组。"
OPENGL_EXTERN_C void opengl_glDeleteTextures_117_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    void*    arg2 = pArgInf[1].m_pAryData;

}

