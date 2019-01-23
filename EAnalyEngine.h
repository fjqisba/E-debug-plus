#pragma once

#include "E-Debug.h"



typedef struct sectionAlloc		//内存拷贝
{
	UCHAR* SectionAddr;  //申请的内存空间地址,格式设定为UCHAR* 更好一点
	ULONG  dwBase;       //原始代码区段的基址
	ULONG  dwSize;       //原始代码区段的大小
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
	EAnalysis(ULONG dwVBase);
	~EAnalysis();
	
	UINT AnalysisMode;

	BOOL EStaticLibInit();     //静态编译--初始化
	BOOL GetUserEntryPoint();  //静态编译--取用户结束地址

	INT FindOriginSection(ULONG addr);		//寻找地址是否在区段表中,返回index
	INT FindVirutalSection(ULONG addr);	//寻找地址是否在申请的地址中,返回index

	INT MakeSureInValidSection(ULONG referaddr,ULONG virtualaddr);		//确保得到的地址在合法区间内
		
	INT UpdateSection(ULONG addr);	
	INT AddSection(ULONG addr);  //内存拷贝表中增加区段,返回新的index

	DWORD   Search_BinEx(byte *pSrc, byte *pTrait, int nSrcLen, int nTraitLen);
	ULONG	O2V(ULONG dwVaddr, UINT index);		//原始地址转换为虚拟地址
	ULONG	V2O(ULONG dwOaddr, UINT index);		//虚拟地址转换为原始地址


	DWORD	GetPoint(DWORD dwAddr);
	DWORD	GetOriginPoint(DWORD dwAddr, UINT index);
	DWORD dwUsercodeStart; //用户代码的起始地址
	DWORD dwUsercodeEnd;   //用户代码的结束地址

	vector<sectionAlloc> SectionMap;    //维护一份内存拷贝表

	ULONG DLLCALL=0;		//保存DLL_CALL地址,在查找DLL命令的时候会用到


	PEENTRYINFO pEnteyInfo; // entry info
private:
protected:
};