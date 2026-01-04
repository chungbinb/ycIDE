#include <Windows.h>
#include <time.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <strstream>
#include <random>
#include "common.h"
#include "flag.h"
#include <iostream>

#include "../ECodeParserTest/ECodeParser.h"


using namespace std;

static int ccount;
static vector<vector<string>> clist;

ECodeParser* g_parser;

template<typename T>
T Random(T min, T max)
{
	random_device e;
	uniform_int_distribution<T> u(min, max);
	return u(e);
}

vector<string>& GetCopyright()
{
	int index = Random<size_t>(0, clist.size() - 1);
	return clist[index];
}

vector<ESection_Variable> GetVariables(LPBYTE& pointer, bool jump = false)
{
	vector<ESection_Variable> arr;
	UINT count = GetData<UINT>(pointer);
	UINT length = GetData<UINT>(pointer);
	LPBYTE oldptr = pointer + length;
	if (!jump)
	{
		arr = vector<ESection_Variable>(count);
		ETAG* tags = new ETAG[count];
		LPBYTE* os = new LPBYTE[count];
		for (size_t i = 0; i < count; i++)
			tags[i] = GetData<ETAG>(pointer);
		LPBYTE mptr = pointer + count * sizeof(UINT);
		for (size_t i = 0; i < count; i++)
			os[i] = mptr + GetData<UINT>(pointer);
		for (size_t i = 0; i < count; i++)
		{
			ESection_Variable sv;
			pointer = os[i];
			UINT size = GetData<UINT>(pointer);
			sv.Tag = tags[i];
			sv.DataType = GetData<DataType>(pointer);
			sv.Attributes = GetData<EVariableAttr>(pointer);
			sv.ArrayInfo.Dimension = GetData<BYTE>(pointer);
			for (size_t n = 0; n < sv.ArrayInfo.Dimension; n++)
				sv.ArrayInfo.Subscript.push_back(GetData<UINT>(pointer));
			sv.Name = GetAutoString(pointer);
			sv.Remark = GetAutoString(pointer);
			arr[i] = sv;
		}
		delete tags;
		delete os;
	}
	pointer = oldptr;
	return arr;
}

vector<ESection_Program_Assembly> GetAssemblies(LPBYTE& pointer, bool isclass = true)
{
	vector<ESection_Program_Assembly> arr;
	UINT length = GetData<UINT>(pointer) / 2;
	size_t count = length / sizeof(ETAG);
	ETAG* tags = new ETAG[count];
	for (size_t i = 0; i < count; i++)
		tags[i] = GetData<ETAG>(pointer);
	pointer += length;
	for (size_t i = 0; i < count; i++)
	{
		ESection_Program_Assembly spa;
		spa.Tag = tags[i];
		if (spa.Tag.Type2 == ETYPE::WindowClass)
		{
			spa.Base = GetData<ETAG>(pointer);
			pointer += 4;
		}
		else
		{
			spa.Status = GetData<ETagStatus>(pointer);
			if (isclass)
				spa.Base = GetData<ETAG>(pointer);
		}
		spa.Name = GetString(pointer);
		spa.Remark = GetString(pointer);
		if (isclass)
		{
			length = GetData<UINT>(pointer);
			size_t size = length / sizeof(ETAG);
			for (size_t n = 0; n < size; n++)
				spa.Methods.push_back(GetData<ETAG>(pointer));
		}
		spa.Variables = GetVariables(pointer);
		arr.push_back(spa);
	}
	delete tags;
	return arr;
}

vector<ESection_Program_Method> GetMethods(LPBYTE& pointer)
{
	vector<ESection_Program_Method> arr;
	UINT length = GetData<UINT>(pointer) / 2;
	size_t count = length / sizeof(ETAG);
	ETAG* tags = new ETAG[count];
	for (size_t i = 0; i < count; i++)
		tags[i] = GetData<ETAG>(pointer);
	pointer += length;
	for (size_t i = 0; i < count; i++)
	{
		ESection_Program_Method spm;
		spm.Tag = tags[i];
		spm.Class = GetData<ETAG>(pointer);
		spm.Attributes = GetData<EMethodAttr>(pointer);
		spm.ReturnType = GetData<DataType>(pointer);
		spm.Name = GetString(pointer);
		spm.Remark = GetString(pointer);
		spm.Variables = GetVariables(pointer);
		spm.Parameters = GetVariables(pointer);
		spm.RowOffset = GetBytes(pointer);
		spm.FlowOffset = GetBytes(pointer);
		spm.MethodOffset = GetBytes(pointer);
		spm.VariableOffset = GetBytes(pointer);
		spm.ParameterOffset = GetBytes(pointer);
		spm.Code = GetBytes(pointer);
		arr.push_back(spm);
	}
	delete tags;
	return arr;
}

