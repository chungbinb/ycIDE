#include "..\include_opengl_header.h"

// 调用格式: SDT_BOOL 载入3DS文件, 命令说明: "载入以3DS为后缀的三维模型文件，并编译为一个显示列表。如果3DS文件中包含有纹理图片，那么只有BMP位图格式的纹理才可以被使用。否则无法显示纹理。使用“调用列表”命令可以显示模型。成功返回真，失败返回假。"
// 参数<1>: 3DS文件名 SDT_TEXT, 参数说明: "需要载入的3DS文件名称。"
// 参数<2>: 列表号 SDT_INT, 参数说明: "可以直接指定列表号或者使用由“生成列表号”取得的返回值，必须是大于0的整数。"
// 参数<3>: &存放被取回纹理号的数组变量 数组 SDT_INT, 参数说明: "在命令执行完毕后，本变量数组内被顺序填入在载入3DS文件时候创建的纹理号。变量数组内原有数据被全部销毁，变量数组的维数被自动调整为一维数组。"
OPENGL_EXTERN_C void opengl_glLoad3DS_103_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    INT      arg2 = pArgInf[1].m_int;
    void**   arg3 = pArgInf[2].m_ppAryData;

}

