#include "..\include_opengl_header.h"

// 调用格式: SDT_BOOL 载入位图数据, 命令说明: "成功载入返回真，失败返回假"
// 参数<1>: 位图文件名 SDT_TEXT, 参数说明: "允许使用1位、4位、8位、16位、24位、32位的翻转行序或非翻转行序的非压缩BMP位图文件格式"
// 参数<2>: [背景色 SDT_INT], 参数说明: "位图中指定为背景色的区域，其颜色值的通道部分将被定义为0。如果为空，将被定义为1。可以通过使用“通道混合测试”进行处理。\r\n\t如果希望将背景色部分显示为透明，可以执行：\r\n\t  启用功能 (#通道测试)\r\n\t  通道混合测试 (#大于, 0)\r\n\t  设置纹理映射方式 (#纹理映射模式, #调节处理)"
OPENGL_EXTERN_C void opengl_auxDIBImageLoad_67_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    INT      arg2 = pArgInf[1].m_int;

}

