/*
    版权声明：
    本文件版权为易语言作者吴涛所有，仅授权给第三方用作开发易语言支持库，禁止用于其他任何场合。
*/

//本单元中使用了MFC类, 仅供编写带窗口组件的支持库使用

#ifndef __UNTSHARE_H
#define	__UNTSHARE_H


void StrAryLangCnv (CStringArray& strary, PFN_CNV fnCnv, int nParam);

#define BEGIN_DEFINE_LANG_CNV  \
        HGLOBAL LangCnv (LPBYTE pAllData, LPINT pnAllDataSize, PFN_CNV fnCnv, int nParam)  \
        {  \
            if (LoadData (pAllData, *pnAllDataSize) == FALSE)  \
                return NULL;  \

#define CSTRING_CNV(str) \
        fnCnv ((char*)(LPCTSTR)str, CNV_NULL, nParam);

#define CSTRINGARRAY_CNV(strary) \
        StrAryLangCnv (strary, fnCnv, nParam);

#define FONTMEM_CNV(memFont) \
        if (memFont.GetSize () == sizeof (LOGFONT))  \
            fnCnv ((char*)((LOGFONT*)memFont.GetPtr ())->lfFaceName, CNV_FONTNAME, nParam);

#define END_DEFINE_LANG_CNV  \
            return SaveData (pnAllDataSize);  \
        }

#define DEFINE_LANGCNV_CALLBACK(class_name)  \
        HGLOBAL WINAPI LangCnv##class_name (LPBYTE pAllData, LPINT pnAllDataSize, PFN_CNV fnCnv, int nParam)  \
        {  \
            CPI##class_name inf;  \
            return inf.LangCnv (pAllData, pnAllDataSize, fnCnv, nParam);  \
        }

#define GET_LANGCNV_CALLBACK_FNNAME(class_name)  LangCnv##class_name


///////////////////////////////////////////////////

class CPropertyInfo
{
public:
	CPropertyInfo ()
	{
		init ();
	}

	virtual void init ()  { }
	virtual BOOL Serialize (CArchive& ar)
	{
		if (ar.IsLoading () == TRUE)
			init ();
		return TRUE;
	}

	HGLOBAL SaveData (LPINT pnAllDataSize = NULL);
	BOOL LoadData (LPBYTE pData, INT nDataSize);
};

COLORREF ProcessSysBackColor (COLORREF clr);
BOOL CreateImageList (CImageList* pImglist, LPBYTE p, INT nSize);
void ChangeBorder (CWnd* pUnit, INT nBorderType);

CWnd* GetWndPtr (PMDATA_INF parg);
BOOL MyCreateFont (CFont& font, LOGFONT* pLogFont);
void ModiUnitStyle (CWnd* pUnit, DWORD dwAddStyle, DWORD dwRemoveStyle, BOOL ExStyel = FALSE);
void SetUnitStyle (CWnd* pUnit, BOOL blSet, DWORD dwStyle);
void SerializeCString (CString& str, CArchive& ar);
void SetStr (CString& str, LPTSTR szText);
HICON LoadIco (LPBYTE pData, INT nDataSize, BOOL blIsBigIcon);


//属性信息占位符
#define UNIT_PROPERTY_MASK_1 \
	{\
	/*m_szName*/	_T(""),\
	/*m_szEgName*/	_T(""),\
	/*m_szExplain*/	NULL,\
	/*m_shtType*/	0,\
	/*m_wState*/	UW_IS_HIDED,\
	/*m_szzPickStr*/NULL,\
	}
