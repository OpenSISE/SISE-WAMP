
// SISE-WAMP.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������



// CSISEWAMPApp: 
// �йش����ʵ�֣������ SISE-WAMP.cpp
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

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSISEWAMPApp theApp;



	
