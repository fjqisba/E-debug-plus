#pragma once
#include "E-Debug.h"


// CPage0 对话框

class CPage0 : public CDialog
{
	DECLARE_DYNAMIC(CPage0)

public:
	CPage0(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage0();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE0 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CDialog* GetHwnd();
	virtual BOOL OnInitDialog();
	CListCtrl m_List;
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On32772();
	BOOL ApplyEsig(string& name);
};
