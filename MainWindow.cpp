// MainWindow.cpp : 实现文件
//


/*
	MainWindow,分析界面交互的接口
*/


#include "stdafx.h"
#include "E-Debug.h"
#include "afxdialogex.h"
#include "MainWindow.h"
#include <vector>
#include "EAnalyEngine.h"

extern HINSTANCE g_hInstace;

CMainWindow *pMaindlg;
EAnalysis	*pEAnalysisEngine;

// CMainWindow 对话框

IMPLEMENT_DYNAMIC(CMainWindow, CDialog)

CMainWindow::CMainWindow(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MainWindow, pParent)
{

}

CMainWindow::~CMainWindow()
{

}

void CMainWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	DDX_Control(pDX, IDC_LOG, m_output);
}


BEGIN_MESSAGE_MAP(CMainWindow, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMainWindow::OnTcnSelchangeTab1)
	ON_LBN_SELCHANGE(IDC_LOG, &CMainWindow::OnLbnSelchangeLog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMainWindow 消息处理程序


BOOL CMainWindow::OnInitDialog() {
	CDialog::OnInitDialog();
	pMaindlg = this;   //得到窗口指针
	
	HICON hIcon = LoadIcon(g_hInstace, MAKEINTRESOURCE(IDI_ICON));
	SetIcon(hIcon, FALSE);
	SetIcon(hIcon, TRUE);

	m_Tab.InsertItem(0, _T("支持库命令"));
	m_Tab.InsertItem(1, _T("API命令"));
	//m_Tab.InsertItem(2, _T("窗口组件"));

	CRect rc;
	m_Tab.GetClientRect(&rc);
	rc.top += 26;
	rc.left += 4;
	rc.right -= 6;
	rc.bottom -= 6;


	ULONG uBase, uSize;
	INT nPos;
	Getdisassemblerrange(&uBase, &uSize);

	outputInfo("->开始分析当前区段....  分析地址: % 08X  内存大小: % 08X", uBase, uSize);
	
	pEAnalysisEngine = new EAnalysis(uBase,uSize);   //初始化内存
	if (pEAnalysisEngine->EStaticLibInit()) {    //易语言静态编译识别+初始化,识别失败返回false
		
		std::vector<std::string> krnlCmd =
		{ "错误回调", "DLL命令", "三方支持库命令", "核心支持库命令",
			"读取组件属性", "设置组件属性", "分配内存", "重新分配内存",
			"释放内存", "结束", "窗口消息循环", "载入启动窗口", "初始化" };

		nPos = outputInfo("->  用户自写代码段开始 : %08X     ", pEAnalysisEngine->dwUsercodeStart);
		m_output.SetItemData(nPos, pEAnalysisEngine->dwUsercodeStart);
		Insertname(pEAnalysisEngine->dwUsercodeStart, NM_COMMENT, "用户代码段开始");

		if (!pEAnalysisEngine->GetUserEntryPoint()) {
			pEAnalysisEngine->dwUsercodeEnd = uBase + uSize -1;
			nPos = outputInfo("->  用户自写代码段结束 : %08X     ", pEAnalysisEngine->dwUsercodeEnd);
		}
		else {
			nPos = outputInfo("->  用户自写代码段结束 : %08X     ", pEAnalysisEngine->dwUsercodeEnd);
			m_output.SetItemData(nPos, pEAnalysisEngine->dwUsercodeEnd);
			Insertname(pEAnalysisEngine->dwUsercodeEnd, NM_COMMENT, "易语言程序入口");
		}

		DWORD	dwKrnlEntry = pEAnalysisEngine->pEnteyInfo->dwEString;
		if (dwKrnlEntry == 0) {
			dwKrnlEntry = pEAnalysisEngine->pEnteyInfo->dwEWindow;
		}
		DWORD	dwPoint;

		UINT index = pEAnalysisEngine->FindSection(dwKrnlEntry);

		if (index == -1) {           //区段有可能被切割,加入新的区段
			index = pEAnalysisEngine->AddSection(dwKrnlEntry);
		}
	
		int proadd = 100 / krnlCmd.size();
		for (int i = krnlCmd.size() - 1; i >= 0; i--)
		{
			Progress(promile = promile + proadd, "正在识别核心支持库命令");
			dwKrnlEntry -= sizeof(DWORD);
			dwPoint = pEAnalysisEngine->GetPoint(pEAnalysisEngine->O2V(dwKrnlEntry,index));
			Insertname(dwPoint, NM_LABEL, (char*)krnlCmd[i].c_str());
		}

		m_page1.Create(IDD_PAGE1, &m_Tab);
		m_page1.MoveWindow(&rc);

		m_page2.Create(IDD_PAGE2, &m_Tab);
		m_page2.MoveWindow(&rc);

	}

	else {
		outputInfo("->  该程序有可能不是易语言程序...");
		return true;
	}

	/*m_page3.Create(IDD_PAGE3, &m_Tab);
	m_page3.MoveWindow(&rc);*/

	m_page1.ShowWindow(true);

	return true;
}

void CMainWindow::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nCursel = m_Tab.GetCurSel()+1;
	switch (nCursel)
	{
	case 1:
		if (IsWindow(m_page1.m_hWnd)) {
			m_page1.ShowWindow(true);
		}
		if (IsWindow(m_page2.m_hWnd)) {
			m_page2.ShowWindow(false);
		}
		if (IsWindow(m_page3.m_hWnd)) {
			m_page3.ShowWindow(false);
		}
		break;
	case 2:
		if (IsWindow(m_page1.m_hWnd)) {
			m_page1.ShowWindow(false);
		}
		if (IsWindow(m_page2.m_hWnd)) {
			m_page2.ShowWindow(true);
		}
		if (IsWindow(m_page3.m_hWnd)) {
			m_page3.ShowWindow(false);
		}
		break;
	case 3:
		if (IsWindow(m_page1.m_hWnd)) {
			m_page1.ShowWindow(false);
		}
		if (IsWindow(m_page2.m_hWnd)) {
			m_page2.ShowWindow(false);
		}
		if (IsWindow(m_page3.m_hWnd)) {
			m_page3.ShowWindow(true);
		}
		break;
	default:
		break;
	}
	*pResult = 0;
}

int CMainWindow::outputInfo(char *formatText, ...)
{
	USES_CONVERSION;
	char buf[10000] = { 0 };
	va_list st;
	va_start(st, formatText);
	vsprintf_s(buf, formatText, st);
	va_end(st);
	return m_output.InsertString(-1, A2W(buf));
}


void CMainWindow::OnLbnSelchangeLog()
{
	int		nCurSel;
	DWORD	dwItemData;
	nCurSel = m_output.GetCurSel();
	dwItemData = m_output.GetItemData(nCurSel);
	if (dwItemData != 0)
	{
		Setcpu(NULL, dwItemData, NULL, NULL, CPU_NOFOCUS);
	}
}




void CMainWindow::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (MessageBox(L"确定要退出分析窗口吗？", L"退出提示", MB_ICONINFORMATION | MB_YESNO) == IDNO)
		return; 
	CDialog::OnClose();
}
