#pragma once

#define __EWIZARD_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 ewizard_test_0_ewizard
#define EWIZARD_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__EWIZARD_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "ewizard_test_0_ewizard"
#define EWIZARD_NAME_STR(_index, _name) ______E_FNENAME(__EWIZARD_NAME(_index, _name))

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
#define EWIZARD_DEF(_MAKE) \
    _MAKE(  0, "复制程序", CopyProgram, "通过复制的方式建立指定程序项", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ewizard_global_var + 0)\
    _MAKE(  1, "复制程序段", CopyCode, "通过复制的方式在指定子程序中建立一段程序代码，请一定注意确保欲复制的代码块完整，不交叉跨越分枝及循环语句，否则将导致系统出错", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ewizard_global_var + 3)\
    _MAKE(  2, "删除程序", RemoveProgram, "删除程序中的指定程序项", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ewizard_global_var + 6)\
    _MAKE(  3, "删除程序段", RemoveCode, "删除指定子程序中的一段程序代码，必须确保待删除的代码块完整，不交叉跨越分枝及循环语句，否则将导致系统出错", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ewizard_global_var + 8)\
    _MAKE(  4, "删除标记程序", RemoveMarkedItem, "删除程序中所有注释内包含指定标记的各类程序项目（不包括程序代码段）", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ewizard_global_var + 10)\
    _MAKE(  5, "修改程序", ModiProgram, NULL, 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ewizard_global_var + 11)\
    _MAKE(  6, "置组件属性", SetCtrlProperty, NULL, 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ewizard_global_var + 15)\
    _MAKE(  7, "置语句备注", SetStatmentRemark, "设置子程序中指定语句的备注信息", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ewizard_global_var + 17)\
    _MAKE(  8, "置程序信息", SetProgramInfo, "设置程序整体的相关信息", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_ewizard_global_var + 20)\
    _MAKE(  9, "置作者信息", SetAuthorInfo, "设置程序与作者相关的信息", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 8, g_argumentInfo_ewizard_global_var + 25)\
    _MAKE( 10, "定义模板变量", SetTemplateVar, "设置模板变量的值，模板变量可以用作自动程序替换。可以在模板易语言程序中使用模板变量与模板变量条件表达式，具体使用方法为：\r\n\r\n一、在子程序代码中使用模板变量条件表达式：\r\n    在相应程序语句备注中加上以下指令以定义生成条件（符合该条件此段程序块将被包含到生成后的易语言程序中，否则会被自动抛弃）：\r\n    1、$如果(条件) 或 $if(条件)，后者为前者的英文方式。\r\n       条件可以定义多个，只要有一个条件为真整体即为真。如：“$如果(条件1)$如果(条件2)”\r\n    2、$否则(条件) 或 $else(条件)\r\n    3、$否则 或 $else\r\n    4、$结束 或 $end\r\n  其中，“条件”参数可以使用以下表达式之一：\r\n    1、模板变量                如果指定模板变量已被定义则为真，否则为假。\r\n    2、!模板变量               如果指定模板变量未被定义则为真，否则为假。\r\n    3、模板变量 = \"xxx\"        如果指定模板变量的内容等于指定文本内容则为真，否则为假，其中“xxx”为任意文本。\r\n    4、模板变量 = xxx          如果指定模板变量的内容等于指定数值则为真，否则为假，其中“xxx”为任意数值。\r\n    5、模板变量1 = 模板变量2   如果模板变量1的内容等于模板变量2则为真，否则为假。\r\n    6、模板变量 <> \"xxx\"       如果指定模板变量的内容不等于指定文本内容则为真，否则为假，其中“xxx”为任意文本。\r\n    7、模板变量 <> xxx          如果指定模板变量的内容不等于指定数值则为真，否则为假，其中“xxx”为任意数值。\r\n    8、模板变量1 <> 模板变量2  如果模板变量1的内容不等于模板变量2则为真，否则为假。\r\n  注意：必须确保模板变量条件语句代码块完整，不交叉跨越分枝及循环语句，否则将导致系统出错。另外不要将模板变量条件表达式设置在分枝和循环代码块的首尾语句上，最好单独设置在一条空白语句上，该空白语句在生成后会被自动删除。\r\n  简例1，其中“使用数据源1”和“使用数据源2”为模板变量：\r\n    ※备注：$如果 (使用数据源1)\r\n    数据源1.刷新 ()\r\n    ※备注：$否则 (使用数据源2)\r\n    数据源2.刷新 ()\r\n    ※备注：$否则\r\n    数据源3.刷新 ()\r\n    ※备注：$结束\r\n  简例2，其中“欲使用数据源”为模板变量：\r\n    ※备注：$如果 (欲使用数据源 = \"1\")\r\n    数据源1.刷新 ()\r\n    ※备注：$否则 (欲使用数据源 = \"2\")\r\n    数据源2.刷新 ()\r\n    ※备注：$否则\r\n    数据源3.刷新 ()\r\n    ※备注：$结束\r\n\r\n二、在其它位置使用模板变量条件表达式：\r\n    在任何其它程序项目的备注中加入以下指令以定义生成条件（符合该条件此程序项将被包含到生成后的易语言程序中，否则会被自动抛弃）：\r\n    $如果(条件) 或 $if(条件)，后者为前者的英文方式，有关此指令的具体解释见前。\r\n  简例，在模板易语言程序的“数据源1”窗口组件的备注中加入“$如果 (使用数据源1)”指令即可达到只有定义了“使用数据源1”模板变量，在最终生成的易语言程序中才包含此组件的目的：\r\n    条件可以定义多个，只要有一个条件为真整体即为真。如：“$如果(条件1)$如果(条件2)”\r\n\r\n三、在子程序代码中使用模板变量替换参数：\r\n  在程序语句中使用如下格式的参数会在生成易语言程序时自动被替换：\r\n    \"$(模板变量名)\"\r\n  简例，如果“变量1”模板变量的内容为文本型数据“单位1”，则下述语句\r\n    程序变量 = \"$(变量1)\"\r\n  在最终程序生成后将变为：\r\n    程序变量 = \"单位1\"\r\n  而当“变量1”模板变量的内容为数值型数据123时，则此语句将被替换为：\r\n    程序变量 = 123\r\n", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ewizard_global_var + 33)\
    _MAKE( 11, "删除模板变量", RemoveTemplateVar, "删除先前所定义的模板变量", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ewizard_global_var + 33)\
    _MAKE( 12, "清除修改记录", ClearModiRec, "清除先前所有修改或设置命令所记录下来的模板程序修改记录", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ewizard_global_var + 0)\
    _MAKE( 13, "写出程序", WritePrg, "根据已有的模板程序修改记录对模板程序进行实际修改。如果修改成功返回空文本，否则返回具体的错误信息。修改成功后的结果在向导程序退出后将被易系统自动载入编辑。无论写出成功或者失败，本命令退出时都会自动清除所有模板程序修改信息", 1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ewizard_global_var + 35)\
    _MAKE( 14, "添加模块引用", AddEComRef, "增加对指定易模块文件的引用到程序中", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ewizard_global_var + 37)\
    _MAKE( 15, "删除模块引用", RemoveEComRef, "删除程序中对指定易模块文件的引用", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ewizard_global_var + 38)

