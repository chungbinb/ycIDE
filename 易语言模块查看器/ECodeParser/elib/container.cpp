
#include "stdafx.h"
#include "lib2.h"
#include "container.h"
#include <AFXPRIV.H>

extern CHandleMap* PASCAL afxMapHWND(BOOL bCreate = FALSE);
#include <../src/winhand_.h>

// 用作支持对其他支持库中的窗口组件进行通知反馈。
BOOL MReflectLastMsg (HWND hWndChild, LRESULT* pResult)
{
	// get the map, and if no map, then this message does not need reflection
	CHandleMap* pMap = afxMapHWND();
	if (pMap == NULL)
		return FALSE;

	// check if in permanent map, if it is reflect it (could be OLE control)
	CWnd* pWnd = (CWnd*)pMap->LookupPermanent(hWndChild);
	ASSERT(pWnd == NULL || pWnd->m_hWnd == hWndChild);
	if (pWnd == NULL)
	{
		pWnd = (CWnd*)::SendMessage (hWndChild, WU_GET_WND_PTR, 0, 0);
		if (pWnd == NULL)
			return FALSE;
		ASSERT (pWnd->m_hWnd == hWndChild);
	}

	// only OLE controls and permanent windows will get reflected msgs
	ASSERT(pWnd != NULL);
	return pWnd->SendChildNotifyLastMsg(pResult);
}

BOOL MProcessNotify (CWnd* pWnd, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	ASSERT(pResult != NULL);
	NMHDR* pNMHDR = (NMHDR*)lParam;
	HWND hWndCtrl = pNMHDR->hwndFrom;

	// get the child ID from the window itself
	UINT nID = ::GetDlgCtrlID (hWndCtrl);
	int nCode = pNMHDR->code;

	ASSERT(hWndCtrl != NULL);
	ASSERT(::IsWindow(hWndCtrl));

	if (_afxThreadState->m_hLockoutNotifyWindow == pWnd->m_hWnd)
		return TRUE;        // locked out - ignore control notification

	// reflect notification to child window control
	if (MReflectLastMsg(hWndCtrl, pResult))
		return TRUE;        // eaten by child

	AFX_NOTIFY notify;
	notify.pResult = pResult;
	notify.pNMHDR = pNMHDR;
	return pWnd->OnCmdMsg(nID, MAKELONG(nCode, WM_NOTIFY), &notify, NULL);
}

class CTestCmdUI : public CCmdUI
{
public:
	CTestCmdUI();

public: // re-implementations only
	virtual void Enable(BOOL bOn);
	virtual void SetCheck(int nCheck);
	virtual void SetRadio(BOOL bOn);
	virtual void SetText(LPCTSTR);

	BOOL m_bEnabled;
};

BOOL ProcessCommand (CWnd* pWnd, WPARAM wParam, LPARAM lParam)
{
	UINT nID = LOWORD(wParam);
	HWND hWndCtrl = (HWND)lParam;
	int nCode = HIWORD(wParam);

    HWND hWndBak = pWnd->m_hWnd;

	// default routing for command messages (through closure table)

	if (hWndCtrl == NULL)
	{
		// zero IDs for normal commands are not allowed
		if (nID == 0)
			return FALSE;

		// make sure command has not become disabled before routing
		CTestCmdUI state;
		state.m_nID = nID;
		pWnd->OnCmdMsg(nID, CN_UPDATE_COMMAND_UI, &state, NULL);
		if (!state.m_bEnabled)
		{
			TRACE1("Warning: not executing disabled command %d\n", nID);
			return TRUE;
		}

		// menu or accelerator
		nCode = CN_COMMAND;
	}
	else
	{

		// control notification
		ASSERT(nID == 0 || ::IsWindow(hWndCtrl));

		if (_afxThreadState->m_hLockoutNotifyWindow == pWnd->m_hWnd)
			return TRUE;        // locked out - ignore control notification

		// reflect notification to child window control
		if (MReflectLastMsg (hWndCtrl, NULL))
			return TRUE;    // eaten by child

    	if (CWnd::FromHandlePermanent (hWndCtrl) == NULL)
        {
            static CDWordArray s_dwaryHWNDs;

            INT nCount = s_dwaryHWNDs.GetSize ();
            BOOL blFound = FALSE;
            for (INT i = 0; i < nCount; i++)
            {
                if (s_dwaryHWNDs [i] == (DWORD)hWndCtrl)
                {
                    blFound = TRUE;
                    break;
                }
            }

            if (blFound == FALSE)
            {
                s_dwaryHWNDs.Add ((DWORD)hWndCtrl);

		        ::SendMessage (hWndCtrl, WM_COMMAND, wParam, lParam);

                nCount = s_dwaryHWNDs.GetSize ();
                for (i = 0; i < nCount; i++)
                {
                    if (s_dwaryHWNDs [i] == (DWORD)hWndCtrl)
                    {
                        s_dwaryHWNDs.RemoveAt (i);
                        break;
                    }
                }
            }

			return TRUE;
        }

		// zero IDs for normal commands are not allowed
		if (nID == 0)
			return FALSE;
	}

#ifdef _DEBUG
	if (nCode < 0 && nCode != (int)0x8000)
		TRACE1("Implementation Warning: control notification = $%X.\n",
			nCode);
#endif

    if (::IsWindow (hWndBak))
    	return pWnd->OnCmdMsg (nID, nCode, NULL, NULL);
    else
        return TRUE;
}

