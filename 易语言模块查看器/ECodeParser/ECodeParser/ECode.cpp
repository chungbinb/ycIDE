#include "stdafx.h"
#include "ECode.h"
#include <fstream>
#include <sstream>
#include <strstream>
#include <iostream>
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

BOOL writeFile(const string& file, const string& data)
{
	FILE* f;
	fopen_s(&f, file.c_str(), "w");
	if (!f)
	{
		cout << "打开文件:\t" << file << "失败\n";
		return 0;
	}

	fwrite(data.c_str(), sizeof(char), data.size() + 1, f);
	fclose(f);
	return TRUE;
}


string num2string(double num)
{
	string str;
	stringstream ss;
	ss << num;
	str = ss.str();
	return str;
}

string i2a(LONGLONG i)
{
	char num[50] = { 0 };
	sprintf_s(num, "%lld", i);
	return num;
}

// 获取信息,并写入到文件, 包括用户配置,加载的模块,加载的支持库
string ECode::getSysInfo()
{
	ESection_SystemInfo& sys = _einfo.SystemInfo;
	// 系统配置,
	string str("系统配置");
#pragma region sysinfo
	str.append("\n[易语言版本号]\n");
	str.append(i2a(sys.Version1));
	str.append(".");
	str.append(i2a(sys.Version2));
	str.append("\n[文件类型]\n");
	str.append((sys.fileType == E) ? ".e" : ".ec");
	str.append("\n[编译类型]\n");
	switch (sys.compileType)
	{
	case WindowsForm:
		str.append("窗口程序");
		break;
	case WindowsConsole:
		str.append("控制台程序");
		break;
	case WindowsDLL:
		str.append("DLL动态链接库");
		break;
	case WindowsEC:
		str.append("易模块");
		break;
	case LinuxConsole:
		str.append("Linux控制台程序");
		break;
	case LinuxEC:
		str.append("Linux模块");
		break;
	default:
		break;
	}
	str.append("\n\n");
#pragma endregion
	// 用户配置 文件->配置 这里面的信息
	ESection_UserInfo& user = _einfo.UserInfo;

#pragma region userinfo
	str.append("用户配置");
	str.append("\n[程序名称]\n");
	str.append(user.ProjectName);

	str.append("\n[程序描述]\n");
	str.append(user.Remark);

	str.append("\n[程序版本号]\n");
	str.append(i2a(user.Major));
	str.append(".");
	str.append(i2a(user.Minor));

	str.append("\n[创建号]\n");
	str.append(i2a(user.Build));
	str.append(".");
	str.append(i2a(user.Revision));

	str.append("\n[版权声明]\n");
	str.append(user.Copyright);

	str.append("\n[作者]\n");
	str.append(user.Author);

	str.append("\n[邮政编码]\n");
	str.append(user.ZipCode);

	str.append("\n[联系地址]\n");
	str.append(user.Address);

	str.append("\n[电话号码]\n");
	str.append(user.Phone);

	str.append("\n[传真]\n");
	str.append(user.Fax);

	str.append("\n[邮箱]\n");
	str.append(user.Email);

	str.append("\n[主页]\n");
	str.append(user.HomePage);

	str.append("\n[所使用编译插件名]\n");
	str.append(user.PluginName);

	str.append("\n\n");
#pragma endregion


	// 加载的模块
	ESection_ECList& ec = _einfo.ECList;
	str.append("模块引用列表");
#pragma region eclist
	for (auto i = ec.List.begin(); i < ec.List.end(); i++)
	{
		str.append("\n模块名字:");
		str.append(i->Name);
		str.append("\n模块路径:");
		str.append(i->Path);
	}
	str.append("\n\n");
#pragma endregion


	// 加载的模块
	vector<ESection_Library>& lib = _einfo.Program.Libraries;
	str.append("支持库引用列表");

#pragma region liblist
	for (auto i = lib.begin(); i < lib.end(); i++)
	{
		str.append("\n版本:");
		str.append(i2a(i->Major));
		str.append(".");
		str.append(i2a(i->Minor));
		str.append("\n支持库名:");
		str.append(i->Name);
		str.append("\n文件名字:");
		str.append(i->FileName);
		str.append("\n唯一标识:");
		str.append(i->Guid);
	}
#pragma endregion


	return str;
}

