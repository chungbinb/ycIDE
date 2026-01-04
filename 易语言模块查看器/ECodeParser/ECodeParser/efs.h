//易源码结构
#pragma once
#pragma warning (disable:4309)
#pragma warning (disable:4369)
#pragma warning (disable:4251)
#pragma warning (disable:4018)

#include "../elib/lib2.h"

using namespace std;



#pragma region enum
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
	ParamBegin = 0x36,	// 参数开始
	ParamEnd = 0x01,	// 参数结束
	NullParam = 0x16,	// 空参数
	NumberConst = 0x17,	// 数值常量
	BoolConst = 0x18,	// 逻辑型常量
	DatetimeConst = 0x19,	// 时间常量
	StringConst = 0x1A,	// 文本常量
	Const = 0x1B,		// 
	LibConst = 0x1C,
	Variable = 0x1D,	// 变量
	n4 = 0x1E,
	BinConst = 0x1F,	// 字节集常量
	BinEnd = 0x20,		// 字节集常量结束
	Method = 0x21,		// 
	LibConstStruct = 0x23,
	ParameterBegin = 0x38,	// 参数开始
	ParameterEnd = 0x37,	// 参数结束
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

enum DataType : UINT
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
	StaticClass = 0x09,	// 程序集
	WindowClass = 0x19,	// 窗口程序集
	Class = 0x49,		// 类
	Struct = 0x41,
	GlobalField = 0x05,
	Field = 0x15,
	Variable = 0x25,
	Member = 0x35,
	Parameter = 0x45,
	Method = 0x04,
	Window = 0x52,
	Unit = 0x06,
	Element = 0x16,
	Const = 0x18,
	Image = 0x28,
	Music = 0x38,
	Dll = 0x0A
};

// 记录子程序/方法 是否公开,是否是外部的
enum EMethodAttr : UINT
{
	M_None = 0x00,
	M_Public = 0x08,
	M_Contraction = 0x30,
	M_Extern = 0x80
};

// 变量,参数属性, 可空,参考,数组,传址,公开,无
enum EVariableAttr : USHORT
{
	V_None = 0x00,		// 默认
	V_Static = 0x01,	// 静态
	V_Out = 0x02,		// 参考/传址
	V_Optional = 0x04,	// 可空
	V_Array = 0x08,		// 数组
	V_Public = 0x0100,
	V_Extern = 0x0200
};

