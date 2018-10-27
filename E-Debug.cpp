// E-Debug.cpp : 定义 DLL 的初始化例程。
//



/*————————————————————————————
E-debug   DLL与OD的交互接口
————————————————————————————*/




#include "stdafx.h"
#include "E-Debug.h"
#include "MainWindow.h"   //载入窗口需要

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HINSTANCE	g_hInstace;


extc int _export cdecl ODBG_Plugindata(char shortname[32])
{
	strcpy(shortname, "E-Debug Plus 1.0");
	return PLUGIN_VERSION;
}

extc int _export cdecl ODBG_Plugininit(int ollydbgversion, HWND hw, DWORD *features)
{
	char tmpbuffer[MAX_PATH] = {};
	GetCurrentDirectoryA(MAX_PATH, tmpbuffer);
	StrCpyA((char*)DIRECTORY, tmpbuffer);
	
	if (ollydbgversion < PLUGIN_VERSION)
	{
		Addtolist(0, 0, "提示: 插件版本与OD不匹配!");
		return -1;
	}
	Addtolist(0, 0, "%s", "E-Debug Plus 1.0");
	Addtolist(0, -1, "%s","  by:fjqisba");

	//在这里修复一个OD 界面选项BUG
	int* Fix = (int*)34152;
	if (*(DWORD*)4647851 == 99688) {
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)4647851, &Fix, 4, NULL);
	}
	return 0;
}

extc int _export cdecl ODBG_Pluginmenu(int origin, CHAR data[4096], VOID *item)
{
	if (origin == PM_MAIN)
	{
		strcpy(data, "0&打开分析窗口|1&关于软件");
	}
	return 1;
}

extc void _export cdecl ODBG_Pluginaction(int origin, int action, VOID *item)
{
	if (origin == PM_MAIN) {
		switch (action) {
		case 0:
		{
			if (Getcputhreadid() == 0) {
				AfxMessageBox(L"未载入程序，请载入程序后，跳转到程序领空重试!");
				Flash("未载入程序!");
				break;
			}
			CMainWindow *pMainDlg = new CMainWindow;
			pMainDlg->Create(IDD_MainWindow,NULL);
			pMainDlg->ShowWindow(SW_SHOW);
			break;
		}
		case 1:
		{
			CString szInfo;
			szInfo += "Plugin:E-Debug Plus\r\n";
			szInfo += "Verion:1.0\r\n";
			szInfo += "Bug:fjqisba@sohu.com\r\n";
			szInfo += " Thanks to Xjun";
			MessageBox(NULL, szInfo, L"About", MB_ICONINFORMATION);
			break;
		}
		default:
			break;
		}
		

	}
}

BEGIN_MESSAGE_MAP(CEDebugApp, CWinApp)
END_MESSAGE_MAP()


// CEDebugApp 构造

CEDebugApp::CEDebugApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CEDebugApp 对象

CEDebugApp theApp;


// CEDebugApp 初始化

BOOL CEDebugApp::InitInstance()
{
	CWinApp::InitInstance();
	g_hInstace = AfxGetInstanceHandle();
	return TRUE;
}