// 取dll命令信息
string ECode::getDllInfo()
{

	vector<ESection_Program_Dll>& Dlls = _einfo.Program.Dlls;
	//string str(".版本 2\n\n");	// 易语言代码头
	string str;	// 易语言代码头

	for (auto dll = Dlls.begin(); dll < Dlls.end(); dll++)
	{
		if (dll->Status & C_Hide)
			continue;

		str.append(".DLL命令 ");		// .DLL命令 
		str.append(dll->ShowName);	// .DLL命令 显示名字
		str.append(", ");			// .DLL命令 显示名字,
		str.append(Type2Name(dll->ReturnType));	// .DLL命令 显示名字, 整数型,
		str.append(",");			// .DLL命令 显示名字, 整数型,
		str.append("\"");			// .DLL命令 显示名字, 整数型,"
		str.append(dll->Lib);		// .DLL命令 显示名字, 整数型,"user32.dll
		str.append("\", ");			// .DLL命令 显示名字, 整数型,"user32.dll", 
		str.append("\"");			// .DLL命令 显示名字, 整数型,"user32.dll", "
		str.append(dll->Name);		// .DLL命令 显示名字, 整数型,"user32.dll", "CreatwWindowExW
		str.append("\", ");			// .DLL命令 显示名字, 整数型,"user32.dll", "CreatwWindowExW", 
		if (dll->Status&C_Public)
			str.append("公开, ");	// .DLL命令 显示名字, 整数型,"user32.dll", "CreatwWindowExW", 公开, 
		else
			str.append(", ");		// .DLL命令 显示名字, 整数型,"user32.dll", "CreatwWindowExW", , 
	
		str.append(dll->Remark);	// .DLL命令 显示名字, 整数型,"user32.dll", "CreatwWindowExW", , 备注
		str.append("\n");	// 换行,开始添加参数


		for (auto arg = dll->Parameters.begin(); arg < dll->Parameters.end(); arg++)
		{
			str.append("    .参数 ");	// .参数 
			str.append(arg->Name);		// .参数 参数名
			str.append(", ");			// .参数 参数名, 
			str.append(Type2Name(arg->DataType));	// .参数 参数名, 整数型
			str.append(", ");			// .参数 参数名, 整数型, 
			if (arg->Attributes & V_Out)
				str.append("传址 ");		// .参数 参数名, 整数型, 传址 
			if (arg->Attributes & V_Array)
				str.append("数组");		// .参数 参数名, 整数型, 数组
			str.append(", ");			// .参数 参数名, 整数型, 传址 数组, 
			str.append(arg->Remark);	// .参数 参数名, 整数型, 传址 数组, 备注
			str.append("\n");	// 换行,下一个参数继续
		}

		str.append("\n");	// 换行,开始下一个DLL命令

	}
	return str;
}

string ECode::getDataType()
{
	vector<ESection_Program_Assembly>& DataType = _einfo.Program.Structs;
	//string str(".版本 2\n\n");	// 易语言代码头
	string str;	// 易语言代码头

	for (auto type = DataType.begin(); type < DataType.end(); type++)
	{
		if (type->Status & C_Extern)
			continue;	// 外部的不处理,继续下一次循环

		str.append(".数据类型 ");		// .数据类型
		str.append(type->Name);		// .数据类型 LOGFONT
		str.append(", ");			// .数据类型 LOGFONT, 
		if (type->Status & C_Public)
			str.append("公开");		// .数据类型 LOGFONT, 公开
		str.append(", ");			// .数据类型 LOGFONT, 公开, 
		str.append(type->Remark);	// .数据类型 LOGFONT, 公开, 备注
		str.append("\n");	// 换行,开始加入成员

		for (auto arg = type->Variables.begin(); arg < type->Variables.end(); arg++)
		{
			//.成员 PropertyId, 整数型, 传址, "5654", 备注
			str.append("    .成员 ");	// .成员
			str.append(arg->Name);		// .成员 成员名
			str.append(", ");			// .成员 成员名, 
			str.append(Type2Name(arg->DataType));	// .成员 成员名, 整数型
			str.append(", ");			// .成员 成员名, 整数型, 
			if (arg->Attributes & V_Out)
				str.append("传址, ");	// .成员 成员名, 整数型, 传址, 
			if (arg->Attributes & V_Array)
			{
				str.append("\"");		// .成员 成员名, 整数型, 传址, "
				for (int i = 0; i < arg->ArrayInfo.Dimension; i++)
				{
					char tmpChar[20] = { 0 };
					_itoa_s(i, tmpChar, 10);
					str.append(tmpChar);// .成员 成员名, 整数型, 传址, "10
					if (i < arg->ArrayInfo.Dimension - 1)
					{
						str.append(",");// .成员 成员名, 整数型, 传址, "10,
					}

				}
				str.append("\""); str.append(",");// .成员 成员名, 整数型, 传址, "10,20"
			}
			str.append(", ");			// .成员 成员名, 整数型, 传址, "10,20", 
			str.append(arg->Remark);	// .成员 成员名, 整数型, 传址, "10,20", 备注
			str.append("\n");	// 换行,下一个成员继续
		}
		str.append("\n");	// 换行,洗衣歌数据类型继续
	}
	return str;
}

