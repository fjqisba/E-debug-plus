#pragma once
#include "stdafx.h"
#include "resource.h"		
#include "Plugin.h"

#pragma comment(lib,"Ollydbg.lib")

#define StaticMode 0  //静态编译模式
#define NormalMode 1  //编译或独立编译模式
#define CMode      2  //黑月编译，属于C语言模式

/*一份E-debug一份EsigList*/
struct EsigInfo
{
	string Category;		//只有一层小类别
	string Name;			//Esig库名称
	string Description;		//Esig库描述
	string Path;			//路径
};

/*————————————————————————————
声明全局变量
————————————————————————————*/
extern char DIRECTORY[MAX_PATH];  //插件目录
extern vector<EsigInfo> EsigList;
extern HINSTANCE g_hInstace;

/*————————————————————————————
以下是一些基础函数,能放在这里的函数尽量放在这里
————————————————————————————*/

HANDLE GethProcess();	 //返回所调试进程的句柄

UINT ByteToDec(char* HexCode);	//十六到十,快速版，只支持一个字节

void HexToBin(string& HexCode, UCHAR* BinCode);

char* DecToByte(UINT DecCode);

UCHAR HByteToBin(char HByte);	//半个字节的HEX转换为整数

INT findstr(const char* Func, const char* find);

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

static string GetMidString(string& src,const char* left, const char* right, int offset) {		//参数一为原文本,参数二为左边文本,参数三为右边文本,参数四为起始偏移
	int start = src.find(left, offset);
	if (start == -1) {
		return "";
	}

	int end = src.find(right, start);
	if (end == -1) {
		return "";
	}

	return src.substr(start + strlen(left), end - start - strlen(left));
}