#include <Windows.h>
#include <string>
#include <vector>
#include "common.h"

vector<string> split(const string &s, const string &delim)
{
	vector<string> elems;
	size_t pos = 0;
	size_t len = s.length();
	size_t delim_len = delim.length();
	if (delim_len == 0) return elems;
	while (pos < len)
	{
		int find_pos = s.find(delim, pos);
		if (find_pos < 0)
		{
			elems.push_back(s.substr(pos, len - pos));
			break;
		}
		elems.push_back(s.substr(pos, find_pos - pos));
		pos = find_pos + delim_len;
	}
	return elems;
}

string &replace(string &s, const string &src, const string &dst)
{
	size_t pos = 0;
	size_t srclen = src.length();
	size_t dstlen = dst.length();
	while ((pos = s.find(src, pos)) != string::npos)
	{
		s.replace(pos, srclen, dst);
		pos += dstlen;
	}
	return s;
}

string join(vector<string> &lst, const string &delim)
{
	string ret;
	for (string &s : lst)
	{
		if (!ret.empty())
			ret.append(delim);
		ret.append(s);
	}
	return ret;
}

void Decode_Str(BYTE data[], const BYTE key[])
{
	size_t key_len = 4;
	size_t len = 30;
	for (size_t i = 0, key_i = 1; i < len; i++, key_i++)
		data[i] = data[i] ^ key[key_i % key_len];
}

UINT GetCheckValue(LPBYTE data, size_t len)
{
	size_t key_len = sizeof(int);
	UINT key = 0;
	LPBYTE ptr = (LPBYTE)&key;
	for (size_t i = 0; i < len; i++)
		ptr[i % key_len] = ptr[i % key_len] ^ data[i];
	return key;
}

string GetNoZeroString(LPBYTE &pointer, bool jump)
{
	string str;
	UINT size = GetData<UINT>(pointer);
	if (!jump)
	{
		for (UINT len = size; len > 0; len--)
		{
			if (pointer[len - 1] != 0)
			{
				str = string((char*)pointer, len);
				break;
			}
		}
	}
	pointer += size;
	return str;
}

string GetString(LPBYTE &pointer, bool jump)
{
	string str;
	UINT size = GetData<UINT>(pointer);
	if (!jump) str = string((char*)pointer, size);
	pointer += size;
	return str;
}

string GetAutoString(LPBYTE &pointer, bool jump)
{
	string str;
	char* lpstr = (char*)pointer;
	size_t len = strlen(lpstr);
	if (!jump) str = string(lpstr, len);
	pointer += len + 1;
	return str;
}

vector<BYTE> GetBytes(LPBYTE &pointer, bool jump)
{
	vector<BYTE> arr;
	UINT size = GetData<UINT>(pointer);
	if (!jump) arr = vector<BYTE>(pointer, pointer + size);
	pointer += size;
	return arr;
}

vector<BYTE> GetBytes(LPBYTE pointer, UINT len, bool jump)
{
	vector<BYTE> arr;
	if (!jump) arr = vector<BYTE>(pointer, pointer + len);
	pointer += len;
	return arr;
}

void WriteString(ostream *stream, const string &data)
{
	UINT size = data.length();
	stream->write(C(&size), sizeof(UINT));
	stream->write(data.c_str(), size);
}

void WriteAutoString(ostream *stream, const string &data)
{
	stream->write(data.c_str(), data.length() + 1);
}