string ECode::getGlobal()
{
	vector<ESection_Variable>& Global = _einfo.Program.GlobalVariables;
	//string str(".版本 2\n\n");	// 易语言代码头
	string str;	// 易语言代码头

	for (auto g = Global.begin(); g < Global.end(); g++)
	{
		if (g->Attributes & V_Extern)
			continue;	// 外部的不加入

		// .全局变量 变量1, 整数型, 公开, "5,8", asdasda
		str.append(".全局变量 ");		// .全局变量
		str.append(g->Name);		// .全局变量 g_hWnd
		str.append(", ");			// .全局变量 g_hWnd,
		str.append(Type2Name(g->DataType));	// .全局变量 g_hWnd, 整数型
		str.append(", ");			// .全局变量 g_hWnd, 整数型,
		if (g->Attributes & V_Public)
			str.append("公开");		// .全局变量 g_hWnd, 整数型,公开
		str.append(", ");			// .全局变量 g_hWnd, 整数型,公开, 
		if (g->Attributes & V_Array)
		{
			str.append("\"");		// .全局变量 g_hWnd, 整数型,公开, "
			for (int i = 0; i < g->ArrayInfo.Dimension; i++)
			{
				char tmpChar[20] = { 0 };
				_itoa_s(i, tmpChar, 10);
				str.append(tmpChar);// .全局变量 g_hWnd, 整数型,公开, "10
				if (i < g->ArrayInfo.Dimension - 1)
				{
					str.append(",");// .全局变量 g_hWnd, 整数型,公开, "10,
				}

			}
			str.append("\"");		// .全局变量 g_hWnd, 整数型,公开, "10,10"
		}
		str.append(", ");			// .全局变量 g_hWnd, 整数型,公开, "10,10", 
		str.append(g->Remark);		// .全局变量 g_hWnd, 整数型,公开, "10,10", 备注
		str.append("\n");	// 换行处理下一个全局变量
	}
	return str;
}

vector<string> ECode::getAssemblies()
{
	vector<string> strs;
	vector<ESection_Program_Assembly>& Assembly = _einfo.Program.Assemblies;
	for (auto assembly = Assembly.begin(); assembly < Assembly.end(); assembly++)
	{
		//string str(".版本 2\n\n");	// 易语言代码头
		string str;	// 易语言代码头
		if (assembly->Methods.size() > 0)	// 子程序数量必须大于0才做判断
		{
			ESection_Program_Method* method = FindInfo(_einfo.Program.Methods, assembly->Methods[0]);
			// 取第一个子程序,查看是否为外部,是外部就跳出本次循环
			if (method->Attributes & M_Extern)
				continue;
		}
		else
		{
			// 走到这就是程序集没有子程序
			continue;
		}
		string file;
		switch (assembly->Tag.Type2)
		{
		case Class:
			file.append("类 ");
			break;
		case StaticClass:
			file.append("程序集 ");
			break;
		case WindowClass:
			file.append("窗口程序集 ");
			break;
		default:
			continue;	// 其他不处理,只处理程序集,类,窗口程序集
		}
		file = file + " " + assembly->Name;
		strs.push_back(file);

		// 程序集和类前缀都是程序集,程序集没有基类和公开
		str.append(".程序集 ");		// .程序集 
		str.append(assembly->Name);	// .程序集 测试程序集
		str.append(", ");			// .程序集 测试程序集, 

		ESection_Program_Assembly* m = getMethod(assembly->Base);
		if (m) str.append(m->Name);	// .程序集 测试程序集, 基类
		
		str.append(", ");			// .程序集 测试程序集, , 
		if (assembly->Status & C_Public)
			str.append("公开");		// .程序集 测试程序集, 基类, 公开
		
		str.append(", ");			// .程序集 测试程序集, , , 
		str.append(assembly->Remark);	// .程序集 测试程序集, , , 备注
		str.append("\n");	// 换行开始加入程序集变量

		for (auto var = assembly->Variables.begin(); var < assembly->Variables.end(); var++)
		{
			str.append(".程序集变量 ");	// .程序集变量 
			str.append(var->Name);		// .程序集变量 m_hWnd
			str.append(", ");			// .程序集变量 m_hWnd, 
			str.append(Type2Name(var->DataType));	// .程序集变量 m_hWnd, 整数型
			str.append(", ");			// .程序集变量 m_hWnd, 整数型, 
			// 这里应该是易语言要对其程序集的,所以留空了一个
			str.append(", ");			// .程序集变量 m_hWnd, 整数型, , 
			if (var->Attributes & V_Array)
			{
				str.append("\"");		// .程序集变量 m_hWnd, 整数型, , "
				str.append(__getArr(var->ArrayInfo));	// .程序集变量 m_hWnd, 整数型, , "10,20
				str.append("\"");		// .程序集变量 m_hWnd, 整数型, , "10,20"
			}
			str.append(", ");			// .程序集变量 m_hWnd, 整数型, , "10,20", 
			str.append(var->Remark);	// .程序集变量 m_hWnd, 整数型, , "10,20", 备注
			str.append("\n");	// 换行, 继续下一个程序集变量
		}
		str.append("\n");	// 换行, 开始加入子程序/方法
		
		// 循环把数组里的子程序都写出来
		for (auto fun = assembly->Methods.begin(); fun < assembly->Methods.end(); fun++)
		{
			LPSTR tmp = EMethod2Code(*fun);
			if (tmp)
			{
				str.append(tmp);
				LocalFree(tmp);
			}
		}
		strs.push_back(str);
		str = "";
	}
	
	return strs;
}

