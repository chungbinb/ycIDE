/**
 * @file editbox.cpp
 * @brief 编辑框组件实现
 * @date 2026-01-18
 */

#define _CRT_SECURE_NO_WARNINGS

#include "../include_krnln_header.h"
#include "editbox.h"
#include <stdlib.h>
#include <string.h>
#include <windowsx.h>

// 前向声明
static LRESULT CALLBACK EditBox_SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ========== 创建编辑框 ==========
HUNIT WINAPI EditBox_Create(PUNIT_CREATE_PARAMS pParams)
{
    if (!pParams) return NULL;
    
    // 分配编辑框数据
    PEDITBOX_DATA pEdit = (PEDITBOX_DATA)Unit_Alloc(sizeof(EDITBOX_DATA) - sizeof(WIN_UNIT_BASE));
    if (!pEdit) return NULL;
    
    // 初始化基础数据
    pEdit->base.dwFormID = pParams->dwFormID;
    pEdit->base.dwUnitID = pParams->dwUnitID;
    pEdit->base.hParent = pParams->hParentWnd;
    
    // 设置默认值
    pEdit->clrBack = RGB(255, 255, 255);  // 白色背景
    pEdit->clrFore = RGB(0, 0, 0);        // 黑色文字
    pEdit->nBorderStyle = EDITBOX_BORDER_FIXED3D;
    pEdit->nAlign = EDITBOX_ALIGN_LEFT;
    pEdit->nScrollBar = EDITBOX_SCROLLBAR_NONE;
    pEdit->nMaxLen = 0;  // 0表示无限制
    pEdit->nInputMode = EDITBOX_MODE_ALL;
    pEdit->wchPwdChar = L'*';
    pEdit->bMultiLine = FALSE;
    pEdit->bPassword = FALSE;
    pEdit->bReadOnly = FALSE;
    pEdit->bHideSel = TRUE;
    
    // 从属性数据读取属性
    if (pParams->pAllData && pParams->nAllDataSize > 0)
    {
        LPSTR pszContent = NULL;
        if (Unit_ReadPropText(pParams->pAllData, pParams->nAllDataSize, PROP_EDITBOX_CONTENT, &pszContent))
        {
            pEdit->pszContent = Unit_AnsiToWide(pszContent, NULL);
            free(pszContent);
        }
        
        INT nValue;
        if (Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_EDITBOX_BORDERSTYLE, &nValue))
            pEdit->nBorderStyle = nValue;
        if (Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_EDITBOX_ALIGN, &nValue))
            pEdit->nAlign = nValue;
        if (Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_EDITBOX_SCROLLBAR, &nValue))
            pEdit->nScrollBar = nValue;
        if (Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_EDITBOX_MAXLEN, &nValue))
            pEdit->nMaxLen = nValue;
        if (Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_EDITBOX_INPUTMODE, &nValue))
            pEdit->nInputMode = nValue;
        
        BOOL bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_EDITBOX_MULTILINE, &bValue))
            pEdit->bMultiLine = bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_EDITBOX_PASSWORD, &bValue))
            pEdit->bPassword = bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_EDITBOX_READONLY, &bValue))
            pEdit->bReadOnly = bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_EDITBOX_HIDESEL, &bValue))
            pEdit->bHideSel = bValue;
        
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_EDITBOX_VISIBLE, &bValue))
        {
            if (bValue)
                pEdit->base.dwState |= UNIT_STATE_VISIBLE;
        }
        else
        {
            pEdit->base.dwState |= UNIT_STATE_VISIBLE;
        }
        
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_EDITBOX_DISABLED, &bValue))
        {
            if (bValue)
                pEdit->base.dwState |= UNIT_STATE_DISABLED;
        }
        
        COLORREF clr;
        if (Unit_ReadPropColor(pParams->pAllData, pParams->nAllDataSize, PROP_EDITBOX_BACKCOLOR, &clr))
            pEdit->clrBack = clr;
        if (Unit_ReadPropColor(pParams->pAllData, pParams->nAllDataSize, PROP_EDITBOX_FORECOLOR, &clr))
            pEdit->clrFore = clr;
        
        LOGFONTW lf;
        if (Unit_ReadPropFont(pParams->pAllData, pParams->nAllDataSize, PROP_EDITBOX_FONT, &lf))
            pEdit->hFont = Unit_CreateFont(&lf);
    }
    else
    {
        pEdit->base.dwState |= UNIT_STATE_VISIBLE;
    }
    
    // 创建背景画刷
    pEdit->hBackBrush = CreateSolidBrush(pEdit->clrBack);
    
    // 计算编辑框样式
    DWORD dwStyle = WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL;
    DWORD dwExStyle = 0;
    
    if (pEdit->base.dwState & UNIT_STATE_VISIBLE)
        dwStyle |= WS_VISIBLE;
    if (pEdit->base.dwState & UNIT_STATE_DISABLED)
        dwStyle |= WS_DISABLED;
    
    // 边框样式
    if (pEdit->nBorderStyle == EDITBOX_BORDER_SINGLE)
    {
        dwStyle |= WS_BORDER;
    }
    else if (pEdit->nBorderStyle == EDITBOX_BORDER_FIXED3D)
    {
        dwExStyle |= WS_EX_CLIENTEDGE;
    }
    
    // 对齐方式
    switch (pEdit->nAlign)
    {
    case EDITBOX_ALIGN_CENTER:
        dwStyle |= ES_CENTER;
        break;
    case EDITBOX_ALIGN_RIGHT:
        dwStyle |= ES_RIGHT;
        break;
    default:
        dwStyle |= ES_LEFT;
        break;
    }
    
    // 多行
    if (pEdit->bMultiLine)
    {
        dwStyle |= ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN;
        dwStyle &= ~ES_AUTOHSCROLL;
    }
    
    // 滚动条
    if (pEdit->nScrollBar & EDITBOX_SCROLLBAR_HORIZ)
        dwStyle |= WS_HSCROLL;
    if (pEdit->nScrollBar & EDITBOX_SCROLLBAR_VERT)
        dwStyle |= WS_VSCROLL;
    
    // 密码输入
    if (pEdit->bPassword && !pEdit->bMultiLine)
        dwStyle |= ES_PASSWORD;
    
    // 只读
    if (pEdit->bReadOnly)
        dwStyle |= ES_READONLY;
    
    // 隐藏选择
    if (!pEdit->bHideSel)
        dwStyle |= ES_NOHIDESEL;
    
    // 数字输入模式
    if (pEdit->nInputMode == EDITBOX_MODE_NUMBER)
        dwStyle |= ES_NUMBER;
    
    // 创建Windows编辑框
    int x = pParams->x;
    int y = pParams->y;
    int cx = pParams->cx > 0 ? pParams->cx : 120;
    int cy = pParams->cy > 0 ? pParams->cy : (pEdit->bMultiLine ? 80 : 20);
    
    HWND hWnd = CreateWindowExW(
        dwExStyle,
        L"EDIT",
        pEdit->pszContent ? pEdit->pszContent : L"",
        dwStyle,
        x, y, cx, cy,
        pParams->hParentWnd,
        (HMENU)(UINT_PTR)pParams->uID,
        GetModuleHandle(NULL),
        NULL
    );
    
    if (!hWnd)
    {
        if (pEdit->hBackBrush) DeleteObject(pEdit->hBackBrush);
        Unit_Free((PWIN_UNIT_BASE)pEdit);
        return NULL;
    }
    
    pEdit->base.hWnd = hWnd;
    Unit_SetHWnd((PWIN_UNIT_BASE)pEdit, hWnd);
    
    // 子类化编辑框
    pEdit->base.pfnOldProc = (WNDPROC)SetWindowLongPtrW(
        hWnd, GWLP_WNDPROC, (LONG_PTR)EditBox_SubclassProc);
    
    // 存储编辑框数据到窗口
    SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)pEdit);
    
    // 设置字体
    if (pEdit->hFont)
    {
        SendMessage(hWnd, WM_SETFONT, (WPARAM)pEdit->hFont, TRUE);
    }
    else
    {
        HFONT hDefaultFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        SendMessage(hWnd, WM_SETFONT, (WPARAM)hDefaultFont, TRUE);
    }
    
    // 设置最大长度
    if (pEdit->nMaxLen > 0)
    {
        SendMessage(hWnd, EM_SETLIMITTEXT, (WPARAM)pEdit->nMaxLen, 0);
    }
    
    // 设置密码字符
    if (pEdit->bPassword && !pEdit->bMultiLine)
    {
        SendMessage(hWnd, EM_SETPASSWORDCHAR, (WPARAM)pEdit->wchPwdChar, 0);
    }
    
    return (HUNIT)pEdit;
}

