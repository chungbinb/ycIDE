#ifndef __E_STATIC_LIB
#include "include_exmlparser_header.h"

// XML树  下的方法索引
static INT s_dtCmdIndexexmlparser_EXMLParser_static_var_00[] = 
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 
    20, 21, 22, 30, 31, 32, 33, 34, 35, 36, 
    37, 38, 39, 
};
static int s_dtCmdIndexexmlparser_EXMLParser_static_var_count_00 = sizeof(s_dtCmdIndexexmlparser_EXMLParser_static_var_00) / sizeof(s_dtCmdIndexexmlparser_EXMLParser_static_var_00[0]);

// XML树  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventexmlparser_EXMLParser_static_var_00[] = 
{
    /*000*/ {SDT_INT, 0, "创建标志", "CreateFlag", "1为已经创建 0为没有创建", NULL, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "XML根节点的指针", "pXMLRootNode", NULL, LES_HIDED, (INT)0x00000000},

};
static int s_objEventexmlparser_EXMLParser_static_var_count_00 = sizeof(s_objEventexmlparser_EXMLParser_static_var_00) / sizeof(s_objEventexmlparser_EXMLParser_static_var_00[0]);

LIB_DATA_TYPE_INFO g_DataType_exmlparser_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "XML树", "EXMLParser", "该数据类型提供对W3C标准XML文件的读写支持",
            s_dtCmdIndexexmlparser_EXMLParser_static_var_count_00, s_dtCmdIndexexmlparser_EXMLParser_static_var_00, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventexmlparser_EXMLParser_static_var_count_00, s_objEventexmlparser_EXMLParser_static_var_00
        },
};
int g_DataType_exmlparser_global_var_count = sizeof(g_DataType_exmlparser_global_var) / sizeof(g_DataType_exmlparser_global_var[0]);

#endif

