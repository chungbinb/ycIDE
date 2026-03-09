#pragma once

#define __SPEC_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 spec_test_0_spec
#define SPEC_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__SPEC_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "spec_test_0_spec"
#define SPEC_NAME_STR(_index, _name) ______E_FNENAME(__SPEC_NAME(_index, _name))

// 这个宏定义了所有的命令, 以后需要命令名数组, 声明命令等, 都可以使用这个宏
// 下面为_MAKE宏参数列表, 除了第一个成员外, 其他都是 CMD_INFO 这个结构的成员
//  0 _index            = 命令索引, 会根据这个索引生成命令名, 这个索引也能让你更直观的看到这个命令是在命令数组里的下标
//  1 _szName           = 命令的中文名, 需要加双引号, 这个会显示在易语言支持库命令列表里
//  2 _szEgName         = 命令的英文名, 不能加双引号, 这个会显示在易语言支持库命令列表英文名字里, 会根据这个英文名字生成c++实现功能函数名, 这个也是静态库导出的符号名
//  3 _szExplain        = 命令详细解释, 需要加双引号, 这个会显示在易语言支持库命令列表的详细解释里
//  4 _shtCategory      = 全局命令的所属类别, 从1开始, 对象成员命令的此值为-1
//  5 _wState           = 标记, CT_开头常量, _CMD_OS(__OS_WIN) 表示支持win系统, _CMD_OS(OS_ALL) 表示支持所有系统, 如果返回数组或者变长参数, 需要或上对应标志位
//  6 _dtRetValType     = 返回值类型, 使用前注意转换HIWORD为0的内部数据类型值到程序中使用的数据类型值
//  7 _wReserved        = 保留字段, 填0
//  8 _shtUserLevel     = 命令的用户学习难度级别, LVL_SIMPLE=初级命令, LVL_SECONDARY=中级命令, LVL_HIGH=高级命令
//  9 _shtBitmapIndex   = 指定图像索引, 从1开始, 0表示无, 显示在支持库列表里的图标
// 10 _shtBitmapCount   = 图像数目(用作动画)
// 11 _nArgCount        = 命令的参数数目
// 12 _pBeginArgInfo    = 参数起始地址
#define SPEC_DEF(_MAKE) \
    _MAKE(  0, "置入代码", MachineCode, "在编译后文件代码段中当前语句位置置入指定的机器指令码数据", 3, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 45, LVL_HIGH, 0, 0, 1, g_argumentInfo_spec_global_var + 0)\
    _MAKE(  1, "取变量地址", GetVarAddress, "返回指定变量的内存地址（在内联汇编代码中可以在执行本命令后立即检查EAX寄存器获得），如欲修改该地址处内容（尤其是文本型、字节集型或复合数据类型变量），请谨慎操作，否则很可能造成内存垃圾或程序崩溃", 3, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_spec_global_var + 1)\
    _MAKE(  2, "取子程序地址", GetSubAddress, "返回指定子程序的内存代码地址（在内联汇编代码中可以在执行本命令后立即检查EAX寄存器获得）", 3, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_spec_global_var + 2)\
    _MAKE(  3, "取变量数据地址", GetVarDataAddr, "取指定变量中数据的内存地址。返回值为0表示没有取到变量地址。对文本型或字节集型变量而言，如果其长度为0，将返回0；对数组变量而言，如果其成员数为0，也将返回0。对于文本型或字节集型变量，返回值为文本数据或字节集数据的地址。其它情况下，本命令与“取变量地址”具有相同的返回值。注意：一旦变量超出其作用域范围，或文本型/字节集型变量被重新赋值，或数组变量被重新赋值，先前取到的变量地址将变成无效地址，再对该地址进行读写操作很有可能导致程序崩溃。", 3, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_spec_global_var + 3)\
    _MAKE(  4, "调试输出", Trace, "类似于易语言核心库中的“输出调试文本()”命令，向输出面板输出一行调试文本。本命令可接受任意多个通用型(任意基本类型)参数及数组。与“输出调试文本()”不同，本命令的所有参数数据仅输出为一行文本（各参数之间以“ | ”间隔）——既保持了数据相关性，又有效利用了输出面板的横向宽度。另外，本命令对各种数据的输出格式都颇为友好：如字节集被输出为形如“字节集:n{101,121,117,121,97,110,...}”，其中n为字节集长度，{}之间为字节集数据；如数组被输出为形如“数组:m,n{...}”，其中“m,n”表示该数组为m行n列，{}之间为数组数据。", 2, _CMD_OS(__OS_WIN) | CT_DISABLED_IN_RELEASE | CT_ALLOW_APPEND_NEW_ARG, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_spec_global_var + 4)\
    _MAKE(  5, "验证", Verify, "类似于易语言核心库中的“检查()”命令，本命令在调试版中验证参数值（或其计算结果）是否为“零值”，如果是“零值”则易程序的执行将被暂停且警示（相当于设置了条件断点）。与“检查()”命令不同的是，本命令在运行版中仅仅忽略“检查”这个步骤，但不会放弃执行参数语句（这在参数为命令/方法调用或子语句的情况下尤为重要）；此外，本命令可以检查任意基本类型的参数，且可以同时检查多个参数。如果提供了多个参数，所有的参数都会被验证，但只要出现一个“零值”，程序就会被暂停且警示。“零值”定义：字节型/短整数型/整数型/长整数型值等于0，小数型/双精度小数型绝对值小于0.00001，逻辑型值为“假”，文本型/字节集型数据长度等于0，数组成员数等于0，日期时间值为“1899/12/30 00:00:00”，子程序指针的执行地址为0。注：目前暂不支持“子语句类型”的参数。", 2, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_spec_global_var + 5)\
    _MAKE(  6, "调用子程序", CallFunction, "以stdcall方式(即WINAPI或Pascal)调用指定的子程序（子程序指针，或子程序地址）并取回被调用子程序的返回值，支持任意多个参数，参数支持除字节集之外的其它所有基本数据类型。调用子程序之前，您需要明确地知道它的参数类型和参数个数，以及返回值类型。本命令返回“真”仅表示调用过程中没有出异常情况，并不表示被调用的子程序执行成功。", 4, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_HIGH, 0, 0, 4, g_argumentInfo_spec_global_var + 6)\
    _MAKE(  7, "延迟", Delay, "延迟一段时间，然后继续执行后面的程序代码。与易语言核心支持库中的“延时()”命令不同的是，本命令在等待期间允许用户进行其它操作（如点击按钮等）。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_spec_global_var + 10)\
    _MAKE(  8, "推迟调用子程序", DelayCallFunction, "本命令被执行时，在内部启动一个计时器（精度约为100毫秒），然后立刻返回（以便易语言继续执行后面的程序代码），当计时时间到时才调用参数所指定的子程序。如上所述，本命令不会阻塞易语言程序的运行。本命令的第四个参数“返回值”目前不予使用。请注意保证子程序调用时参数依然有效。被推迟调用的子程序是在另外的线程中调用的，请注意保证线程安全。", 4, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_HIGH, 0, 0, 5, g_argumentInfo_spec_global_var + 11)\
    _MAKE(  9, "申请内存", AllocMem, "向易语言运行时系统申请指定大小的内存区域。执行成功返回申请到的内存首地址，失败返回0。由本命令申请的内存必须通过“释放内存”命令释放。", 3, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_spec_global_var + 16)\
    _MAKE( 10, "释放内存", FreeMem, "释放由“申请内存”所获取的内存区域。内存区域被释放后，不允许再进行读写操作。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_spec_global_var + 18)\
    _MAKE( 11, "调用易系统功能", ESysFunction, "具体的功能号及其所对应的参数值和返回值，请参考易语言支持库开发手册。", 4, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 3, g_argumentInfo_spec_global_var + 19)\
    _MAKE( 12, "取文本", GetText, "本命令有多项功能：将任意基本数据类型的参数或数组参数转换为文本；文本中包含引号（\"、“、”）和换行符等情况下的简捷处理方案；提供类似于C/C++语言中sprintf()的格式化文本功能。\r\n    示例：取文本 (“据说，<a href=`www.eyuyan.com`>~%s~</a>诞生于%d年秋。”, “易语言”, 2000)\r\n    等效于：“据说，<a href=” ＋ #引号 ＋ “www.eyuyan.com” ＋ #引号 ＋ “>” ＋ #左引号 ＋ “易语言” ＋ #右引号 ＋ “</a>诞生于” ＋ 到文本 (2000) ＋ “年秋。”\r\n    都将生成文本：“据说，<a href=\"www.eyuyan.com\">“易语言”</a>诞生于2000年秋。”\r\n    但很明显，前者比后者简捷明了很多，且输入代码时更加方便直观。后者因为 #引号、#左引号、#右引号 等常量以及加号（“+”）的使用，导致代码分散且混乱，不易输入和阅读。", 4, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_TEXT, 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_spec_global_var + 22)\
    _MAKE( 13, "取文本_属性设置", GetText_Set, "本命令用于设置“取文本”命令所使用的各项属性值。对于这些属性，系统都有默认的初始值（详见本命令的参数说明），故一般情况下无需调用此命令。所有文本属性值均不允许超过10个字符（或5个汉字），否则将导致设置失败且返回“假”。通过本命令所设置的属性值将一直有效，直至程序结束或下一次调用本命令修改了此修改值。", 4, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 4, g_argumentInfo_spec_global_var + 24)\
    _MAKE( 14, "取文本_属性读取", GetText_Get, "本命令用于获取“取文本”命令所使用的各项属性设置，结果将存入相应的参数变量中。", 4, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_HIGH, 0, 0, 4, g_argumentInfo_spec_global_var + 28)\
    _MAKE( 15, "推迟调用子程序_高精度计时", DelayCallFunction_precisely, "本命令被执行时，在内部启动一个计时器（精度小于1毫秒），然后立刻返回（以便易语言继续执行后面的程序代码），当计时时间到时才调用参数所指定的子程序。如上所述，本命令不会阻塞易语言程序的运行。本命令的第四个参数“返回值”目前不予使用。请注意保证子程序调用时参数依然有效。被推迟调用的子程序是在另外的线程中调用的，请注意保证线程安全。相对于“推迟调用子程序”，本命令只是计时精确度更高（同时更耗费CPU），其他功能相同。", 4, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_HIGH, 0, 0, 5, g_argumentInfo_spec_global_var + 32)

