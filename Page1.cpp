/*————————————————————————————
E-debug   命令识别
————————————————————————————*/

#include "stdafx.h"
#include "E-Debug.h"
#include "Page1.h"
#include "EAnalyEngine.h"
#include "Elib.h"
#include "MainWindow.h"
#include "TrieTree.h"

extern CMainWindow *pMaindlg;
extern map<string, LIBMAP> m_LibMap;
extern vector<CDialog*> Tab_HWND;	//窗口控件指针


char DIRECTORY[MAX_PATH];
static int addrtype;
static int nametype;

IMPLEMENT_DYNAMIC(CPage1, CDialog)

CPage1::CPage1(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PAGE1, pParent)
{
	
}

CPage1::~CPage1()
{
	m_map.clear();
}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTLib, m_lib);
	DDX_Control(pDX, IDC_LISTCommand, m_command);
}

BEGIN_MESSAGE_MAP(CPage1, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LISTLib, &CPage1::OnNMClickListlib)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LISTCommand, &CPage1::OnLvnColumnclickListcommand)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCommand, &CPage1::OnNMDblclkListcommand)
	ON_NOTIFY(NM_RCLICK, IDC_LISTCommand, &CPage1::OnNMRClickListcommand)
	ON_COMMAND(ID_32771, &CPage1::On32771)
END_MESSAGE_MAP()

CDialog* CPage1::GetHwnd(){
	return this;
}

