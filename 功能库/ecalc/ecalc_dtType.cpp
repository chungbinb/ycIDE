#ifndef __E_STATIC_LIB
#include "include_ecalc_header.h"

// 复数运算  下的方法索引
static INT s_dtCmdIndexecalc_ComplexCalc_static_var_00[] = 
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
    10, 11, 12, 111, 
};
static int s_dtCmdIndexecalc_ComplexCalc_static_var_count_00 = sizeof(s_dtCmdIndexecalc_ComplexCalc_static_var_00) / sizeof(s_dtCmdIndexecalc_ComplexCalc_static_var_00[0]);

// 矩阵运算  下的方法索引
static INT s_dtCmdIndexecalc_MatrixCalc_static_var_01[] = 
{
    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 
    23, 
};
static int s_dtCmdIndexecalc_MatrixCalc_static_var_count_01 = sizeof(s_dtCmdIndexecalc_MatrixCalc_static_var_01) / sizeof(s_dtCmdIndexecalc_MatrixCalc_static_var_01[0]);

// 傅立叶变换  下的方法索引
static INT s_dtCmdIndexecalc_FourierTransform_static_var_02[] = 
{
    24, 25, 26, 27, 28, 
};
static int s_dtCmdIndexecalc_FourierTransform_static_var_count_02 = sizeof(s_dtCmdIndexecalc_FourierTransform_static_var_02) / sizeof(s_dtCmdIndexecalc_FourierTransform_static_var_02[0]);

// 微积分  下的方法索引
static INT s_dtCmdIndexecalc_Calculous_static_var_03[] = 
{
    29, 30, 31, 
};
static int s_dtCmdIndexecalc_Calculous_static_var_count_03 = sizeof(s_dtCmdIndexecalc_Calculous_static_var_03) / sizeof(s_dtCmdIndexecalc_Calculous_static_var_03[0]);

// 概要统计  下的方法索引
static INT s_dtCmdIndexecalc_SummaryStats_static_var_04[] = 
{
    32, 
};
static int s_dtCmdIndexecalc_SummaryStats_static_var_count_04 = sizeof(s_dtCmdIndexecalc_SummaryStats_static_var_04) / sizeof(s_dtCmdIndexecalc_SummaryStats_static_var_04[0]);

// 联立方程  下的方法索引
static INT s_dtCmdIndexecalc_GaussJordan_static_var_05[] = 
{
    33, 34, 
};
static int s_dtCmdIndexecalc_GaussJordan_static_var_count_05 = sizeof(s_dtCmdIndexecalc_GaussJordan_static_var_05) / sizeof(s_dtCmdIndexecalc_GaussJordan_static_var_05[0]);

// 多重回归  下的方法索引
static INT s_dtCmdIndexecalc_MultipleReg_static_var_06[] = 
{
    35, 
};
static int s_dtCmdIndexecalc_MultipleReg_static_var_count_06 = sizeof(s_dtCmdIndexecalc_MultipleReg_static_var_06) / sizeof(s_dtCmdIndexecalc_MultipleReg_static_var_06[0]);

// 曲线拟合  下的方法索引
static INT s_dtCmdIndexecalc_CurveFit_static_var_07[] = 
{
    36, 37, 38, 
};
static int s_dtCmdIndexecalc_CurveFit_static_var_count_07 = sizeof(s_dtCmdIndexecalc_CurveFit_static_var_07) / sizeof(s_dtCmdIndexecalc_CurveFit_static_var_07[0]);

// 其他计算  下的方法索引
static INT s_dtCmdIndexecalc_OtherCalc_static_var_08[] = 
{
    39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 
    49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 
    59, 60, 61, 
};
static int s_dtCmdIndexecalc_OtherCalc_static_var_count_08 = sizeof(s_dtCmdIndexecalc_OtherCalc_static_var_08) / sizeof(s_dtCmdIndexecalc_OtherCalc_static_var_08[0]);

// 大数  下的方法索引
static INT s_dtCmdIndexecalc_UnlNum_static_var_09[] = 
{
    62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 
    72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 
    82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 
    92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 
    102, 103, 104, 105, 106, 107, 108, 110, 
};
static int s_dtCmdIndexecalc_UnlNum_static_var_count_09 = sizeof(s_dtCmdIndexecalc_UnlNum_static_var_09) / sizeof(s_dtCmdIndexecalc_UnlNum_static_var_09[0]);

