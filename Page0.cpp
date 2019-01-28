// Page0.cpp : 实现文件
//

#include "stdafx.h"
#include "E-Debug.h"
#include "Page0.h"
#include "MainWindow.h"
#include "TrieTree.h"

extern map<string, LIBMAP> m_LibMap;
extern vector<CDialog*> Tab_HWND;
extern CMainWindow *pMaindlg;

// CPage0 对话框

IMPLEMENT_DYNAMIC(CPage0, CDialog)

CPage0::CPage0(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PAGE0, pParent)
{

}

CPage0::~CPage0()
{

}

void CPage0::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CPage0, CDialog)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CPage0::OnNMRClickList1)
	ON_COMMAND(ID_32772, &CPage0::On32772)
END_MESSAGE_MAP()

CDialog* CPage0::GetHwnd() {
	return this;
}

// CPage0 消息处理程序
BOOL CPage0::OnInitDialog() {

	CDialog::OnInitDialog();

	LONG lStyle;

	lStyle = GetWindowLong(m_List.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_List.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = m_List.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_List.SetExtendedStyle(dwStyle); //设置扩展风格

	m_List.InsertColumn(0, L"文件", LVCFMT_LEFT, 175);
	m_List.InsertColumn(1, L"选项", LVCFMT_CENTER, 80);
	m_List.InsertColumn(2, L"描述", LVCFMT_CENTER, 270);

	USES_CONVERSION;

	for (UINT n = 0;n < EsigList.size();n++) {
		m_List.InsertItem(n, A2W(EsigList[n].Name.c_str()));		
		m_List.SetItemText(n, 2, A2W(EsigList[n].Description.c_str()));
	}

	return true;
}

void CPage0::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	static CMenu Menu;
	static CMenu *pSubMenu;
	if (Menu == NULL) {
		Menu.LoadMenu(IDR_MENU2);
		pSubMenu = Menu.GetSubMenu(0);
	}
	if (pNMItemActivate->iItem != -1) {
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	*pResult = 0;
}


void CPage0::On32772()	//应用特征
{
	USES_CONVERSION;
	int nPos = m_List.GetSelectionMark();
	if (nPos == -1) {
		return;
	}

	nPos = m_List.GetNextItem(-1, LVNI_SELECTED);
	while (nPos != -1) {
		CString	strCom = m_List.GetItemText(nPos, 0);

		EsigInfo info;
		for (UINT n = 0;n < EsigList.size();n++) {
			if (strCom.Compare(A2W(EsigList[n].Name.c_str())) == 0) {
				info = EsigList[n];
				break;
			}
		}

		int flag = m_List.GetItemText(nPos, 1).Compare(L"已应用");	//是否已应用过特征

		m_LibMap[info.Name].Command_addr.clear();
		m_LibMap[info.Name].Command_name.clear();

		if (flag == 0) {		//如果已经应用过则删除原先特征
			LVFINDINFO findinfo;
			findinfo.flags = LVFI_PARTIAL | LVFI_STRING;
			findinfo.psz = A2W(info.Name.c_str());
			for (UINT n = 0;n < pMaindlg->m_page1.m_lib.GetItemCount();n++) {
				if (pMaindlg->m_page1.m_lib.GetItemText(n, 0).Find(A2W(info.Name.c_str())) != -1) {		
					pMaindlg->m_page1.m_lib.DeleteItem(n);
					pMaindlg->m_page1.m_lib.DeleteItem(n);
					pMaindlg->m_page1.m_lib.DeleteItem(n);
					break;
				}
			}		
		}

		TrieTree Tree;
		Tree.LoadSig(info.Path.c_str());
		
		Tree.ScanSig(pEAnalysisEngine->SectionMap[0].SectionAddr, pEAnalysisEngine->SectionMap[0].dwSize, info.Name);


		UINT Pos = 0;

		CString LibStr;

		if (m_LibMap[info.Name].Command_addr.size() != 0) {		
			LibStr.Format(L"%s(命令总数:%d)", A2W(info.Name.c_str()), m_LibMap[info.Name].Command_addr.size());
			pMaindlg->m_page1.m_lib.InsertItem(Pos, LibStr);Pos++;

			LibStr.Format(L"   %s", A2W(info.Description.c_str()));
			pMaindlg->m_page1.m_lib.InsertItem(Pos, LibStr);Pos++;
			pMaindlg->m_page1.m_lib.InsertItem(Pos, L"――――――――――――――――――――――――");Pos++;
		}

		m_List.SetItemText(nPos, 1, L"已应用");
		nPos = m_List.GetNextItem(nPos, LVNI_SELECTED);	
	}
	// TODO: 在此添加命令处理程序代码
}
