#ifndef __E_STATIC_LIB
#include "include_mysql_header.h"

LIB_CONST_INFO g_ConstInfo_mysql_global_var[] = 
{
//#define    CT_NULL 0
//#define    CT_NUM  1      // value sample: 3.1415926
//#define    CT_BOOL 2      // value sample: 1
//#define    CT_TEXT 3      // value sample: "abc"
//LPCSTR m_szName;          // 常量名
//LPCSTR m_szEgName;        // 英文名
//LPCSTR m_szExplain;       // 说明
//SHORT  m_shtLayout;       // 必须为 1
//SHORT  m_shtType;         // 常量类型, CT_NULL=空常量, CT_NUM=数值型,double保存值, CT_BOOL=逻辑型, CT_TEXT=文本型
//LPCSTR m_szText;          // CT_TEXT
//DOUBLE m_dbValue;         // CT_NUM、CT_BOOL
    /*000*/ { "字段基本类型", NULL, NULL, 1, CT_NUM, NULL, 1 },
    /*001*/ { "字段附加类型", NULL, NULL, 1, CT_NUM, NULL, 2 },
    /*002*/ { "增加字段", NULL, NULL, 1, CT_NUM, NULL, 1 },
    /*003*/ { "修改字段", NULL, NULL, 1, CT_NUM, NULL, 2 },
    /*004*/ { "删除字段", NULL, NULL, 1, CT_NUM, NULL, 3 },
    /*005*/ { "增加索引", NULL, NULL, 1, CT_NUM, NULL, 4 },
    /*006*/ { "删除索引", NULL, NULL, 1, CT_NUM, NULL, 5 },
    /*007*/ { "增加主键", NULL, NULL, 1, CT_NUM, NULL, 6 },
    /*008*/ { "删除主键", NULL, NULL, 1, CT_NUM, NULL, 7 },
    /*009*/ { "最大长度", NULL, NULL, 1, CT_NUM, NULL, 1 },
    /*010*/ { "无符号", NULL, NULL, 1, CT_NUM, NULL, 2 },
    /*011*/ { "以0填充", NULL, NULL, 1, CT_NUM, NULL, 4 },
    /*012*/ { "二进制", NULL, NULL, 1, CT_NUM, NULL, 8 },
    /*013*/ { "MYSQL未压缩浮点数型", "decimal", "未压缩浮点数，不能无符号。行为如同一个文本型：“未压缩”意味着数字作为一个字符串被存储，值的每一位使用一个字符。值的最大范围与DOUBLE相同", 1, CT_NUM, NULL, 0 },
    /*014*/ { "MYSQL字节型", "tinyint", "一个字节。有符号的范围是-128到127，无符号的范围是0到255", 1, CT_NUM, NULL, 101 },
    /*015*/ { "MYSQL逻辑型", "bool", "逻辑型，一个字节", 1, CT_NUM, NULL, 102 },
    /*016*/ { "MYSQL短整型", "smallint", "短整型，有符号的范围是-32768到32767，无符号的范围是0到65535", 1, CT_NUM, NULL, 2 },
    /*017*/ { "MYSQL整型", "int", "整型，有符号的范围是-2147483648到2147483647，无符号的范围是0到4294967295", 1, CT_NUM, NULL, 3 },
    /*018*/ { "MYSQL浮点型", "float", "浮点型，不能无符号。允许的值是-3.402823466E+38到-1.175494351E-38，0 和1.175494351E-38到3.402823466E+38", 1, CT_NUM, NULL, 4 },
    /*019*/ { "MYSQL双精度浮点型", "double", "双精度浮点型，不能无符号。允许的值是-1.7976931348623157E+308到-2.2250738585072014E-308、 0和2.2250738585072014E-308到1.7976931348623157E+308", 1, CT_NUM, NULL, 5 },
    /*020*/ { "MYSQL时间戳记型", "timestamp", "时间戳记，范围是'1970-01-01 00:00:00'到2037年的某时。MySQL以YYYYMMDDHHMMSS、YYMMDDHHMMSS、YYYYMMDD或YYMMDD格式来显示时间戳记的值", 1, CT_NUM, NULL, 7 },
    /*021*/ { "MYSQL长整型", "bigint", "长整型，有符号的范围是-9223372036854775808到9223372036854775807，无符号的范围是0到18446744073709551615", 1, CT_NUM, NULL, 8 },
    /*022*/ { "MYSQL24位整型", "mediumint", "24整型，有符号的范围是-8388608到8388607，无符号的范围是0到16777215", 1, CT_NUM, NULL, 9 },
    /*023*/ { "MYSQL日期型", "date", "日期，支持的范围是'1000-01-01'到'9999-12-31'。MySQL以'YYYY-MM-DD'格式来显示日期值，但是允许你使用字符串或数字把值赋给类型为日期的列", 1, CT_NUM, NULL, 10 },
    /*024*/ { "MYSQL时间型", "time", "时间，范围是'-838:59:59'到'838:59:59'。MySQL以'HH:MM:SS'格式来显示时间值，但是允许你使用字符串或数字把值赋给类型为时间的列", 1, CT_NUM, NULL, 11 },
    /*025*/ { "MYSQL日期与时间型", "datetime", "日期与时间，支持的范围是'1000-01-01 00:00:00'到'9999-12-31 23:59:59'。MySQL以'YYYY-MM-DD HH:MM:SS'格式来显示日期与时间的值，但是允许你使用字符串或数字把值赋给属性是日期与时间的列", 1, CT_NUM, NULL, 12 },
    /*026*/ { "MYSQL年份型", "year", "年，允许的值是1901到2155，和0000（4位年格式），如果你使用2位，1970-2069( 70-69)。MySQL以YYYY格式来显示年的值，但是允许你把使用字符串或数字值赋给属性是年的列", 1, CT_NUM, NULL, 13 },
    /*027*/ { "MYSQL短字节集", "tinyblob", "短字节集，最大长度为255(2^8-1)个字符", 1, CT_NUM, NULL, 25201 },
    /*028*/ { "MYSQL短备注型", "tinytext", "短备注，最大长度为255(2^8-1)个字符", 1, CT_NUM, NULL, 25202 },
    /*029*/ { "MYSQL备注型", "text", "备注，最大长度为65535(2^16-1)个字符", 1, CT_NUM, NULL, 25203 },
    /*030*/ { "MYSQL字节集型", "blob", "字节集，最大长度为65535(2^16-1)个字符", 1, CT_NUM, NULL, 25204 },
    /*031*/ { "MYSQL长字节集型", "mediumblob", "长字节集，最大长度为16777215(2^24-1)个字符", 1, CT_NUM, NULL, 25205 },
    /*032*/ { "MYSQL长备注型", "mediumtext", "长备注，最大长度为16777215(2^24-1)个字符", 1, CT_NUM, NULL, 25206 },
    /*033*/ { "MYSQL巨字节集型", "longblob", "巨字节集，最大长度为4294967295(2^32-1)个字符", 1, CT_NUM, NULL, 25207 },
    /*034*/ { "MYSQL巨备注型", "longtext", "巨备注，最大长度为4294967295(2^32-1)个字符", 1, CT_NUM, NULL, 25208 },
    /*035*/ { "MYSQL变长文本型", "varchar", "变长文本型，一个变长字符串", 1, CT_NUM, NULL, 253 },
    /*036*/ { "MYSQL枚举型", "enum", "枚举，一个仅有一个值的字符串对象，这个值式选自与值列表'value1'、'value2', ...,或NULL。一个枚举最多能有65535不同的值", 1, CT_NUM, NULL, 25401 },
    /*037*/ { "MYSQL集合型", "set", "集合，能有零个或多个值的一个字符串对象，其中每一个必须从值列表'value1', 'value2', ...选出。一个集合最多能有64个成员", 1, CT_NUM, NULL, 25402 },
    /*038*/ { "MYSQL文本型", "char", "文本型，一个定长字符串", 1, CT_NUM, NULL, 25403 },
    /*039*/ { "字节与逻辑型", "TINYINT", "字节与逻辑型，不能用与定义字段，可以使用他来判断字段类型", 1, CT_NUM, NULL, 1 },
    /*040*/ { "备注与字节集型", "TINYINT", "备注与字节集型，不能用与定义字段，可以使用他来判断字段类型", 1, CT_NUM, NULL, 252 },
    /*041*/ { "枚举与集合型", "TINYINT", "枚举与集合型，不能用与定义字段，可以使用他来判断字段类型", 1, CT_NUM, NULL, 254 },
    /*042*/ { "无任何权限", NULL, NULL, 1, CT_NUM, NULL, 0 },
    /*043*/ { "查询权限", NULL, NULL, 1, CT_NUM, NULL, 1 },
    /*044*/ { "增加权限", NULL, NULL, 1, CT_NUM, NULL, 2 },
    /*045*/ { "更改权限", NULL, NULL, 1, CT_NUM, NULL, 4 },
    /*046*/ { "删除权限", NULL, NULL, 1, CT_NUM, NULL, 8 },
    /*047*/ { "索引权限", NULL, NULL, 1, CT_NUM, NULL, 16 },
    /*048*/ { "改变表权限", NULL, NULL, 1, CT_NUM, NULL, 32 },
    /*049*/ { "创建表或库或索引权限", NULL, NULL, 1, CT_NUM, NULL, 64 },
    /*050*/ { "删除表或库权限", NULL, NULL, 1, CT_NUM, NULL, 128 },
    /*051*/ { "备注表或库权限", NULL, NULL, 1, CT_NUM, NULL, 256 },
    /*052*/ { "重新装载服务器权限", NULL, NULL, 1, CT_NUM, NULL, 512 },
    /*053*/ { "关闭服务器权限", NULL, NULL, 1, CT_NUM, NULL, 1024 },
    /*054*/ { "服务器进程管理权限", NULL, NULL, 1, CT_NUM, NULL, 2048 },
    /*055*/ { "服务器文件存取权限", NULL, NULL, 1, CT_NUM, NULL, 4096 },
    /*056*/ { "所有权限", NULL, NULL, 1, CT_NUM, NULL, 8192 },
};
int g_ConstInfo_mysql_global_var_count = sizeof(g_ConstInfo_mysql_global_var) / sizeof(g_ConstInfo_mysql_global_var[0]);
#endif
