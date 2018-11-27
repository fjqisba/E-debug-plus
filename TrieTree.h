#pragma once
#include "E-Debug.h"
#include "Page1.h"
#include "EAnalyEngine.h"

extern CMainWindow *pMaindlg;
extern  EAnalysis	*pEAnalysisEngine;

class TrieTreeNode {
public:
	TrieTreeNode() {
		ChildNodes = new TrieTreeNode*[256];
		for (int i = 0;i < 256;i++) {
			ChildNodes[i] = NULL;
		}
		EsigText = NULL;
		FuncName = NULL;
		IsMatched = false;
	}
public:
	vector<TrieTreeNode*> SpecialNodes;
	TrieTreeNode **ChildNodes;

	BOOL IsMatched;     //是否已经匹配过
	char* FuncName;		//函数名称
	char* EsigText;		//节点所包含的Esig文字
};

//————————————————————————//

class TrieTree
{
public:
	TrieTree();
	~TrieTree() { Destroy(root); };
	BOOL Insert(string& FuncTxt,const string& FuncName);	//插入节点
	void MatchSig(UCHAR* CodeSrc, ULONG SrcLen);    //参数一是代码起始节点,参数二为代码块大小,扫描版本
	char* MatchSig(UCHAR* CodeSrc);					//单点匹配
private:
	TrieTreeNode* root;
	TrieTreeNode* AddNode(TrieTreeNode* p, string& Txt);		//增加普通节点
	TrieTreeNode* AddSpecialNode(TrieTreeNode*p, string& Txt);	//增加特殊节点
	void Destroy(TrieTreeNode* p);
	char* Match(TrieTreeNode*p, UCHAR* FuncSrc);		//参数一为匹配节点,参数二为匹配二进制,返回匹配成功的函数文本
};

TrieTree::TrieTree()
{
	root = new TrieTreeNode();
}

void TrieTree::Destroy(TrieTreeNode* p) {
	if (!p) {
		return;
	}
	for (int i = 0;i < p->SpecialNodes.size();i++) {
		Destroy(p->SpecialNodes[i]);
	}
	for (int i = 0;i < 256;i++) {
		Destroy(p->ChildNodes[i]);
	}
	if (p->EsigText) {
		delete[] p->EsigText;
		p->EsigText = NULL;
	}
	if (p->FuncName) {
		delete[] p->FuncName;
		p->FuncName = NULL;
	}
	delete p;
	p = NULL;
}

char* TrieTree::Match(TrieTreeNode* p, UCHAR* FuncSrc) {
	if (p->FuncName && !p->IsMatched) {		//如果删掉IsMatched判断,则Esig可匹配无限次
		p->IsMatched = true;
		return p->FuncName;
	}
	for (UINT i = 0;i < p->SpecialNodes.size();i++) {		//先处理特殊节点
		/*if (p->SpecialNodes[i]->EsigText[0] == '-' && p->SpecialNodes[i]->EsigText[1] == '-' && p->SpecialNodes[i]->EsigText[2] == '>') {

		}
		else if (p->SpecialNodes[i]->EsigText[0]=='<' && p->SpecialNodes[i]->EsigText[1]=='[') {	//FF15 CALL的判定
			if (*FuncSrc != 0xFF || *(FuncSrc + 1) != 0x15 || IsBadReadPtr((ULONG*)(FuncSrc + 2), 4) != 0) {      //含有VMP和SE代码
				return NULL;
			}
			

		}
		else if (p->SpecialNodes[i]->EsigText[0] == '<' && *FuncSrc == 0xE8) {	//这是一个CALL的基本判定
			
		}
		else {*/
			if ((p->SpecialNodes[i]->EsigText[0] == '?' || HByteToBin(p->SpecialNodes[i]->EsigText[0])==*(FuncSrc)>>4) &&(p->SpecialNodes[i]->EsigText[1] == '?'|| HByteToBin(p->SpecialNodes[i]->EsigText[1]) == *(FuncSrc)& 0x0F)) {		//第一个是通配符
				return Match(p->SpecialNodes[i], FuncSrc + 1);		//继续匹配下一层
			}
		//}
	}

	if (!p->ChildNodes[*FuncSrc]) {
		return NULL;
	}
	return Match(p->ChildNodes[*FuncSrc], FuncSrc + 1);
}

char* TrieTree::MatchSig(UCHAR* CodeSrc) {
	TrieTreeNode* p = root;		//当前指针指向root

	return Match(p, (UCHAR*)CodeSrc);
}

