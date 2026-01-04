#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <map>

#pragma region efs
#include "../elib/lib2.h"

enum ECode_Head : BYTE
{
	IfeElse = 0x50,
	IfePreEnd = 0x51,
	IfPreEnd = 0x52,
	SwitchCase = 0x53,
	SwitchPreEnd = 0x54,
	Call = 0x6A,
	Ife = 0x6B,
	If = 0x6C,
	SwitchBegin = 0x6D,
	Switch = 0x6E,
	SwitchDefault = 0x6F,
	LoopBegin = 0x70,
	LoopEnd = 0x71,
	IfeEnd = 0x72,
	IfEnd = 0x73,
	SwitchEnd = 0x74
};

enum class ECode_Type : BYTE
{
	ParamBegin = 0x36,
	ParamEnd = 0x01,
	NullParam = 0x16,
	NumberConst = 0x17,
	BoolConst = 0x18,
	DatetimeConst = 0x19,
	StringConst = 0x1A,
	Const = 0x1B,
	LibConst = 0x1C,
	Variable = 0x1D,
	n4 = 0x1E,
	BinConst = 0x1F,
	BinEnd = 0x20,
	Method = 0x21,
	LibConstStruct = 0x23,
	ParameterBegin = 0x38,
	ParameterEnd = 0x37,
	Field = 0x39,
	ArrayBegin = 0x3A,
	NumberIndex = 0x3B
};

enum class EProcess_Tag : BYTE
{
	Else = 0x50,
	IfeEnd = 0x51,
	IfEnd = 0x52,
	Case = 0x53,
	SwitchEnd = 0x54
};

enum FileType : UINT
{
	E = 0x01,
	EC = 0x03
};

enum CompileType : UINT
{
	WindowsForm = 0x00,
	WindowsConsole = 0x01,
	WindowsDLL = 0x02,
	WindowsEC = 0x03E8,
	LinuxConsole = 0x2710,
	LinuxEC = 0x2AF8
};

enum DataType : DWORD
{
	EDT_VOID = _SDT_NULL,
	EDT_ALL = _SDT_ALL,
	EDT_BYTE = SDT_BYTE,
	EDT_SHORT = SDT_SHORT,
	EDT_INT = SDT_INT,
	EDT_LONG = SDT_INT64,
	EDT_FLOAT = SDT_FLOAT,
	EDT_DOUBLE = SDT_DOUBLE,
	EDT_BOOL = SDT_BOOL,
	EDT_DATETIME = SDT_DATE_TIME,
	EDT_TEXT = SDT_TEXT,
	EDT_BIN = SDT_BIN,
	EDT_SUBPTR = SDT_SUB_PTR
};

enum ETYPE : BYTE
{
	StaticClass = 0x09,
	WindowClass = 0x19,
	Class = 0x49,
	Struct = 0x41,
	Variable = 0x25,
	Field = 0x15,
	GlobalField = 0x05,
	Method = 0x04,
	Window = 0x52,
	Unit = 0x06,
	Element = 0x16,
	Const = 0x18,
	Image = 0x28,
	Music = 0x38,
	Dll = 0x0A
};

enum EMethodAttr : UINT
{
	M_None = 0x00,
	M_Public = 0x08,
	M_Extern = 0x80
};

enum EVariableAttr : USHORT
{
	V_None = 0x00,
	V_Static = 0x01,
	V_Out = 0x02,
	V_Optional = 0x04,
	V_Array = 0x08,
	V_Public = 0x0100,
	V_Extern = 0x0200
};

enum ETagStatus : UINT
{
	C_None = 0x00,
	C_Public = 0x01,
	C_Extern = 0x02
};

enum Cursor : UINT
{
	C_ARROW = (UINT)IDC_ARROW,
	C_IBEAM = (UINT)IDC_IBEAM,
	C_WAIT = (UINT)IDC_WAIT,
	C_CROSS = (UINT)IDC_CROSS,
	C_UPARROW = (UINT)IDC_UPARROW,
	C_SIZE = (UINT)IDC_SIZE,
	C_ICON = (UINT)IDC_ICON,
	C_SIZENWSE = (UINT)IDC_SIZENWSE,
	C_SIZENESW = (UINT)IDC_SIZENESW,
	C_SIZEWE = (UINT)IDC_SIZEWE,
	C_SIZENS = (UINT)IDC_SIZENS,
	C_SIZEALL = (UINT)IDC_SIZEALL,
	C_NO = (UINT)IDC_NO,
	C_HAND = (UINT)IDC_HAND,
	C_APPSTARTING = (UINT)IDC_APPSTARTING,
	C_HELP = (UINT)IDC_HELP
};

