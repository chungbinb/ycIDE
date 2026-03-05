#pragma once

// 系统核心支持库命令类型定义
// 自动生成于 2026-01-04 15:09:00
// 命令总数: 672

#define __KRNLN_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 根据函数索引和名称拼接完整函数名
// 使用标准 ## 连接，兼容 IntelliSense/Clang 解析器
#define _KRNLN_CONCAT4(a,b,c,d) a##_##b##_##c##_##d
#define _KRNLN_NAME_IMPL(e, nm, idx) _KRNLN_CONCAT4(e, nm, idx, e)
#define KRNLN_NAME(_index, _name) _KRNLN_NAME_IMPL(__E_FNENAME, _name, _index)

// 生成函数名字符串
#define KRNLN_NAME_STR(_index, _name) ______E_FNENAME(__KRNLN_NAME(_index, _name))

// 命令定义宏
// 参数说明:
//  0 _index = 命令索引
//  1 _szName = 命令中文名称
//  2 _szEgName = 命令英文名称(显示用)
//  3 _szFuncId = 函数标识符(用于生成C++函数名,避免关键字)
//  4 _szExplain = 命令详细说明
//  5 _shtCategory = 命令类别索引(从1开始)
//  6 _wState = 命令状态标志
//  7 _dtRetValType = 返回值类型
//  8 _wReserved = 保留字段
//  9 _shtUserLevel = 用户级别
// 10 _shtBitmapIndex = 图标索引
// 11 _shtBitmapCount = 图标数目
// 12 _nArgCount = 参数数目
// 13 _pBeginArgInfo = 参数信息起始地址

