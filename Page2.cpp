/*————————————————————————————
E-debug   DLL命令调用识别
————————————————————————————*/

#include "stdafx.h"
#include "E-Debug.h"
#include "Page2.h"
#include "afxdialogex.h"
#include "EAnalyEngine.h"
#include "MainWindow.h"


static int ordertype;
static int libtype;
static int cmdtype;
static int counttype;
// CPage2 对话框
extern  EAnalysis	*pEAnalysisEngine;
extern 	CMainWindow *pMaindlg;


IMPLEMENT_DYNAMIC(CPage2, CDialog)

CPage2::CPage2(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PAGE2, pParent)
{

}

CPage2::~CPage2()
{
	m_map.clear();
}

void CPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTAPI, m_api);
}


BEGIN_MESSAGE_MAP(CPage2, CDialog)
//	ON_NOTIFY(NM_RDBLCLK, IDC_LISTAPI, &CPage2::OnNMRDblclkListapi)
ON_NOTIFY(NM_RCLICK, IDC_LISTAPI, &CPage2::OnNMRClickListapi)
ON_COMMAND(ID_32771, &CPage2::On32771)
ON_NOTIFY(LVN_COLUMNCLICK, IDC_LISTAPI, &CPage2::OnLvnColumnclickListapi)
END_MESSAGE_MAP()


// CPage2 消息处理程序