string ECode::getConstInfo()
{
	ESection_ResourcesConst& constInfo = _einfo.Resources.Constants;
	//.常量 a, "“123”", 公开, asd
	//.常量 bb, "123456", , asdwqw
	//.常量 bbc, "真"
	//.常量 asdasqwe, , 公开, asd
	//.常量 asdas
	//.常量 常量1, "<文本长度: 12>"
	//.常量 sj, "[2018年2月2日12时12分13秒]"


	string str;					// .常量 
	for (auto i = constInfo.Const.begin(); i < constInfo.Const.end(); i++)
	{
		if (i->Attr & CA_Extern)	// 外部的不处理
			continue;
		str.append(".常量 ");
		str.append(i->Name);	// .常量 WM_CLOSE
		str.append(", ");		// .常量 WM_CLOSE, 
		switch (i->Type)
		{
		case CT_Void:		// 空常量
			str.append(", ");
			break;
		case CT_Number:		// 数值型常量
		{
			str.append("\"");	// .常量 WM_CLOSE, "
			double num = *((double*)(&(i->Data[0])));
			char tmp[50] = { 0 };
			sprintf_s(tmp, "%lf", num);
			//str.append(num2string(num));	// .常量 WM_CLOSE, "16
			str.append(tmp);	// .常量 WM_CLOSE, "16
			str.append("\", ");	// .常量 WM_CLOSE, "16", 
		}
		break;
		case CT_Bool:		// 逻辑型常量
		{
			str.append("\"");	// .常量 WM_CLOSE, "
			str.append((i->Data[0] == 0) ? "假" : "真");	// .常量 WM_CLOSE, "真
			str.append("\", ");	// .常量 WM_CLOSE, "真", 
		}
		break;
		case CT_DateTime:	// 时间型常量
		{
			str.append("\"");	// .常量 WM_CLOSE, "
			double num = *((double*)(&(i->Data[0])));
			str.append(DateTime2Str(num));	// .常量 WM_CLOSE, "[2008年8月8日12时13分14秒]
			str.append("\", ");	// .常量 WM_CLOSE, "[2008年8月8日12时13分14秒]"
		}
		break;
		case CT_Text:		// 文本型常量
		{
			//.常量 常量1, "<文本长度: 12>"
			str.append("\"");
			if (i->Attr & CA_MultiLine)
			{
				char tmp[50];
				sprintf_s(tmp, "<文本长度: %d>", i->Data.size());
				str.append(tmp);	// .常量 WM_CLOSE, "<文本长度: 12>
			}
			else
			{
				str.append("“");
				str.append((LPSTR)&i->Data[0]);
				str.append("”");
			}
			str.append("\", ");
		}
		break;
		default:
			break;
		}
		if (i->Attr & CA_Public)
			str.append("公开");
		str.append(", ");
		str.append(i->Remark);
		str.append("\n");	//换行,继续增加下一个常量
	}
	return str;
}

