#pragma once

using namespace std;

template<typename T> bool arrcmp(const T arr1[], const T arr2[], size_t len)
{
	for (size_t i = 0; i < len; i++)
		if (arr1[i] != arr2[i])
			return false;
	return true;
}

// 从源码数据里取出数据,并将指向源码数据的指针指向读取数据后的下一个字节
template<typename T> T GetData(LPBYTE &pointer)
{
	T data;
	memcpy(&data, pointer, sizeof(T));
	pointer += sizeof(T);
	return data;
}

// 从源码数据里取出数据,并将指向源码数据的指针指向读取数据后的下一个字节
template<typename T> T GetData(LPBYTE pointer, long long &offset)
{
	T data;
	memcpy(&data, pointer + offset, sizeof(T));
	offset += sizeof(T);
	return data;
}

template<UINT I>
struct Byte
{
	BYTE value[I];
};

vector<string> split(string &s, const string &delim);
string join(vector<string> &lst, const string &delim);
void Decode_Str(BYTE data[], const BYTE key[]);
void GetNoZeroString(LPBYTE &pointer, string *result = NULL, bool append = false);
void GetString(LPBYTE &pointer, string *result = NULL, bool append = false);
void GetAutoString(LPBYTE &pointer, string *result = NULL, bool append = false);
void GetBytes(LPBYTE &pointer, vector<BYTE> *result = NULL);
void GetBytes(LPBYTE pointer, UINT len, vector<BYTE> *result);