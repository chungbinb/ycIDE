#pragma once

#define C(c) (LPSTR)c
#define D(d) (LPBYTE)d

using namespace std;

template<typename T> bool arrcmp(const T arr1[], const T arr2[], size_t len)
{
	for (size_t i = 0; i < len; i++)
		if (arr1[i] != arr2[i])
			return false;
	return true;
}

template<typename T> T GetData(LPBYTE &pointer)
{
	T data;
	memcpy(&data, pointer, sizeof(T));
	pointer += sizeof(T);
	return data;
}

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

vector<string> split(const string &s, const string &delim);
string &replace(string &s, const string &src, const string &dst);
string join(vector<string> &lst, const string &delim);
void Decode_Str(BYTE data[], const BYTE key[]);
UINT GetCheckValue(LPBYTE data, size_t len);
string GetNoZeroString(LPBYTE &pointer, bool jump = false);
string GetString(LPBYTE &pointer, bool jump = false);
string GetAutoString(LPBYTE &pointer, bool jump = false);
vector<BYTE> GetBytes(LPBYTE &pointer, bool jump = false);
vector<BYTE> GetBytes(LPBYTE pointer, UINT len, bool jump = false);
void WriteString(ostream *stream, const string &data);
void WriteAutoString(ostream *stream, const string &data);