enum EElementStatus : UINT
{
	E_None = 0x00,
	E_Visible = 0x01,
	E_Disable = 0x02
};

enum EConstAttr : USHORT
{
	CA_Public = 0x02,
	CA_MultiLine = 0x10
};

enum EConstType : BYTE
{
	CT_Void = 0x16,
	CT_Number = 0x17,
	CT_Bool = 0x18,
	CT_DateTime = 0x19,
	CT_Text = 0x1A
};

struct ETAG
{
	USHORT ID;
	BYTE Type1;
	ETYPE Type2;
};

struct LIBTAG
{
	USHORT ID;
	USHORT LibID;
};

struct LIBCONST
{
	USHORT LibID;
	USHORT ID;
};

struct LIBCONSTSTRUCT
{
	LIBTAG LibType;
	UINT Index;
};

struct EKeyValPair
{
	ETAG Value;
	ETAG Key;
};

struct EFile_Header
{
	BYTE Magic1[4];
	BYTE Magic2[4];
};

struct ESection_Header
{
	BYTE Magic[4];
	UINT Info_CheckSum;
};

struct ESection_Info
{
	BYTE Key[4];
	BYTE Name[30];
	short reserve_fill_1;
	UINT Index;
	UINT CodeType;
	UINT Data_CheckSum;
	UINT DataLength;
	int reserve_item[10];
};

struct ESection_SystemInfo
{
	USHORT Version1;
	USHORT Version2;
	int Unknow_1;
	int Unknow_2;
	int Unknow_3;
	FileType FileType;
	int Unknow_4;
	CompileType CompileType;
	int Unknow_5[8];
};

struct ESection_UserInfo
{
	std::string ProjectName;
	std::string Remark;
	std::string Author;
	std::string ZipCode;
	std::string Address;
	std::string Phone;
	std::string Fax;
	std::string Email;
	std::string HomePage;
	std::string Copyright;
	UINT Major;
	UINT Minor;
	UINT Build;
	UINT Revision;
	std::string PluginName;
	ESection_UserInfo() {
		Major = 0;
		Minor = 0;
		Build = 0;
		Revision = 0;
	}
};

struct ESection_ArrayInfo
{
	UINT Dimension;
	std::vector<UINT> Subscript;
};

struct ESection_Library
{
	std::string FileName;
	std::string Guid;
	int Major;
	int Minor;
	std::string Name;
};

struct EBase
{
	ETAG Tag;
};

struct ESection_Variable : EBase
{
	DataType DataType;
	EVariableAttr Attributes;
	ESection_ArrayInfo ArrayInfo;
	std::string Name;
	std::string Remark;
};

struct ESection_Program_Assembly : EBase
{
	ETagStatus Status;
	ETAG Base;
	std::string Name;
	std::string Remark;
	std::vector<ETAG> Methods;
	std::vector<ESection_Variable> Variables;
};

struct ESection_Program_Method : EBase
{
	ETAG Class;
	EMethodAttr Attributes;
	DataType ReturnType;
	std::string Name;
	std::string Remark;
	std::vector<ESection_Variable> Variables;
	std::vector<ESection_Variable> Parameters;
	std::vector<BYTE> RowOffset;
	std::vector<BYTE> FlowOffset;
	std::vector<BYTE> MethodOffset;
	std::vector<BYTE> VariableOffset;
	std::vector<BYTE> ParameterOffset;
	std::vector<BYTE> Code;
};

struct ESection_Program_Dll : EBase
{
	ETagStatus Status;
	DataType ReturnType;
	std::string ShowName;
	std::string Remark;
	std::string Lib;
	std::string Name;
	std::vector<ESection_Variable> Parameters;
};