// ========== 销毁编辑框 ==========
BOOL WINAPI EditBox_Destroy(HUNIT hUnit)
{
    PEDITBOX_DATA pEdit = (PEDITBOX_DATA)hUnit;
    if (!pEdit) return FALSE;
    
    // 取消子类化
    if (pEdit->base.hWnd && pEdit->base.pfnOldProc)
    {
        SetWindowLongPtrW(pEdit->base.hWnd, GWLP_WNDPROC, (LONG_PTR)pEdit->base.pfnOldProc);
    }
    
    // 销毁Windows窗口
    if (pEdit->base.hWnd && IsWindow(pEdit->base.hWnd))
    {
        DestroyWindow(pEdit->base.hWnd);
    }
    
    // 释放资源
    if (pEdit->pszContent)
        free(pEdit->pszContent);
    if (pEdit->hFont)
        DeleteObject(pEdit->hFont);
    if (pEdit->hBackBrush)
        DeleteObject(pEdit->hBackBrush);
    
    Unit_Free((PWIN_UNIT_BASE)pEdit);
    return TRUE;
}

// ========== 子类化过程 ==========
static LRESULT CALLBACK EditBox_SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PEDITBOX_DATA pEdit = (PEDITBOX_DATA)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
    if (!pEdit || !pEdit->base.pfnOldProc)
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    
    switch (uMsg)
    {
    case WM_CHAR:
        // 输入模式过滤
        if (pEdit->nInputMode == EDITBOX_MODE_NUMBER)
        {
            // 只允许数字和控制字符
            if (wParam >= L'0' && wParam <= L'9')
                break;
            if (wParam < 32 || wParam == 127)  // 控制字符
                break;
            return 0;  // 过滤掉其他字符
        }
        else if (pEdit->nInputMode == EDITBOX_MODE_FLOAT)
        {
            // 允许数字、小数点、负号
            if (wParam >= L'0' && wParam <= L'9')
                break;
            if (wParam == L'.' || wParam == L'-')
                break;
            if (wParam < 32 || wParam == 127)
                break;
            return 0;
        }
        else if (pEdit->nInputMode == EDITBOX_MODE_ALPHA)
        {
            // 只允许英文字母
            if ((wParam >= L'A' && wParam <= L'Z') || (wParam >= L'a' && wParam <= L'z'))
                break;
            if (wParam < 32 || wParam == 127)
                break;
            return 0;
        }
        
        Unit_FireEvent(&pEdit->base, EVENT_CHAR, 1, (INT)wParam);
        break;
        
    case WM_KEYDOWN:
        Unit_FireEvent(&pEdit->base, EVENT_KEYDOWN, 1, (INT)wParam);
        break;
        
    case WM_KEYUP:
        Unit_FireEvent(&pEdit->base, EVENT_KEYUP, 1, (INT)wParam);
        break;
        
    case WM_LBUTTONDOWN:
        Unit_FireEvent(&pEdit->base, EVENT_MOUSEDOWN, 3,
            LOWORD(lParam), HIWORD(lParam), (INT)wParam);
        break;
        
    case WM_LBUTTONUP:
        Unit_FireEvent(&pEdit->base, EVENT_MOUSEUP, 3,
            LOWORD(lParam), HIWORD(lParam), (INT)wParam);
        break;
        
    case WM_MOUSEMOVE:
        Unit_FireEvent(&pEdit->base, EVENT_MOUSEMOVE, 3,
            LOWORD(lParam), HIWORD(lParam), (INT)wParam);
        break;
        
    case WM_SETFOCUS:
        pEdit->base.dwState |= UNIT_STATE_FOCUSED;
        Unit_FireEvent(&pEdit->base, EVENT_GOTFOCUS, 0);
        break;
        
    case WM_KILLFOCUS:
        pEdit->base.dwState &= ~UNIT_STATE_FOCUSED;
        Unit_FireEvent(&pEdit->base, EVENT_LOSTFOCUS, 0);
        break;
        
    case WM_DESTROY:
        Unit_FireEvent(&pEdit->base, EVENT_DESTROY, 0);
        break;
    }
    
    // 处理反射消息
    LRESULT lResult = CallWindowProcW(pEdit->base.pfnOldProc, hWnd, uMsg, wParam, lParam);
    
    // 内容改变通知
    if (uMsg == WM_COMMAND && HIWORD(wParam) == EN_CHANGE)
    {
        Unit_FireEvent(&pEdit->base, EVENT_EDITBOX_CHANGE, 0);
    }
    
    return lResult;
}

