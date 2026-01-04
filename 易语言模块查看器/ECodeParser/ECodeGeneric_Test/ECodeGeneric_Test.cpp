// ECodeGeneric_Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <vector>
#include "../EcGuard/common.h"
#include "ECodeGeneric.h"
#include <strstream>


const BYTE systemInfo_key[4] = { 25,115,0,2 };
const BYTE userInfo_key[4] = { 25,115,0,1 };

void GenESection_SystemInfo(const ESection_SystemInfo& systemInfo, ostrstream* outbuffer)
{
	ESection_Info si = { 0 };

	memcpy(si.Key, systemInfo_key, 4);
	memcpy(si.Name, "系统信息段", 10);
	Decode_Str(si.Name, si.Key);

	si.Index = 1;
	si.reserve_fill_1 = 0;
	si.CodeType = 0;

	si.DataLength = sizeof(ESection_SystemInfo);
	si.Data_CheckSum = GetCheckValue(D(&systemInfo), sizeof(ESection_SystemInfo));

	ESection_Header sh;
	memcpy_s(sh.Magic, 4, Magic_Section, 4);
	sh.Info_CheckSum = GetCheckValue(D(&si), sizeof(ESection_Info));

	outbuffer->write(C(&sh), sizeof(ESection_Header));
	outbuffer->write(C(&si), sizeof(ESection_Info));
	outbuffer->write(C(&systemInfo), sizeof(ESection_SystemInfo));
}

void GenESection_UserInfo(const ESection_UserInfo& userInfo, ostrstream* outbuffer)
{
	ESection_Info si = { 0 };

	memcpy(si.Key, userInfo_key, 4);
	memcpy(si.Name, "用户信息段", 10);
	Decode_Str(si.Name, si.Key);

	si.Index = 2;
	si.reserve_fill_1 = 0;
	si.CodeType = 1;

	ostrstream* tmpStream = new ostrstream();
	UINT bufferLen = userInfo.ProjectName.size();

	tmpStream->write(C(&bufferLen), 4);
	tmpStream->write(userInfo.ProjectName.c_str(), bufferLen);

	bufferLen = userInfo.Remark.size();
	tmpStream->write(C(&bufferLen), 4);
	tmpStream->write(userInfo.Remark.c_str(), bufferLen);

	bufferLen = userInfo.Author.size();
	tmpStream->write(C(&bufferLen), 4);
	tmpStream->write(userInfo.Author.c_str(), bufferLen);

	bufferLen = userInfo.ZipCode.size();
	tmpStream->write(C(&bufferLen), 4);
	tmpStream->write(userInfo.ZipCode.c_str(), bufferLen);

	bufferLen = userInfo.Address.size();
	tmpStream->write(C(&bufferLen), 4);
	tmpStream->write(userInfo.Address.c_str(), bufferLen);

	bufferLen = userInfo.Phone.size();
	tmpStream->write(C(&bufferLen), 4);
	tmpStream->write(userInfo.Phone.c_str(), bufferLen);

	bufferLen = userInfo.Fax.size();
	tmpStream->write(C(&bufferLen), 4);
	tmpStream->write(userInfo.Fax.c_str(), bufferLen);

	bufferLen = userInfo.Email.size();
	tmpStream->write(C(&bufferLen), 4);
	tmpStream->write(userInfo.Email.c_str(), bufferLen);

	bufferLen = userInfo.HomePage.size();
	tmpStream->write(C(&bufferLen), 4);
	tmpStream->write(userInfo.HomePage.c_str(), bufferLen);

	bufferLen = userInfo.Copyright.size();
	tmpStream->write(C(&bufferLen), 4);
	tmpStream->write(userInfo.Copyright.c_str(), bufferLen);

	tmpStream->write(C(&userInfo.Major), 4);
	tmpStream->write(C(&userInfo.Minor), 4);
	tmpStream->write(C(&userInfo.Build), 4);
	tmpStream->write(C(&userInfo.Revision), 4);

	bufferLen = userInfo.PluginName.size();
	tmpStream->write(C(&bufferLen), 4);
	if (bufferLen == 0)
		tmpStream->put(0);
	else
		tmpStream->write(userInfo.PluginName.c_str(), bufferLen);

	si.DataLength = tmpStream->pcount();


	si.Data_CheckSum = GetCheckValue(D(tmpStream->str()), si.DataLength);

	ESection_Header sh;
	memcpy_s(sh.Magic, 4, Magic_Section, 4);
	sh.Info_CheckSum = GetCheckValue(D(&si), sizeof(ESection_Info));

	outbuffer->write(C(&sh), sizeof(ESection_Header));
	outbuffer->write(C(&si), sizeof(ESection_Info));
	outbuffer->write(C(tmpStream->str()), si.DataLength);
	delete tmpStream;
}

int main()
{
	ostrstream outStream;

	///形成文件头8字节
	EFile_Header header;
	memcpy_s(header.Magic1, 4, Magic1, 4);
	memcpy_s(header.Magic2, 4, Magic2, 4);

	outStream.write(C(&header), sizeof(EFile_Header));

	ESection_SystemInfo systemInfo = { 0 };
	systemInfo.Version1 = 5;
	systemInfo.Version2 = 6;
	systemInfo.Unknow_1 = 1;
	systemInfo.Unknow_2 = 1;
	systemInfo.Unknow_3 = 458753;
	systemInfo.FileType = FileType::E;
	GenESection_SystemInfo(systemInfo, &outStream);

	ESection_UserInfo userInfo;
	userInfo.Major = 1;
	GenESection_UserInfo(userInfo, &outStream);





	FILE* f;
	fopen_s(&f, "D:\\2.e", "wb");
	if (f != nullptr)
	{
		fwrite(outStream.str(), outStream.pcount(), 1, f);

		fclose(f);
	}

	std::cout << "Hello World!\n";
}