// CPage1 消息处理程序
BOOL CPage1::OnInitDialog() {
	CDialog::OnInitDialog();
	USES_CONVERSION;
	LONG lStyle;
	
	lStyle = GetWindowLong(m_lib.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_lib.m_hWnd, GWL_STYLE, lStyle);//设置style

	lStyle = GetWindowLong(m_command.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_command.m_hWnd, GWL_STYLE, lStyle);//设置style
	
	DWORD dwStyle = m_lib.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
									//dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_lib.SetExtendedStyle(dwStyle); //设置扩展风格

	dwStyle = m_command.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_command.SetExtendedStyle(dwStyle); //设置扩展风格


	m_lib.InsertColumn(0, _T("支持库信息"), LVCFMT_LEFT, 250);

	m_command.InsertColumn(0, L"地址", LVCFMT_LEFT, 65);
	m_command.InsertColumn(1, L"命令名称", LVCFMT_CENTER, 205);

	//————————————————————

	if (pEAnalysisEngine->EStaticLibInit()) {		//检测到是易语言静态编译程序

		pEAnalysisEngine->AnalysisMode = StaticMode;
		vector<string> krnlCmd =
		{ "错误回调", "DLL命令", "三方支持库命令", "核心支持库命令",
			"读取组件属性", "设置组件属性", "分配内存", "重新分配内存",
			"释放内存", "结束", "窗口消息循环", "载入启动窗口", "初始化" };

		Insertname(pEAnalysisEngine->dwUsercodeStart, NM_COMMENT, "用户代码段开始");

		if (!pEAnalysisEngine->GetUserEntryPoint()) {
			pEAnalysisEngine->dwUsercodeEnd = pEAnalysisEngine->SectionMap[0].dwBase + pEAnalysisEngine->SectionMap[0].dwSize - 1;
		}
		else {
			INT nPos = pMaindlg->outputInfo("->  易语言程序入口 : %08X     ", pEAnalysisEngine->dwUsercodeEnd);
			pMaindlg->m_output.SetItemData(nPos, pEAnalysisEngine->dwUsercodeEnd);
			Insertname(pEAnalysisEngine->dwUsercodeEnd, NM_COMMENT, "易语言程序入口");
		}

		DWORD	dwKrnlEntry = pEAnalysisEngine->pEnteyInfo->dwEString;
		if (dwKrnlEntry == 0) {
			dwKrnlEntry = pEAnalysisEngine->pEnteyInfo->dwEWindow;
		}
		DWORD	dwPoint;

		INT index = pEAnalysisEngine->UpdateSection(dwKrnlEntry);
		for (int i = krnlCmd.size() - 1; i >= 0; i--)
		{
			dwKrnlEntry -= sizeof(DWORD);
			dwPoint = pEAnalysisEngine->GetPoint(pEAnalysisEngine->O2V(dwKrnlEntry, index));
			if (i == 1) {				//获取DLL命令调用地址
				pEAnalysisEngine->DLLCALL = dwKrnlEntry;
			}
			Insertname(dwPoint, NM_LABEL, (char*)krnlCmd[i].c_str());
		}

		//——————————————————————————————

		int			nPos = 0;
		DWORD		pFirst = pEAnalysisEngine->pEnteyInfo->pLibEntey;
		PLIB_INFO	pLibInfo = NULL;
		CString     LibStr;
		
		INT r_index = pEAnalysisEngine->UpdateSection(pFirst);
		if (r_index == -1) {
			pMaindlg->outputInfo("致命的错误");
			return true;
		}
		for (UINT i = 0; i < pEAnalysisEngine->pEnteyInfo->dwLibNum; i++)  //对于解析出来的每个支持库
		{
			pLibInfo = (PLIB_INFO)pEAnalysisEngine->O2V(pEAnalysisEngine->GetOriginPoint(pFirst, r_index), r_index);
			string Name = (char*)pEAnalysisEngine->O2V((DWORD)pLibInfo->m_szName, r_index);
			string Guid = (char*)pEAnalysisEngine->O2V((DWORD)pLibInfo->m_szGuid, r_index);
			//得到最重要的Name和Guid,pLibInfo->m_nMajorVersion已屏蔽

			char szDirectory[MAX_PATH] = { 0 };
			for (UINT n = 0;n < EsigList.size();n++) {			//得到支持库路径
				if (EsigList[n].Category != "易语言") {
					continue;
				}
				if (EsigList[n].Name == Name && EsigList[n].Description == Guid) {
					strcpy_s(szDirectory, EsigList[n].Path.c_str());
					pMaindlg->m_page0.m_List.SetItemText(n, 1, L"已应用");
					break;
				}
			}
			TrieTree	Tree;
			BOOL Sret = Tree.LoadSig(szDirectory);    //读取ESig文件

			DWORD		pFunc = pEAnalysisEngine->O2V((DWORD)pLibInfo->m_pCmdsFunc, r_index);
			DWORD		dwAddress;
			if (Sret == false) {					  //如果读取不到Sig文件
				for (int n = 0;n < pLibInfo->m_nCmdCount;n++) {
					dwAddress = pEAnalysisEngine->GetPoint(pFunc);
					m_LibMap[Name].Command_addr.push_back(dwAddress);
					m_LibMap[Name].Command_name.push_back("Esig Not Founded");
					pFunc += sizeof(int);
				}
			}
			else {
				for (int n = 0;n < pLibInfo->m_nCmdCount;n++) {
					dwAddress = pEAnalysisEngine->GetPoint(pFunc);
					m_LibMap[Name].Command_addr.push_back(dwAddress);
					char* FuncName = Tree.MatchSig((UCHAR*)pEAnalysisEngine->O2V(dwAddress, 0));
					if (FuncName) {
						m_LibMap[Name].Command_name.push_back(FuncName);
						Insertname(dwAddress, NM_LABEL, FuncName);
					}
					else
					{
						m_LibMap[Name].Command_name.push_back("Error");
						Insertname(dwAddress, NM_LABEL, "未知命令");
					}
					pFunc += sizeof(int);
				}
			}

			LibStr.Format(L"%s(命令总数:%d)", A2W(Name.c_str()), m_LibMap[Name].Command_addr.size());
			pMaindlg->m_page1.m_lib.InsertItem(nPos, LibStr);nPos++;
			LibStr.Format(L"   %s", A2W(Guid.c_str()));
			pMaindlg->m_page1.m_lib.InsertItem(nPos, LibStr);nPos++;
			pMaindlg->m_page1.m_lib.InsertItem(nPos, L"――――――――――――――――――――――――");nPos++;
			pFirst += sizeof(DWORD);
		}
		//Progress(1000, "正在扫描基础特征,请等待......");
		//Progress(0, "");
		//Infoline("识别命令完毕...");
		pMaindlg->outputInfo("->  分析易语言<KrnlLibCmd>&&<LibCmd>完毕...");

		//————————————————————————

	}
	
	return true;
}


