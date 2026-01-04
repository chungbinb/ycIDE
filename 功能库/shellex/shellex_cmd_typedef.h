#pragma once

#define __SHELLEX_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 shellex_test_0_shellex
#define SHELLEX_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__SHELLEX_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "shellex_test_0_shellex"
#define SHELLEX_NAME_STR(_index, _name) ______E_FNENAME(__SHELLEX_NAME(_index, _name))

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
#define SHELLEX_DEF(_MAKE) \
    _MAKE(  0, "添加提示", AddTooltip, "创建一个提示标签。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_shellex_global_var + 0)\
    _MAKE(  1, "删除提示", DelTooltip, "删除一个控件的提示标签。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_shellex_global_var + 4)\
    _MAKE(  2, "置提示文本", SetTooltipText, "设置指定控件的提示文本。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_shellex_global_var + 5)\
    _MAKE(  3, "置提示底色", SetTooltipBColor, "设置所有控件的提示标签的背景颜色。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_shellex_global_var + 7)\
    _MAKE(  4, "置提示文本色", SetTooltipTColor, "设置所有控件的提示标签的文本颜色。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_shellex_global_var + 8)\
    _MAKE(  5, "置提示时间", SetTooltipTime, "设置所有控件的提示标签的显示时间。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_shellex_global_var + 9)\
    _MAKE(  6, "置提示图标", SetTooltipIcon, "设置所有控件的提示标签的显示图标。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_shellex_global_var + 10)\
    _MAKE(  7, "置提示字体", SetTooltipFont, "设置所有控件的提示标签的字体。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_shellex_global_var + 12)\
    _MAKE(  8, "设置文件拖放", DragFiles, "将组件设置成拖放状态", 2, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_shellex_global_var + 14)\
    _MAKE(  9, "设置树型框拖放", DragTree, "将组件设置拖放", 2, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_shellex_global_var + 16)\
    _MAKE( 10, "注册热键", RegHotKey, "注册系统热键，返回一个热键标识，失败返回0。", 3, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_shellex_global_var + 19)\
    _MAKE( 11, "撤销热键", UnRegHotKey, "撤销所注册的系统热键", 3, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_shellex_global_var + 23)\
    _MAKE( 12, "取指针地址", GetAddr, "返回一个结构的指针地址。", 4, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_shellex_global_var + 25)\
    _MAKE( 13, "执行子程序", CallPtr, "执行一个易语言的子程序。", 4, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_shellex_global_var + 26)\
    _MAKE( 14, "置提示延迟", SetDelayDisplay, "设置提示框延迟显示的时间 也是就鼠标放上去多长时间显示 ", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_shellex_global_var + 28)

