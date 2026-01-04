#ifndef __E_STATIC_LIB
#include "include_edatastructure_header.h"

// 节点  下的方法索引
static INT s_dtCmdIndexedatastructure_ENode_static_var_00[] = 
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
    10, 11, 12, 
};
static int s_dtCmdIndexedatastructure_ENode_static_var_count_00 = sizeof(s_dtCmdIndexedatastructure_ENode_static_var_00) / sizeof(s_dtCmdIndexedatastructure_ENode_static_var_00[0]);

// 链表  下的方法索引
static INT s_dtCmdIndexedatastructure_EList_static_var_01[] = 
{
    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 
    23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 
    33, 
};
static int s_dtCmdIndexedatastructure_EList_static_var_count_01 = sizeof(s_dtCmdIndexedatastructure_EList_static_var_01) / sizeof(s_dtCmdIndexedatastructure_EList_static_var_01[0]);

// 栈  下的方法索引
static INT s_dtCmdIndexedatastructure_EStack_static_var_02[] = 
{
    34, 35, 36, 37, 38, 39, 40, 41, 42, 
};
static int s_dtCmdIndexedatastructure_EStack_static_var_count_02 = sizeof(s_dtCmdIndexedatastructure_EStack_static_var_02) / sizeof(s_dtCmdIndexedatastructure_EStack_static_var_02[0]);

// 队列  下的方法索引
static INT s_dtCmdIndexedatastructure_EQueue_static_var_03[] = 
{
    43, 44, 45, 46, 47, 48, 49, 50, 51, 
};
static int s_dtCmdIndexedatastructure_EQueue_static_var_count_03 = sizeof(s_dtCmdIndexedatastructure_EQueue_static_var_03) / sizeof(s_dtCmdIndexedatastructure_EQueue_static_var_03[0]);

// 二叉树  下的方法索引
static INT s_dtCmdIndexedatastructure_EBiTree_static_var_04[] = 
{
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 
    62, 63, 64, 65, 66, 67, 68, 69, 70, 
};
static int s_dtCmdIndexedatastructure_EBiTree_static_var_count_04 = sizeof(s_dtCmdIndexedatastructure_EBiTree_static_var_04) / sizeof(s_dtCmdIndexedatastructure_EBiTree_static_var_04[0]);

// 树  下的方法索引
static INT s_dtCmdIndexedatastructure_ETree_static_var_05[] = 
{
    71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 
    81, 82, 83, 84, 85, 86, 
};
static int s_dtCmdIndexedatastructure_ETree_static_var_count_05 = sizeof(s_dtCmdIndexedatastructure_ETree_static_var_05) / sizeof(s_dtCmdIndexedatastructure_ETree_static_var_05[0]);

// 表  下的方法索引
static INT s_dtCmdIndexedatastructure_Map_static_var_06[] = 
{
    88, 89, 90, 91, 92, 93, 94, 95, 96, 
};
static int s_dtCmdIndexedatastructure_Map_static_var_count_06 = sizeof(s_dtCmdIndexedatastructure_Map_static_var_06) / sizeof(s_dtCmdIndexedatastructure_Map_static_var_06[0]);

// 节点  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventedatastructure_ENode_static_var_00[] = 
{
    /*000*/ {SDT_INT, 0, "节点的指针", "NodePointer", "无", LES_HIDED, (INT)0x00000000},

};
static int s_objEventedatastructure_ENode_static_var_count_00 = sizeof(s_objEventedatastructure_ENode_static_var_00) / sizeof(s_objEventedatastructure_ENode_static_var_00[0]);

// 链表  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventedatastructure_EList_static_var_01[] = 
{
    /*000*/ {SDT_INT, 0, "链表当前节点的指针", "CurrrentNodePointer", "无", LES_HIDED, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "链表中节点的数量", "ListNodeCount", "无", LES_HIDED, (INT)0x00000000},

};
static int s_objEventedatastructure_EList_static_var_count_01 = sizeof(s_objEventedatastructure_EList_static_var_01) / sizeof(s_objEventedatastructure_EList_static_var_01[0]);

// 栈  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventedatastructure_EStack_static_var_02[] = 
{
    /*000*/ {SDT_INT, 0, "栈顶指针", "StackTopPointer", "无", LES_HIDED, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "栈中节点的数量", "StackNodeCount", "无", LES_HIDED, (INT)0x00000000},

};
static int s_objEventedatastructure_EStack_static_var_count_02 = sizeof(s_objEventedatastructure_EStack_static_var_02) / sizeof(s_objEventedatastructure_EStack_static_var_02[0]);

// 队列  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventedatastructure_EQueue_static_var_03[] = 
{
    /*000*/ {SDT_INT, 0, "队列首指针", "QueueTopPointer", "无", LES_HIDED, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "队列尾指针", "QueueTailPointer", "无", LES_HIDED, (INT)0x00000000},
    /*002*/ {SDT_INT, 0, "队列中节点的数量", "QueueNodeCount", "无", NULL, (INT)0x00000000},

};
static int s_objEventedatastructure_EQueue_static_var_count_03 = sizeof(s_objEventedatastructure_EQueue_static_var_03) / sizeof(s_objEventedatastructure_EQueue_static_var_03[0]);