#define UNIT_PROPERTY_MASK_2	UNIT_PROPERTY_MASK_1,UNIT_PROPERTY_MASK_1
#define UNIT_PROPERTY_MASK_3	UNIT_PROPERTY_MASK_2,UNIT_PROPERTY_MASK_1
#define UNIT_PROPERTY_MASK_4	UNIT_PROPERTY_MASK_3,UNIT_PROPERTY_MASK_1
#define UNIT_PROPERTY_MASK_5	UNIT_PROPERTY_MASK_4,UNIT_PROPERTY_MASK_1
#define UNIT_PROPERTY_MASK_6	UNIT_PROPERTY_MASK_5,UNIT_PROPERTY_MASK_1
#define UNIT_PROPERTY_MASK_7	UNIT_PROPERTY_MASK_6,UNIT_PROPERTY_MASK_1
#define UNIT_PROPERTY_MASK_8	UNIT_PROPERTY_MASK_7,UNIT_PROPERTY_MASK_1
#define UNIT_PROPERTY_MASK_9	UNIT_PROPERTY_MASK_8,UNIT_PROPERTY_MASK_1
#define UNIT_PROPERTY_MASK_10	UNIT_PROPERTY_MASK_9,UNIT_PROPERTY_MASK_1

#define MASK_UNIT_PROPERTY_1	UNIT_PROPERTY_MASK_1
#define MASK_UNIT_PROPERTY_2	UNIT_PROPERTY_MASK_2
#define MASK_UNIT_PROPERTY_3	UNIT_PROPERTY_MASK_3
#define MASK_UNIT_PROPERTY_4	UNIT_PROPERTY_MASK_4
#define MASK_UNIT_PROPERTY_5	UNIT_PROPERTY_MASK_5
#define MASK_UNIT_PROPERTY_6	UNIT_PROPERTY_MASK_6
#define MASK_UNIT_PROPERTY_7	UNIT_PROPERTY_MASK_7
#define MASK_UNIT_PROPERTY_8	UNIT_PROPERTY_MASK_8
#define MASK_UNIT_PROPERTY_9	UNIT_PROPERTY_MASK_9
#define MASK_UNIT_PROPERTY_10	UNIT_PROPERTY_MASK_10


//事件信息占位符
#define EVENT_INFO2_MASK_1 \
	{\
	/*m_szName*/		_T(""),\
	/*m_szExplain*/		NULL,\
	/*m_dwState*/		EV_IS_HIDED | EV_IS_VER2,\
	/*m_nArgCount*/		0,\
	/*m_pBeginArgInfo*/	NULL,\
	/*m_dtRetDataType*/	_SDT_NULL,\
	}
#define EVENT_INFO2_MASK_2	EVENT_INFO2_MASK_1,EVENT_INFO2_MASK_1
#define EVENT_INFO2_MASK_3	EVENT_INFO2_MASK_2,EVENT_INFO2_MASK_1
#define EVENT_INFO2_MASK_4	EVENT_INFO2_MASK_3,EVENT_INFO2_MASK_1
#define EVENT_INFO2_MASK_5	EVENT_INFO2_MASK_4,EVENT_INFO2_MASK_1
#define EVENT_INFO2_MASK_6	EVENT_INFO2_MASK_5,EVENT_INFO2_MASK_1
#define EVENT_INFO2_MASK_7	EVENT_INFO2_MASK_6,EVENT_INFO2_MASK_1
#define EVENT_INFO2_MASK_8	EVENT_INFO2_MASK_7,EVENT_INFO2_MASK_1
#define EVENT_INFO2_MASK_9	EVENT_INFO2_MASK_8,EVENT_INFO2_MASK_1
#define EVENT_INFO2_MASK_10	EVENT_INFO2_MASK_9,EVENT_INFO2_MASK_1

#define MASK_EVENT_INFO2_1	EVENT_INFO2_MASK_1
#define MASK_EVENT_INFO2_2	EVENT_INFO2_MASK_2
#define MASK_EVENT_INFO2_3	EVENT_INFO2_MASK_3
#define MASK_EVENT_INFO2_4	EVENT_INFO2_MASK_4
#define MASK_EVENT_INFO2_5	EVENT_INFO2_MASK_5
#define MASK_EVENT_INFO2_6	EVENT_INFO2_MASK_6
#define MASK_EVENT_INFO2_7	EVENT_INFO2_MASK_7
#define MASK_EVENT_INFO2_8	EVENT_INFO2_MASK_8
#define MASK_EVENT_INFO2_9	EVENT_INFO2_MASK_9
#define MASK_EVENT_INFO2_10	EVENT_INFO2_MASK_10


#endif //__UNTSHARE_H

