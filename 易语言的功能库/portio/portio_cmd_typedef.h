#pragma once

#define __PORTIO_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 portio_test_0_portio
#define PORTIO_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__PORTIO_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "portio_test_0_portio"
#define PORTIO_NAME_STR(_index, _name) ______E_FNENAME(__PORTIO_NAME(_index, _name))

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
#define PORTIO_DEF(_MAKE) \
    _MAKE(  0, "端口读入", PortIn, "从指定端口读入并返回一个字节数据，注意如果没有安装port95nt.exe，本命令将失败并始终返回0。请在运行本命令的目的机器上下载安装第三方免费软件 http://www.dywt.com.cn/port95nt.exe ", 1, _CMD_OS(__OS_WIN), SDT_BYTE, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_portio_global_var + 0)\
    _MAKE(  1, "端口写出", PortOut, "向指定端口写出一个字节数据，注意如果没有安装port95nt.exe，本命令将失败。请在运行本命令的目的机器上下载安装第三方免费软件 http://www.dywt.com.cn/port95nt.exe ", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_portio_global_var + 1)\
    _MAKE(  2, "查询并口", CheckParallelPort, "返回当前计算机上所有并口的配置情况。返回数据是一个整数数组，其中每三个成员描述一个并口的信息。第一个成员记录此并口的端口访问地址，第二个记录该并口的类型，为以下整数值之一： 0: 未定义;  1: SPP;  2: PS2;  3: EPP;  4: ECP 。如果并口类型为ECP，第三个成员记录具体的ECP模式，为以下整数值之一： 0: 未定义;  1: SPP;  2: PS2;  3: FastCentronics;  4: ECP;  5: EPP;  6: Test;  7: Configuration 。如果并口类型不为ECP，则第三个成员始终为0。注意如果没有安装port95nt.exe，本命令将失败并返回空整数数组。请在运行本命令的目的机器上下载安装第三方免费软件 http://www.dywt.com.cn/port95nt.exe ", 2, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_portio_global_var + 0)\
    _MAKE(  3, "置ECP模式", SetECPMode, "如果指定并口类型为ECP，则可以使用本命令设置其具体的ECP模式。注意如果没有安装port95nt.exe，本命令将失败。请在运行本命令的目的机器上下载安装第三方免费软件 http://www.dywt.com.cn/port95nt.exe ", 2, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_portio_global_var + 3)

