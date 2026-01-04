#include "stdafx.h"
#include <afxadv.h>
#include "untshare.h"

void StrAryLangCnv (CStringArray& strary, PFN_CNV fnCnv, int nParam)
{
    int nCount = strary.GetSize ();
    for (int i = 0; i < nCount; i++)
        fnCnv ((char*)(LPCTSTR)strary.ElementAt (i), CNV_NULL, nParam);
}

HGLOBAL CPropertyInfo::SaveData (LPINT pnAllDataSize)
{
	CSharedFile file;
	CArchive ar (&file, CArchive::store);

	if (Serialize (ar) == TRUE)
	{
		ar.Close ();

		DWORD dwLength = file.GetLength ();
		HGLOBAL hGlobal = file.Detach ();
		::GlobalUnlock (hGlobal);
		::GlobalReAlloc (hGlobal, dwLength, GMEM_MOVEABLE);
		//     注意由于Windows系统内存管理按字对齐的原因，最后hGlobal的大小（GloalSize返回）
		// 可能会大于dwLength值0-3个字节。

        if (pnAllDataSize != NULL)
            *pnAllDataSize = (INT)dwLength;

		return hGlobal;
	}
	else
    {
        if (pnAllDataSize != NULL)
            *pnAllDataSize = 0;
		return NULL;
    }
}

BOOL CPropertyInfo::LoadData (LPBYTE pData, INT nDataSize)
{
	init ();

	if (pData != NULL && nDataSize > 0)
	{
		CMemFile file;
		file.Attach (pData, nDataSize);

		CArchive ar (&file, CArchive::load);
		return Serialize (ar);
	}
	else
		return TRUE;
}

void ChangeBorder (CWnd* pUnit, INT nBorderType)
{
	DWORD dwStyle = NULL, dwExStyle = NULL;
	switch (nBorderType)
	{
//	case 0:		// 无边框
//		break;
	case 1:		// 凹入式
		dwExStyle = WS_EX_CLIENTEDGE;
		break;
	case 2:		// 凸出式
		dwExStyle = WS_EX_DLGMODALFRAME;
		break;
	case 3:		// 浅凹入式
		dwExStyle = WS_EX_STATICEDGE;
		break;
	case 4:		// 镜框式
		dwExStyle = WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME;
		break;
	case 5:
		dwStyle = WS_BORDER;
		break;
	}
	pUnit->ModifyStyleEx (WS_EX_STATICEDGE | WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME,
			dwExStyle, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE |
			SWP_FRAMECHANGED | SWP_DRAWFRAME);
	pUnit->ModifyStyle (WS_BORDER, dwStyle,
			SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE |
			SWP_FRAMECHANGED | SWP_DRAWFRAME);
}

BOOL CreateImageList (CImageList* pImglist, LPBYTE p, INT nSize)
{
	ASSERT(pImglist);
	if (pImglist == NULL) return FALSE;

	if (nSize > sizeof (DWORD) + sizeof (COLORREF) &&
			*(LPDWORD)p == IMAGE_LIST_DATA_MARK)
	{
		CMemFile file;
		file.Attach (p + sizeof (DWORD) + sizeof (COLORREF),
				nSize - sizeof (DWORD) - sizeof (COLORREF));
		CArchive ar (&file, CArchive::load);
		
		//注: CImageList.Read() 生成的可能是一个临时的CImageList对象! 
		//通过引入一个CImageList局部变量并在它基础上创建一个新的持久对象来解决此问题。
		CImageList tempImglist;
		if (tempImglist.Read (&ar))
		{
			if (pImglist->GetSafeHandle())
				pImglist->DeleteImageList ();
			return pImglist->Create(&tempImglist); //此处创建的pImglist为持久对象
		}
	}
	
	return FALSE;
}

COLORREF ProcessSysBackColor (COLORREF clr)
{
	if (clr == CLR_DEFAULT)
		return ::GetSysColor (COLOR_BTNFACE);
	else
		return clr;
}

// 绝对不会返回NULL或者窗口句柄无效的CWnd*指针。
CWnd* GetWndPtr (PMDATA_INF pInf)
{
	return (CWnd*)NotifySys (NRS_GET_AND_CHECK_UNIT_PTR, pInf->m_unit.m_dwFormID,
			pInf->m_unit.m_dwUnitID);
}

BOOL MyCreateFont (CFont& font, LOGFONT* pLogFont)
{
    if ((HFONT)font != NULL)
        font.DeleteObject ();

	LOGFONT lgfont;
	::CopyMemory ((LPBYTE)&lgfont, (LPBYTE)pLogFont, sizeof (LOGFONT));

	lgfont.lfCharSet = DEFAULT_CHARSET;
	lgfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lgfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lgfont.lfQuality = PROOF_QUALITY;
	lgfont.lfPitchAndFamily = DEFAULT_PITCH;

	return font.CreateFontIndirect (&lgfont);
}

