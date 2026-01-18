#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 模板测试, 命令说明: "为模板运算设定影响范围。必须启用“#模板测试”，才能执行测试。不使用的时候需要关闭。"
// 参数<1>: 测试方式 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t512、#失效，始终为假；\r\n\t513、#小于，如果测试值的测试位小于输入值的测试位时为真；\r\n\t514、#等于，如果测试值的测试位等于输入值的测试位时为真；\r\n\t515、#小于等于，如果测试值的测试位小于等于输入值的测试位时为真；\r\n\t516、#大于，如果测试值的测试位大于输入值的测试位时为真；\r\n\t517、#不等于，如果测试值的测试位不等于输入值的测试位时为真；\r\n\t518、#大于等于，如果测试值的测试位大于等于输入值的测试位时为真；\r\n\t519、#有效，始终为真"
// 参数<2>: 测试位 SDT_INT, 参数说明: "需要测试模板的位"
// 参数<3>: 测试值 SDT_INT, 参数说明: NULL
OPENGL_EXTERN_C void opengl_glStencilFunc_36_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;

}

