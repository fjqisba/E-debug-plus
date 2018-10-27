// E-Debug.h : E-Debug DLL 的主头文件
//
#include "stdafx.h"

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

extern char DIRECTORY[MAX_PATH];  //定义全局变量,插件目录

// CEDebugApp
// 有关此类实现的信息，请参阅 E-Debug.cpp
//

static HANDLE GethProcess() {           //返回所调试进程的句柄
	return (HANDLE)*(DWORD*)0x4D5A68;
}

class CEDebugApp : public CWinApp
{
public:
	CEDebugApp();

// 重写
public:
	virtual BOOL InitInstance();
	
	DECLARE_MESSAGE_MAP()
};