// 状态, 公开/外部/无
enum ETagStatus : UINT
{
	C_None = 0x00,
	C_Public = 0x01,
	C_Extern = 0x02,
	C_Hide = 0x04
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
	CA_None = 0x00,
	CA_Public = 0x02,
	CA_Extern = 0x04,
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
#pragma endregion


struct ETAG
{
	USHORT ID;		// ID号
	BYTE Type1;		// 
	ETYPE Type2;	// 标签的类型
	ETAG();
	ETAG(UINT uint);
	bool operator==(ETAG tag);
	bool operator!=(ETAG tag);
	bool operator==(UINT uint);
	bool operator!=(UINT uint);
	bool operator==(int uint);
	bool operator!=(int uint);
	operator UINT();
};

struct LIBTAG
{
	USHORT ID;
	USHORT LibID;
	LIBTAG();
	LIBTAG(UINT uint);
	operator UINT();
};

struct LIBCONST
{
	USHORT LibID;
	USHORT ID;
	LIBCONST();
	LIBCONST(UINT uint);
	operator UINT();
};

struct LIBCONSTSTRUCT
{
	LIBTAG LibType;
	UINT Index;
	LIBCONSTSTRUCT();
	LIBCONSTSTRUCT(UINT64 uint);
	operator UINT64();
};

struct EKeyValPair
{
	ETAG Value;
	ETAG Key;
	EKeyValPair();
	EKeyValPair(UINT64 uint);
	operator UINT64();
};

struct EFile_Header		// 易语言源码头,前8个字节
{
	BYTE Magic1[4];		// CNWT
	BYTE Magic2[4];		// EPRG
};

struct ESection_Header
{
	BYTE Magic[4];		// { 0x19, 0x73, 0x11, 0x15 } {25, 115, 17, 21}
	UINT Info_CheckSum;	// 通过计算ESection_Info的数据而来。GetCheckValue(ESection_Info,sizeof(ESection_Info))
};

struct ESection_Info	// 
{
	BYTE Key[4];		// 解密key
	BYTE Name[30];		// 段名称
	short reserve_fill_1;// 保留
	UINT Index;			// 该段的位置，从1开始，系统信息段为1，用户为2，依次往后排。
	UINT CodeType;		// 代码类型，根据实际段内容设置，EC或E
	UINT Data_CheckSum;	// 将该段的信息(data)，进行加密，得到校验和。 GetCheckValue(data,DataLength)
	UINT DataLength;	// 当前段data信息长度。
	int reserve_item[10];//保留
};

// 系统信息,包括易语言主版本,文件类型,编译类型
struct ESection_SystemInfo
{
	USHORT Version1;	// 易语言主版本
	USHORT Version2;	// 易语言次版本
	int Unknow_1;		// 备用
	int Unknow_2;		// 备用
	int Unknow_3;		// 备用
	FileType fileType;	// 文件类型,是模块还是.e源码
	int Unknow_4;		// 备用
	CompileType compileType;	// 编译类型, exe窗口程序, 控制台, dll, ec模块, linux控制台, Linux模块
	int Unknow_5[8];	//备用
	ESection_SystemInfo() {
		memset(this, 0, sizeof(ESection_SystemInfo));
		/*Version1 = Version2 = Unknow_1 = Unknow_2 = Unknow_3 = Unknow_4;
		fileType = (FileType)0;
		compileType = (CompileType)0;
		memset(Unknow_5, 0, sizeof(Unknow_5));*/
	}
};


// 源码配置信息, 文件->配置 这里面的信息
struct ESection_UserInfo
{
	std::string ProjectName;	// 程序名称, 文件->配置->程序名称
	std::string Remark;			// 程序描述, 文件->配置->程序描述
	std::string Author;			// 作者
	std::string ZipCode;		// 邮政编码
	std::string Address;		// 联系地址
	std::string Phone;			// 电话号码
	std::string Fax;			// 传真
	std::string Email;			// 邮箱
	std::string HomePage;		// 主页
	std::string Copyright;		// 版权声明
	UINT Major;					// 程序主版本号
	UINT Minor;					// 程序次版本号
	UINT Build;					// 主创建号
	UINT Revision;				// 次创建号
	std::string PluginName;		// 所使用编译插件名
	ESection_UserInfo() { Major = Minor = Build = Revision = 0; }
};

// 数组信息,记录数组维数和每个维数的成员数
struct ESection_ArrayInfo
{
	UINT Dimension;			// 数组维数
	vector<UINT> Subscript;	// 每一维的成员数
};

// 支持库信息,记录库名,GUID,版本号
struct ESection_Library
{
	string FileName;	// 支持库名,比如 krnln
	string Guid;		// 支持库guid
	int Major;			// 主版本号
	int Minor;			// 次版本号
	string Name;		// 支持库名字, 比如 系统核心支持库
	ESection_Library();
	ESection_Library(nullptr_t);
	bool operator==(nullptr_t);
	bool operator!=(nullptr_t);
};

// 标签
struct EBase
{
	ETAG Tag;
	bool operator==(nullptr_t);
	bool operator!=(nullptr_t);
	static EBase Null;
};

// 参数/变量/数据结构成员 定义
struct ESection_Variable : EBase
{
	DataType DataType;				// 参数类型
	EVariableAttr Attributes;		// 属性, 可空,参考,数组,公开,传址
	ESection_ArrayInfo ArrayInfo;	// 数组信息
	string Name;					// 参数名
	string Remark;					// 参数备注
	ESection_Variable();
	ESection_Variable(nullptr_t);
};

// 程序集/类信息, 记录所有子程序/方法的数据,变量数据,是否公开
struct ESection_Program_Assembly : EBase
{
	ETagStatus Status;		// 状态, 公开/外部/无
	ETAG Base;				// 父类, 应该只有类有这个
	string Name;			// 名称
	string Remark;			// 备注
	vector<ETAG> Methods;	// 程序集/类 里的所有子程序/方法 的标签
	vector<ESection_Variable> Variables;	// 类成员变量/程序集变量/变量/参数/数据结构成员 个数
	ESection_Program_Assembly();
	ESection_Program_Assembly(nullptr_t);
	ESection_Variable* FindField(ETAG tag);
};

// 子程序信息,记录子程序的状态,名字,备注,参数,变量,代码
struct ESection_Program_Method : EBase
{
	ETAG Class;					// 标签
	EMethodAttr Attributes;		// 状态 公开/外部/无
	DataType ReturnType;		// 返回值类型
	string Name;				// 子程序名字  _启动子程序
	string Remark;				// 备注
	vector<ESection_Variable> Variables;	// 子程序下的参数/变量定义
	vector<ESection_Variable> Parameters;	// 子程序下的参数/变量定义
	vector<BYTE> RowOffset;		// 行偏移
	vector<BYTE> FlowOffset;	// 
	vector<BYTE> MethodOffset;
	vector<BYTE> VariableOffset;
	vector<BYTE> ParameterOffset;
	vector<BYTE> Code;
	ESection_Program_Method();
	ESection_Program_Method(nullptr_t);
};

// DLL信息
struct ESection_Program_Dll : EBase
{
	ETagStatus Status;		// 状态, 公开/外部/无, C_Hide是外部
	DataType ReturnType;	// 返回值类型
	string ShowName;		// 显示在易语言里的DLL名字
	string Remark;			// 备注
	string Lib;				// 引用的库, 比如 user32.dll
	string Name;			// 实际调用的dll命令名
	vector<ESection_Variable> Parameters;	// DLL参数信息
};

// 源码调用的支持库,源码里的程序集数据,子程序数据,全局变量,数据类型,DLL信息
struct ESection_Program
{
	vector<ESection_Library> Libraries;					// 源码使用的支持库信息
	vector<ESection_Program_Assembly> Assemblies;		// 源码里所有的程序集信息,包括加载的模块里的程序集信息
	vector<ESection_Program_Assembly> ReferAssemblies;	// 参考程序集??
	vector<ESection_Program_Method> Methods;			// 子程序信息,
	map<UINT, int>	Methods_map;						//这个是专门为子程序信息建立的索引数组，解决当命令很多时查找慢的问题。
	vector<ESection_Program_Method> ReferMethods;		// 
	vector<ESection_Variable> GlobalVariables;			// 全局变量,包括模块里的全局变量
	vector<ESection_Variable> ReferGlobalVariables;		// 
	vector<ESection_Program_Assembly> Structs;			// 定义的数据结构,包括模块里定义的结构
	vector<ESection_Program_Assembly> ReferStructs;		// 
	vector<ESection_Program_Dll> Dlls;					// DLL信息,包括自己的和模块里的
};

struct ESection_TagStatus : EBase
{
	ETagStatus Status;
	ESection_TagStatus();
	ESection_TagStatus(nullptr_t);
};

struct ESection_AuxiliaryInfo2
{
	vector<ESection_TagStatus> Tags;
};

// 加载的模块信息
struct ESection_ECList_Info
{
	string Name;	// 模块名字
	string Path;	// 模块路径
};

// 加载的模块列表
struct ESection_ECList
{
	vector<ESection_ECList_Info> List;
};

struct ESection_Resources_FormElement : EBase
{
	string Name;
	string Remark;
	LIBTAG Type;
	UINT Left;
	UINT Top;
	UINT Width;
	UINT Height;
	vector<BYTE> Cursor;
	string Mark;
	EElementStatus Status;
	vector<EKeyValPair> Events;
	vector<BYTE> Data;
	ESection_Resources_FormElement();
	ESection_Resources_FormElement(nullptr_t);
};

struct ESection_Resources_FormMenu : EBase
{
	string Name;
	string Title;
	ETAG Type;

};

struct ESection_Resources_Form : EBase
{
	string Name;	// 窗口名字
	string Remark;	// 窗口备注
	ETAG Class;		// 
	vector<ESection_Resources_FormElement> Elements;
	ESection_Resources_Form();
	ESection_Resources_Form(nullptr_t);
};

// 常量信息
struct ESection_Resources_Base : EBase
{
	EConstAttr Attr;
	string Name;	// 常量/资源 名
	string Remark;	// 常量/资源 备注
};

// 常量信息
struct ESection_Resources_Const : ESection_Resources_Base
{
	EConstType Type;	// 常量类型	CT_***
	vector<BYTE> Data;	// 常量数据
	ESection_Resources_Const();
	ESection_Resources_Const(nullptr_t);
};

// 记录资源数据,资源名字,备注,数据,
struct ESection_Resources_Data : ESection_Resources_Base
{
	vector<BYTE> Data;	// 资源数据
	ESection_Resources_Data();
	ESection_Resources_Data(nullptr_t);
};

struct ESection_ResourcesConst
{
	vector<ESection_Resources_Const> Const;	// 常量状态,常量类型,常量名,常量数据
	vector<ESection_Resources_Data> Data;	// 声音/图片资源
};

struct ESection_Resources
{
	vector<ESection_Resources_Form> Forms;	// 窗口
	ESection_ResourcesConst Constants;		// 声音,图片 资源
};

struct ESection_UnitInfo
{
	ETAG Form;
	ETAG Element;
	UINT Index;
	ETAG Method;
	ESection_UnitInfo() { memset(this, 0, sizeof(ESection_UnitInfo)); }
};

struct ESection_AuxiliaryInfo1
{
	vector<ESection_UnitInfo> UnitInfos;
};

struct EFlowOffset
{
	BYTE Type;
	EKeyValPair Range;
};

struct EInfo
{
	ESection_SystemInfo SystemInfo;		// 系统信息段, 包括易语言主版本,文件类型,编译类型
	ESection_UserInfo UserInfo;			// 用户配置信息, 源码配置信息, 文件->配置 这里面的信息
	ESection_Program Program;			// 源码调用的支持库,源码里的程序集数据,子程序数据,全局变量,数据类型,DLL信息
	ESection_AuxiliaryInfo2 TagStatus;	// 
	ESection_ECList ECList;				// 加载的模块列表
	ESection_Resources Resources;		// 程序资源段,常量,窗口
	ESection_AuxiliaryInfo1 UnitInfo;	// 
	EInfo() { memset(&SystemInfo, 0, sizeof(ESection_SystemInfo)); }
};

const BYTE Magic1[4] = { 'C', 'N', 'W', 'T' };
const BYTE Magic2[4] = { 'E', 'P', 'R', 'G' };
const BYTE Magic_Encrypt[4] = { 'W', 'T', 'L', 'E' };
const BYTE Magic_Section[4] = { 25, 115, 17, 21 };
const BYTE KEY[4] = { 25, 115, 0, 7 };

#define SP "\r"
#define REMARK -1
#define CUSTOM -2
#define DLL -3
#define ETAG2UINT(etag) (UINT)MAKELONG(etag.ID, MAKEWORD(etag.Type1, etag.Type2))

template<typename T>
T* FindInfo(vector<T>& list, ETAG tag)
{
	for (T& var : list)
		if (static_cast<EBase>(var).Tag == tag)
			return &var;
	return NULL;
}

