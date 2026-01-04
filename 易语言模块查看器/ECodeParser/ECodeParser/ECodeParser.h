#pragma once
#include "efs.h"
using namespace std;

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


typedef void(__stdcall* EVENT)(void* obj);

class __declspec(dllexport) ECodeParser
{
protected:
	ECodeParser() { ; }
public:
	ECodeParser(LPCSTR eroot);
	~ECodeParser();
	void AddEvent(LPCSTR name, EVENT e);
	void CallEvent(LPCSTR name, LPVOID obj);

	// 解密源码,把源码所有数据都解密存放到 EInfo指针,并返回该指针
	// data	= 易语言源码数据,.e实际数据
	// size	= .e文件的尺寸
	// enc = 是否分析的为源码，如果是源码，则使用假。如果分析的为剪辑版的数据，则需要设置为真。
	EInfo *Parse(LPBYTE data, size_t size, BOOL enc);
	LPBYTE GetDerived(ETAG tag);
	EBase *FindEBase(ETAG tag);
	LPSTR EMethod2Code(ETAG tag);
	LPSTR EMethodCode2Code(ETAG tag);
	
	HWND EForm2Window(ETAG tag);
	// 数据类型到文本类型
	string Type2Name(DataType type);

protected:
	EInfo _einfo;
	string _eroot;
	map<UINT, vector<UINT>> _inherit;
	map<string, PLIB_INFO> _libInfo;

	vector<EVENT> _info;
	vector<EVENT> _lib;
	vector<EVENT> _ec;
	vector<EVENT> _window;
	vector<EVENT> _class;
	vector<EVENT> _assembly;
	vector<EVENT> _method;
	vector<EVENT> _struct;
	vector<EVENT> _dll;
	vector<EVENT> _global;
	vector<EVENT> _const;
	vector<EVENT> _resource;
	vector<EVENT> _end;

	void AddDerived(ETAG key, ETAG value);
	void EnumDerived(vector<UINT>& list, ETAG tag);
	PLIB_INFO FindLibInfo(string &libName);
	PCMD_INFO FindLibCmd(USHORT libID, UINT id);
	PLIB_DATA_TYPE_INFO FindLibDataType(USHORT libID, USHORT id);
	PLIB_CONST_INFO FindLibConst(USHORT libID, USHORT id);
	string CustomType2Name(ETAG type);
	string Var2Name(ETAG tag, ESection_Program_Method &method);
	string Method2Name(ETAG tag, short libID);
	string Const2Name(ETAG tag);
	string LibConst2Name(LIBCONST libConst);
	string LibConstStruct2Name(LIBCONSTSTRUCT libConst);
	string DateTime2Str(double dt);
	string Num2Str(double num);
	string FormatMethodCode(ETAG tag, short libID, vector<string> &params);
	string Var2Str(ESection_Program_Method &method, LPBYTE &code, LPBYTE end);


	string Call2Str(ESection_Program_Method &method, LPBYTE &code, LPBYTE end);

	// 把数据转换成文本的易语言代码,不包含子程序头部,参数,变量
	string Code2Str(ESection_Program_Method &method);

	// 获取变量/参数/成员数据,包括名字,数据类型,备注,属性
	void GetVariables(LPBYTE &pointer, vector<ESection_Variable> *result = NULL);

	// 取程序集数据,包括程序集变量,
	void GetAssemblies(LPBYTE &pointer, vector<ESection_TagStatus> &tagstatus, vector<ESection_Program_Assembly> *result, vector<ESection_Program_Assembly> *refer = NULL, bool isclass = true);

	// 取子程序数据,包括dll名,调用的库,调用的名字,备注,参数
	void GetMethods(LPBYTE &pointer, vector<ESection_Program_Method> *result, map<UINT,int> *methods_map , vector<ESection_Program_Method> *refer = NULL, bool callback = true);

	// 取DLL数据,包括dll名,调用的库,调用的名字,备注,参数
	void GetDlls(LPBYTE &pointer, vector<ESection_Program_Dll> *result);