// ========== 编辑框命令实现 ==========

void WINAPI EditBox_SetContent(PEDITBOX_DATA pEdit, LPCWSTR pszContent)
{
    if (pEdit)
    {
        if (pEdit->pszContent)
            free(pEdit->pszContent);
        pEdit->pszContent = pszContent ? _wcsdup(pszContent) : NULL;
        
        if (pEdit->base.hWnd)
        {
            SetWindowTextW(pEdit->base.hWnd, pszContent ? pszContent : L"");
        }
    }
}

LPWSTR WINAPI EditBox_GetContent(PEDITBOX_DATA pEdit)
{
    if (!pEdit || !pEdit->base.hWnd) return NULL;
    
    INT nLen = GetWindowTextLengthW(pEdit->base.hWnd);
    if (nLen <= 0) return _wcsdup(L"");
    
    LPWSTR pszText = (LPWSTR)malloc((nLen + 1) * sizeof(WCHAR));
    if (pszText)
    {
        GetWindowTextW(pEdit->base.hWnd, pszText, nLen + 1);
    }
    return pszText;
}

void WINAPI EditBox_AppendContent(PEDITBOX_DATA pEdit, LPCWSTR pszText)
{
    if (pEdit && pEdit->base.hWnd && pszText)
    {
        // 移动光标到末尾
        INT nLen = GetWindowTextLengthW(pEdit->base.hWnd);
        SendMessage(pEdit->base.hWnd, EM_SETSEL, nLen, nLen);
        // 替换选中内容（即追加）
        SendMessageW(pEdit->base.hWnd, EM_REPLACESEL, FALSE, (LPARAM)pszText);
    }
}

