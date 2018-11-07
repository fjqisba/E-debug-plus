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
	t_dump pd;
	if (origin == PM_MAIN)
	{
		strcpy(data, "0&打开分析窗口|1&关于软件");
		return 1;
	}
	if (origin == PM_DISASM || origin == PM_CPUDUMP) {
		strcpy(data, "0 易语言{0 取十六进制|1 到字节集}");
		return 1;
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
		return;
	}
	HGLOBAL hClip;
	t_dump* DumpData;
	if (origin == PM_DISASM) {
		DumpData = (t_dump*)Plugingetvalue(VAL_CPUDASM);
	}
	else if(origin==PM_CPUDUMP)
	{
		DumpData = (t_dump*)Plugingetvalue(VAL_CPUDDUMP);
	}

	int len = DumpData->sel1 - DumpData->sel0;
	byte *buffer = new byte[len];
	memset(buffer, 0, len);
	Readmemory(buffer, DumpData->sel0, len, MM_RESILENT);
	if (!OpenClipboard(NULL)) {
		return;
	}

	EmptyClipboard();

	if (action == 0) {    //取十六进制
		hClip = GlobalAlloc(GMEM_MOVEABLE, len * 2 + 1);  //申请剪切文本空间
		char *buff;
		buff = (char*)GlobalLock(hClip);
		for (int n = 0;n < len;n++) {					//十六进制转换
			sprintf(buff + n * 2, "%02X", buffer[n]);
		}
		GlobalUnlock(hClip);
		SetClipboardData(CF_TEXT, hClip);
		CloseClipboard();
		delete[] buffer;
		return;
	}
	if (action == 1) {	  //到字节集
		hClip = GlobalAlloc(GMEM_MOVEABLE, len * 4 + 1);  //申请剪切文本空间
		char *buff = (char*)GlobalLock(hClip);
		memset(buff, 0, len * 4 + 1);
		string byteset;//字节集
		byteset.append("{");
		for (int n = 0;n < len;n++) {					//十六进制转换
			sprintf(buff, "%d,", buffer[n]);
			byteset.append(buff);
		}
		byteset[byteset.length() - 1] = '}';
		byteset.copy(buff, byteset.length());
		GlobalUnlock(hClip);
		SetClipboardData(CF_TEXT, hClip);
		CloseClipboard();   //不需要使用GlobalFree,因为内存已经由剪切板来托管
		delete[] buffer;
		return;
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

