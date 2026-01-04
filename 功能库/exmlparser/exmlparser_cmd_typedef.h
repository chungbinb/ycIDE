#pragma once

#define __EXMLPARSER_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 exmlparser_test_0_exmlparser
#define EXMLPARSER_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__EXMLPARSER_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "exmlparser_test_0_exmlparser"
#define EXMLPARSER_NAME_STR(_index, _name) ______E_FNENAME(__EXMLPARSER_NAME(_index, _name))

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
#define EXMLPARSER_DEF(_MAKE) \
    _MAKE(  0, "创建", CreateNewTree, "在内存中创建一个新的XML的解析树。如果XML树已经创建，则自动释放当前XML树，然后执行创建操作。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_exmlparser_global_var + 0)\
    _MAKE(  1, "导入", LoadFrom, "从XML文件或字节集中导入数据，并根据该数据重建XML树（如果XML树已经创建，则自动释放当前XML树，然后执行导入操作）。该方法执行成功返回真，执行失败返回假。本方法将对导入的数据进行必要的语法检查，如果发现非法的数据格式，将直接返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_exmlparser_global_var + 1)\
    _MAKE(  2, "导出到文件", WriteToFile, "保存当前XML树的全部数据到指定的文件。该方法执行成功返回真，执行失败返回假。注：可使用“取XML数据()”获取相同的字节集数据而不必保存到文件。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_exmlparser_global_var + 2)\
    _MAKE(  3, "释放", Release, "释放当前XML树，释放后该XML树将不再可用。如果XML树在离开作用域前没有释放，系统会自动释放XML树", -1, _CMD_OS(__OS_WIN) | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_exmlparser_global_var + 0)\
    _MAKE(  4, "取根节点名", GetRootNodeCaption, "取得当前解析树根节点的名称。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_exmlparser_global_var + 6)\
    _MAKE(  5, "取子节点数", GetChildNodeNumber, "取得参数节点的所有子节点的个数。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlparser_global_var + 7)\
    _MAKE(  6, "取子节点名", GetChildNodeCaption, "取得参数节点的所有子节点名称。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlparser_global_var + 9)\
    _MAKE(  7, "取节点值", GetChildNodeValue, "取得参数节点的值。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlparser_global_var + 11)\
    _MAKE(  8, "取二进制值", GetBinaryValue, "取得参数节点的二进制数据的值。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlparser_global_var + 13)\
    _MAKE(  9, "取全部属性名", GetNodeAttrName, "取得参数节点的所有属性的名称。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlparser_global_var + 15)\
    _MAKE( 10, "取属性值", GetNodeAttrValue, "取得参数节点的特定属性的值。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_exmlparser_global_var + 17)\
    _MAKE( 11, "插入节点", InsertNode, "新建一个节点，并把该参数节点作为该新节点的父节点，然后插入到XML树中。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_exmlparser_global_var + 20)\
    _MAKE( 12, "插入属性", InsertAttr, "在参数节点中插入一个新的属性，并给属性赋值。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_exmlparser_global_var + 24)\
    _MAKE( 13, "删除节点", DelNode, "该方法会删除给定节点和他的所有子节点，该方法不允许删除根节点。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_exmlparser_global_var + 7)\
    _MAKE( 14, "删除属性", DelAttr, "该方法会删除给定节点的指定属性。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlparser_global_var + 27)\
    _MAKE( 15, "修改节点名", ModifyNodeCaption, "该方法会修改指定节点的名称。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlparser_global_var + 29)\
    _MAKE( 16, "修改节点值", ModifyNodeValue, "该方法会修改指定节点的值。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_exmlparser_global_var + 31)\
    _MAKE( 17, "修改二进制值", ModifyNodeBinaryValue, "该方法会修改指定节点的值为二进制的值。注意该方法指定的二进制数据不应太大，否则该支持库将无法解析。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlparser_global_var + 34)\
    _MAKE( 18, "修改属性名", ModifyNodeAttrCaption, "该方法会修改指定节点的属性名称。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_exmlparser_global_var + 36)\
    _MAKE( 19, "修改属性值", ModifyNodeAttrValue, "该方法会修改指定节点的属性值。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_exmlparser_global_var + 39)\
    _MAKE( 20, "批量删除节点", BatchDeleteNode, "该方法要删除节点的路径参数可以支持通配符（*,?），该方法会删除所有符合路径条件的节点和该节点的子节点，该方法不允许删除根节点。注意索引形式的路径参数不支持通配符。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_exmlparser_global_var + 42)\
    _MAKE( 21, "批量取节点值", BatchGetNodeValue, "该方法要取值节点的路径参数可以支持通配符（*,?），该方法会取所有符合路径条件的节点的值。注意索引形式的路径参数不支持通配符。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlparser_global_var + 43)\
    _MAKE( 22, "批量修改节点值", BatchModifyNodeValue, "该方法要修改节点的路径参数可以支持通配符（*,?），该方法会修改所有符合路径条件的节点的值。注意索引形式的路径参数不支持通配符。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_exmlparser_global_var + 45)\
    _MAKE( 23, "匹配通配符", BlurMatch, "该方法第一个参数包含通配符（*,?），通配符“*”代表零个或多个任意字符，通配符“?”代表一个任意字符，第二个参数是一个不包含通配符的常量字符串，该方法得到匹配字符串和常量字符串的比较结果。比较相等返回真，不等返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlparser_global_var + 48)\
    _MAKE( 24, "无法识别的名字_24", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_exmlparser_global_var + 0)\
    _MAKE( 25, "无法识别的名字_25", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_exmlparser_global_var + 0)\
    _MAKE( 26, "无法识别的名字_26", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_exmlparser_global_var + 0)\
    _MAKE( 27, "无法识别的名字_27", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_exmlparser_global_var + 0)\
    _MAKE( 28, "无法识别的名字_28", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_exmlparser_global_var + 0)\
    _MAKE( 29, "无法识别的名字_29", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_exmlparser_global_var + 0)\
    _MAKE( 30, "取根节点名文本", GetRootNodeCaption, "取得当前解析树根节点的名称文本。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_exmlparser_global_var + 50)\
    _MAKE( 31, "取子节点个数", GetChildNodeNumber, "取得参数节点的所有子节点的个数。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlparser_global_var + 51)\
    _MAKE( 32, "取所有子节点名", GetAllChildNodeCaption, "取得由参数节点的所有子节点名称组成的文本数组。", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlparser_global_var + 53)\
    _MAKE( 33, "取节点值文本", GetChildNodeValue, "取得参数节点值文本。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlparser_global_var + 55)\
    _MAKE( 34, "取节点值字节集", GetBinaryValue, "取得参数节点的二进制数据值。", -1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlparser_global_var + 57)\
    _MAKE( 35, "取属性个数", GetNodeAttrCount, "取得由参数节点的属性个数。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlparser_global_var + 59)\
    _MAKE( 36, "取所有属性名", GetNodeAllAttrName, "取得由参数节点的所有属性名称组成的文本数组。", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlparser_global_var + 61)\
    _MAKE( 37, "取属性值文本", GetNodeAttrValue, "取得参数节点的特定属性值文本。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_exmlparser_global_var + 63)\
    _MAKE( 38, "取XML数据", GetXMLData, "取当前XML树的全部数据，并转换为字节集后返回。如果执行失败，返回值为空字节集。本方法所返回的数据与“导出到文件()”所写入文件的数据完全等同。", -1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_exmlparser_global_var + 66)\
    _MAKE( 39, "取节点名文本", GetNodeCaption, "取得参数指定节点的节点名。通常用于以索引形式（如\"@1/@2\"）指定节点路径的情况。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlparser_global_var + 70)

