/**
 * @file control_base.cpp
 * @brief 窗口组件基础功能实现
 * @date 2026-01-18
 */

#include "../include_krnln_header.h"
#include "control_base.h"
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <map>
#include <mutex>

// 组件管理
static std::map<HWND, PWIN_UNIT_BASE> g_unitMap;
static std::mutex g_unitMapMutex;

// 全局通知函数指针（由易语言运行时设置）
static PFN_INTERFACE g_pfnNotifySystem = NULL;

// ============ 组件管理实现 ============

PWIN_UNIT_BASE Unit_Alloc(SIZE_T nExtraSize)
{
    SIZE_T totalSize = sizeof(WIN_UNIT_BASE) + nExtraSize;
    PWIN_UNIT_BASE pUnit = (PWIN_UNIT_BASE)calloc(1, totalSize);
    return pUnit;
}

void Unit_Free(PWIN_UNIT_BASE pUnit)
{
    if (pUnit)
    {
        // 从映射中移除
        if (pUnit->hWnd)
        {
            std::lock_guard<std::mutex> lock(g_unitMapMutex);
            g_unitMap.erase(pUnit->hWnd);
        }
        free(pUnit);
    }
}

PWIN_UNIT_BASE Unit_FromHWnd(HWND hWnd)
{
    std::lock_guard<std::mutex> lock(g_unitMapMutex);
    auto it = g_unitMap.find(hWnd);
    if (it != g_unitMap.end())
    {
        return it->second;
    }
    return NULL;
}

void Unit_SetHWnd(PWIN_UNIT_BASE pUnit, HWND hWnd)
{
    if (pUnit)
    {
        std::lock_guard<std::mutex> lock(g_unitMapMutex);
        
        // 如果之前有旧的句柄，先移除
        if (pUnit->hWnd && pUnit->hWnd != hWnd)
        {
            g_unitMap.erase(pUnit->hWnd);
        }
        
        pUnit->hWnd = hWnd;
        
        if (hWnd)
        {
            g_unitMap[hWnd] = pUnit;
        }
    }
}

// ============ 事件触发实现 ============

// 第一类事件通知
void Unit_FireEvent(PWIN_UNIT_BASE pUnit, INT nEventIndex, INT nArgCount, ...)
{
    if (!pUnit) return;
    
    EVENT_NOTIFY eventNotify;
    memset(&eventNotify, 0, sizeof(EVENT_NOTIFY));
    
    eventNotify.m_dwFormID = pUnit->dwFormID;
    eventNotify.m_dwUnitID = pUnit->dwUnitID;
    eventNotify.m_nEventIndex = nEventIndex;
    eventNotify.m_nArgCount = std::min(nArgCount, 5);
    
    if (nArgCount > 0)
    {
        va_list args;
        va_start(args, nArgCount);
        for (INT i = 0; i < eventNotify.m_nArgCount; i++)
        {
            eventNotify.m_nArgValue[i] = va_arg(args, INT);
        }
        va_end(args);
    }
    
    // 通知易语言运行时
    if (g_pfnNotifySystem)
    {
        // TODO: 调用NotifySystem(NRS_EVENT_NOTIFY, &eventNotify, 0)
    }
}

// 第二类事件通知（支持更多参数类型）
void Unit_FireEvent2(PWIN_UNIT_BASE pUnit, INT nEventIndex, INT nArgCount, PEVENT_ARG_VALUE pArgs)
{
    if (!pUnit) return;
    
    EVENT_NOTIFY2 eventNotify;
    memset(&eventNotify, 0, sizeof(EVENT_NOTIFY2));
    
    eventNotify.m_dwFormID = pUnit->dwFormID;
    eventNotify.m_dwUnitID = pUnit->dwUnitID;
    eventNotify.m_nEventIndex = nEventIndex;
    eventNotify.m_nArgCount = std::min(nArgCount, MAX_EVENT2_ARG_COUNT);
    
    if (pArgs && nArgCount > 0)
    {
        memcpy(eventNotify.m_arg, pArgs, sizeof(EVENT_ARG_VALUE) * eventNotify.m_nArgCount);
    }
    
    // 通知易语言运行时
    if (g_pfnNotifySystem)
    {
        // TODO: 调用NotifySystem(NRS_EVENT_NOTIFY2, &eventNotify, 0)
    }
}

// ============ 属性数据解析实现 ============

/**
 * 属性数据格式：
 * 对于每个属性：
 *   WORD wPropIndex  - 属性索引
 *   WORD wDataType   - 数据类型
 *   DWORD dwDataSize - 数据大小
 *   BYTE[] data      - 属性数据
 */
