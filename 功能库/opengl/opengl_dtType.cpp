#ifndef __E_STATIC_LIB
#include "include_opengl_header.h"

// 像素格式  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventopengl_PIXELFORMATDESCRIPTOR_static_var_00[] = 
{
    /*000*/ {SDT_SHORT, 0, "大小", "nSize", "当前版本的像素格式大小，固定为40。", NULL, (INT)0x00000000},
    /*001*/ {SDT_SHORT, 0, "版本", "nVersion", "当前像素格式的版本，固定为1。", NULL, (INT)0x00000000},
    /*002*/ {SDT_INT, 0, "标志", "dwFlags", "说明像素支持的格式。", NULL, (INT)0x00000000},
    /*003*/ {SDT_BYTE, 0, "像素类型", "iPixelType", "说明像素使用的类型，参数值可以为以下常量：0、红绿蓝混合通道模式；1、颜色索引模式，16色下有效。", NULL, (INT)0x00000000},
    /*004*/ {SDT_BYTE, 0, "色彩深度", "cColorBits", "说明像素选定的色彩位数为8位、16位、24位还是32位。", NULL, (INT)0x00000000},
    /*005*/ {SDT_BYTE, 0, "红色位", "cRedBits", NULL, NULL, (INT)0x00000000},
    /*006*/ {SDT_BYTE, 0, "红色转换位", "cRedShift", NULL, NULL, (INT)0x00000000},
    /*007*/ {SDT_BYTE, 0, "绿色位", "cGreenBits", NULL, NULL, (INT)0x00000000},
    /*008*/ {SDT_BYTE, 0, "绿色转换位", "cGreenShift", NULL, NULL, (INT)0x00000000},
    /*009*/ {SDT_BYTE, 0, "蓝色位", "cBlueBits", NULL, NULL, (INT)0x00000000},
    /*010*/ {SDT_BYTE, 0, "蓝色转换位", "cBlueShift", NULL, NULL, (INT)0x00000000},
    /*011*/ {SDT_BYTE, 0, "通道缓存", "cAlphaBits", NULL, NULL, (INT)0x00000000},
    /*012*/ {SDT_BYTE, 0, "通道转换位", "cAlphaShift", NULL, NULL, (INT)0x00000000},
    /*013*/ {SDT_BYTE, 0, "累计缓存位", "cAccumBits", "说明像素选定的累计缓存位数。", NULL, (INT)0x00000000},
    /*014*/ {SDT_BYTE, 0, "红色累计位", "cAccumRedBits", NULL, NULL, (INT)0x00000000},
    /*015*/ {SDT_BYTE, 0, "绿色累计位", "cAccumGreenBits", NULL, NULL, (INT)0x00000000},
    /*016*/ {SDT_BYTE, 0, "蓝色累计位", "cAccumBlueBits", NULL, NULL, (INT)0x00000000},
    /*017*/ {SDT_BYTE, 0, "通道累计位", "cAccumAlphaBits", NULL, NULL, (INT)0x00000000},
    /*018*/ {SDT_BYTE, 0, "cDepthBits", "cDepthBits", "说明像素选定的色彩深度为16位、24位还是32位", NULL, (INT)0x00000000},
    /*019*/ {SDT_BYTE, 0, "cStencilBits", "cStencilBits", "说明像素选定的模板缓存位数。", NULL, (INT)0x00000000},
    /*020*/ {SDT_BYTE, 0, "辅助缓存位", "cAuxBuffers", NULL, NULL, (INT)0x00000000},
    /*021*/ {SDT_BYTE, 0, "层类型", "iLayerType", NULL, NULL, (INT)0x00000000},
    /*022*/ {SDT_BYTE, 0, "保留位", "bReserved", NULL, NULL, (INT)0x00000000},
    /*023*/ {SDT_INT, 0, "遮罩层", "dwLayerMask", NULL, NULL, (INT)0x00000000},
    /*024*/ {SDT_INT, 0, "可视遮罩层", "dwVisibleMask", NULL, NULL, (INT)0x00000000},
    /*025*/ {SDT_INT, 0, "衰减遮罩层", "dwDamageMask", NULL, NULL, (INT)0x00000000},

};
static int s_objEventopengl_PIXELFORMATDESCRIPTOR_static_var_count_00 = sizeof(s_objEventopengl_PIXELFORMATDESCRIPTOR_static_var_00) / sizeof(s_objEventopengl_PIXELFORMATDESCRIPTOR_static_var_00[0]);

LIB_DATA_TYPE_INFO g_DataType_opengl_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "像素格式", "PIXELFORMATDESCRIPTOR", NULL,
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventopengl_PIXELFORMATDESCRIPTOR_static_var_count_00, s_objEventopengl_PIXELFORMATDESCRIPTOR_static_var_00
        },
};
int g_DataType_opengl_global_var_count = sizeof(g_DataType_opengl_global_var) / sizeof(g_DataType_opengl_global_var[0]);

#endif