void TrieTree::MatchSig(UCHAR* CodeSrc, ULONG SrcLen) {
	
	TrieTreeNode* p = root;		//当前指针指向root

	for (ULONG offset = 0;offset < SrcLen;offset++) {
		char* FuncName = Match(p, (UCHAR*)CodeSrc + offset);
		if (FuncName) {
			DWORD dwAddr = pEAnalysisEngine->V2O((DWORD)CodeSrc + offset, 0);
			//pMaindlg->m_page1.m_map[2].Command_addr.push_back(dwAddr);
			//pMaindlg->m_page1.m_map[2].Command_name.push_back(FuncName);
			Insertname(dwAddr, NM_LABEL, FuncName);
		}
	}
	return ;
}

BOOL TrieTree::Insert(string& FuncTxt,const string& FuncName) {
	TrieTreeNode *p = root;//当前节点指针指向ROOT节点

	string BasicTxt;
	string SpecialTxt;

	for (UINT n = 0;n < FuncTxt.length();n++) {
		if (FuncTxt[n] == '-' && FuncTxt[n + 1] == '-' && FuncTxt[n + 2] == '>') {		//-->长跳转
			SpecialTxt = "-->";
			p = AddSpecialNode(p, SpecialTxt);
			n = n + 2;
			continue;
		}
		else if (FuncTxt[n] == '<' && FuncTxt[n + 1] == '[')	//FF15 CALL
		{
			int post = FuncTxt.find("]>", n);
			if (post == -1) {
				return false;
			}

			SpecialTxt = FuncTxt.substr(n, post - n + 2);   //得到文本中的IAT函数
			p = AddSpecialNode(p, SpecialTxt);
			n = post + 1;
			continue;
		}
		else if (FuncTxt[n] == '<')	//函数CALL
		{
			int post = FuncTxt.find('>', n);
			if (post == -1) {
				return false;
			}
			SpecialTxt = FuncTxt.substr(n, post - n + 1);
			p = AddSpecialNode(p, SpecialTxt);
			n = post;
			continue;
		}
		else if (FuncTxt[n] == '[' && FuncTxt[n + 1] == ']' && FuncTxt[n + 2] == '>'){	//FF25跳转
			continue;
		}
		else if (FuncTxt[n]=='['){
			int post = FuncTxt.find(']', n);
			if (post == -1) {
				return false;
			}
			SpecialTxt = FuncTxt.substr(n, post - n + 1);
			n = post;
			continue;
		}
		else if(FuncTxt[n]=='?' || FuncTxt[n+1]=='?')	//存在通配符
		{
			SpecialTxt= FuncTxt.substr(n, 2);
			p = AddSpecialNode(p, SpecialTxt);
			n = n + 1;
			continue;
		}
		else {
			BasicTxt = FuncTxt.substr(n, 2);
			p = AddNode(p, BasicTxt);
			n = n + 1;
		}
	}

	p->FuncName = new char[FuncName.length() + 1];strcpy_s(p->FuncName, FuncName.length() + 1, FuncName.c_str());
	return true;
}

TrieTreeNode* TrieTree::AddNode(TrieTreeNode* p, string& Txt) {
	UCHAR index = 0;
	HexToBin(Txt, &index);
	if (p->ChildNodes[index]) {
		return p->ChildNodes[index];
	}

	TrieTreeNode* NewNode = new TrieTreeNode(); //如果所有的节点中都没有,则创建一个新节点
	p->ChildNodes[index] = NewNode;      //当前节点加入新子节点

	NewNode->EsigText = new char[Txt.length() + 1];strcpy_s(NewNode->EsigText, Txt.length() + 1, Txt.c_str());//赋值EsigTxt
	return NewNode;
}

TrieTreeNode* TrieTree::AddSpecialNode(TrieTreeNode* p, string& Txt) {		//特殊节点数组
	for (int i = 0;i < p->SpecialNodes.size();i++) {		//遍历当前子节点
		if (Txt.compare(p->SpecialNodes[i]->EsigText) == 0) {
			return p->SpecialNodes[i];
		}
	}

	TrieTreeNode* NewNode = new TrieTreeNode(); //如果所有的节点中都没有,则创建一个新节点
	p->SpecialNodes.push_back(NewNode);      //当前节点加入新子节点
	NewNode->EsigText = new char[Txt.length() + 1];strcpy_s(NewNode->EsigText, Txt.length() + 1, Txt.c_str());//赋值EsigTxt
	return NewNode;
}