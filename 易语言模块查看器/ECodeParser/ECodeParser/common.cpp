#include "stdafx.h"
#include "common.h"

vector<string> split(string &s, const string &delim)
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

string join(vector<string> &lst, const string &delim)
{
	string ret;
	for (string &s : lst)
	{
		if (!ret.empty())
			ret += delim;
		ret += s;
	}
	return ret;
}

// 解密源码里的数据,把他转成文本格式
void Decode_Str(BYTE data[], const BYTE key[])
{
	size_t key_len = 4;
	size_t len = 30;
	// 把加密的数据和key传进来,使用这个key进行解码
	for (size_t i = 0, key_i = 1; i < len; i++, key_i++)
	{
		// 异或 data[i] ^ key,一直从key里的 1,2,3,0 这样重复异或
		data[i] = data[i] ^ key[key_i % key_len];
	}
}

void GetNoZeroString(LPBYTE &pointer, string *result, bool append)
{
	UINT size = GetData<UINT>(pointer);
	if (result != NULL)
	{
		for (UINT len = size; len > 0; len--)
		{
			if (pointer[len - 1] != 0)
			{
				if (append)
					result->append((char*)pointer, len);
				else
					result->assign((char*)pointer, len);
				break;
			}
		}
	}
	pointer += size;
}

void GetString(LPBYTE &pointer, string *result, bool append)
{
	UINT size = GetData<UINT>(pointer);
	if (result != NULL)
	{
		if (append)
			result->append((char*)pointer, size);
		else
			result->assign((char*)pointer, size);
	}
	pointer += size;
}

void GetAutoString(LPBYTE &pointer, string *result, bool append)
{
	char* lpstr = (char*)pointer;
	size_t len = strlen(lpstr);
	if (result != NULL)
	{
		if (append)
			result->append(lpstr, len);
		else
			result->assign(lpstr, len);
	}
	pointer += len + 1;
}

void GetBytes(LPBYTE &pointer, vector<BYTE> *result)
{
	UINT size = GetData<UINT>(pointer);
	if (result != NULL)
		result->assign(pointer, pointer + size);
	pointer += size;
}

void GetBytes(LPBYTE pointer, UINT len, vector<BYTE> *result)
{
	if (result != NULL)
		result->assign(pointer, pointer + len);
	pointer += len;
}