#define KRNLN_DEF(_MAKE) \
    _MAKE(   0, "如果", "ife", ife, "本命令根据提供的逻辑参数的值，来决定是否改变程序的执行位置，如果提供的逻辑参数值为真，程序继续顺序向下执行，否则跳转到左侧箭头线指向的命令处去执行。本命令为初级", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 0)\
    _MAKE(   1, "如果真", "if", if_, "本命令根据提供的逻辑参数的值，来决定是否改变程序的执行位置，如果提供的逻辑参数值为真，程序继续顺序向下执行，否则跳转到左侧箭头线指向的命令处去执行。同“如果”命", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1)\
    _MAKE(   2, "判断", "switch", switch_, "本命令根据提供的逻辑参数的值，来决定是否改变程序的执行位置，如果提供的逻辑参数值为真，程序继续顺序向下执行，否则跳转到下一分支处去继续判断。本命令为初级命令。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 2)\
    _MAKE(   3, "判断循环首", "while", while_, "本命令根据提供的逻辑参数的值，来决定是否进入循环。如果提供的逻辑参数值为真，程序顺序执行下一条命令进入循环，否则跳转到本命令所对应的“判断循环尾”命令的下一条命", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 3)\
    _MAKE(   4, "判断循环尾", "wend", wend, "本命令已经被隐藏。 执行本命令将返回到对应的“判断循环首”命令处继续进行循环。本命令为初级命令。", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE(   5, "循环判断首", "DoWhile", DoWhile, "执行本命令后将顺序执行下一条命令进入程序循环。本命令为初级命令。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE(   6, "循环判断尾", "loop", loop, "本命令已经被隐藏。 本命令根据提供的逻辑参数的值，来决定是否返回到循环首部继续进行循环。如果提供的逻辑参数值为真，程序返回到对应的“循环判断首”命令处继续进行循", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 4)\
    _MAKE(   7, "计次循环首", "counter", counter, "执行本命令将把循环体内的命令重复执行指定的次数。提供的整数参数值决定将重复执行的次数。本命令为初级命令。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 5)\
    _MAKE(   8, "计次循环尾", "CounterLoop", CounterLoop, "本命令已经被隐藏。 如果计次循环的循环次数未到，执行本命令将返回到对应的“计次循环首”命令处继续进行循环，否则将顺序执行下一条命令退出本计次循环。本命令为初级命", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE(   9, "变量循环首", "for", for_, "执行本命令将利用变量对循环体内的命令进行循环执行。第一次执行此命令时将使用“变量起始值”参数初始化“循环变量”参数所指定的变量。每次（包括第一次）执行到此命令处", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 7)\
    _MAKE(  10, "变量循环尾", "next", next, "本命令已经被隐藏。 将在对应的“变量循环首”命令中所指定的数值变量递增或递减指定的值后返回到循环首部继续执行。本命令为初级命令。", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE(  11, "到循环尾", "continue", continue_, "本命令转移当前程序执行位置到当前所处循环体的循环尾语句处。本命令为初级命令。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE(  12, "跳出循环", "break", break_, "本命令转移当前程序执行位置到当前所处循环体循环尾语句的下一条语句处。本命令为初级命令。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE(  13, "返回", "return", return_, "本命令转移当前程序执行位置到调用本子程序语句的下一条语句处,并可根据需要返回一个值到子程序调用处。本命令为初级命令。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 11)\
    _MAKE(  14, "结束", "end", end, "本命令结束当前易程序的运行。本命令为初级命令。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE(  15, "相乘", "mul", mul, "返回两个数值的乘积，运算符号为“*”或“×”。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 2, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 12)\
    _MAKE(  16, "相除", "div", div, "返回两个数值的商，运算符号为“/”或“÷”。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 2, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 14)\
    _MAKE(  17, "整除", "IDiv", IDiv, "求出两个数值的商，并返回其整数部分，运算符号为“\\”。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 2, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 16)\
    _MAKE(  18, "求余数", "mod", mod, "求出两个数值的商，并返回余数部分，运算符号为“%”或“Mod”。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 2, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 18)\
    _MAKE(  19, "相加", "add", add, "运算符号为“+”，用途为：1、返回两个数值的和；2、将两个文本首尾连接起来，返回连接后的文本；3、将两个字节集首尾连接起来，返回连接后的字节集。本命令为初级命令", 2, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 20)\
    _MAKE(  20, "相减", "sub", sub, "返回两个数值的差，运算符号为“-”。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 2, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 22)\
    _MAKE(  21, "负", "neg", neg, "反转一个数值的正负符号并返回反转后的值，运算符号为“-”（放在表达式首）。本命令为初级命令。", 2, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 24)\
    _MAKE(  22, "取符号", "sgn", sgn, "返回一个整数，如果小于零，表明给定数值为负；如果等于零，表明给定数值为零；如果大于零，表明给定数值为正。本命令为初级命令。", 2, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 25)\
    _MAKE(  23, "取绝对值", "abs", abs, "如果所提供数值为字节型，则将直接返回该数值。本命令为初级命令。", 2, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 26)\
    _MAKE(  24, "取整", "int", int_, "返回一个小数的整数部分。本命令与“绝对取整”命令不相同之处为： 如果给定小数为负数，则本命令返回小于或等于该小数的第一个负整数，而“绝对取整”命令则会返回大于或", 2, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 27)\
    _MAKE(  25, "绝对取整", "fix", fix, "返回一个小数的整数部分。本命令与“取整”命令不相同之处为： 如果给定小数为负数，则本命令返回大于或等于该小数的第一个负整数，而“取整”命令则会返回小于或等于该小", 2, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 28)\
    _MAKE(  26, "四舍五入", "round", round, "返回按照指定的方式进行四舍五入运算的结果数值。本命令为初级命令。", 2, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 29)\
    _MAKE(  27, "求次方", "pow", pow, "返回指定数值的指定次方。本命令为初级命令。", 2, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 31)\
    _MAKE(  28, "求平方根", "sqr", sqr, "返回指定参数的平方根。本命令为初级命令。", 2, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 33)\
    _MAKE(  29, "求正弦", "sin", sin, "返回指定角的正弦值。本命令为初级命令。", 2, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 34)\
    _MAKE(  30, "求余弦", "cos", cos, "返回指定角的余弦值。本命令为初级命令。", 2, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 35)\
    _MAKE(  31, "求正切", "tan", tan, "返回指定角的正切值。本命令为初级命令。", 2, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 36)\
    _MAKE(  32, "求反正切", "atn", atn, "返回指定数的反正切值。本命令为初级命令。", 2, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 37)\
    _MAKE(  33, "求自然对数", "log", log, "返回指定参数的自然对数值。自然对数是以 e 为底的对数。常量 #e 的值大约是 2.718282。本命令为初级命令。", 2, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 38)\
    _MAKE(  34, "求反对数", "exp", exp, "返回 e（自然对数的底）的某次方。本命令为初级命令。", 2, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 39)\
    _MAKE(  35, "是否运算正确", "IsCalcOK", IsCalcOK, "对乘、除、“求次方”、“求平方根”、“求正弦值”、“求余弦值”、“求正切值”、“求反正切值”、“求自然对数”、“求反对数”等等数学运算命令所计算出来的双精度结果", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 40)\
    _MAKE(  36, "置随机数种子", "randomize", randomize, "为随机数生成器初始化一个种子值，不同的种子值将导致“取随机数”命令返回不同的随机数系列。本命令为初级命令。", 2, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 41)\
    _MAKE(  37, "取随机数", "rnd", rnd, "返回一个指定范围内的随机数值。在使用本命令取一系列的随机数之前，应该先使用“置随机数种子”命令为随机数生成器初始化一个种子值。本命令为初级命令。", 2, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 42)\
    _MAKE(  38, "等于", "cmd_38", cmd_38, "被比较值与比较值相同时返回真，否则返回假，运算符号为“=”或“==”。本命令为初级命令。", 3, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 44)\
    _MAKE(  39, "不等于", "cmd_39", cmd_39, "被比较值与比较值不相同时返回真，否则返回假，运算符号为“<>”或“!=”或“≠”。本命令为初级命令。", 3, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 46)\
    _MAKE(  40, "小于", "cmd_40", cmd_40, "被比较值小于比较值时返回真，否则返回假，运算符号为“<”。本命令为初级命令。", 3, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 48)\
    _MAKE(  41, "大于", "cmd_41", cmd_41, "被比较值大于比较值时返回真，否则返回假，运算符号为“>”。本命令为初级命令。", 3, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 50)\
    _MAKE(  42, "小于或等于", "cmd_42", cmd_42, "被比较值小于或等于比较值时返回真，否则返回假，运算符号为“<=”或“≤”。本命令为初级命令。", 3, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 52)\
    _MAKE(  43, "大于或等于", "cmd_43", cmd_43, "被比较值大于或等于比较值时返回真，否则返回假，运算符号为“>=”或“≥”。本命令为初级命令。", 3, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 54)\
    _MAKE(  44, "近似等于", "like", like, "当比较文本在被比较文本的首部被包容时返回真，否则返回假，运算符号为“?=”或“≈”。本命令为初级命令。", 3, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 56)\
    _MAKE(  45, "并且", "and", and, "如果所提供的两个参数逻辑值都为真则返回真，否则返回假，运算符号为“&&”或“And”或“且”。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 3, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 58)\
    _MAKE(  46, "或者", "or", or, "如果所提供的两个参数逻辑值中任意有一个为真则返回真，否则返回假，运算符号为“||”或“Or”或“或”。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 3, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 60)\
    _MAKE(  47, "取反", "not", not, "如果参数值为真则返回假，如果参数值为假则返回真。本命令为初级命令。", 3, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 62)\
    _MAKE(  48, "位取反", "bnot", bnot, "将指定数值每一个比特位的值取反后返回。本命令为中级命令。", 4, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 63)\
    _MAKE(  49, "位与", "band", band, "如两个数值有某共同比特位为1，则返回值的对应位也为1，否则为0。返回计算后的结果。本命令为中级命令。命令参数表中最后一个参数可以被重复添加。", 4, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 64)\
    _MAKE(  50, "位或", "bor", bor, "如两个数值中有一个数值的某一比特位不为零，则返回值的对应位就为1，否则为0。返回计算后的结果。本命令为中级命令。命令参数表中最后一个参数可以被重复添加。", 4, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 66)\
    _MAKE(  51, "位异或", "bxor", bxor, "如两个数值对应比特位的值不相等，则返回值的对应位就为1，否则为0。返回计算后的结果。本命令为中级命令。命令参数表中最后一个参数可以被重复添加。", 4, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 68)\
    _MAKE(  52, "左移", "shl", shl, "将某整数的数据位左移指定位数，返回移动后的结果。本命令为中级命令。", 4, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 70)\
    _MAKE(  53, "右移", "shr", shr, "将某整数的数据位右移指定位数，返回移动后的结果。本命令为中级命令。", 4, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 72)\
    _MAKE(  54, "合并整数", "MakeLong", MakeLong, "将第一个整数的低16位放置到结果整数的低16位，将第二个整数的低16位放置到结果整数的高16位，以此合并成一个整数，并返回合并后的结果。本命令为中级命令。", 4, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 74)\
    _MAKE(  55, "合并短整数", "MakeWord", MakeWord, "将第一个整数的低8位放置到结果短整数的低8位，将第二个整数的低8位放置到结果短整数的高8位，以此合并成一个短整数，并返回合并后的结果。本命令为中级命令。", 4, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 76)\
    _MAKE(  56, "赋值", "set", set, "将指定的常数、常数集、常量、资源、对象或者变量赋于到指定的变量或变量数组中去。运算符号为“=”。本命令为初级命令。", 5, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 78)\
    _MAKE(  57, "连续赋值", "store", store, "将指定的常数、常数集、常量、资源、对象或者变量赋于到一系列变量或变量数组中去。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 5, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 80)\
    _MAKE(  58, "重定义数组", "ReDim", ReDim, "本命令可以重新定义指定数组的维数及各维的上限值。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 6, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 82)\
    _MAKE(  59, "取数组成员数", "GetAryElementCount", GetAryElementCount, "取指定数组变量的全部成员数目，如果该变量不为数组，返回-1，因此本命令也可以用作检查指定变量是否为数组变量。本命令为初级命令。", 6, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 85)\
    _MAKE(  60, "取数组下标", "UBound", UBound, "返回指定数组维可用的最大下标（最小下标固定为1）。如果给定变量不为数组变量或指定维不存在，返回 0 。本命令为初级命令。", 6, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 86)\
    _MAKE(  61, "复制数组", "CopyAry", CopyAry, "将数组数据复制到指定的数组变量，该数组变量内的所有数据和数组维定义信息将被全部覆盖。本命令为初级命令。", 6, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 88)\
    _MAKE(  62, "加入成员", "AddElement", AddElement, "将数据加入到指定数组变量的尾部，并通过重新定义数组维数自动增加其成员数目。数组变量如为多维数组，加入完毕后将被转换为单维数组。本命令为初级命令。", 6, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 90)\
    _MAKE(  63, "插入成员", "InsElement", InsElement, "将数据插入到指定数组变量的指定位置，并通过重新定义数组维数自动增加其成员数目。数组变量如为多维数组，插入完毕后将被转换为单维数组。本命令为初级命令。", 6, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 92)\
    _MAKE(  64, "删除成员", "RemoveElement", RemoveElement, "删除指定数组变量中的成员，并通过重新定义数组维数自动减少其成员数目。数组变量如为多维数组，删除完毕后将被转换为单维数组。返回所实际删除的成员数目。本命令为初级命", 6, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 95)\
    _MAKE(  65, "清除数组", "RemoveAll", RemoveAll, "删除指定数组变量中的所有成员，释放这些成员所占用的存储空间，重新定义该变量为单维 0 成员数组变量。本命令为初级命令。", 6, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 98)\
    _MAKE(  66, "数组排序", "SortAry", SortAry, "对指定数值数组变量内的所有数组成员进行快速排序，不影响数组的维定义信息，排序结果存放回该数组变量。本命令为初级命令。", 6, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 99)\
    _MAKE(  67, "数组清零", "ZeroAry", ZeroAry, "将指定数值数组变量内的所有成员值全部设置为零，不影响数组的维定义信息。本命令为初级命令。", 6, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 101)\
    _MAKE(  68, "取命令行", "GetCmdLine", GetCmdLine, "本命令可以取出在启动易程序时附加在其可执行文件名后面的所有以空格分隔的命令行文本段。本命令为初级命令。", 7, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 102)\
    _MAKE(  69, "取运行目录", "GetRunPath", GetRunPath, "取当前被执行的易程序文件所处的目录。本命令为初级命令。", 7, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE(  70, "取执行文件名", "GetRunFileName", GetRunFileName, "取当前被执行的易程序文件的名称。本命令为初级命令。", 7, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE(  71, "读环境变量", "GetEnv", GetEnv, "返回文本，它关连于一个操作系统环境变量。成功时返回所取得的值，失败则返回空文本。本命令为初级命令。", 7, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 103)\
    _MAKE(  72, "写环境变量", "PutEnv", PutEnv, "修改或建立指定的操作系统环境变量。成功返回真，失败返回假。本命令为初级命令。", 7, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 104)\
    _MAKE(  73, "取所有发音", "GetAllPY", GetAllPY, "返回包含指定汉字所有拼音编码的文本数组（支持多音字），目前仅支持国标汉字。返回的数组结果可以用“复制数组”命令保存到同类型数组变量。本命令为初级命令。", 8, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 106)\
    _MAKE(  74, "取发音数目", "GetPYCount", GetPYCount, "返回指定汉字的发音数目。本命令为初级命令。", 8, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 107)\
    _MAKE(  75, "取拼音", "GetPY", GetPY, "返回包含指定汉字的指定拼音编码的文本。如果该指定拼音编码不存在，将返回空文本。目前仅支持国标汉字。多音字的第一个发音为常用音。本命令为初级命令。", 8, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 108)\
    _MAKE(  76, "取声母", "GetSM", GetSM, "返回包含指定汉字指定拼音编码的声母部分文本。如果指定拼音编码不存在或该汉字此发音无声母，将返回空文本。目前仅支持国标汉字。本命令为初级命令。", 8, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 110)\
    _MAKE(  77, "取韵母", "GetYM", GetYM, "返回包含指定汉字指定拼音编码的韵母部分文本。如果指定拼音编码不存在，将返回空文本。目前仅支持国标汉字。本命令为初级命令。", 8, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 112)\
    _MAKE(  78, "发音比较", "CompPY", CompPY, "比较两段文本的发音，如果发音相同返回真，否则返回假。对于文本中的非国标汉字部分，将仅进行简单的值比较。本命令为初级命令。", 8, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 114)\
    _MAKE(  79, "输入字比较", "CompPYCode", CompPYCode, "将一个拼音输入字文本与另外一个普通文本比较，如果两者相符，返回真，否则返回假。本命令为初级命令。 一、首拼及全拼类： 如：“jsj”匹配“计算机”，同时“jis", 8, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 118)\
    _MAKE(  80, "取文本长度", "len", len, "取文本型数据的长度，不包含结束0。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 122)\
    _MAKE(  81, "取文本左边", "left", left, "返回一个文本，其中包含指定文本中从左边算起指定数量的字符。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 123)\
    _MAKE(  82, "取文本右边", "right", right, "返回一个文本，其中包含指定文本中从右边算起指定数量的字符。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 125)\
    _MAKE(  83, "取文本中间", "mid", mid, "返回一个文本，其中包含指定文本中从指定位置算起指定数量的字符。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 127)\
    _MAKE(  84, "字符", "chr", chr, "返回一个文本，其中包含有与指定字符代码相关的字符。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 130)\
    _MAKE(  85, "取代码", "asc", asc, "返回文本中指定位置处字符的代码。如果指定位置超出文本长度，返回0。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 131)\
    _MAKE(  86, "寻找文本", "InStr", InStr, "返回一个整数值，指定一文本在另一文本中最先出现的位置，位置值从 1 开始。如果未找到，返回-1。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 133)\
    _MAKE(  87, "倒找文本", "InStrRev", InStrRev, "返回一个整数值，指定一文本在另一文本中最后出现的位置，位置值从 1 开始。如果未找到，返回-1。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 137)\
    _MAKE(  88, "到大写", "UCase", UCase, "将文本中的小写英文字母变换为大写，返回变换后的结果文本。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 141)\
    _MAKE(  89, "到小写", "LCase", LCase, "将文本中的大写英文字母变换为小写，返回变换后的结果文本。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 142)\
    _MAKE(  90, "到全角", "QJCase", QJCase, "将文本中的半角字母、空格或数字变换为全角，返回变换后的结果文本。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 143)\
    _MAKE(  91, "到半角", "BJCase", BJCase, "将文本中的全角字母、空格或数字变换为半角，返回变换后的结果文本。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 144)\
    _MAKE(  92, "到文本", "str", str, "返回一个文本，代表指定数值、逻辑值或日期时间被转换后的结果。如果为文本数据，将被直接返回。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 145)\
    _MAKE(  93, "删首空", "LTrim", LTrim, "返回一个文本，其中包含被删除了首部全角或半角空格的指定文本。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 146)\
    _MAKE(  94, "删尾空", "RTrim", RTrim, "返回一个文本，其中包含被删除了尾部全角或半角空格的指定文本。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 147)\
    _MAKE(  95, "删首尾空", "trim", trim, "返回一个文本，其中包含被删除了首部及尾部全角或半角空格的指定文本。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 148)\
    _MAKE(  96, "删全部空", "TrimAll", TrimAll, "返回一个文本，其中包含被删除了所有全角或半角空格的指定文本。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 149)\
    _MAKE(  97, "文本替换", "ReplaceText", ReplaceText, "将指定文本的某一部分用其它的文本替换。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 150)\
    _MAKE(  98, "子文本替换", "RpSubText", RpSubText, "返回一个文本，该文本中指定的子文本已被替换成另一子文本，并且替换发生的次数也是被指定的。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_krnln_global_var + 154)\
    _MAKE(  99, "取空白文本", "space", space, "返回具有指定数目半角空格的文本。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 160)\
    _MAKE( 100, "取重复文本", "string", string, "返回一个文本，其中包含指定次数的文本重复结果。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 161)\
    _MAKE( 101, "文本比较", "StrComp", StrComp, "如果返回值小于0，表示文本一小于文本二；如果等于0，表示文本一等于文本二；如果大于0，表示文本一大于文本二。如果比较时区分大小写，也可以使用比较运算符进行同样的", 9, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 163)\
    _MAKE( 102, "分割文本", "split", split, "将指定文本进行分割，返回分割后的一维文本数组。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 166)\
    _MAKE( 103, "指针到文本", "pstr", pstr, "返回指定内存指针所指向地址处的文本，注意调用本命令前一定要确保所提供的内存指针真实有效，且指向一个以零字符结束的文本串。本命令的最佳使用场合就是在易语言回调子程", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 169)\
    _MAKE( 104, "文本到UTF8", "StrToUTF8", StrToUTF8, "将所指定文本转换到UTF8格式后返回,注意所返回UTF8文本数据包括结束零字符.本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 170)\
    _MAKE( 105, "UTF8到文本", "UTF8ToStr", UTF8ToStr, "将所指定UTF8文本数据转换到通常文本后返回。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 171)\
    _MAKE( 106, "文本到UTF16", "StrToUTF16", StrToUTF16, "将所指定文本转换到UTF16格式后返回,注意所返回UTF16文本数据包括结束零字符.本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 172)\
    _MAKE( 107, "UTF16到文本", "UTF16ToStr", UTF16ToStr, "将所指定UTF16文本数据转换到通常文本后返回。本命令为初级命令。", 9, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 173)\
    _MAKE( 108, "到时间", "ToTime", ToTime, "将指定文本转换为时间并返回。如果给定文本不符合书写格式要求或者时间值错误导致不能进行转换，将返回100年1月1日。如果给定参数本身就是时间数据，将直接返回该时间", 10, _CMD_OS(__OS_WIN), SDT_DATE_TIME, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 174)\
    _MAKE( 109, "增减时间", "TimeChg", TimeChg, "返回一个时间，这一时间被加上或减去了一段间隔。如果改变后的时间无效，将自动靠近最近的有效时间。本命令为初级命令。", 10, _CMD_OS(__OS_WIN), SDT_DATE_TIME, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 175)\
    _MAKE( 110, "取时间间隔", "TimeDiff", TimeDiff, "返回一个数值，表示“时间1”减去“时间2”之后的间隔数目。注意：每个星期以星期天为第一天。本命令为初级命令。", 10, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 178)\
    _MAKE( 111, "取某月天数", "GetDaysOfSpecMonth", GetDaysOfSpecMonth, "返回指定月份所包含的天数。如果给定的月份无效，返回0。本命令为初级命令。", 10, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 181)\
    _MAKE( 112, "时间到文本", "TimeToText", TimeToText, "将指定时间转换为文本并返回。本命令为初级命令。", 10, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 183)\
    _MAKE( 113, "取时间部分", "TimePart", TimePart, "返回一个包含已知时间指定部分的整数。本命令为初级命令。", 10, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 185)\
    _MAKE( 114, "取年份", "year", year, "返回一个值为 100 到 9999 之间的整数，表示指定时间中的年份。本命令为初级命令。", 10, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 187)\
    _MAKE( 115, "取月份", "month", month, "返回一个值为 1 到 12 之间的整数，表示指定时间中的月份。本命令为初级命令。", 10, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 188)\
    _MAKE( 116, "取日", "day", day, "返回一个值为 1 到 31 之间的整数，表示一个月中的某一日。本命令为初级命令。", 10, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 189)\
    _MAKE( 117, "取星期几", "WeekDay", WeekDay, "返回一个值为 1 到 7 之间的整数，表示一个星期中的某一日。星期日为 1，星期一为 2，依此类推。本命令为初级命令。", 10, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 190)\
    _MAKE( 118, "取小时", "hour", hour, "返回一个值为 0 到 23 之间的整数，表示一天中的某一小时。本命令为初级命令。", 10, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 191)\
    _MAKE( 119, "取分钟", "minute", minute, "返回一个值为 0 到 59 之间的整数，表示一小时中的某一分钟。本命令为初级命令。", 10, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 192)\
    _MAKE( 120, "取秒", "second", second, "返回一个值为 0 到 59 之间的整数，表示一分钟中的某一秒。本命令为初级命令。", 10, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 193)\
    _MAKE( 121, "指定时间", "GetSpecTime", GetSpecTime, "返回包含指定年、月、日、小时、分、秒的时间。如果指定了无效时间，将自动使用最相近的有效时间代替。本命令为初级命令。", 10, _CMD_OS(__OS_WIN), SDT_DATE_TIME, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_krnln_global_var + 194)\
    _MAKE( 122, "取现行时间", "now", now, "返回当前系统日期及时间。本命令为初级命令。", 10, _CMD_OS(__OS_WIN), SDT_DATE_TIME, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 123, "置现行时间", "SetSysTime", SetSysTime, "设置当前系统日期及时间。成功返回真，失败返回假。本命令为初级命令。", 10, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 200)\
    _MAKE( 124, "取日期", "GetDatePart", GetDatePart, "返回一个日期时间型数据的日期部分，其小时、分钟、秒被固定设置为0时0分0秒。本命令为初级命令。", 10, _CMD_OS(__OS_WIN), SDT_DATE_TIME, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 201)\
    _MAKE( 125, "取时间", "GetTimePart", GetTimePart, "返回一个日期时间型数据的时间部分，其年、月、日被固定设置为2000年1月1日。本命令为初级命令。", 10, _CMD_OS(__OS_WIN), SDT_DATE_TIME, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 202)\
    _MAKE( 126, "到数值", "val", val, "返回包含于文本内的数值，文本中是一个适当类型的数值，支持全角书写方式。本命令也可用作将其他类型的数据转换为双精度小数。本命令为初级命令。", 11, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 203)\
    _MAKE( 127, "数值到大写", "UNum", UNum, "将数值转换为简体或繁体的大写形式，返回转换后的文本。本命令为初级命令。", 11, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 204)\
    _MAKE( 128, "数值到金额", "NumToRMB", NumToRMB, "将数值转换为金额的简体或繁体大写形式，返回转换后的文本。本命令为初级命令。", 11, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 206)\
    _MAKE( 129, "数值到格式文本", "NumToText", NumToText, "返回一个文本，代表指定数值被格式转换后的结果。本命令为初级命令。", 11, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 208)\
    _MAKE( 130, "取十六进制文本", "GetHexText", GetHexText, "返回一个文本，代表指定数值的十六进制形式。本命令为初级命令。", 11, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 211)\
    _MAKE( 131, "取八进制文本", "GetOctText", GetOctText, "返回一个文本，代表指定数值的八进制形式。本命令为初级命令。", 11, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 212)\
    _MAKE( 132, "到字节", "ToByte", ToByte, "返回包含于文本内的字节值，文本中是一个适当类型的数值，支持全角书写方式。本命令也可用作将其他类型的数据转换为字节。本命令为初级命令。", 11, _CMD_OS(__OS_WIN), SDT_BYTE, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 213)\
    _MAKE( 133, "到短整数", "ToShort", ToShort, "返回包含于文本内的短整数值，文本中是一个适当类型的数值，支持全角书写方式。本命令也可用作将其他类型的数据转换为短整数。本命令为初级命令。", 11, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 214)\
    _MAKE( 134, "到整数", "ToInt", ToInt, "返回包含于文本内的整数值，文本中是一个适当类型的数值，支持全角书写方式。本命令也可用作将其他类型的数据转换为整数。本命令为初级命令。", 11, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 215)\
    _MAKE( 135, "到长整数", "ToLong", ToLong, "返回包含于文本内的长整数值，文本中是一个适当类型的数值，支持全角书写方式。本命令也可用作将其他类型的数据转换为长整数。本命令为初级命令。", 11, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 216)\
    _MAKE( 136, "到小数", "ToFloat", ToFloat, "返回包含于文本内的小数值，文本中是一个适当类型的数值，支持全角书写方式。本命令也可用作将其他类型的数据转换为小数。本命令为初级命令。", 11, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 217)\
    _MAKE( 137, "十六进制", "hex", hex, "计算返回所指定十六进制文本常量对应的整数值. 本命令在编译时被直接预处理为整数型参量值,不影响程序执行效率.本命令为初级命令。", 11, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 218)\
    _MAKE( 138, "二进制", "binary", binary, "计算返回所指定二进制文本常量对应的整数值. 本命令在编译时被直接预处理为整数型参量值,不影响程序执行效率.本命令为初级命令。", 11, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 219)\
    _MAKE( 139, "反转整数字节序", "ReverseIntBytes", ReverseIntBytes, "将所指定整数的字节序反转,返回反转后的结果值. 譬如十六进制整数0x12345678,反转后将返回0x78563412. 本命令在与类似Java这样的语言进行数", 11, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 220)\
    _MAKE( 140, "取字节集长度", "BinLen", BinLen, "取字节集型数据的长度。本命令为初级命令。", 12, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 221)\
    _MAKE( 141, "到字节集", "ToBin", ToBin, "将指定数据转换为字节集后返回转换结果。本命令为初级命令。", 12, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 222)\
    _MAKE( 142, "取字节集数据", "GetBinElement", GetBinElement, "取出字节集中指定位置指定数据类型的数据。本命令为初级命令。", 12, _CMD_OS(__OS_WIN), _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 223)\
    _MAKE( 143, "取字节集左边", "BinLeft", BinLeft, "返回一个字节集，其中包含指定字节集中从左边算起指定数量的字节。本命令为初级命令。", 12, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 226)\
    _MAKE( 144, "取字节集右边", "BinRight", BinRight, "返回一个字节集，其中包含指定字节集中从右边算起指定数量的字节。本命令为初级命令。", 12, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 228)\
    _MAKE( 145, "取字节集中间", "BinMid", BinMid, "返回一个字节集，其中包含指定字节集中从指定位置算起指定数量的字节。本命令为初级命令。", 12, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 230)\
    _MAKE( 146, "寻找字节集", "InBin", InBin, "返回一字节集在另一字节集中最先出现的位置，位置值从 1 开始。如果未找到，返回 -1。本命令为初级命令。", 12, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 233)\
    _MAKE( 147, "倒找字节集", "InBinRev", InBinRev, "返回一字节集在另一字节集中最后出现的位置，位置值从 1 开始。如果未找到，返回 -1。本命令为初级命令。", 12, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 236)\
    _MAKE( 148, "字节集替换", "RpBin", RpBin, "将指定字节集的某一部分用其它的字节集替换，然后返回替换后的结果。本命令为初级命令。", 12, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 239)\
    _MAKE( 149, "子字节集替换", "RpSubBin", RpSubBin, "返回一个字节集，该字节集中指定的子字节集已被替换成另一子字节集，并且替换发生的次数也是被指定的。本命令为初级命令。", 12, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 243)\
    _MAKE( 150, "取空白字节集", "SpaceBin", SpaceBin, "返回具有特定数目 0 字节的字节集。本命令为初级命令。", 12, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 248)\
    _MAKE( 151, "取重复字节集", "bin", bin, "返回一个字节集，其中包含指定次数的字节集重复结果。本命令为初级命令。", 12, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 249)\
    _MAKE( 152, "分割字节集", "SplitBin", SplitBin, "将指定字节集进行分割，返回分割后的一维字节集数组。本命令为初级命令。", 12, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 251)\
    _MAKE( 153, "指针到字节集", "pbin", pbin, "返回指定内存指针所指向地址处的一段数据，注意调用本命令前一定要确保所提供的内存地址段真实有效。本命令的最佳使用场合就是在易语言回调子程序和易语言DLL公开子程序", 12, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 254)\
    _MAKE( 154, "指针到整数", "p2int", p2int, "返回指定内存指针所指向地址处的一个整数(INT)，注意调用本命令前一定要确保所提供的内存地址段真实有效。本命令为高级命令。", 12, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 256)\
    _MAKE( 155, "指针到小数", "p2float", p2float, "返回指定内存指针所指向地址处的一个小数(FLOAT)，注意调用本命令前一定要确保所提供的内存地址段真实有效。本命令为高级命令。", 12, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 257)\
    _MAKE( 156, "指针到双精度小数", "p2double", p2double, "返回指定内存指针所指向地址处的一个双精度小数(DOUBLE)，注意调用本命令前一定要确保所提供的内存地址段真实有效。本命令为高级命令。", 12, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 258)\
    _MAKE( 157, "取字节集内整数", "GetIntInsideBin", GetIntInsideBin, "返回字节集中所指定位置处的整数。本命令为初级命令。", 12, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 259)\
    _MAKE( 158, "置字节集内整数", "SetIntInsideBin", SetIntInsideBin, "设置字节集中所指定位置处的整数值。本命令为初级命令。", 12, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 262)\
    _MAKE( 159, "取磁盘总空间", "GetDiskTotalSpace", GetDiskTotalSpace, "返回以 1024 字节（KB）为单位的指定磁盘全部空间。如果失败，返回-1。本命令为初级命令。", 13, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 266)\
    _MAKE( 160, "取磁盘剩余空间", "GetDiskFreeSpace", GetDiskFreeSpace, "返回以 1024 字节（KB）为单位的指定磁盘现行剩余空间。如果失败，返回-1。本命令为初级命令。", 13, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 267)\
    _MAKE( 161, "取磁盘卷标", "GetDiskLabel", GetDiskLabel, "返回指定磁盘的卷标文本。本命令为初级命令。", 13, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 268)\
    _MAKE( 162, "置磁盘卷标", "SetDiskLabel", SetDiskLabel, "设置指定磁盘的卷标文本。成功返回真，失败返回假。本命令为初级命令。", 13, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 269)\
    _MAKE( 163, "改变驱动器", "ChDrive", ChDrive, "改变当前的缺省驱动器。成功返回真，失败返回假。本命令为初级命令。", 13, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 271)\
    _MAKE( 164, "改变目录", "ChDir", ChDir, "改变当前的目录，但不会改变缺省驱动器位置。成功返回真，失败返回假。本命令为初级命令。", 13, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 272)\
    _MAKE( 165, "取当前目录", "CurDir", CurDir, "返回一个文本，用来代表当前的目录。如果失败，返回空文本。本命令为初级命令。", 13, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 166, "创建目录", "MkDir", MkDir, "创建一个新的目录。成功返回真，失败返回假。本命令为初级命令。", 13, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 273)\
    _MAKE( 167, "删除目录", "RmDir", RmDir, "删除一个存在的目录及其中的所有子目录和下属文件，请务必谨慎使用本命令。成功返回真，失败返回假。本命令为初级命令。", 13, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 274)\
    _MAKE( 168, "复制文件", "FileCopy", FileCopy, "成功返回真，失败返回假。本命令为初级命令。", 13, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 275)\
    _MAKE( 169, "移动文件", "FileMove", FileMove, "将文件从一个位置移动到另外一个位置。成功返回真，失败返回假。本命令为初级命令。", 13, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 277)\
    _MAKE( 170, "删除文件", "kill", kill, "成功返回真，失败返回假。本命令为初级命令。", 13, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 279)\
    _MAKE( 171, "文件更名", "name", name, "重新命名一个文件或目录。成功返回真，失败返回假。本命令为初级命令。", 13, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 280)\
    _MAKE( 172, "文件是否存在", "IsFileExist", IsFileExist, "判断指定的磁盘文件是否真实存在。如存在返回真，否则返回假。本命令为初级命令。", 13, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 282)\
    _MAKE( 173, "寻找文件", "dir", dir, "返回一个文本，用以表示所找到的文件名或目录名，它必须与所要求的名称格式或文件属性相匹配。支持使用多字符 (*) 和单字符 (?) 通配符来指定多重文件。在某子程", 13, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 283)\
    _MAKE( 174, "取文件时间", "FileDateTime", FileDateTime, "返回指定文件被创建或最后修改后的日期和时间。如果该文件不存在，将返回100年1月1日。本命令为初级命令。", 13, _CMD_OS(__OS_WIN), SDT_DATE_TIME, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 285)\
    _MAKE( 175, "取文件尺寸", "FileLen", FileLen, "返回一个文件的长度，单位是字节。如果该文件不存在，将返回 -1。本命令为初级命令。", 13, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 286)\
    _MAKE( 176, "取文件属性", "GetAttr", GetAttr, "返回一个文件或目录的属性。此属性值由以下常量或其和构成： 1、#只读文件； 2、#隐藏文件；  4、#系统文件； 16、#子目录；32、#存档文件 。要判断是否", 13, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 287)\
    _MAKE( 177, "置文件属性", "SetAttr", SetAttr, "为一个文件设置属性信息。成功返回真，失败返回假。本命令为初级命令。", 13, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 288)\
    _MAKE( 178, "取临时文件名", "GetTempFileName", GetTempFileName, "返回一个在指定目录中确定不存在的 .TMP 全路径文件名称。本命令为初级命令。", 13, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 290)\
    _MAKE( 179, "读入文件", "ReadFile", ReadFile, "返回一个字节集，其中包含指定文件的所有数据。本命令为初级命令。", 13, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 291)\
    _MAKE( 180, "写到文件", "WriteFile", WriteFile, "本命令用作将一个或数个字节集顺序写到指定文件中，文件原有内容被覆盖。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 13, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 292)\
    _MAKE( 181, "打开文件", "open", open, "打开一个普通文件，以对文件进行输入或输出。成功返回被打开文件的文件号，失败返回 0。本命令为初级命令。 1、#读入：从指定文件读入数据，如果该文件不存在则失败；", 14, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 294)\
    _MAKE( 182, "打开内存文件", "OpenMemFile", OpenMemFile, "建立一个内存文件，以计算机内存为存储介质对数据进行文件式的快速输入或输出。成功返回被打开文件的文件号，失败返回 0。本命令为初级命令。", 14, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 183, "关闭文件", "close", close, "关闭被打开的各种类型文件。本命令为初级命令。", 14, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 297)\
    _MAKE( 184, "关闭所有文件", "reset", reset, "关闭所有现行被打开的文件。本命令为初级命令。", 14, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 185, "锁住文件", "lock", lock, "在有若干个进程访问同一个文件的环境中，使用本命令拒绝其它用户对被打开文件中的某个区域进行读写访问。成功返回真，失败返回假。本命令为初级命令。", 14, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 298)\
    _MAKE( 186, "解锁文件", "Unlock", Unlock, "解除由“锁住文件”命令对文件所进行的锁定。成功返回真，失败返回假。注意：本命令调用时所提供的参数值必须与调用“锁住文件”命令时所提供的参数值完全一致。本命令为初", 14, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 302)\
    _MAKE( 187, "移动读写位置", "FSeek", FSeek, "在被打开的文件中，设置下一个读或写操作的位置。成功返回真，失败返回假。本命令为初级命令。", 14, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 305)\
    _MAKE( 188, "移到文件首", "SeekToBegin", SeekToBegin, "在被打开的文件中，设置下一个读或写操作的位置到文件首。成功返回真，失败返回假。本命令为初级命令。", 14, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 308)\
    _MAKE( 189, "移到文件尾", "SeekToEnd", SeekToEnd, "在被打开的文件中，设置下一个读或写操作的位置到文件尾。成功返回真，失败返回假。本命令为初级命令。", 14, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 309)\
    _MAKE( 190, "读入字节集", "ReadBin", ReadBin, "本命令用作从文件中当前读写位置读取并返回一段字节集数据，实际读入长度（即返回的字节集的尺寸）可能会小于欲读入长度。如果读入失败，将返回一个空字节集并且自动将当前", 14, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 310)\
    _MAKE( 191, "写出字节集", "WriteBin", WriteBin, "本命令用作写出一段或数段字节集数据到文件中当前读写位置处。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 14, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 312)\
    _MAKE( 192, "读入文本", "ReadText", ReadText, "本命令用作从文件中当前读写位置读取并返回一段文本数据，实际读入长度（即返回文本的尺寸）可能会小于欲读入长度。如果该数据中存在字节 0 或 26（文本结束标志），", 14, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 314)\
    _MAKE( 193, "写出文本", "WriteText", WriteText, "本命令用作写出一段或数段文本数据到文件中当前读写位置处。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 14, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 316)\
    _MAKE( 194, "读入一行", "ReadLine", ReadLine, "本命令用作从文件中当前读写位置读取并返回一行文本数据，行末的回车及换行符将被抛弃。如果读入失败，将返回一个空文本并且自动将当前文件读写位置移到文件尾部。本命令为", 14, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 318)\
    _MAKE( 195, "写文本行", "WriteLine", WriteLine, "本命令用作写出一行或多行文本数据到文件中当前读写位置处，每行的尾部将被自动加上回车及换行符。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可", 14, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 319)\
    _MAKE( 196, "读入数据", "read", read, "本命令应该与“写出数据”命令配合使用，用作从文件中当前读写位置读取格式数据到指定的一系列变量或数组变量中。成功返回真，失败返回假。本命令为中级命令。命令参数表中", 14, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 321)\
    _MAKE( 197, "写出数据", "write", write, "本命令应该与“读入数据”命令配合使用，用作写出一系列变量或数组变量的格式数据到文件中当前读写位置处。成功返回真，失败返回假。 具体各种数据类型数据的写出格式为：", 14, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 323)\
    _MAKE( 198, "是否在文件尾", "feof", feof, "如果指定被打开文件的读写位置已经处于该文件数据的尾部，则返回真，否则返回假。本命令为初级命令。", 14, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 325)\
    _MAKE( 199, "取读写位置", "loc", loc, "返回指定被打开文件的当前读/写位置，位置值从 0 开始。如果给定文件号无效或失败，返回 -1 。本命令为初级命令。", 14, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 327)\
    _MAKE( 200, "取文件长度", "lof", lof, "返回指定被打开文件的尺寸，该尺寸以字节为单位。如果给定文件号无效或失败，返回 -1 。本命令为初级命令。", 14, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 328)\
    _MAKE( 201, "插入字节集", "InsBin", InsBin, "本命令用作插入一段或数段字节集数据到文件中当前读写位置处。该文件被打开时必须给予\"#读写\"或\"#改读\"权限。成功返回真，失败返回假。本命令为初级命令。命令参数表", 14, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 329)\
    _MAKE( 202, "插入文本", "InsText", InsText, "本命令用作插入一段或数段文本数据到文件中当前读写位置处。该文件被打开时必须给予\"#读写\"或\"#改读\"权限。成功返回真，失败返回假。本命令为初级命令。命令参数表中", 14, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 331)\
    _MAKE( 203, "插入文本行", "InsLine", InsLine, "本命令用作插入一段或数段文本行到文件中当前读写位置处。该文件被打开时必须给予\"#读写\"或\"#改读\"权限。成功返回真，失败返回假。本命令为初级命令。命令参数表中最", 14, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 333)\
    _MAKE( 204, "删除数据", "RemoveData", RemoveData, "本命令用作在文件中当前读写位置处删除一段字节数据，文件后面的数据顺序前移。该文件被打开时必须给予\"#读写\"或\"#改读\"权限。成功返回真，失败返回假。本命令为初级", 14, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 335)\
    _MAKE( 205, "打开加密文件", "CryptOpen", CryptOpen, "打开一个以指定密码加密的文件，以对此文件进行快速安全访问，支持大尺寸文件。成功返回被打开文件的文件号，失败返回 0。本命令为初级命令。 1、#读入：从指定文件读", 14, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 337)\
    _MAKE( 206, "运行", "run", run, "本命令运行指定的可执行文件或者外部命令。如果成功，返回真，否则返回假。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 342)\
    _MAKE( 207, "取剪辑板文本", "GetClipBoardText", GetClipBoardText, "返回存放于当前 Windows 系统剪辑板中的文本。如果当前剪辑板中无文本数据，将返回空文本。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 208, "置剪辑板文本", "SetClipBoardText", SetClipBoardText, "将指定文本存放到当前 Windows 系统剪辑板中去，剪辑板中的原有内容被覆盖。成功返回真，失败返回假。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 345)\
    _MAKE( 209, "剪辑板中可有文本", "IsHaveTextInClip", IsHaveTextInClip, "如果当前 Windows 系统剪辑板中有文本数据，则返回真，否则返回假。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 210, "清除剪辑板", "ClearClipBoard", ClearClipBoard, "清除当前 Windows 系统剪辑板中的所有数据。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 211, "取屏幕宽度", "GetScreenWidth", GetScreenWidth, "返回屏幕当前显示区域的宽度，单位为像素点。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 212, "取屏幕高度", "GetScreenHeight", GetScreenHeight, "返回屏幕当前显示区域的高度，单位为像素点。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 213, "取鼠标水平位置", "GetCursorHorzPos", GetCursorHorzPos, "返回鼠标指针的当前水平位置，单位为像素点，相对于屏幕左边。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 214, "取鼠标垂直位置", "GetCursorVertPos", GetCursorVertPos, "返回鼠标指针的当前垂直位置，单位为像素点，相对于屏幕顶边。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 215, "取颜色数", "GetColorCount", GetColorCount, "返回当前显示方式所提供的最大颜色显示数目。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 216, "输入框", "InputBox", InputBox, "在一对话框中显示提示，等待用户输入正文并按下按钮。如果用户在确认输入后（按下“确认输入”按钮或回车键）退出，返回真，否则返回假。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_krnln_global_var + 346)\
    _MAKE( 217, "信息框", "MsgBox", MsgBox, "在对话框中显示信息，等待用户单击按钮，并返回一个整数告诉用户单击哪一个按钮。该整数为以下常量值之一： 0、#确认钮； 1、#取消钮； 2、#放弃钮； 3、#重试", 15, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 352)\
    _MAKE( 218, "鸣叫", "beep", beep, "通过计算机媒体设备或者喇叭发出一个声音。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 219, "取启动时间", "GetTickCount", GetTickCount, "返回自 Windows 系统启动后到现在为止所经历过的毫秒数。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 220, "置等待鼠标", "SetWaitCursor", SetWaitCursor, "本命令设置现行鼠标的形状为沙漏形，用作在即将长时间执行程序前提示操作者。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 221, "恢复鼠标", "RestroeCursor", RestroeCursor, "本命令恢复现行鼠标的原有形状，用作与“置等待鼠标”命令配对使用。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 222, "延时", "sleep", sleep, "本命令暂停当前程序的运行并等待指定的时间。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 356)\
    _MAKE( 223, "取文本注册项", "GetTextRegItem", GetTextRegItem, "在 Windows 注册表中返回指定的文本类型注册表项值。如欲读取注册项默认值，请在项目名后加“\\”号，如“test\\”。本命令为中级命令。", 15, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 357)\
    _MAKE( 224, "取数值注册项", "GetNumRegItem", GetNumRegItem, "在 Windows 注册表中返回指定的数值类型注册表项值。如欲读取注册项默认值，请在项目名后加“\\”号，如“test\\”。本命令为中级命令。", 15, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 360)\
    _MAKE( 225, "取字节集注册项", "GetBinRegItem", GetBinRegItem, "在 Windows 注册表中返回指定的字节集类型注册表项值。如欲读取注册项默认值，请在项目名后加“\\”号，如“test\\”。本命令为中级命令。", 15, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 363)\
    _MAKE( 226, "写注册项", "SaveRegItem", SaveRegItem, "在 Windows 注册表中保存或建立指定的注册表项。如欲写入注册项默认值，请在项目名后加“\\”号，如“test\\”。成功返回真，否则返回假。本命令为中级命令。", 15, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 366)\
    _MAKE( 227, "删除注册项", "DeleteRegItem", DeleteRegItem, "在 Windows 注册表中删除指定注册表项或注册表目录。如欲删除注册项默认值，请在项目名后加“\\”号，如“test\\”。成功返回真，否则返回假。注意在删除目录", 15, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 369)\
    _MAKE( 228, "注册项是否存在", "IsRegItemExist", IsRegItemExist, "如果指定注册表项存在，返回真，否则返回假。如欲检查注册项是否有默认值，请在项目名后加“\\”号，如“test\\”。本命令为中级命令。", 15, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 371)\
    _MAKE( 229, "取默认底色", "GetBackColor", GetBackColor, "取回Windows系统的默认窗口背景颜色。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 230, "快照", "GetWinPic", GetWinPic, "捕获指定窗口或屏幕上所有现有显示内容，返回相应图片数据。如果失败，返回空字节集。本命令为高级命令。", 15, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 373)\
    _MAKE( 231, "读配置项", "GetKeyText", GetKeyText, "读取指定配置文件中指定项目的文本内容。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 376)\
    _MAKE( 232, "写配置项", "SetKeyText", SetKeyText, "将指定文本内容写入指定配置项中或者删除指定的配置项或节，如果指定配置文件不存在，将会自动创建。成功返回真，失败返回假。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 380)\
    _MAKE( 233, "取配置节名", "GetSectionNames", GetSectionNames, "返回指定配置文件中所有已有节名的文本数组。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 384)\
    _MAKE( 234, "取操作系统类别", "GetSysVer", GetSysVer, "返回当前操作系统的版本类别。返回值为以下值之一：0、未知； 1、Windows 32S； 2、Windows 9X (包含Win95、Win98、WinME等)", 15, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 235, "多文件对话框", "OpenManyFileDialog", OpenManyFileDialog, "显示一个文件打开对话框，允许用户选择或输入多个所需要打开的已存在文件，返回用户所选择或输入后的结果文本数组。如果用户未输入或按“取消”按钮退出，则返回一个成员数", 15, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_krnln_global_var + 385)\
    _MAKE( 236, "取程序名称", "GetAppName", GetAppName, "返回所指定类型的程序名称. 本命令为编译预处理命令.本命令为初级命令。", 15, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 391)\
    _MAKE( 237, "交换变量", "XchgVar", XchgVar, "快速交换两个变量的内容. 本命令对于文本、字节集、自定义数据类型不会进行值数据拷贝,仅交换其指针值,因此速度要比通过赋值命令间接交换快得多.本命令要求这两个变量", 15, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 392)\
    _MAKE( 238, "强制交换变量", "ForceXchgVar", ForceXchgVar, "快速交换两个变量的内容. 本命令对于文本、字节集、自定义数据类型不会进行值数据拷贝,仅交换其指针值,因此速度要比通过赋值命令间接交换快得多.本命令与\"交换变量\"", 15, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 394)\
    _MAKE( 239, "取运行时数据类型", "GetRuntimeDataType", GetRuntimeDataType, "返回所提供参数的运行时数据类型值. 注意: 如果该数据类型是在除系统核心支持库外的其它支持库中定义的,那么每一次程序启动后该数据类型值可能会发生变化(因为载入程", 15, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 396)\
    _MAKE( 240, "宏", "Macro", Macro, "在编译程序时,通过提供一系列自定义的常量数据参数来调用系统内置或者外部提供的程序接口,获得其处理结果并将其整合进编译结果中.本命令为初级命令。命令参数表中最后一", 15, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 397)\
    _MAKE( 241, "类自身", "this", this, "只能在类方法中使用本命令,用作返回存储有指向本对象自身数据指针的变量地址.本命令为初级命令。", 15, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 242, "取操作系统类别2", "GetSysVer2", GetSysVer2, "返回当前操作系统的版本类别。返回值为以下值之一：0、未知； 1、Windows 32S； 2、Windows 9X (包含Win95、Win98、WinME等)", 15, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 243, "取最后错误", "GetLastError", GetLastError, "调用操作系统的GetLastError函数,返回该API函数被调用后的返回值.本命令为初级命令。", 15, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 244, "取统一文本", "GetUTextBin", GetUTextBin, "返回所指定文本的Unicode格式字节集。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 399)\
    _MAKE( 245, "取统一文本长度", "GetUTextLength", GetUTextLength, "返回所指定文本转换到Unicode格式文本后的字符数目。本命令为初级命令。", 15, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 402)\
    _MAKE( 246, "播放音乐", "PlayMusic", PlayMusic, "可以播放 .WAV、.MID 声音文件或相应格式的字节集声音数据、声音资源。成功返回真，失败返回假。本命令为初级命令。", 16, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 404)\
    _MAKE( 247, "停止播放", "PlayStop", PlayStop, "停止正在播放的音乐。本命令为初级命令。", 16, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 248, "播放MID", "PlayMID", PlayMID, "可以自动连续播放多个 MIDI 声音文件（注意不支持 WAV ）或相应格式的字节集声音数据、声音资源。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 16, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 406)\
    _MAKE( 249, "播放MP3", "PlayMP3", PlayMP3, "可以自动连续播放多个 MP3 音乐文件。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 16, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 409)\
    _MAKE( 250, "同步播放MP3", "SyncPlayMP3", SyncPlayMP3, "播放一次指定的MP3音乐文件，在播放过程中，会自动给指定标签组件发送反馈事件通知，以同步提供当前播放进度百分比。本命令为初级命令。", 16, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 411)\
    _MAKE( 251, "暂停播放MP3", "PauseMP3Play", PauseMP3Play, "如果当前正在播放由“播放MP3”或“同步播放MP3”命令所指定的MP3音乐，则将其暂停。本命令为初级命令。", 16, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 252, "继续播放MP3", "UnPauseMP3Play", UnPauseMP3Play, "如果当前由“播放MP3”或“同步播放MP3”命令所播放MP3音乐已经被暂停，则继续播放。本命令为初级命令。", 16, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 253, "取MP3播放状态", "GetMP3PlayState", GetMP3PlayState, "返回当前由“播放MP3”或“同步播放MP3”命令所播放MP3音乐的播放状态： 0：已经被停止或未播放； -1：正在播放但被暂停； 1：正在播放。本命令为初级命令", 16, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 254, "输出调试文本", "OutputDebugText", OutputDebugText, "本命令仅在易程序的调试版本中被执行，在发布版本中将被直接跳过。 使用本命令可以在 E 调试系统中的“输出”窗口内输出指定的文本行以帮助调试，该文本之前被自动加上", 17, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 415)\
    _MAKE( 255, "暂停", "stop", stop, "本命令仅在易程序的调试版本中被执行，在发布版本中将被直接跳过。 可以在子程序中的任何地方放置此命令，使用此命令，就相当于在程序代码中设置断点。本命令为初级命令。", 17, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 256, "检查", "assert", assert, "本命令仅在易程序的调试版本中被执行，在发布版本中将被直接跳过。 执行本命令时，如果给定参数的条件值被计算后结果为假，易程序的执行将被暂停且警示。可以在子程序中的", 17, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 416)\
    _MAKE( 257, "是否为调试版", "IsDebugVer", IsDebugVer, "如果当前 EXE 易程序执行文件为易语言编辑环境调试运行程序时编译出来的调试版本，返回真。否则表明为发布版本，返回假。本命令为初级命令。", 17, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 258, "载入", "LoadWin", LoadWin, "载入指定的窗口，根据窗口的“可视”属性决定是否显示它。成功返回真，失败返回假。本命令为初级命令。", 18, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 417)\
    _MAKE( 259, "选择", "iif", iif, "根据所提供参数的值，返回两部分中的其中一个。本命令为初级命令。", 18, _CMD_OS(__OS_WIN), _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 420)\
    _MAKE( 260, "多项选择", "choose", choose, "从参数列表项目中选择并返回一个值。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 18, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 423)\
    _MAKE( 261, "是否为空", "IsMissing", IsMissing, "返回一个逻辑值，指出在调用子程序时是否为指定参数提供了数据（该参数必定有“可空”标志），如未提供，返回真，否则返回假。本命令为初级命令。", 18, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 425)\
    _MAKE( 262, "是否已创建", "IsCreated", IsCreated, "如果指定的窗口或窗口组件有效且已经被载入或创建，返回真，否则返回假。对于窗口菜单项组件，如果其所处窗口已经被载入，命令将返回真，否则返回假。本命令为初级命令。", 18, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 426)\
    _MAKE( 263, "取数据类型尺寸", "GetDataTypeSize", GetDataTypeSize, "取指定数据类型的数据尺寸，该数据类型不能为文本、字节集、库或用户自定义数据类型。本命令为初级命令。", 18, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 427)\
    _MAKE( 264, "取颜色值", "rgb", rgb, "返回一个整数，用来表示一个红绿蓝颜色值。本命令为初级命令。", 18, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 428)\
    _MAKE( 265, "取事件组件", "GetEventUnit", GetEventUnit, "返回一个有效的组件型数据，指定当前所正在处理的事件是从何组件实际产生的。本命令主要用于在事件处理子程序中动态确定事件的来源。注意：只能在处理事件的过程首部使用本", 18, _CMD_OS(__OS_WIN), _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 266, "事件转移", "EventPost", EventPost, "设置将第一个组件上产生的所有事件转交由第二个同窗口同类型组件的事件处理子程序去处理，就好象此事件是在第二个组件上实际发生的一样。本命令可用来设置用同一事件处理子", 18, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 431)\
    _MAKE( 267, "复制窗口组件", "CopyWinUnit", CopyWinUnit, "复制创建与指定窗口组件设计状态完全一致的新组件。该被复制组件将放在原组件的下面，其事件被自动转移到原组件上。如果复制时被复制窗口组件所在的窗口尚未被载入，那么复", 18, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 433)\
    _MAKE( 268, "处理事件", "DoEvents", DoEvents, "暂时转让控制权，以便让 Windows 操作系统有机会处理其它的如用户键盘或鼠标输入等事件。直到操作系统处理并发送完程序队列中的所有事件后，命令才会返回。本命令", 18, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 269, "载入图片", "LoadPic", LoadPic, "载入指定图片，以供画板、打印机等对象的“画图片”、“取图片宽度”、“取图片高度”之类成员命令使用。如成功则返回被载入图片的图片号，失败返回 0。本命令为初级命令", 18, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 435)\
    _MAKE( 270, "卸载图片", "UnloadPic", UnloadPic, "卸载被载入的图片，图片被卸载后不能再被使用。如果某图片被载入后未被卸载，在易程序退出时将自动被卸载。本命令为初级命令。", 18, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 436)\
    _MAKE( 271, "取硬盘特征字", "GetHDiskCode", GetHDiskCode, "返回电脑中第一个物理硬盘的物理特征字，该特征字是仅硬件相关的，也就是说与任何软件系统都无关（包括操作系统）。用户可以使用此特征字来限制自己的程序仅在某一台计算机", 18, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 272, "取系统语言", "GetSysLang", GetSysLang, "返回当前系统运行时环境所支持的语言版本类型，为以下常量值之一： 1、GBK中文； 2、英文； 3、BIG5中文； 4、日文。本命令为初级命令。", 18, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 273, "写到内存", "WriteMem", WriteMem, "将数据写出到指定的内存区域，注意调用本命令前一定要确保所提供的内存区域真实有效。本命令的最佳使用场合就是在易语言回调子程序和易语言DLL公开子程序中用作对外输出", 18, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 437)\
    _MAKE( 274, "DLL命令调用转向", "SetDllCmdInf", SetDllCmdInf, "本命令用来设置下次程序中执行任意一条DLL命令时所使用的DLL库文件名和在DLL库文件中的调用命令名，此设置将覆盖该被调用DLL命令在程序设计时所提供的原始值。", 18, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 440)\
    _MAKE( 275, "置错误提示管理", "SetErrorManger", SetErrorManger, "本命令用来设置当运行时如果产生了导致程序崩溃的严重错误时用来对该错误进行详细提示的子程序地址，如果未进行此项设置，发生严重错误时运行时环境将自动提示该错误的详细", 18, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 442)\
    _MAKE( 276, "置DLL装载目录", "SetDllCmdLoadPath", SetDllCmdLoadPath, "本命令用来设置当程序中执行到DLL命令时装载其DLL库文件的优先装载路径，即系统将优先到该路径下去装载指定的DLL文件。本命令所设置结果对所有DLL命令设置中的", 18, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 443)\
    _MAKE( 277, "取组件名称", "GetUnitName", GetUnitName, "返回指定窗口组件的名称，如果所提供对象不是窗口组件，将返回空文本。本命令为初级命令。", 18, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 444)\
    _MAKE( 278, "取对象类型", "GetObjectType", GetObjectType, "如果指定对象的类型为在支持库内定义的数据类型，则返回该数据类型的名称，否则返回空文本。本命令为初级命令。", 18, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 445)\
    _MAKE( 279, "寻找组件", "FindUnit", FindUnit, "按照指定条件寻找符合要求的窗口组件，成功返回非零寻找句柄，未找到返回零。注意，所返回的非零寻找句柄在不再使用后必须调用“清除组件寻找句柄”命令释放。本命令为初级", 18, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 446)\
    _MAKE( 280, "取找到组件数目", "GetFoundUnitCount", GetFoundUnitCount, "返回“寻找组件”命令所找到的组件数目。本命令为初级命令。", 18, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 451)\
    _MAKE( 281, "取所找到组件", "GetFoundUnit", GetFoundUnit, "返回“寻找组件”命令所找到的指定索引位置处的组件。本命令为初级命令。", 18, _CMD_OS(__OS_WIN), _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 452)\
    _MAKE( 282, "清除组件寻找句柄", "ReleaseFounddHandle", ReleaseFounddHandle, "释放“寻找组件”命令所返回的组件寻找句柄，此句柄以后不能再使用。本命令为初级命令。", 18, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 454)\
    _MAKE( 283, "置入代码", "MachineCode", MachineCode, "在编译后文件代码段中当前语句位置置入指定的机器指令码数据。本命令为高级命令。", 18, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 455)\
    _MAKE( 284, "运行控制台程序", "RunConsoleApp", RunConsoleApp, "运行指定的控制台程序(不会打开控制台窗口),返回是否运行成功.本命令为初级命令。", 18, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 456)\
    _MAKE( 285, "取核心库版本", "GetKrnlLibVer", GetKrnlLibVer, "返回核心库的当前版本号。本命令为初级命令。", 18, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 286, "条件宏是否存在", "IsCondMacroDefined", IsCondMacroDefined, "返回所指定的编译条件宏是否已经被定义. 编译条件宏在\"程序->配置->编译条件\"菜单功能中设置,所有在当前被激活编译条件中存在的条件宏均认为被定义.如果当前为编", 18, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 460)\
    _MAKE( 287, "是否支持多用户", "MutiUser", MutiUser, "检查本支持库所提供的数据库功能是否支持多用户同时对数据库操作。如果支持，返回真，否则返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 288, "取错误码", "GetErrCode", GetErrCode, "如果执行某数据库命令失败，在其后执行本命令可以取回非 0 的数据库错误码值。如果该数据库命令执行成功，执行本命令将返回 0 。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 289, "取错误信息", "GetErrMsg", GetErrMsg, "如果执行某数据库命令失败，在其后执行本命令可以取回错误信息文本。如果该数据库命令执行成功，执行本命令将返回空文本。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 290, "创建", "create", create, "创建指定的数据库文件，如果该数据库文件存在，将被覆盖。成功返回真，失败返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 461)\
    _MAKE( 291, "打开", "use", use, "打开指定的数据库文件。成功返回真，并自动将当前数据库设置为此数据库，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。 1、#无限制：允许其", 19, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_krnln_global_var + 463)\
    _MAKE( 292, "替换打开", "ruse", ruse, "打开指定的数据库文件。成功返回真，并自动关闭当前数据库后将当前数据库设置为此数据库，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。 1、", 19, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_krnln_global_var + 470)\
    _MAKE( 293, "置当前库", "select", select, "本命令选择一个已经被打开的数据库为当前数据库，大部分数据库操作命令都针对当前数据库，譬如：记录指针的移动、字段的读写等等。如果指定数据库不存在或者尚未被打开，命", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 477)\
    _MAKE( 294, "取当前库", "dbf", dbf, "返回当前数据库的别名，如不存在别名，则返回其名称。如果未设置当前数据库，将返回空文本。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 295, "关闭", "CloseDBF", CloseDBF, "本命令关闭已经被打开的指定数据库。如果没有提供数据库名称，默认为关闭当前数据库。如果当前数据库被关闭，当前数据库状态值将被设置为空，此时可重新使用“置当前库”命", 19, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 478)\
    _MAKE( 296, "全部关闭", "CloseAll", CloseAll, "本命令关闭当前已经被打开的所有数据库。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 297, "取库文件名", "GetDBF", GetDBF, "返回当前数据库的全路径文件名。如果当前数据库不存在，返回空文本。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 298, "是否已打开", "used", used, "如果指定数据库文件已经被打开，返回真，否则返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 479)\
    _MAKE( 299, "取记录数", "RecCount", RecCount, "返回当前数据库的记录数目。如果没有设置当前数据库或者失败，将返回 0 。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 300, "取创建时间", "DateCreated", DateCreated, "返回当前数据库被创建的时间。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_DATE_TIME, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 301, "取字段数", "FCount", FCount, "返回当前数据库中字段的数目。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 302, "取字段名", "GetFieldName", GetFieldName, "返回当前数据库中指定字段的名称。如果指定字段不存在，将返回空文本。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 480)\
    _MAKE( 303, "改字段名", "ModiFieldName", ModiFieldName, "修改当前数据库中指定字段的名称。本命令只有当数据库的共享打开方式为“#禁止读写”时才有效。成功返回真，失败返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 481)\
    _MAKE( 304, "取字段类型", "GetFieldType", GetFieldType, "返回当前数据库中指定字段的类型，字段类型值为以下常量值之一： 1、#字节型； 2、#短整数型； 3、#整数型； 4、#长整数型； 5、#小数型； 6、#双精度小", 19, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 483)\
    _MAKE( 305, "取字段尺寸", "FSize", FSize, "返回当前数据库中指定字段在记录中所占的尺寸。如果指定字段不存在，将返回 0 。注意：备注型和字节集型字段将返回 4 。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 484)\
    _MAKE( 306, "新建索引", "CreateIndex", CreateIndex, "在当前数据库中创建并打开一个索引文件。索引的存在将降低对数据库记录的更新速度，所以除非是较大的数据库（最起码在一千条记录以上），否则一般不用使用索引。成功返回真", 19, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 485)\
    _MAKE( 307, "置当前索引", "SetIndexTo", SetIndexTo, "为当前数据库指定当前索引，在进行索引查找时将使用当前索引。如果指定索引不存在，将返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 490)\
    _MAKE( 308, "取当前索引", "ndx", ndx, "返回当前数据库的当前索引名称。如果未设置当前数据库或无当前索引，返回空文本。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 309, "更新索引", "reindex", reindex, "通过完全重建来更新当前数据库的当前索引。成功返回真，失败返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 310, "取索引数", "GetIndexCount", GetIndexCount, "返回当前数据库中已经被打开索引的数目。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 311, "取索引名", "GetIndexName", GetIndexName, "返回当前数据库中指定索引的名称。如果指定位置处不存在索引，将返回空文本。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 491)\
    _MAKE( 312, "取索引字段", "GetIndexField", GetIndexField, "返回当前数据库中指定索引内所有被索引字段的名称数组。如果指定位置处不存在索引，将返回空数组。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 492)\
    _MAKE( 313, "置加锁重试时间", "SetLockTimeout", SetLockTimeout, "本命令设置对数据库进行加锁时，如果该位置已经被其他用户锁住或者正在被其他用户访问，系统反复重试加锁的时间。当你的数据库可能被多用户同时访问时，必须在程序首使用本", 19, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 493)\
    _MAKE( 314, "锁住数据库", "LockDBF", LockDBF, "本命令执行后独占锁住整个当前数据库，拒绝其它用户的任何访问。成功返回真，失败返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 315, "解锁数据库", "UnlockDBF", UnlockDBF, "本命令解除“锁住数据库”命令对当前数据库加的锁。成功返回真，失败返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 316, "锁住增删", "LockCount", LockCount, "本命令执行后独占锁住对当前数据库的增删操作，其它用户可以读写已存在的任何记录，但是不能对数据库记录进行增删。也就是说：不能执行改变数据库记录数目的任何操作。注意", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 317, "解增删锁", "UnlockCount", UnlockCount, "本命令解除“锁住增删”命令对当前数据库加的锁。成功返回真，失败返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 318, "锁住记录", "LockRec", LockRec, "本命令执行后可以锁住指定的一个或多个记录。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 19, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 494)\
    _MAKE( 319, "解锁记录", "UnlockRec", UnlockRec, "本命令解除“锁住记录”命令对记录所加的锁。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 19, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 495)\
    _MAKE( 320, "全部解锁", "UnlockAll", UnlockAll, "本命令解除由“锁住数据库”、“锁住增删”、“锁住记录”等命令对当前数据库加上的所有锁。成功返回真，失败返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 321, "取平均值", "avg", avg, "本命令根据当前数据库计算并返回某数值型表达式的平均值，命令执行后当前记录指针保持不变。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 496)\
    _MAKE( 322, "求和", "sum", sum, "本命令根据当前数据库计算并返回某数值型表达式的和，命令执行后当前记录指针保持不变。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 498)\
    _MAKE( 323, "取最大值", "max", max, "本命令根据当前数据库计算并返回某数值型表达式的最大值，当前记录指针被移动到具有最大值的记录。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 500)\
    _MAKE( 324, "取最小值", "min", min, "本命令根据当前数据库计算并返回某数值型表达式的最小值，当前记录指针被移动到具有最小值的记录。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 502)\
    _MAKE( 325, "取最大时间", "MaxTime", MaxTime, "本命令根据当前数据库计算并返回某日期时间型表达式的最大值，当前记录指针被移动到具有最大值的记录。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_DATE_TIME, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 504)\
    _MAKE( 326, "取最小时间", "MinTime", MinTime, "本命令根据当前数据库计算并返回某日期时间型表达式的最小值，当前记录指针被移动到具有最小值的记录。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_DATE_TIME, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 506)\
    _MAKE( 327, "计算数目", "count", count, "本命令返回当前数据库中指定范围记录的数目。命令执行后当前记录指针保持不变。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 508)\
    _MAKE( 328, "复制结构", "CopyStructTo", CopyStructTo, "复制当前数据库的结构到另外一个数据库。命令执行后当前记录指针保持不变。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 19, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 509)\
    _MAKE( 329, "复制记录", "CopyTo", CopyTo, "复制当前数据库的记录到另外一个数据库。命令执行后当前记录指针保持不变。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 19, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 511)\
    _MAKE( 330, "计算排序", "csort", csort, "根据指定数值表达式的计算值排序复制当前数据库的记录到另外一个数据库。命令执行后当前记录指针保持不变。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一", 19, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 514)\
    _MAKE( 331, "排序", "sort", sort, "根据指定字段排序复制当前数据库的记录到另外一个数据库。命令执行后当前记录指针保持不变。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重", 19, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 519)\
    _MAKE( 332, "分类计算", "total", total, "对当前数据库进行分类计算，将结果输出到另外一个数据库。注意所有的备注或字节集字段内容将不会被输出到目的数据库中。 命令执行后当前记录指针保持不变。成功返回真，失", 19, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 524)\
    _MAKE( 333, "添加", "AppendFrom", AppendFrom, "将指定数据库中的记录添加到当前数据库。命令执行后当前记录指针保持不变。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 19, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 529)\
    _MAKE( 334, "加记录", "Append", Append, "本命令在当前数据库的尾部添加一条记录，并将当前记录指针移动到该记录处。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 19, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 532)\
    _MAKE( 335, "加空记录", "AppendBlank", AppendBlank, "本命令在当前数据库的尾部添加一条空记录，并将当前记录指针移动到该记录上。成功返回真，失败返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 336, "替换", "replace", replace, "本命令替换当前数据库中的记录数据，当前记录指针保持不变。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 19, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 533)\
    _MAKE( 337, "修改", "modify", modify, "本命令一次性修改当前数据库中当前记录内多个字段的内容，当前记录指针保持不变。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 19, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 535)\
    _MAKE( 338, "删除", "delete", delete_, "标记删除当前数据库中的当前记录或者指定范围内的记录，当前记录指针位置保持不变。注意：此命令并不实际删除记录，仅仅只加上删除标记，记录依旧存在并可以读写。成功返回", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 536)\
    _MAKE( 339, "是否已删除", "IsDeleted", IsDeleted, "如果当前数据库的当前记录已经被“删除”命令加上删除标记，返回真，否则返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 340, "恢复删除", "recall", recall, "去掉当前数据库中当前记录或者指定范围内记录的删除标记，当前记录指针位置保持不变。成功返回真，失败返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 537)\
    _MAKE( 341, "彻底删除", "pack", pack, "将当前数据库中所有被加上删除标记的记录从数据库中彻底清除，当前记录指针位置将被移动到数据库首记录。记录被彻底删除后将不复存在，并且无法恢复。注意执行本命令将重组", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 342, "清空", "zap", zap, "彻底清除当前数据库中的所有记录。成功返回真，失败返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 343, "读", "r", r, "返回当前数据库中当前记录处指定字段的数据内容，所返回数据的类型等同于字段的类型。如果指定字段不存在，将产生运行时错误。执行本命令后使用“取错误码”命令可以判断是", 19, _CMD_OS(__OS_WIN), _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 538)\
    _MAKE( 344, "写", "w", w, "将数据写入到当前数据库中当前记录处的指定字段内。成功返回真，失败返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 539)\
    _MAKE( 345, "读字段", "ReadField", ReadField, "返回指定数据库中当前记录处指定字段的数据内容，所返回数据的类型等同于字段的类型。如果指定字段不存在，将产生运行时错误。本命令与“读”命令的不同之处为本命令可以从", 19, _CMD_OS(__OS_WIN), _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 541)\
    _MAKE( 346, "写字段", "WriteField", WriteField, "将数据写入到指定数据库中当前记录处的指定字段内。成功返回真，失败返回假。本命令与“写”命令的不同之处为本命令可以写入非当前数据库中的字段。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 543)\
    _MAKE( 347, "附加字节集", "AppendBin", AppendBin, "将字节集数据添加到当前记录处指定字节集型字段的尾部。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 19, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 546)\
    _MAKE( 348, "附加备注", "AppendMemo", AppendMemo, "将文本数据添加到当前记录处指定备注型字段的尾部。成功返回真，失败返回假。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 19, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 548)\
    _MAKE( 349, "索引查找", "seek", seek, "本命令在当前数据库中使用其当前索引来快速寻找某一记录，寻找从当前记录位置（包括当前记录）处开始。注意为命令所提供的欲搜寻值参数的数目必须与被索引字段的数目一致。", 19, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 550)\
    _MAKE( 350, "查找", "locate", locate, "本命令从当前数据库中当前记录位置处（包括当前记录）开始寻找符合给定条件的记录。如成功找到返回真，并且将当前记录指针移至所找到的记录。如出错或未找到则返回假，当前", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 551)\
    _MAKE( 351, "到首记录", "GoTop", GoTop, "将当前数据库中当前记录指针移动到数据库的第一条记录上。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 352, "到尾记录", "GoBottom", GoBottom, "将当前数据库中当前记录指针移动到数据库的最后一条记录上。成功返回真，失败返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 353, "跳过", "skip", skip, "将当前数据库中当前记录指针向前或者向后移动数条记录。成功返回真，失败返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 552)\
    _MAKE( 354, "取记录号", "RecNO", RecNO, "返回当前数据库中当前记录指针所指向记录的记录号。如果当前记录指针在首记录前，返回 0 ；如果在尾记录后，返回值为记录总数加一。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 355, "跳到", "goto", goto_, "改变当前数据库中的当前记录指针到指定的记录号。如果所欲跳到记录超出了数据库的首尾边界，将自动停留在对应边界记录处。成功返回真，失败返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 553)\
    _MAKE( 356, "取标签", "GetBookmark", GetBookmark, "在数据库的存在期间，同一数据库内任何时候任何一条记录（包括已经被实际删除的记录）都有一个唯一的非 0 正整数标签值，使用本命令可以取回当前数据库当前记录的这一标", 19, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 357, "记录是否存在", "IsRecExist", IsRecExist, "如果当前数据库中存在具有指定标签值的记录，命令返回真，否则返回假。本命令可以用作在多用户环境中检查某记录是否被其它用户实际删除。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 554)\
    _MAKE( 358, "标签跳转", "GotoBookmark", GotoBookmark, "使用本命令可以改变当前数据库中的当前记录指针到具有指定标签的记录。如成功找到返回真，并且将当前记录指针移至该记录。如出错或未找到则返回假，当前记录指针位置保持不", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 555)\
    _MAKE( 359, "首记录前", "bof", bof, "如果当前数据库的当前记录指针已在首记录的前面，返回真，否则返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 360, "尾记录后", "eof", eof, "如果当前数据库的当前记录指针已在尾记录的后面，返回真，否则返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 361, "写出缓存", "flush", flush, "为了加快对数据库数据的存取速度，对数据库所进行的写操作将首先被保存在写缓存区内，等候一段时间后才会被自动写入到数据库文件中。使用本命令可以将当前数据库停留在写缓", 19, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 362, "写出所有缓存", "FlushAll", FlushAll, "将所有数据库停留在写缓存区内的数据都立即写入到对应的数据库磁盘文件中。一般情况下无需使用本命令。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 363, "编辑", "edit", edit, "提供一个对话框，用作查看或编辑修改当前数据库的记录，起始位置从当前记录开始。本命令退出后当前记录指针被移动到首记录。本命令为初级命令。命令参数表中最后一个参数可", 19, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 556)\
    _MAKE( 364, "是否已加密", "IsEncrypted", IsEncrypted, "如果需要提供密码才能访问指定的数据库文件（即已经被加密），返回真，否则返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 559)\
    _MAKE( 365, "置数据库密码", "SetPassword", SetPassword, "本命令用作设置或修改当前数据库的访问密码，注意数据库文件必须可写。成功返回真，失败返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 560)\
    _MAKE( 366, "密码输入框", "InputDBPwd", InputDBPwd, "本命令用作输入数据库的访问密码，如果用户在确认输入后（按下“确认”按钮或回车键）退出，返回真，否则返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 561)\
    _MAKE( 367, "复制密码", "CopyPassword", CopyPassword, "本命令可以将当前数据库的访问密码设置为等同于其他已打开数据库，注意数据库文件必须可写。成功返回真，失败返回假。本命令为初级命令。", 19, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 563)\
    _MAKE( 368, "取主机名", "GetHostName", GetHostName, "返回本机的主机名，用作在网络通讯中标志本机地址。本命令为初级命令。", 20, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 369, "通信测试", "ping", ping, "测试与指定主机是否能够正常通讯。返回被测试主机的通讯响应时间。如果无法通讯或者测试失败，返回 -1。本命令为初级命令。", 20, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 564)\
    _MAKE( 370, "转换为主机名", "IPToHostName", IPToHostName, "将指定的 IP 地址转换为其主机名。如果失败返回空文本。本命令为初级命令。", 20, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 566)\
    _MAKE( 371, "转换为IP地址", "HostNameToIP", HostNameToIP, "将指定的主机名转换为其 IP 地址。如果失败返回空文本。本命令为初级命令。", 20, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 567)\
    _MAKE( 372, "标准输出", "fputs", fputs, "在标准输出设备或标准错误设备上输出指定的内容，注意本命令只能在控制台程序中使用。本命令为初级命令。命令参数表中最后一个参数可以被重复添加。", 21, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 568)\
    _MAKE( 373, "标准输入", "fgets", fgets, "在标准输入设备上请求输入最多包含2048个字符的一行文本，返回用户所输入的内容。注意本命令只能在控制台程序中使用。本命令为初级命令。", 21, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 570)\
    _MAKE( 374, "载入易包文件", "LoadEPKFile", LoadEPKFile, "预先载入所指定的易包文件(如果文件名未提供后缀则默认为\".epk\"),防止系统以后在程序中调用到该易包内公开子程序时采用默认方法自动载入.成功返回真,失败返回假", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 571)\
    _MAKE( 375, "载入易包数据", "LoadEPKData", LoadEPKData, "预先载入所指定易包的数据,防止系统以后在程序中调用到该易包内公开子程序时采用默认方法自动载入.成功返回真,失败返回假.所载入易包不需要刻意去释放,在程序退出时会", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 573)\
    _MAKE( 376, "卸载易包", "UnloadEPK", UnloadEPK, "卸载已被载入的指定名称的易包,成功返回真,失败返回假.本命令不支持在多线程环境和静态编译模式下使用.执行本命令前,必须确保以下几点,否则后果无法预测: 1. 在", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 575)\
    _MAKE( 377, "易包是否被载入", "IsEPKLoaded", IsEPKLoaded, "返回所指定名称的易包是否已经被载入.本命令不支持在静态编译模式下使用。本命令为高级命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 576)\
    _MAKE( 378, "复制易包常量", "CloneConstData", CloneConstData, "本命令仅在编译易包时有效,用作根据所提供的常量数据建立返回对应的非常量数据.在对易包公开子程序以参考方式传递进来的参数变量使用文本/字节集/数组三类常量进行赋值", 22, _CMD_OS(__OS_WIN), _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 577)\
    _MAKE( 379, "取窗口句柄", "GetHWnd", GetHWnd, "取出本窗口或窗口组件的窗口句柄（即HWND）。本命令为高级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 380, "销毁", "destroy", destroy, "销毁本窗口或窗口组件。窗口被销毁后，所有在载入本窗口之后对本窗口及其内窗口组件所进行的修改或设置都将被抛弃，窗口被销毁后如想再次使用必须重新装载。注意：当窗口调", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 578)\
    _MAKE( 381, "获取焦点", "SetFocus", SetFocus, "将焦点移动到窗口组件。如果对窗口使用本方法，窗口将自动把焦点转移到第一个有能力保留焦点的窗口组件上去。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 382, "可有焦点", "IsFocus", IsFocus, "如果当前窗口组件具有焦点，则返回真，否则返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 383, "取用户区宽度", "GetClientWidth", GetClientWidth, "返回窗口或窗口组件用户区域的宽度，单位为像素点。对窗口而言，用户区域为窗口区域减去标题栏、菜单栏、边框后的区域；对窗口组件而言，用户区域等同于其窗口组件区域。本", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 384, "取用户区高度", "GetClientHeight", GetClientHeight, "返回窗口或窗口组件用户区域的高度，单位为像素点。对窗口而言，用户区域为窗口区域减去标题栏、菜单栏、边框后的区域；对窗口组件而言，用户区域等同于其窗口组件区域。本", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 385, "禁止重画", "LockWindowUpdate", LockWindowUpdate, "禁止窗口或窗口组件重画，以避免当频繁进行操作时引起闪烁。注意调用本命令后必须调用“允许重画”命令进行恢复。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 386, "允许重画", "UnlockWindowUpdate", UnlockWindowUpdate, "重新允许被禁止的窗口或窗口组件重画。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 387, "重画", "invalidate", invalidate, "通知 Windows 系统本窗口或窗口组件上的显示内容需要在以后被全部重画。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 388, "部分重画", "InvalidateRect", InvalidateRect, "通知 Windows 系统本窗口或窗口组件上的显示内容需要在以后被部分重画。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 579)\
    _MAKE( 389, "取消重画", "validate", validate, "通知 Windows 系统本窗口或窗口组件不再需要被重画，保留现有的全部显示内容。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 390, "刷新显示", "UpdateWindow", UpdateWindow, "如果本窗口或窗口组件上的显示内容需要被全部或部分重画，则立即进行重画更新，否则不进行任何操作直接返回。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 391, "移动", "move", move, "改变窗口或窗口组件的位置或尺寸。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 583)\
    _MAKE( 392, "调整层次", "ZOrder", ZOrder, "改变窗口或窗口组件的现行所处层次。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 587)\
    _MAKE( 393, "弹出菜单", "PopupMenu", PopupMenu, "用于在窗口上的当前鼠标位置或指定坐标位置显示弹出式菜单，如果调用对象为窗口组件，则自动使用其所在的窗口。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 588)\
    _MAKE( 394, "发送信息", "SendMessage", SendMessage, "将指定信息发送到窗口或窗口组件，并等待且取回信息反馈值。本命令为中级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 591)\
    _MAKE( 395, "投递信息", "PostMessage", PostMessage, "将指定信息发送到窗口或窗口组件，不等待直接返回。本命令为中级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 594)\
    _MAKE( 396, "取标记组件", "GetSpecTagUnit", GetSpecTagUnit, "返回当前窗口中具有指定标记数值文本的组件。如果不存在，将产生运行时错误。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 597)\
    _MAKE( 397, "置外形图片", "SetShapePic", SetShapePic, "使用图片来设置窗口的外形，注意图片类型不能为图标及鼠标指针。另外，图片的轮廓应该尽量简单，以免影响窗口的刷新速度。如果调用对象为窗口组件，将自动使用其所在的窗口", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 598)\
    _MAKE( 398, "激活", "Activate", Activate, "本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 399, "置托盘图标", "SetTrayIcon", SetTrayIcon, "设置本程序在系统托盘中的图标。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 600)\
    _MAKE( 400, "弹出托盘菜单", "PopupTrayMenu", PopupTrayMenu, "在当前鼠标位置弹出指定菜单，本命令一般用作支持托盘菜单的弹出。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 602)\
    _MAKE( 401, "置父窗口", "SetParentWnd", SetParentWnd, "设置指定窗口或窗口组件为本对象窗口或窗口组件的父窗口。本命令为初级对象成员命令。 成员属性“标题”所在数据类型为“窗口”，英文名称为“caption”，类型为“", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 603)\
    _MAKE( 402, "加入文本", "AddText", AddText, "将指定文本加入到编辑框内容的尾部。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添加。 成员属性“内容”所在数据类型为“编辑框”，英文名称为“co", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 604)\
    _MAKE( 403, "取设备句柄", "GetHDC", GetHDC, "如当前用户程序正在处理本画板所产生的“绘画”事件，则返回画板所对应的设备句柄（即HDC），否则返回 0。本命令为高级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 404, "清除", "cls", cls, "清除画板上指定区域的内容并将当前文本写出位置移动到被清除区左上角。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 605)\
    _MAKE( 405, "取点", "GetPixel", GetPixel, "返回画板上指定点的颜色值。如果失败，将返回 -1。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 609)\
    _MAKE( 406, "画点", "SetPixel", SetPixel, "本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 611)\
    _MAKE( 407, "画直线", "LineTo", LineTo, "使用画笔在画板上画出一条直线。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 614)\
    _MAKE( 408, "画椭圆", "ellipse", ellipse, "本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 618)\
    _MAKE( 409, "画弧线", "ArcTo", ArcTo, "使用画笔在画板上画出一条弧线。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 8, g_argumentInfo_krnln_global_var + 622)\
    _MAKE( 410, "画弦", "chord", chord, "本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 8, g_argumentInfo_krnln_global_var + 630)\
    _MAKE( 411, "画饼", "pie", pie, "本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 8, g_argumentInfo_krnln_global_var + 638)\
    _MAKE( 412, "画矩形", "DrawRect", DrawRect, "使用画笔在画板上画出一个矩形，矩形的内部使用刷子填充。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 646)\
    _MAKE( 413, "画渐变矩形", "DrawJBRect", DrawJBRect, "本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添加。", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_krnln_global_var + 650)\
    _MAKE( 414, "填充矩形", "FillRect", FillRect, "将画板上指定的矩形区域用当前刷子填充。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 657)\
    _MAKE( 415, "画圆角矩形", "RoundRect", RoundRect, "使用画笔在画板上画出一个圆角矩形，圆角矩形的内部使用刷子填充。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_krnln_global_var + 661)\
    _MAKE( 416, "翻转矩形区", "InvertRect", InvertRect, "将画板上指定矩形区域的颜色翻转过来。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 667)\
    _MAKE( 417, "画多边形", "polygon", polygon, "如果所画的多边形没有闭合，将自动闭合。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 671)\
    _MAKE( 418, "置写出位置", "SetWritePos", SetWritePos, "设置下次使用“写文本行”或“写出”命令输出数据时的位置。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 673)\
    _MAKE( 419, "写文本行", "print", print, "在当前写出位置写出指定的文本、数值、逻辑值或日期时间，并将现行写出位置调整到下行行首。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添加。", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 675)\
    _MAKE( 420, "滚动写行", "sprint", sprint, "在当前写出位置写出指定的文本、数值、逻辑值或日期时间，并将现行写出位置调整到下行行首。如果现行画板高度无法容纳当前所要写出的行，则自动向上滚动画板内容。本命令为", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 676)\
    _MAKE( 421, "写出", "write", write, "在当前写出位置处写出指定的文本、数值、逻辑值或日期时间。自动调整现行写出位置到所写出数据的末位置。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 677)\
    _MAKE( 422, "定位写出", "say", say, "在指定写出位置处写出指定的文本、数值、逻辑值或日期时间，不改变现行写出位置。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添加。", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 678)\
    _MAKE( 423, "取宽度", "GetWidth", GetWidth, "返回指定数据的写出宽度，使用当前绘画单位。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 681)\
    _MAKE( 424, "取高度", "GetHeight", GetHeight, "返回指定数据的写出高度，使用当前绘画单位。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 682)\
    _MAKE( 425, "画图片", "DrawPic", DrawPic, "本命令为初级对象成员命令。 1、#拷贝； 2、#翻转拷贝； 3、#位异或； 4、#位或； 5、#位与 本参数也可以指定透明色，但必须是负颜色数值。如： 画板1.", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_krnln_global_var + 683)\
    _MAKE( 426, "取图片宽度", "GetPicWidth", GetPicWidth, "返回指定图片的宽度，使用当前绘画单位。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 689)\
    _MAKE( 427, "取图片高度", "GetPicHeight", GetPicHeight, "返回指定图片的高度，使用当前绘画单位。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 690)\
    _MAKE( 428, "复制", "copy", copy, "将源画板（本命令的调用画板对象）中指定区域的内容快速复制到目的画板中的指定位置，使用源和目的画板各自的当前绘画单位。如果源画板当前不可视，其“自动重画”属性必须", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 8, g_argumentInfo_krnln_global_var + 691)\
    _MAKE( 429, "取图片", "GetPic", GetPic, "返回画板上所有现有显示内容的图片数据。如果失败，返回空字节集。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 699)\
    _MAKE( 430, "单位转换", "UnitCnv", UnitCnv, "将像素单位座标值转换到当前绘画单位，或将当前绘画单位座标值转换到像素单位。本命令为初级对象成员命令。 成员属性“边框”所在数据类型为“画板”，英文名称为“bor", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 701)\
    _MAKE( 431, "调用反馈事件", "SendLabelMsg", SendLabelMsg, "产生标签的反馈事件，以调用此标签的“反馈事件”用户事件处理子程序，可以用作在多线程处理中将控制权转移到程序主线程上去执行。返回用户事件处理子程序所返回的值，如果", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 703)\
    _MAKE( 432, "取顶端可见项目", "GetTopIndex", GetTopIndex, "返回组合框列表部分中当前最顶端可见项目的索引。0 为项目一，1 为项目二，如此类推。失败返回 -1 。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 433, "置顶端可见项目", "SetTopIndex", SetTopIndex, "设置组合框列表部分中当前最顶端的可见项目，必要时将自动滚动组合框的列表部分。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 706)\
    _MAKE( 434, "取项目数", "GetCount", GetCount, "本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 435, "取项目数值", "GetItemData", GetItemData, "返回与指定项目相关联的数值。如果指定项目不存在，将返回 -1 。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 707)\
    _MAKE( 436, "置项目数值", "SetItemData", SetItemData, "设置与指定项目相关联的数值。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 708)\
    _MAKE( 437, "取项目文本", "GetItemText", GetItemText, "返回指定项目的文本。如果指定项目不存在，将返回空文本。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 710)\
    _MAKE( 438, "置项目文本", "SetItemtext", SetItemtext, "设置指定项目的文本。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 711)\
    _MAKE( 439, "加入项目", "AddString", AddString, "加入指定项目到组合框列表部分的尾部，成功返回加入后该项目所处的位置，失败返回 -1 。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 713)\
    _MAKE( 440, "插入项目", "InsertString", InsertString, "插入指定项目到组合框列表部分的指定位置处，成功返回插入后该项目所处的位置，失败返回 -1 。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 715)\
    _MAKE( 441, "删除项目", "DeleteString", DeleteString, "删除组合框列表部分指定位置处的项目。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 718)\
    _MAKE( 442, "清空", "clear", clear, "删除组合框列表部分中的所有项目。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 443, "选择", "SelItem", SelItem, "在所有项目中寻找首部包含指定文本的项目，如找到，则选中它，并返回该项目的位置索引，否则返回 -1。本命令为初级对象成员命令。 成员属性“类型”所在数据类型为“组", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 719)\
    _MAKE( 444, "取顶端可见项目", "GetTopIndex", GetTopIndex, "返回列表框中当前最顶端可见项目的索引。0 为项目一，1 为项目二，如此类推。失败返回 -1 。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 445, "置顶端可见项目", "SetTopIndex", SetTopIndex, "设置列表框中当前最顶端的可见项目，必要时将自动滚动列表框。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 720)\
    _MAKE( 446, "取项目数", "GetCount", GetCount, "本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 447, "取项目数值", "GetItemData", GetItemData, "返回与指定项目相关联的数值。如果指定项目不存在，将返回 -1 。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 721)\
    _MAKE( 448, "置项目数值", "SetItemData", SetItemData, "设置与指定项目相关联的数值。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 722)\
    _MAKE( 449, "取项目文本", "GetItemText", GetItemText, "返回指定项目的文本。如果指定项目不存在，将返回空文本。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 724)\
    _MAKE( 450, "置项目文本", "SetItemtext", SetItemtext, "设置指定项目的文本。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 725)\
    _MAKE( 451, "取已选择项目数", "GetSelCount", GetSelCount, "返回已被选择项目的数目。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 452, "取所有被选择项目", "GetSelItems", GetSelItems, "返回一个整数数组，內含所有当前被选择项目的位置索引。如果当前没有被选择项目，返回空数组。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 453, "是否被选择", "IsSelected", IsSelected, "如果指定项目被选择，则返回真，否则返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 727)\
    _MAKE( 454, "选择项目", "select", select, "选择或取消选择指定项目。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 728)\
    _MAKE( 455, "取焦点项目", "GetCaretIndex", GetCaretIndex, "本命令仅在多选列表框中使用，用作返回当前焦点项目的位置索引。如果在单选列表框中使用本命令，将返回当前被选择项目的位置索引。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 456, "置焦点项目", "SetCaretIndex", SetCaretIndex, "本命令仅在多选列表框中使用，用作设置当前焦点项目。如果在单选列表框中使用本命令，将设置当前被选择项目。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 730)\
    _MAKE( 457, "加入项目", "AddString", AddString, "加入指定项目到列表框的尾部，成功返回加入后该项目所处的位置，失败返回 -1 。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 731)\
    _MAKE( 458, "插入项目", "InsertString", InsertString, "插入指定项目到列表框的指定位置处，成功返回插入后该项目所处的位置，失败返回 -1 。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 733)\
    _MAKE( 459, "删除项目", "DeleteString", DeleteString, "删除列表框指定位置处的项目。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 736)\
    _MAKE( 460, "清空", "clear", clear, "删除列表框中的所有项目。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 461, "选择", "SelItem", SelItem, "在所有项目中寻找首部包含指定文本的项目，如找到，则选中它，并返回该项目的位置索引，否则返回 -1 。本命令仅在单选列表框中使用，如果在多选列表框中使用，将返回 ", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 737)\
    _MAKE( 462, "取顶端可见项目", "GetTopIndex", GetTopIndex, "返回列表框中当前最顶端可见项目的索引。0 为项目一，1 为项目二，如此类推。失败返回 -1 。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 463, "置顶端可见项目", "SetTopIndex", SetTopIndex, "设置列表框中当前最顶端的可见项目，必要时将自动滚动列表框。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 738)\
    _MAKE( 464, "取项目数", "GetCount", GetCount, "本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 465, "取项目数值", "GetItemData", GetItemData, "返回与指定项目相关联的数值。如果指定项目不存在，将返回 -1 。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 739)\
    _MAKE( 466, "置项目数值", "SetItemData", SetItemData, "设置与指定项目相关联的数值。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 740)\
    _MAKE( 467, "取项目文本", "GetItemText", GetItemText, "返回指定项目的文本。如果指定项目不存在，将返回空文本。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 742)\
    _MAKE( 468, "置项目文本", "SetItemtext", SetItemtext, "设置指定项目的文本。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 743)\
    _MAKE( 469, "加入项目", "AddString", AddString, "加入指定项目到列表框的尾部，成功返回加入后该项目所处的位置，失败返回 -1 。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 745)\
    _MAKE( 470, "插入项目", "InsertString", InsertString, "插入指定项目到列表框的指定位置处，成功返回插入后该项目所处的位置，失败返回 -1 。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 747)\
    _MAKE( 471, "删除项目", "DeleteString", DeleteString, "删除列表框指定位置处的项目。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 750)\
    _MAKE( 472, "清空", "clear", clear, "删除列表框中的所有项目。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 473, "选择", "SelItem", SelItem, "在所有项目中寻找首部包含指定文本的项目，如找到，则选中它，并返回该项目的位置索引，否则返回 -1。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 751)\
    _MAKE( 474, "是否被选中", "IsChecked", IsChecked, "如果与指定项目对应的选择框被选中，则返回真，否则返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 752)\
    _MAKE( 475, "选中项目", "SetCheck", SetCheck, "选中或取消选中与指定项目对应的选择框。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 753)\
    _MAKE( 476, "是否被允许", "IsEnabled", IsEnabled, "如果与指定项目对应的选择框被允许操作，则返回真，否则返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 755)\
    _MAKE( 477, "允许", "enable", enable, "允许或禁止对指定项目进行选择操作。成功返回真，失败返回假。本命令为初级对象成员命令。 成员属性“边框”所在数据类型为“选择列表框”，英文名称为“border”，", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 756)\
    _MAKE( 478, "取子夹数目", "GetCount", GetCount, "本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 479, "取子夹名称", "GetName", GetName, "本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 758)\
    _MAKE( 480, "置子夹名称", "SetName", SetName, "置入成功返回真，否则返回假。本命令为初级对象成员命令。 成员属性“表头方向”所在数据类型为“选择夹”，英文名称为“HeaderWay”，类型为“整数型（int）", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 759)\
    _MAKE( 481, "跳转", "goto", goto_, "跳转到指定的邮件或 Internet 地址。本命令为初级对象成员命令。 成员属性“标题”所在数据类型为“超级链接框”，英文名称为“caption”，类型为“文本", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 482, "打开", "open", open, "打开当前类型的对话框，返回一个逻辑值。对于类型为“打开文件”、“保存文件”、“字体选择”的对话框，如果为真，表示用户已通过该对话框输入了有效数据，否则表示用户取", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 483, "取设备句柄", "GetHDC", GetHDC, "如当前用户程序正在进行打印作业，则返回对应的打印机设备句柄（即HDC），否则返回 0。本命令为高级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 484, "开始打印", "StartDoc", StartDoc, "返回真表示已经成功地进入了打印作业，进入打印作业后文本写出位置将被重置为 0 。如果执行本命令时已经在打印作业中或者用户在打印设置对话框中选择了取消，返回假。进", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_krnln_global_var + 761)\
    _MAKE( 485, "结束打印", "EndDoc", EndDoc, "如果当前已经进入了打印作业，调用本命令可完成此作业。结束打印后易程序进行的后续打印绘画操作都将被忽略。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 486, "取消打印", "AbortDoc", AbortDoc, "如果当前已经进入了打印作业，调用本命令可立即中止且取消该作业。如果操作系统的打印管理器正在处理该打印作业（打印管理器正在运行并且允许后台打印），那么该作业将被删", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 487, "换页", "NewPage", NewPage, "调用本命令使打印机完成当前页的打印，文本写出位置被重置为 0 ，并走纸到下一页左上角。如果换页成功返回真，否则将自动取消打印并返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 488, "开始下一份", "NewCopy", NewCopy, "调用本命令后将结束在当前页上的打印作业，并将当前打印份数加一，当前页号置回到首页页号，以进入下一份拷贝的打印。如果成功返回真，失败将自动取消打印并返回假。本命令", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 489, "画点", "SetPixel", SetPixel, "本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 767)\
    _MAKE( 490, "画直线", "LineTo", LineTo, "使用画笔在画板上画出一条直线。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 770)\
    _MAKE( 491, "画椭圆", "ellipse", ellipse, "本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 774)\
    _MAKE( 492, "画弧线", "ArcTo", ArcTo, "使用画笔在画板上画出一条弧线。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 8, g_argumentInfo_krnln_global_var + 778)\
    _MAKE( 493, "画弦", "chord", chord, "本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 8, g_argumentInfo_krnln_global_var + 786)\
    _MAKE( 494, "画饼", "pie", pie, "本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 8, g_argumentInfo_krnln_global_var + 794)\
    _MAKE( 495, "画矩形", "DrawRect", DrawRect, "使用画笔在画板上画出一个矩形，矩形的内部使用刷子填充。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 802)\
    _MAKE( 496, "画渐变矩形", "DrawJBRect", DrawJBRect, "本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添加。", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_krnln_global_var + 806)\
    _MAKE( 497, "填充矩形", "FillRect", FillRect, "将画板上指定的矩形区域用当前刷子填充。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 813)\
    _MAKE( 498, "画圆角矩形", "RoundRect", RoundRect, "使用画笔在画板上画出一个圆角矩形，圆角矩形的内部使用刷子填充。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_krnln_global_var + 817)\
    _MAKE( 499, "画多边形", "polygon", polygon, "本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 823)\
    _MAKE( 500, "置写出位置", "SetWritePos", SetWritePos, "设置下次使用“写文本行”或“写出”命令输出数据时的位置。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 825)\
    _MAKE( 501, "写文本行", "print", print, "在当前写出位置写出指定的文本、数值、逻辑值或日期时间，并将现行写出位置调整到下行行首。支持自动换页。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 827)\
    _MAKE( 502, "写出", "write", write, "在当前写出位置处写出指定的文本、数值、逻辑值或日期时间。自动调整现行写出位置到所写出数据的末位置。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 828)\
    _MAKE( 503, "定位写出", "say", say, "在指定写出位置处写出指定的文本、数值、逻辑值或日期时间，不改变现行写出位置。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添加。", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 829)\
    _MAKE( 504, "取宽度", "GetWidth", GetWidth, "返回指定数据的写出宽度，使用当前绘画单位。如果调用本命令时尚未进入打印，将返回 0 。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 832)\
    _MAKE( 505, "取高度", "GetHeight", GetHeight, "返回指定数据的写出高度，使用当前绘画单位。如果调用本命令时尚未进入打印，将返回 0 。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 833)\
    _MAKE( 506, "画图片", "DrawPic", DrawPic, "本命令为初级对象成员命令。 1、#拷贝； 2、#翻转拷贝； 3、#位异或； 4、#位或； 5、#位与 本参数也可以指定透明色，但必须是负颜色数值。如： 画板1.", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_krnln_global_var + 834)\
    _MAKE( 507, "取图片宽度", "GetPicWidth", GetPicWidth, "返回指定图片的宽度。如果调用本命令时尚未进入打印，使用像素点为单位，否则使用当前绘画单位。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 841)\
    _MAKE( 508, "取图片高度", "GetPicHeight", GetPicHeight, "返回指定图片的高度。如果调用本命令时尚未进入打印，使用像素点为单位，否则使用当前绘画单位。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 842)\
    _MAKE( 509, "置自定义纸张类型", "SetCustomPaperType", SetCustomPaperType, "添加或修改自定义纸张类型。本方法仅在Windows NT/2000/XP下有效，且要求具有打印机驱动的完全控制权限。注意：太小或太大的纸张大小和页边距，可能不被", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_krnln_global_var + 843)\
    _MAKE( 510, "删除自定义纸张类型", "DeleteCustomPaperType", DeleteCustomPaperType, "删除指定自定义纸张类型。本方法仅在Windows NT/2000/XP下有效，且要求具有打印机驱动的完全控制权限。本命令为高级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 850)\
    _MAKE( 511, "取自定义纸张大小", "GetCustomPaperType", GetCustomPaperType, "取指定自定义纸张类型的纸张大小及边距，并写入后面的六个参数中。如果指定的自定义纸张类型不存在，返回“假”。本方法仅在Windows NT/2000/XP下有效，", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_krnln_global_var + 851)\
    _MAKE( 512, "取所有纸张类型", "SetCustomPaperType", SetCustomPaperType, "取当前默认打印机所支持的所有纸张类型（包括系统定义纸张类型和自定义纸张类型），返回一维文本数组，其中存放了的各纸张类型的名称。本方法仅在Windows NT/2", 22, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 513, "单位转换", "UnitCnv", UnitCnv, "将像素单位座标值转换到当前绘画单位，或将当前绘画单位座标值转换到像素单位。注意在执行本方法之前，必须已经开始打印。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 858)\
    _MAKE( 514, "取设备名称", "GetPrinterDeviceName", GetPrinterDeviceName, "如果已经开始打印，则返回当前打印机设备的名称。本命令为高级对象成员命令。 成员属性“绘画单位”所在数据类型为“打印机”，英文名称为“unit”，类型为“整数型（", 22, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 515, "发送数据", "send", send, "发送数据到指定主机上的指定端口。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 860)\
    _MAKE( 516, "取回数据", "recv", recv, "取回所接收到的数据。本命令必须在“数据到达”事件的处理子程序中使用。本命令为初级对象成员命令。 成员属性“端口”所在数据类型为“数据报”，英文名称为“port”", 22, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 517, "连接", "connect", connect, "连接到指定主机上的指定端口，该主机上的该端口必须已经被某一服务器组件监听。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 863)\
    _MAKE( 518, "断开连接", "CloseConnect", CloseConnect, "断开与服务器的已有连接。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 519, "发送数据", "send", send, "在成功建立与服务器的连接后，发送数据到服务器端组件。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 865)\
    _MAKE( 520, "取回数据", "recv", recv, "取回所接收到的数据。本命令必须在“数据到达”事件的处理子程序中使用。本命令为初级对象成员命令。 事件名称：数据到达 当服务器端将数据发送过来后，会产生本事件。在", 22, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 521, "取回数据", "recv", recv, "取回所接收到的数据。本命令必须在“数据到达”事件的处理子程序中使用。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 522, "取回客户", "GetClient", GetClient, "当接收到“客户进入”、“客户离开”或“数据到达”事件时，在该事件的处理子程序中可调用本方法取回对应的客户地址（IP地址 + 端口）。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 523, "发送数据", "send", send, "向指定已经连接进来的客户发送数据。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 866)\
    _MAKE( 524, "断开客户", "CloseClient", CloseClient, "断开与指定客户之间的连接。本命令为初级对象成员命令。 成员属性“端口”所在数据类型为“服务器”，英文名称为“port”，类型为“整数型（int）”。 指定监听数", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 869)\
    _MAKE( 525, "启动", "start", start, "启动或重新启动对指定端口的操作。在对端口进行操作之前必须首先启动，如在端口启动后又更改了端口属性必须重新启动。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 526, "停止", "stop", stop, "关闭已经启动的指定端口。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 527, "发送数据", "send", send, "从端口发送指定的数据。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 870)\
    _MAKE( 528, "信号操作", "Signal", Signal, "本命令可以设置或清除通讯端口上指定信号的状态。成功返回真，失败返回假。本命令为初级对象成员命令。 成员属性“端口号”所在数据类型为“端口”，英文名称为“port", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 871)\
    _MAKE( 529, "置光标", "SetCaret", SetCaret, "本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 873)\
    _MAKE( 530, "选择", "Select", Select, "本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 875)\
    _MAKE( 531, "全部选择", "SelectAll", SelectAll, "本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 532, "取光标行号", "CaretRow", CaretRow, "本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 533, "取光标列号", "CaretCol", CaretCol, "本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 534, "取选择行数", "SelectRows", SelectRows, "被选择的行区域起始于光标所在行。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 535, "取选择列数", "SelectCols", SelectCols, "被选择的列区域起始于光标所在列。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 536, "等宽缩放", "InWin", InWin, "将表格内容缩放到与表格窗口等宽显示。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 537, "全部复制", "CopyAll", CopyAll, "复制所有表格数据到剪贴板。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 538, "复制", "Copy", Copy, "复制指定区域表格数据到剪贴板。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 879)\
    _MAKE( 539, "粘贴到光标处", "PasteToCaret", PasteToCaret, "粘贴剪贴板中的表格数据到当前光标处。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 540, "粘贴", "Paste", Paste, "粘贴剪贴板中的表格数据到指定位置。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 883)\
    _MAKE( 541, "打印", "Print", Print, "打印表格数据到打印机。注意欲设置打印份数等打印设置信息请到与表格相连接的数据源中设置。成功返回空文本，失败(不包含操作者中断打印)返回非空错误文本。本命令为初级", 22, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 885)\
    _MAKE( 542, "打印预览", "PrintPreview", PrintPreview, "本命令为初级对象成员命令。 成员属性“数据源”所在数据类型为“表格”，英文名称为“DataSource”，类型为“文本型（text）”。 指定提供数据的数据源组", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 543, "到首记录", "GoTop", GoTop, "将数据源中当前记录指针移动到第一条记录上。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 544, "到尾记录", "GoBottom", GoBottom, "将数据源中当前记录指针移动到最后一条记录上。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 545, "跳过", "skip", skip, "将数据源中当前记录指针向前或者向后移动数条记录。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 887)\
    _MAKE( 546, "跳到", "goto", goto_, "改变数据源中的当前记录指针到指定的记录号。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 888)\
    _MAKE( 547, "取记录号", "RecNO", RecNO, "返回数据源当前记录指针所指向记录的记录号。记录号从1开始。如果当前记录指针无效，所返回记录号为0。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 548, "取表头行数", "GetFixedRowCount", GetFixedRowCount, "返回当以表格的形式表现数据源中的数据时表头所占的行数。表头行在表格中显示时不会滚动，在打印时会自动打印在每一页的顶部。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 549, "置表头行数", "SetFixedRowCount", SetFixedRowCount, "设置当以表格的形式表现数据源中的数据时表头所占的行数。表头行在表格中显示时不会滚动，在打印时会自动打印在每一页的顶部。注意如果数据源所使用的数据提供者不支持此特", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 889)\
    _MAKE( 550, "取表头列数", "GetFixedColCount", GetFixedColCount, "返回当以表格的形式表现数据源中的数据时表头所占的列数。表头列在表格中显示时不会滚动。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 551, "置表头列数", "SetFixedColCount", SetFixedColCount, "设置当以表格的形式表现数据源中的数据时表头所占的列数。表头列在表格中显示时不会滚动。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 890)\
    _MAKE( 552, "取行高", "GetRowHeight", GetRowHeight, "返回数据源中指定行在表现时的高度，单位为0.1毫米。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 891)\
    _MAKE( 553, "置行高", "SetRowHeight", SetRowHeight, "设置数据源中数据行在表现时的高度，单位为0.1毫米。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 892)\
    _MAKE( 554, "取列宽", "GetColWidth", GetColWidth, "返回数据源中指定列在表现时的宽度，单位为0.1毫米。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 895)\
    _MAKE( 555, "置列宽", "SetColWidth", SetColWidth, "设置数据源中数据列在表现时的宽度，单位为0.1毫米。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 896)\
    _MAKE( 556, "取类型", "GetType", GetType, "返回数据源中指定单元格的数据类型。返回值为以下常量值之一： 1、#文本； 2、#图片文件名； 3、#图片数据； 4、#字节集数据。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 899)\
    _MAKE( 557, "置类型", "SetType", SetType, "设置数据源中指定单元格的数据类型。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 901)\
    _MAKE( 558, "取文本色", "GetTextColor", GetTextColor, "返回数据源中指定单元格在表现时的文本颜色。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 906)\
    _MAKE( 559, "置文本色", "SetTextColor", SetTextColor, "设置数据源中指定单元格在表现时的文本颜色。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 908)\
    _MAKE( 560, "取背景色", "GetBackground", GetBackground, "返回数据源中指定单元格在表现时的背景颜色。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 913)\
    _MAKE( 561, "置背景色", "SetBackground", SetBackground, "设置数据源中指定单元格在表现时的背景颜色。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 915)\
    _MAKE( 562, "取字体名", "GetFontName", GetFontName, "返回数据源中指定单元格在表现时所使用字体的名称。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 920)\
    _MAKE( 563, "置字体名", "SetFontName", SetFontName, "设置数据源中指定单元格在表现时所使用字体的名称。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 922)\
    _MAKE( 564, "取字体尺寸", "GetFontSize", GetFontSize, "返回数据源中指定单元格在表现时所使用字体的尺寸，单位为0.1毫米。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 927)\
    _MAKE( 565, "置字体尺寸", "SetFontSize", SetFontSize, "设置数据源中指定单元格在表现时所使用字体的尺寸，单位为0.1毫米。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 929)\
    _MAKE( 566, "取字体属性", "GetFontAttr", GetFontAttr, "返回数据源中指定单元格在表现时所使用字体的属性。返回值为以下常量值之一或之和： 1、#粗体； 2、#斜体； 4、#下划线； 8、#删除线。本命令为初级对象成员命", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 934)\
    _MAKE( 567, "置字体属性", "SetFontAttr", SetFontAttr, "设置数据源中指定单元格在表现时所使用字体的属性。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 936)\
    _MAKE( 568, "取边距", "GetExtra", GetExtra, "返回数据源中指定单元格在表现其中数据时至其单元格边框之间的空白距离，单位为0.1毫米。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 941)\
    _MAKE( 569, "置边距", "SetExtra", SetExtra, "设置数据源中指定单元格在表现其中数据时至其单元格边框之间的空白距离，单位为0.1毫米。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 943)\
    _MAKE( 570, "取文本输入格式", "GetInputType", GetInputType, "返回数据源中某文本型单元格的输入格式。当新的数据通过输入方式更新到该单元格内之前，将首先自动根据此格式转换。返回值为以下常量值之一： 0、#通常型； 1、#字节", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 948)\
    _MAKE( 571, "置文本输入格式", "SetInputType", SetInputType, "设置数据源中某文本型单元格的输入格式。当新的数据通过输入方式更新到该单元格内之前，将首先自动根据此格式转换。注意如果数据源所使用的数据提供者不支持此特性，本命令", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 950)\
    _MAKE( 572, "取对齐方式", "GetAlignMode", GetAlignMode, "返回数据源中指定单元格在表现时所使用的对齐方式。返回值为以下常量值之一： 1、#上左； 2、#上中； 3、#上右； 4、#中左； 5、#中中； 6、#中右； 7", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 955)\
    _MAKE( 573, "置对齐方式", "SetAlignMode", SetAlignMode, "设置数据源中指定单元格在表现时所使用的对齐方式。注意如果数据源所使用的数据提供者不支持某些属性，该属性将被忽略。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 957)\
    _MAKE( 574, "取密码方式", "GetPwdMode", GetPwdMode, "返回数据源中指定单元格在表现时是否使用密码方式。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 962)\
    _MAKE( 575, "置密码方式", "SetPwdMode", SetPwdMode, "如设置值为真，则数据源中指定单元格在表现时以密码字符呈现。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 964)\
    _MAKE( 576, "取文本", "GetText", GetText, "返回数据源中指定单元格的文本内容。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 969)\
    _MAKE( 577, "置文本", "SetText", SetText, "设置数据源中指定单元格的文本内容，注意该单元格类型必须为“#文本”或“#图片文件名”。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 971)\
    _MAKE( 578, "取数据", "GetData", GetData, "返回数据源中指定单元格的图片或字节集数据内容。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 974)\
    _MAKE( 579, "置数据", "SetData", SetData, "设置数据源中指定单元格的图片或字节集数据内容，注意该单元格类型必须为“#图片数据”或“#字节集数据”。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 976)\
    _MAKE( 580, "合并", "Cmb", Cmb, "组合数据源中指定范围内的单元格，使之以一个单元格的形式表现。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 979)\
    _MAKE( 581, "分解", "Split", Split, "分解数据源中指定的已经组合的单元格，行列参数指向被组合单元格内的任何一个单元格即可。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 983)\
    _MAKE( 582, "是否被合并", "HasCmb", HasCmb, "如果数据源中指定单元格与周边指定单元格有组合关系，返回真，否则返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 985)\
    _MAKE( 583, "加线条", "AddLine", AddLine, "为数据源中指定范围内单元格添加线条。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 988)\
    _MAKE( 584, "删线条", "RemoveLine", RemoveLine, "将数据源中指定范围内单元格内的线条去除。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 993)\
    _MAKE( 585, "是否有线条", "HasLine", HasLine, "如果数据源中指定单元格有指定线条，返回真，否则返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 998)\
    _MAKE( 586, "清除", "EmptyCell", EmptyCell, "清除数据源中指定范围内单元格内容为空文本。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 1001)\
    _MAKE( 587, "置初始属性", "SetInitData", SetInitData, "设置在数据源中初始或者添加新单元格时默认的单元格初始属性。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 10, g_argumentInfo_krnln_global_var + 1005)\
    _MAKE( 588, "初始尺寸", "InitSize", InitSize, "初始数据源中数据的行列数，注意数据源中所有原有数据将被清除。注意某些数据提供者可能不支持此方法。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1015)\
    _MAKE( 589, "取行数", "GetRowCount", GetRowCount, "返回数据源中现行数据行数。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 590, "取列数", "GetColCount", GetColCount, "返回数据源中现行数据列数。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 591, "插入行", "InsertRow", InsertRow, "在数据源中指定位置处插入新数据行。注意某些数据提供者可能不支持在中间插入行。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1017)\
    _MAKE( 592, "添加行", "AppendRow", AppendRow, "在数据源中最后一行后插入新数据行。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1019)\
    _MAKE( 593, "删除行", "RemoveRow", RemoveRow, "在数据源中指定位置处删除数据行。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1020)\
    _MAKE( 594, "插入列", "InsertCol", InsertCol, "在数据源中指定位置处插入新数据列。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1022)\
    _MAKE( 595, "删除列", "RemoveCol", RemoveCol, "在数据源中指定位置处删除数据列。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1024)\
    _MAKE( 596, "添加", "Append", Append, "将指定其它数据源或者数据提供者中的数据添加到本数据源的尾部。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 1026)\
    _MAKE( 597, "存到字节集", "SaveDS", SaveDS, "将数据源中所有现有数据保存在字节集中返回。如果失败，将返回空字节集。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 598, "从字节集读", "LoadDS", LoadDS, "将指定字节集内的数据源数据写入到数据源中，数据源内所有原有数据将被清除。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1029)\
    _MAKE( 599, "存到文件", "SaveDSFile", SaveDSFile, "将数据源中所有现有数据保存到指定文件中。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1030)\
    _MAKE( 600, "从文件读", "LoadDSFile", LoadDSFile, "将指定文件内的数据源数据写入到数据源中，数据源内所有原有数据将被清除。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1031)\
    _MAKE( 601, "单元格到字节集", "SaveDSCell", SaveDSCell, "将数据源中指定范围内单元格数据保存在字节集中返回。如果失败，将返回空字节集。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 1032)\
    _MAKE( 602, "字节集到单元格", "LoadDSCell", LoadDSCell, "将指定字节集内的单元格数据写入到数据源中指定位置。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 1036)\
    _MAKE( 603, "单元格到文件", "SaveDSCellFile", SaveDSCellFile, "将数据源中指定范围内单元格数据保存到指定文件中。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 1040)\
    _MAKE( 604, "文件到单元格", "LoadDSCellFile", LoadDSCellFile, "将指定文件内的单元格数据写入到数据源中指定位置。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_krnln_global_var + 1045)\
    _MAKE( 605, "刷新", "Refrush", Refrush, "通知所有基于本数据源的数据处理者，使其自动从数据源重新获取并显示数据。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 606, "保存更改", "SaveChange", SaveChange, "通知所有基于本数据源的数据处理者，使其自动将处理或更改后的数据写回到数据源中。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 607, "打印设置", "SetupPrinter", SetupPrinter, "调用对话框设置数据的打印配置信息。当操作者按确认按钮退出对话框时返回真，否则返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 608, "取打印设置", "GetPrintInf", GetPrintInf, "返回打印数据源数据时所将使用的设置信息。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 609, "置打印设置", "SetPrintInf", SetPrintInf, "设置打印数据源数据时所将使用的设置信息。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1049)\
    _MAKE( 610, "取打印页宽", "GetPageWidth", GetPageWidth, "返回打印数据源数据时所将使用打印纸的正文区域宽度，单位为0.1毫米。注意：1、此宽度不包含页边距；2、“打印设置”命令执行后，如果操作者改变了纸张类型，此值将自", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 611, "取打印页高", "GetPageHeight", GetPageHeight, "返回打印数据源数据时所将使用打印纸的正文区域高度，单位为0.1毫米。注意：1、此宽度不包含页边距；2、“打印设置”命令执行后，如果操作者改变了纸张类型，此值将自", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 612, "取只读方式", "GetReadOnly", GetReadOnly, "返回数据源中指定单元格在表现时是否只读。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1050)\
    _MAKE( 613, "置只读方式", "SetReadOnly", SetReadOnly, "如设置值为真，则数据源中指定单元格在表现时只能读取而不能写入。注意： 1、如果数据处理者不支持此特性，则本属性无效； 2、如果数据提供者不支持此特性，本命令将被", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 1052)\
    _MAKE( 614, "打开", "open", open, "打开指定的ODBC数据源以供以后操作。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_krnln_global_var + 1057)\
    _MAKE( 615, "关闭", "close", close, "关闭当前被打开的数据库。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 616, "取连接文本", "GetConnect", GetConnect, "返回当前被打开数据库的ODBC数据源连接文本。失败返回空文本。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 617, "启动事务", "BeginTrans", BeginTrans, "启动当前被打开数据库的事务，成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 618, "回滚事务", "RollbackTrans", RollbackTrans, "回滚当前被打开数据库的事务，所有在上一次事务提交前对数据库所作的修改均被取消。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 619, "提交事务", "CommitTrans", CommitTrans, "提交当前被打开数据库的事务。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 620, "查询", "query", query, "对当前被打开数据库进行数据查询，成功返回结果记录集句柄，失败返回0。注意当不再使用此记录集时，必须使用“关闭记录集”命令将其关闭。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1060)\
    _MAKE( 621, "重新查询", "Requery", Requery, "对指定的记录集进行重新查询，并将其当前记录指针重定位到首记录。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1061)\
    _MAKE( 622, "关闭记录集", "CloseRecordset", CloseRecordset, "关闭指定的记录集。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1062)\
    _MAKE( 623, "首记录前", "bof", bof, "如果指定记录集的当前记录指针已在首记录的前面，返回真，否则返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1063)\
    _MAKE( 624, "尾记录后", "eof", eof, "如果指定记录集的当前记录指针已在尾记录的后面，返回真，否则返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1064)\
    _MAKE( 625, "到首记录", "GoTop", GoTop, "将指定记录集的当前记录指针移动到第一条记录上。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1065)\
    _MAKE( 626, "到尾记录", "GoBottom", GoBottom, "将指定记录集的当前记录指针移动到最后一条记录上。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1066)\
    _MAKE( 627, "到前一记录", "GoPrev", GoPrev, "将指定记录集的当前记录指针向前移动一条记录。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1067)\
    _MAKE( 628, "到后一记录", "GoNext", GoNext, "将指定记录集的当前记录指针向后移动一条记录。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1068)\
    _MAKE( 629, "读", "r", r, "返回指定记录集的当前记录处指定字段的数据内容，所返回数据的类型与字段SQL类型对照表如下： 返回数据类型   字段SQL类型 -----------------", 22, _CMD_OS(__OS_WIN), _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1069)\
    _MAKE( 630, "执行", "ExecuteSql", ExecuteSql, "执行指定的非查询类SQL语句，SQL语句中的列名（尤其是中文列名）可用中括号括住。如果SQL语句中涉及到备注或者字节集型字段，请在相应位置加上问号，然后再加上对", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1071)\
    _MAKE( 631, "打开MDB数据库", "OpenMDB", OpenMDB, "本命令是针对 ACCESS MDB 数据库的特定打开方法，通过自建 ODBC 连接文本来打开 MDB 数据库。调用本命令后无需再调用“打开”命令。成功返回真，失", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_krnln_global_var + 1073)\
    _MAKE( 632, "打开SQL数据库", "OpenSqlServerDB", OpenSqlServerDB, "本命令是针对 SQL SERVER 数据库的特定打开方法，通过自建 ODBC 连接文本来打开 SQL SERVER 数据库。调用本命令后无需再调用“打开”命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_krnln_global_var + 1078)\
    _MAKE( 633, "创建", "CreateObject", CreateObject, "创建指定类型的 COM 对象，本对象中的原有内容将被释放。成功返回真，否则返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1084)\
    _MAKE( 634, "获取", "GetObject", GetObject, "获取当前操作系统中已经存在的指定类型 COM 对象，本对象中的原有内容将被释放。成功返回真，否则返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1086)\
    _MAKE( 635, "清除", "Clear", Clear, "将本对象的内容释放并清空。如果不调用本方法，则对象在退出其作用区域时会自动被释放。例如：假设对象存在于子程序局部变量中，当子程序调用退出时，该对象会被自动释放。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 636, "是否为空", "IsEmpty", IsEmpty, "如果本对象的内容为空，返回真，否则返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 637, "是否相等", "IsEqual", IsEqual, "如果本对象的内容与指定对象的内容相等，返回真，否则返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1087)\
    _MAKE( 638, "读文本属性", "GetTextProperty", GetTextProperty, "读取并返回本对象的指定文本类型属性值，如果失败将返回空文本，且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参数可以被", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1088)\
    _MAKE( 639, "读数值属性", "GetNumProperty", GetNumProperty, "读取并返回本对象的指定数值类型属性值，如果失败将返回数值0，且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参数可以被", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1090)\
    _MAKE( 640, "读逻辑属性", "GetBoolProperty", GetBoolProperty, "读取并返回本对象的指定逻辑型属性值，如果失败将返回假，且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1092)\
    _MAKE( 641, "读日期属性", "GetDateProperty", GetDateProperty, "读取并返回本对象的指定日期时间型属性值，如果失败将返回100年1月1日，且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_DATE_TIME, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1094)\
    _MAKE( 642, "读对象型属性", "GetObjectProperty", GetObjectProperty, "读取并返回本对象的指定对象属性值，如果失败将返回内容为空的对象，且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参数可", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1096)\
    _MAKE( 643, "读属性", "GetProperty", GetProperty, "读取并返回本对象的指定属性值，本方法可以用作读取任意类型的属性。如果失败将返回类型值为空的变体型对象，且紧跟其后执行“取错误”方法将返回非空文本，否则将返回包含", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1098)\
    _MAKE( 644, "写属性", "SetProperty", SetProperty, "设置本对象指定属性的值，如果失败，返回假，并且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添加。", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1100)\
    _MAKE( 645, "方法", "RunMethod", RunMethod, "执行本对象的指定方法并忽略其返回值，如果失败，紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添加。", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1102)\
    _MAKE( 646, "通用方法", "RunVariantMethod", RunVariantMethod, "执行本对象的指定方法并返回一个变体型对象值，该对象内记录该方法的返回数据。本命令可以用作执行返回任何数据类型数据的方法，如果该方法没有返回数据，所返回变体型对象", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1104)\
    _MAKE( 647, "文本方法", "RunTextMethod", RunTextMethod, "执行本对象返回文本类型数据的方法，如果失败将返回空文本，且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1106)\
    _MAKE( 648, "数值方法", "RunNumMethod", RunNumMethod, "执行本对象返回数值类型数据的方法，如果失败将返回数值0，且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1108)\
    _MAKE( 649, "逻辑方法", "RunBoolMethod", RunBoolMethod, "执行本对象返回逻辑型数据的方法，如果失败将返回假，且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参数可以被重复添加。", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1110)\
    _MAKE( 650, "日期方法", "RunDateMethod", RunDateMethod, "执行本对象返回日期时间型数据的方法，如果失败将返回100年1月1日，且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_DATE_TIME, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1112)\
    _MAKE( 651, "对象型方法", "RunObjectMethod", RunObjectMethod, "执行本对象返回对象型数据的方法，如果失败将返回内容为空的对象，且紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。命令参数表中最后一个参数可以", 22, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1114)\
    _MAKE( 652, "创建图片对象", "CreatePicDispObj", CreatePicDispObj, "为指定图片数据创建对应的 COM 图片对象，本对象中的原有内容将被释放。成功返回真，否则返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1116)\
    _MAKE( 653, "创建字体对象", "CreateFontDispObj", CreateFontDispObj, "为指定字体数据创建对应的 COM 字体对象，本对象中的原有内容将被释放。成功返回真，否则返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1117)\
    _MAKE( 654, "取回图片", "GetPic", GetPic, "如果本对象为 COM 图片对象，则取回其具体图片数据。成功返回图片数据字节集，失败返回空字节集。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 655, "取回字体", "GetFont", GetFont, "如果本对象为 COM 字体对象，则取回其具体字体数据。如果失败，紧跟其后执行“取错误”方法将返回非空文本。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 656, "取错误", "GetErrorText", GetErrorText, "当读写对象属性、执行对象方法或取回字体时，紧跟该语句后执行本方法可以检查其是否执行成功。如果成功，本命令将返回空文本，如果失败，本命令将返回一个描述具体错误信息", 22, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 657, "查看", "ViewObjInf", ViewObjInf, "本命令仅在易程序的调试版本中被执行，在发布版本中将被直接跳过。 通过对话框的方式查看本对象的调用格式信息，便于编写相关程序。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 658, "取接口", "QueryInterface", QueryInterface, "获取指定对象或者OCX窗口组件中的指定接口，本对象中的原有内容将被释放。成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1118)\
    _MAKE( 659, "清除", "Clear", Clear, "将本对象的内容释放并清空。如果不调用本方法，则对象在退出其作用区域时会自动被释放。例如：假设对象存在于子程序局部变量中，当子程序调用退出时，该对象会被自动释放。", 22, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 660, "取类型", "GetType", GetType, "取回当前变体型对象的数据类型。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 661, "取数组成员数", "GetElementCount", GetElementCount, "如果本对象中存放的是数组数据，执行本方法将返回其成员数目，如果不是数组数据，执行本方法将返回 -1 。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, NULL)\
    _MAKE( 662, "取文本", "GetText", GetText, "返回本对象中或本对象数组成员中的文本数据，如果当前数据的数据类型不为文本型，将自动进行转换，如果转换失败将返回空文本。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1120)\
    _MAKE( 663, "取数值", "GetNum", GetNum, "返回本对象中或本对象数组成员中的数值数据，如果当前数据的数据类型不为数值或错误值型，将自动进行转换，如果转换失败将返回 0 。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1121)\
    _MAKE( 664, "取逻辑值", "GetBool", GetBool, "返回本对象中或本对象数组成员中的逻辑值数据，如果当前数据的数据类型不为逻辑型，将自动进行转换，如果转换失败将返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1122)\
    _MAKE( 665, "取日期", "GetDateTime", GetDateTime, "返回本对象中或本对象数组成员中的日期时间值数据，如果当前数据的数据类型不为日期时间型，将自动进行转换，如果转换失败将返回100年1月1日。本命令为初级对象成员命", 22, _CMD_OS(__OS_WIN), SDT_DATE_TIME, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1123)\
    _MAKE( 666, "取对象", "GetObject", GetObject, "返回本对象中或本对象数组成员中的COM对象型数据，如果当前数据的数据类型不为COM对象型，将返回空COM对象。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1124)\
    _MAKE( 667, "取变体型", "GetVariant", GetVariant, "返回本对象中或本对象数组成员中的变体型数据类型数据，如果当前数据的数据类型不为变体型，将自动进行转换。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1125)\
    _MAKE( 668, "赋值", "set", set, "设置本对象的内容，成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1126)\
    _MAKE( 669, "创建数组", "CreateArray", CreateArray, "创建指定类型和指定成员数的空白数组，对象中的原有内容将被清空，成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_krnln_global_var + 1128)\
    _MAKE( 670, "置类型", "SetType", SetType, "清除当前变体型对象中的原有内容，并改变它的数据类型，成功返回真，失败返回假。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1130)\
    _MAKE( 671, "取字节集", "GetBin", GetBin, "返回本对象中或本对象数组成员中的字节集数据，如果当前数据的数据类型不为字节集型，将自动进行转换，如果转换失败将返回空字节集。本命令为初级对象成员命令。", 22, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_krnln_global_var + 1131)
