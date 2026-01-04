#pragma once

#define __EIMGCONVERTER_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 eimgconverter_test_0_eimgconverter
#define EIMGCONVERTER_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__EIMGCONVERTER_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "eimgconverter_test_0_eimgconverter"
#define EIMGCONVERTER_NAME_STR(_index, _name) ______E_FNENAME(__EIMGCONVERTER_NAME(_index, _name))

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
#define EIMGCONVERTER_DEF(_MAKE) \
    _MAKE(  0, "转换到JPG", ConvertToJPG, "将本库支持读取的图像文件转换为JPEG文件，支持大部分JPEG压缩选项。成功返回 1 ，失败返回一个小于等于零的数值，具体为： 0: 未知错误； -1: 文件不存在； -2: 系统资源不足； -3: 该文件格式不被支持； -4: 编码时出错； -6: 编码时系统资源不足； -7: 编码参数错误。 目前可以读取如下源格式的图像并转换到JPEG格式：标准JPEG（全部格式）、BMP（目前仅支持24位BMP）、TIFF（单页图像所有格式）、PNG（32位及32位以下）、PCX（8位和24位，RLE压缩）、PCD（取第三幅）", 0, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_eimgconverter_global_var + 0)\
    _MAKE(  1, "转换到BMP", ConvertToBMP, "将本库支持读取的图像文件转换为24位的BMP文件。成功返回 1 ，失败返回一个小于等于零的数值，具体为： 0: 未知错误； -1: 文件不存在； -2: 系统资源不足； -3: 该文件格式不被支持； -4: 编码时出错； -6: 编码时系统资源不足； -7: 编码参数错误。 目前可以读取如下源格式的图像并转换到BMP格式：标准JPEG（全部格式）、BMP（目前仅支持24位BMP）、TIFF（单页图像所有格式）、PNG（32位及32位以下）、PCX（8位和24位，RLE压缩）、PCD（取第三幅）", 0, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_eimgconverter_global_var + 6)\
    _MAKE(  2, "转换到TIF", ConvertToTIF, "将本库支持读取的图像文件转换为TIFF文件。成功返回 1 ，失败返回一个小于等于零的数值，具体为： 0: 未知错误； -1: 文件不存在； -2: 系统资源不足； -3: 该文件格式不被支持； -4: 编码时出错； -6: 编码时系统资源不足； -7: 编码参数错误。 目前可以读取如下源格式的图像并转换到TIFF格式：标准JPEG（全部格式）、BMP（目前仅支持24位BMP）、TIFF（单页图像所有格式）、PNG（32位及32位以下）、PCX（8位和24位，RLE压缩）、PCD（取第三幅）", 0, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_eimgconverter_global_var + 8)\
    _MAKE(  3, "转换到PNG", ConvertToPNG, "将本库支持读取的图像文件转换为24位的PNG文件。成功返回 1 ，失败返回一个小于等于零的数值，具体为： 0: 未知错误； -1: 文件不存在； -2: 系统资源不足； -3: 该文件格式不被支持； -4: 编码时出错； -6: 编码时系统资源不足； -7: 编码参数错误。目前可以读取如下源格式的图像并转换到PNG格式：标准JPEG（全部格式）、BMP（目前仅支持24位BMP）、TIFF（单页图像所有格式）、PNG（32位及32位以下）、PCX（8位和24位，RLE压缩）、PCD（取第三幅）", 0, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_eimgconverter_global_var + 11)\
    _MAKE(  4, "转换到PPM", ConvertToPPM, "将本库支持读取的图像文件转换为PPM文件，目前支持文本模式和二进制模式。成功返回 1 ，失败返回一个小于等于零的数值，具体为： 0: 未知错误； -1: 文件不存在； -2: 系统资源不足； -3: 该文件格式不被支持； -4: 编码时出错； -6: 编码时系统资源不足； -7: 编码参数错误。 目前可以读取如下源格式的图像并转换到PPM格式：标准JPEG（全部格式）、BMP（目前仅支持24位BMP）、TIFF（单页图像所有格式）、PNG（32位及32位以下）、PCX（8位和24位，RLE压缩）、PCD（取第三幅）", 0, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_eimgconverter_global_var + 14)\
    _MAKE(  5, "转换到PGM", ConvertToPGM, "将本库支持读取的图像文件转换为PGM文件，目前支持文本模式和二进制模式。成功返回 1 ，失败返回一个小于等于零的数值，具体为： 0: 未知错误； -1: 文件不存在； -2: 系统资源不足； -3: 该文件格式不被支持； -4: 编码时出错； -6: 编码时系统资源不足； -7: 编码参数错误 目前可以读取如下源格式的图像并转换到PGM格式：标准JPEG（全部格式）、BMP（目前仅支持24位BMP）、TIFF（单页图像所有格式）、PNG（32位及32位以下）、PCX（8位和24位，RLE压缩）、PCD（取第三幅）", 0, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_eimgconverter_global_var + 17)\
    _MAKE(  6, "转换到PBM", ConvertToPBM, "将本库支持读取的图像文件转换为PBM文件，目前支持文本模式。成功返回 1 ，失败返回一个小于等于零的数值，具体为： 0: 未知错误； -1: 文件不存在； -2: 系统资源不足； -3: 该文件格式不被支持； -4: 编码时出错； -6: 编码时系统资源不足； -7: 编码参数错误。 目前可以读取如下源格式的图像并转换到PBM格式：标准JPEG（全部格式）、BMP（目前仅支持24位BMP）、TIFF（单页图像所有格式）、PNG（32位及32位以下）、PCX（8位和24位，RLE压缩）、PCD（取第三幅）", 0, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_eimgconverter_global_var + 20)\
    _MAKE(  7, "转换到PCX", ConvertToPCX, "将本库支持读取的图像文件转换为24位的PCX文件。成功返回 1 ，失败返回一个小于等于零的数值，具体为： 0: 未知错误； -1: 文件不存在； -2: 系统资源不足； -3: 该文件格式不被支持； -4: 编码时出错； -6: 编码时系统资源不足； -7: 编码参数错误 目前可以读取如下源格式的图像并转换到PCX格式：标准JPEG（全部格式）、BMP（目前仅支持24位BMP）、TIFF（单页图像所有格式）、PNG（32位及32位以下）、PCX（8位和24位，RLE压缩）、PCD（取第三幅）", 0, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_eimgconverter_global_var + 22)\
    _MAKE(  8, "转换到PIC", ConvertToPIC, "将本库支持读取的图像文件转换为24位的PICS文件。成功返回 1 ，失败返回一个小于等于零的数值，具体为： 0: 未知错误； -1: 文件不存在； -2: 系统资源不足； -3: 该文件格式不被支持； -4: 编码时出错； -6: 编码时系统资源不足； -7: 编码参数错误。 目前可以读取如下源格式的图像并转换到PICS格式：标准JPEG（全部格式）、BMP（目前仅支持24位BMP）、TIFF（单页图像所有格式）、PNG（32位及32位以下）、PCX（8位和24位，RLE压缩）、PCD（取第三幅）", 0, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_eimgconverter_global_var + 24)\
    _MAKE(  9, "转换到SGI", ConvertToSGI, "将本库支持读取的图像文件转换为24位的SGI文件。成功返回 1 ，失败返回一个小于等于零的数值，具体为： 0: 未知错误； -1: 文件不存在； -2: 系统资源不足； -3: 该文件格式不被支持； -4: 编码时出错； -6: 编码时系统资源不足； -7: 编码参数错误。目前可以读取如下源格式的图像并转换到SGI格式：标准JPEG（全部格式）、BMP（目前仅支持24位BMP）、TIFF（单页图像所有格式）、PNG（32位及32位以下）、PCX（8位和24位，RLE压缩）、PCD（取第三幅）", 0, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_eimgconverter_global_var + 26)