vector<ESection_Program_Dll> GetDlls(LPBYTE& pointer)
{
	vector<ESection_Program_Dll> arr;
	UINT length = GetData<UINT>(pointer) / 2;
	size_t count = length / sizeof(ETAG);
	ETAG* tags = new ETAG[count];
	for (size_t i = 0; i < count; i++)
		tags[i] = GetData<ETAG>(pointer);
	pointer += length;
	for (size_t i = 0; i < count; i++)
	{
		ESection_Program_Dll spd;
		spd.Tag = tags[i];
		spd.Status = GetData<ETagStatus>(pointer);
		spd.ReturnType = GetData<DataType>(pointer);
		spd.ShowName = GetString(pointer);
		spd.Remark = GetString(pointer);
		spd.Lib = GetString(pointer);
		spd.Name = GetString(pointer);
		spd.Parameters = GetVariables(pointer);
		arr.push_back(spd);
	}
	delete tags;
	return arr;
}

void WriteVectorVariable(ostream* stream, vector<ESection_Variable>& list)
{
	ostrstream tag(NULL, 0, ios::binary);
	ostrstream offset(NULL, 0, ios::binary);
	ostrstream data(NULL, 0, ios::binary);
	for (ESection_Variable& item : list)
	{
		tag.write(C(&item.Tag), sizeof(ETAG));
		UINT len = data.pcount();
		offset.write(C(&len), sizeof(UINT));
		ostrstream buffer(NULL, 0, ios::binary);
		buffer.write(C(&item.DataType), sizeof(DataType));
		buffer.write(C(&item.Attributes), sizeof(EVariableAttr));
		buffer.write(C(&item.ArrayInfo.Dimension), sizeof(BYTE));
		buffer.write(C(item.ArrayInfo.Subscript.begin()._Ptr), item.ArrayInfo.Dimension * sizeof(UINT));
		WriteAutoString(&buffer, item.Name);
		WriteAutoString(&buffer, item.Remark);
		len = buffer.pcount();
		data.write(C(&len), sizeof(UINT));
		data.write(buffer.str(), len);
	}
	UINT len = list.size();
	stream->write(C(&len), sizeof(UINT));
	len = tag.pcount() + offset.pcount() + data.pcount();
	stream->write(C(&len), sizeof(UINT));
	stream->write(tag.str(), tag.pcount());
	stream->write(offset.str(), offset.pcount());
	stream->write(data.str(), data.pcount());
}

void WriteVectorAssembly(ostream* stream, vector<ESection_Program_Assembly>& list)
{
	ostrstream tag(NULL, 0, ios::binary);
	ostrstream offset(NULL, 0, ios::binary);
	ostrstream data(NULL, 0, ios::binary);
	for (ESection_Program_Assembly& item : list)
	{
		tag.write(C(&item.Tag), sizeof(ETAG));
		UINT len = data.pcount();
		offset.write(C(&len), sizeof(UINT));
		if (item.Tag.Type2 == ETYPE::WindowClass)
		{
			data.write(C(&item.Base), sizeof(ETAG));
			len = 0;
			data.write(C(&len), sizeof(UINT));
		}
		else
		{
			data.write(C(&item.Status), sizeof(ETagStatus));
			data.write(C(&item.Base), sizeof(ETAG));
		}
		WriteString(&data, item.Name);
		WriteString(&data, item.Remark);
		len = item.Methods.size() * sizeof(ETAG);
		data.write(C(&len), sizeof(UINT));
		data.write(C(item.Methods.begin()._Ptr), len);
		WriteVectorVariable(&data, item.Variables);
	}
	UINT len = tag.pcount() + offset.pcount();
	stream->write(C(&len), sizeof(UINT));
	stream->write(tag.str(), tag.pcount());
	stream->write(offset.str(), offset.pcount());
	stream->write(data.str(), data.pcount());
}

