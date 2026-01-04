#include <Windows.h>
#include <string>
#include <vector>
#include <strstream>
#include <random>
#include "VMProtectSDK.h"
#include "efs.h"
#include "common.h"

static char* s_szName = nullptr;

__declspec(noinline)
void Pad(LPBYTE inData, UINT inLen, LPBYTE outData, LPUINT outLen)
{
	VMProtectBegin("Pad");
	memcpy(outData, inData, inLen);
	*outLen = inLen;
	UINT size = inLen % 16;
	if (size != 0 || inData[inLen - 1] < 16)
	{
		size = 16 - size;
		*outLen += size;
		random_device e;
		uniform_int_distribution<USHORT> u(0, 255);
		for (size_t i = 0; i < size - 1; i++)
			*(outData + inLen + i) = (BYTE)u(e);
		*(outData + *outLen - 1) = size;
	}
	VMProtectEnd();
}

void TestEncrypt(LPUINT data, LPUINT key, UINT offset)
{
	VMProtectBeginMutation("TestEncrypt");
	UINT n = *data ^ *key;
	UINT check = 1 << (offset % 32);
	if ((n & check) == 0 && (*data & check) == 0)
		;
	else
		*data = n;
	*key = *data;
	VMProtectEnd();
}

__declspec(noinline)
int Encrypt(LPBYTE inData, UINT inLen, const BYTE key[8], LPBYTE outData, LPUINT outLen)
{
	VMProtectBeginMutation("Encrypt");
	Pad(inData, inLen, outData, outLen);
	LPUINT arr = (LPUINT)outData;
	LPUINT end = (LPUINT)(outData + *outLen);
	LPUINT rk = (LPUINT)key;
	for (LPUINT begin = arr; begin < end; begin += 2)
	{
		begin[0] ^= rk[0];
		begin[1] ^= rk[1];
	}
	UINT k[2] = { end[-2], end[-1] };
	random_device e;
	uniform_int_distribution<USHORT> u(2, 15);
	int round = u(e);
	for (size_t r = 0; r < round; r++)
	{
		size_t i = 1;
		for (LPUINT begin = arr; begin < end; begin += 2, i += 2)
		{
			TestEncrypt(begin, k, i);
			TestEncrypt(begin + 1, k + 1, i + 1);
		}
	}
	end[-2] ^= rk[0];
	end[-1] ^= rk[1];
	VMProtectEnd();
	return round;
}

void WriteVectorVariable(ostream *stream, vector<ESection_Variable> &list)
{
	VMProtectBeginMutation("WriteVectorVariable");
	ostrstream tag(NULL, 0, ios::binary);
	ostrstream offset(NULL, 0, ios::binary);
	ostrstream data(NULL, 0, ios::binary);
	for (ESection_Variable &item : list)
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
	VMProtectEnd();
}

void WriteVectorMethod(ostream *stream, vector<ESection_Program_Method> &list)
{
	VMProtectBeginMutation("WriteVectorMethod");
	ostrstream tag(NULL, 0, ios::binary);
	ostrstream offset(NULL, 0, ios::binary);
	ostrstream data(NULL, 0, ios::binary);
	for (ESection_Program_Method &item : list)
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
	VMProtectEnd();
}

vector<ESection_Variable> GetVariables(LPBYTE &pointer, bool jump = false)
{
	VMProtectBeginMutation("GetVariables");
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
	VMProtectEnd();
	return arr;
}

vector<ESection_Program_Assembly> GetAssemblies(LPBYTE &pointer, bool isclass = true)
{
	VMProtectBeginMutation("GetAssemblies");
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
		spa.Variables = GetVariables(pointer, true);
		arr.push_back(spa);
	}
	delete tags;
	VMProtectEnd();
	return arr;
}

vector<ESection_Program_Method> GetMethods(LPBYTE &pointer)
{
	VMProtectBeginMutation("GetMethods");
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
	VMProtectEnd();
	return arr;
}

