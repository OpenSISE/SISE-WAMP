
// SISE-WAMP.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号



// CSISEWAMPApp: 
// 有关此类的实现，请参阅 SISE-WAMP.cpp
//
UINT MyThreadProc(LPVOID pParam);
BOOL ExecDosCmd(char * command);
BOOL StartMysql(CString);
BOOL StartHttpd(CString);

UINT RunTimeThread(LPVOID pParam);


class CSISEWAMPApp : public CWinApp
{
public:
	CSISEWAMPApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CSISEWAMPApp theApp;



	