void ModiUnitStyle (CWnd* pUnit, DWORD dwAddStyle, DWORD dwRemoveStyle, BOOL ExStyel)
{
	int index = GWL_STYLE;
	if(ExStyel) index = GWL_EXSTYLE;
	DWORD dwOldStyle = GetWindowLong (pUnit->m_hWnd, index);
	DWORD dwNewStyle = (dwOldStyle & ~dwRemoveStyle) | dwAddStyle;
	if (dwNewStyle != dwOldStyle)
		SetWindowLong (pUnit->m_hWnd, index, dwNewStyle);
}

void SetUnitStyle (CWnd* pUnit, BOOL blSet, DWORD dwStyle)
{
	ModiUnitStyle (pUnit, (blSet ? dwStyle : 0), (blSet ? 0 : dwStyle));
}

void SerializeCString (CString& str, CArchive& ar)
{
	INT nSize;
	if (ar.IsStoring() == TRUE)
	{
		nSize = str.GetLength () * sizeof (TCHAR);
		ar << nSize;
		if (nSize > 0)
			ar.Write ((LPTSTR)(LPCTSTR)str, nSize);
	}
	else
	{
		str.Empty ();

		ar >> nSize;
		ASSERT (nSize >= 0);
		if (nSize > 0)
		{
			LPTSTR p = str.GetBuffer (nSize);
			ar.Read (p, nSize);
			str.ReleaseBuffer();
		}
	}
}

void SetStr (CString& str, LPTSTR szText)
{
	if (szText != NULL)
		str = szText;
	else
		str.Empty ();
}


//返回图标数据的图标句柄
HICON LoadIco (LPBYTE pData, INT nDataSize, BOOL blIsBigIcon)
{
	if (pData == NULL || nDataSize <= 0)  return NULL;

	CMemFile file;
	file.Attach (pData, nDataSize);

	LPBYTE pBuf = NULL;

	TRY
	{
		WORD wBuf [3];
		if (file.Read ((LPBYTE)wBuf, sizeof (wBuf)) != sizeof (wBuf) ||
				wBuf [0] != 0 || (wBuf [1] != 1 && wBuf [1] != 2) || wBuf [2] <= 0)
			return NULL;

		BOOL blIsIcon = wBuf [1] == 1;
		INT nNumImages = (INT)wBuf [2];
		ASSERT (nNumImages > 0);
		if (nNumImages <= 0)  return NULL;

		typedef struct
		{
			BYTE	bWidth;               // Width of the image
			BYTE	bHeight;              // Height of the image (times 2)
			BYTE	bColorCount;          // Number of colors in image (0 if >=8bpp)
			BYTE	bReserved;            // Reserved
			WORD	wPlanes;              // Color Planes
			WORD	wBitCount;            // Bits per pixel
			DWORD	dwBytesInRes;         // how many bytes in this resource?
			DWORD	dwImageOffset;        // where in the file is this image
		} ICONDIRENTRY, *LPICONDIRENTRY;

		ICONDIRENTRY info;
		for (INT nImageIndex = 0; nImageIndex < nNumImages; nImageIndex++)
		{
			file.Seek (nImageIndex * sizeof (ICONDIRENTRY) + sizeof (wBuf), CFile::begin);

			if (file.Read ((LPBYTE)&info, sizeof (ICONDIRENTRY)) != sizeof (ICONDIRENTRY))
				return NULL;

			if (blIsBigIcon == TRUE ? info.bWidth == 32 : info.bWidth == 16)
				break;
		}
		if (nImageIndex == nNumImages)
			return NULL;

		if (nImageIndex >= nNumImages)
			nImageIndex = nNumImages - 1;

		pBuf = new BYTE [info.dwBytesInRes];
		if (pBuf == NULL)
			return NULL;

        file.Seek (info.dwImageOffset, CFile::begin);
		if (file.Read (pBuf, info.dwBytesInRes) != info.dwBytesInRes)
		{
			delete [] pBuf;
			return NULL;
		}

		LPBITMAPINFO pBitmapInfo = (LPBITMAPINFO)pBuf;

		HICON hIcon = CreateIconFromResourceEx (pBuf, info.dwBytesInRes,
				TRUE, 0x00030000, pBitmapInfo->bmiHeader.biWidth,
				pBitmapInfo->bmiHeader.biHeight / 2, 0);

		if (hIcon == NULL && pBitmapInfo->bmiHeader.biBitCount != 16)
			hIcon = CreateIconFromResource (pBuf, info.dwBytesInRes, TRUE, 0x00030000);

		delete [] pBuf;
		return hIcon;
	}
	END_TRY

	if (pBuf != NULL)
		delete[] pBuf;

	return NULL;
}