void AppendPackInfo(ostream *outStream, ostrstream *pack, UINT index)
{
	VMProtectBegin("AppendPackInfo");
	const BYTE PackKey[4] = { 0x19, 0x73, 0x00, 0xff };
	const BYTE EncKey[8] = { 72, 167, 105, 96, 133, 251, 168, 5 };
	const BYTE EncKey2[8] = { 72, 167, 88, 96, 77, 251, 168, 9 };
	const BYTE EncKey3[8] = { 72, 167, 71, 96, 21, 251, 168, 13 };
	const BYTE EncKey4[8] = { 72, 167, 73, 98, 30, 251, 168, 12 };//1.2.5
	const BYTE EncKey5[8] = { 96, 97, 98, 99, 100, 101, 102, 103 };//1.2.6


	ESection_Info si = { 0 };
	memcpy(si.Key, PackKey, 4);
	memcpy(si.Name, "程序加固段", 11);
	Decode_Str(si.Name, si.Key);
	si.Index = index;
	UINT size = pack->pcount();
	LPBYTE data = new BYTE[size * 2];
	si.reserve_fill_1 = 5;//enc ver
	si.CodeType = Encrypt(D(pack->str()), size, EncKey4, data, &size);
	si.DataLength = size;
	si.Data_CheckSum = GetCheckValue(data, size);
	ESection_Header sh;
	memcpy(sh.Magic, Magic_Section, 4);
	sh.Info_CheckSum = GetCheckValue(D(&si), sizeof(ESection_Info));
	outStream->write(C(&sh), sizeof(ESection_Header));
	outStream->write(C(&si), sizeof(ESection_Info));
	outStream->write(C(data), size);
	delete[] data;
	VMProtectEnd();
}

