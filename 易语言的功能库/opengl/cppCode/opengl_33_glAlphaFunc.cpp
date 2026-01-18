#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 通道混合测试, 命令说明: "当比较函数返回真的时候才显示出效果来。只有在红绿蓝混合通道模式下，启用“#通道测试”后才有效。不使用的时候需要关闭。"
// 参数<1>: 比较函数 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t512、#失效，始终为假；\r\n\t513、#小于，如果输入值小于测试值时为真；\r\n\t514、#等于，如果输入值等于测试值时为真；\r\n\t515、#小于等于，如果输入值小于等于测试值时为真；\r\n\t516、#大于，如果输入值大于测试值时为真；\r\n\t517、#不等于，如果输入值不等于测试值时为真；\r\n\t518、#大于等于，如果输入值大于等于测试值时为真；\r\n\t519、#有效，始终为真"
// 参数<2>: 测试值 SDT_FLOAT, 参数说明: "范围在[0，1]内的数值。否则无效"
OPENGL_EXTERN_C void opengl_glAlphaFunc_33_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    FLOAT    arg2 = pArgInf[1].m_float;

}