BOOL CPage1::MatchCode_FAST(UCHAR* FuncSrc, UCHAR* BinCode, int nLen)  //参数一与参数二对比,参数三为对比长度
{
	if (nLen == 0)
	{
		return FALSE;
	}
	for (int i = 0; i < nLen; i++)
	{
		if (FuncSrc[i] != BinCode[i])
			return FALSE;
	}
	return TRUE;
}

BOOL CPage1::MatchCode(UCHAR* FuncSrc,string& FuncTxt)  //参数一为虚拟地址,参数二为函数文本
{
	USES_CONVERSION;
	UCHAR* pSrc = FuncSrc;  //初始化函数代码指针

	for (UINT n = 0;n < FuncTxt.length();n++) {
		if (FuncTxt[n] == '-' && FuncTxt[n + 1] == '-' && FuncTxt[n + 2] == '>') {    //--> 长跳转,暂未通过实践验证
			if (*pSrc != 0xE9) {
				return false;
			}

			DWORD offset = *(DWORD*)(pSrc + 1);
			pSrc = pSrc + offset + 5;
			if (IsBadReadPtr(pSrc, 4) != 0) {
				return false;
			}

			n = n + 2;
			continue;
		}
		else if (FuncTxt[n] == '<' && FuncTxt[n + 1] == '[')  //FF15 CALL
		{
			if (*pSrc != 0xFF || *(pSrc + 1) != 0x15 || IsBadReadPtr((ULONG*)(pSrc + 2), 4) != 0) {      //含有VMP和SE代码
				return false;
			}

			int post = FuncTxt.find("]>", n);
			if (post == -1) {
				return false;
			}

			string IATEAT = FuncTxt.substr(n + 2, post - n - 2);   //得到文本中的IAT函数

			CString IATCom;
			CString EATCom;
			char buffer[256] = { 0 };
			int EATpos = IATEAT.find("||");
			if (EATpos != -1) {            //存在自定义EAT
				IATCom = IATEAT.substr(0, EATpos).c_str();
				EATCom = IATEAT.substr(EATpos + 2).c_str();
			}
			else
			{
				IATCom = IATEAT.c_str();
				EATCom = IATEAT.substr(IATEAT.find('.') + 1).c_str();
			}

			ULONG addr = *(ULONG*)(pSrc + 2);
			if (Findname(addr, NM_IMPORT, buffer) != 0 && IATCom.CompareNoCase(A2W(buffer)) == 0) {  //首先IAT匹配
				pSrc = pSrc + 6;
				n = post + 1;
				continue;
			}
			if (Findname(*(ULONG*)pEAnalysisEngine->O2V(addr, Currentindex), NM_EXPORT, buffer) != 0 && EATCom.CompareNoCase(A2W(buffer)) == 0) {      //EAT匹配
				pSrc = pSrc + 6;
				n = post + 1;
				continue;
			}
			return false;
		}
		else if (FuncTxt[n] == '<')    //普通CALL
		{
			if (*pSrc != 0xE8) {   //必须判断
				return false;
			}

			int post = FuncTxt.find('>', n);
			if (post == -1) {
				return false;
			}
			string SubFunc = FuncTxt.substr(n + 1, post - n - 1);   //子函数名称
			DWORD addr = (DWORD)pSrc + *(DWORD*)(pSrc + 1) + 5;

			if (m_RFunc[addr] == SubFunc) {       //首先查询此函数是否已匹配过
				pSrc = pSrc + 5;
				n = post;
				continue;
			}
			if (IsBadReadPtr((ULONG*)addr, 4) != 0) {
				return false;
			}
			if (MatchCode((UCHAR*)addr, m_subFunc[SubFunc])) {
				pSrc = pSrc + 5;
				n = post;
				m_RFunc[addr] = SubFunc;
				continue;
			}
			return false;
		}
		else if (FuncTxt[n] == '[' && FuncTxt[n + 1] == ']' && FuncTxt[n + 2] == '>')   //FF25函数跳转
		{
			if (*pSrc != 0xFF || *(pSrc + 1) != 0x25) {
				return false;
			}

			ULONG jmpaddr = *(ULONG*)(pSrc + 2);  //得到存放函数地址的指针

			UINT r_index = pEAnalysisEngine->FindOriginSection(jmpaddr);
			if (r_index == -1) {
				r_index = pEAnalysisEngine->AddSection(jmpaddr);
			}

			jmpaddr = *(ULONG*)pEAnalysisEngine->O2V(jmpaddr, r_index);   //得到函数地址
			if (jmpaddr == 0) {
				return false;
			}

			r_index = pEAnalysisEngine->FindOriginSection(jmpaddr);
			if (r_index == -1) {
				r_index = pEAnalysisEngine->AddSection(jmpaddr);
			}
			Currentindex = r_index;

			pSrc = (UCHAR*)pEAnalysisEngine->O2V(jmpaddr, Currentindex); //函数真实地址转换为虚拟地址

			n = n + 2;
			continue;
		}
		else if (FuncTxt[n] == '[')  //FF25 IAT跳转
		{
			if (*pSrc != 0xFF || *(pSrc + 1) != 0x25) {
				return false;
			}

			int post = FuncTxt.find(']', n);
			if (post == -1) {
				return false;
			}
			string IATEAT = FuncTxt.substr(n + 1, post - n - 1);

			if (IsBadReadPtr((ULONG*)(pSrc + 2), 4) != 0) {   //检查指针合法性
				return false;
			}

			CString IATCom;
			CString EATCom;
			char buffer[256] = { 0 };
			int EATpos = IATEAT.find("||");
			if (EATpos != -1) {            //存在自定义EAT
				IATCom = IATEAT.substr(0, EATpos).c_str();
				EATCom = IATEAT.substr(EATpos + 2).c_str();
			}
			else
			{
				IATCom = IATEAT.c_str();
				EATCom = IATEAT.substr(IATEAT.find('.') + 1).c_str();
			}

			ULONG addr = *(ULONG*)(pSrc + 2);
			if (Findname(addr, NM_IMPORT, buffer) != 0 && IATCom.CompareNoCase(A2W(buffer)) == 0) {  //首先IAT匹配
				pSrc = pSrc + 6;
				n = post;
				continue;
			}
			if (Findname(*(ULONG*)pEAnalysisEngine->O2V(addr, Currentindex), NM_EXPORT, buffer) != 0 && EATCom.CompareNoCase(A2W(buffer)) == 0) {      //EAT匹配
				pSrc = pSrc + 6;
				n = post;
				continue;
			}
			return false;
		}
		else if (FuncTxt[n] == '?')   //模糊匹配
		{
			n++;
			pSrc++;
			continue;
		}
		else     //普通代码
		{
			UCHAR ByteCode=0;
			HexToBin(FuncTxt.substr(n, 2), &ByteCode);
			if (*pSrc == ByteCode) {
				n++;
				pSrc++;
				continue;
			}
			//pMaindlg->outputInfo("失败地址:%X", pEAnalysisEngine->V2O((DWORD)pSrc, 0));

			//pMaindlg->outputInfo("失败文:%X", FuncSrc);
			return false;
		}
	}

	return TRUE;
}

