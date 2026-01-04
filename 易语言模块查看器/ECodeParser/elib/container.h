
#ifndef __CONTAINER_H
#define __CONTAINER_H

BOOL ProcessCommand (CWnd* pWnd, WPARAM wParam, LPARAM lParam);
BOOL MProcessNotify (CWnd* pWnd, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
BOOL ProcessDrawItem (int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
BOOL MReflectLastMsg (HWND hWndChild, LRESULT* pResult);
BOOL ProcesssScroll (CWnd* pScrollBar);

#define DEFINE_CONTAINER_NO_COMMAND  \
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);  \
	afx_msg int OnCompareItem(int nIDCtl, LPCOMPAREITEMSTRUCT lpCompareItemStruct);  \
	afx_msg void OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct);  \
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);  \
    afx_msg HBRUSH OnCtlColor(CDC*, CWnd* pWnd, UINT);  \
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);  \
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

#define DEFINE_CONTAINER \
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);  \
    DEFINE_CONTAINER_NO_COMMAND

#define DEFINE_CONTAINER_EVENT  \
	ON_WM_COMPAREITEM()  \
	ON_WM_DELETEITEM()  \
	ON_WM_DRAWITEM()  \
	ON_WM_CTLCOLOR()  \
	ON_WM_HSCROLL()  \
	ON_WM_VSCROLL()

#define IMPLEMENT_CONTAINER_NO_COMMAND(classname) \
    BOOL classname::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)  \
    {  \
        return MProcessNotify (this, wParam, lParam, pResult);  \
    }  \
    void classname::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)  \
    {  \
        if (ProcessDrawItem (nIDCtl, lpDrawItemStruct) == FALSE)  \
            Default ();  \
    }  \
    int classname::OnCompareItem(int nIDCtl, LPCOMPAREITEMSTRUCT lpCompareItemStruct)  \
    {  \
	    LRESULT lResult;  \
	    if (MReflectLastMsg (lpCompareItemStruct->hwndItem, &lResult))  \
		    return (int)lResult;  \
        \
	    return (int)Default();  \
    }  \
    void classname::OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct)  \
    {  \
        if (MReflectLastMsg (lpDeleteItemStruct->hwndItem, NULL) == FALSE)  \
            Default ();  \
    }  \
    void classname::OnHScroll(UINT, UINT, CScrollBar* pScrollBar)  \
    {  \
	    if (ProcesssScroll (pScrollBar) == FALSE)  \
    	    Default();  \
    }  \
    void classname::OnVScroll(UINT, UINT, CScrollBar* pScrollBar)  \
    {  \
	    if (ProcesssScroll (pScrollBar) == FALSE)  \
    	    Default();  \
    }  \
    HBRUSH classname::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT)  \
    {  \
	    ASSERT(pWnd != NULL && pWnd->m_hWnd != NULL);  \
        \
        LRESULT lResult;  \
	    if (pWnd->SendChildNotifyLastMsg (&lResult))  \
		    return (HBRUSH)lResult;  \
        \
        struct  \
        {  \
	        HWND hWnd;  \
	        HDC hDC;  \
	        UINT nCtlType;  \
        } ctl;  \
		ctl.hDC = (HDC)pDC->GetSafeHdc ();  \
	    _AFX_THREAD_STATE* pThreadState = _afxThreadState.GetData();  \
	    ctl.nCtlType = pThreadState->m_lastSentMsg.message - WM_CTLCOLORMSGBOX;  \
	    ASSERT(ctl.nCtlType >= CTLCOLOR_MSGBOX);  \
	    ASSERT(ctl.nCtlType <= CTLCOLOR_STATIC);  \
        \
		lResult = pWnd->SendMessage (WM_REFLECT_BASE + WM_CTLCOLOR, 0, (LPARAM)&ctl);  \
		if (lResult != NULL)  \
		    return (HBRUSH)lResult;  \
        \
	    return (HBRUSH)Default();  \
    }

#define IMPLEMENT_CONTAINER(classname) \
    BOOL classname::OnCommand(WPARAM wParam, LPARAM lParam)  \
    {  \
        return ProcessCommand (this, wParam, lParam);  \
    }  \
    IMPLEMENT_CONTAINER_NO_COMMAND (classname)

#endif

