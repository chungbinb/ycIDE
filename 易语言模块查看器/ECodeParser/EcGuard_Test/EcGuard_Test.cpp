// EcGuard_Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include <strstream>

#include <windows.h>

EXTERN_C void EcGuard(UINT index, LPBYTE inData, UINT inDataLen, LPBYTE* outData, UINT* outDataLen, LPBYTE* outPack, UINT* outPackLen);

__declspec(noinline)
int GetCheckValue(LPBYTE data, size_t len)
{
	size_t key_len = sizeof(int);
	int key = 0;
	LPBYTE ptr = (LPBYTE)& key;
	for (size_t i = 0; i < len; i++)
	{
		int num = i % key_len;
		ptr[num] = ptr[num] ^ data[i];
	}
	return key;
}

int main()
{
	std::ifstream fs("D:\\1.bin", std::ios::binary);
	std::ostrstream stream(NULL, 0, std::ios::binary);
	stream << fs.rdbuf();
	fs.close();

	int a =  GetCheckValue((LPBYTE)stream.str(), stream.pcount());
	printf("%d\n", a);
	return -1;



	/*std::ifstream fs("F:\\Documents\\Desktop\\精易皮肤模块2.0.ec", std::ios::binary);
	std::ostrstream stream(NULL, 0, std::ios::binary);
	stream << fs.rdbuf();
	fs.close();



	LPBYTE* outdata = NULL,* outpack =NULL;
	UINT outDataLen = 0,outPackLen=0;

	EcGuard(0, (LPBYTE)stream.str(), stream.pcount(), outdata, &outDataLen, outpack, &outPackLen);*/



    std::cout << "Hello World!\n";
}