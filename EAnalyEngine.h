#pragma once

#include "E-Debug.h"

static BOOL ReadSig(const char *lpMapPath, map<string, string>& m_subFunc, map<string, string>& m_Func)		//参数一为路径,参数二为返回的子函数特征文本,参数三为返回的函数特征文本
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

typedef struct sectionAlloc		//内存拷贝
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
	EAnalysis(ULONG dwVBase);
	~EAnalysis();
	
	BOOL EStaticLibInit();     //静态编译--初始化
	BOOL GetUserEntryPoint();  //静态编译--取用户结束地址

	UINT FindSection(DWORD addr); //寻找地址是否在区段表中,返回index
	UINT AddSection(DWORD addr);  //内存拷贝表中增加区段,返回新的index

	DWORD   Search_BinEx(byte *pSrc, byte *pTrait, int nSrcLen, int nTraitLen);
	DWORD	O2V(DWORD dwVaddr, UINT index);		//原始地址转换为虚拟地址
	DWORD	V2O(DWORD dwOaddr, UINT index);		//虚拟地址转换为原始地址


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