LPBYTE Unit_ParsePropData(LPBYTE pData, INT nDataSize, INT nPropIndex, INT* pnPropSize)
{
    if (!pData || nDataSize < 8) return NULL;
    
    LPBYTE pCurrent = pData;
    LPBYTE pEnd = pData + nDataSize;
    
    while (pCurrent + 8 <= pEnd)
    {
        WORD wIndex = *(WORD*)pCurrent;
        WORD wType = *(WORD*)(pCurrent + 2);
        DWORD dwSize = *(DWORD*)(pCurrent + 4);
        
        if (pCurrent + 8 + dwSize > pEnd) break;
        
        if ((INT)wIndex == nPropIndex)
        {
            if (pnPropSize) *pnPropSize = (INT)dwSize;
            return pCurrent + 8;
        }
        
        pCurrent += 8 + dwSize;
    }
    
    return NULL;
}

BOOL Unit_ReadPropInt(LPBYTE pData, INT nDataSize, INT nPropIndex, INT* pValue)
{
    INT nPropSize = 0;
    LPBYTE pPropData = Unit_ParsePropData(pData, nDataSize, nPropIndex, &nPropSize);
    
    if (pPropData && nPropSize >= (INT)sizeof(INT))
    {
        *pValue = *(INT*)pPropData;
        return TRUE;
    }
    
    return FALSE;
}

BOOL Unit_ReadPropBool(LPBYTE pData, INT nDataSize, INT nPropIndex, BOOL* pValue)
{
    INT nPropSize = 0;
    LPBYTE pPropData = Unit_ParsePropData(pData, nDataSize, nPropIndex, &nPropSize);
    
    if (pPropData && nPropSize >= (INT)sizeof(BOOL))
    {
        *pValue = *(BOOL*)pPropData;
        return TRUE;
    }
    
    return FALSE;
}

BOOL Unit_ReadPropText(LPBYTE pData, INT nDataSize, INT nPropIndex, LPSTR* ppText)
{
    INT nPropSize = 0;
    LPBYTE pPropData = Unit_ParsePropData(pData, nDataSize, nPropIndex, &nPropSize);
    
    if (pPropData && nPropSize > 0)
    {
        // 分配内存并复制文本
        *ppText = (LPSTR)malloc(nPropSize + 1);
        if (*ppText)
        {
            memcpy(*ppText, pPropData, nPropSize);
            (*ppText)[nPropSize] = '\0';
            return TRUE;
        }
    }
    
    return FALSE;
}

BOOL Unit_ReadPropColor(LPBYTE pData, INT nDataSize, INT nPropIndex, COLORREF* pColor)
{
    INT nPropSize = 0;
    LPBYTE pPropData = Unit_ParsePropData(pData, nDataSize, nPropIndex, &nPropSize);
    
    if (pPropData && nPropSize >= (INT)sizeof(COLORREF))
    {
        *pColor = *(COLORREF*)pPropData;
        return TRUE;
    }
    
    return FALSE;
}

BOOL Unit_ReadPropFont(LPBYTE pData, INT nDataSize, INT nPropIndex, LOGFONTW* pFont)
{
    INT nPropSize = 0;
    LPBYTE pPropData = Unit_ParsePropData(pData, nDataSize, nPropIndex, &nPropSize);
    
    if (pPropData && nPropSize >= (INT)sizeof(LOGFONTW))
    {
        memcpy(pFont, pPropData, sizeof(LOGFONTW));
        return TRUE;
    }
    
    return FALSE;
}

// ============ 窗口子类化实现 ============

