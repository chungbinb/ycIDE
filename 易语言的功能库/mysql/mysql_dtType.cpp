#ifndef __E_STATIC_LIB
#include "include_mysql_header.h"

// 字段信息类型  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventmysql_field_info_static_var_00[] = 
{
    /*000*/ {SDT_INT, 0, "类型", "type", "本结构的类型 #字段基本类型 或 #字段附加类型。为#字段基本类型时主键与索引将被忽略；为#字段附加类型时，除主键与索引外其它元素将被忽略。", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*001*/ {SDT_TEXT, 0, "字段名", "field_name", "如果为中文字段名请在字段名两边加“`”，如：`姓名`。", NULL, (INT)0x00000000},
    /*002*/ {SDT_INT, 0, "字段类型", "field_type", "详细类型说明，请参见本支持库定义的字段类型常量", NULL, (INT)0x00000000},
    /*003*/ {SDT_BOOL, 0, "列内容为空", "not_null", "如果为真则允许列内容为空，如果为假则列内容不能为空", NULL, (INT)0x00000000},
    /*004*/ {SDT_TEXT, 0, "列数据默认值", "field_default", "列内容的默认值，如果没有默认值为空字符串", NULL, (INT)0x00000000},
    /*005*/ {SDT_BOOL, 0, "自增量标记", "increment", "如果为真则当前字段具有自增量属性，即在插入新数据时该字段会在原有最大值的基础上自动增加1，设置次属性。详细介绍请参考MYSQL手册", NULL, (INT)0x00000000},
    /*006*/ {SDT_INT, 0, "附加类型信息", "type_append", "附加与字段的属性，其值为 #最大长度 #无符号 #以0填充 #二进制。可以把属性相加，如 #无符号 + #以0填充。属性具体含义请参见MYSQL手册", NULL, (INT)0x00000000},
    /*007*/ {SDT_TEXT, 0, "附加内容", "content_append", "如果附加类型信息具有 #最大长度 类型那么这个参数为最大长度，如果字段类型为 #枚举 或 #集合 那么这个参数为枚举或集合的取值范围，如果本结构类型为 #字段附加类型，并且为索引。那么本成员变量为索引名", NULL, (INT)0x00000000},
    /*008*/ {SDT_TEXT, 0, "主键", "primary_key", "设置为主键的字段名，如“fieldname”。为#附加类型时有效，每个表中只可以有一个主键，具体作用请参见MYSQL手册", NULL, (INT)0x00000000},
    /*009*/ {SDT_TEXT, 0, "索引", "index", "设置为索引的字段名可以有多个，通过“,”分割，如“fieldname1,fieldname2,fieldname3”。为#附加类型时有效，具体作用请参见MYSQL手册", NULL, (INT)0x00000000},

};
static int s_objEventmysql_field_info_static_var_count_00 = sizeof(s_objEventmysql_field_info_static_var_00) / sizeof(s_objEventmysql_field_info_static_var_00[0]);

// 表更改信息类型  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventmysql_table_alter_static_var_01[] = 
{
    /*000*/ {SDT_TEXT, 0, "字段名", "field_name", "当修改类型为#修改字段时使用本成员作为旧字段名。当为#删除字段时本成员为准备删除的字段名", NULL, (INT)0x00000000},
    /*001*/ {SDT_TEXT, 0, "字段信息", "field_info", "当修改类型为#增加索引时本成员作为索引使用的字段。如果需要使用多个字段可以通过“,“分割。如”fieldname1,fieldname2,fieldname3“", NULL, (INT)0x00000000},
    /*002*/ {SDT_TEXT, 0, "索引名", "index", "当修改类型为#增加索引或#删除索引时本成员作为索引名使用", NULL, (INT)0x00000000},
    /*003*/ {SDT_TEXT, 0, "主键名", "primary", "当修改类型为#增加主键时本成员作为主键名使用", NULL, (INT)0x00000000},

};
static int s_objEventmysql_table_alter_static_var_count_01 = sizeof(s_objEventmysql_table_alter_static_var_01) / sizeof(s_objEventmysql_table_alter_static_var_01[0]);

LIB_DATA_TYPE_INFO g_DataType_mysql_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "字段信息类型", "field_info", NULL,
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventmysql_field_info_static_var_count_00, s_objEventmysql_field_info_static_var_00
        },
/*001*/ { "表更改信息类型", "table_alter", NULL,
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventmysql_table_alter_static_var_count_01, s_objEventmysql_table_alter_static_var_01
        },
};
int g_DataType_mysql_global_var_count = sizeof(g_DataType_mysql_global_var) / sizeof(g_DataType_mysql_global_var[0]);

#endif

