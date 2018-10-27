#pragma once
#include "afxcmn.h"
#include "Page1.h"
#include "Page2.h"
#include "Page3.h"
#include "afxwin.h"

// CMainWindow 对话框

class CMainWindow : public CDialog
{
	DECLARE_DYNAMIC(CMainWindow)

public:
	CMainWindow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainWindow();
	int outputInfo(char *formatText, ...);
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MainWindow };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL Initsucceed;  //为假则退出流程
	virtual BOOL OnInitDialog();
	CTabCtrl m_Tab;
	CPage1 m_page1;
	CPage2 m_page2;
	CPage3 m_page3;

	double promile=0;

	
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CListBox m_output;
	afx_msg void OnLbnSelchangeLog();
	afx_msg void OnClose();
};