void WriteVectorMethod(ostream* stream, vector<ESection_Program_Method>& list)
{
	ostrstream tag(NULL, 0, ios::binary);
	ostrstream offset(NULL, 0, ios::binary);
	ostrstream data(NULL, 0, ios::binary);
	for (ESection_Program_Method& item : list)
	{
		tag.write(C(&item.Tag), sizeof(ETAG));
		UINT len = data.pcount();
		offset.write(C(&len), sizeof(UINT));
		data.write(C(&item.Class), sizeof(ETAG));
		data.write(C(&item.Attributes), sizeof(EMethodAttr));
		data.write(C(&item.ReturnType), sizeof(DataType));
		WriteString(&data, item.Name);
		WriteString(&data, item.Remark);
		WriteVectorVariable(&data, item.Variables);
		WriteVectorVariable(&data, item.Parameters);
		len = item.RowOffset.size() * sizeof(BYTE);
		data.write(C(&len), sizeof(UINT));
		data.write(C(item.RowOffset.begin()._Ptr), len);
		len = item.FlowOffset.size() * sizeof(BYTE);
		data.write(C(&len), sizeof(UINT));
		data.write(C(item.FlowOffset.begin()._Ptr), len);
		len = item.MethodOffset.size() * sizeof(BYTE);
		data.write(C(&len), sizeof(UINT));
		data.write(C(item.MethodOffset.begin()._Ptr), len);
		len = item.VariableOffset.size() * sizeof(BYTE);
		data.write(C(&len), sizeof(UINT));
		data.write(C(item.VariableOffset.begin()._Ptr), len);
		len = item.ParameterOffset.size() * sizeof(BYTE);
		data.write(C(&len), sizeof(UINT));
		data.write(C(item.ParameterOffset.begin()._Ptr), len);
		len = item.Code.size() * sizeof(BYTE);
		data.write(C(&len), sizeof(UINT));
		data.write(C(item.Code.begin()._Ptr), len);
	}
	UINT len = tag.pcount() + offset.pcount();
	stream->write(C(&len), sizeof(UINT));
	stream->write(tag.str(), tag.pcount());
	stream->write(offset.str(), offset.pcount());
	stream->write(data.str(), data.pcount());
}

void ParseUserInfo(LPBYTE pointer, LPBYTE end, ostream* outStream)
{
	ESection_UserInfo sui;
	sui.ProjectName = GetString(pointer);
	sui.Remark = GetString(pointer);
	sui.Author = GetString(pointer);
	sui.ZipCode = GetString(pointer);
	sui.Address = GetString(pointer);
	sui.Phone = GetString(pointer);
	sui.Fax = GetString(pointer);
	sui.Email = GetString(pointer);
	sui.HomePage = GetString(pointer);
	sui.Copyright = GetString(pointer);
	sui.Major = GetData<UINT>(pointer);
	sui.Minor = GetData<UINT>(pointer);
	sui.Build = GetData<UINT>(pointer);
	sui.Revision = GetData<UINT>(pointer);
	string tmp = GetString(pointer);
	if (sui.Remark.empty())
		sui.Remark = join(GetCopyright(), "\r\n");
	else
	{
		size_t pos = sui.Remark.find("\r\n@备注:\r\n", 0);
		if (pos == string::npos)
		{
			sui.Remark.append("\r\n@备注:\r\n");
			sui.Remark.append(join(GetCopyright(), "\r\n"));
		}
		else if (sui.Copyright.empty())
			sui.Copyright = join(GetCopyright(), "\r\n");
	}
	WriteString(outStream, sui.ProjectName);
	WriteString(outStream, sui.Remark);
	WriteString(outStream, sui.Author);
	WriteString(outStream, sui.ZipCode);
	WriteString(outStream, sui.Address);
	WriteString(outStream, sui.Phone);
	WriteString(outStream, sui.Fax);
	WriteString(outStream, sui.Email);
	WriteString(outStream, sui.HomePage);
	WriteString(outStream, sui.Copyright);
	outStream->write(C(&sui.Major), sizeof(UINT));
	outStream->write(C(&sui.Minor), sizeof(UINT));
	outStream->write(C(&sui.Build), sizeof(UINT));
	outStream->write(C(&sui.Revision), sizeof(UINT));
	WriteString(outStream, tmp);
	outStream->write(C(pointer), end - pointer);
}

