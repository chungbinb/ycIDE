/*
    版权声明：
    本文件版权为易语言作者吴涛所有，仅授权给第三方用作开发易语言支持库，禁止用于其他任何场合。
*/

//本单元不使用MFC类, 可供编写跨平台的支持库或非窗口组件支持库使用

#ifndef __FN_SHARE_H
#define __FN_SHARE_H

INT WINAPI ProcessNotifyLib (INT nMsg, DWORD dwParam1, DWORD dwParam2);
INT WINAPI NotifySys (INT nMsg, DWORD dwParam1, DWORD dwParam2);

char* CloneTextData (char* ps);
char* CloneTextData (char* ps, INT nTextLen);
LPBYTE CloneBinData (LPBYTE pData, INT nDataSize);
void GReportError (char* szErrText);
void* MMalloc (INT nSize);
void MFree (void* p);
LPBYTE GetAryElementInf (void* pAryData, LPINT pnElementCount);
INT GetDataTypeType (DATA_TYPE dtDataType);
void SetLogfont_CompileLanVer(LPLOGFONT pLogfont); //根据易编译版本设置默认的等宽字体

#ifdef __COMPILE_FNR	// 此宏在编译配置中设置
	#define	_WT(text)	""
#else
	#define	_WT(text)	text
#endif

//add by liigo
typedef INT (cdecl *PFN_ON_SYS_NOTIFY) (INT nMsg, DWORD dwParam1, DWORD dwParam2);
//end add


#ifndef _private
#define _private  //称识为只私有
#endif

/*
//!!! 以下函数均假设数据类型之成员全部为INT类型(或同样占用4个字节)
//index: 0..*/

_private void* _GetPointerByIndex(void *pBase, int index);
_private INT _GetIntByIndex(void *pBase, INT index);
_private void _SetIntByIndex(void *pBase, INT index, INT value);
INT GetIntByIndex(PMDATA_INF pArgInf, INT index);
void SetIntByIndex(PMDATA_INF pArgInf, INT index, INT value);
void* GetPointerByIndex(PMDATA_INF pArgInf, INT index);

//命令信息占位符
#define CMD_INFO_MASK_1 \
		{\
		/*ccname*/	_T("???"),\
		/*egname*/	_T("???"),\
		/*explain*/	NULL,\
		/*category*/-1,\
		/*state*/	CT_IS_HIDED,\
		/*ret*/		_SDT_NULL,\
		/*reserved*/0,\
		/*level*/	LVL_SIMPLE,\
		/*bmp inx*/	0,\
		/*bmp num*/	0,\
		/*ArgCount*/0,\
		/*arg lp*/	NULL,\
		}
#define CMD_INFO_MASK_2		CMD_INFO_MASK_1,CMD_INFO_MASK_1
#define CMD_INFO_MASK_3		CMD_INFO_MASK_2,CMD_INFO_MASK_1
#define CMD_INFO_MASK_4		CMD_INFO_MASK_3,CMD_INFO_MASK_1
#define CMD_INFO_MASK_5		CMD_INFO_MASK_4,CMD_INFO_MASK_1
#define CMD_INFO_MASK_6		CMD_INFO_MASK_5,CMD_INFO_MASK_1
#define CMD_INFO_MASK_7		CMD_INFO_MASK_6,CMD_INFO_MASK_1
#define CMD_INFO_MASK_8		CMD_INFO_MASK_7,CMD_INFO_MASK_1
#define CMD_INFO_MASK_9		CMD_INFO_MASK_8,CMD_INFO_MASK_1
#define CMD_INFO_MASK_10	CMD_INFO_MASK_9,CMD_INFO_MASK_1

#define MASK_CMD_INFO_1		CMD_INFO_MASK_1
#define MASK_CMD_INFO_2		CMD_INFO_MASK_2
#define MASK_CMD_INFO_3		CMD_INFO_MASK_3
#define MASK_CMD_INFO_4		CMD_INFO_MASK_4
#define MASK_CMD_INFO_5		CMD_INFO_MASK_5
#define MASK_CMD_INFO_6		CMD_INFO_MASK_6
#define MASK_CMD_INFO_7		CMD_INFO_MASK_7
#define MASK_CMD_INFO_8		CMD_INFO_MASK_8
#define MASK_CMD_INFO_9		CMD_INFO_MASK_9
#define MASK_CMD_INFO_10	CMD_INFO_MASK_10


//数据类型占位符
#define DATATYPE_INFO_MASK_1 \
    {\
	/*m_szName*/			_T(""),\
	/*m_szEgName*/			_T(""),\
	/*m_szExplain*/			NULL,\
	/*m_nCmdCount*/			0,\
	/*m_pnCmdsIndex*/		NULL,\
	/*m_dwState*/			NULL,\
	/*m_dwUnitBmpID*/		0,\
	/*m_nEventCount*/		0,\
	/*m_pEventBegin*/		NULL,\
	/*m_nPropertyCount*/	0,\
	/*m_pPropertyBegin*/	NULL,\
	/*m_pfnGetInterface*/	NULL,\
	/*m_nElementCount*/		0,\
	/*m_pElementBegin*/		NULL,\
	}