	void GetElements(LPBYTE &pointer, vector<ESection_Resources_FormElement> *result = NULL);

	// 获取窗口数据
	void GetForms(LPBYTE &pointer, vector<ESection_Resources_Form> *result, bool jump = false);

	// 获取常量数据
	void GetConst(LPBYTE &pointer, ESection_ResourcesConst *result);

	// 获取用户段, 在易语言 程序->配置 里的数据
	void GetUserInfo(LPBYTE pointer, ESection_UserInfo *result);

	// 获取源码程序段的信息,由 Parse() 调用
	void GetLibraries(LPBYTE pointer, vector<ESection_TagStatus> &tagstatus, ESection_Program *result);

	// 获取剪辑版源码的信息,由 Parse() 调用
	void GetLibraries2(LPBYTE pointer, vector<ESection_TagStatus> &tagstatus, ESection_Program *result);
	void GetECList(LPBYTE pointer, ESection_ECList *result);

	//  获取程序资源段,包括窗口,资源,常量
	void GetResources(LPBYTE pointer, ESection_Resources *result);
};

class __declspec(dllexport) ECode : public ECodeParser
{
public:
	ECode(LPCSTR eroot) :ECodeParser(eroot) { ; }
	// 获取信息,并写入到文件, 包括用户配置,加载的模块,加载的支持库,参数为保存到的文件路径
	string getSysInfo();
	// 取dll命令信息,参数为保存到的文件路径
	string getDllInfo();
	// 取定义的数据类型,参数为保存到的文件路径
	string getDataType();
	// 取全局变量,参数为保存到的文件路径
	string getGlobal();
	// 取程序集,类,窗口程序集,参数为保存到的文件路径
	vector<string> getAssemblies();
	// 取常量数据
	string getConstInfo();
	// 保存解析好的源码, 失败返回错误码, -1=路径错误,-2=传入的文件不存在,-3=创建保存目录失败,-4=载入文件失败
	int saveECode(const string& eFile, int mode);


public:
	ESection_Program_Assembly* getMethod(ETAG tag);
protected:
	void addString(string& str, LONGLONG n);
	void addString(string& str, string n);
	string __getArr(ESection_ArrayInfo& arr);

};



EXPORT ECode* ECodeParser_Create(LPCSTR eroot);
EXPORT void ECodeParser_Destroy(ECode* parser);
EXPORT void ECodeParser_AddEvent(ECode* parser, LPCSTR name, EVENT e);
EXPORT EInfo* ECodeParser_Parse(ECode* parser, LPBYTE data, size_t size, BOOL enc);
EXPORT LPBYTE ECodeParser_GetDerived(ECode* parser, ETAG tag);
EXPORT EBase* ECodeParser_FindEBase(ECode* parser, ETAG tag);
EXPORT LPSTR ECodeParser_EMethod2Code(ECode* parser, ETAG tag);
EXPORT LPSTR ECodeParser_EMethodCode2Code(ECode* parser, ETAG tag);
EXPORT HWND ECodeParser_EForm2Window(ECode* parser, ETAG tag);
EXPORT LPBYTE Ptr2Arr(vector<BYTE>* v, int size);
EXPORT LPBYTE Ptr2Bin(vector<BYTE>* v);
EXPORT LPSTR Ptr2Str(string* s);
EXPORT void FreePtr(LPVOID ptr);
EXPORT void FreeEBase(EBase* ptr);



// 获取信息,并返回数据, 包括用户配置,加载的模块,加载的支持库
EXPORT LPSTR getSysInfo(ECode* parser);
// 取dll命令信息
EXPORT LPSTR getDllInfo(ECode* parser);
// 取定义的数据类型
EXPORT LPSTR getDataType(ECode* parser);
// 取全局变量
EXPORT LPSTR getGlobal(ECode* parser);
// 取程序集,类,窗口程序集
EXPORT LPSTR getAssemblies(ECode* parser);

// 写到文件
EXPORT BOOL writeFile(const string& file, const string& data);