struct ESection_Program
{
	std::vector<ESection_Library> Libraries;
	std::vector<ESection_Program_Assembly> Assemblies;
	std::vector<ESection_Program_Assembly> ReferAssemblies;
	std::vector<ESection_Program_Method> Methods;
	std::vector<ESection_Program_Method> ReferMethods;
	std::vector<ESection_Variable> GlobalVariables;
	std::vector<ESection_Variable> ReferGlobalVariables;
	std::vector<ESection_Program_Assembly> Structs;
	std::vector<ESection_Program_Assembly> ReferStructs;
	std::vector<ESection_Program_Dll> Dlls;
};

struct ESection_TagStatus : EBase
{
	ETagStatus Status;
};

struct ESection_AuxiliaryInfo2
{
	std::vector<ESection_TagStatus> Tags;
};

struct ESection_ECList_Info
{
	std::string Name;
	std::string Path;
};

struct ESection_ECList
{
	std::vector<ESection_ECList_Info> List;
};

struct ESection_Resources_FormElement : EBase
{
	std::string Name;
	std::string Remark;
	LIBTAG Type;
	UINT Left;
	UINT Top;
	UINT Width;
	UINT Height;
	std::vector<BYTE> Cursor;
	std::string Mark;
	EElementStatus Status;
	std::vector<EKeyValPair> Events;
	std::vector<BYTE> Data;
};

struct ESection_Resources_FormMenu : EBase
{
	std::string Name;
	std::string Title;
	ETAG Type;

};

struct ESection_Resources_Form : EBase
{
	std::string Name;
	std::string Remark;
	ETAG Class;
	std::vector<ESection_Resources_FormElement> Elements;
};

struct ESection_Resources_Base : EBase
{
	EConstAttr Attr;
	std::string Name;
	std::string Remark;
};

struct ESection_Resources_Const : ESection_Resources_Base
{
	EConstType Type;
	std::vector<BYTE> Data;
};

struct ESection_Resources_Data : ESection_Resources_Base
{
	std::vector<BYTE> Data;
};

struct ESection_ResourcesConst
{
	std::vector<ESection_Resources_Const> Const;
	std::vector<ESection_Resources_Data> Data;
};

struct ESection_Resources
{
	std::vector<ESection_Resources_Form> Forms;
	ESection_ResourcesConst Constants;
};

struct ESection_UnitInfo
{
	ETAG Form;
	ETAG Element;
	UINT Index;
	ETAG Method;
};

struct ESection_AuxiliaryInfo1
{
	std::vector<ESection_UnitInfo> UnitInfos;
};

const BYTE Magic1[4] = { 'C', 'N', 'W', 'T' };
const BYTE Magic2[4] = { 'E', 'P', 'R', 'G' };
const BYTE Magic_Encrypt[4] = { 'W', 'T', 'L', 'E' };
const BYTE Magic_Section[4] = { 0x19, 0x73, 0x11, 0x15 };
const BYTE KEY[4] = { 25, 115, 0, 7 };

#define SP "\r"
#define REMARK -1
#define CUSTOM -2
#define DLL -3
#define ETAG2UINT(etag) (UINT)MAKELONG(etag.ID, MAKEWORD(etag.Type1, etag.Type2))
#pragma endregion

#define EVENT_INFO "info"
#define EVENT_LIB "lib"
#define EVENT_EC "ec"
#define EVENT_WINDOW "window"
#define EVENT_CLASS "class"
#define EVENT_ASSEMBLY "assembly"
#define EVENT_METHOD "method"
#define EVENT_STRUCT "struct"
#define EVENT_DLL "dll"
#define EVENT_GLOBAL "global"
#define EVENT_CONST "const"
#define EVENT_RESOURCE "resource"
#define EVENT_END "end"

typedef void(__stdcall * EVENT)(void* obj);

struct EInfo
{
	ESection_SystemInfo SystemInfo;
	ESection_UserInfo UserInfo;
	ESection_Program Program;
	ESection_AuxiliaryInfo2 TagStatus;
	ESection_ECList ECList;
	ESection_Resources Resources;
	ESection_AuxiliaryInfo1 UnitInfo;
};
