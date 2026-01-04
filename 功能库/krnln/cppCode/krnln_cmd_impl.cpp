#include "include_krnln_header.h"

// 系统核心支持库命令实现
// 自动生成于 2026-01-04 10:36:10
// 命令总数: 672

// 注意: 此文件包含所有命令的占位实现
// 需要根据实际功能逐个实现

// 命令 0: 如果
// 类别: 流程控制
// 说明: 本命令根据提供的逻辑参数的值，来决定是否改变程序的执行位置，如果提供的逻辑参数值为真，程序继续顺序向下执行，否则跳转到左侧箭头线指向的命令处去执行。本命令为初级...
EXTERN_C void KRNLN_NAME(0, ife)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 1: 如果真
// 类别: 流程控制
// 说明: 本命令根据提供的逻辑参数的值，来决定是否改变程序的执行位置，如果提供的逻辑参数值为真，程序继续顺序向下执行，否则跳转到左侧箭头线指向的命令处去执行。同“如果”命...
EXTERN_C void KRNLN_NAME(1, if_)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 2: 判断
// 类别: 流程控制
// 说明: 本命令根据提供的逻辑参数的值，来决定是否改变程序的执行位置，如果提供的逻辑参数值为真，程序继续顺序向下执行，否则跳转到下一分支处去继续判断。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(2, switch_)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 3: 判断循环首
// 类别: 流程控制
// 说明: 本命令根据提供的逻辑参数的值，来决定是否进入循环。如果提供的逻辑参数值为真，程序顺序执行下一条命令进入循环，否则跳转到本命令所对应的“判断循环尾”命令的下一条命...
EXTERN_C void KRNLN_NAME(3, while_)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 4: 判断循环尾
// 类别: 流程控制
// 说明: 本命令已经被隐藏。 执行本命令将返回到对应的“判断循环首”命令处继续进行循环。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(4, wend)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 5: 循环判断首
// 类别: 流程控制
// 说明: 执行本命令后将顺序执行下一条命令进入程序循环。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(5, DoWhile)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 6: 循环判断尾
// 类别: 流程控制
// 说明: 本命令已经被隐藏。 本命令根据提供的逻辑参数的值，来决定是否返回到循环首部继续进行循环。如果提供的逻辑参数值为真，程序返回到对应的“循环判断首”命令处继续进行循...
EXTERN_C void KRNLN_NAME(6, loop)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 7: 计次循环首
// 类别: 流程控制
// 说明: 执行本命令将把循环体内的命令重复执行指定的次数。提供的整数参数值决定将重复执行的次数。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(7, counter)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 8: 计次循环尾
// 类别: 流程控制
// 说明: 本命令已经被隐藏。 如果计次循环的循环次数未到，执行本命令将返回到对应的“计次循环首”命令处继续进行循环，否则将顺序执行下一条命令退出本计次循环。本命令为初级命...
EXTERN_C void KRNLN_NAME(8, CounterLoop)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 9: 变量循环首
// 类别: 流程控制
// 说明: 执行本命令将利用变量对循环体内的命令进行循环执行。第一次执行此命令时将使用“变量起始值”参数初始化“循环变量”参数所指定的变量。每次（包括第一次）执行到此命令处...
EXTERN_C void KRNLN_NAME(9, for_)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 10: 变量循环尾
// 类别: 流程控制
// 说明: 本命令已经被隐藏。 将在对应的“变量循环首”命令中所指定的数值变量递增或递减指定的值后返回到循环首部继续执行。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(10, next)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 11: 到循环尾
// 类别: 流程控制
// 说明: 本命令转移当前程序执行位置到当前所处循环体的循环尾语句处。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(11, continue_)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 12: 跳出循环
// 类别: 流程控制
// 说明: 本命令转移当前程序执行位置到当前所处循环体循环尾语句的下一条语句处。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(12, break_)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 13: 返回
// 类别: 流程控制
// 说明: 本命令转移当前程序执行位置到调用本子程序语句的下一条语句处,并可根据需要返回一个值到子程序调用处。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(13, return_)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 14: 结束
// 类别: 流程控制
// 说明: 本命令结束当前易程序的运行。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(14, end)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 15: 相乘
// 类别: 算术运算
// 说明: 返回两个数值的乘积，运算符号为“*”或“×”。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(15, mul)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 16: 相除
// 类别: 算术运算
// 说明: 返回两个数值的商，运算符号为“/”或“÷”。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(16, div)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 17: 整除
// 类别: 算术运算
// 说明: 求出两个数值的商，并返回其整数部分，运算符号为“\”。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(17, IDiv)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 18: 求余数
// 类别: 算术运算
// 说明: 求出两个数值的商，并返回余数部分，运算符号为“%”或“Mod”。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(18, mod)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 19: 相加
// 类别: 算术运算
// 说明: 运算符号为“+”，用途为：1、返回两个数值的和；2、将两个文本首尾连接起来，返回连接后的文本；3、将两个字节集首尾连接起来，返回连接后的字节集。本命令为初级命令...
EXTERN_C void KRNLN_NAME(19, add)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 20: 相减
// 类别: 算术运算
// 说明: 返回两个数值的差，运算符号为“-”。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(20, sub)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 21: 负
// 类别: 算术运算
// 说明: 反转一个数值的正负符号并返回反转后的值，运算符号为“-”（放在表达式首）。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(21, neg)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 22: 取符号
// 类别: 算术运算
// 说明: 返回一个整数，如果小于零，表明给定数值为负；如果等于零，表明给定数值为零；如果大于零，表明给定数值为正。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(22, sgn)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 23: 取绝对值
// 类别: 算术运算
// 说明: 如果所提供数值为字节型，则将直接返回该数值。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(23, abs)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 24: 取整
// 类别: 算术运算
// 说明: 返回一个小数的整数部分。本命令与“绝对取整”命令不相同之处为： 如果给定小数为负数，则本命令返回小于或等于该小数的第一个负整数，而“绝对取整”命令则会返回大于或...
EXTERN_C void KRNLN_NAME(24, int_)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 25: 绝对取整
// 类别: 算术运算
// 说明: 返回一个小数的整数部分。本命令与“取整”命令不相同之处为： 如果给定小数为负数，则本命令返回大于或等于该小数的第一个负整数，而“取整”命令则会返回小于或等于该小...
EXTERN_C void KRNLN_NAME(25, fix)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 26: 四舍五入
// 类别: 算术运算
// 说明: 返回按照指定的方式进行四舍五入运算的结果数值。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(26, round)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 27: 求次方
// 类别: 算术运算
// 说明: 返回指定数值的指定次方。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(27, pow)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 28: 求平方根
// 类别: 算术运算
// 说明: 返回指定参数的平方根。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(28, sqr)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 29: 求正弦
// 类别: 算术运算
// 说明: 返回指定角的正弦值。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(29, sin)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 30: 求余弦
// 类别: 算术运算
// 说明: 返回指定角的余弦值。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(30, cos)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 31: 求正切
// 类别: 算术运算
// 说明: 返回指定角的正切值。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(31, tan)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 32: 求反正切
// 类别: 算术运算
// 说明: 返回指定数的反正切值。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(32, atn)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 33: 求自然对数
// 类别: 算术运算
// 说明: 返回指定参数的自然对数值。自然对数是以 e 为底的对数。常量 #e 的值大约是 2.718282。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(33, log)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 34: 求反对数
// 类别: 算术运算
// 说明: 返回 e（自然对数的底）的某次方。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(34, exp)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 35: 是否运算正确
// 类别: 算术运算
// 说明: 对乘、除、“求次方”、“求平方根”、“求正弦值”、“求余弦值”、“求正切值”、“求反正切值”、“求自然对数”、“求反对数”等等数学运算命令所计算出来的双精度结果...
EXTERN_C void KRNLN_NAME(35, IsCalcOK)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 36: 置随机数种子
// 类别: 算术运算
// 说明: 为随机数生成器初始化一个种子值，不同的种子值将导致“取随机数”命令返回不同的随机数系列。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(36, randomize)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 37: 取随机数
// 类别: 算术运算
// 说明: 返回一个指定范围内的随机数值。在使用本命令取一系列的随机数之前，应该先使用“置随机数种子”命令为随机数生成器初始化一个种子值。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(37, rnd)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 38: 等于
// 类别: 逻辑比较
// 说明: 被比较值与比较值相同时返回真，否则返回假，运算符号为“=”或“==”。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(38, cmd_38)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 39: 不等于
// 类别: 逻辑比较
// 说明: 被比较值与比较值不相同时返回真，否则返回假，运算符号为“<>”或“!=”或“≠”。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(39, cmd_39)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 40: 小于
// 类别: 逻辑比较
// 说明: 被比较值小于比较值时返回真，否则返回假，运算符号为“<”。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(40, cmd_40)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 41: 大于
// 类别: 逻辑比较
// 说明: 被比较值大于比较值时返回真，否则返回假，运算符号为“>”。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(41, cmd_41)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 42: 小于或等于
// 类别: 逻辑比较
// 说明: 被比较值小于或等于比较值时返回真，否则返回假，运算符号为“<=”或“≤”。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(42, cmd_42)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 43: 大于或等于
// 类别: 逻辑比较
// 说明: 被比较值大于或等于比较值时返回真，否则返回假，运算符号为“>=”或“≥”。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(43, cmd_43)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 44: 近似等于
// 类别: 逻辑比较
// 说明: 当比较文本在被比较文本的首部被包容时返回真，否则返回假，运算符号为“?=”或“≈”。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(44, like)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 45: 并且
// 类别: 逻辑比较
// 说明: 如果所提供的两个参数逻辑值都为真则返回真，否则返回假，运算符号为“&&”或“And”或“且”。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(45, and)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 46: 或者
// 类别: 逻辑比较
// 说明: 如果所提供的两个参数逻辑值中任意有一个为真则返回真，否则返回假，运算符号为“||”或“Or”或“或”。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(46, or)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 47: 取反
// 类别: 逻辑比较
// 说明: 如果参数值为真则返回假，如果参数值为假则返回真。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(47, not)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 48: 位取反
// 类别: 位运算
// 说明: 将指定数值每一个比特位的值取反后返回。本命令为中级命令。...
EXTERN_C void KRNLN_NAME(48, bnot)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 49: 位与
// 类别: 位运算
// 说明: 如两个数值有某共同比特位为1，则返回值的对应位也为1，否则为0。返回计算后的结果。本命令为中级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(49, band)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 50: 位或
// 类别: 位运算
// 说明: 如两个数值中有一个数值的某一比特位不为零，则返回值的对应位就为1，否则为0。返回计算后的结果。本命令为中级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(50, bor)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 51: 位异或
// 类别: 位运算
// 说明: 如两个数值对应比特位的值不相等，则返回值的对应位就为1，否则为0。返回计算后的结果。本命令为中级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(51, bxor)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 52: 左移
// 类别: 位运算
// 说明: 将某整数的数据位左移指定位数，返回移动后的结果。本命令为中级命令。...
EXTERN_C void KRNLN_NAME(52, shl)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 53: 右移
// 类别: 位运算
// 说明: 将某整数的数据位右移指定位数，返回移动后的结果。本命令为中级命令。...
EXTERN_C void KRNLN_NAME(53, shr)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 54: 合并整数
// 类别: 位运算
// 说明: 将第一个整数的低16位放置到结果整数的低16位，将第二个整数的低16位放置到结果整数的高16位，以此合并成一个整数，并返回合并后的结果。本命令为中级命令。...
EXTERN_C void KRNLN_NAME(54, MakeLong)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 55: 合并短整数
// 类别: 位运算
// 说明: 将第一个整数的低8位放置到结果短整数的低8位，将第二个整数的低8位放置到结果短整数的高8位，以此合并成一个短整数，并返回合并后的结果。本命令为中级命令。...
EXTERN_C void KRNLN_NAME(55, MakeWord)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 56: 赋值
// 类别: 变量操作
// 说明: 将指定的常数、常数集、常量、资源、对象或者变量赋于到指定的变量或变量数组中去。运算符号为“=”。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(56, set)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 57: 连续赋值
// 类别: 变量操作
// 说明: 将指定的常数、常数集、常量、资源、对象或者变量赋于到一系列变量或变量数组中去。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(57, store)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 58: 重定义数组
// 类别: 数组操作
// 说明: 本命令可以重新定义指定数组的维数及各维的上限值。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(58, ReDim)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 59: 取数组成员数
// 类别: 数组操作
// 说明: 取指定数组变量的全部成员数目，如果该变量不为数组，返回-1，因此本命令也可以用作检查指定变量是否为数组变量。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(59, GetAryElementCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 60: 取数组下标
// 类别: 数组操作
// 说明: 返回指定数组维可用的最大下标（最小下标固定为1）。如果给定变量不为数组变量或指定维不存在，返回 0 。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(60, UBound)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 61: 复制数组
// 类别: 数组操作
// 说明: 将数组数据复制到指定的数组变量，该数组变量内的所有数据和数组维定义信息将被全部覆盖。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(61, CopyAry)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 62: 加入成员
// 类别: 数组操作
// 说明: 将数据加入到指定数组变量的尾部，并通过重新定义数组维数自动增加其成员数目。数组变量如为多维数组，加入完毕后将被转换为单维数组。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(62, AddElement)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 63: 插入成员
// 类别: 数组操作
// 说明: 将数据插入到指定数组变量的指定位置，并通过重新定义数组维数自动增加其成员数目。数组变量如为多维数组，插入完毕后将被转换为单维数组。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(63, InsElement)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 64: 删除成员
// 类别: 数组操作
// 说明: 删除指定数组变量中的成员，并通过重新定义数组维数自动减少其成员数目。数组变量如为多维数组，删除完毕后将被转换为单维数组。返回所实际删除的成员数目。本命令为初级命...
EXTERN_C void KRNLN_NAME(64, RemoveElement)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 65: 清除数组
// 类别: 数组操作
// 说明: 删除指定数组变量中的所有成员，释放这些成员所占用的存储空间，重新定义该变量为单维 0 成员数组变量。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(65, RemoveAll)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 66: 数组排序
// 类别: 数组操作
// 说明: 对指定数值数组变量内的所有数组成员进行快速排序，不影响数组的维定义信息，排序结果存放回该数组变量。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(66, SortAry)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 67: 数组清零
// 类别: 数组操作
// 说明: 将指定数值数组变量内的所有成员值全部设置为零，不影响数组的维定义信息。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(67, ZeroAry)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 68: 取命令行
// 类别: 环境存取
// 说明: 本命令可以取出在启动易程序时附加在其可执行文件名后面的所有以空格分隔的命令行文本段。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(68, GetCmdLine)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 69: 取运行目录
// 类别: 环境存取
// 说明: 取当前被执行的易程序文件所处的目录。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(69, GetRunPath)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 70: 取执行文件名
// 类别: 环境存取
// 说明: 取当前被执行的易程序文件的名称。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(70, GetRunFileName)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 71: 读环境变量
// 类别: 环境存取
// 说明: 返回文本，它关连于一个操作系统环境变量。成功时返回所取得的值，失败则返回空文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(71, GetEnv)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 72: 写环境变量
// 类别: 环境存取
// 说明: 修改或建立指定的操作系统环境变量。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(72, PutEnv)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 73: 取所有发音
// 类别: 拼音处理
// 说明: 返回包含指定汉字所有拼音编码的文本数组（支持多音字），目前仅支持国标汉字。返回的数组结果可以用“复制数组”命令保存到同类型数组变量。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(73, GetAllPY)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 74: 取发音数目
// 类别: 拼音处理
// 说明: 返回指定汉字的发音数目。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(74, GetPYCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 75: 取拼音
// 类别: 拼音处理
// 说明: 返回包含指定汉字的指定拼音编码的文本。如果该指定拼音编码不存在，将返回空文本。目前仅支持国标汉字。多音字的第一个发音为常用音。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(75, GetPY)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 76: 取声母
// 类别: 拼音处理
// 说明: 返回包含指定汉字指定拼音编码的声母部分文本。如果指定拼音编码不存在或该汉字此发音无声母，将返回空文本。目前仅支持国标汉字。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(76, GetSM)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 77: 取韵母
// 类别: 拼音处理
// 说明: 返回包含指定汉字指定拼音编码的韵母部分文本。如果指定拼音编码不存在，将返回空文本。目前仅支持国标汉字。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(77, GetYM)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 78: 发音比较
// 类别: 拼音处理
// 说明: 比较两段文本的发音，如果发音相同返回真，否则返回假。对于文本中的非国标汉字部分，将仅进行简单的值比较。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(78, CompPY)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 79: 输入字比较
// 类别: 拼音处理
// 说明: 将一个拼音输入字文本与另外一个普通文本比较，如果两者相符，返回真，否则返回假。本命令为初级命令。 一、首拼及全拼类： 如：“jsj”匹配“计算机”，同时“jis...
EXTERN_C void KRNLN_NAME(79, CompPYCode)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 80: 取文本长度
// 类别: 文本操作
// 说明: 取文本型数据的长度，不包含结束0。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(80, len)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 81: 取文本左边
// 类别: 文本操作
// 说明: 返回一个文本，其中包含指定文本中从左边算起指定数量的字符。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(81, left)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 82: 取文本右边
// 类别: 文本操作
// 说明: 返回一个文本，其中包含指定文本中从右边算起指定数量的字符。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(82, right)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 83: 取文本中间
// 类别: 文本操作
// 说明: 返回一个文本，其中包含指定文本中从指定位置算起指定数量的字符。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(83, mid)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 84: 字符
// 类别: 文本操作
// 说明: 返回一个文本，其中包含有与指定字符代码相关的字符。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(84, chr)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 85: 取代码
// 类别: 文本操作
// 说明: 返回文本中指定位置处字符的代码。如果指定位置超出文本长度，返回0。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(85, asc)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 86: 寻找文本
// 类别: 文本操作
// 说明: 返回一个整数值，指定一文本在另一文本中最先出现的位置，位置值从 1 开始。如果未找到，返回-1。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(86, InStr)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 87: 倒找文本
// 类别: 文本操作
// 说明: 返回一个整数值，指定一文本在另一文本中最后出现的位置，位置值从 1 开始。如果未找到，返回-1。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(87, InStrRev)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 88: 到大写
// 类别: 文本操作
// 说明: 将文本中的小写英文字母变换为大写，返回变换后的结果文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(88, UCase)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 89: 到小写
// 类别: 文本操作
// 说明: 将文本中的大写英文字母变换为小写，返回变换后的结果文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(89, LCase)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 90: 到全角
// 类别: 文本操作
// 说明: 将文本中的半角字母、空格或数字变换为全角，返回变换后的结果文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(90, QJCase)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 91: 到半角
// 类别: 文本操作
// 说明: 将文本中的全角字母、空格或数字变换为半角，返回变换后的结果文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(91, BJCase)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 92: 到文本
// 类别: 文本操作
// 说明: 返回一个文本，代表指定数值、逻辑值或日期时间被转换后的结果。如果为文本数据，将被直接返回。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(92, str)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 93: 删首空
// 类别: 文本操作
// 说明: 返回一个文本，其中包含被删除了首部全角或半角空格的指定文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(93, LTrim)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 94: 删尾空
// 类别: 文本操作
// 说明: 返回一个文本，其中包含被删除了尾部全角或半角空格的指定文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(94, RTrim)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 95: 删首尾空
// 类别: 文本操作
// 说明: 返回一个文本，其中包含被删除了首部及尾部全角或半角空格的指定文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(95, trim)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 96: 删全部空
// 类别: 文本操作
// 说明: 返回一个文本，其中包含被删除了所有全角或半角空格的指定文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(96, TrimAll)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 97: 文本替换
// 类别: 文本操作
// 说明: 将指定文本的某一部分用其它的文本替换。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(97, ReplaceText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 98: 子文本替换
// 类别: 文本操作
// 说明: 返回一个文本，该文本中指定的子文本已被替换成另一子文本，并且替换发生的次数也是被指定的。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(98, RpSubText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 99: 取空白文本
// 类别: 文本操作
// 说明: 返回具有指定数目半角空格的文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(99, space)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 100: 取重复文本
// 类别: 文本操作
// 说明: 返回一个文本，其中包含指定次数的文本重复结果。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(100, string)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 101: 文本比较
// 类别: 文本操作
// 说明: 如果返回值小于0，表示文本一小于文本二；如果等于0，表示文本一等于文本二；如果大于0，表示文本一大于文本二。如果比较时区分大小写，也可以使用比较运算符进行同样的...
EXTERN_C void KRNLN_NAME(101, StrComp)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 102: 分割文本
// 类别: 文本操作
// 说明: 将指定文本进行分割，返回分割后的一维文本数组。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(102, split)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 103: 指针到文本
// 类别: 文本操作
// 说明: 返回指定内存指针所指向地址处的文本，注意调用本命令前一定要确保所提供的内存指针真实有效，且指向一个以零字符结束的文本串。本命令的最佳使用场合就是在易语言回调子程...
EXTERN_C void KRNLN_NAME(103, pstr)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 104: 文本到UTF8
// 类别: 文本操作
// 说明: 将所指定文本转换到UTF8格式后返回,注意所返回UTF8文本数据包括结束零字符.本命令为初级命令。...
EXTERN_C void KRNLN_NAME(104, StrToUTF8)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 105: UTF8到文本
// 类别: 文本操作
// 说明: 将所指定UTF8文本数据转换到通常文本后返回。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(105, UTF8ToStr)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 106: 文本到UTF16
// 类别: 文本操作
// 说明: 将所指定文本转换到UTF16格式后返回,注意所返回UTF16文本数据包括结束零字符.本命令为初级命令。...
EXTERN_C void KRNLN_NAME(106, StrToUTF16)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 107: UTF16到文本
// 类别: 文本操作
// 说明: 将所指定UTF16文本数据转换到通常文本后返回。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(107, UTF16ToStr)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 108: 到时间
// 类别: 时间操作
// 说明: 将指定文本转换为时间并返回。如果给定文本不符合书写格式要求或者时间值错误导致不能进行转换，将返回100年1月1日。如果给定参数本身就是时间数据，将直接返回该时间...
EXTERN_C void KRNLN_NAME(108, ToTime)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 109: 增减时间
// 类别: 时间操作
// 说明: 返回一个时间，这一时间被加上或减去了一段间隔。如果改变后的时间无效，将自动靠近最近的有效时间。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(109, TimeChg)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 110: 取时间间隔
// 类别: 时间操作
// 说明: 返回一个数值，表示“时间1”减去“时间2”之后的间隔数目。注意：每个星期以星期天为第一天。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(110, TimeDiff)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 111: 取某月天数
// 类别: 时间操作
// 说明: 返回指定月份所包含的天数。如果给定的月份无效，返回0。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(111, GetDaysOfSpecMonth)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 112: 时间到文本
// 类别: 时间操作
// 说明: 将指定时间转换为文本并返回。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(112, TimeToText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 113: 取时间部分
// 类别: 时间操作
// 说明: 返回一个包含已知时间指定部分的整数。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(113, TimePart)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 114: 取年份
// 类别: 时间操作
// 说明: 返回一个值为 100 到 9999 之间的整数，表示指定时间中的年份。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(114, year)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 115: 取月份
// 类别: 时间操作
// 说明: 返回一个值为 1 到 12 之间的整数，表示指定时间中的月份。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(115, month)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 116: 取日
// 类别: 时间操作
// 说明: 返回一个值为 1 到 31 之间的整数，表示一个月中的某一日。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(116, day)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 117: 取星期几
// 类别: 时间操作
// 说明: 返回一个值为 1 到 7 之间的整数，表示一个星期中的某一日。星期日为 1，星期一为 2，依此类推。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(117, WeekDay)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 118: 取小时
// 类别: 时间操作
// 说明: 返回一个值为 0 到 23 之间的整数，表示一天中的某一小时。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(118, hour)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 119: 取分钟
// 类别: 时间操作
// 说明: 返回一个值为 0 到 59 之间的整数，表示一小时中的某一分钟。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(119, minute)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 120: 取秒
// 类别: 时间操作
// 说明: 返回一个值为 0 到 59 之间的整数，表示一分钟中的某一秒。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(120, second)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 121: 指定时间
// 类别: 时间操作
// 说明: 返回包含指定年、月、日、小时、分、秒的时间。如果指定了无效时间，将自动使用最相近的有效时间代替。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(121, GetSpecTime)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 122: 取现行时间
// 类别: 时间操作
// 说明: 返回当前系统日期及时间。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(122, now)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 123: 置现行时间
// 类别: 时间操作
// 说明: 设置当前系统日期及时间。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(123, SetSysTime)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 124: 取日期
// 类别: 时间操作
// 说明: 返回一个日期时间型数据的日期部分，其小时、分钟、秒被固定设置为0时0分0秒。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(124, GetDatePart)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 125: 取时间
// 类别: 时间操作
// 说明: 返回一个日期时间型数据的时间部分，其年、月、日被固定设置为2000年1月1日。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(125, GetTimePart)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 126: 到数值
// 类别: 数值转换
// 说明: 返回包含于文本内的数值，文本中是一个适当类型的数值，支持全角书写方式。本命令也可用作将其他类型的数据转换为双精度小数。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(126, val)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 127: 数值到大写
// 类别: 数值转换
// 说明: 将数值转换为简体或繁体的大写形式，返回转换后的文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(127, UNum)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 128: 数值到金额
// 类别: 数值转换
// 说明: 将数值转换为金额的简体或繁体大写形式，返回转换后的文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(128, NumToRMB)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 129: 数值到格式文本
// 类别: 数值转换
// 说明: 返回一个文本，代表指定数值被格式转换后的结果。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(129, NumToText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 130: 取十六进制文本
// 类别: 数值转换
// 说明: 返回一个文本，代表指定数值的十六进制形式。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(130, GetHexText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 131: 取八进制文本
// 类别: 数值转换
// 说明: 返回一个文本，代表指定数值的八进制形式。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(131, GetOctText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 132: 到字节
// 类别: 数值转换
// 说明: 返回包含于文本内的字节值，文本中是一个适当类型的数值，支持全角书写方式。本命令也可用作将其他类型的数据转换为字节。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(132, ToByte)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 133: 到短整数
// 类别: 数值转换
// 说明: 返回包含于文本内的短整数值，文本中是一个适当类型的数值，支持全角书写方式。本命令也可用作将其他类型的数据转换为短整数。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(133, ToShort)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 134: 到整数
// 类别: 数值转换
// 说明: 返回包含于文本内的整数值，文本中是一个适当类型的数值，支持全角书写方式。本命令也可用作将其他类型的数据转换为整数。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(134, ToInt)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 135: 到长整数
// 类别: 数值转换
// 说明: 返回包含于文本内的长整数值，文本中是一个适当类型的数值，支持全角书写方式。本命令也可用作将其他类型的数据转换为长整数。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(135, ToLong)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 136: 到小数
// 类别: 数值转换
// 说明: 返回包含于文本内的小数值，文本中是一个适当类型的数值，支持全角书写方式。本命令也可用作将其他类型的数据转换为小数。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(136, ToFloat)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 137: 十六进制
// 类别: 数值转换
// 说明: 计算返回所指定十六进制文本常量对应的整数值. 本命令在编译时被直接预处理为整数型参量值,不影响程序执行效率.本命令为初级命令。...
EXTERN_C void KRNLN_NAME(137, hex)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 138: 二进制
// 类别: 数值转换
// 说明: 计算返回所指定二进制文本常量对应的整数值. 本命令在编译时被直接预处理为整数型参量值,不影响程序执行效率.本命令为初级命令。...
EXTERN_C void KRNLN_NAME(138, binary)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 139: 反转整数字节序
// 类别: 数值转换
// 说明: 将所指定整数的字节序反转,返回反转后的结果值. 譬如十六进制整数0x12345678,反转后将返回0x78563412. 本命令在与类似Java这样的语言进行数...
EXTERN_C void KRNLN_NAME(139, ReverseIntBytes)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 140: 取字节集长度
// 类别: 字节集操作
// 说明: 取字节集型数据的长度。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(140, BinLen)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 141: 到字节集
// 类别: 字节集操作
// 说明: 将指定数据转换为字节集后返回转换结果。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(141, ToBin)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 142: 取字节集数据
// 类别: 字节集操作
// 说明: 取出字节集中指定位置指定数据类型的数据。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(142, GetBinElement)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 143: 取字节集左边
// 类别: 字节集操作
// 说明: 返回一个字节集，其中包含指定字节集中从左边算起指定数量的字节。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(143, BinLeft)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 144: 取字节集右边
// 类别: 字节集操作
// 说明: 返回一个字节集，其中包含指定字节集中从右边算起指定数量的字节。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(144, BinRight)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 145: 取字节集中间
// 类别: 字节集操作
// 说明: 返回一个字节集，其中包含指定字节集中从指定位置算起指定数量的字节。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(145, BinMid)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 146: 寻找字节集
// 类别: 字节集操作
// 说明: 返回一字节集在另一字节集中最先出现的位置，位置值从 1 开始。如果未找到，返回 -1。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(146, InBin)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 147: 倒找字节集
// 类别: 字节集操作
// 说明: 返回一字节集在另一字节集中最后出现的位置，位置值从 1 开始。如果未找到，返回 -1。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(147, InBinRev)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 148: 字节集替换
// 类别: 字节集操作
// 说明: 将指定字节集的某一部分用其它的字节集替换，然后返回替换后的结果。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(148, RpBin)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 149: 子字节集替换
// 类别: 字节集操作
// 说明: 返回一个字节集，该字节集中指定的子字节集已被替换成另一子字节集，并且替换发生的次数也是被指定的。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(149, RpSubBin)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 150: 取空白字节集
// 类别: 字节集操作
// 说明: 返回具有特定数目 0 字节的字节集。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(150, SpaceBin)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 151: 取重复字节集
// 类别: 字节集操作
// 说明: 返回一个字节集，其中包含指定次数的字节集重复结果。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(151, bin)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 152: 分割字节集
// 类别: 字节集操作
// 说明: 将指定字节集进行分割，返回分割后的一维字节集数组。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(152, SplitBin)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 153: 指针到字节集
// 类别: 字节集操作
// 说明: 返回指定内存指针所指向地址处的一段数据，注意调用本命令前一定要确保所提供的内存地址段真实有效。本命令的最佳使用场合就是在易语言回调子程序和易语言DLL公开子程序...
EXTERN_C void KRNLN_NAME(153, pbin)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 154: 指针到整数
// 类别: 字节集操作
// 说明: 返回指定内存指针所指向地址处的一个整数(INT)，注意调用本命令前一定要确保所提供的内存地址段真实有效。本命令为高级命令。...
EXTERN_C void KRNLN_NAME(154, p2int)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 155: 指针到小数
// 类别: 字节集操作
// 说明: 返回指定内存指针所指向地址处的一个小数(FLOAT)，注意调用本命令前一定要确保所提供的内存地址段真实有效。本命令为高级命令。...
EXTERN_C void KRNLN_NAME(155, p2float)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 156: 指针到双精度小数
// 类别: 字节集操作
// 说明: 返回指定内存指针所指向地址处的一个双精度小数(DOUBLE)，注意调用本命令前一定要确保所提供的内存地址段真实有效。本命令为高级命令。...
EXTERN_C void KRNLN_NAME(156, p2double)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 157: 取字节集内整数
// 类别: 字节集操作
// 说明: 返回字节集中所指定位置处的整数。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(157, GetIntInsideBin)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 158: 置字节集内整数
// 类别: 字节集操作
// 说明: 设置字节集中所指定位置处的整数值。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(158, SetIntInsideBin)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 159: 取磁盘总空间
// 类别: 磁盘操作
// 说明: 返回以 1024 字节（KB）为单位的指定磁盘全部空间。如果失败，返回-1。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(159, GetDiskTotalSpace)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 160: 取磁盘剩余空间
// 类别: 磁盘操作
// 说明: 返回以 1024 字节（KB）为单位的指定磁盘现行剩余空间。如果失败，返回-1。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(160, GetDiskFreeSpace)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 161: 取磁盘卷标
// 类别: 磁盘操作
// 说明: 返回指定磁盘的卷标文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(161, GetDiskLabel)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 162: 置磁盘卷标
// 类别: 磁盘操作
// 说明: 设置指定磁盘的卷标文本。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(162, SetDiskLabel)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 163: 改变驱动器
// 类别: 磁盘操作
// 说明: 改变当前的缺省驱动器。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(163, ChDrive)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 164: 改变目录
// 类别: 磁盘操作
// 说明: 改变当前的目录，但不会改变缺省驱动器位置。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(164, ChDir)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 165: 取当前目录
// 类别: 磁盘操作
// 说明: 返回一个文本，用来代表当前的目录。如果失败，返回空文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(165, CurDir)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 166: 创建目录
// 类别: 磁盘操作
// 说明: 创建一个新的目录。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(166, MkDir)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 167: 删除目录
// 类别: 磁盘操作
// 说明: 删除一个存在的目录及其中的所有子目录和下属文件，请务必谨慎使用本命令。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(167, RmDir)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 168: 复制文件
// 类别: 磁盘操作
// 说明: 成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(168, FileCopy)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 169: 移动文件
// 类别: 磁盘操作
// 说明: 将文件从一个位置移动到另外一个位置。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(169, FileMove)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 170: 删除文件
// 类别: 磁盘操作
// 说明: 成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(170, kill)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 171: 文件更名
// 类别: 磁盘操作
// 说明: 重新命名一个文件或目录。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(171, name)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 172: 文件是否存在
// 类别: 磁盘操作
// 说明: 判断指定的磁盘文件是否真实存在。如存在返回真，否则返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(172, IsFileExist)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 173: 寻找文件
// 类别: 磁盘操作
// 说明: 返回一个文本，用以表示所找到的文件名或目录名，它必须与所要求的名称格式或文件属性相匹配。支持使用多字符 (*) 和单字符 (?) 通配符来指定多重文件。在某子程...
EXTERN_C void KRNLN_NAME(173, dir)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 174: 取文件时间
// 类别: 磁盘操作
// 说明: 返回指定文件被创建或最后修改后的日期和时间。如果该文件不存在，将返回100年1月1日。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(174, FileDateTime)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 175: 取文件尺寸
// 类别: 磁盘操作
// 说明: 返回一个文件的长度，单位是字节。如果该文件不存在，将返回 -1。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(175, FileLen)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 176: 取文件属性
// 类别: 磁盘操作
// 说明: 返回一个文件或目录的属性。此属性值由以下常量或其和构成： 1、#只读文件； 2、#隐藏文件；  4、#系统文件； 16、#子目录；32、#存档文件 。要判断是否...
EXTERN_C void KRNLN_NAME(176, GetAttr)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 177: 置文件属性
// 类别: 磁盘操作
// 说明: 为一个文件设置属性信息。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(177, SetAttr)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 178: 取临时文件名
// 类别: 磁盘操作
// 说明: 返回一个在指定目录中确定不存在的 .TMP 全路径文件名称。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(178, GetTempFileName)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 179: 读入文件
// 类别: 磁盘操作
// 说明: 返回一个字节集，其中包含指定文件的所有数据。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(179, ReadFile)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 180: 写到文件
// 类别: 磁盘操作
// 说明: 本命令用作将一个或数个字节集顺序写到指定文件中，文件原有内容被覆盖。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(180, WriteFile)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 181: 打开文件
// 类别: 文件读写
// 说明: 打开一个普通文件，以对文件进行输入或输出。成功返回被打开文件的文件号，失败返回 0。本命令为初级命令。 1、#读入：从指定文件读入数据，如果该文件不存在则失败；...
EXTERN_C void KRNLN_NAME(181, open)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 182: 打开内存文件
// 类别: 文件读写
// 说明: 建立一个内存文件，以计算机内存为存储介质对数据进行文件式的快速输入或输出。成功返回被打开文件的文件号，失败返回 0。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(182, OpenMemFile)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 183: 关闭文件
// 类别: 文件读写
// 说明: 关闭被打开的各种类型文件。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(183, close)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 184: 关闭所有文件
// 类别: 文件读写
// 说明: 关闭所有现行被打开的文件。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(184, reset)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 185: 锁住文件
// 类别: 文件读写
// 说明: 在有若干个进程访问同一个文件的环境中，使用本命令拒绝其它用户对被打开文件中的某个区域进行读写访问。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(185, lock)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 186: 解锁文件
// 类别: 文件读写
// 说明: 解除由“锁住文件”命令对文件所进行的锁定。成功返回真，失败返回假。注意：本命令调用时所提供的参数值必须与调用“锁住文件”命令时所提供的参数值完全一致。本命令为初...
EXTERN_C void KRNLN_NAME(186, Unlock)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 187: 移动读写位置
// 类别: 文件读写
// 说明: 在被打开的文件中，设置下一个读或写操作的位置。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(187, FSeek)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 188: 移到文件首
// 类别: 文件读写
// 说明: 在被打开的文件中，设置下一个读或写操作的位置到文件首。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(188, SeekToBegin)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 189: 移到文件尾
// 类别: 文件读写
// 说明: 在被打开的文件中，设置下一个读或写操作的位置到文件尾。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(189, SeekToEnd)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 190: 读入字节集
// 类别: 文件读写
// 说明: 本命令用作从文件中当前读写位置读取并返回一段字节集数据，实际读入长度（即返回的字节集的尺寸）可能会小于欲读入长度。如果读入失败，将返回一个空字节集并且自动将当前...
EXTERN_C void KRNLN_NAME(190, ReadBin)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 191: 写出字节集
// 类别: 文件读写
// 说明: 本命令用作写出一段或数段字节集数据到文件中当前读写位置处。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(191, WriteBin)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 192: 读入文本
// 类别: 文件读写
// 说明: 本命令用作从文件中当前读写位置读取并返回一段文本数据，实际读入长度（即返回文本的尺寸）可能会小于欲读入长度。如果该数据中存在字节 0 或 26（文本结束标志），...
EXTERN_C void KRNLN_NAME(192, ReadText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 193: 写出文本
// 类别: 文件读写
// 说明: 本命令用作写出一段或数段文本数据到文件中当前读写位置处。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(193, WriteText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 194: 读入一行
// 类别: 文件读写
// 说明: 本命令用作从文件中当前读写位置读取并返回一行文本数据，行末的回车及换行符将被抛弃。如果读入失败，将返回一个空文本并且自动将当前文件读写位置移到文件尾部。本命令为...
EXTERN_C void KRNLN_NAME(194, ReadLine)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 195: 写文本行
// 类别: 文件读写
// 说明: 本命令用作写出一行或多行文本数据到文件中当前读写位置处，每行的尾部将被自动加上回车及换行符。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可...
EXTERN_C void KRNLN_NAME(195, WriteLine)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 196: 读入数据
// 类别: 文件读写
// 说明: 本命令应该与“写出数据”命令配合使用，用作从文件中当前读写位置读取格式数据到指定的一系列变量或数组变量中。成功返回真，失败返回假。本命令为中级命令。命令参数表中...
EXTERN_C void KRNLN_NAME(196, read)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 197: 写出数据
// 类别: 文件读写
// 说明: 本命令应该与“读入数据”命令配合使用，用作写出一系列变量或数组变量的格式数据到文件中当前读写位置处。成功返回真，失败返回假。 具体各种数据类型数据的写出格式为：...
EXTERN_C void KRNLN_NAME(197, write)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 198: 是否在文件尾
// 类别: 文件读写
// 说明: 如果指定被打开文件的读写位置已经处于该文件数据的尾部，则返回真，否则返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(198, feof)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 199: 取读写位置
// 类别: 文件读写
// 说明: 返回指定被打开文件的当前读/写位置，位置值从 0 开始。如果给定文件号无效或失败，返回 -1 。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(199, loc)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 200: 取文件长度
// 类别: 文件读写
// 说明: 返回指定被打开文件的尺寸，该尺寸以字节为单位。如果给定文件号无效或失败，返回 -1 。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(200, lof)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 201: 插入字节集
// 类别: 文件读写
// 说明: 本命令用作插入一段或数段字节集数据到文件中当前读写位置处。该文件被打开时必须给予"#读写"或"#改读"权限。成功返回真，失败返回假。本命令为初级命令。命令参数表...
EXTERN_C void KRNLN_NAME(201, InsBin)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 202: 插入文本
// 类别: 文件读写
// 说明: 本命令用作插入一段或数段文本数据到文件中当前读写位置处。该文件被打开时必须给予"#读写"或"#改读"权限。成功返回真，失败返回假。本命令为初级命令。命令参数表中...
EXTERN_C void KRNLN_NAME(202, InsText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 203: 插入文本行
// 类别: 文件读写
// 说明: 本命令用作插入一段或数段文本行到文件中当前读写位置处。该文件被打开时必须给予"#读写"或"#改读"权限。成功返回真，失败返回假。本命令为初级命令。命令参数表中最...
EXTERN_C void KRNLN_NAME(203, InsLine)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 204: 删除数据
// 类别: 文件读写
// 说明: 本命令用作在文件中当前读写位置处删除一段字节数据，文件后面的数据顺序前移。该文件被打开时必须给予"#读写"或"#改读"权限。成功返回真，失败返回假。本命令为初级...
EXTERN_C void KRNLN_NAME(204, RemoveData)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 205: 打开加密文件
// 类别: 文件读写
// 说明: 打开一个以指定密码加密的文件，以对此文件进行快速安全访问，支持大尺寸文件。成功返回被打开文件的文件号，失败返回 0。本命令为初级命令。 1、#读入：从指定文件读...
EXTERN_C void KRNLN_NAME(205, CryptOpen)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 206: 运行
// 类别: 系统处理
// 说明: 本命令运行指定的可执行文件或者外部命令。如果成功，返回真，否则返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(206, run)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 207: 取剪辑板文本
// 类别: 系统处理
// 说明: 返回存放于当前 Windows 系统剪辑板中的文本。如果当前剪辑板中无文本数据，将返回空文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(207, GetClipBoardText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 208: 置剪辑板文本
// 类别: 系统处理
// 说明: 将指定文本存放到当前 Windows 系统剪辑板中去，剪辑板中的原有内容被覆盖。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(208, SetClipBoardText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 209: 剪辑板中可有文本
// 类别: 系统处理
// 说明: 如果当前 Windows 系统剪辑板中有文本数据，则返回真，否则返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(209, IsHaveTextInClip)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 210: 清除剪辑板
// 类别: 系统处理
// 说明: 清除当前 Windows 系统剪辑板中的所有数据。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(210, ClearClipBoard)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 211: 取屏幕宽度
// 类别: 系统处理
// 说明: 返回屏幕当前显示区域的宽度，单位为像素点。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(211, GetScreenWidth)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 212: 取屏幕高度
// 类别: 系统处理
// 说明: 返回屏幕当前显示区域的高度，单位为像素点。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(212, GetScreenHeight)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 213: 取鼠标水平位置
// 类别: 系统处理
// 说明: 返回鼠标指针的当前水平位置，单位为像素点，相对于屏幕左边。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(213, GetCursorHorzPos)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 214: 取鼠标垂直位置
// 类别: 系统处理
// 说明: 返回鼠标指针的当前垂直位置，单位为像素点，相对于屏幕顶边。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(214, GetCursorVertPos)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 215: 取颜色数
// 类别: 系统处理
// 说明: 返回当前显示方式所提供的最大颜色显示数目。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(215, GetColorCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 216: 输入框
// 类别: 系统处理
// 说明: 在一对话框中显示提示，等待用户输入正文并按下按钮。如果用户在确认输入后（按下“确认输入”按钮或回车键）退出，返回真，否则返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(216, InputBox)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 217: 信息框
// 类别: 系统处理
// 说明: 在对话框中显示信息，等待用户单击按钮，并返回一个整数告诉用户单击哪一个按钮。该整数为以下常量值之一： 0、#确认钮； 1、#取消钮； 2、#放弃钮； 3、#重试...
EXTERN_C void KRNLN_NAME(217, MsgBox)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 218: 鸣叫
// 类别: 系统处理
// 说明: 通过计算机媒体设备或者喇叭发出一个声音。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(218, beep)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 219: 取启动时间
// 类别: 系统处理
// 说明: 返回自 Windows 系统启动后到现在为止所经历过的毫秒数。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(219, GetTickCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 220: 置等待鼠标
// 类别: 系统处理
// 说明: 本命令设置现行鼠标的形状为沙漏形，用作在即将长时间执行程序前提示操作者。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(220, SetWaitCursor)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 221: 恢复鼠标
// 类别: 系统处理
// 说明: 本命令恢复现行鼠标的原有形状，用作与“置等待鼠标”命令配对使用。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(221, RestroeCursor)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 222: 延时
// 类别: 系统处理
// 说明: 本命令暂停当前程序的运行并等待指定的时间。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(222, sleep)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 223: 取文本注册项
// 类别: 系统处理
// 说明: 在 Windows 注册表中返回指定的文本类型注册表项值。如欲读取注册项默认值，请在项目名后加“\”号，如“test\”。本命令为中级命令。...
EXTERN_C void KRNLN_NAME(223, GetTextRegItem)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 224: 取数值注册项
// 类别: 系统处理
// 说明: 在 Windows 注册表中返回指定的数值类型注册表项值。如欲读取注册项默认值，请在项目名后加“\”号，如“test\”。本命令为中级命令。...
EXTERN_C void KRNLN_NAME(224, GetNumRegItem)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 225: 取字节集注册项
// 类别: 系统处理
// 说明: 在 Windows 注册表中返回指定的字节集类型注册表项值。如欲读取注册项默认值，请在项目名后加“\”号，如“test\”。本命令为中级命令。...
EXTERN_C void KRNLN_NAME(225, GetBinRegItem)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 226: 写注册项
// 类别: 系统处理
// 说明: 在 Windows 注册表中保存或建立指定的注册表项。如欲写入注册项默认值，请在项目名后加“\”号，如“test\”。成功返回真，否则返回假。本命令为中级命令。...
EXTERN_C void KRNLN_NAME(226, SaveRegItem)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 227: 删除注册项
// 类别: 系统处理
// 说明: 在 Windows 注册表中删除指定注册表项或注册表目录。如欲删除注册项默认值，请在项目名后加“\”号，如“test\”。成功返回真，否则返回假。注意在删除目录...
EXTERN_C void KRNLN_NAME(227, DeleteRegItem)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 228: 注册项是否存在
// 类别: 系统处理
// 说明: 如果指定注册表项存在，返回真，否则返回假。如欲检查注册项是否有默认值，请在项目名后加“\”号，如“test\”。本命令为中级命令。...
EXTERN_C void KRNLN_NAME(228, IsRegItemExist)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 229: 取默认底色
// 类别: 系统处理
// 说明: 取回Windows系统的默认窗口背景颜色。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(229, GetBackColor)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 230: 快照
// 类别: 系统处理
// 说明: 捕获指定窗口或屏幕上所有现有显示内容，返回相应图片数据。如果失败，返回空字节集。本命令为高级命令。...
EXTERN_C void KRNLN_NAME(230, GetWinPic)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 231: 读配置项
// 类别: 系统处理
// 说明: 读取指定配置文件中指定项目的文本内容。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(231, GetKeyText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 232: 写配置项
// 类别: 系统处理
// 说明: 将指定文本内容写入指定配置项中或者删除指定的配置项或节，如果指定配置文件不存在，将会自动创建。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(232, SetKeyText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 233: 取配置节名
// 类别: 系统处理
// 说明: 返回指定配置文件中所有已有节名的文本数组。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(233, GetSectionNames)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 234: 取操作系统类别
// 类别: 系统处理
// 说明: 返回当前操作系统的版本类别。返回值为以下值之一：0、未知； 1、Windows 32S； 2、Windows 9X (包含Win95、Win98、WinME等)...
EXTERN_C void KRNLN_NAME(234, GetSysVer)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 235: 多文件对话框
// 类别: 系统处理
// 说明: 显示一个文件打开对话框，允许用户选择或输入多个所需要打开的已存在文件，返回用户所选择或输入后的结果文本数组。如果用户未输入或按“取消”按钮退出，则返回一个成员数...
EXTERN_C void KRNLN_NAME(235, OpenManyFileDialog)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 236: 取程序名称
// 类别: 系统处理
// 说明: 返回所指定类型的程序名称. 本命令为编译预处理命令.本命令为初级命令。...
EXTERN_C void KRNLN_NAME(236, GetAppName)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 237: 交换变量
// 类别: 系统处理
// 说明: 快速交换两个变量的内容. 本命令对于文本、字节集、自定义数据类型不会进行值数据拷贝,仅交换其指针值,因此速度要比通过赋值命令间接交换快得多.本命令要求这两个变量...
EXTERN_C void KRNLN_NAME(237, XchgVar)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 238: 强制交换变量
// 类别: 系统处理
// 说明: 快速交换两个变量的内容. 本命令对于文本、字节集、自定义数据类型不会进行值数据拷贝,仅交换其指针值,因此速度要比通过赋值命令间接交换快得多.本命令与"交换变量"...
EXTERN_C void KRNLN_NAME(238, ForceXchgVar)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 239: 取运行时数据类型
// 类别: 系统处理
// 说明: 返回所提供参数的运行时数据类型值. 注意: 如果该数据类型是在除系统核心支持库外的其它支持库中定义的,那么每一次程序启动后该数据类型值可能会发生变化(因为载入程...
EXTERN_C void KRNLN_NAME(239, GetRuntimeDataType)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 240: 宏
// 类别: 系统处理
// 说明: 在编译程序时,通过提供一系列自定义的常量数据参数来调用系统内置或者外部提供的程序接口,获得其处理结果并将其整合进编译结果中.本命令为初级命令。命令参数表中最后一...
EXTERN_C void KRNLN_NAME(240, Macro)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 241: 类自身
// 类别: 系统处理
// 说明: 只能在类方法中使用本命令,用作返回存储有指向本对象自身数据指针的变量地址.本命令为初级命令。...
EXTERN_C void KRNLN_NAME(241, this)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 242: 取操作系统类别2
// 类别: 系统处理
// 说明: 返回当前操作系统的版本类别。返回值为以下值之一：0、未知； 1、Windows 32S； 2、Windows 9X (包含Win95、Win98、WinME等)...
EXTERN_C void KRNLN_NAME(242, GetSysVer2)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 243: 取最后错误
// 类别: 系统处理
// 说明: 调用操作系统的GetLastError函数,返回该API函数被调用后的返回值.本命令为初级命令。...
EXTERN_C void KRNLN_NAME(243, GetLastError)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 244: 取统一文本
// 类别: 系统处理
// 说明: 返回所指定文本的Unicode格式字节集。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(244, GetUTextBin)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 245: 取统一文本长度
// 类别: 系统处理
// 说明: 返回所指定文本转换到Unicode格式文本后的字符数目。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(245, GetUTextLength)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 246: 播放音乐
// 类别: 媒体播放
// 说明: 可以播放 .WAV、.MID 声音文件或相应格式的字节集声音数据、声音资源。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(246, PlayMusic)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 247: 停止播放
// 类别: 媒体播放
// 说明: 停止正在播放的音乐。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(247, PlayStop)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 248: 播放MID
// 类别: 媒体播放
// 说明: 可以自动连续播放多个 MIDI 声音文件（注意不支持 WAV ）或相应格式的字节集声音数据、声音资源。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(248, PlayMID)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 249: 播放MP3
// 类别: 媒体播放
// 说明: 可以自动连续播放多个 MP3 音乐文件。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(249, PlayMP3)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 250: 同步播放MP3
// 类别: 媒体播放
// 说明: 播放一次指定的MP3音乐文件，在播放过程中，会自动给指定标签组件发送反馈事件通知，以同步提供当前播放进度百分比。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(250, SyncPlayMP3)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 251: 暂停播放MP3
// 类别: 媒体播放
// 说明: 如果当前正在播放由“播放MP3”或“同步播放MP3”命令所指定的MP3音乐，则将其暂停。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(251, PauseMP3Play)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 252: 继续播放MP3
// 类别: 媒体播放
// 说明: 如果当前由“播放MP3”或“同步播放MP3”命令所播放MP3音乐已经被暂停，则继续播放。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(252, UnPauseMP3Play)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 253: 取MP3播放状态
// 类别: 媒体播放
// 说明: 返回当前由“播放MP3”或“同步播放MP3”命令所播放MP3音乐的播放状态： 0：已经被停止或未播放； -1：正在播放但被暂停； 1：正在播放。本命令为初级命令...
EXTERN_C void KRNLN_NAME(253, GetMP3PlayState)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 254: 输出调试文本
// 类别: 程序调试
// 说明: 本命令仅在易程序的调试版本中被执行，在发布版本中将被直接跳过。 使用本命令可以在 E 调试系统中的“输出”窗口内输出指定的文本行以帮助调试，该文本之前被自动加上...
EXTERN_C void KRNLN_NAME(254, OutputDebugText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 255: 暂停
// 类别: 程序调试
// 说明: 本命令仅在易程序的调试版本中被执行，在发布版本中将被直接跳过。 可以在子程序中的任何地方放置此命令，使用此命令，就相当于在程序代码中设置断点。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(255, stop)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 256: 检查
// 类别: 程序调试
// 说明: 本命令仅在易程序的调试版本中被执行，在发布版本中将被直接跳过。 执行本命令时，如果给定参数的条件值被计算后结果为假，易程序的执行将被暂停且警示。可以在子程序中的...
EXTERN_C void KRNLN_NAME(256, assert)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 257: 是否为调试版
// 类别: 程序调试
// 说明: 如果当前 EXE 易程序执行文件为易语言编辑环境调试运行程序时编译出来的调试版本，返回真。否则表明为发布版本，返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(257, IsDebugVer)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 258: 载入
// 类别: 其他
// 说明: 载入指定的窗口，根据窗口的“可视”属性决定是否显示它。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(258, LoadWin)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 259: 选择
// 类别: 其他
// 说明: 根据所提供参数的值，返回两部分中的其中一个。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(259, iif)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 260: 多项选择
// 类别: 其他
// 说明: 从参数列表项目中选择并返回一个值。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(260, choose)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 261: 是否为空
// 类别: 其他
// 说明: 返回一个逻辑值，指出在调用子程序时是否为指定参数提供了数据（该参数必定有“可空”标志），如未提供，返回真，否则返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(261, IsMissing)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 262: 是否已创建
// 类别: 其他
// 说明: 如果指定的窗口或窗口组件有效且已经被载入或创建，返回真，否则返回假。对于窗口菜单项组件，如果其所处窗口已经被载入，命令将返回真，否则返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(262, IsCreated)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 263: 取数据类型尺寸
// 类别: 其他
// 说明: 取指定数据类型的数据尺寸，该数据类型不能为文本、字节集、库或用户自定义数据类型。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(263, GetDataTypeSize)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 264: 取颜色值
// 类别: 其他
// 说明: 返回一个整数，用来表示一个红绿蓝颜色值。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(264, rgb)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 265: 取事件组件
// 类别: 其他
// 说明: 返回一个有效的组件型数据，指定当前所正在处理的事件是从何组件实际产生的。本命令主要用于在事件处理子程序中动态确定事件的来源。注意：只能在处理事件的过程首部使用本...
EXTERN_C void KRNLN_NAME(265, GetEventUnit)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 266: 事件转移
// 类别: 其他
// 说明: 设置将第一个组件上产生的所有事件转交由第二个同窗口同类型组件的事件处理子程序去处理，就好象此事件是在第二个组件上实际发生的一样。本命令可用来设置用同一事件处理子...
EXTERN_C void KRNLN_NAME(266, EventPost)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 267: 复制窗口组件
// 类别: 其他
// 说明: 复制创建与指定窗口组件设计状态完全一致的新组件。该被复制组件将放在原组件的下面，其事件被自动转移到原组件上。如果复制时被复制窗口组件所在的窗口尚未被载入，那么复...
EXTERN_C void KRNLN_NAME(267, CopyWinUnit)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 268: 处理事件
// 类别: 其他
// 说明: 暂时转让控制权，以便让 Windows 操作系统有机会处理其它的如用户键盘或鼠标输入等事件。直到操作系统处理并发送完程序队列中的所有事件后，命令才会返回。本命令...
EXTERN_C void KRNLN_NAME(268, DoEvents)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 269: 载入图片
// 类别: 其他
// 说明: 载入指定图片，以供画板、打印机等对象的“画图片”、“取图片宽度”、“取图片高度”之类成员命令使用。如成功则返回被载入图片的图片号，失败返回 0。本命令为初级命令...
EXTERN_C void KRNLN_NAME(269, LoadPic)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 270: 卸载图片
// 类别: 其他
// 说明: 卸载被载入的图片，图片被卸载后不能再被使用。如果某图片被载入后未被卸载，在易程序退出时将自动被卸载。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(270, UnloadPic)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 271: 取硬盘特征字
// 类别: 其他
// 说明: 返回电脑中第一个物理硬盘的物理特征字，该特征字是仅硬件相关的，也就是说与任何软件系统都无关（包括操作系统）。用户可以使用此特征字来限制自己的程序仅在某一台计算机...
EXTERN_C void KRNLN_NAME(271, GetHDiskCode)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 272: 取系统语言
// 类别: 其他
// 说明: 返回当前系统运行时环境所支持的语言版本类型，为以下常量值之一： 1、GBK中文； 2、英文； 3、BIG5中文； 4、日文。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(272, GetSysLang)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 273: 写到内存
// 类别: 其他
// 说明: 将数据写出到指定的内存区域，注意调用本命令前一定要确保所提供的内存区域真实有效。本命令的最佳使用场合就是在易语言回调子程序和易语言DLL公开子程序中用作对外输出...
EXTERN_C void KRNLN_NAME(273, WriteMem)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 274: DLL命令调用转向
// 类别: 其他
// 说明: 本命令用来设置下次程序中执行任意一条DLL命令时所使用的DLL库文件名和在DLL库文件中的调用命令名，此设置将覆盖该被调用DLL命令在程序设计时所提供的原始值。...
EXTERN_C void KRNLN_NAME(274, SetDllCmdInf)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 275: 置错误提示管理
// 类别: 其他
// 说明: 本命令用来设置当运行时如果产生了导致程序崩溃的严重错误时用来对该错误进行详细提示的子程序地址，如果未进行此项设置，发生严重错误时运行时环境将自动提示该错误的详细...
EXTERN_C void KRNLN_NAME(275, SetErrorManger)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 276: 置DLL装载目录
// 类别: 其他
// 说明: 本命令用来设置当程序中执行到DLL命令时装载其DLL库文件的优先装载路径，即系统将优先到该路径下去装载指定的DLL文件。本命令所设置结果对所有DLL命令设置中的...
EXTERN_C void KRNLN_NAME(276, SetDllCmdLoadPath)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 277: 取组件名称
// 类别: 其他
// 说明: 返回指定窗口组件的名称，如果所提供对象不是窗口组件，将返回空文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(277, GetUnitName)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 278: 取对象类型
// 类别: 其他
// 说明: 如果指定对象的类型为在支持库内定义的数据类型，则返回该数据类型的名称，否则返回空文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(278, GetObjectType)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 279: 寻找组件
// 类别: 其他
// 说明: 按照指定条件寻找符合要求的窗口组件，成功返回非零寻找句柄，未找到返回零。注意，所返回的非零寻找句柄在不再使用后必须调用“清除组件寻找句柄”命令释放。本命令为初级...
EXTERN_C void KRNLN_NAME(279, FindUnit)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 280: 取找到组件数目
// 类别: 其他
// 说明: 返回“寻找组件”命令所找到的组件数目。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(280, GetFoundUnitCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 281: 取所找到组件
// 类别: 其他
// 说明: 返回“寻找组件”命令所找到的指定索引位置处的组件。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(281, GetFoundUnit)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 282: 清除组件寻找句柄
// 类别: 其他
// 说明: 释放“寻找组件”命令所返回的组件寻找句柄，此句柄以后不能再使用。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(282, ReleaseFounddHandle)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 283: 置入代码
// 类别: 其他
// 说明: 在编译后文件代码段中当前语句位置置入指定的机器指令码数据。本命令为高级命令。...
EXTERN_C void KRNLN_NAME(283, MachineCode)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 284: 运行控制台程序
// 类别: 其他
// 说明: 运行指定的控制台程序(不会打开控制台窗口),返回是否运行成功.本命令为初级命令。...
EXTERN_C void KRNLN_NAME(284, RunConsoleApp)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 285: 取核心库版本
// 类别: 其他
// 说明: 返回核心库的当前版本号。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(285, GetKrnlLibVer)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 286: 条件宏是否存在
// 类别: 其他
// 说明: 返回所指定的编译条件宏是否已经被定义. 编译条件宏在"程序->配置->编译条件"菜单功能中设置,所有在当前被激活编译条件中存在的条件宏均认为被定义.如果当前为编...
EXTERN_C void KRNLN_NAME(286, IsCondMacroDefined)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 287: 是否支持多用户
// 类别: 数据库
// 说明: 检查本支持库所提供的数据库功能是否支持多用户同时对数据库操作。如果支持，返回真，否则返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(287, MutiUser)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 288: 取错误码
// 类别: 数据库
// 说明: 如果执行某数据库命令失败，在其后执行本命令可以取回非 0 的数据库错误码值。如果该数据库命令执行成功，执行本命令将返回 0 。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(288, GetErrCode)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 289: 取错误信息
// 类别: 数据库
// 说明: 如果执行某数据库命令失败，在其后执行本命令可以取回错误信息文本。如果该数据库命令执行成功，执行本命令将返回空文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(289, GetErrMsg)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 290: 创建
// 类别: 数据库
// 说明: 创建指定的数据库文件，如果该数据库文件存在，将被覆盖。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(290, create)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 291: 打开
// 类别: 数据库
// 说明: 打开指定的数据库文件。成功返回真，并自动将当前数据库设置为此数据库，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。 1、#无限制：允许其...
EXTERN_C void KRNLN_NAME(291, use)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 292: 替换打开
// 类别: 数据库
// 说明: 打开指定的数据库文件。成功返回真，并自动关闭当前数据库后将当前数据库设置为此数据库，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。 1、...
EXTERN_C void KRNLN_NAME(292, ruse)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 293: 置当前库
// 类别: 数据库
// 说明: 本命令选择一个已经被打开的数据库为当前数据库，大部分数据库操作命令都针对当前数据库，譬如：记录指针的移动、字段的读写等等。如果指定数据库不存在或者尚未被打开，命...
EXTERN_C void KRNLN_NAME(293, select)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 294: 取当前库
// 类别: 数据库
// 说明: 返回当前数据库的别名，如不存在别名，则返回其名称。如果未设置当前数据库，将返回空文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(294, dbf)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 295: 关闭
// 类别: 数据库
// 说明: 本命令关闭已经被打开的指定数据库。如果没有提供数据库名称，默认为关闭当前数据库。如果当前数据库被关闭，当前数据库状态值将被设置为空，此时可重新使用“置当前库”命...
EXTERN_C void KRNLN_NAME(295, CloseDBF)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 296: 全部关闭
// 类别: 数据库
// 说明: 本命令关闭当前已经被打开的所有数据库。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(296, CloseAll)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 297: 取库文件名
// 类别: 数据库
// 说明: 返回当前数据库的全路径文件名。如果当前数据库不存在，返回空文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(297, GetDBF)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 298: 是否已打开
// 类别: 数据库
// 说明: 如果指定数据库文件已经被打开，返回真，否则返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(298, used)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 299: 取记录数
// 类别: 数据库
// 说明: 返回当前数据库的记录数目。如果没有设置当前数据库或者失败，将返回 0 。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(299, RecCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 300: 取创建时间
// 类别: 数据库
// 说明: 返回当前数据库被创建的时间。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(300, DateCreated)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 301: 取字段数
// 类别: 数据库
// 说明: 返回当前数据库中字段的数目。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(301, FCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 302: 取字段名
// 类别: 数据库
// 说明: 返回当前数据库中指定字段的名称。如果指定字段不存在，将返回空文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(302, GetFieldName)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 303: 改字段名
// 类别: 数据库
// 说明: 修改当前数据库中指定字段的名称。本命令只有当数据库的共享打开方式为“#禁止读写”时才有效。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(303, ModiFieldName)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 304: 取字段类型
// 类别: 数据库
// 说明: 返回当前数据库中指定字段的类型，字段类型值为以下常量值之一： 1、#字节型； 2、#短整数型； 3、#整数型； 4、#长整数型； 5、#小数型； 6、#双精度小...
EXTERN_C void KRNLN_NAME(304, GetFieldType)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 305: 取字段尺寸
// 类别: 数据库
// 说明: 返回当前数据库中指定字段在记录中所占的尺寸。如果指定字段不存在，将返回 0 。注意：备注型和字节集型字段将返回 4 。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(305, FSize)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 306: 新建索引
// 类别: 数据库
// 说明: 在当前数据库中创建并打开一个索引文件。索引的存在将降低对数据库记录的更新速度，所以除非是较大的数据库（最起码在一千条记录以上），否则一般不用使用索引。成功返回真...
EXTERN_C void KRNLN_NAME(306, CreateIndex)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 307: 置当前索引
// 类别: 数据库
// 说明: 为当前数据库指定当前索引，在进行索引查找时将使用当前索引。如果指定索引不存在，将返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(307, SetIndexTo)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 308: 取当前索引
// 类别: 数据库
// 说明: 返回当前数据库的当前索引名称。如果未设置当前数据库或无当前索引，返回空文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(308, ndx)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 309: 更新索引
// 类别: 数据库
// 说明: 通过完全重建来更新当前数据库的当前索引。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(309, reindex)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 310: 取索引数
// 类别: 数据库
// 说明: 返回当前数据库中已经被打开索引的数目。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(310, GetIndexCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 311: 取索引名
// 类别: 数据库
// 说明: 返回当前数据库中指定索引的名称。如果指定位置处不存在索引，将返回空文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(311, GetIndexName)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 312: 取索引字段
// 类别: 数据库
// 说明: 返回当前数据库中指定索引内所有被索引字段的名称数组。如果指定位置处不存在索引，将返回空数组。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(312, GetIndexField)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 313: 置加锁重试时间
// 类别: 数据库
// 说明: 本命令设置对数据库进行加锁时，如果该位置已经被其他用户锁住或者正在被其他用户访问，系统反复重试加锁的时间。当你的数据库可能被多用户同时访问时，必须在程序首使用本...
EXTERN_C void KRNLN_NAME(313, SetLockTimeout)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 314: 锁住数据库
// 类别: 数据库
// 说明: 本命令执行后独占锁住整个当前数据库，拒绝其它用户的任何访问。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(314, LockDBF)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 315: 解锁数据库
// 类别: 数据库
// 说明: 本命令解除“锁住数据库”命令对当前数据库加的锁。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(315, UnlockDBF)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 316: 锁住增删
// 类别: 数据库
// 说明: 本命令执行后独占锁住对当前数据库的增删操作，其它用户可以读写已存在的任何记录，但是不能对数据库记录进行增删。也就是说：不能执行改变数据库记录数目的任何操作。注意...
EXTERN_C void KRNLN_NAME(316, LockCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 317: 解增删锁
// 类别: 数据库
// 说明: 本命令解除“锁住增删”命令对当前数据库加的锁。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(317, UnlockCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 318: 锁住记录
// 类别: 数据库
// 说明: 本命令执行后可以锁住指定的一个或多个记录。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(318, LockRec)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 319: 解锁记录
// 类别: 数据库
// 说明: 本命令解除“锁住记录”命令对记录所加的锁。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(319, UnlockRec)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 320: 全部解锁
// 类别: 数据库
// 说明: 本命令解除由“锁住数据库”、“锁住增删”、“锁住记录”等命令对当前数据库加上的所有锁。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(320, UnlockAll)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 321: 取平均值
// 类别: 数据库
// 说明: 本命令根据当前数据库计算并返回某数值型表达式的平均值，命令执行后当前记录指针保持不变。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(321, avg)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 322: 求和
// 类别: 数据库
// 说明: 本命令根据当前数据库计算并返回某数值型表达式的和，命令执行后当前记录指针保持不变。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(322, sum)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 323: 取最大值
// 类别: 数据库
// 说明: 本命令根据当前数据库计算并返回某数值型表达式的最大值，当前记录指针被移动到具有最大值的记录。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(323, max)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 324: 取最小值
// 类别: 数据库
// 说明: 本命令根据当前数据库计算并返回某数值型表达式的最小值，当前记录指针被移动到具有最小值的记录。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(324, min)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 325: 取最大时间
// 类别: 数据库
// 说明: 本命令根据当前数据库计算并返回某日期时间型表达式的最大值，当前记录指针被移动到具有最大值的记录。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(325, MaxTime)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 326: 取最小时间
// 类别: 数据库
// 说明: 本命令根据当前数据库计算并返回某日期时间型表达式的最小值，当前记录指针被移动到具有最小值的记录。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(326, MinTime)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 327: 计算数目
// 类别: 数据库
// 说明: 本命令返回当前数据库中指定范围记录的数目。命令执行后当前记录指针保持不变。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(327, count)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 328: 复制结构
// 类别: 数据库
// 说明: 复制当前数据库的结构到另外一个数据库。命令执行后当前记录指针保持不变。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(328, CopyStructTo)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 329: 复制记录
// 类别: 数据库
// 说明: 复制当前数据库的记录到另外一个数据库。命令执行后当前记录指针保持不变。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(329, CopyTo)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 330: 计算排序
// 类别: 数据库
// 说明: 根据指定数值表达式的计算值排序复制当前数据库的记录到另外一个数据库。命令执行后当前记录指针保持不变。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一...
EXTERN_C void KRNLN_NAME(330, csort)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 331: 排序
// 类别: 数据库
// 说明: 根据指定字段排序复制当前数据库的记录到另外一个数据库。命令执行后当前记录指针保持不变。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重...
EXTERN_C void KRNLN_NAME(331, sort)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 332: 分类计算
// 类别: 数据库
// 说明: 对当前数据库进行分类计算，将结果输出到另外一个数据库。注意所有的备注或字节集字段内容将不会被输出到目的数据库中。 命令执行后当前记录指针保持不变。成功返回真，失...
EXTERN_C void KRNLN_NAME(332, total)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 333: 添加
// 类别: 数据库
// 说明: 将指定数据库中的记录添加到当前数据库。命令执行后当前记录指针保持不变。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(333, AppendFrom)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 334: 加记录
// 类别: 数据库
// 说明: 本命令在当前数据库的尾部添加一条记录，并将当前记录指针移动到该记录处。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(334, Append)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 335: 加空记录
// 类别: 数据库
// 说明: 本命令在当前数据库的尾部添加一条空记录，并将当前记录指针移动到该记录上。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(335, AppendBlank)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 336: 替换
// 类别: 数据库
// 说明: 本命令替换当前数据库中的记录数据，当前记录指针保持不变。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(336, replace)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 337: 修改
// 类别: 数据库
// 说明: 本命令一次性修改当前数据库中当前记录内多个字段的内容，当前记录指针保持不变。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(337, modify)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 338: 删除
// 类别: 数据库
// 说明: 标记删除当前数据库中的当前记录或者指定范围内的记录，当前记录指针位置保持不变。注意：此命令并不实际删除记录，仅仅只加上删除标记，记录依旧存在并可以读写。成功返回...
EXTERN_C void KRNLN_NAME(338, delete_)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 339: 是否已删除
// 类别: 数据库
// 说明: 如果当前数据库的当前记录已经被“删除”命令加上删除标记，返回真，否则返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(339, IsDeleted)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 340: 恢复删除
// 类别: 数据库
// 说明: 去掉当前数据库中当前记录或者指定范围内记录的删除标记，当前记录指针位置保持不变。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(340, recall)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 341: 彻底删除
// 类别: 数据库
// 说明: 将当前数据库中所有被加上删除标记的记录从数据库中彻底清除，当前记录指针位置将被移动到数据库首记录。记录被彻底删除后将不复存在，并且无法恢复。注意执行本命令将重组...
EXTERN_C void KRNLN_NAME(341, pack)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 342: 清空
// 类别: 数据库
// 说明: 彻底清除当前数据库中的所有记录。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(342, zap)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 343: 读
// 类别: 数据库
// 说明: 返回当前数据库中当前记录处指定字段的数据内容，所返回数据的类型等同于字段的类型。如果指定字段不存在，将产生运行时错误。执行本命令后使用“取错误码”命令可以判断是...
EXTERN_C void KRNLN_NAME(343, r)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 344: 写
// 类别: 数据库
// 说明: 将数据写入到当前数据库中当前记录处的指定字段内。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(344, w)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 345: 读字段
// 类别: 数据库
// 说明: 返回指定数据库中当前记录处指定字段的数据内容，所返回数据的类型等同于字段的类型。如果指定字段不存在，将产生运行时错误。本命令与“读”命令的不同之处为本命令可以从...
EXTERN_C void KRNLN_NAME(345, ReadField)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 346: 写字段
// 类别: 数据库
// 说明: 将数据写入到指定数据库中当前记录处的指定字段内。成功返回真，失败返回假。本命令与“写”命令的不同之处为本命令可以写入非当前数据库中的字段。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(346, WriteField)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 347: 附加字节集
// 类别: 数据库
// 说明: 将字节集数据添加到当前记录处指定字节集型字段的尾部。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(347, AppendBin)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 348: 附加备注
// 类别: 数据库
// 说明: 将文本数据添加到当前记录处指定备注型字段的尾部。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(348, AppendMemo)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 349: 索引查找
// 类别: 数据库
// 说明: 本命令在当前数据库中使用其当前索引来快速寻找某一记录，寻找从当前记录位置（包括当前记录）处开始。注意为命令所提供的欲搜寻值参数的数目必须与被索引字段的数目一致。...
EXTERN_C void KRNLN_NAME(349, seek)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 350: 查找
// 类别: 数据库
// 说明: 本命令从当前数据库中当前记录位置处（包括当前记录）开始寻找符合给定条件的记录。如成功找到返回真，并且将当前记录指针移至所找到的记录。如出错或未找到则返回假，当前...
EXTERN_C void KRNLN_NAME(350, locate)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 351: 到首记录
// 类别: 数据库
// 说明: 将当前数据库中当前记录指针移动到数据库的第一条记录上。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(351, GoTop)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 352: 到尾记录
// 类别: 数据库
// 说明: 将当前数据库中当前记录指针移动到数据库的最后一条记录上。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(352, GoBottom)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 353: 跳过
// 类别: 数据库
// 说明: 将当前数据库中当前记录指针向前或者向后移动数条记录。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(353, skip)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 354: 取记录号
// 类别: 数据库
// 说明: 返回当前数据库中当前记录指针所指向记录的记录号。如果当前记录指针在首记录前，返回 0 ；如果在尾记录后，返回值为记录总数加一。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(354, RecNO)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 355: 跳到
// 类别: 数据库
// 说明: 改变当前数据库中的当前记录指针到指定的记录号。如果所欲跳到记录超出了数据库的首尾边界，将自动停留在对应边界记录处。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(355, goto_)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 356: 取标签
// 类别: 数据库
// 说明: 在数据库的存在期间，同一数据库内任何时候任何一条记录（包括已经被实际删除的记录）都有一个唯一的非 0 正整数标签值，使用本命令可以取回当前数据库当前记录的这一标...
EXTERN_C void KRNLN_NAME(356, GetBookmark)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 357: 记录是否存在
// 类别: 数据库
// 说明: 如果当前数据库中存在具有指定标签值的记录，命令返回真，否则返回假。本命令可以用作在多用户环境中检查某记录是否被其它用户实际删除。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(357, IsRecExist)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 358: 标签跳转
// 类别: 数据库
// 说明: 使用本命令可以改变当前数据库中的当前记录指针到具有指定标签的记录。如成功找到返回真，并且将当前记录指针移至该记录。如出错或未找到则返回假，当前记录指针位置保持不...
EXTERN_C void KRNLN_NAME(358, GotoBookmark)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 359: 首记录前
// 类别: 数据库
// 说明: 如果当前数据库的当前记录指针已在首记录的前面，返回真，否则返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(359, bof)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 360: 尾记录后
// 类别: 数据库
// 说明: 如果当前数据库的当前记录指针已在尾记录的后面，返回真，否则返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(360, eof)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 361: 写出缓存
// 类别: 数据库
// 说明: 为了加快对数据库数据的存取速度，对数据库所进行的写操作将首先被保存在写缓存区内，等候一段时间后才会被自动写入到数据库文件中。使用本命令可以将当前数据库停留在写缓...
EXTERN_C void KRNLN_NAME(361, flush)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 362: 写出所有缓存
// 类别: 数据库
// 说明: 将所有数据库停留在写缓存区内的数据都立即写入到对应的数据库磁盘文件中。一般情况下无需使用本命令。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(362, FlushAll)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 363: 编辑
// 类别: 数据库
// 说明: 提供一个对话框，用作查看或编辑修改当前数据库的记录，起始位置从当前记录开始。本命令退出后当前记录指针被移动到首记录。本命令为初级命令。命令参数表中最后一个参数可...
EXTERN_C void KRNLN_NAME(363, edit)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 364: 是否已加密
// 类别: 数据库
// 说明: 如果需要提供密码才能访问指定的数据库文件（即已经被加密），返回真，否则返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(364, IsEncrypted)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 365: 置数据库密码
// 类别: 数据库
// 说明: 本命令用作设置或修改当前数据库的访问密码，注意数据库文件必须可写。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(365, SetPassword)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 366: 密码输入框
// 类别: 数据库
// 说明: 本命令用作输入数据库的访问密码，如果用户在确认输入后（按下“确认”按钮或回车键）退出，返回真，否则返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(366, InputDBPwd)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 367: 复制密码
// 类别: 数据库
// 说明: 本命令可以将当前数据库的访问密码设置为等同于其他已打开数据库，注意数据库文件必须可写。成功返回真，失败返回假。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(367, CopyPassword)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 368: 取主机名
// 类别: 网络通信
// 说明: 返回本机的主机名，用作在网络通讯中标志本机地址。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(368, GetHostName)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 369: 通信测试
// 类别: 网络通信
// 说明: 测试与指定主机是否能够正常通讯。返回被测试主机的通讯响应时间。如果无法通讯或者测试失败，返回 -1。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(369, ping)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 370: 转换为主机名
// 类别: 网络通信
// 说明: 将指定的 IP 地址转换为其主机名。如果失败返回空文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(370, IPToHostName)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 371: 转换为IP地址
// 类别: 网络通信
// 说明: 将指定的主机名转换为其 IP 地址。如果失败返回空文本。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(371, HostNameToIP)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 372: 标准输出
// 类别: 控制台操作
// 说明: 在标准输出设备或标准错误设备上输出指定的内容，注意本命令只能在控制台程序中使用。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(372, fputs)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 373: 标准输入
// 类别: 控制台操作
// 说明: 在标准输入设备上请求输入最多包含2048个字符的一行文本，返回用户所输入的内容。注意本命令只能在控制台程序中使用。本命令为初级命令。...
EXTERN_C void KRNLN_NAME(373, fgets)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 374: 载入易包文件
// 类别: 易包
// 说明: 预先载入所指定的易包文件(如果文件名未提供后缀则默认为".epk"),防止系统以后在程序中调用到该易包内公开子程序时采用默认方法自动载入.成功返回真,失败返回假...
EXTERN_C void KRNLN_NAME(374, LoadEPKFile)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 375: 载入易包数据
// 类别: 易包
// 说明: 预先载入所指定易包的数据,防止系统以后在程序中调用到该易包内公开子程序时采用默认方法自动载入.成功返回真,失败返回假.所载入易包不需要刻意去释放,在程序退出时会...
EXTERN_C void KRNLN_NAME(375, LoadEPKData)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 376: 卸载易包
// 类别: 易包
// 说明: 卸载已被载入的指定名称的易包,成功返回真,失败返回假.本命令不支持在多线程环境和静态编译模式下使用.执行本命令前,必须确保以下几点,否则后果无法预测: 1. 在...
EXTERN_C void KRNLN_NAME(376, UnloadEPK)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 377: 易包是否被载入
// 类别: 易包
// 说明: 返回所指定名称的易包是否已经被载入.本命令不支持在静态编译模式下使用。本命令为高级命令。...
EXTERN_C void KRNLN_NAME(377, IsEPKLoaded)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 378: 复制易包常量
// 类别: 易包
// 说明: 本命令仅在编译易包时有效,用作根据所提供的常量数据建立返回对应的非常量数据.在对易包公开子程序以参考方式传递进来的参数变量使用文本/字节集/数组三类常量进行赋值...
EXTERN_C void KRNLN_NAME(378, CloneConstData)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 379: 取窗口句柄
// 类别: 易包
// 说明: 取出本窗口或窗口组件的窗口句柄（即HWND）。本命令为高级对象成员命令。...
EXTERN_C void KRNLN_NAME(379, GetHWnd)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 380: 销毁
// 类别: 易包
// 说明: 销毁本窗口或窗口组件。窗口被销毁后，所有在载入本窗口之后对本窗口及其内窗口组件所进行的修改或设置都将被抛弃，窗口被销毁后如想再次使用必须重新装载。注意：当窗口调...
EXTERN_C void KRNLN_NAME(380, destroy)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 381: 获取焦点
// 类别: 易包
// 说明: 将焦点移动到窗口组件。如果对窗口使用本方法，窗口将自动把焦点转移到第一个有能力保留焦点的窗口组件上去。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(381, SetFocus)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 382: 可有焦点
// 类别: 易包
// 说明: 如果当前窗口组件具有焦点，则返回真，否则返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(382, IsFocus)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 383: 取用户区宽度
// 类别: 易包
// 说明: 返回窗口或窗口组件用户区域的宽度，单位为像素点。对窗口而言，用户区域为窗口区域减去标题栏、菜单栏、边框后的区域；对窗口组件而言，用户区域等同于其窗口组件区域。本...
EXTERN_C void KRNLN_NAME(383, GetClientWidth)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 384: 取用户区高度
// 类别: 易包
// 说明: 返回窗口或窗口组件用户区域的高度，单位为像素点。对窗口而言，用户区域为窗口区域减去标题栏、菜单栏、边框后的区域；对窗口组件而言，用户区域等同于其窗口组件区域。本...
EXTERN_C void KRNLN_NAME(384, GetClientHeight)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 385: 禁止重画
// 类别: 易包
// 说明: 禁止窗口或窗口组件重画，以避免当频繁进行操作时引起闪烁。注意调用本命令后必须调用“允许重画”命令进行恢复。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(385, LockWindowUpdate)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 386: 允许重画
// 类别: 易包
// 说明: 重新允许被禁止的窗口或窗口组件重画。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(386, UnlockWindowUpdate)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 387: 重画
// 类别: 易包
// 说明: 通知 Windows 系统本窗口或窗口组件上的显示内容需要在以后被全部重画。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(387, invalidate)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 388: 部分重画
// 类别: 易包
// 说明: 通知 Windows 系统本窗口或窗口组件上的显示内容需要在以后被部分重画。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(388, InvalidateRect)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 389: 取消重画
// 类别: 易包
// 说明: 通知 Windows 系统本窗口或窗口组件不再需要被重画，保留现有的全部显示内容。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(389, validate)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 390: 刷新显示
// 类别: 易包
// 说明: 如果本窗口或窗口组件上的显示内容需要被全部或部分重画，则立即进行重画更新，否则不进行任何操作直接返回。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(390, UpdateWindow)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 391: 移动
// 类别: 易包
// 说明: 改变窗口或窗口组件的位置或尺寸。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(391, move)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 392: 调整层次
// 类别: 易包
// 说明: 改变窗口或窗口组件的现行所处层次。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(392, ZOrder)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 393: 弹出菜单
// 类别: 易包
// 说明: 用于在窗口上的当前鼠标位置或指定坐标位置显示弹出式菜单，如果调用对象为窗口组件，则自动使用其所在的窗口。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(393, PopupMenu)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 394: 发送信息
// 类别: 易包
// 说明: 将指定信息发送到窗口或窗口组件，并等待且取回信息反馈值。本命令为中级对象成员命令。...
EXTERN_C void KRNLN_NAME(394, SendMessage)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 395: 投递信息
// 类别: 易包
// 说明: 将指定信息发送到窗口或窗口组件，不等待直接返回。本命令为中级对象成员命令。...
EXTERN_C void KRNLN_NAME(395, PostMessage)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 396: 取标记组件
// 类别: 易包
// 说明: 返回当前窗口中具有指定标记数值文本的组件。如果不存在，将产生运行时错误。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(396, GetSpecTagUnit)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 397: 置外形图片
// 类别: 易包
// 说明: 使用图片来设置窗口的外形，注意图片类型不能为图标及鼠标指针。另外，图片的轮廓应该尽量简单，以免影响窗口的刷新速度。如果调用对象为窗口组件，将自动使用其所在的窗口...
EXTERN_C void KRNLN_NAME(397, SetShapePic)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 398: 激活
// 类别: 易包
// 说明: 本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(398, Activate)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 399: 置托盘图标
// 类别: 易包
// 说明: 设置本程序在系统托盘中的图标。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(399, SetTrayIcon)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 400: 弹出托盘菜单
// 类别: 易包
// 说明: 在当前鼠标位置弹出指定菜单，本命令一般用作支持托盘菜单的弹出。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(400, PopupTrayMenu)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 401: 置父窗口
// 类别: 易包
// 说明: 设置指定窗口或窗口组件为本对象窗口或窗口组件的父窗口。本命令为初级对象成员命令。 成员属性“标题”所在数据类型为“窗口”，英文名称为“caption”，类型为“...
EXTERN_C void KRNLN_NAME(401, SetParentWnd)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 402: 加入文本
// 类别: 易包
// 说明: 将指定文本加入到编辑框内容的尾部。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添加。 成员属性“内容”所在数据类型为“编辑框”，英文名称为“co...
EXTERN_C void KRNLN_NAME(402, AddText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 403: 取设备句柄
// 类别: 易包
// 说明: 如当前用户程序正在处理本画板所产生的“绘画”事件，则返回画板所对应的设备句柄（即HDC），否则返回 0。本命令为高级对象成员命令。...
EXTERN_C void KRNLN_NAME(403, GetHDC)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 404: 清除
// 类别: 易包
// 说明: 清除画板上指定区域的内容并将当前文本写出位置移动到被清除区左上角。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(404, cls)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 405: 取点
// 类别: 易包
// 说明: 返回画板上指定点的颜色值。如果失败，将返回 -1。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(405, GetPixel)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 406: 画点
// 类别: 易包
// 说明: 本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(406, SetPixel)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 407: 画直线
// 类别: 易包
// 说明: 使用画笔在画板上画出一条直线。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(407, LineTo)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 408: 画椭圆
// 类别: 易包
// 说明: 本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(408, ellipse)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 409: 画弧线
// 类别: 易包
// 说明: 使用画笔在画板上画出一条弧线。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(409, ArcTo)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 410: 画弦
// 类别: 易包
// 说明: 本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(410, chord)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 411: 画饼
// 类别: 易包
// 说明: 本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(411, pie)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 412: 画矩形
// 类别: 易包
// 说明: 使用画笔在画板上画出一个矩形，矩形的内部使用刷子填充。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(412, DrawRect)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 413: 画渐变矩形
// 类别: 易包
// 说明: 本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(413, DrawJBRect)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 414: 填充矩形
// 类别: 易包
// 说明: 将画板上指定的矩形区域用当前刷子填充。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(414, FillRect)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 415: 画圆角矩形
// 类别: 易包
// 说明: 使用画笔在画板上画出一个圆角矩形，圆角矩形的内部使用刷子填充。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(415, RoundRect)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 416: 翻转矩形区
// 类别: 易包
// 说明: 将画板上指定矩形区域的颜色翻转过来。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(416, InvertRect)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 417: 画多边形
// 类别: 易包
// 说明: 如果所画的多边形没有闭合，将自动闭合。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(417, polygon)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 418: 置写出位置
// 类别: 易包
// 说明: 设置下次使用“写文本行”或“写出”命令输出数据时的位置。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(418, SetWritePos)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 419: 写文本行
// 类别: 易包
// 说明: 在当前写出位置写出指定的文本、数值、逻辑值或日期时间，并将现行写出位置调整到下行行首。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(419, print)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 420: 滚动写行
// 类别: 易包
// 说明: 在当前写出位置写出指定的文本、数值、逻辑值或日期时间，并将现行写出位置调整到下行行首。如果现行画板高度无法容纳当前所要写出的行，则自动向上滚动画板内容。本命令为...
EXTERN_C void KRNLN_NAME(420, sprint)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 421: 写出
// 类别: 易包
// 说明: 在当前写出位置处写出指定的文本、数值、逻辑值或日期时间。自动调整现行写出位置到所写出数据的末位置。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添...
EXTERN_C void KRNLN_NAME(421, write)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 422: 定位写出
// 类别: 易包
// 说明: 在指定写出位置处写出指定的文本、数值、逻辑值或日期时间，不改变现行写出位置。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(422, say)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 423: 取宽度
// 类别: 易包
// 说明: 返回指定数据的写出宽度，使用当前绘画单位。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(423, GetWidth)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 424: 取高度
// 类别: 易包
// 说明: 返回指定数据的写出高度，使用当前绘画单位。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(424, GetHeight)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 425: 画图片
// 类别: 易包
// 说明: 本命令为初级对象成员命令。 1、#拷贝； 2、#翻转拷贝； 3、#位异或； 4、#位或； 5、#位与 本参数也可以指定透明色，但必须是负颜色数值。如： 画板1....
EXTERN_C void KRNLN_NAME(425, DrawPic)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 426: 取图片宽度
// 类别: 易包
// 说明: 返回指定图片的宽度，使用当前绘画单位。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(426, GetPicWidth)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 427: 取图片高度
// 类别: 易包
// 说明: 返回指定图片的高度，使用当前绘画单位。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(427, GetPicHeight)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 428: 复制
// 类别: 易包
// 说明: 将源画板（本命令的调用画板对象）中指定区域的内容快速复制到目的画板中的指定位置，使用源和目的画板各自的当前绘画单位。如果源画板当前不可视，其“自动重画”属性必须...
EXTERN_C void KRNLN_NAME(428, copy)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 429: 取图片
// 类别: 易包
// 说明: 返回画板上所有现有显示内容的图片数据。如果失败，返回空字节集。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(429, GetPic)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 430: 单位转换
// 类别: 易包
// 说明: 将像素单位座标值转换到当前绘画单位，或将当前绘画单位座标值转换到像素单位。本命令为初级对象成员命令。 成员属性“边框”所在数据类型为“画板”，英文名称为“bor...
EXTERN_C void KRNLN_NAME(430, UnitCnv)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 431: 调用反馈事件
// 类别: 易包
// 说明: 产生标签的反馈事件，以调用此标签的“反馈事件”用户事件处理子程序，可以用作在多线程处理中将控制权转移到程序主线程上去执行。返回用户事件处理子程序所返回的值，如果...
EXTERN_C void KRNLN_NAME(431, SendLabelMsg)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 432: 取顶端可见项目
// 类别: 易包
// 说明: 返回组合框列表部分中当前最顶端可见项目的索引。0 为项目一，1 为项目二，如此类推。失败返回 -1 。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(432, GetTopIndex)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 433: 置顶端可见项目
// 类别: 易包
// 说明: 设置组合框列表部分中当前最顶端的可见项目，必要时将自动滚动组合框的列表部分。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(433, SetTopIndex)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 434: 取项目数
// 类别: 易包
// 说明: 本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(434, GetCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 435: 取项目数值
// 类别: 易包
// 说明: 返回与指定项目相关联的数值。如果指定项目不存在，将返回 -1 。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(435, GetItemData)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 436: 置项目数值
// 类别: 易包
// 说明: 设置与指定项目相关联的数值。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(436, SetItemData)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 437: 取项目文本
// 类别: 易包
// 说明: 返回指定项目的文本。如果指定项目不存在，将返回空文本。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(437, GetItemText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 438: 置项目文本
// 类别: 易包
// 说明: 设置指定项目的文本。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(438, SetItemtext)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 439: 加入项目
// 类别: 易包
// 说明: 加入指定项目到组合框列表部分的尾部，成功返回加入后该项目所处的位置，失败返回 -1 。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(439, AddString)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 440: 插入项目
// 类别: 易包
// 说明: 插入指定项目到组合框列表部分的指定位置处，成功返回插入后该项目所处的位置，失败返回 -1 。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(440, InsertString)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 441: 删除项目
// 类别: 易包
// 说明: 删除组合框列表部分指定位置处的项目。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(441, DeleteString)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 442: 清空
// 类别: 易包
// 说明: 删除组合框列表部分中的所有项目。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(442, clear)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 443: 选择
// 类别: 易包
// 说明: 在所有项目中寻找首部包含指定文本的项目，如找到，则选中它，并返回该项目的位置索引，否则返回 -1。本命令为初级对象成员命令。 成员属性“类型”所在数据类型为“组...
EXTERN_C void KRNLN_NAME(443, SelItem)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 444: 取顶端可见项目
// 类别: 易包
// 说明: 返回列表框中当前最顶端可见项目的索引。0 为项目一，1 为项目二，如此类推。失败返回 -1 。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(444, GetTopIndex)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 445: 置顶端可见项目
// 类别: 易包
// 说明: 设置列表框中当前最顶端的可见项目，必要时将自动滚动列表框。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(445, SetTopIndex)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 446: 取项目数
// 类别: 易包
// 说明: 本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(446, GetCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 447: 取项目数值
// 类别: 易包
// 说明: 返回与指定项目相关联的数值。如果指定项目不存在，将返回 -1 。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(447, GetItemData)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 448: 置项目数值
// 类别: 易包
// 说明: 设置与指定项目相关联的数值。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(448, SetItemData)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 449: 取项目文本
// 类别: 易包
// 说明: 返回指定项目的文本。如果指定项目不存在，将返回空文本。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(449, GetItemText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 450: 置项目文本
// 类别: 易包
// 说明: 设置指定项目的文本。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(450, SetItemtext)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 451: 取已选择项目数
// 类别: 易包
// 说明: 返回已被选择项目的数目。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(451, GetSelCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 452: 取所有被选择项目
// 类别: 易包
// 说明: 返回一个整数数组，內含所有当前被选择项目的位置索引。如果当前没有被选择项目，返回空数组。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(452, GetSelItems)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 453: 是否被选择
// 类别: 易包
// 说明: 如果指定项目被选择，则返回真，否则返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(453, IsSelected)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 454: 选择项目
// 类别: 易包
// 说明: 选择或取消选择指定项目。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(454, select)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 455: 取焦点项目
// 类别: 易包
// 说明: 本命令仅在多选列表框中使用，用作返回当前焦点项目的位置索引。如果在单选列表框中使用本命令，将返回当前被选择项目的位置索引。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(455, GetCaretIndex)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 456: 置焦点项目
// 类别: 易包
// 说明: 本命令仅在多选列表框中使用，用作设置当前焦点项目。如果在单选列表框中使用本命令，将设置当前被选择项目。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(456, SetCaretIndex)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 457: 加入项目
// 类别: 易包
// 说明: 加入指定项目到列表框的尾部，成功返回加入后该项目所处的位置，失败返回 -1 。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(457, AddString)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 458: 插入项目
// 类别: 易包
// 说明: 插入指定项目到列表框的指定位置处，成功返回插入后该项目所处的位置，失败返回 -1 。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(458, InsertString)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 459: 删除项目
// 类别: 易包
// 说明: 删除列表框指定位置处的项目。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(459, DeleteString)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 460: 清空
// 类别: 易包
// 说明: 删除列表框中的所有项目。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(460, clear)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 461: 选择
// 类别: 易包
// 说明: 在所有项目中寻找首部包含指定文本的项目，如找到，则选中它，并返回该项目的位置索引，否则返回 -1 。本命令仅在单选列表框中使用，如果在多选列表框中使用，将返回 ...
EXTERN_C void KRNLN_NAME(461, SelItem)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 462: 取顶端可见项目
// 类别: 易包
// 说明: 返回列表框中当前最顶端可见项目的索引。0 为项目一，1 为项目二，如此类推。失败返回 -1 。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(462, GetTopIndex)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 463: 置顶端可见项目
// 类别: 易包
// 说明: 设置列表框中当前最顶端的可见项目，必要时将自动滚动列表框。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(463, SetTopIndex)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 464: 取项目数
// 类别: 易包
// 说明: 本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(464, GetCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 465: 取项目数值
// 类别: 易包
// 说明: 返回与指定项目相关联的数值。如果指定项目不存在，将返回 -1 。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(465, GetItemData)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 466: 置项目数值
// 类别: 易包
// 说明: 设置与指定项目相关联的数值。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(466, SetItemData)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 467: 取项目文本
// 类别: 易包
// 说明: 返回指定项目的文本。如果指定项目不存在，将返回空文本。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(467, GetItemText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 468: 置项目文本
// 类别: 易包
// 说明: 设置指定项目的文本。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(468, SetItemtext)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 469: 加入项目
// 类别: 易包
// 说明: 加入指定项目到列表框的尾部，成功返回加入后该项目所处的位置，失败返回 -1 。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(469, AddString)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 470: 插入项目
// 类别: 易包
// 说明: 插入指定项目到列表框的指定位置处，成功返回插入后该项目所处的位置，失败返回 -1 。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(470, InsertString)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 471: 删除项目
// 类别: 易包
// 说明: 删除列表框指定位置处的项目。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(471, DeleteString)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 472: 清空
// 类别: 易包
// 说明: 删除列表框中的所有项目。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(472, clear)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 473: 选择
// 类别: 易包
// 说明: 在所有项目中寻找首部包含指定文本的项目，如找到，则选中它，并返回该项目的位置索引，否则返回 -1。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(473, SelItem)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 474: 是否被选中
// 类别: 易包
// 说明: 如果与指定项目对应的选择框被选中，则返回真，否则返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(474, IsChecked)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 475: 选中项目
// 类别: 易包
// 说明: 选中或取消选中与指定项目对应的选择框。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(475, SetCheck)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 476: 是否被允许
// 类别: 易包
// 说明: 如果与指定项目对应的选择框被允许操作，则返回真，否则返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(476, IsEnabled)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 477: 允许
// 类别: 易包
// 说明: 允许或禁止对指定项目进行选择操作。成功返回真，失败返回假。本命令为初级对象成员命令。 成员属性“边框”所在数据类型为“选择列表框”，英文名称为“border”，...
EXTERN_C void KRNLN_NAME(477, enable)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 478: 取子夹数目
// 类别: 易包
// 说明: 本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(478, GetCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 479: 取子夹名称
// 类别: 易包
// 说明: 本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(479, GetName)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 480: 置子夹名称
// 类别: 易包
// 说明: 置入成功返回真，否则返回假。本命令为初级对象成员命令。 成员属性“表头方向”所在数据类型为“选择夹”，英文名称为“HeaderWay”，类型为“整数型（int）...
EXTERN_C void KRNLN_NAME(480, SetName)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 481: 跳转
// 类别: 易包
// 说明: 跳转到指定的邮件或 Internet 地址。本命令为初级对象成员命令。 成员属性“标题”所在数据类型为“超级链接框”，英文名称为“caption”，类型为“文本...
EXTERN_C void KRNLN_NAME(481, goto_)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 482: 打开
// 类别: 易包
// 说明: 打开当前类型的对话框，返回一个逻辑值。对于类型为“打开文件”、“保存文件”、“字体选择”的对话框，如果为真，表示用户已通过该对话框输入了有效数据，否则表示用户取...
EXTERN_C void KRNLN_NAME(482, open)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 483: 取设备句柄
// 类别: 易包
// 说明: 如当前用户程序正在进行打印作业，则返回对应的打印机设备句柄（即HDC），否则返回 0。本命令为高级对象成员命令。...
EXTERN_C void KRNLN_NAME(483, GetHDC)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 484: 开始打印
// 类别: 易包
// 说明: 返回真表示已经成功地进入了打印作业，进入打印作业后文本写出位置将被重置为 0 。如果执行本命令时已经在打印作业中或者用户在打印设置对话框中选择了取消，返回假。进...
EXTERN_C void KRNLN_NAME(484, StartDoc)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 485: 结束打印
// 类别: 易包
// 说明: 如果当前已经进入了打印作业，调用本命令可完成此作业。结束打印后易程序进行的后续打印绘画操作都将被忽略。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(485, EndDoc)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 486: 取消打印
// 类别: 易包
// 说明: 如果当前已经进入了打印作业，调用本命令可立即中止且取消该作业。如果操作系统的打印管理器正在处理该打印作业（打印管理器正在运行并且允许后台打印），那么该作业将被删...
EXTERN_C void KRNLN_NAME(486, AbortDoc)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 487: 换页
// 类别: 易包
// 说明: 调用本命令使打印机完成当前页的打印，文本写出位置被重置为 0 ，并走纸到下一页左上角。如果换页成功返回真，否则将自动取消打印并返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(487, NewPage)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 488: 开始下一份
// 类别: 易包
// 说明: 调用本命令后将结束在当前页上的打印作业，并将当前打印份数加一，当前页号置回到首页页号，以进入下一份拷贝的打印。如果成功返回真，失败将自动取消打印并返回假。本命令...
EXTERN_C void KRNLN_NAME(488, NewCopy)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 489: 画点
// 类别: 易包
// 说明: 本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(489, SetPixel)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 490: 画直线
// 类别: 易包
// 说明: 使用画笔在画板上画出一条直线。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(490, LineTo)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 491: 画椭圆
// 类别: 易包
// 说明: 本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(491, ellipse)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 492: 画弧线
// 类别: 易包
// 说明: 使用画笔在画板上画出一条弧线。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(492, ArcTo)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 493: 画弦
// 类别: 易包
// 说明: 本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(493, chord)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 494: 画饼
// 类别: 易包
// 说明: 本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(494, pie)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 495: 画矩形
// 类别: 易包
// 说明: 使用画笔在画板上画出一个矩形，矩形的内部使用刷子填充。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(495, DrawRect)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 496: 画渐变矩形
// 类别: 易包
// 说明: 本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(496, DrawJBRect)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 497: 填充矩形
// 类别: 易包
// 说明: 将画板上指定的矩形区域用当前刷子填充。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(497, FillRect)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 498: 画圆角矩形
// 类别: 易包
// 说明: 使用画笔在画板上画出一个圆角矩形，圆角矩形的内部使用刷子填充。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(498, RoundRect)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 499: 画多边形
// 类别: 易包
// 说明: 本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(499, polygon)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 500: 置写出位置
// 类别: 易包
// 说明: 设置下次使用“写文本行”或“写出”命令输出数据时的位置。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(500, SetWritePos)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 501: 写文本行
// 类别: 易包
// 说明: 在当前写出位置写出指定的文本、数值、逻辑值或日期时间，并将现行写出位置调整到下行行首。支持自动换页。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复...
EXTERN_C void KRNLN_NAME(501, print)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 502: 写出
// 类别: 易包
// 说明: 在当前写出位置处写出指定的文本、数值、逻辑值或日期时间。自动调整现行写出位置到所写出数据的末位置。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添...
EXTERN_C void KRNLN_NAME(502, write)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 503: 定位写出
// 类别: 易包
// 说明: 在指定写出位置处写出指定的文本、数值、逻辑值或日期时间，不改变现行写出位置。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(503, say)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 504: 取宽度
// 类别: 易包
// 说明: 返回指定数据的写出宽度，使用当前绘画单位。如果调用本命令时尚未进入打印，将返回 0 。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(504, GetWidth)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 505: 取高度
// 类别: 易包
// 说明: 返回指定数据的写出高度，使用当前绘画单位。如果调用本命令时尚未进入打印，将返回 0 。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(505, GetHeight)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 506: 画图片
// 类别: 易包
// 说明: 本命令为初级对象成员命令。 1、#拷贝； 2、#翻转拷贝； 3、#位异或； 4、#位或； 5、#位与 本参数也可以指定透明色，但必须是负颜色数值。如： 画板1....
EXTERN_C void KRNLN_NAME(506, DrawPic)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 507: 取图片宽度
// 类别: 易包
// 说明: 返回指定图片的宽度。如果调用本命令时尚未进入打印，使用像素点为单位，否则使用当前绘画单位。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(507, GetPicWidth)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 508: 取图片高度
// 类别: 易包
// 说明: 返回指定图片的高度。如果调用本命令时尚未进入打印，使用像素点为单位，否则使用当前绘画单位。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(508, GetPicHeight)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 509: 置自定义纸张类型
// 类别: 易包
// 说明: 添加或修改自定义纸张类型。本方法仅在Windows NT/2000/XP下有效，且要求具有打印机驱动的完全控制权限。注意：太小或太大的纸张大小和页边距，可能不被...
EXTERN_C void KRNLN_NAME(509, SetCustomPaperType)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 510: 删除自定义纸张类型
// 类别: 易包
// 说明: 删除指定自定义纸张类型。本方法仅在Windows NT/2000/XP下有效，且要求具有打印机驱动的完全控制权限。本命令为高级对象成员命令。...
EXTERN_C void KRNLN_NAME(510, DeleteCustomPaperType)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 511: 取自定义纸张大小
// 类别: 易包
// 说明: 取指定自定义纸张类型的纸张大小及边距，并写入后面的六个参数中。如果指定的自定义纸张类型不存在，返回“假”。本方法仅在Windows NT/2000/XP下有效，...
EXTERN_C void KRNLN_NAME(511, GetCustomPaperType)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 512: 取所有纸张类型
// 类别: 易包
// 说明: 取当前默认打印机所支持的所有纸张类型（包括系统定义纸张类型和自定义纸张类型），返回一维文本数组，其中存放了的各纸张类型的名称。本方法仅在Windows NT/2...
EXTERN_C void KRNLN_NAME(512, SetCustomPaperType)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 513: 单位转换
// 类别: 易包
// 说明: 将像素单位座标值转换到当前绘画单位，或将当前绘画单位座标值转换到像素单位。注意在执行本方法之前，必须已经开始打印。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(513, UnitCnv)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 514: 取设备名称
// 类别: 易包
// 说明: 如果已经开始打印，则返回当前打印机设备的名称。本命令为高级对象成员命令。 成员属性“绘画单位”所在数据类型为“打印机”，英文名称为“unit”，类型为“整数型（...
EXTERN_C void KRNLN_NAME(514, GetPrinterDeviceName)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 515: 发送数据
// 类别: 易包
// 说明: 发送数据到指定主机上的指定端口。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(515, send)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 516: 取回数据
// 类别: 易包
// 说明: 取回所接收到的数据。本命令必须在“数据到达”事件的处理子程序中使用。本命令为初级对象成员命令。 成员属性“端口”所在数据类型为“数据报”，英文名称为“port”...
EXTERN_C void KRNLN_NAME(516, recv)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 517: 连接
// 类别: 易包
// 说明: 连接到指定主机上的指定端口，该主机上的该端口必须已经被某一服务器组件监听。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(517, connect)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 518: 断开连接
// 类别: 易包
// 说明: 断开与服务器的已有连接。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(518, CloseConnect)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 519: 发送数据
// 类别: 易包
// 说明: 在成功建立与服务器的连接后，发送数据到服务器端组件。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(519, send)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 520: 取回数据
// 类别: 易包
// 说明: 取回所接收到的数据。本命令必须在“数据到达”事件的处理子程序中使用。本命令为初级对象成员命令。 事件名称：数据到达 当服务器端将数据发送过来后，会产生本事件。在...
EXTERN_C void KRNLN_NAME(520, recv)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 521: 取回数据
// 类别: 易包
// 说明: 取回所接收到的数据。本命令必须在“数据到达”事件的处理子程序中使用。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(521, recv)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 522: 取回客户
// 类别: 易包
// 说明: 当接收到“客户进入”、“客户离开”或“数据到达”事件时，在该事件的处理子程序中可调用本方法取回对应的客户地址（IP地址 + 端口）。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(522, GetClient)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 523: 发送数据
// 类别: 易包
// 说明: 向指定已经连接进来的客户发送数据。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(523, send)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 524: 断开客户
// 类别: 易包
// 说明: 断开与指定客户之间的连接。本命令为初级对象成员命令。 成员属性“端口”所在数据类型为“服务器”，英文名称为“port”，类型为“整数型（int）”。 指定监听数...
EXTERN_C void KRNLN_NAME(524, CloseClient)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 525: 启动
// 类别: 易包
// 说明: 启动或重新启动对指定端口的操作。在对端口进行操作之前必须首先启动，如在端口启动后又更改了端口属性必须重新启动。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(525, start)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 526: 停止
// 类别: 易包
// 说明: 关闭已经启动的指定端口。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(526, stop)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 527: 发送数据
// 类别: 易包
// 说明: 从端口发送指定的数据。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(527, send)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 528: 信号操作
// 类别: 易包
// 说明: 本命令可以设置或清除通讯端口上指定信号的状态。成功返回真，失败返回假。本命令为初级对象成员命令。 成员属性“端口号”所在数据类型为“端口”，英文名称为“port...
EXTERN_C void KRNLN_NAME(528, Signal)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 529: 置光标
// 类别: 易包
// 说明: 本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(529, SetCaret)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 530: 选择
// 类别: 易包
// 说明: 本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(530, Select)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 531: 全部选择
// 类别: 易包
// 说明: 本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(531, SelectAll)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 532: 取光标行号
// 类别: 易包
// 说明: 本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(532, CaretRow)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 533: 取光标列号
// 类别: 易包
// 说明: 本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(533, CaretCol)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 534: 取选择行数
// 类别: 易包
// 说明: 被选择的行区域起始于光标所在行。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(534, SelectRows)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 535: 取选择列数
// 类别: 易包
// 说明: 被选择的列区域起始于光标所在列。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(535, SelectCols)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 536: 等宽缩放
// 类别: 易包
// 说明: 将表格内容缩放到与表格窗口等宽显示。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(536, InWin)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 537: 全部复制
// 类别: 易包
// 说明: 复制所有表格数据到剪贴板。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(537, CopyAll)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 538: 复制
// 类别: 易包
// 说明: 复制指定区域表格数据到剪贴板。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(538, Copy)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 539: 粘贴到光标处
// 类别: 易包
// 说明: 粘贴剪贴板中的表格数据到当前光标处。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(539, PasteToCaret)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 540: 粘贴
// 类别: 易包
// 说明: 粘贴剪贴板中的表格数据到指定位置。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(540, Paste)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 541: 打印
// 类别: 易包
// 说明: 打印表格数据到打印机。注意欲设置打印份数等打印设置信息请到与表格相连接的数据源中设置。成功返回空文本，失败(不包含操作者中断打印)返回非空错误文本。本命令为初级...
EXTERN_C void KRNLN_NAME(541, Print)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 542: 打印预览
// 类别: 易包
// 说明: 本命令为初级对象成员命令。 成员属性“数据源”所在数据类型为“表格”，英文名称为“DataSource”，类型为“文本型（text）”。 指定提供数据的数据源组...
EXTERN_C void KRNLN_NAME(542, PrintPreview)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 543: 到首记录
// 类别: 易包
// 说明: 将数据源中当前记录指针移动到第一条记录上。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(543, GoTop)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 544: 到尾记录
// 类别: 易包
// 说明: 将数据源中当前记录指针移动到最后一条记录上。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(544, GoBottom)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 545: 跳过
// 类别: 易包
// 说明: 将数据源中当前记录指针向前或者向后移动数条记录。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(545, skip)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 546: 跳到
// 类别: 易包
// 说明: 改变数据源中的当前记录指针到指定的记录号。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(546, goto_)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 547: 取记录号
// 类别: 易包
// 说明: 返回数据源当前记录指针所指向记录的记录号。记录号从1开始。如果当前记录指针无效，所返回记录号为0。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(547, RecNO)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 548: 取表头行数
// 类别: 易包
// 说明: 返回当以表格的形式表现数据源中的数据时表头所占的行数。表头行在表格中显示时不会滚动，在打印时会自动打印在每一页的顶部。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(548, GetFixedRowCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 549: 置表头行数
// 类别: 易包
// 说明: 设置当以表格的形式表现数据源中的数据时表头所占的行数。表头行在表格中显示时不会滚动，在打印时会自动打印在每一页的顶部。注意如果数据源所使用的数据提供者不支持此特...
EXTERN_C void KRNLN_NAME(549, SetFixedRowCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 550: 取表头列数
// 类别: 易包
// 说明: 返回当以表格的形式表现数据源中的数据时表头所占的列数。表头列在表格中显示时不会滚动。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(550, GetFixedColCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 551: 置表头列数
// 类别: 易包
// 说明: 设置当以表格的形式表现数据源中的数据时表头所占的列数。表头列在表格中显示时不会滚动。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对...
EXTERN_C void KRNLN_NAME(551, SetFixedColCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 552: 取行高
// 类别: 易包
// 说明: 返回数据源中指定行在表现时的高度，单位为0.1毫米。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(552, GetRowHeight)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 553: 置行高
// 类别: 易包
// 说明: 设置数据源中数据行在表现时的高度，单位为0.1毫米。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(553, SetRowHeight)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 554: 取列宽
// 类别: 易包
// 说明: 返回数据源中指定列在表现时的宽度，单位为0.1毫米。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(554, GetColWidth)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 555: 置列宽
// 类别: 易包
// 说明: 设置数据源中数据列在表现时的宽度，单位为0.1毫米。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(555, SetColWidth)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 556: 取类型
// 类别: 易包
// 说明: 返回数据源中指定单元格的数据类型。返回值为以下常量值之一： 1、#文本； 2、#图片文件名； 3、#图片数据； 4、#字节集数据。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(556, GetType)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 557: 置类型
// 类别: 易包
// 说明: 设置数据源中指定单元格的数据类型。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(557, SetType)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 558: 取文本色
// 类别: 易包
// 说明: 返回数据源中指定单元格在表现时的文本颜色。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(558, GetTextColor)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 559: 置文本色
// 类别: 易包
// 说明: 设置数据源中指定单元格在表现时的文本颜色。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(559, SetTextColor)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 560: 取背景色
// 类别: 易包
// 说明: 返回数据源中指定单元格在表现时的背景颜色。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(560, GetBackground)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 561: 置背景色
// 类别: 易包
// 说明: 设置数据源中指定单元格在表现时的背景颜色。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(561, SetBackground)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 562: 取字体名
// 类别: 易包
// 说明: 返回数据源中指定单元格在表现时所使用字体的名称。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(562, GetFontName)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 563: 置字体名
// 类别: 易包
// 说明: 设置数据源中指定单元格在表现时所使用字体的名称。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(563, SetFontName)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 564: 取字体尺寸
// 类别: 易包
// 说明: 返回数据源中指定单元格在表现时所使用字体的尺寸，单位为0.1毫米。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(564, GetFontSize)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 565: 置字体尺寸
// 类别: 易包
// 说明: 设置数据源中指定单元格在表现时所使用字体的尺寸，单位为0.1毫米。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(565, SetFontSize)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 566: 取字体属性
// 类别: 易包
// 说明: 返回数据源中指定单元格在表现时所使用字体的属性。返回值为以下常量值之一或之和： 1、#粗体； 2、#斜体； 4、#下划线； 8、#删除线。本命令为初级对象成员命...
EXTERN_C void KRNLN_NAME(566, GetFontAttr)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 567: 置字体属性
// 类别: 易包
// 说明: 设置数据源中指定单元格在表现时所使用字体的属性。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(567, SetFontAttr)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 568: 取边距
// 类别: 易包
// 说明: 返回数据源中指定单元格在表现其中数据时至其单元格边框之间的空白距离，单位为0.1毫米。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(568, GetExtra)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 569: 置边距
// 类别: 易包
// 说明: 设置数据源中指定单元格在表现其中数据时至其单元格边框之间的空白距离，单位为0.1毫米。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级...
EXTERN_C void KRNLN_NAME(569, SetExtra)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 570: 取文本输入格式
// 类别: 易包
// 说明: 返回数据源中某文本型单元格的输入格式。当新的数据通过输入方式更新到该单元格内之前，将首先自动根据此格式转换。返回值为以下常量值之一： 0、#通常型； 1、#字节...
EXTERN_C void KRNLN_NAME(570, GetInputType)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 571: 置文本输入格式
// 类别: 易包
// 说明: 设置数据源中某文本型单元格的输入格式。当新的数据通过输入方式更新到该单元格内之前，将首先自动根据此格式转换。注意如果数据源所使用的数据提供者不支持此特性，本命令...
EXTERN_C void KRNLN_NAME(571, SetInputType)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 572: 取对齐方式
// 类别: 易包
// 说明: 返回数据源中指定单元格在表现时所使用的对齐方式。返回值为以下常量值之一： 1、#上左； 2、#上中； 3、#上右； 4、#中左； 5、#中中； 6、#中右； 7...
EXTERN_C void KRNLN_NAME(572, GetAlignMode)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 573: 置对齐方式
// 类别: 易包
// 说明: 设置数据源中指定单元格在表现时所使用的对齐方式。注意如果数据源所使用的数据提供者不支持某些属性，该属性将被忽略。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(573, SetAlignMode)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 574: 取密码方式
// 类别: 易包
// 说明: 返回数据源中指定单元格在表现时是否使用密码方式。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(574, GetPwdMode)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 575: 置密码方式
// 类别: 易包
// 说明: 如设置值为真，则数据源中指定单元格在表现时以密码字符呈现。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(575, SetPwdMode)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 576: 取文本
// 类别: 易包
// 说明: 返回数据源中指定单元格的文本内容。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(576, GetText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 577: 置文本
// 类别: 易包
// 说明: 设置数据源中指定单元格的文本内容，注意该单元格类型必须为“#文本”或“#图片文件名”。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(577, SetText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 578: 取数据
// 类别: 易包
// 说明: 返回数据源中指定单元格的图片或字节集数据内容。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(578, GetData)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 579: 置数据
// 类别: 易包
// 说明: 设置数据源中指定单元格的图片或字节集数据内容，注意该单元格类型必须为“#图片数据”或“#字节集数据”。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(579, SetData)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 580: 合并
// 类别: 易包
// 说明: 组合数据源中指定范围内的单元格，使之以一个单元格的形式表现。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(580, Cmb)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 581: 分解
// 类别: 易包
// 说明: 分解数据源中指定的已经组合的单元格，行列参数指向被组合单元格内的任何一个单元格即可。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对...
EXTERN_C void KRNLN_NAME(581, Split)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 582: 是否被合并
// 类别: 易包
// 说明: 如果数据源中指定单元格与周边指定单元格有组合关系，返回真，否则返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(582, HasCmb)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 583: 加线条
// 类别: 易包
// 说明: 为数据源中指定范围内单元格添加线条。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(583, AddLine)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 584: 删线条
// 类别: 易包
// 说明: 将数据源中指定范围内单元格内的线条去除。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(584, RemoveLine)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 585: 是否有线条
// 类别: 易包
// 说明: 如果数据源中指定单元格有指定线条，返回真，否则返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(585, HasLine)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 586: 清除
// 类别: 易包
// 说明: 清除数据源中指定范围内单元格内容为空文本。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(586, EmptyCell)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 587: 置初始属性
// 类别: 易包
// 说明: 设置在数据源中初始或者添加新单元格时默认的单元格初始属性。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(587, SetInitData)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 588: 初始尺寸
// 类别: 易包
// 说明: 初始数据源中数据的行列数，注意数据源中所有原有数据将被清除。注意某些数据提供者可能不支持此方法。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(588, InitSize)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 589: 取行数
// 类别: 易包
// 说明: 返回数据源中现行数据行数。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(589, GetRowCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 590: 取列数
// 类别: 易包
// 说明: 返回数据源中现行数据列数。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(590, GetColCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 591: 插入行
// 类别: 易包
// 说明: 在数据源中指定位置处插入新数据行。注意某些数据提供者可能不支持在中间插入行。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(591, InsertRow)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 592: 添加行
// 类别: 易包
// 说明: 在数据源中最后一行后插入新数据行。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(592, AppendRow)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 593: 删除行
// 类别: 易包
// 说明: 在数据源中指定位置处删除数据行。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(593, RemoveRow)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 594: 插入列
// 类别: 易包
// 说明: 在数据源中指定位置处插入新数据列。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(594, InsertCol)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 595: 删除列
// 类别: 易包
// 说明: 在数据源中指定位置处删除数据列。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(595, RemoveCol)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 596: 添加
// 类别: 易包
// 说明: 将指定其它数据源或者数据提供者中的数据添加到本数据源的尾部。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(596, Append)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 597: 存到字节集
// 类别: 易包
// 说明: 将数据源中所有现有数据保存在字节集中返回。如果失败，将返回空字节集。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(597, SaveDS)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 598: 从字节集读
// 类别: 易包
// 说明: 将指定字节集内的数据源数据写入到数据源中，数据源内所有原有数据将被清除。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(598, LoadDS)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 599: 存到文件
// 类别: 易包
// 说明: 将数据源中所有现有数据保存到指定文件中。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(599, SaveDSFile)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 600: 从文件读
// 类别: 易包
// 说明: 将指定文件内的数据源数据写入到数据源中，数据源内所有原有数据将被清除。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(600, LoadDSFile)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 601: 单元格到字节集
// 类别: 易包
// 说明: 将数据源中指定范围内单元格数据保存在字节集中返回。如果失败，将返回空字节集。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(601, SaveDSCell)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 602: 字节集到单元格
// 类别: 易包
// 说明: 将指定字节集内的单元格数据写入到数据源中指定位置。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(602, LoadDSCell)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 603: 单元格到文件
// 类别: 易包
// 说明: 将数据源中指定范围内单元格数据保存到指定文件中。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(603, SaveDSCellFile)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 604: 文件到单元格
// 类别: 易包
// 说明: 将指定文件内的单元格数据写入到数据源中指定位置。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(604, LoadDSCellFile)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 605: 刷新
// 类别: 易包
// 说明: 通知所有基于本数据源的数据处理者，使其自动从数据源重新获取并显示数据。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(605, Refrush)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 606: 保存更改
// 类别: 易包
// 说明: 通知所有基于本数据源的数据处理者，使其自动将处理或更改后的数据写回到数据源中。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(606, SaveChange)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 607: 打印设置
// 类别: 易包
// 说明: 调用对话框设置数据的打印配置信息。当操作者按确认按钮退出对话框时返回真，否则返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(607, SetupPrinter)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 608: 取打印设置
// 类别: 易包
// 说明: 返回打印数据源数据时所将使用的设置信息。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(608, GetPrintInf)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 609: 置打印设置
// 类别: 易包
// 说明: 设置打印数据源数据时所将使用的设置信息。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(609, SetPrintInf)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 610: 取打印页宽
// 类别: 易包
// 说明: 返回打印数据源数据时所将使用打印纸的正文区域宽度，单位为0.1毫米。注意：1、此宽度不包含页边距；2、“打印设置”命令执行后，如果操作者改变了纸张类型，此值将自...
EXTERN_C void KRNLN_NAME(610, GetPageWidth)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 611: 取打印页高
// 类别: 易包
// 说明: 返回打印数据源数据时所将使用打印纸的正文区域高度，单位为0.1毫米。注意：1、此宽度不包含页边距；2、“打印设置”命令执行后，如果操作者改变了纸张类型，此值将自...
EXTERN_C void KRNLN_NAME(611, GetPageHeight)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 612: 取只读方式
// 类别: 易包
// 说明: 返回数据源中指定单元格在表现时是否只读。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(612, GetReadOnly)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 613: 置只读方式
// 类别: 易包
// 说明: 如设置值为真，则数据源中指定单元格在表现时只能读取而不能写入。注意： 1、如果数据处理者不支持此特性，则本属性无效； 2、如果数据提供者不支持此特性，本命令将被...
EXTERN_C void KRNLN_NAME(613, SetReadOnly)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 614: 打开
// 类别: 易包
// 说明: 打开指定的ODBC数据源以供以后操作。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(614, open)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 615: 关闭
// 类别: 易包
// 说明: 关闭当前被打开的数据库。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(615, close)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 616: 取连接文本
// 类别: 易包
// 说明: 返回当前被打开数据库的ODBC数据源连接文本。失败返回空文本。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(616, GetConnect)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 617: 启动事务
// 类别: 易包
// 说明: 启动当前被打开数据库的事务，成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(617, BeginTrans)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 618: 回滚事务
// 类别: 易包
// 说明: 回滚当前被打开数据库的事务，所有在上一次事务提交前对数据库所作的修改均被取消。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(618, RollbackTrans)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 619: 提交事务
// 类别: 易包
// 说明: 提交当前被打开数据库的事务。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(619, CommitTrans)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 620: 查询
// 类别: 易包
// 说明: 对当前被打开数据库进行数据查询，成功返回结果记录集句柄，失败返回0。注意当不再使用此记录集时，必须使用“关闭记录集”命令将其关闭。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(620, query)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 621: 重新查询
// 类别: 易包
// 说明: 对指定的记录集进行重新查询，并将其当前记录指针重定位到首记录。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(621, Requery)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 622: 关闭记录集
// 类别: 易包
// 说明: 关闭指定的记录集。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(622, CloseRecordset)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 623: 首记录前
// 类别: 易包
// 说明: 如果指定记录集的当前记录指针已在首记录的前面，返回真，否则返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(623, bof)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 624: 尾记录后
// 类别: 易包
// 说明: 如果指定记录集的当前记录指针已在尾记录的后面，返回真，否则返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(624, eof)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 625: 到首记录
// 类别: 易包
// 说明: 将指定记录集的当前记录指针移动到第一条记录上。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(625, GoTop)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 626: 到尾记录
// 类别: 易包
// 说明: 将指定记录集的当前记录指针移动到最后一条记录上。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(626, GoBottom)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 627: 到前一记录
// 类别: 易包
// 说明: 将指定记录集的当前记录指针向前移动一条记录。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(627, GoPrev)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 628: 到后一记录
// 类别: 易包
// 说明: 将指定记录集的当前记录指针向后移动一条记录。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(628, GoNext)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 629: 读
// 类别: 易包
// 说明: 返回指定记录集的当前记录处指定字段的数据内容，所返回数据的类型与字段SQL类型对照表如下： 返回数据类型   字段SQL类型 -----------------...
EXTERN_C void KRNLN_NAME(629, r)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 630: 执行
// 类别: 易包
// 说明: 执行指定的非查询类SQL语句，SQL语句中的列名（尤其是中文列名）可用中括号括住。如果SQL语句中涉及到备注或者字节集型字段，请在相应位置加上问号，然后再加上对...
EXTERN_C void KRNLN_NAME(630, ExecuteSql)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 631: 打开MDB数据库
// 类别: 易包
// 说明: 本命令是针对 ACCESS MDB 数据库的特定打开方法，通过自建 ODBC 连接文本来打开 MDB 数据库。调用本命令后无需再调用“打开”命令。成功返回真，失...
EXTERN_C void KRNLN_NAME(631, OpenMDB)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 632: 打开SQL数据库
// 类别: 易包
// 说明: 本命令是针对 SQL SERVER 数据库的特定打开方法，通过自建 ODBC 连接文本来打开 SQL SERVER 数据库。调用本命令后无需再调用“打开”命令。...
EXTERN_C void KRNLN_NAME(632, OpenSqlServerDB)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 633: 创建
// 类别: 易包
// 说明: 创建指定类型的 COM 对象，本对象中的原有内容将被释放。成功返回真，否则返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(633, CreateObject)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 634: 获取
// 类别: 易包
// 说明: 获取当前操作系统中已经存在的指定类型 COM 对象，本对象中的原有内容将被释放。成功返回真，否则返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(634, GetObject)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 635: 清除
// 类别: 易包
// 说明: 将本对象的内容释放并清空。如果不调用本方法，则对象在退出其作用区域时会自动被释放。例如：假设对象存在于子程序局部变量中，当子程序调用退出时，该对象会被自动释放。...
EXTERN_C void KRNLN_NAME(635, Clear)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 636: 是否为空
// 类别: 易包
// 说明: 如果本对象的内容为空，返回真，否则返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(636, IsEmpty)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 637: 是否相等
// 类别: 易包
// 说明: 如果本对象的内容与指定对象的内容相等，返回真，否则返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(637, IsEqual)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 638: 读文本属性
// 类别: 易包
// 说明: 读取并返回本对象的指定文本类型属性值，如果失败将返回空文本，且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参数可以被...
EXTERN_C void KRNLN_NAME(638, GetTextProperty)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 639: 读数值属性
// 类别: 易包
// 说明: 读取并返回本对象的指定数值类型属性值，如果失败将返回数值0，且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参数可以被...
EXTERN_C void KRNLN_NAME(639, GetNumProperty)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 640: 读逻辑属性
// 类别: 易包
// 说明: 读取并返回本对象的指定逻辑型属性值，如果失败将返回假，且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添...
EXTERN_C void KRNLN_NAME(640, GetBoolProperty)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 641: 读日期属性
// 类别: 易包
// 说明: 读取并返回本对象的指定日期时间型属性值，如果失败将返回100年1月1日，且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一...
EXTERN_C void KRNLN_NAME(641, GetDateProperty)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 642: 读对象型属性
// 类别: 易包
// 说明: 读取并返回本对象的指定对象属性值，如果失败将返回内容为空的对象，且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参数可...
EXTERN_C void KRNLN_NAME(642, GetObjectProperty)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 643: 读属性
// 类别: 易包
// 说明: 读取并返回本对象的指定属性值，本方法可以用作读取任意类型的属性。如果失败将返回类型值为空的变体型对象，且紧跟其后执行“取错误”方法将返回非空文本，否则将返回包含...
EXTERN_C void KRNLN_NAME(643, GetProperty)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 644: 写属性
// 类别: 易包
// 说明: 设置本对象指定属性的值，如果失败，返回假，并且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(644, SetProperty)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 645: 方法
// 类别: 易包
// 说明: 执行本对象的指定方法并忽略其返回值，如果失败，紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(645, RunMethod)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 646: 通用方法
// 类别: 易包
// 说明: 执行本对象的指定方法并返回一个变体型对象值，该对象内记录该方法的返回数据。本命令可以用作执行返回任何数据类型数据的方法，如果该方法没有返回数据，所返回变体型对象...
EXTERN_C void KRNLN_NAME(646, RunVariantMethod)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 647: 文本方法
// 类别: 易包
// 说明: 执行本对象返回文本类型数据的方法，如果失败将返回空文本，且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复...
EXTERN_C void KRNLN_NAME(647, RunTextMethod)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 648: 数值方法
// 类别: 易包
// 说明: 执行本对象返回数值类型数据的方法，如果失败将返回数值0，且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复...
EXTERN_C void KRNLN_NAME(648, RunNumMethod)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 649: 逻辑方法
// 类别: 易包
// 说明: 执行本对象返回逻辑型数据的方法，如果失败将返回假，且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添加。...
EXTERN_C void KRNLN_NAME(649, RunBoolMethod)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 650: 日期方法
// 类别: 易包
// 说明: 执行本对象返回日期时间型数据的方法，如果失败将返回100年1月1日，且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参...
EXTERN_C void KRNLN_NAME(650, RunDateMethod)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 651: 对象型方法
// 类别: 易包
// 说明: 执行本对象返回对象型数据的方法，如果失败将返回内容为空的对象，且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参数可以...
EXTERN_C void KRNLN_NAME(651, RunObjectMethod)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 652: 创建图片对象
// 类别: 易包
// 说明: 为指定图片数据创建对应的 COM 图片对象，本对象中的原有内容将被释放。成功返回真，否则返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(652, CreatePicDispObj)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 653: 创建字体对象
// 类别: 易包
// 说明: 为指定字体数据创建对应的 COM 字体对象，本对象中的原有内容将被释放。成功返回真，否则返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(653, CreateFontDispObj)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 654: 取回图片
// 类别: 易包
// 说明: 如果本对象为 COM 图片对象，则取回其具体图片数据。成功返回图片数据字节集，失败返回空字节集。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(654, GetPic)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}

// 命令 655: 取回字体
// 类别: 易包
// 说明: 如果本对象为 COM 字体对象，则取回其具体字体数据。如果失败，紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(655, GetFont)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 656: 取错误
// 类别: 易包
// 说明: 当读写对象属性、执行对象方法或取回字体时，紧跟该语句后执行本方法可以检查其是否执行成功。如果成功，本命令将返回空文本，如果失败，本命令将返回一个描述具体错误信息...
EXTERN_C void KRNLN_NAME(656, GetErrorText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 657: 查看
// 类别: 易包
// 说明: 本命令仅在易程序的调试版本中被执行，在发布版本中将被直接跳过。 通过对话框的方式查看本对象的调用格式信息，便于编写相关程序。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(657, ViewObjInf)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 658: 取接口
// 类别: 易包
// 说明: 获取指定对象或者OCX窗口组件中的指定接口，本对象中的原有内容将被释放。成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(658, QueryInterface)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 659: 清除
// 类别: 易包
// 说明: 将本对象的内容释放并清空。如果不调用本方法，则对象在退出其作用区域时会自动被释放。例如：假设对象存在于子程序局部变量中，当子程序调用退出时，该对象会被自动释放。...
EXTERN_C void KRNLN_NAME(659, Clear)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 660: 取类型
// 类别: 易包
// 说明: 取回当前变体型对象的数据类型。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(660, GetType)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 661: 取数组成员数
// 类别: 易包
// 说明: 如果本对象中存放的是数组数据，执行本方法将返回其成员数目，如果不是数组数据，执行本方法将返回 -1 。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(661, GetElementCount)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_int = 0;
}

// 命令 662: 取文本
// 类别: 易包
// 说明: 返回本对象中或本对象数组成员中的文本数据，如果当前数据的数据类型不为文本型，将自动进行转换，如果转换失败将返回空文本。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(662, GetText)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pText = NULL;
}

// 命令 663: 取数值
// 类别: 易包
// 说明: 返回本对象中或本对象数组成员中的数值数据，如果当前数据的数据类型不为数值或错误值型，将自动进行转换，如果转换失败将返回 0 。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(663, GetNum)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_double = 0.0;
}

// 命令 664: 取逻辑值
// 类别: 易包
// 说明: 返回本对象中或本对象数组成员中的逻辑值数据，如果当前数据的数据类型不为逻辑型，将自动进行转换，如果转换失败将返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(664, GetBool)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 665: 取日期
// 类别: 易包
// 说明: 返回本对象中或本对象数组成员中的日期时间值数据，如果当前数据的数据类型不为日期时间型，将自动进行转换，如果转换失败将返回100年1月1日。本命令为初级对象成员命...
EXTERN_C void KRNLN_NAME(665, GetDateTime)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 666: 取对象
// 类别: 易包
// 说明: 返回本对象中或本对象数组成员中的COM对象型数据，如果当前数据的数据类型不为COM对象型，将返回空COM对象。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(666, GetObject)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 667: 取变体型
// 类别: 易包
// 说明: 返回本对象中或本对象数组成员中的变体型数据类型数据，如果当前数据的数据类型不为变体型，将自动进行转换。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(667, GetVariant)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}

// 命令 668: 赋值
// 类别: 易包
// 说明: 设置本对象的内容，成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(668, set)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 669: 创建数组
// 类别: 易包
// 说明: 创建指定类型和指定成员数的空白数组，对象中的原有内容将被清空，成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(669, CreateArray)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 670: 置类型
// 类别: 易包
// 说明: 清除当前变体型对象中的原有内容，并改变它的数据类型，成功返回真，失败返回假。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(670, SetType)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_bool = FALSE;
}

// 命令 671: 取字节集
// 类别: 易包
// 说明: 返回本对象中或本对象数组成员中的字节集数据，如果当前数据的数据类型不为字节集型，将自动进行转换，如果转换失败将返回空字节集。本命令为初级对象成员命令。...
EXTERN_C void KRNLN_NAME(671, GetBin)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
    pRetData->m_pBin = NULL;
}
