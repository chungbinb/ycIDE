#pragma once

#define __DP1_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 dp1_test_0_dp1
#define DP1_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__DP1_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "dp1_test_0_dp1"
#define DP1_NAME_STR(_index, _name) ______E_FNENAME(__DP1_NAME(_index, _name))

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
#define DP1_DEF(_MAKE) \
    _MAKE(  0, "压缩数据", Compress, "压缩一段字节集数据，返回被压缩后的结果字节集。如果压缩失败，返回空字节集", 1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_dp1_global_var + 0)\
    _MAKE(  1, "解压数据", Uncompress, "解压一个内含被压缩数据的字节集，返回被解压后的结果字节集。如果压缩失败，返回空字节集。注意解压后的数据也有可能是空字节集", 1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_dp1_global_var + 0)\
    _MAKE(  2, "取数据摘要", GetMD5, "返回一段字节集数据的MD5数据摘要编码文本。不同数据的MD5码都不一样，因此本命令可以用作保证重要数据不会被篡改", 2, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_dp1_global_var + 0)\
    _MAKE(  3, "加密数据", Encrypt, "加密一段字节集数据，返回加密后的结果字节集。如果失败，返回空字节集", 3, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_dp1_global_var + 1)\
    _MAKE(  4, "解密数据", Decrypt, "解密一段加密后的字节集数据，返回解密后的结果字节集。注意本命令并不对密码文本进行校验，如果密码提供错误，将返回错误的结果。如果失败，返回空字节集", 3, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_dp1_global_var + 4)\
    _MAKE(  5, "数字签名", RSAEncrypt, "使用RSA非对称密钥算法对指定字节集数据进行签署，支持32到4096之间任意合法的RSA位数，返回签署后的结果文本。\r\n  附一、电子签名系统简要实现方法：\r\n  首先给所有具有签署权利的人员授予不同的RSA钥匙，RSA钥匙由“公钥”、“私钥”、“公共模数”三部分组成，为了生成符合要求的随机密钥，可以下载使用 http://dywt.com.cn/RSATool2v14.rar 第三方工具软件（内有操作说明）。然后将所有人员的“公钥”和“公共模数”成对向外公布，“私钥”由被授予人员自行妥善保管。\r\n  被授权人员使用“私钥”和“公共模数”对指定数据进行签署，然后将签署后的结果文本随同被签署数据一起发送，接收者收到后，使用该签署者公开的“公钥”和“公共模数”及签署结果文本对被签署数据进行验证，如验证通过则说明该数据必定是此签署者所认可的，且没有经过任何篡改。\r\n  附二、数字签名用作软件注册系统的实现步骤：\r\n  1、欲注册用户提供注册信息（如硬件代码、用户姓名等）；\r\n  2、软件作者使用自己的私钥对该注册信息进行数字签署得到签署结果文本；\r\n  3、将此签署结果文本作为注册钥匙文件发送给用户；\r\n  4、在用户端的软件使用相同的用户信息、注册钥匙文件及软件作者的公钥进行签名验证，如果通过表明已经注册，否则表示未注册。使用本方法在私钥未泄露的前提下，可以绝对避免破解者做出软件的注册机", 2, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_dp1_global_var + 7)\
    _MAKE(  6, "签名验证", RSACheck, "使用指定公钥和签署结果文本对被签署字节集数据进行验证，如验证通过则说明该字节集数据必定是此公钥拥有者（即签署者）所认可的，且签署后未经任何篡改。验证通过返回真，否则返回假", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_dp1_global_var + 10)

