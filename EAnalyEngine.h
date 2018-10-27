#include "stdafx.h"

#pragma once

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




typedef struct sectionAlloc
{
	BYTE* SectionAddr;  //申请的内存空间地址
	DWORD dwBase;       //原始代码区段的基址
	DWORD dwSize;       //原始代码区段的大小
}*psectionAlloc;

typedef struct _ENTRYINFO // 易语言入口信息
{
	DWORD	dwMagic;		//<- 未知
	DWORD	dwUnkown1;		//+4 未知
	DWORD	dwUnkown2;		//+8 未知
	DWORD	dwUserCodeStart;//+c 用户代码开始
	DWORD	dwEString;		//+10 字符串 资源       如果没有字符串资源,则为0
	DWORD	dwEStringSize;	//+14 字符串 资源大小   如果没有字符串资源,则为0
	DWORD	dwEWindow;		//+18 创建组件信息      包括窗口、标签等控件
	DWORD	dwEWindowSize;	//+1c 大小
	DWORD	dwLibNum;		//+20 支持库数量
	DWORD	pLibEntey;		//+24 支持库信息入口
	DWORD	dwApiCount;     //+28 Api数量
	DWORD	pLibName;		//+2C 指向库名称
	DWORD	pApiName;		//+30 指向Api名称
}*PEENTRYINFO;



class EAnalysis
{
public:
	EAnalysis(ULONG dwVBase, ULONG dwVsize);
	~EAnalysis();
	
	BOOL EStaticLibInit();     //静态编译--初始化
	BOOL GetUserEntryPoint();  //静态编译--取用户结束地址

	UINT FindSection(DWORD addr); //寻找地址是否在区段表中,返回index
	UINT AddSection(DWORD addr);  //内存拷贝表中增加区段,返回新的index

	DWORD   Search_BinEx(byte *pSrc, byte *pTrait, int nSrcLen, int nTraitLen);
	DWORD	O2V(DWORD dwVaddr, UINT index);//origin addr to virtual addr
	DWORD	V2O(DWORD dwOaddr, UINT index);


	DWORD	GetPoint(DWORD dwAddr);
	DWORD	GetOriginPoint(DWORD dwAddr, UINT index);
	DWORD dwUsercodeStart; //用户代码的起始地址
	DWORD dwUsercodeEnd;   //用户代码的结束地址

	vector<sectionAlloc> SectionMap;    //维护一份内存拷贝表

	PEENTRYINFO pEnteyInfo; // entry info
private:
protected:
};