// 二叉树  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventedatastructure_EBiTree_static_var_04[] = 
{
    /*000*/ {SDT_INT, 0, "二叉树当前节点指针", "BiTreeCurNodePtr", "无", LES_HIDED, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "二叉树根节点指针", "BiTreeRootPtr", "无", LES_HIDED, (INT)0x00000000},

};
static int s_objEventedatastructure_EBiTree_static_var_count_04 = sizeof(s_objEventedatastructure_EBiTree_static_var_04) / sizeof(s_objEventedatastructure_EBiTree_static_var_04[0]);

// 树  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventedatastructure_ETree_static_var_05[] = 
{
    /*000*/ {SDT_INT, 0, "树当前节点指针", "TreeCurNodePtr", "无", LES_HIDED, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "树根节点指针", "TreeRootPtr", "无", LES_HIDED, (INT)0x00000000},

};
static int s_objEventedatastructure_ETree_static_var_count_05 = sizeof(s_objEventedatastructure_ETree_static_var_05) / sizeof(s_objEventedatastructure_ETree_static_var_05[0]);

// 表  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventedatastructure_Map_static_var_06[] = 
{
    /*000*/ {SDT_INT, 0, NULL, "Map", NULL, LES_HIDED, (INT)0x00000000},

};
static int s_objEventedatastructure_Map_static_var_count_06 = sizeof(s_objEventedatastructure_Map_static_var_06) / sizeof(s_objEventedatastructure_Map_static_var_06[0]);

LIB_DATA_TYPE_INFO g_DataType_edatastructure_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "节点", "ENode", "该数据类型是一种带有属性数据的节点数据类型，一个节点可以拥有零个或多个属性数据",
            s_dtCmdIndexedatastructure_ENode_static_var_count_00, s_dtCmdIndexedatastructure_ENode_static_var_00, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventedatastructure_ENode_static_var_count_00, s_objEventedatastructure_ENode_static_var_00
        },
/*001*/ { "链表", "EList", "该数据类型是一种数据容器，其中可以保存“节点”型数据，在内存中以数据链形式存在",
            s_dtCmdIndexedatastructure_EList_static_var_count_01, s_dtCmdIndexedatastructure_EList_static_var_01, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventedatastructure_EList_static_var_count_01, s_objEventedatastructure_EList_static_var_01
        },
/*002*/ { "栈", "EStack", "该数据类型是一种数据容器，其中可以保存“节点”型数据，该容器以“后进先出”的顺序存取数据",
            s_dtCmdIndexedatastructure_EStack_static_var_count_02, s_dtCmdIndexedatastructure_EStack_static_var_02, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventedatastructure_EStack_static_var_count_02, s_objEventedatastructure_EStack_static_var_02
        },
/*003*/ { "队列", "EQueue", "该数据类型是一种数据容器，其中可以保存“节点”型数据，该容器以“先进先出”的顺序存取数据",
            s_dtCmdIndexedatastructure_EQueue_static_var_count_03, s_dtCmdIndexedatastructure_EQueue_static_var_03, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventedatastructure_EQueue_static_var_count_03, s_objEventedatastructure_EQueue_static_var_03
        },
/*004*/ { "二叉树", "EBiTree", "该数据类型是一种数据容器，其中可以保存“节点”型数据，该容器以二叉树结构存取数据。该二叉树是根据每个节点的键值组织成为二叉排序树，该二叉树的上的任何一个节点的键值一定大于它的左子树节点的键值并且小于右子树节点的键值",
            s_dtCmdIndexedatastructure_EBiTree_static_var_count_04, s_dtCmdIndexedatastructure_EBiTree_static_var_04, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventedatastructure_EBiTree_static_var_count_04, s_objEventedatastructure_EBiTree_static_var_04
        },
/*005*/ { "树", "ETree", "该数据类型是一种数据容器，其中可以保存“节点”型数据，该容器以普通树结构存取数据",
            s_dtCmdIndexedatastructure_ETree_static_var_count_05, s_dtCmdIndexedatastructure_ETree_static_var_05, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventedatastructure_ETree_static_var_count_05, s_objEventedatastructure_ETree_static_var_05
        },
/*006*/ { "表", "Map", "该数据类型是一种数据容器，其数据为多个“键-值”对，其中“键”为字节集类型，“值”为“节点”数据。表（Map）的主要用途是在大批量数据中快速查找相应的数据（根据“键”查找“值”）。",
            s_dtCmdIndexedatastructure_Map_static_var_count_06, s_dtCmdIndexedatastructure_Map_static_var_06, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventedatastructure_Map_static_var_count_06, s_objEventedatastructure_Map_static_var_06
        },
};
int g_DataType_edatastructure_global_var_count = sizeof(g_DataType_edatastructure_global_var) / sizeof(g_DataType_edatastructure_global_var[0]);

#endif

