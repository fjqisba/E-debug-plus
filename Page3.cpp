// Page3.cpp : 实现文件
//

#include "stdafx.h"
#include "E-Debug.h"
#include "Page3.h"
#include "afxdialogex.h"
#include "MainWindow.h"
#include "EAnalyEngine.h"

extern 	CMainWindow *pMaindlg;
extern  EAnalysis	*pEAnalysisEngine;

// CPage3 对话框

IMPLEMENT_DYNAMIC(CPage3, CDialog)



HTREEITEM hroot;

CPage3::CPage3(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PAGE3, pParent)
{

}

CPage3::~CPage3()
{
}

void CPage3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	DDX_Control(pDX, IDC_STATIC_BOX, ControlType);
}


BEGIN_MESSAGE_MAP(CPage3, CDialog)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE1, &CPage3::OnTvnItemexpandingTree1)
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE1, &CPage3::OnTvnSelchangingTree1)
END_MESSAGE_MAP()


// CPage3 消息处理程序

BOOL CPage3::OnInitDialog() {
	CDialog::OnInitDialog();


	m_Tree.SetLineColor(RGB(0, 0, 255));

	hroot=m_Tree.InsertItem(_T("易语言窗口"));

	
	m_ICO.Create(16,16,ILC_COLOR32,255,255);
	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_UnKnow));  //默认图标

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_Window));  //窗口  1

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_EDIT));   //编辑框 2

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_Pic));    //图片框 3 

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_ShapeBox));    //外形框 4

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_DrawPanel));    //画板 5

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_GroupBox));    //分组框 6

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_Label));   //标签 7

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_BUTTON));  //按钮 8

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_CheckBox));  //选择框 9

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_RadioBox));  //单选框 10

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_ComboBox));  //组合框 11

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_ListBox));  //列表框 12
	
	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_ChkListBox));  //选择列表框 13

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_HScrollBar));  //横向滚动条 14

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_VScrollBar));  //纵向滚动条 15

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_ProcessBar));  //进度条 16

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_SliderBar));  //滑块条 17

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_Tab));  //选择夹 18

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_AnimateBox));  //影像框 19

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_DatePicker));  //日期框 20

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_MonthCalendar));  //月历 21

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_DriverBox));  //驱动器框 22

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_DirBox));  //目录框 23

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_FileBox));  //文件框 24

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_ColorPicker));  //颜色选择器 25

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_HyperLinker));  //超级链接框 26

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_Spin));  //调节器 27

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_CommonDlg));  //通用对话框 28

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_Timer));  //时钟 29

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_printer));  //打印机 30

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_UDP));  //数据报 31

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_Client));  //客户 32

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_Server));  //服务器 33

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_SerialPort));  //端口 34

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_Grid));  //表格 35

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_DataSrc));  //数据源 36
	
	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_NProvider));  //通用提供者 37

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_DBProvider));  //数据库提供者 38

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_PicBtn));  //图形按钮 39

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_ODBCDB));  //外部数据库 40

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_ODBCProvider));  //外部数据库提供者 41

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_StatusBar));  //状态条 42

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_ToolBar));  //公共条 43

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_ListView));  //超级列表框 44

	m_ICO.Add(AfxGetApp()->LoadIcon(IDI_ICON_TransLabel));  //透明标签 45

	m_Tree.SetImageList(&m_ICO,TVSIL_NORMAL);

	//————————————————
	WindowInfo m_WindowInfo;
	DWORD EipAddr = pEAnalysisEngine->pEnteyInfo->dwEWindow; //Eip地址为当前解析地址
	if (EipAddr == 0) {
		return TRUE;
	}

	UINT r_index = pEAnalysisEngine->FindSection(EipAddr);
	if (r_index == -1) {
		r_index = pEAnalysisEngine->AddSection(EipAddr);
	}

	m_WindowInfo.WindowCount =*(unsigned long*)pEAnalysisEngine->O2V(EipAddr,r_index)>>3;
	if (m_WindowInfo.WindowCount == 0) {
		return TRUE;
	}


	CString WinID;
	CString ControlID;

	
	for (int i = 0;i < m_WindowInfo.WindowCount;i++) {
		EipAddr = EipAddr + 4;
		WinID.Format(L"0x%X", *(DWORD*)(pEAnalysisEngine->O2V(EipAddr,r_index)));
		HTREEITEM temp = m_Tree.InsertItem(WinID, hroot);
		m_WindowInfo.WindowId.push_back(temp);
		m_Tree.SetItemData(temp, i); //保存窗口index
	}

	EipAddr = EipAddr + 4 * (m_WindowInfo.WindowCount+1); //跳过WindowAddr

	WindowPropery m_WindowPropery = {};

	//——————————————————————————————————

	

	DWORD NextWindowProery = EipAddr + 24; //窗口起始地址


	for (int i = 0;i < m_WindowInfo.WindowCount;i++) {      //解析窗口
		Window.push_back(m_WindowPropery);
		EipAddr = EipAddr + 16;

		Window[i].ControlCount = *(unsigned long*)pEAnalysisEngine->O2V(EipAddr,r_index);  //得到控件个数
		
		if (Window[i].ControlCount == 0) {
			return true;
		}
		
		EipAddr = EipAddr + 4;
		Window[i].ControlSize= *(unsigned long*)pEAnalysisEngine->O2V(EipAddr,r_index);   //得到控件总大小
		
		
		for (int j = 0;j < Window[i].ControlCount;j++) {
			EipAddr = EipAddr + 4;
			ControlID.Format(L"0x%X", *(unsigned long*)pEAnalysisEngine->O2V(EipAddr, r_index));
			Window[i].ControlID.push_back(m_Tree.InsertItem(ControlID,m_WindowInfo.WindowId[i])); //保存控件ID
		}
		
		for (int t = 0;t < Window[i].ControlCount;t++) {          //解析控件
			EipAddr = EipAddr + 4;
			Window[i].Controloffset.push_back(*(unsigned long*)pEAnalysisEngine->O2V(EipAddr, r_index));
			
		}



		DWORD ControlStartAddr = EipAddr + 8;  //初始化控件基址

		for (int t = 0;t < Window[i].ControlCount;t++) {

			EipAddr = ControlStartAddr+ Window[i].Controloffset[t];
			Window[i].ChildWindowId = *(unsigned long*)pEAnalysisEngine->O2V(EipAddr, r_index);

			m_Tree.SetItemData(Window[i].ControlID[t],t); //保存控件顺序

			switch (Window[i].ChildWindowId)
			{
			case 0x10001:  //窗体
				Window[i].ControlType.push_back("窗口");
				m_Tree.SetItemImage(Window[i].ControlID[t], 1, 1);
				break;
			case 0x10005: //编辑框
				Window[i].ControlType.push_back("编辑框");
				m_Tree.SetItemImage(Window[i].ControlID[t], 2, 2);
				break;
			case 0x10006: //图片框
				Window[i].ControlType.push_back("图片框");
				m_Tree.SetItemImage(Window[i].ControlID[t], 3, 3);
				break;
			case 0x10007: //外形框
				Window[i].ControlType.push_back("外形框");
				m_Tree.SetItemImage(Window[i].ControlID[t], 4, 4);
				break;
			case 0x10008: //画板
				Window[i].ControlType.push_back("画板");
				m_Tree.SetItemImage(Window[i].ControlID[t], 5, 5);
				break;
			case 0x10009: //分组框
				Window[i].ControlType.push_back("分组框");
				m_Tree.SetItemImage(Window[i].ControlID[t], 6, 6);
				break;
			case 0x1000A: //标签
				Window[i].ControlType.push_back("标签");
				m_Tree.SetItemImage(Window[i].ControlID[t], 7, 7);
				break;
			case 0x1000B: //按钮
				Window[i].ControlType.push_back("按钮");
				m_Tree.SetItemImage(Window[i].ControlID[t], 8, 8);
				break;
			case 0x1000C: //选择框
				Window[i].ControlType.push_back("选择框");
				m_Tree.SetItemImage(Window[i].ControlID[t], 9, 9);
				break;
			case 0x1000D://单选框
				Window[i].ControlType.push_back("单选框");
				m_Tree.SetItemImage(Window[i].ControlID[t], 10, 10);
				break;
			case 0x1000E://组合框
				Window[i].ControlType.push_back("组合框");
				m_Tree.SetItemImage(Window[i].ControlID[t], 11, 11);
				break;
			case 0x1000F://列表框
				Window[i].ControlType.push_back("列表框");
				m_Tree.SetItemImage(Window[i].ControlID[t], 12, 12);
				break;
			case 0x10010://选择列表框
				Window[i].ControlType.push_back("选择列表框");
				m_Tree.SetItemImage(Window[i].ControlID[t], 13, 13);
				break;
			case 0x10011://横向滚动条
				Window[i].ControlType.push_back("横向滚动条");
				m_Tree.SetItemImage(Window[i].ControlID[t], 14, 14);
				break;
			case 0x10012://纵向滚动条
				Window[i].ControlType.push_back("纵向滚动条");
				m_Tree.SetItemImage(Window[i].ControlID[t], 15, 15);
				break;
			case 0x10013:
				Window[i].ControlType.push_back("进度条");
				m_Tree.SetItemImage(Window[i].ControlID[t], 16, 16);
				break;
			case 0x10014:
				Window[i].ControlType.push_back("滑块条");
				m_Tree.SetItemImage(Window[i].ControlID[t], 17, 17);
				break;
			case 0x10015:
				Window[i].ControlType.push_back("选择夹");
				m_Tree.SetItemImage(Window[i].ControlID[t], 18, 18);
				break;
			case 0x10016:
				Window[i].ControlType.push_back("影像框");
				m_Tree.SetItemImage(Window[i].ControlID[t], 19, 19);
				break;
			case 0x10017:
				Window[i].ControlType.push_back("日期框");
				m_Tree.SetItemImage(Window[i].ControlID[t], 20, 20);
				break;
			case 0x10018:
				Window[i].ControlType.push_back("月历");
				m_Tree.SetItemImage(Window[i].ControlID[t], 21, 21);
				break;
			case 0x10019:
				Window[i].ControlType.push_back("驱动器框");
				m_Tree.SetItemImage(Window[i].ControlID[t], 22, 22);
				break;
			case 0x1001A:
				Window[i].ControlType.push_back("目录框");
				m_Tree.SetItemImage(Window[i].ControlID[t], 23, 23);
				break;
			case 0x1001B:
				Window[i].ControlType.push_back("文件框");
				m_Tree.SetItemImage(Window[i].ControlID[t], 24, 24);
				break;
			case 0x1001C:
				Window[i].ControlType.push_back("颜色选择器");
				m_Tree.SetItemImage(Window[i].ControlID[t], 25, 25);
				break;
			case 0x1001D:
				Window[i].ControlType.push_back("超级链接框");
				m_Tree.SetItemImage(Window[i].ControlID[t], 26, 26);
				break;
			case 0x1001E:
				Window[i].ControlType.push_back("调节器");
				m_Tree.SetItemImage(Window[i].ControlID[t], 27, 27);
				break;
			case 0x1001F:
				Window[i].ControlType.push_back("通用对话框");
				m_Tree.SetItemImage(Window[i].ControlID[t], 28, 28);
				break;
			case 0x10020:
				Window[i].ControlType.push_back("时钟");
				m_Tree.SetItemImage(Window[i].ControlID[t], 29, 29);
				break;
			case 0x10021:
				Window[i].ControlType.push_back("打印机");
				m_Tree.SetItemImage(Window[i].ControlID[t], 30, 30);
				break;
			case 0x10024:
				Window[i].ControlType.push_back("数据报");
				m_Tree.SetItemImage(Window[i].ControlID[t], 31, 31);
				break;
			case 0x10025:
				Window[i].ControlType.push_back("客户");
				m_Tree.SetItemImage(Window[i].ControlID[t], 32, 32);
				break;
			case 0x10026:
				Window[i].ControlType.push_back("服务器");
				m_Tree.SetItemImage(Window[i].ControlID[t], 33, 33);
				break;
			case 0x10027:
				Window[i].ControlType.push_back("端口");
				m_Tree.SetItemImage(Window[i].ControlID[t], 34, 34);
				break;
			case 0x10029:
				Window[i].ControlType.push_back("表格");
				m_Tree.SetItemImage(Window[i].ControlID[t], 35, 35);
				break;
			case 0x1002A:
				Window[i].ControlType.push_back("数据源");
				m_Tree.SetItemImage(Window[i].ControlID[t], 36, 36);
				break;
			case 0x1002B:
				Window[i].ControlType.push_back("通用提供者");
				m_Tree.SetItemImage(Window[i].ControlID[t], 37, 37);
				break;
			case 0x1002C:
				Window[i].ControlType.push_back("数据库提供者");
				m_Tree.SetItemImage(Window[i].ControlID[t], 38, 38);
				break;
			case 0x1002D:
				Window[i].ControlType.push_back("图形按钮");
				m_Tree.SetItemImage(Window[i].ControlID[t], 39, 39);
				break;
			case 0x1002E:
				Window[i].ControlType.push_back("外部数据库");
				m_Tree.SetItemImage(Window[i].ControlID[t], 40, 40);
				break;
			case 0x1002F:
				Window[i].ControlType.push_back("外部数据库提供者");
				m_Tree.SetItemImage(Window[i].ControlID[t], 41, 41);
				break;
			default:
				Window[i].ControlType.push_back("未知");
				//pMaindlg->outputInfo("%X", Window[i].ChildWindowId);
				break;
			}
		}

		NextWindowProery = NextWindowProery + Window[i].ControlSize +24;
		EipAddr = NextWindowProery -24 ;
	}

	
	for (int i = 0;i < m_WindowInfo.WindowCount;i++) {
		m_Tree.Expand(m_WindowInfo.WindowId[i],TVE_EXPAND);
	}
	m_Tree.Expand(hroot,TVE_EXPAND);
	
	return TRUE;
}



void CPage3::OnTvnItemexpandingTree1(NMHDR *pNMHDR, LRESULT *pResult)   //点击易语言窗口的时候,不让收缩
{
	USES_CONVERSION;
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	
	if (pNMTreeView->itemNew.hItem == hroot && pNMTreeView->action==TVE_COLLAPSE) {
		*pResult = TRUE;
	}
	else {
		*pResult = 0;
	}


}


void CPage3::OnTvnSelchangingTree1(NMHDR *pNMHDR, LRESULT *pResult)  //控件被单击
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	if (m_Tree.GetChildItem(pNMTreeView->itemNew.hItem) == 0) {   //说明为控件
		
		

		
		DWORD m_index = m_Tree.GetItemData(m_Tree.GetParentItem(pNMTreeView->itemNew.hItem));
		DWORD n_index = m_Tree.GetItemData(pNMTreeView->itemNew.hItem);
		
		if (!Window[m_index].ControlType[n_index].empty()) {
			ControlType.SetWindowText(A2W(Window[m_index].ControlType[n_index].c_str()));
		}


	}
	

	*pResult = 0;
}