#define DATATYPE_INFO_MASK_2	DATATYPE_INFO_MASK_1,DATATYPE_INFO_MASK_1
#define DATATYPE_INFO_MASK_3	DATATYPE_INFO_MASK_2,DATATYPE_INFO_MASK_1
#define DATATYPE_INFO_MASK_4	DATATYPE_INFO_MASK_3,DATATYPE_INFO_MASK_1
#define DATATYPE_INFO_MASK_5	DATATYPE_INFO_MASK_4,DATATYPE_INFO_MASK_1
#define DATATYPE_INFO_MASK_6	DATATYPE_INFO_MASK_5,DATATYPE_INFO_MASK_1
#define DATATYPE_INFO_MASK_7	DATATYPE_INFO_MASK_6,DATATYPE_INFO_MASK_1
#define DATATYPE_INFO_MASK_8	DATATYPE_INFO_MASK_7,DATATYPE_INFO_MASK_1
#define DATATYPE_INFO_MASK_9	DATATYPE_INFO_MASK_8,DATATYPE_INFO_MASK_1
#define DATATYPE_INFO_MASK_10	DATATYPE_INFO_MASK_9,DATATYPE_INFO_MASK_1

#define MASK_DATATYPE_INFO_1	DATATYPE_INFO_MASK_1
#define MASK_DATATYPE_INFO_2	DATATYPE_INFO_MASK_2
#define MASK_DATATYPE_INFO_3	DATATYPE_INFO_MASK_3
#define MASK_DATATYPE_INFO_4	DATATYPE_INFO_MASK_4
#define MASK_DATATYPE_INFO_5	DATATYPE_INFO_MASK_5
#define MASK_DATATYPE_INFO_6	DATATYPE_INFO_MASK_6
#define MASK_DATATYPE_INFO_7	DATATYPE_INFO_MASK_7
#define MASK_DATATYPE_INFO_8	DATATYPE_INFO_MASK_8
#define MASK_DATATYPE_INFO_9	DATATYPE_INFO_MASK_9
#define MASK_DATATYPE_INFO_10	DATATYPE_INFO_MASK_10


//数据类型成员占位符
#define DATATYPE_ELEMENT_MASK_1 \
	{\
/*m_dtType*/	SDT_INT,\
/*m_pArySpec*/	NULL,\
/*m_szName*/	_WT(""),\
/*m_szEgName*/	_WT(""),\
/*m_szExplain*/	NULL,\
/*m_dwState*/	LES_HIDED,\
/*m_nDefault*/	0,\
	}

#define DATATYPE_ELEMENT_MASK_2		DATATYPE_ELEMENT_MASK_1,DATATYPE_ELEMENT_MASK_1
#define DATATYPE_ELEMENT_MASK_3		DATATYPE_ELEMENT_MASK_2,DATATYPE_ELEMENT_MASK_1
#define DATATYPE_ELEMENT_MASK_4		DATATYPE_ELEMENT_MASK_3,DATATYPE_ELEMENT_MASK_1
#define DATATYPE_ELEMENT_MASK_5		DATATYPE_ELEMENT_MASK_4,DATATYPE_ELEMENT_MASK_1
#define DATATYPE_ELEMENT_MASK_6		DATATYPE_ELEMENT_MASK_5,DATATYPE_ELEMENT_MASK_1
#define DATATYPE_ELEMENT_MASK_7		DATATYPE_ELEMENT_MASK_6,DATATYPE_ELEMENT_MASK_1
#define DATATYPE_ELEMENT_MASK_8		DATATYPE_ELEMENT_MASK_7,DATATYPE_ELEMENT_MASK_1
#define DATATYPE_ELEMENT_MASK_9		DATATYPE_ELEMENT_MASK_8,DATATYPE_ELEMENT_MASK_1
#define DATATYPE_ELEMENT_MASK_10	DATATYPE_ELEMENT_MASK_9,DATATYPE_ELEMENT_MASK_1

#define MASK_DATATYPE_ELEMENT_1		DATATYPE_ELEMENT_MASK_1
#define MASK_DATATYPE_ELEMENT_2		DATATYPE_ELEMENT_MASK_2
#define MASK_DATATYPE_ELEMENT_3		DATATYPE_ELEMENT_MASK_3
#define MASK_DATATYPE_ELEMENT_4		DATATYPE_ELEMENT_MASK_4
#define MASK_DATATYPE_ELEMENT_5		DATATYPE_ELEMENT_MASK_5
#define MASK_DATATYPE_ELEMENT_6		DATATYPE_ELEMENT_MASK_6
#define MASK_DATATYPE_ELEMENT_7		DATATYPE_ELEMENT_MASK_7
#define MASK_DATATYPE_ELEMENT_8		DATATYPE_ELEMENT_MASK_8
#define MASK_DATATYPE_ELEMENT_9		DATATYPE_ELEMENT_MASK_9
#define MASK_DATATYPE_ELEMENT_10	DATATYPE_ELEMENT_MASK_10


//空白函数指针占位符
#define NULL_1	 NULL
#define NULL_2	 NULL,NULL
#define NULL_3	 NULL_2,NULL_1
#define NULL_4	 NULL_3,NULL_1
#define NULL_5	 NULL_4,NULL_1
#define NULL_6	 NULL_5,NULL_1
#define NULL_7	 NULL_6,NULL_1
#define NULL_8	 NULL_7,NULL_1
#define NULL_9	 NULL_8,NULL_1
#define NULL_10	 NULL_9,NULL_1

#define	MASK_NULL_1		NULL_1
#define MASK_NULL_2		NULL_2
#define MASK_NULL_3		NULL_3
#define MASK_NULL_4		NULL_4
#define MASK_NULL_5		NULL_5
#define MASK_NULL_6		NULL_6
#define MASK_NULL_7		NULL_7
#define MASK_NULL_8		NULL_8
#define MASK_NULL_9		NULL_9
#define MASK_NULL_10	NULL_10


#endif //__FN_SHARE_H
