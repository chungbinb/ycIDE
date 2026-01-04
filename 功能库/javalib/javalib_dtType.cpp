#ifndef __E_STATIC_LIB
#include "include_javalib_header.h"

// Java虚拟机  下的方法索引
static INT s_dtCmdIndexjavalib_JavaVM_static_var_00[] = 
{
    0, 1, 2, 3, 4, 
};
static int s_dtCmdIndexjavalib_JavaVM_static_var_count_00 = sizeof(s_dtCmdIndexjavalib_JavaVM_static_var_00) / sizeof(s_dtCmdIndexjavalib_JavaVM_static_var_00[0]);

// Java本地接口  下的方法索引
static INT s_dtCmdIndexjavalib_JNIEnv_static_var_01[] = 
{
    0, 5, 6, 7, 8, 9, 10, 11, 12, 13, 
    14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 
    24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 
    34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 
    44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 
    54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 
    64, 65, 
};
static int s_dtCmdIndexjavalib_JNIEnv_static_var_count_01 = sizeof(s_dtCmdIndexjavalib_JNIEnv_static_var_01) / sizeof(s_dtCmdIndexjavalib_JNIEnv_static_var_01[0]);

// 数组类型  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventjavalib_ArrayType_static_var_02[] = 
{
    /*000*/ {SDT_INT, 0, "逻辑型", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*001*/ {SDT_INT, 0, "字节型", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*002*/ {SDT_INT, 0, "字符型", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*003*/ {SDT_INT, 0, "短整数型", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000004},
    /*004*/ {SDT_INT, 0, "整数型", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000005},
    /*005*/ {SDT_INT, 0, "长整数型", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000006},
    /*006*/ {SDT_INT, 0, "小数型", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000007},
    /*007*/ {SDT_INT, 0, "双精度小数型", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000008},

};
static int s_objEventjavalib_ArrayType_static_var_count_02 = sizeof(s_objEventjavalib_ArrayType_static_var_02) / sizeof(s_objEventjavalib_ArrayType_static_var_02[0]);

LIB_DATA_TYPE_INFO g_DataType_javalib_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "Java虚拟机", "JavaVM", "Java虚拟机，即Java Virtual Machine（JVM）。本类型主要实现对Java虚拟机的生命周期管理。一般情况下，“Java虚拟机”会自动创建和销毁，不需要用户介入。但如需特别指定“用户类路径”或“本地库路径”，则用户必须显式调用“创建()”方法自行创建“Java虚拟机”。",
            s_dtCmdIndexjavalib_JavaVM_static_var_count_00, s_dtCmdIndexjavalib_JavaVM_static_var_00, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            NULL, NULL
        },
/*001*/ { "Java本地接口", "JNIEnv", "本类型是对JNI Environment的封装。JNI即Java Native Interface：JAVA本地接口。程序中主要使用本类型完成与Java虚拟机的交互，通过调用本类型的一系列方法，达到访问Java类库的目的。本类型的方法大致分为以下几类：类/对象操作；全局/局部引用；访问对象成员；调用对象方法；字符串操作；数组操作；异常处理；其它。这些方法基本上涵盖了访问Java类库所需的一切。对于Java领域中的“类”“对象”“成员”“方法”“字符串”“数组”“异常”等类或对象，在易语言中分别用一个“整数型”标志符来表示，相应的称为“类标志符”“对象标志符”“成员标志符”“方法标志符”“字符串对象标志符”“数组标志符”“异常对象标志符”等。此类标志符在本支持库中被广泛使用。调用相关的方法可以获得所需标志符。",
            s_dtCmdIndexjavalib_JNIEnv_static_var_count_01, s_dtCmdIndexjavalib_JNIEnv_static_var_01, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            NULL, NULL
        },
/*002*/ { "数组类型", "ArrayType", "用于“Java本地接口.创建数组()”方法的第二个参数，指定将创建可容纳何种基本数据类型的数组。",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventjavalib_ArrayType_static_var_count_02, s_objEventjavalib_ArrayType_static_var_02
        },
};
int g_DataType_javalib_global_var_count = sizeof(g_DataType_javalib_global_var) / sizeof(g_DataType_javalib_global_var[0]);

#endif