void ParseProgram(LPBYTE pointer, LPBYTE end, ostream *outStream, ostream *pack)
{
	VMProtectBeginMutation("ParseProgram");
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
	GetAssemblies(pointer);
	outStream->write(C(begin), pointer - begin);
	vector<ESection_Program_Method> methods = GetMethods(pointer);
	UINT size = methods.size();
	pack->write(C(&size), sizeof(UINT));
	bool first = true;
	for (ESection_Program_Method &item : methods)
	{
		if (item.Attributes != 8 && item.Attributes != 0x38 && item.Attributes != 0x88)
		{
			//char szLog[128] = { 0 };
			//sprintf_s(szLog, 128, "Name:%s,%d", item.Name.c_str(), item.Attributes);
			//OutputDebugStringA(szLog);
			//continue;
		}

		size = item.MethodOffset.size();
		pack->write(C(&size), sizeof(UINT));
		LPBYTE ptr = item.MethodOffset.begin()._Ptr;
		pack->write(C(ptr), size);
		item.MethodOffset.clear();

		size = item.VariableOffset.size();
		pack->write(C(&size), sizeof(UINT));
		ptr = item.VariableOffset.begin()._Ptr;
		pack->write(C(ptr), size);
		item.VariableOffset.clear();

		size = item.ParameterOffset.size();
		pack->write(C(&size), sizeof(UINT));
		ptr = item.ParameterOffset.begin()._Ptr;
		pack->write(C(ptr), size);
		item.ParameterOffset.clear();

		size = item.Code.size();
		pack->write(C(&size), sizeof(UINT));
		ptr = item.Code.begin()._Ptr;
		pack->write(C(ptr), size);

		char prompt[1024];
		ZeroMemory(prompt, 1024);
		if (s_szName == nullptr)
		{
			sprintf_s(prompt, 1024, "本模块使用模块加固保护,请安装易语言助手后启用模块守护插件即可正常使用");
		}
		else
		{
			sprintf_s(prompt, 1024, "[%s]已使用模块保护，请安装易语言助手后启用模块守护插件即可正常使用", s_szName);
		}

		string output("\x6a\xc5\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x36", 18);
		output.push_back((char)ECode_Type::StringConst);
		size = strlen(prompt) + 1;
		output.append(C(&size), 4);
		output.append(prompt, size);
		output.append("\x01", 1);
		if (item.ReturnType != DataType::EDT_VOID)
		{
			string returnCode("\x6a\x0d\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x36", 18);
			switch (item.ReturnType)
			{
			case DataType::EDT_BYTE:
			case DataType::EDT_SHORT:
			case DataType::EDT_INT:
			case DataType::EDT_LONG:
			case DataType::EDT_FLOAT:
			case DataType::EDT_DOUBLE:
				returnCode.push_back((char)ECode_Type::NumberConst);
				returnCode.append("\x00\x00\x00\x00\x00\x00\x00\x00", 8);
				break;
			case DataType::EDT_BOOL:
				returnCode.push_back((char)ECode_Type::BoolConst);
				returnCode.append("\x00\x00", 2);
				break;
			case DataType::EDT_TEXT:
				returnCode.push_back((char)ECode_Type::StringConst);
				returnCode.append("\x01\x00\x00\x00\x00", 5);
				break;
			case DataType::EDT_BIN:
				returnCode.push_back((char)ECode_Type::BinConst);
				returnCode.push_back((char)ECode_Type::BinEnd);
				break;
			case DataType::EDT_DATETIME:
				returnCode.push_back((char)ECode_Type::DatetimeConst);
				returnCode.append("\x00\x00\x00\x00\x00\x00\x00\x00", 8);
				break;
			case DataType::EDT_ALL:
				break;
			default:
			{
				size = output.size() + returnCode.size();
				item.VariableOffset.resize(sizeof(UINT));
				ptr = item.VariableOffset.begin()._Ptr;
				*(LPUINT)ptr = size;
				returnCode.push_back((char)ECode_Type::Variable);
				returnCode.push_back((char)ECode_Type::ParameterBegin);
				bool ss = false;
				for (ESection_Variable& var : item.Variables)
				{
					if (var.DataType == item.ReturnType)
					{
						returnCode.append(C(&var.Tag), sizeof(ETAG));
						ss = true;
						break;
					}
				}
				if (ss == false)
				{
					for (ESection_Variable& var : item.Parameters)
					{
						if (var.DataType == item.ReturnType)
						{
							returnCode.append(C(&var.Tag), sizeof(ETAG));
							ss = true;
							break;
						}
					}
				}
				if (ss == false)
				{
					ESection_Variable var;
					var.DataType = item.ReturnType;
					var.Tag = item.Tag;
					var.Tag.Type2 = ETYPE::Variable;
					var.ArrayInfo.Dimension = 0;
					item.Variables.push_back(var);
					returnCode.append(C(&var.Tag), sizeof(ETAG));
				}
				returnCode.push_back((char)ECode_Type::ParameterEnd);
				break;
			}
			}
			returnCode.push_back((char)ECode_Type::ParamEnd);
		other:
			output.append(returnCode);
		}
		item.Code.assign(output.begin(), output.end());
		//if ((item.Attributes & EMethodAttr::M_Public) == 0)
		//	item.Remark.clear();
	}
	WriteVectorMethod(outStream, methods);
	outStream->write(C(pointer), end - pointer);

	delete[]s_szName;
	s_szName = nullptr;
	VMProtectEnd();
}
EXTERN_C void SetModuleName(const char* szName)
{
	VMProtectBeginMutation("EcGuard");
	int strLen = strlen(szName);
	s_szName = new char[strLen + 1];
	ZeroMemory(s_szName, strLen + 1);
	strcpy_s(s_szName, strLen + 1, szName);
	VMProtectEnd();
}

EXTERN_C void EcGuard(UINT index, LPBYTE inData, UINT inDataLen, LPBYTE *outData, UINT *outDataLen, LPBYTE *outPack, UINT *outPackLen)
{
	VMProtectBeginMutation("EcGuard");
	ostrstream stream(NULL, 0, ios::binary);
	ostrstream pack(NULL, 0, ios::binary);
	ParseProgram(inData, inData + inDataLen, &stream, &pack);
	*outDataLen = stream.pcount();
	*outData = (LPBYTE)malloc(*outDataLen);
	memcpy(*outData, stream.str(), *outDataLen);
	ostrstream pack2(NULL, 0, ios::binary);
	AppendPackInfo(&pack2, &pack, index);
	*outPackLen = pack2.pcount();
	*outPack = (LPBYTE)malloc(*outPackLen);
	memcpy(*outPack, pack2.str(), *outPackLen);
	VMProtectEnd();
}

EXTERN_C void FreeData(LPVOID data)
{
	free(data);
}