void WINAPI EditBox_Clear(PEDITBOX_DATA pEdit)
{
    if (pEdit && pEdit->base.hWnd)
    {
        SetWindowTextW(pEdit->base.hWnd, L"");
    }
}

void WINAPI EditBox_SetSel(PEDITBOX_DATA pEdit, INT nStart, INT nEnd)
{
    if (pEdit && pEdit->base.hWnd)
    {
        SendMessage(pEdit->base.hWnd, EM_SETSEL, nStart, nEnd);
    }
}

void WINAPI EditBox_GetSel(PEDITBOX_DATA pEdit, INT* pnStart, INT* pnEnd)
{
    if (pEdit && pEdit->base.hWnd)
    {
        DWORD dwStart = 0, dwEnd = 0;
        SendMessage(pEdit->base.hWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
        if (pnStart) *pnStart = (INT)dwStart;
        if (pnEnd) *pnEnd = (INT)dwEnd;
    }
}

LPWSTR WINAPI EditBox_GetSelText(PEDITBOX_DATA pEdit)
{
    if (!pEdit || !pEdit->base.hWnd) return NULL;
    
    DWORD dwStart = 0, dwEnd = 0;
    SendMessage(pEdit->base.hWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
    
    if (dwStart >= dwEnd) return _wcsdup(L"");
    
    // 获取全部文本
    INT nLen = GetWindowTextLengthW(pEdit->base.hWnd);
    LPWSTR pszAll = (LPWSTR)malloc((nLen + 1) * sizeof(WCHAR));
    if (!pszAll) return NULL;
    
    GetWindowTextW(pEdit->base.hWnd, pszAll, nLen + 1);
    
    // 提取选中部分
    INT nSelLen = dwEnd - dwStart;
    LPWSTR pszSel = (LPWSTR)malloc((nSelLen + 1) * sizeof(WCHAR));
    if (pszSel)
    {
        wcsncpy(pszSel, pszAll + dwStart, nSelLen);
        pszSel[nSelLen] = L'\0';
    }
    
    free(pszAll);
    return pszSel;
}

void WINAPI EditBox_ReplaceSel(PEDITBOX_DATA pEdit, LPCWSTR pszText)
{
    if (pEdit && pEdit->base.hWnd)
    {
        SendMessageW(pEdit->base.hWnd, EM_REPLACESEL, TRUE, (LPARAM)(pszText ? pszText : L""));
    }
}

INT WINAPI EditBox_GetLineCount(PEDITBOX_DATA pEdit)
{
    if (pEdit && pEdit->base.hWnd)
    {
        return (INT)SendMessage(pEdit->base.hWnd, EM_GETLINECOUNT, 0, 0);
    }
    return 0;
}

INT WINAPI EditBox_GetLine(PEDITBOX_DATA pEdit, INT nLine, LPWSTR pszBuffer, INT nBufferSize)
{
    if (!pEdit || !pEdit->base.hWnd || !pszBuffer || nBufferSize <= 0)
        return 0;
    
    // 第一个WORD指定缓冲区大小
    *(WORD*)pszBuffer = (WORD)nBufferSize;
    return (INT)SendMessageW(pEdit->base.hWnd, EM_GETLINE, nLine, (LPARAM)pszBuffer);
}

void WINAPI EditBox_ScrollCaret(PEDITBOX_DATA pEdit)
{
    if (pEdit && pEdit->base.hWnd)
    {
        SendMessage(pEdit->base.hWnd, EM_SCROLLCARET, 0, 0);
    }
}

BOOL WINAPI EditBox_CanUndo(PEDITBOX_DATA pEdit)
{
    if (pEdit && pEdit->base.hWnd)
    {
        return (BOOL)SendMessage(pEdit->base.hWnd, EM_CANUNDO, 0, 0);
    }
    return FALSE;
}

void WINAPI EditBox_Undo(PEDITBOX_DATA pEdit)
{
    if (pEdit && pEdit->base.hWnd)
    {
        SendMessage(pEdit->base.hWnd, EM_UNDO, 0, 0);
    }
}

void WINAPI EditBox_Cut(PEDITBOX_DATA pEdit)
{
    if (pEdit && pEdit->base.hWnd)
    {
        SendMessage(pEdit->base.hWnd, WM_CUT, 0, 0);
    }
}

void WINAPI EditBox_Copy(PEDITBOX_DATA pEdit)
{
    if (pEdit && pEdit->base.hWnd)
    {
        SendMessage(pEdit->base.hWnd, WM_COPY, 0, 0);
    }
}

void WINAPI EditBox_Paste(PEDITBOX_DATA pEdit)
{
    if (pEdit && pEdit->base.hWnd)
    {
        SendMessage(pEdit->base.hWnd, WM_PASTE, 0, 0);
    }
}

void WINAPI EditBox_SelectAll(PEDITBOX_DATA pEdit)
{
    if (pEdit && pEdit->base.hWnd)
    {
        SendMessage(pEdit->base.hWnd, EM_SETSEL, 0, -1);
    }
}

// ========== 属性操作 ==========

BOOL WINAPI EditBox_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue)
{
    PEDITBOX_DATA pEdit = (PEDITBOX_DATA)hUnit;
    if (!pEdit || !pValue) return FALSE;
    
    RECT rc = {0};
    if (pEdit->base.hWnd)
    {
        GetWindowRect(pEdit->base.hWnd, &rc);
        MapWindowPoints(HWND_DESKTOP, pEdit->base.hParent, (LPPOINT)&rc, 2);
    }
    
    switch (nIndex)
    {
    case PROP_EDITBOX_LEFT:
        pValue->m_int = rc.left;
        return TRUE;
    case PROP_EDITBOX_TOP:
        pValue->m_int = rc.top;
        return TRUE;
    case PROP_EDITBOX_WIDTH:
        pValue->m_int = rc.right - rc.left;
        return TRUE;
    case PROP_EDITBOX_HEIGHT:
        pValue->m_int = rc.bottom - rc.top;
        return TRUE;
    case PROP_EDITBOX_VISIBLE:
        pValue->m_bool = (pEdit->base.dwState & UNIT_STATE_VISIBLE) != 0;
        return TRUE;
    case PROP_EDITBOX_DISABLED:
        pValue->m_bool = (pEdit->base.dwState & UNIT_STATE_DISABLED) != 0;
        return TRUE;
    case PROP_EDITBOX_CONTENT:
        {
            LPWSTR pszContent = EditBox_GetContent(pEdit);
            pValue->m_szText = pszContent ? Unit_WideToAnsi(pszContent, NULL) : NULL;
            free(pszContent);
        }
        return TRUE;
    case PROP_EDITBOX_BORDERSTYLE:
        pValue->m_int = pEdit->nBorderStyle;
        return TRUE;
    case PROP_EDITBOX_ALIGN:
        pValue->m_int = pEdit->nAlign;
        return TRUE;
    case PROP_EDITBOX_MULTILINE:
        pValue->m_bool = pEdit->bMultiLine;
        return TRUE;
    case PROP_EDITBOX_PASSWORD:
        pValue->m_bool = pEdit->bPassword;
        return TRUE;
    case PROP_EDITBOX_READONLY:
        pValue->m_bool = pEdit->bReadOnly;
        return TRUE;
    case PROP_EDITBOX_MAXLEN:
        pValue->m_int = pEdit->nMaxLen;
        return TRUE;
    case PROP_EDITBOX_BACKCOLOR:
        pValue->m_clr = pEdit->clrBack;
        return TRUE;
    case PROP_EDITBOX_FORECOLOR:
        pValue->m_clr = pEdit->clrFore;
        return TRUE;
    default:
        return FALSE;
    }
}

BOOL WINAPI EditBox_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip)
{
    PEDITBOX_DATA pEdit = (PEDITBOX_DATA)hUnit;
    if (!pEdit || !pValue) return FALSE;
    
    switch (nIndex)
    {
    case PROP_EDITBOX_LEFT:
    case PROP_EDITBOX_TOP:
    case PROP_EDITBOX_WIDTH:
    case PROP_EDITBOX_HEIGHT:
        if (pEdit->base.hWnd)
        {
            RECT rc;
            GetWindowRect(pEdit->base.hWnd, &rc);
            MapWindowPoints(HWND_DESKTOP, pEdit->base.hParent, (LPPOINT)&rc, 2);
            
            if (nIndex == PROP_EDITBOX_LEFT)
                rc.left = pValue->m_int;
            else if (nIndex == PROP_EDITBOX_TOP)
                rc.top = pValue->m_int;
            else if (nIndex == PROP_EDITBOX_WIDTH)
                rc.right = rc.left + pValue->m_int;
            else
                rc.bottom = rc.top + pValue->m_int;
            
            SetWindowPos(pEdit->base.hWnd, NULL,
                rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
                SWP_NOZORDER);
        }
        return FALSE;
        
    case PROP_EDITBOX_VISIBLE:
        if (pValue->m_bool)
        {
            pEdit->base.dwState |= UNIT_STATE_VISIBLE;
            if (pEdit->base.hWnd)
                ShowWindow(pEdit->base.hWnd, SW_SHOW);
        }
        else
        {
            pEdit->base.dwState &= ~UNIT_STATE_VISIBLE;
            if (pEdit->base.hWnd)
                ShowWindow(pEdit->base.hWnd, SW_HIDE);
        }
        return FALSE;
        
    case PROP_EDITBOX_DISABLED:
        if (pValue->m_bool)
            pEdit->base.dwState |= UNIT_STATE_DISABLED;
        else
            pEdit->base.dwState &= ~UNIT_STATE_DISABLED;
        if (pEdit->base.hWnd)
            EnableWindow(pEdit->base.hWnd, !pValue->m_bool);
        return FALSE;
        
    case PROP_EDITBOX_CONTENT:
        if (pValue->m_szText)
        {
            LPWSTR pszWide = Unit_AnsiToWide(pValue->m_szText, NULL);
            EditBox_SetContent(pEdit, pszWide);
            Unit_FreeString(pszWide);
        }
        else
        {
            EditBox_SetContent(pEdit, NULL);
        }
        return FALSE;
        
    case PROP_EDITBOX_READONLY:
        pEdit->bReadOnly = pValue->m_bool;
        if (pEdit->base.hWnd)
        {
            SendMessage(pEdit->base.hWnd, EM_SETREADONLY, pValue->m_bool, 0);
        }
        return FALSE;
        
    case PROP_EDITBOX_MAXLEN:
        pEdit->nMaxLen = pValue->m_int;
        if (pEdit->base.hWnd)
        {
            SendMessage(pEdit->base.hWnd, EM_SETLIMITTEXT, pValue->m_int, 0);
        }
        return FALSE;
        
    case PROP_EDITBOX_BACKCOLOR:
        pEdit->clrBack = pValue->m_clr;
        if (pEdit->hBackBrush)
            DeleteObject(pEdit->hBackBrush);
        pEdit->hBackBrush = CreateSolidBrush(pEdit->clrBack);
        if (pEdit->base.hWnd)
            InvalidateRect(pEdit->base.hWnd, NULL, TRUE);
        return FALSE;
        
    case PROP_EDITBOX_FORECOLOR:
        pEdit->clrFore = pValue->m_clr;
        if (pEdit->base.hWnd)
            InvalidateRect(pEdit->base.hWnd, NULL, TRUE);
        return FALSE;
        
    // 需要重建的属性
    case PROP_EDITBOX_BORDERSTYLE:
    case PROP_EDITBOX_ALIGN:
    case PROP_EDITBOX_MULTILINE:
    case PROP_EDITBOX_SCROLLBAR:
    case PROP_EDITBOX_PASSWORD:
        if (ppszTip)
            *ppszTip = "修改此属性需要重新创建组件";
        return TRUE;
        
    default:
        return FALSE;
    }
}

