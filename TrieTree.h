#pragma once
#include "E-Debug.h"
#include "Page0.h"
#include "Page1.h"
#include "EAnalyEngine.h"

#define TYPE_NORMAL 0	
#define TYPE_LONGJMP 1	       //      -->
#define TYPE_CALL   2	       //      <>
#define TYPE_JMPAPI 3	       //      []
#define TYPE_CALLAPI 4	      //      <[]>

#define TYPE_PUSH   6	      //      !!
#define TYPE_LEFTPASS 11      //      ?
#define TYPE_RIGHTPASS 12     //       ?
#define TYPE_ALLPASS   13     //      ??


extern  EAnalysis	*pEAnalysisEngine;

typedef struct FuncMap
{
	vector<string>  Command_name;	//函数名称
	vector<DWORD>   Command_addr;	//函数地址
}*pFuncMap;




class TrieTreeNode {
public:
	TrieTreeNode();

	vector<TrieTreeNode*> SpecialNodes;
	TrieTreeNode **ChildNodes;

	UINT SpecialType;	//一个数字代表类型

	char* EsigText;		//一段文字代表数据
	char* FuncName;		//函数名称

	BOOL IsMatched;     //是否已经匹配过
};

//————————————————————————//

class TrieTree
{
public:
	TrieTree();
	~TrieTree() { Destroy(root); };
	BOOL LoadSig(const char* lpMapPath);
	
	void  ScanSig(UCHAR* CodeSrc, ULONG SrcLen,string& LibName);    //参数一是代码起始节点,参数二为代码块大小,参数三为对应扫描库名
	char* MatchSig(UCHAR* CodeSrc);					//单点匹配
	
	
	

protected:
	BOOL Insert(string& FuncTxt, const string& FuncName);		//插入函数,节点的函数名必须唯一！
	BOOL CmpCall(UCHAR* FuncSrc, string& FuncTxt);				//低配版匹配函数法,用于匹配子函数

private:
	TrieTreeNode* root;
	TrieTreeNode* AddNode(TrieTreeNode* p, string& Txt);		//增加普通节点
	TrieTreeNode* AddSpecialNode(TrieTreeNode*p, UINT type, string Txt);	//增加特殊节点

	

	void Destroy(TrieTreeNode* p);
	char* Match(TrieTreeNode*p, UCHAR* FuncSrc);		//参数一为匹配节点,参数二为匹配地址,返回匹配成功的函数文本

	BOOL IsAligned=false;
	
	
	map<string, string> m_subFunc;	//子函数,函数名称和函数文本一一映射
	map<ULONG,string> m_RFunc;  //R代表Runtime,运行时记录函数,永远记住 地址和函数名称是一一映射的！不要试图一个地址多个函数名称
};









