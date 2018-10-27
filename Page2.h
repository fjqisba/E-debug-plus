#pragma once
#include "afxcmn.h"
#include <map>
#include <vector>

using namespace std;
// CPage2 对话框

class CPage2 : public CDialog
{
	DECLARE_DYNAMIC(CPage2)

public:
	CPage2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage2();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_api;
	map <int, vector<DWORD>> m_map;
	
//	afx_msg void OnNMRDblclkListapi(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListapi(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On32771();
	afx_msg void OnLvnColumnclickListapi(NMHDR *pNMHDR, LRESULT *pResult);
};
