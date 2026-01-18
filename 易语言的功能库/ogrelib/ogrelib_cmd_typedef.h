#pragma once

#define __OGRELIB_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 ogrelib_test_0_ogrelib
#define OGRELIB_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__OGRELIB_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "ogrelib_test_0_ogrelib"
#define OGRELIB_NAME_STR(_index, _name) ______E_FNENAME(__OGRELIB_NAME(_index, _name))

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
#define OGRELIB_DEF(_MAKE) \
    _MAKE(  0, "构造函数", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(  1, "析构函数", destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(  2, "释放私有数据", FreePrivateData, "释放关联在本资源上的私有数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(  3, "置私有数据", SetPrivateData, "关联一块数据到本资源上。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 1)\
    _MAKE(  4, "取设备", GetDevice, "获得使用此资源的设备。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 4)\
    _MAKE(  5, "取级数", GetLevelCount, "当资源是一个多级别的纹理时，获取纹理级别的数量。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(  6, "取优先级", GetPriority, "获取本资源的优先级。受托管的资源才有优先级，如本资源不是受托管的，则返回0。优先级决定了哪个资源先被系统释放，优先级低的先被释放。受托管资源优先级默认为0。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(  7, "取私有数据", GetPrivateData, "将已关联到本资源的数据，复制到指定的字节集中。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 5)\
    _MAKE(  8, "取类型", GetType, "返回值为“资源类型”常量数据类型中的值。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(  9, "预加载", PreLoad, "预加载一个托管资源。调用此方法表示应用程序一会儿要使用此资源。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 10, "置优先级", SetPriority, "设置本资源的优先级。关于优先级请看“取优先级”方法说明。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 7)\
    _MAKE( 11, "构造函数", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 12, "析构函数", destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 13, "释放私有数据", FreePrivateData, "释放关联在本资源上的私有数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 14, "生成多纹理子级别", GenerateMipSubLevels, "生成多纹理子级别。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 15, "取过滤类型", GetAutoGenFilterType, "获取生成多纹理子级别用的过滤类型。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 16, "取设备", GetDevice, "获得使用此资源的设备。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 4)\
    _MAKE( 17, "取级数", GetLevelCount, "当资源是一个多级别的纹理时，获取纹理级别的数量。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 18, "取精细级别", GetLOD, "返回一个受托管资源设置的精细级别。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 19, "取优先级", GetPriority, "获取本资源的优先级。受托管的资源才有优先级，如本资源不是受托管的，则返回0。优先级决定了哪个资源先被系统释放，优先级低的先被释放。受托管资源优先级默认为0。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 20, "取私有数据", GetPrivateData, "将已关联到本资源的数据，复制到指定的字节集中。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 8)\
    _MAKE( 21, "取类型", GetType, "返回值为“资源类型”常量数据类型中的值。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 22, "预加载", PreLoad, "预加载一个托管资源。调用此方法表示应用程序一会儿要使用此资源。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 23, "置过滤类型", SetAutoGenFilterType, "设置生成多纹理子级别用的过滤类型。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 10)\
    _MAKE( 24, "置精细级别", SetLOD, "设置一个受托管资源的精细级别，精细级别表示多纹理级别应该加载到显存几个级别。还表示精细级别是多少。例如：纹理被建立为256*256,这时如果设置为0表示最大层是256*256,如果设置为1表示最大层是128*128。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 11)\
    _MAKE( 25, "置优先级", SetPriority, "设置本资源的优先级。关于优先级请看“取优先级”方法说明。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 7)\
    _MAKE( 26, "置私有数据", SetPrivateData, "关联一块数据到本资源上。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 12)\
    _MAKE( 27, "构造函数", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 28, "析构函数", destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 29, "增加脏区域", AddDirtyRect, "增加一块脏区域到资源中。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 15)\
    _MAKE( 30, "释放私有数据", FreePrivateData, "释放关联在本立体上的私有数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 31, "生成多纹理子级别", GenerateMipSubLevels, "生成多纹理子级别。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 32, "取过滤类型", GetAutoGenFilterType, "获取生成多纹理子级别用的过滤类型。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 33, "取立方体面页面", GetCubeMapSurface, "获取立方体面所对应的页面。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 17)\
    _MAKE( 34, "取设备", GetDevice, "获得使用此资源的设备。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 4)\
    _MAKE( 35, "取级数", GetLevelCount, "当资源是一个多级别的纹理时，获取纹理级别的数量。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 36, "取级别信息", GetLevelDesc, "获级指定级别的纹理信息。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 20)\
    _MAKE( 37, "取精细级别", GetLOD, "返回一个受托管资源设置的精细级别。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 38, "取优先级", GetPriority, "获取本资源的优先级。受托管的资源才有优先级，如本资源不是受托管的，则返回0。优先级决定了哪个资源先被系统释放，优先级低的先被释放。受托管资源优先级默认为0。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 39, "取私有数据", GetPrivateData, "将已关联到本资源的数据，复制到指定的字节集中。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 22)\
    _MAKE( 40, "取类型", GetType, "返回值为“资源类型”常量数据类型中的值。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 41, "预加载", PreLoad, "预加载一个托管资源。调用此方法表示应用程序一会儿要使用此资源。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 42, "置过滤类型", SetAutoGenFilterType, "设置生成多纹理子级别用的过滤类型。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 10)\
    _MAKE( 43, "置精细级别", SetLOD, "设置一个受托管资源的精细级别，精细级别表示多纹理级别应该加载到显存几个级别。还表示精细级别是多少。例如：纹理被建立为256*256,这时如果设置为0表示最大层是256*256,如果设置为1表示最大层是128*128。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 11)\
    _MAKE( 44, "置优先级", SetPriority, "设置本资源的优先级。关于优先级请看“取优先级”方法说明。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 7)\
    _MAKE( 45, "置私有数据", SetPrivateData, "关联一块数据到本资源上。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 24)\
    _MAKE( 46, "写入数据", WriteData, "在当前资源中写入一块数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ogrelib_global_var + 27)\
    _MAKE( 47, "读出数据", ReadData, "从当前资源中读出一块数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ogrelib_global_var + 31)\
    _MAKE( 48, "构造函数", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 49, "析构函数", destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 50, "释放私有数据", FreePrivateData, "释放关联在本资源上的私有数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 51, "取索引信息", GetDesc, "获级当前索引缓冲的信息。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 35)\
    _MAKE( 52, "取设备", GetDevice, "获得使用此资源的设备。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 4)\
    _MAKE( 53, "取优先级", GetPriority, "获取本资源的优先级。受托管的资源才有优先级，如本资源不是受托管的，则返回0。优先级决定了哪个资源先被系统释放，优先级低的先被释放。受托管资源优先级默认为0。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 54, "取私有数据", GetPrivateData, "将已关联到本资源的数据，复制到指定的字节集中。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 36)\
    _MAKE( 55, "取类型", GetType, "返回值为“资源类型”常量数据类型中的值。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 56, "预加载", PreLoad, "预加载一个托管资源。调用此方法表示应用程序一会儿要使用此资源。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 57, "置优先级", SetPriority, "设置本资源的优先级。关于优先级请看“取优先级”方法说明。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 7)\
    _MAKE( 58, "置私有数据", SetPrivateData, "关联一块数据到本资源上。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 38)\
    _MAKE( 59, "写入数据", WriteData, "在当前资源中写入一块数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 41)\
    _MAKE( 60, "读出数据", ReadData, "从当前资源中读出一块数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 44)\
    _MAKE( 61, "构造函数", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 62, "析构函数", destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 63, "增加脏区域", AddDirtyRect, "增加一块脏区域到资源中。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 47)\
    _MAKE( 64, "释放私有数据", FreePrivateData, "释放关联在本资源上的私有数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 65, "生成多纹理子级别", GenerateMipSubLevels, "生成多纹理子级别。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 66, "取过滤类型", GetAutoGenFilterType, "获取生成多纹理子级别用的过滤类型。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 67, "取设备", GetDevice, "获得使用此资源的设备。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 4)\
    _MAKE( 68, "取级数", GetLevelCount, "当资源是一个多级别的纹理时，获取纹理级别的数量。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 69, "取级别信息", GetLevelDesc, "获级指定级别的纹理信息。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 48)\
    _MAKE( 70, "取精细级别", GetLOD, "返回一个受托管资源设置的精细级别。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 71, "取优先级", GetPriority, "获取本资源的优先级。受托管的资源才有优先级，如本资源不是受托管的，则返回0。优先级决定了哪个资源先被系统释放，优先级低的先被释放。受托管资源优先级默认为0。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 72, "取私有数据", GetPrivateData, "将已关联到本资源的数据，复制到指定的字节集中。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 50)\
    _MAKE( 73, "取级别页面", GetSurfaceLevel, "获取指定级别所对应的页面。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 52)\
    _MAKE( 74, "取类型", GetType, "返回值为“资源类型”常量数据类型中的值。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 75, "写入数据", WriteData, "在当前资源中写入一块数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 54)\
    _MAKE( 76, "读出数据", ReadData, "从当前资源中读出一块数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 57)\
    _MAKE( 77, "构造函数", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 78, "析构函数", destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 79, "释放私有数据", FreePrivateData, "释放关联在本资源上的私有数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 80, "取缓冲信息", GetDesc, "获级当前顶点缓冲的信息。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 60)\
    _MAKE( 81, "取设备", GetDevice, "获得使用此资源的设备。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 4)\
    _MAKE( 82, "取优先级", GetPriority, "获取本资源的优先级。受托管的资源才有优先级，如本资源不是受托管的，则返回0。优先级决定了哪个资源先被系统释放，优先级低的先被释放。受托管资源优先级默认为0。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 83, "取私有数据", GetPrivateData, "将已关联到本资源的数据，复制到指定的字节集中。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 61)\
    _MAKE( 84, "取类型", GetType, "返回值为“资源类型”常量数据类型中的值。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 85, "预加载", PreLoad, "预加载一个托管资源。调用此方法表示应用程序一会儿要使用此资源。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 86, "置优先级", SetPriority, "设置本资源的优先级。关于优先级请看“取优先级”方法说明。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 7)\
    _MAKE( 87, "置私有数据", SetPrivateData, "关联一块数据到本资源上。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 63)\
    _MAKE( 88, "写入数据", WriteData, "在当前资源中写入一块数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 66)\
    _MAKE( 89, "读出数据", ReadData, "从当前资源中读出一块数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 68)\
    _MAKE( 90, "构造函数", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 91, "析构函数", destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 92, "释放私有数据", FreePrivateData, "释放关联在本立体上的私有数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE( 93, "取父容器", GetContainer, "获取这个立体的父容器。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 71)\
    _MAKE( 94, "取信息", GetDesc, "获级当前体积信息。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 72)\
    _MAKE( 95, "取设备", GetDevice, "获得使用此资源的设备。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 4)\
    _MAKE( 96, "取私有数据", GetPrivateData, "将已关联到本资源的数据，复制到指定的字节集中。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 73)\
    _MAKE( 97, "置私有数据", SetPrivateData, "关联一块数据到本资源上。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 75)\
    _MAKE( 98, "写入数据", WriteData, "在当前资源中写入一块数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 78)\
    _MAKE( 99, "读出数据", ReadData, "从当前资源中读出一块数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 80)\
    _MAKE(100, "构造函数", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(101, "析构函数", destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(102, "增加脏区域", AddDirtyBox, "增加一块脏区域到资源中。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 82)\
    _MAKE(103, "释放私有数据", FreePrivateData, "释放关联在本资源上的私有数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(104, "生成多纹理子级别", GenerateMipSubLevels, "生成多纹理子级别。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(105, "取过滤类型", GetAutoGenFilterType, "获取生成多纹理子级别用的过滤类型。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(106, "取设备", GetDevice, "获得使用此资源的设备。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 4)\
    _MAKE(107, "取级数", GetLevelCount, "当资源是一个多级别的纹理时，获取纹理级别的数量。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(108, "取级别信息", GetLevelDesc, "获级指定级别的纹理信息。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 83)\
    _MAKE(109, "取精细级别", GetLOD, "返回一个受托管资源设置的精细级别。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(110, "取优先级", GetPriority, "获取本资源的优先级。受托管的资源才有优先级，如本资源不是受托管的，则返回0。优先级决定了哪个资源先被系统释放，优先级低的先被释放。受托管资源优先级默认为0。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(111, "取私有数据", GetPrivateData, "将已关联到本资源的数据，复制到指定的字节集中。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 85)\
    _MAKE(112, "取类型", GetType, "返回值为“资源类型”常量数据类型中的值。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(113, "取级别立体", GetVolumeLevel, "获取指定级别所对应的立体。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 87)\
    _MAKE(114, "预加载", PreLoad, "预加载一个托管资源。调用此方法表示应用程序一会儿要使用此资源。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(115, "置过滤类型", SetAutoGenFilterType, "设置生成多纹理子级别用的过滤类型。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 10)\
    _MAKE(116, "置精细级别", SetLOD, "设置一个受托管资源的精细级别，精细级别表示多纹理级别应该加载到显存几个级别。还表示精细级别是多少。例如：纹理被建立为256*256,这时如果设置为0表示最大层是256*256,如果设置为1表示最大层是128*128。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 11)\
    _MAKE(117, "置优先级", SetPriority, "设置本资源的优先级。关于优先级请看“取优先级”方法说明。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 7)\
    _MAKE(118, "置私有数据", SetPrivateData, "关联一块数据到本资源上。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 89)\
    _MAKE(119, "写入数据", WriteData, "在当前资源中写入一块数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 92)\
    _MAKE(120, "读出数据", ReadData, "从当前资源中读出一块数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 95)\
    _MAKE(121, "构造函数", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(122, "析构函数", destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(123, "取声明", GetDeclaration, "获取声明", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 98)\
    _MAKE(124, "取设备", GetDevice, "获取设备", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 4)\
    _MAKE(125, "hid1", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(126, "hid2", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(127, "hid3", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(128, "hid4", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(129, "hid5", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(130, "hid6", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(131, "hid7", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(132, "hid8", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(133, "hid9", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(134, "设置纹理", SetTexture, "为设备设置一层纹理。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 99)\
    _MAKE(135, "构造函数", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(136, "析构函数", destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(137, "取设备环境", GetDC, "取页面设备环境。成功返回设备环境，失败返回-1。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(138, "取页面信息", GetSurfaceInfo, "取页面的描述信息（包括页面宽度、高度、格式、内存分配类型等信息）。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 101)\
    _MAKE(139, "锁定页面", LockRect, "锁定页面上的一个矩形区域。", -1, _CMD_OS(__OS_WIN), MAKELONG(0x1E, 0), 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 103)\
    _MAKE(140, "页面解锁", UnlockRect, "当你锁定了一个页面，并且又使用完这个页面后，你必须调用这个方法解锁这个页面。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(141, "释放设备环境", ReleaseDC, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 105)\
    _MAKE(142, "构造函数", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(143, "析构函数", destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(144, "创建设备", CreateDevice, "创建设备对象，几乎所有的3D绘图功能都要通过它实现。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ogrelib_global_var + 106)\
    _MAKE(145, "设置灯光", SetLight, "在渲染场景中放置灯光,注必须调用“启用灯光”方法才能使这个灯光发生作用。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 110)\
    _MAKE(146, "启用灯光", Enablelight, "启用已经放置好的一个灯光。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 112)\
    _MAKE(147, "设置材质", SetMaterial, "设置以后渲染用的材质,渲染过一个物体之后可以再次调用此方法设置另外一种材质继续渲染下一个物体。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 114)\
    _MAKE(148, "清除后台缓冲", Clear, "用指定的颜色清除后台缓冲。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_ogrelib_global_var + 115)\
    _MAKE(149, "开始场景", BeginScene, "通知3D设备即将开始渲染。必须与结束场景命令配对使用，设备创建完毕，调用渲染命令之前必须调用本命令。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(150, "结束场景", EndScene, "通知3D设备结束渲染。必须与开始场景命令配对使用，即使渲染失败，你也应该在下一次调用开始场景之前调用本命令。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(151, "交换页面", Present, "将后台缓冲区中的内容显示到前台，并刷新窗口。渲染完毕后，请注意一定不要忘记使用本命令显示渲染结果，否则不会有任何输出。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 120)\
    _MAKE(152, "创建顶点缓冲", CreateVertexBuffer, "创建顶点缓冲。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_ogrelib_global_var + 123)\
    _MAKE(153, "渲染图元", DrawPrimitive, "渲染指定的顶点序列。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 128)\
    _MAKE(154, "渲染索引图元", DrawIndexedPrimitive, "渲染指定的索引顶点序列。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_ogrelib_global_var + 131)\
    _MAKE(155, "置源数据流", SetStreamSource, "使顶点缓冲区绑定一个设备数据流。渲染顶点缓冲中的顶点数据需要几个步骤。首先，你需要设置源数据流。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ogrelib_global_var + 137)\
    _MAKE(156, "置灵活顶点格式", SetFVF, "设置自定义的灵活顶点格式（FVF）。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 141)\
    _MAKE(157, "置渲染状态", SetRenderState, "设置设备的渲染状态。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 142)\
    _MAKE(158, "置变换矩阵", SetTransform, "设置设备的世界变换矩阵。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 144)\
    _MAKE(159, "置剪裁平面系数", SetClipPlane, "设置剪裁平面系数。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 146)\
    _MAKE(160, "置纹理调色板", SetCurrentTexturePalette, "设置当前的纹理调色板。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 148)\
    _MAKE(161, "置光标位置", SetCursorPosition, "设置光标位置。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 149)\
    _MAKE(162, "置光标属性", SetCursorProperties, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 152)\
    _MAKE(163, "置Z模板缓冲页面", SetDepthStencilSurface, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 155)\
    _MAKE(164, "置对话框模式", SetDialogBoxMode, "设置是否在全屏模式应用程序中使用GDI对话框。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 156)\
    _MAKE(165, "置索引缓冲", SetIndices, "设置索引缓冲数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 157)\
    _MAKE(166, "置曲面表面模式", SetNPatchMode, "开启或关闭曲面表面模式（N-patches）。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 158)\
    _MAKE(167, "置调色板单元", SetPaletteEntries, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 159)\
    _MAKE(168, "置渲染对象", SetRenderTarget, "给设备设置一个新的颜色缓冲页面。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 161)\
    _MAKE(169, "置取样器状态", SetSamplerState, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 163)\
    _MAKE(170, "置顶点处理模式", SetVertexProcessing, "设置顶点处理模式，可以设为软件顶点处理和硬件顶点处理两种模式。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 166)\
    _MAKE(171, "置纹理层状态", SetTextureStageState, "设置纹理层状态值", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 167)\
    _MAKE(172, "取设备特性", GetDeviceCaps, "取得渲染设备的技术特性。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 170)\
    _MAKE(173, "取后台缓冲页面", GetBackBuffer, "从设备交换链中取后台缓冲页面。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 171)\
    _MAKE(174, "取深度缓冲页面", GetDepthStencilSurface, "取得设备拥有的深度缓冲页面。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 174)\
    _MAKE(175, "取显示模式", GetDisplayMode, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 175)\
    _MAKE(176, "复制前台缓冲页面", GetFrontBufferData, "将设备的前台缓冲页面复制到程序指定的系统内存缓冲中。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 177)\
    _MAKE(177, "取灵活顶点格式", GetFVF, "取灵活顶点格式（FVF）。成功返回顶点格式，失败返回-1。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(178, "取灯光", GetLight, "获取由“设置灯光”方法所设置的灯光。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 179)\
    _MAKE(179, "取灯光状态", GetLightEnable, "取得灯光的开启状态。真为开启、假为关闭。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 181)\
    _MAKE(180, "取材质", GetMaterial, "获取当前材质。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 182)\
    _MAKE(181, "取渲染状态", GetRenderState, "取指定的渲染状态。成功返回状态值，失败返回-1。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 142)\
    _MAKE(182, "取渲染目标页面", GetRenderTarget, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 183)\
    _MAKE(183, "取顶点处理模式", GetVertexProcessing, "取顶点处理模式。软件顶点处理返回1，硬件顶点处理返回0。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(184, "取变换矩阵", GetTransform, "取设备的世界变换矩阵。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 185)\
    _MAKE(185, "取剪裁平面系数", GetClipPlane, "取剪裁平面系数。成功返回系数，失败返回-1。", -1, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 146)\
    _MAKE(186, "取视见区", GetViewport, "取剪裁平面系数。成功返回系数，失败返回-1。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 187)\
    _MAKE(187, "取纹理", GetTexture, "取设备指定纹理层的纹理。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 188)\
    _MAKE(188, "显示光标", ShowCursor, "显示或隐藏光标。返回真表示光标先前可见；假表示光标先前不可见。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 190)\
    _MAKE(189, "拉伸块复制", StretchRect, "将源页面指定区域的像素块拉伸后复制到目的页面的指定区域。该命令一般在改变视频流的纵横比之后使用。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_ogrelib_global_var + 191)\
    _MAKE(190, "测试协作等级状态", TestCooperativeLevel, "报告设备的当前协作等级状态。返回以下值：0、设备操作和程序调用可以继续；1、设备已丢失但这时不能重启，无法渲染；2、设备已丢失但这时能重启。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(191, "块复制", UpdateSurface, "将源页面指定区域的像素块复制到目的页面的指定区域。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_ogrelib_global_var + 196)\
    _MAKE(192, "创建新状态块", CreateStateBlock, "创建一个新的状态块，它包含全部的设备状态或是仅与顶点或像素处理相关的设备状态。如果该方法失败，很可能是显示模式改变了。为了从此类失败中恢复，应用程序应该重新创建表面，然后重新创建状态块。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 201)\
    _MAKE(193, "开始记录状态块", BeginStateBlock, "使系统开始把设备状态的改变记录到一个状态块中，而不是将它们（新的设备状态）应用于设备。可以记录以下状态：设备状态、渲染状态、光照和材质的参数、变换状态、纹理层状态和当前纹理信息。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(194, "结束记录状态块", EndStateBlock, "当应用程序完成对状态块的记录后，应该调用本方法通知系统以结束记录。如果该方法失败，很可能是因为显示模式改变了。只要重新创建表面并再次记录状态块就可以达到这个目的。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 203)\
    _MAKE(195, "填充颜色", ColorFill, "允许应用程序用指定的颜色填充一个页面的指定区域。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 204)\
    _MAKE(196, "创建附加交换链", CreateAdditionalSwapChain, "通过调用本命令，应用程序可以为设备创建附加的交换链。应用程序可以为每个视区创建一个交换链并将每个交换链与某个特定窗口相关联。应用程序在每个交换链的后缓存中渲染图像，然后分别显示它们。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 207)\
    _MAKE(197, "创建深度模板页面", CreateDepthStencilSurface, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_ogrelib_global_var + 209)\
    _MAKE(198, "创建索引缓冲", CreateIndexBuffer, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ogrelib_global_var + 216)\
    _MAKE(199, "创建离屏页面", CreateOffscreenPlainSurface, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_ogrelib_global_var + 220)\
    _MAKE(200, "创建渲染目标页面", CreateRenderTarget, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_ogrelib_global_var + 225)\
    _MAKE(201, "创建顶点着色器声明", CreateVertexDeclaration, "从设备和顶点元素创建顶点着色器声明。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 232)\
    _MAKE(202, "置顶点着色器声明", SetVertexDeclaration, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 234)\
    _MAKE(203, "取顶点着色器声明", GetVertexDeclaration, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 235)\
    _MAKE(204, "重置", Reset, "重置交换链的类型、尺寸和格式。本方法是当设备丢失时唯一有效的方法，并且是应用程序可用来把设备从丢失状态恢复到操作状态的唯一方法。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 236)\
    _MAKE(205, "验证纹理和渲染状态", ValidateDevice, "对照单次渲染用硬件验证纹理和渲染状态。这种方法一般在应用程序初始化时调用。返回值为完成想得到的渲染效果所需要的渲染次数，失败返回-1。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(206, "创建纹理", CreateTexture, "创建一个纹理资源。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_ogrelib_global_var + 237)\
    _MAKE(207, "创建立方体纹理", CreateCubeTexture, "创建一个立方体纹理资源。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_ogrelib_global_var + 244)\
    _MAKE(208, "创建立体纹理", CreateVolumeTexture, "创建一个立体纹理资源。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 8, g_argumentInfo_ogrelib_global_var + 250)\
    _MAKE(209, "修改变换矩阵公式", MultiplyTransform, "用指定的矩阵乘以设备的世界、视图或投影矩阵。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 258)\
    _MAKE(210, "创建顶点着色器", CreateVertexShader, "创建一个顶点着色器。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 260)\
    _MAKE(211, "置顶点着色器常量", SetVertexShaderConstantF, "常量数据类型为小数型数组。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 262)\
    _MAKE(212, "置顶点着色器常量1", SetVertexShaderConstantF, "常量数据类型为变换矩阵。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 265)\
    _MAKE(213, "置顶点着色器", SetVertexShader, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 268)\
    _MAKE(214, "取顶点着色器", GetVertexShader, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 269)\
    _MAKE(215, "创建像素着色器", CreatePixelShader, "创建一个像素着色器。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 270)\
    _MAKE(216, "置像素着色器常量", SetPixelShaderConstantF, "常量数据类型为小数型数组。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 272)\
    _MAKE(217, "置像素着色器常量1", SetPixelShaderConstantF, "常量数据类型为变换矩阵。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 275)\
    _MAKE(218, "置像素着色器", SetPixelShader, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 278)\
    _MAKE(219, "取像素着色器", GetPixelShader, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 279)\
    _MAKE(220, "取适配器数量", GetAdapterCount, "取系统显示适配器的数量。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(221, "取适配器模式列表", EnumAdapterModes, "取得可用的适配器模式列表。成功返回模式列表数组，失败返回空。", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, MAKELONG(0x22, 0), 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 280)\
    _MAKE(222, "取当前显示模式", GetAdapterDisplayMode, "取适配器当前显示模式。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 282)\
    _MAKE(223, "检测设备类型", CheckDeviceType, "检测指定适配器是否支持指定的设备类型。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_ogrelib_global_var + 284)\
    _MAKE(224, "检测设备页面格式", CheckDeviceFormat, "检测是否可在设备上使用指定的页面格式。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_ogrelib_global_var + 289)\
    _MAKE(225, "构造函数", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(226, "析构函数", destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(227, "取设备", GetDevice, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 295)\
    _MAKE(228, "构造函数", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(229, "析构函数", destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(230, "应用状态块", Apply, "应用状态块到当前设备状态。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(231, "捕获设备状态", Capture, "捕获当前设备的状态。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(232, "取设备", GetDevice, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 295)\
    _MAKE(233, "构造函数", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(234, "析构函数", destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(235, "取后台缓冲页面", GetBackBuffer, "从设备交换链中取得后台缓冲页面。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 296)\
    _MAKE(236, "取设备", GetDevice, "取与交换链相关联的设备。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 295)\
    _MAKE(237, "取显示模式", GetDisplayMode, "取显示模式信息。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 298)\
    _MAKE(238, "复制前台缓冲页面", GetFrontBufferData, "将交换链的前台缓冲页面复制到程序指定的系统内存缓冲中。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 299)\
    _MAKE(239, "取设备参数", GetPresentParameters, "取交换链相关的设备参数。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 300)\
    _MAKE(240, "交换页面", Present, "显示位于前台缓冲页面之后的那个后台缓冲页面。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 301)\
    _MAKE(241, "构造函数", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(242, "析构函数", destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(243, "取设备", GetDevice, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 295)\
    _MAKE(244, "纹理关联", TextureCombine, "将纹理的第几层和维数关联起来也就是说第几层是几维纹理。本命令的返回值可以与“灵活顶点格式”数据类型中的常量连用(位或)。返回-1表示失败原因是参数不正确。", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 304)\
    _MAKE(245, "校验立方体纹理", D3DXCheckCubeTextureRequirements, "校验立方体纹理参数。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_ogrelib_global_var + 306)\
    _MAKE(246, "校验纹理", D3DXCheckTextureRequirements, "校验纹理参数。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_ogrelib_global_var + 312)\
    _MAKE(247, "校验立方纹理", D3DXCheckVolumeTextureRequirements, "校验立方纹理参数。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 8, g_argumentInfo_ogrelib_global_var + 319)\
    _MAKE(248, "生成法线贴图", D3DXComputeNormalMap, "将高度贴图转换为法线贴图。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_ogrelib_global_var + 327)\
    _MAKE(249, "创建空立方体纹理", D3DXCreateCubeTexture, "创建一个空立方体纹理。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_ogrelib_global_var + 333)\
    _MAKE(250, "从文件创建立方体纹理", D3DXCreateCubeTextureFromFile, "从指定的文件创建立方体纹理。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 340)\
    _MAKE(251, "从文件创建立方体纹理扩展", D3DXCreateCubeTextureFromFileEx, "从指定的文件创建立方体纹理。可以指定一些扩展参数。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 13, g_argumentInfo_ogrelib_global_var + 343)\
    _MAKE(252, "从内存文件创建立方体纹理", D3DXCreateCubeTextureFromFileInMemory, "从内存文件创建立方体纹理。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 356)\
    _MAKE(253, "从内存文件创建立方体纹理扩展", D3DXCreateCubeTextureFromFileInMemoryEx, "从指定的内存文件创建立方体纹理扩展。可以指定一些扩展参数。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 13, g_argumentInfo_ogrelib_global_var + 359)\
    _MAKE(254, "创建空纹理", D3DXCreateTexture, "创建一个空纹理。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 8, g_argumentInfo_ogrelib_global_var + 372)\
    _MAKE(255, "从文件创建纹理", D3DXCreateTextureFromFile, "从指定的文件创建纹理。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 380)\
    _MAKE(256, "从文件创建纹理扩展", D3DXCreateTextureFromFileEx, "从指定的文件创建纹理。可以指定一些扩展参数。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 14, g_argumentInfo_ogrelib_global_var + 383)\
    _MAKE(257, "从内存文件创建纹理", D3DXCreateTextureFromFileInMemory, "从内存文件创建纹理。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 397)\
    _MAKE(258, "从内存文件创建纹理扩展", D3DXCreateTextureFromFileInMemoryEx, "从指定的内存文件创建纹理扩展。可以指定一些扩展参数。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 14, g_argumentInfo_ogrelib_global_var + 400)\
    _MAKE(259, "创建空立体纹理", D3DXCreateVolumeTexture, "创建一个空立体纹理。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 9, g_argumentInfo_ogrelib_global_var + 414)\
    _MAKE(260, "从文件创建立体纹理", D3DXCreateVolumeTextureFromFile, "从指定的文件创建立体纹理。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 423)\
    _MAKE(261, "从文件创建立体纹理扩展", D3DXCreateVolumeTextureFromFileEx, "从指定的文件创建立体纹理。可以指定一些扩展参数。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 15, g_argumentInfo_ogrelib_global_var + 426)\
    _MAKE(262, "从内存文件创建立体纹理", D3DXCreateVolumeTextureFromFileInMemory, "从内存文件创建立体纹理。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 441)\
    _MAKE(263, "从内存文件创建立体纹理扩展", D3DXCreateVolumeTextureFromFileInMemoryEx, "从指定的内存文件创建立体纹理扩展。可以指定一些扩展参数。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 15, g_argumentInfo_ogrelib_global_var + 444)\
    _MAKE(264, "D3DXFillCubeTexture", D3DXFillCubeTexture, NULL, 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(265, "D3DXFillCubeTextureTX", D3DXFillCubeTextureTX, NULL, 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(266, "D3DXFillTexture", D3DXFillTexture, NULL, 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(267, "D3DXFillTextureTX", D3DXFillTextureTX, NULL, 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(268, "D3DXFillVolumeTexture", D3DXFillVolumeTexture, NULL, 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(269, "D3DXFillVolumeTextureTX", D3DXFillVolumeTextureTX, NULL, 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(270, "D3DXFilterTexture", D3DXFilterTexture, NULL, 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(271, "从文件中取图像", D3DXGetImageInfoFromFile, "从指定的文件中获取图像信息。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 459)\
    _MAKE(272, "从资源中取图像", D3DXGetImageInfoFromFileInMemory, "从指定的资源(图片资源)中获取图像信息。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 461)\
    _MAKE(273, "D3DXGetImageInfoFromResource", D3DXGetImageInfoFromResource, NULL, 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(274, "从文件中加载页面", D3DXLoadSurfaceFromFile, "从指定的文件中加载页面。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 8, g_argumentInfo_ogrelib_global_var + 463)\
    _MAKE(275, "从内存中加载页面", D3DXLoadSurfaceFromFileInMemory, "从内存中加载页面。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 8, g_argumentInfo_ogrelib_global_var + 471)\
    _MAKE(276, "D3DXLoadSurfaceFromMemory", D3DXLoadSurfaceFromMemory, NULL, 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(277, "D3DXLoadSurfaceFromResource", D3DXLoadSurfaceFromResource, NULL, 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(278, "D3DXLoadSurfaceFromSurface", D3DXLoadSurfaceFromSurface, NULL, 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(279, "D3DXLoadVolumeFromFile", D3DXLoadVolumeFromFile, NULL, 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(280, "D3DXLoadVolumeFromFileInMemory", D3DXLoadVolumeFromFileInMemory, NULL, 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(281, "D3DXLoadVolumeFromMemory", D3DXLoadVolumeFromMemory, NULL, 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(282, "D3DXLoadVolumeFromResource", D3DXLoadVolumeFromResource, NULL, 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(283, "D3DXLoadVolumeFromVolume", D3DXLoadVolumeFromVolume, NULL, 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(284, "保存页面", D3DXSaveSurfaceToFile, "将页面保存为图像格式的文件。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ogrelib_global_var + 479)\
    _MAKE(285, "保存纹理", D3DXSaveTextureToFile, "将纹理保存为图像格式的文件。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 483)\
    _MAKE(286, "保存立体", D3DXSaveVolumeToFile, "将立体空间保存为图像格式的文件。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ogrelib_global_var + 486)\
    _MAKE(287, "合成颜色", TransColor, "将ARGB四分量合成单一的颜色值。", 3, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ogrelib_global_var + 490)\
    _MAKE(288, "浮点型到整型", FloatToInt, "将浮点型数值在内存中的形式转为整型表示，主要用于“设备”中的“置渲染状态”方法的参数“渲染状态值”要求是浮点型的。", 3, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 494)\
    _MAKE(289, "加上", D3DXVec3Add, "两个三维向量相加，返回结果向量", -1, _CMD_OS(__OS_WIN), MAKELONG(0x02, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 495)\
    _MAKE(290, "减去", D3DXVec3Subtract, "两个三维向量相减，返回结果向量", -1, _CMD_OS(__OS_WIN), MAKELONG(0x02, 0), 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 496)\
    _MAKE(291, "求叉乘", D3DXVec3Cross, "两个向量交叉乘，返回结果向量，结果向量垂直于两条叉乘的向量", -1, _CMD_OS(__OS_WIN), MAKELONG(0x02, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 495)\
    _MAKE(292, "求点乘", D3DXVec3Dot, "两个向量的点乘", -1, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 495)\
    _MAKE(293, "取向量长度", D3DXVec3Length, "取空间向量的长度", -1, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(294, "规格化向量", D3DXVec3Normalize, "对向量进行单位化处理，得到一个新的向量以方便使用", -1, _CMD_OS(__OS_WIN), MAKELONG(0x02, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 495)\
    _MAKE(295, "缩放向量", D3DXVec3Scale, "对向量进行按比例缩放", -1, _CMD_OS(__OS_WIN), MAKELONG(0x02, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 498)\
    _MAKE(296, "变换向量", D3DXVec3Transform, "将三维向量变成(x,y,z,1)格式和变换矩阵相乘，得到一个四维向量作为结果返回", -1, _CMD_OS(__OS_WIN), MAKELONG(0x03, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 499)\
    _MAKE(297, "变换坐标", D3DXVec3TransformCoord, "将三维向量格式的坐标值和变换矩阵相乘，得到变换后的坐标值", -1, _CMD_OS(__OS_WIN), MAKELONG(0x02, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 499)\
    _MAKE(298, "标准变换向量", D3DXVec3TransformNormal, "这个函数用矩阵变换向量标准形式 (x, y, z, 0)，不是齐次坐标形式", -1, _CMD_OS(__OS_WIN), MAKELONG(0x02, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 500)\
    _MAKE(299, "到屏幕空间", D3DXVec3Project, "把向量从对象空间投影到屏幕空间", -1, _CMD_OS(__OS_WIN), MAKELONG(0x02, 0), 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ogrelib_global_var + 501)\
    _MAKE(300, "到对象空间", D3DXVec3Unproject, "把向量从屏幕空间投影到对象空间", -1, _CMD_OS(__OS_WIN), MAKELONG(0x02, 0), 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ogrelib_global_var + 505)\
    _MAKE(301, "线形内插值", D3DXVec3Lerp, "操作两个三维向量的线性内插值，返回结果向量。向量线形插值的公式为“V1 + s(V2-V1)”", -1, _CMD_OS(__OS_WIN), MAKELONG(0x02, 0), 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 509)\
    _MAKE(302, "加上", D3DXVec4Add, "两个四维向量相加，返回结果向量", -1, _CMD_OS(__OS_WIN), MAKELONG(0x03, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 511)\
    _MAKE(303, "减去", D3DXVec4Subtract, "两个四维向量相减，返回结果向量", -1, _CMD_OS(__OS_WIN), MAKELONG(0x03, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 511)\
    _MAKE(304, "求叉乘", D3DXVec4Cross, "四维向量交叉乘，返回结果向量", -1, _CMD_OS(__OS_WIN), MAKELONG(0x03, 0), 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 512)\
    _MAKE(305, "求点乘", D3DXVec4Dot, "求四维向量的点乘", -1, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 511)\
    _MAKE(306, "取向量长度", D3DXVec4Length, "取四维向量的长度", -1, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(307, "规格化向量", D3DXVec4Normalize, "对四维向量进行单位化处理，得到一个新的向量以方便使用", -1, _CMD_OS(__OS_WIN), MAKELONG(0x03, 0), 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(308, "缩放向量", D3DXVec4Scale, "对四维向量进行按比例缩放", -1, _CMD_OS(__OS_WIN), MAKELONG(0x03, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 498)\
    _MAKE(309, "变换向量", D3DXVec4Transform, "将四维向量和变换矩阵相乘，得到一个四维向量作为结果返回", -1, _CMD_OS(__OS_WIN), MAKELONG(0x03, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 499)\
    _MAKE(310, "建仿射变换矩阵", D3DXMatrixAffineTransformation, "创建一个仿射变换变换矩阵。本函数是用下面的公式来计算一个仿射变换矩阵：Mout = Ms * (Mrc)-1 * Mr * Mrc * Mt，其中:Mout = 输出矩阵，Ms = 缩放矩阵，Mrc = 旋转矩阵中心，Mr = 旋转矩阵，Mt = 平移矩阵", -1, _CMD_OS(__OS_WIN), MAKELONG(0x04, 0), 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ogrelib_global_var + 514)\
    _MAKE(311, "求行列式值", D3DXMatrixDeterminant, "计算矩阵行列式值", -1, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(312, "建单位矩阵", D3DXMatrixIdentity, "创建一个单位矩阵", -1, _CMD_OS(__OS_WIN), MAKELONG(0x04, 0), 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(313, "求逆矩阵", D3DXMatrixInverse, NULL, -1, _CMD_OS(__OS_WIN), MAKELONG(0x04, 0), 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(314, "是否为单位矩阵", D3DXMatrixIsIdentity, "判断一个矩阵是否为单位矩阵", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(315, "建观察矩阵", D3DXMatrixLookAtLH, "创建一个观察矩阵", -1, _CMD_OS(__OS_WIN), MAKELONG(0x04, 0), 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 518)\
    _MAKE(316, "矩阵相乘", D3DXMatrixMultiply, "两个矩阵相乘", -1, _CMD_OS(__OS_WIN), MAKELONG(0x04, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 521)\
    _MAKE(317, "建正交投影矩阵", D3DXMatrixOrthoLH, "创建一个左手坐标系的正交投影矩阵", -1, _CMD_OS(__OS_WIN), MAKELONG(0x04, 0), 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ogrelib_global_var + 522)\
    _MAKE(318, "自定义正交投影矩阵", D3DXMatrixOrthoOffCenterLH, "创建一个用户定制的左手坐标系的正交投影矩阵", -1, _CMD_OS(__OS_WIN), MAKELONG(0x04, 0), 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_ogrelib_global_var + 526)\
    _MAKE(319, "建透视投影矩阵", D3DXMatrixPerspectiveFovLH, "创建一个左手坐标系的透视投影矩阵", -1, _CMD_OS(__OS_WIN), MAKELONG(0x04, 0), 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ogrelib_global_var + 532)\
    _MAKE(320, "建透视投影矩阵扩展", D3DXMatrixPerspectiveLH, "创建一个左手坐标系的透视投影矩阵", -1, _CMD_OS(__OS_WIN), MAKELONG(0x04, 0), 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ogrelib_global_var + 536)\
    _MAKE(321, "建平面反射矩阵", D3DXMatrixReflect, "从平面方程创建一个平面反射矩阵", -1, _CMD_OS(__OS_WIN), MAKELONG(0x04, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 540)\
    _MAKE(322, "建绕任意轴旋转矩阵", D3DXMatrixRotationAxis, "创建一个可以绕着任意轴旋转的旋转矩阵", -1, _CMD_OS(__OS_WIN), MAKELONG(0x04, 0), 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 541)\
    _MAKE(323, "建四元数旋转矩阵", D3DXMatrixRotationQuaternion, "从一个四元数创建一个旋转矩阵", -1, _CMD_OS(__OS_WIN), MAKELONG(0x04, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 541)\
    _MAKE(324, "建X轴旋转矩阵", D3DXMatrixRotationX, "创建绕着X轴旋转矩阵", -1, _CMD_OS(__OS_WIN), MAKELONG(0x04, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 543)\
    _MAKE(325, "建Y轴旋转矩阵", D3DXMatrixRotationY, "创建绕着Y轴旋转矩阵", -1, _CMD_OS(__OS_WIN), MAKELONG(0x04, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 544)\
    _MAKE(326, "建Z轴旋转矩阵", D3DXMatrixRotationZ, "创建绕着Z轴旋转矩阵", -1, _CMD_OS(__OS_WIN), MAKELONG(0x04, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 545)\
    _MAKE(327, "建缩放矩阵", D3DXMatrixScaling, "创建一个缩放矩阵", -1, _CMD_OS(__OS_WIN), MAKELONG(0x04, 0), 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 546)\
    _MAKE(328, "建平移矩阵", D3DXMatrixTranslation, "创建一个坐标平移矩阵", -1, _CMD_OS(__OS_WIN), MAKELONG(0x04, 0), 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 549)\
    _MAKE(329, "求转置矩阵", D3DXMatrixTranspose, "求一个矩阵的转置矩阵", -1, _CMD_OS(__OS_WIN), MAKELONG(0x04, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 521)\
    _MAKE(330, "创建平面", D3DXPlaneFromPointNormal, "根据一个点和一条平面的法线创建一个平面", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 552)\
    _MAKE(331, "创建平面扩展", D3DXPlaneFromPoints, "根据三个点创建一个平面", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ogrelib_global_var + 554)\
    _MAKE(332, "求点乘四维向量", D3DXPlaneDot, "计算平面方程与四维向量的点乘。如果平面方程系数是(a, b, c, d)和四维向量(x, y, z, w)，那么返回值就按下面的公式计算： a*x + b*y + c*z + d*w", -1, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 511)\
    _MAKE(333, "求点乘三维向量", D3DXPlaneDotCoord, "计算平面跟三维向量的点积，并且平面系数里的W为1如果平面方程系数是 (a, b, c, d)，并且三维向量是 (x, y, z)，那么返回值就用下面的公式计算： a*x + b*y + c*z + d*1", -1, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 495)\
    _MAKE(334, "求正规点乘三维向量", D3DXPlaneDotNormal, "计算平面与3-D向量的点积。并且平面系数里的W为0。如果平面方程系数是(a, b, c, d)，三维向量是 (x, y, z)，那么返回的结果就是用下面公式计算： a*x + b*y + c*z + d*0", -1, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 495)\
    _MAKE(335, "求直线平面交点", D3DXPlaneIntersectLine, "找出直线跟平面的交点。如果直线平行于平面，", -1, _CMD_OS(__OS_WIN), MAKELONG(0x02, 0), 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 557)\
    _MAKE(336, "清空模型", D3DXCleanMesh, "清空一个模型，为简化做准备。", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ogrelib_global_var + 559)\
    _MAKE(337, "计算模型边界盒", D3DXComputeBoundingBox, "计算模型边界盒。使用顶点数组作为输入计算边界盒，可以使用各种顶点格式。", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_ogrelib_global_var + 563)\
    _MAKE(338, "计算模型边界球", D3DXComputeBoundingSphere, "计算模型边界球。使用顶点数组作为输入计算边界球，可以使用各种顶点格式。", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_ogrelib_global_var + 568)\
    _MAKE(339, "计算顶点的法向量", D3DXComputeNormals, "有时.x文件不包含顶点的法向量，这时，如果使用光照，则需要手动计算顶点的法向量。", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 573)\
    _MAKE(340, "计算正切向量", D3DXComputeTangent, "为纹理阶段中给定的纹理坐标计算正切向量。", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_ogrelib_global_var + 575)\
    _MAKE(341, "模型子集到单一三角形带", D3DXConvertMeshSubsetToSingleStrip, "转换指定的模型子集到单一三角形带。", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_ogrelib_global_var + 581)\
    _MAKE(342, "模型子集到连续带", D3DXConvertMeshSubsetToStrips, "转换指定的模型子集到连续带。", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_ogrelib_global_var + 586)\
    _MAKE(343, "创建模型", D3DXCreateMesh, "创建一个空模型。", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_ogrelib_global_var + 593)\
    _MAKE(344, "使用灵活顶点格式创建模型", D3DXCreateMeshFVF, "使用灵活顶点格式创建一个空模型。", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_ogrelib_global_var + 599)\
    _MAKE(345, "是否相交", D3DXIntersect, "检测空间中的一条射线是否与指定的模型相交。如相交则返回相交信息数组，不相交返回空数组。", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 8, g_argumentInfo_ogrelib_global_var + 605)\
    _MAKE(346, "从文件中载入模型", D3DXLoadMeshFromX, "载入存储在模型文件（.x）中的模型数据。本命令先创建一个模型对象，然后从.x文件中读取模型的几何信息。", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_ogrelib_global_var + 613)\
    _MAKE(347, "", D3DXLoadMeshFromXof, NULL, 2, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 620)\
    _MAKE(348, "", D3DXLoadMeshFromXInMemory, NULL, 2, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 620)\
    _MAKE(349, "保存模型", D3DXSaveMeshToX, "保存模型到模型文件(.x)中。", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_ogrelib_global_var + 621)\
    _MAKE(350, "", D3DXSimplifyMesh, NULL, 2, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 620)\
    _MAKE(351, "", D3DXTessellateNPatches, NULL, 2, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 620)\
    _MAKE(352, "", D3DXValidMesh, NULL, 2, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 620)\
    _MAKE(353, "", D3DXWeldVertices, NULL, 2, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 620)\
    _MAKE(354, "构造函数", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(355, "析构函数", destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(356, "克隆模型", CloneMesh, "用定义来克隆一个模型。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ogrelib_global_var + 628)\
    _MAKE(357, "灵活顶点克隆模型", CloneMeshFVF, "用指定的灵活顶点克隆一个模型。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ogrelib_global_var + 632)\
    _MAKE(358, "ConvertAdjacencyToPointReps", ConvertAdjacencyToPointReps, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(359, "ConvertPointRepsToAdjacency", ConvertPointRepsToAdjacency, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(360, "画子模型", DrawSubset, "根据属性表标识符画本模型的子模型。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 636)\
    _MAKE(361, "GenerateAdjacency", GenerateAdjacency, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(362, "取属性表", GetAttributeTable, "获取属性表。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 637)\
    _MAKE(363, "取顶点定义", GetDeclaration, "获取模型的顶点定义。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 638)\
    _MAKE(364, "取设备", GetDevice, "取和此模型关联的设备。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 639)\
    _MAKE(365, "取灵活顶点", GetFVF, "获取此模型用的灵活顶点。返回值是“灵活顶点格式”常量数据类型的一个或多个。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(366, "取索引缓冲", GetIndexBuffer, "获取此模型用的索引缓冲。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 640)\
    _MAKE(367, "取每顶点字节数", GetNumBytesPerVertex, "获取模型每个顶点的字节数。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(368, "取面数", GetNumFaces, "获取模型所有面的个数。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(369, "取顶点数", GetNumVertices, "获取模型所有顶点的个数。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(370, "取选顶", GetOptions, "获取模型目前所用的选项。返回值为“模型创建标志”常量数据类型的一个或多个。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(371, "取顶点缓冲", GetVertexBuffer, "获取此模型用的顶点缓冲。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 641)\
    _MAKE(372, "写入属性数据", WriteAttributeBuffer, "将数据写入模型的属性。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 642)\
    _MAKE(373, "读属性数据", ReadAttributeBuffer, "从模型中读出属性数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 643)\
    _MAKE(374, "写入顶点数据", WriteVertexBuffer, "将数据写入模型的顶点缓冲。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 642)\
    _MAKE(375, "读顶点数据", ReadVertexBuffer, "从模型中读出顶点缓冲数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 645)\
    _MAKE(376, "写入索引数据", WriteIndexBuffer, "将数据写入模型的索引缓冲。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ogrelib_global_var + 642)\
    _MAKE(377, "读索引数据", ReadIndexBuffer, "从模型中读出索引缓冲数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ogrelib_global_var + 647)\
    _MAKE(378, "优化", Optimize, "对当前模型正行优化，生成别一个模型。", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(379, "OptimizeInplace", OptimizeInplace, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(380, "SetAttributeTable", SetAttributeTable, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)\
    _MAKE(381, "UpdateSemantics", UpdateSemantics, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ogrelib_global_var + 0)