bool CPage1::MatchCode_UnEx(unsigned char* pSrc1, unsigned char* pSrc2, int nLen)
{
	float Count = 0;
	if (nLen == 0)
	{
		return FALSE;
	}
	for (int i = 0; i < nLen; i++)
	{
		if (pSrc2[i] == 0x90)//模糊匹配
		{
			Count++;
			continue;
		}
		if (pSrc1[i] != pSrc2[i])
		{
			continue;
		}
		Count++;
	}
	if (Count / (float)nLen > 0.75f) {   
		return TRUE;
	}
	else {
		return false;
	}
}



void CPage1::OnNMClickListlib(NMHDR *pNMHDR, LRESULT *pResult)		//显示出命令和地址
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	USES_CONVERSION;
	
	
	int	nPos= m_lib.GetSelectionMark();
	CString	strCom = m_lib.GetItemText(nPos,0);
	
	string Name = W2A(strCom.GetBuffer());
	
	int Pos = Name.find("(命令总数:");
	if (Pos == -1) {
		return;
	}
	Name=Name.substr(0,Pos);	
	
	m_command.DeleteAllItems();
	CString		strAddress;
	for (UINT n = 0;n < m_LibMap[Name].Command_addr.size();n++) {
		strAddress.Format(L"%08X", m_LibMap[Name].Command_addr[n]);
		m_command.InsertItem(n, strAddress);
		m_command.SetItemData(n, m_LibMap[Name].Command_addr[n]);
		m_command.SetItemText(n, 1, A2W(m_LibMap[Name].Command_name[n].c_str()));
	}

	addrtype = 0;
	nametype = 0;
	*pResult = 0;
}