int ECode::saveECode(const string& eFile, int mode)
{
	if (eFile.size() < 6)
	{
		cout << "文件路径不正确, 请重新输入:\n";
		return -1;
	}
	string path(eFile);
	string fileName;
	if (path.substr(path.size() - 2, 2) != ".e")
	{
		cout << "文件路径不正确, 请重新输入:\n";
		return -1;
	}
	if (!PathFileExistsA(eFile.c_str()))return -2;

	char buffer[512] = { 0 };
	GetTempPathA(512, buffer);

	path = buffer;

	int flag = GetFileAttributesA(path.c_str());
	if (!(flag & FILE_ATTRIBUTE_DIRECTORY) || (flag == INVALID_FILE_ATTRIBUTES && GetLastError() == 2))	// 目录不存在则创建
	{
		if (!CreateDirectoryA(path.c_str(), NULL))
		{
			cout << "文件夹:" << path << "  创建失败\n程序将退出\n";
			return -3;
		}
	}
	fileName = PathFindFileNameA(eFile.c_str());
	int nPos = fileName.rfind('.');
	if (nPos != string::npos)
		fileName = fileName.substr(0, nPos);

	ifstream fs(eFile, ios::binary);
	ostrstream stream(NULL, 0, ios::binary);
	stream << fs.rdbuf();
	fs.close();

	try
	{
		if (stream.str() == NULL)
			throw 10086;
	}
	catch (int i)
	{
		if (i == 10086)
		{
			cout << "文件数据 \"" << eFile << "\" 未获取成功....请重新加载......\n";
			return -4;
		}
	}
	Parse((LPBYTE)stream.str(), stream.pcount(), FALSE);

	string sysInfo = getSysInfo();
	string DllInfo = getDllInfo();
	string DataTypeInfo = getDataType();
	string GlobalInfo = getGlobal();
	vector<string> AssembliesInfo = getAssemblies();
	string constInfo = getConstInfo();
	string strs, consts;

	if (mode == 1)
	{
		string saveFile = path + fileName + ".txt";
		for (auto i = AssembliesInfo.begin(); i < AssembliesInfo.end(); i++)
		{
			i++;	// 第一个记录程序集类型,跳过
			strs += *i;
			//consts;
		}
		strs = strs + GlobalInfo + DllInfo + DataTypeInfo + constInfo + sysInfo;
		writeFile(saveFile, strs);
	}
	else
	{
		//这是写出结果的路径。
		string sys = path + "系统配置.txt";	// 包括用户配置,加载的模块,加载的支持库
		string dll = path + "DLL信息.txt";
		string dataType = path + "数据类型.txt";
		string global = path + "全局变量.txt";
		string constFile = path + "常量.txt";

		writeFile(sys, sysInfo);
		writeFile(dll, DllInfo);
		writeFile(dataType, DataTypeInfo);
		writeFile(global, GlobalInfo);
		writeFile(constFile, constInfo);
		for (auto i = AssembliesInfo.begin(); i < AssembliesInfo.end(); i++)
		{
			string saveFile = path + *i + ".txt";
			i++;	// 第一个记录程序集类型,跳过
			writeFile(saveFile, *i);
		}
	}


	cout << "解析完毕,请查看 \"" << path << "\" 目录下解码好的txt文件\n";
	cout << "请输入易语言源码文件路径(如果不需要解码其他文件, 输入 0 退出程序):\n";
	return 0;
}

void ECode::addString(string& str, LONGLONG n)
{
	string s;
	strstream ss;
	ss << n;
	ss >> s;
	str += s;
	str += "\n";
}

void ECode::addString(string& str, string n)
{
	str += n;
	str += "\n";
}

string ECode::__getArr(ESection_ArrayInfo& arr)
{
	string str;
	for (int i = 0; i < arr.Dimension; i++)
	{
		char tmpChar[20] = { 0 };
		_itoa_s(i, tmpChar, 10);
		str.append(tmpChar);
		if (i < arr.Dimension - 1)
		{
			str.append(",");
		}
	}
	return str;
}

ESection_Program_Assembly* ECode::getMethod(ETAG tag)
{
	ESection_Program_Assembly* method = FindInfo(_einfo.Program.Assemblies, tag);
	return method;
	return nullptr;
}