/**
//the old version
BOOL ProcessCommand (CWnd* pWnd, WPARAM wParam, LPARAM lParam)
{
	UINT nID = LOWORD(wParam);
	HWND hWndCtrl = (HWND)lParam;
	int nCode = HIWORD(wParam);

    HWND hWndBak = pWnd->m_hWnd;

	// default routing for command messages (through closure table)

	if (hWndCtrl == NULL)
	{
		// zero IDs for normal commands are not allowed
		if (nID == 0)
			return FALSE;

		// make sure command has not become disabled before routing
		CTestCmdUI state;
		state.m_nID = nID;
		pWnd->OnCmdMsg(nID, CN_UPDATE_COMMAND_UI, &state, NULL);
		if (!state.m_bEnabled)
		{
			TRACE1("Warning: not executing disabled command %d\n", nID);
			return TRUE;
		}

		// menu or accelerator
		nCode = CN_COMMAND;
	}
	else
	{

		// control notification
		ASSERT(nID == 0 || ::IsWindow(hWndCtrl));

		if (_afxThreadState->m_hLockoutNotifyWindow == pWnd->m_hWnd)
			return TRUE;        // locked out - ignore control notification

		// reflect notification to child window control
		if (MReflectLastMsg (hWndCtrl, NULL))
			return TRUE;    // eaten by child

        static BOOL s_blEntried = FALSE;
    	if (s_blEntried == FALSE && CWnd::FromHandlePermanent (hWndCtrl) == NULL)
        {
            s_blEntried = TRUE;
		    ::SendMessage (hWndCtrl, WM_COMMAND, wParam, lParam);
            s_blEntried = FALSE;
			return TRUE;
        }

		// zero IDs for normal commands are not allowed
		if (nID == 0)
			return FALSE;
	}

#ifdef _DEBUG
	if (nCode < 0 && nCode != (int)0x8000)
		TRACE1("Implementation Warning: control notification = $%X.\n",
			nCode);
#endif

    if (::IsWindow (hWndBak))
    	return pWnd->OnCmdMsg (nID, nCode, NULL, NULL);
    else
        return TRUE;
}
*/

BOOL ProcessDrawItem (int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->CtlType == ODT_MENU)
	{
		CMenu* pMenu = CMenu::FromHandlePermanent(
			(HMENU)lpDrawItemStruct->hwndItem);
		if (pMenu != NULL)
		{
			pMenu->DrawItem(lpDrawItemStruct);
			return TRUE; // eat it
		}
	}
	else
	{
		// reflect notification to child window control
		if (MReflectLastMsg (lpDrawItemStruct->hwndItem, NULL))
			return TRUE;     // eat it
	}
    return FALSE;
}

BOOL ProcesssScroll (CWnd* pScrollBar)
{
	if (pScrollBar != NULL)
    {
		CWnd* pWnd = (CWnd*)pScrollBar->SendMessage (WU_GET_WND_PTR, 0, 0);
		if (pWnd != NULL)
			pScrollBar = pWnd;

        if (pScrollBar->SendChildNotifyLastMsg ())
		    return TRUE;
    }

    return FALSE;
}