static int CALLBACK CompareAddr(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
	if (lParamSort == 0) {
		return lParam1 > lParam2;
	}
	else if (lParamSort == 1) {
		return lParam2 > lParam1;
	}
	return 0;
}

static int CALLBACK CompareName(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
	if (lParamSort == 0) {          //按照文本ASCII比较
		UCHAR x1 = *(UCHAR*)pMaindlg->m_page1.m_command.GetItemText(static_cast<int>(lParam1), 1).GetBuffer();
		UCHAR x2 = *(UCHAR*)pMaindlg->m_page1.m_command.GetItemText(static_cast<int>(lParam2), 1).GetBuffer();
		return x1 > x2;
	}
	else if (lParamSort == 1) {     //按照文本长度比较
		return pMaindlg->m_page1.m_command.GetItemText(static_cast<int>(lParam1), 1).GetLength() > pMaindlg->m_page1.m_command.GetItemText(static_cast<int>(lParam2), 1).GetLength();
	}
	return 0;
}

void CPage1::OnLvnColumnclickListcommand(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->iSubItem == 0) {
		m_command.SortItems(CompareAddr, (DWORD_PTR)addrtype);
		if (addrtype == 0) {  //正向排序
			addrtype = 1;
		}
		else {                //逆向排序
			addrtype = 0;
		}
	}
	else if (pNMLV->iSubItem == 1) {
		m_command.SortItemsEx(CompareName, (DWORD_PTR)nametype);
		if (nametype == 0) {       //按照ASCII排序
			nametype = 1;
		}
		else {                    //按照长度排序
			nametype = 0;
		}
	}
	*pResult = 0;
}



void CPage1::OnNMDblclkListcommand(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	if (m_command.GetSelectedCount() <= 0) {    //说明未选取到命令
		return;
	}
	int nPos = m_command.GetSelectionMark();
	if (nPos == -1) {
		return;
	}
	int dwData = m_command.GetItemData(nPos);
	if (!dwData)
		return;

	Setcpu(0, dwData, 0, 0, CPU_NOFOCUS);

	*pResult = 0;
}

void CPage1::OnNMRClickListcommand(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	CMenu Menu;
	CMenu *pSubMenu;
	if (pNMItemActivate->iItem != -1) {
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		Menu.LoadMenu(IDR_MENU1);
		pSubMenu = Menu.GetSubMenu(0);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	*pResult = 0;
}

void CPage1::On32771()   //查找引用按钮
{
	USES_CONVERSION;
	int nPos = m_command.GetSelectionMark();
	if (nPos == -1) {
		return;
	}
	DWORD dwData = m_command.GetItemData(nPos);
	if (!dwData) {
		return;
	}

	CString	strCom= m_command.GetItemText(nPos, 1);

	if (pEAnalysisEngine->AnalysisMode == StaticMode) {
		pMaindlg->m_output.ResetContent();
		pMaindlg->outputInfo("-> 执行命令   --==查找引用==--");
		byte ComCall[5] = { 0xBB, 0, 0, 0, 0 };
		memcpy(&ComCall[1], &dwData, sizeof(DWORD));
		byte *pTmp = (byte*)pEAnalysisEngine->O2V(pEAnalysisEngine->dwUsercodeStart, 0);

		DWORD	dwSecSize = pEAnalysisEngine->dwUsercodeEnd - pEAnalysisEngine->dwUsercodeStart;
		DWORD	dwResult = pEAnalysisEngine->dwUsercodeStart;    //搜寻结果地址
		DWORD   dwCount = 0;   //已搜索字节数
		while (true) {
			DWORD offset = pEAnalysisEngine->Search_BinEx(pTmp, ComCall, dwSecSize, sizeof(ComCall));//得到偏移地址
			if (offset == 0)
				break;
			dwResult += offset;
			int index = pMaindlg->outputInfo("%08X    mov ebx,%08X    //%s", dwResult, dwData, W2A(strCom)); //显示出结果地址
			pMaindlg->m_output.SetItemData(index, dwResult);
			dwResult += sizeof(ComCall);
			pTmp += offset + sizeof(ComCall);
			dwSecSize -= offset + sizeof(ComCall);
		}
	}
}
