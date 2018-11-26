#pragma once
#include "stdafx.h"
#include "resource.h"		
#include "Plugin.h"

#pragma comment(lib,"Ollydbg.lib")

#define StaticMode 0  //静态编译模式
#define NormalMode 1  //编译或独立编译模式
#define CMode      2  //黑月编译，属于C语言模式



/*————————————————————————————
声明全局变量
————————————————————————————*/
extern char DIRECTORY[MAX_PATH];  //插件目录
extern UINT AnalysisMode;		//当前选择的分析模式




/*————————————————————————————
以下是一些基础函数,能放在这里的函数尽量放在这里
————————————————————————————*/
static HANDLE GethProcess() {           //返回所调试进程的句柄
	return (HANDLE)*(DWORD*)0x4D5A68;
}

static BOOL LoadSig(const char *lpMapPath, map<string, string>& m_subFunc, map<string, string>& m_Func)		//参数一为路径,参数二为返回的子函数特征文本,参数三为返回的函数特征文本
{
	HANDLE hFile = CreateFileA(lpMapPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	m_subFunc.clear();
	m_Func.clear();
	DWORD	dwHitSize = 0;
	DWORD	dwSize = GetFileSize(hFile, &dwHitSize);
	DWORD	dwReadSize;

	char* pMap = (char*)malloc(dwSize);
	ReadFile(hFile, pMap, dwSize, &dwReadSize, NULL);
	string Sig = pMap;

	int delimiter = Sig.find("******");   //分界符
	if (delimiter == -1) {
		return false;
	}
	string SubFunc = Sig.substr(0, delimiter);

	int pos = SubFunc.find_first_of("\r\n");     //子函数
	while (pos != -1) {
		string temp = SubFunc.substr(0, pos);  //单个子函数
		int tempos = temp.find(':');
		if (tempos == -1) {
			break;
		}
		m_subFunc[temp.substr(0, tempos)] = temp.substr(tempos + 1);
		SubFunc = SubFunc.substr(pos + 2);
		pos = SubFunc.find("\r\n");
	}

	string Func = Sig.substr(delimiter + 8);    //全部函数文本
	pos = Func.find("\r\n");

	while (pos != -1) {
		string temp = Func.substr(0, pos);    //单个子函数
		int tempos = temp.find(':');
		if (tempos == -1) {
			break;
		}
		m_Func[temp.substr(0, tempos)] = temp.substr(tempos + 1);
		Func = Func.substr(pos + 2);
		pos = Func.find("\r\n");
	}

	if (pMap) {
		free(pMap);
	}

	CloseHandle(hFile);
	return TRUE;
}

static void HexToBin(string& HexCode, UCHAR* BinCode) {		//十六到十
	static UCHAR BinMap[256] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,		//123456789
		0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,	//ABCDEF
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,	//abcdef
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	for (UINT n = 0;n < HexCode.length() / 2;n++) {
		BinCode[n] = BinMap[HexCode[2 * n]] * 16 + BinMap[HexCode[2 * n + 1]];
	}
}

static UCHAR HByteToBin(char HByte) {		//半个字节的HEX转换为整数
	static UCHAR BinMap[256] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,		//123456789
		0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,	//ABCDEF
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,	//abcdef
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	return BinMap[HByte];
}

static DWORD Search_Bin(byte *pSrc, byte *pTrait, int nSrcLen, int nTraitLen) //低配版二进制搜索,用0x90来代替模糊搜索,返回偏移大小
{
	if (IsBadReadPtr(pSrc, 4) == TRUE)
	{
		return 0;
	}
	int i, j, k;
	for (i = 0; i <= (nSrcLen - nTraitLen); i++)
	{
		if (pSrc[i] == pTrait[0])
		{
			k = i;
			j = 0;
			while (j < nTraitLen)
			{
				k++; j++;
				if (pTrait[j] == 0x90)
				{
					continue;
				}
				if (pSrc[k] != pTrait[j])
				{
					break;
				}
			}

			if (j == nTraitLen)
			{
				return i;
			}

		}

	}
	return 0;
}