// 子类化消息处理
LRESULT CALLBACK Unit_SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PWIN_UNIT_BASE pUnit = Unit_FromHWnd(hWnd);
    if (!pUnit || !pUnit->pfnOldProc)
    {
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
    
    switch (uMsg)
    {
    case WM_LBUTTONDOWN:
        Unit_FireEvent(pUnit, EVENT_MOUSEDOWN, 3, 
            LOWORD(lParam), HIWORD(lParam), (INT)wParam);
        break;
        
    case WM_LBUTTONUP:
        Unit_FireEvent(pUnit, EVENT_MOUSEUP, 3,
            LOWORD(lParam), HIWORD(lParam), (INT)wParam);
        break;
        
    case WM_LBUTTONDBLCLK:
        Unit_FireEvent(pUnit, EVENT_DBLCLICK, 0);
        break;
        
    case WM_MOUSEMOVE:
        Unit_FireEvent(pUnit, EVENT_MOUSEMOVE, 3,
            LOWORD(lParam), HIWORD(lParam), (INT)wParam);
        break;
        
    case WM_KEYDOWN:
        Unit_FireEvent(pUnit, EVENT_KEYDOWN, 1, (INT)wParam);
        break;
        
    case WM_KEYUP:
        Unit_FireEvent(pUnit, EVENT_KEYUP, 1, (INT)wParam);
        break;
        
    case WM_CHAR:
        Unit_FireEvent(pUnit, EVENT_CHAR, 1, (INT)wParam);
        break;
        
    case WM_SETFOCUS:
        pUnit->dwState |= UNIT_STATE_FOCUSED;
        Unit_FireEvent(pUnit, EVENT_GOTFOCUS, 0);
        break;
        
    case WM_KILLFOCUS:
        pUnit->dwState &= ~UNIT_STATE_FOCUSED;
        Unit_FireEvent(pUnit, EVENT_LOSTFOCUS, 0);
        break;
        
    case WM_MOUSELEAVE:
        Unit_FireEvent(pUnit, EVENT_MOUSELEAVE, 0);
        break;
        
    case WM_MOVE:
        Unit_FireEvent(pUnit, EVENT_POSCHANGED, 2,
            LOWORD(lParam), HIWORD(lParam));
        break;
        
    case WM_SIZE:
        Unit_FireEvent(pUnit, EVENT_SIZECHANGED, 2,
            LOWORD(lParam), HIWORD(lParam));
        break;
        
    case WM_DESTROY:
        Unit_FireEvent(pUnit, EVENT_DESTROY, 0);
        Unit_Unsubclass(pUnit);
        break;
    }
    
    return CallWindowProcW(pUnit->pfnOldProc, hWnd, uMsg, wParam, lParam);
}

void Unit_Subclass(PWIN_UNIT_BASE pUnit)
{
    if (pUnit && pUnit->hWnd && !pUnit->pfnOldProc)
    {
        pUnit->pfnOldProc = (WNDPROC)SetWindowLongPtrW(
            pUnit->hWnd, GWLP_WNDPROC, (LONG_PTR)Unit_SubclassProc);
    }
}

void Unit_Unsubclass(PWIN_UNIT_BASE pUnit)
{
    if (pUnit && pUnit->hWnd && pUnit->pfnOldProc)
    {
        SetWindowLongPtrW(pUnit->hWnd, GWLP_WNDPROC, (LONG_PTR)pUnit->pfnOldProc);
        pUnit->pfnOldProc = NULL;
    }
}

// ============ 辅助函数实现 ============

HFONT Unit_CreateFont(const LOGFONTW* pLogFont)
{
    if (!pLogFont) return NULL;
    return CreateFontIndirectW(pLogFont);
}

COLORREF Unit_ParseColorValue(INT nColorValue)
{
    // 易语言颜色格式转换为Windows COLORREF
    // 易语言颜色格式：0xBBGGRR（与COLORREF相同）
    return (COLORREF)nColorValue;
}

// 字符串转换
LPWSTR Unit_AnsiToWide(LPCSTR pszAnsi, INT* pnLen)
{
    if (!pszAnsi) return NULL;
    
    INT nLen = MultiByteToWideChar(CP_ACP, 0, pszAnsi, -1, NULL, 0);
    if (nLen <= 0) return NULL;
    
    LPWSTR pszWide = (LPWSTR)malloc(nLen * sizeof(WCHAR));
    if (pszWide)
    {
        MultiByteToWideChar(CP_ACP, 0, pszAnsi, -1, pszWide, nLen);
        if (pnLen) *pnLen = nLen - 1;
    }
    
    return pszWide;
}

LPSTR Unit_WideToAnsi(LPCWSTR pszWide, INT* pnLen)
{
    if (!pszWide) return NULL;
    
    INT nLen = WideCharToMultiByte(CP_ACP, 0, pszWide, -1, NULL, 0, NULL, NULL);
    if (nLen <= 0) return NULL;
    
    LPSTR pszAnsi = (LPSTR)malloc(nLen);
    if (pszAnsi)
    {
        WideCharToMultiByte(CP_ACP, 0, pszWide, -1, pszAnsi, nLen, NULL, NULL);
        if (pnLen) *pnLen = nLen - 1;
    }
    
    return pszAnsi;
}

void Unit_FreeString(LPVOID pStr)
{
    if (pStr) free(pStr);
}

// 设置全局通知函数
EXTERN_C void WINAPI Unit_SetNotifyFunction(PFN_INTERFACE pfnNotify)
{
    g_pfnNotifySystem = pfnNotify;
}
