#ifndef __INCLUDE_SPEC_HEADER_H__
#define __INCLUDE_SPEC_HEADER_H__
#include "elib/lib2.h"
#include "elib/lang.h"
#include "elib/krnllib.h"
#include "spec_cmd_typedef.h"

#define SPEC_EXTERN_C EXTERN_C

typedef struct CONTROL_STRUCT_BASE
{
    WNDPROC             oldProc;        // 窗口原过程, 一般情况下自己创建的组件都需要子类化, 这里记录组件子类化的原过程
    HFONT               hFont;          // 字体
    HWND                hDesignWnd;     // 如果blInDesignMode为真, 则hDesignWnd是设计窗口的窗口句柄, 否则为0
    HWND                hWndParent;     // 组件父窗口句柄
    HWND                hWnd;           // 组件句柄
    DWORD               hUnit;          // 易语言返回的一个CWnd类指针
    UINT                id;             // 组件的ID
    DWORD               dwWinFormID;    // 本窗口单元所在窗口及本身的ID, 用作通知到系统
    DWORD               dwUnitID;       // 本窗口单元所在窗口及本身的ID, 用作通知到系统。
    BOOL                blInDesignMode; // 是否被易语言IDE调用以进行可视化设计, 运行时为假
    LONG                flags;          // 组件的一些标志, 自己组件自己定义
    DWORD               style;          // 组件的样式, 在 WM_STYLECHANGED 里修改
    DWORD               styleEx;        // 组件的扩展样式, 在 WM_STYLECHANGED 里修改
    DWORD               styleCtl;       // 组件专属扩展样式
}*PCONTROL_STRUCT_BASE;

// 组件统一处理消息函数, 会记录组件的一些基础信息, 返回组件base结构指针
template<typename T = PCONTROL_STRUCT_BASE>
inline T DefControlProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LPCWSTR propName, WNDPROC& oldProc)
{
    // 如果是自己存放数据的话, 这里改成获取自己的数据
    T pBase = (T)GetPropW(hWnd, propName);
    if ( !pBase ) return 0;
    oldProc = pBase->oldProc;
    switch ( message )
    {
    case WM_STYLECHANGED:
    {
        // 记录窗口的样式
        LPSTYLESTRUCT ss = (LPSTYLESTRUCT)lParam;
        if ( wParam == GWL_STYLE )
            pBase->style = ss->styleNew;
        else
            pBase->styleEx = ss->styleNew;
        break;
    }
    case WM_DESTROY:
    {
        // 如果是自己存放数据的话, 这里改成删除自己的数据
        RemovePropW(hWnd, propName);
        // 取消子类化, 不取消的话关闭窗口会崩溃
        SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)pBase->oldProc);
        delete pBase;
        pBase = 0;
        break;
    }
    // 这里可以处理更多通用的消息, 比如 WM_WINDOWPOSCHANGED 把窗口位置记录起来
    // 有或者 WM_SETFOCUS/WM_KILLFOCUS, 把窗口是否有焦点记录起来
    // 更多的通用消息自行扩展
    default:
        break;
    }
    return pBase;
}
#ifndef __E_STATIC_LIB
extern LIB_CONST_INFO g_ConstInfo_spec_global_var[];
extern int g_ConstInfo_spec_global_var_count;
extern CMD_INFO g_cmdInfo_spec_global_var[];
extern PFN_EXECUTE_CMD g_cmdInfo_spec_global_var_fun[];
extern int g_cmdInfo_spec_global_var_count;
extern ARG_INFO g_argumentInfo_spec_global_var[];
extern LIB_DATA_TYPE_INFO g_DataType_spec_global_var[];
extern int g_DataType_spec_global_var_count;
#endif

#define SPEC_DEF_CMD(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    EXTERN_C void SPEC_NAME(_index, _szEgName)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
SPEC_DEF(SPEC_DEF_CMD) // 所有实现命令的声明

#endif