// 算式解析  下的方法索引
static INT s_dtCmdIndexecalc_ExpressionParse_static_var_10[] = 
{
    109, 
};
static int s_dtCmdIndexecalc_ExpressionParse_static_var_count_10 = sizeof(s_dtCmdIndexecalc_ExpressionParse_static_var_10) / sizeof(s_dtCmdIndexecalc_ExpressionParse_static_var_10[0]);

// 复数运算  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventecalc_ComplexCalc_static_var_00[] = 
{
    /*000*/ {SDT_INT, 0, "复数存储句柄", "ComplexHandle", NULL, LES_HIDED, (INT)0x00000000},

};
static int s_objEventecalc_ComplexCalc_static_var_count_00 = sizeof(s_objEventecalc_ComplexCalc_static_var_00) / sizeof(s_objEventecalc_ComplexCalc_static_var_00[0]);

// 大数  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventecalc_UnlNum_static_var_09[] = 
{
    /*000*/ {SDT_INT, 0, "大数运算句柄", "UnlNumHandle", NULL, LES_HIDED, (INT)0x00000000},

};
static int s_objEventecalc_UnlNum_static_var_count_09 = sizeof(s_objEventecalc_UnlNum_static_var_09) / sizeof(s_objEventecalc_UnlNum_static_var_09[0]);

LIB_DATA_TYPE_INFO g_DataType_ecalc_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "复数运算", "ComplexCalc", "本类型用来处理复数的运算",
            s_dtCmdIndexecalc_ComplexCalc_static_var_count_00, s_dtCmdIndexecalc_ComplexCalc_static_var_00, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventecalc_ComplexCalc_static_var_count_00, s_objEventecalc_ComplexCalc_static_var_00
        },
/*001*/ { "矩阵运算", "MatrixCalc", "本类型用来处理常见的矩阵运算",
            s_dtCmdIndexecalc_MatrixCalc_static_var_count_01, s_dtCmdIndexecalc_MatrixCalc_static_var_01, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            NULL, NULL
        },
/*002*/ { "傅立叶变换", "FourierTransform", "本类型用来计算傅立叶变换",
            s_dtCmdIndexecalc_FourierTransform_static_var_count_02, s_dtCmdIndexecalc_FourierTransform_static_var_02, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            NULL, NULL
        },
/*003*/ { "微积分", "Calculous", "本类型用来处理常见的微积分计算",
            s_dtCmdIndexecalc_Calculous_static_var_count_03, s_dtCmdIndexecalc_Calculous_static_var_03, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            NULL, NULL
        },
/*004*/ { "概要统计", "SummaryStats", "本类型用来计算常见的数据概要统计",
            s_dtCmdIndexecalc_SummaryStats_static_var_count_04, s_dtCmdIndexecalc_SummaryStats_static_var_04, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            NULL, NULL
        },
/*005*/ { "联立方程", "GaussJordan", "本类型用来求解联立方程",
            s_dtCmdIndexecalc_GaussJordan_static_var_count_05, s_dtCmdIndexecalc_GaussJordan_static_var_05, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            NULL, NULL
        },
/*006*/ { "多重回归", "MultipleReg", "本类型提供方法计算多重回归",
            s_dtCmdIndexecalc_MultipleReg_static_var_count_06, s_dtCmdIndexecalc_MultipleReg_static_var_06, _DT_OS(__OS_WIN) | LDT_IS_HIDED,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            NULL, NULL
        },
/*007*/ { "曲线拟合", "CurveFit", "本类型提供方法计算曲线拟合",
            s_dtCmdIndexecalc_CurveFit_static_var_count_07, s_dtCmdIndexecalc_CurveFit_static_var_07, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            NULL, NULL
        },
/*008*/ { "其他计算", "OtherCalc", "本类型提供一些常用的数值、科学计算函数",
            s_dtCmdIndexecalc_OtherCalc_static_var_count_08, s_dtCmdIndexecalc_OtherCalc_static_var_08, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            NULL, NULL
        },
/*009*/ { "大数", "UnlNum", "本类型提供对无限精度大数运算的支持",
            s_dtCmdIndexecalc_UnlNum_static_var_count_09, s_dtCmdIndexecalc_UnlNum_static_var_09, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventecalc_UnlNum_static_var_count_09, s_objEventecalc_UnlNum_static_var_09
        },
/*010*/ { "算式解析", "ExpressionParse", "本类型提供常用的算式解析方法",
            s_dtCmdIndexecalc_ExpressionParse_static_var_count_10, s_dtCmdIndexecalc_ExpressionParse_static_var_10, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            NULL, NULL
        },
};
int g_DataType_ecalc_global_var_count = sizeof(g_DataType_ecalc_global_var) / sizeof(g_DataType_ecalc_global_var[0]);

#endif