BOOL WINAPI EditBox_PropertyUpdateUI(HUNIT hUnit, INT nIndex)
{
    return TRUE;
}

// ========== 接口获取 ==========

static HUNIT WINAPI EditBox_Interface_Create(LPBYTE pAllData, INT nAllDataSize,
    DWORD dwStyle, HWND hParentWnd, UINT uID, HMENU hMenu, INT x, INT y, INT cx, INT cy,
    DWORD dwWinFormID, DWORD dwUnitID, HWND hDesignWnd, BOOL blInDesignMode)
{
    UNIT_CREATE_PARAMS params = {0};
    params.pAllData = pAllData;
    params.nAllDataSize = nAllDataSize;
    params.dwStyle = dwStyle;
    params.hParentWnd = hParentWnd;
    params.uID = uID;
    params.hMenu = hMenu;
    params.x = x;
    params.y = y;
    params.cx = cx;
    params.cy = cy;
    params.dwFormID = dwWinFormID;
    params.dwUnitID = dwUnitID;
    params.hDesignWnd = hDesignWnd;
    params.blInDesignMode = blInDesignMode;
    
    return EditBox_Create(&params);
}

PFN_INTERFACE WINAPI EditBox_GetInterface(INT nInterfaceNO)
{
    switch (nInterfaceNO)
    {
    case ITF_CREATE_UNIT:
        return (PFN_INTERFACE)EditBox_Interface_Create;
    case ITF_GET_PROPERTY_DATA:
        return (PFN_INTERFACE)EditBox_GetProperty;
    case ITF_NOTIFY_PROPERTY_CHANGED:
        return (PFN_INTERFACE)EditBox_SetProperty;
    case ITF_PROPERTY_UPDATE_UI:
        return (PFN_INTERFACE)EditBox_PropertyUpdateUI;
    default:
        return NULL;
    }
}