void ParseProgram(LPBYTE pointer, LPBYTE end, int type, ostream* outStream)
{
	LPBYTE begin = pointer;
	pointer += sizeof(UINT);
	pointer += sizeof(UINT);
	GetBytes(pointer, true);
	GetBytes(pointer, true);
	GetBytes(pointer, true);
	USHORT len = GetData<USHORT>(pointer);
	for (size_t i = 0; i < len; i++)
		GetString(pointer, true);
	int flag = GetData<int>(pointer);
	pointer += sizeof(UINT);
	if ((flag & 1) != 0) pointer += 16;
	GetBytes(pointer, true);
	GetBytes(pointer, true);
	outStream->write(C(begin), pointer - begin);
	vector<ESection_Program_Assembly> assemblies = GetAssemblies(pointer);
	vector<ESection_Program_Method> methods = GetMethods(pointer);
	if (type == 1)
	{
		vector<ESection_Program_Method*> list;
		for (ESection_Program_Method& item : methods)
		{
			//if ((item.Attributes & EMethodAttr::M_Extern) == 0 && item.Name.compare("网页_访问") == 0)

			list.push_back(&item);

		}
		for (size_t i = list.size(); i > 0; i--)
		{
			size_t index = i - 1;
			string remark;
			vector<UINT> row;
			ESection_Program_Method* item = list[index];

			std::string code = ECodeParser_EMethodCode2Code(g_parser, item->Tag);
			std::vector<string> strs = split(code, "\r\n");
			list[index]->Code.clear();

			int startpos = 0;
			for (auto line : strs)
			{
				remark.clear();
				remark.append("\x6a\x00\x00\x00\x00\xff\xff\x00\x00\x00\x00\x00\x00", 13);
				UINT len = line.size() + 1;
				remark.append(C(&len), 4);
				remark.append(line.c_str(), len);
				remark.append("\x36\x01", 2);

				int codelen = 0;
				for (byte b : remark)
				{
					codelen++;
					list[index]->Code.push_back(b);
				}

				row.push_back(startpos);
				startpos += codelen;
			}


			//这里还需要修改对应的各种偏移。
			item->ParameterOffset.clear();
			item->FlowOffset.clear();
			item->VariableOffset.clear();
			item->RowOffset.clear();
			//清空全部的偏移，保留一个行偏移即可。

			item->RowOffset.insert(item->RowOffset.begin(), ((vector<BYTE>*) & row)->begin(), ((vector<BYTE>*) & row)->end());
		}
	}
	WriteVectorAssembly(outStream, assemblies);
	WriteVectorMethod(outStream, methods);
	outStream->write(C(pointer), end - pointer);
}

void Parse(LPBYTE data, size_t size, int type, ostream* outStream)
{
	EFile_Header header;
	memcpy(&header, data, sizeof(EFile_Header));
	if (arrcmp(header.Magic1, Magic1, 4) && arrcmp(header.Magic2, Magic2, 4))
	{
		outStream->write(C(&header), sizeof(EFile_Header));
		long long offset = sizeof(EFile_Header);
		while (offset < size)
		{
			ESection_Header sh = GetData<ESection_Header>(data, offset);
			if (!arrcmp(sh.Magic, Magic_Section, 4))
				goto end;
			ESection_Info si = GetData<ESection_Info>(data, offset);
			if (!arrcmp(si.Key, KEY, 4))
				Decode_Str(si.Name, si.Key);
			LPSTR Block_Name = (LPSTR)si.Name;
			if (strcmp(Block_Name, "") == 0)
			{
				outStream->write(C(data + offset - sizeof(ESection_Header) - sizeof(ESection_Info)), sizeof(ESection_Header) + sizeof(ESection_Info) + si.DataLength);
				break;
			}
			else if ((type == 0 || type == 1) && strcmp(Block_Name, "程序段") == 0)
			{
				ostrstream stream(NULL, 0, ios::binary);
				ParseProgram(data + offset, data + offset + si.DataLength, type, &stream);
				UINT oldlen = si.DataLength;
				if (!arrcmp(si.Key, KEY, 4)) Decode_Str(si.Name, si.Key);
				si.DataLength = stream.pcount();
				si.Data_CheckSum = GetCheckValue(D(stream.str()), stream.pcount());
				sh.Info_CheckSum = GetCheckValue(D(&si), sizeof(ESection_Info));
				outStream->write(C(&sh), sizeof(ESection_Header));
				outStream->write(C(&si), sizeof(ESection_Info));
				outStream->write(stream.str(), stream.pcount());
				si.DataLength = oldlen;
				offset += si.DataLength;
			}
			else if (strcmp(Block_Name, "程序资源段") == 0)
			{
				int a = 0;
				offset += si.DataLength;
			}
			else
			{
				outStream->write(C(data + offset - sizeof(ESection_Header) - sizeof(ESection_Info)), sizeof(ESection_Header) + sizeof(ESection_Info) + si.DataLength);
				offset += si.DataLength;
			}
			
		}
	end:;
	}
}

int main()
{
	int type = 1;
	const string path = "F:\\Documents\\Desktop\\修复\\源码.e";
	ifstream inStream(path, ios::binary);
	ostrstream stream(NULL, 0, ios::binary);
	stream << inStream.rdbuf();
	inStream.close();

	g_parser = ECodeParser_Create("E:\\易语言正式版\\");
	EInfo* info = ECodeParser_Parse(g_parser, (LPBYTE)stream.str(), stream.pcount(), FALSE);


	ofstream outStream(path.substr(0, path.length() - 2) + "_ic.e", ios::binary);
	Parse(D(stream.str()), stream.pcount(), type, &outStream);

	ECodeParser_Destroy(g_parser);

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