BOOL CPage2::OnInitDialog() {
	CDialog::OnInitDialog();

	
	LONG lStyle;

	lStyle = GetWindowLong(m_api.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_api.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = m_api.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_api.SetExtendedStyle(dwStyle); //设置扩展风格

	m_api.InsertColumn(0, L"序号", LVCFMT_LEFT, 50);
	m_api.InsertColumn(1, L"DLL库", LVCFMT_CENTER, 90);
	m_api.InsertColumn(2, L"命令名称", LVCFMT_CENTER, 280);
	m_api.InsertColumn(3, L"引用次数", LVCFMT_CENTER, 80);

	USES_CONVERSION;
	char *pszLibname;
	char *pszApiname;
	unsigned long pszLibnameAddr;
	unsigned long pszApinameAddr;
	CString order;
	int nPos = 0;

	if (pEAnalysisEngine->pEnteyInfo->pLibName == 0) {
		pMaindlg->outputInfo("->  DllCmd个数为0...");
		return true;
	}

	UINT r_index = pEAnalysisEngine->FindSection(pEAnalysisEngine->pEnteyInfo->pLibName);
	if (r_index == -1) {
		r_index = pEAnalysisEngine->AddSection(pEAnalysisEngine->pEnteyInfo->pLibName);
	}

	pszLibnameAddr = pEAnalysisEngine->O2V(pEAnalysisEngine->pEnteyInfo->pLibName,r_index);
	pszApinameAddr = pEAnalysisEngine->O2V(pEAnalysisEngine->pEnteyInfo->pApiName,r_index);

	
	for (UINT i = 0;i < pEAnalysisEngine->pEnteyInfo->dwApiCount;i++) {
		pszLibname = (char*)pEAnalysisEngine->O2V(*(long*)pszLibnameAddr,r_index);
		pszApiname = (char*)pEAnalysisEngine->O2V(*(long*)pszApinameAddr, r_index);
		if (*(byte*)pszLibname == NULL) {
			pszLibname = "NULL";
		}
		order.Empty();
		order.Format(L"%d", i);
		m_api.InsertItem(nPos, order);    //序号
		m_api.SetItemData(nPos, i);       //ID号便是数据
		m_api.SetItemText(nPos, 1, A2W(pszLibname));
		m_api.SetItemText(nPos, 2, A2W(pszApiname));
		nPos++;
		pszLibnameAddr += 4;
		pszApinameAddr += 4;
	}

	
	byte ComCall[6] = { 0xB8, 0x90, 0x90, 0x00, 0x00 ,0xE8 };
	byte *pTmp = (byte*)pEAnalysisEngine->O2V(pEAnalysisEngine->dwUsercodeStart,0);

	DWORD	dwSecSize = pEAnalysisEngine->dwUsercodeEnd - pEAnalysisEngine->dwUsercodeStart;
	DWORD	dwResult = pEAnalysisEngine->dwUsercodeStart;    //搜寻结果地址
	DWORD   dwCount = 0;   //已搜索字节数
	DWORD   ORDER = 0;
	

	while (true) {

		DWORD offset = Search_Bin(pTmp, ComCall, dwSecSize, sizeof(ComCall));//得到偏移地址
		if (offset == 0) {
			break;
		}
		dwResult += offset;

		DWORD TEMP = (pEAnalysisEngine->O2V(dwResult + 6, 0));
		if (IsBadReadPtr((DWORD*)TEMP, 4) == 0) {
			DWORD CALLADDR = dwResult + 5 + 5 + *(DWORD*)TEMP;
			CALLADDR = pEAnalysisEngine->O2V(CALLADDR + 2, 0);
			if (IsBadReadPtr((DWORD*)CALLADDR, 4) == 0 && *(DWORD*)CALLADDR == pEAnalysisEngine->DLLCALL) {
				ORDER = *(DWORD*)(pEAnalysisEngine->O2V(dwResult + 1, 0));
				m_map[ORDER].push_back(dwResult);
				Insertname(dwResult, NM_COMMENT, W2A(m_api.GetItemText(ORDER, 2)));
			}
		}
		dwResult += sizeof(ComCall);
		pTmp += offset + sizeof(ComCall);
		dwSecSize -= offset + sizeof(ComCall);
	}

	for (UINT n = 0;n < m_map.size();n++) {     //写出引用次数
		order.Empty();
		order.Format(L"%d", m_map[n].size());
		m_api.SetItemText(n, 3,order);
	}

	pMaindlg->outputInfo("->  获取易语言<DllCmd>完毕...");
	//pMaindlg->outputInfo("->  Bug反馈: https://github.com/fjqisba/E-debug-plus");
	return true;
}


void CPage2::OnNMRClickListapi(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	static CMenu Menu;
	static CMenu *pSubMenu;
	if (Menu == NULL) {
		Menu.LoadMenu(IDR_MENU1);
		pSubMenu = Menu.GetSubMenu(0);
	}
	if (pNMItemActivate->iItem != -1) {
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	*pResult = 0;
}


void CPage2::On32771()  //DLL命令查找引用
{
	USES_CONVERSION;
	int nPos = m_api.GetSelectionMark();
	if (nPos == -1) {
		return;
	}

	pMaindlg->m_output.ResetContent();
	pMaindlg->outputInfo("-> 执行命令   --==查找引用==--");

	int order = m_api.GetItemData(nPos);  //得到真实的序号
	
	for (UINT n = 0;n < m_map[order].size();n++) {
		int index = pMaindlg->outputInfo("%08X    mov eax,%08X     %s", m_map[order][n], order, W2A(m_api.GetItemText(nPos,2))); //显示出结果地址
		pMaindlg->m_output.SetItemData(index, m_map[order][n]);
	}
}

static int CALLBACK CompareOrder(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
	if (lParamSort == 0) {
		return lParam1 > lParam2;
	}
	else if (lParamSort == 1) {
		return lParam2 > lParam1;
	}
	return 0;
}

static int CALLBACK CompareLib(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
	CString C1 = pMaindlg->m_page2.m_api.GetItemText(static_cast<int>(lParam1), 1);
	CString C2 = pMaindlg->m_page2.m_api.GetItemText(static_cast<int>(lParam2), 1);

	if (C1.GetLength() != C2.GetLength()) {
		return C1.GetLength() > C2.GetLength();
	}
	else {
		return  C1 > C2 ;
	}
}

static int CALLBACK CompareCmd(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
	CString C1 = pMaindlg->m_page2.m_api.GetItemText(static_cast<int>(lParam1), 2);
	CString C2 = pMaindlg->m_page2.m_api.GetItemText(static_cast<int>(lParam2), 2);
	if (C1.GetLength() != C2.GetLength()) {
		return C1.GetLength() > C2.GetLength();
	}
	else {
		return  C1 > C2;
	}
}

static int CALLBACK CompareCount(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
	return _ttoi(pMaindlg->m_page2.m_api.GetItemText(lParam1, 3)) > _ttoi(pMaindlg->m_page2.m_api.GetItemText(lParam2, 3));
}

void CPage2::OnLvnColumnclickListapi(NMHDR *pNMHDR, LRESULT *pResult)   //排序
{ 
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	if (pNMLV->iSubItem == 0) {  //序号
		m_api.SortItems(CompareOrder, (DWORD_PTR)ordertype);
		if (ordertype == 0) {  //正向排序
			ordertype = 1;
		}
		else {                //逆向排序
			ordertype = 0;
		}
	}
	else if (pNMLV->iSubItem == 1) {   //DLL库
		m_api.SortItemsEx(CompareLib, (DWORD_PTR)libtype);
	}
	else if (pNMLV->iSubItem == 2) {   //DLL命令名称
		m_api.SortItemsEx(CompareCmd, (DWORD_PTR)cmdtype);
	}
	else if (pNMLV->iSubItem == 3) {   //引用次数排序
		m_api.SortItemsEx(CompareCount, (DWORD_PTR)counttype);
	}

	*pResult = 0;
}


