
// SISE-WAMP.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "SISE-WAMP.h"
#include "SISE-WAMPDlg.h"
#include "CaptionThread.h"
#include "MyIni.h"

#ifdef _DEBUG
#define new DEBUG_NEW

#endif



// CSISEWAMPApp

BEGIN_MESSAGE_MAP(CSISEWAMPApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSISEWAMPApp ����

CSISEWAMPApp::CSISEWAMPApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSISEWAMPApp ����

CSISEWAMPApp theApp;


// CSISEWAMPApp ��ʼ��

BOOL CSISEWAMPApp::InitInstance()
{

	
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);


	WNDCLASS wndMyClass;
	::GetClassInfo(AfxGetInstanceHandle(), "#32770", &wndMyClass);
	wndMyClass.lpszClassName = "SISE-WAMP";
	AfxRegisterClass(&wndMyClass); //ע�ᴰ����
	CWinApp::InitInstance();





	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CSISEWAMPDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}



bool bThreadRunning = true;
UINT MyThreadProc(LPVOID pParam)
{
	Sleep(3000);

	CaptionThread* pObject = (CaptionThread*)pParam;
	if ((pObject == NULL) || (!pObject->IsKindOf(RUNTIME_CLASS(CaptionThread))))
	{
		AfxMessageBox("��������ʧ�ܡ�Prepare Exit Thread");
		return 1;
	}

	char CaptionName[64] = { 0 };
	char DefaultCaption[64] = "      SISE->һ��WAMP����     By:Bison               ";
	HWND hwnd = FindWindow("SISE-WAMP", NULL);
	::SetWindowText(hwnd, DefaultCaption);

	while (bThreadRunning)
	{
		memset(CaptionName, 0, 64);
		::GetWindowText(hwnd, CaptionName, 63);
		if (memcmp(CaptionName, DefaultCaption, 63) != 0)
		{
			::MessageBox(hwnd, CaptionName, " �޷��Ǹ�����.....", NULL);
			::SetWindowText(hwnd, DefaultCaption);
		}
		Sleep(3000);
	}
	return 0;
}

BOOL ExecDosCmd(char * command)
{
	SECURITY_ATTRIBUTES  sa;
	HANDLE hRead, hWrite;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = NULL;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		return FALSE;
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	if (!CreateProcess(NULL, command, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		CloseHandle(hWrite);
		CloseHandle(hRead);
		return FALSE;
	}
	CloseHandle(hWrite);
	char buffer[4096] = { 0 };
	DWORD bytesRead;
	while (true)
	{
		if (ReadFile(hRead, buffer, 4095, &bytesRead, NULL) == NULL)
			break;

		//MessageBox(NULL, buffer, "tips", NULL);   
	}
	CloseHandle(hRead);
	return TRUE;
}

BOOL StartMysql(CString WorkPath)
{
	//WorkPath += "\\";
	//WorkPath += ApacheVersion;
	ExecDosCmd((WorkPath + "\\bin\\httpd.exe -k install").GetBuffer(WorkPath.GetLength()));
	Sleep(500);
	ExecDosCmd((WorkPath + "\\bin\\httpd.exe -k start").GetBuffer(WorkPath.GetLength()));
	return TRUE;
}

BOOL StartHttpd(CString WorkPath)
{
	ExecDosCmd((WorkPath + "\\bin\\mysqld-nt.exe ").GetBuffer(WorkPath.GetLength()));

	return TRUE;
}

UINT RunTimeThread(LPVOID pParam)
{
	CTime StartTime = CTime::GetCurrentTime();
	CString runtime;
	while (true)
	{
		Sleep(1000);
		CTime now = CTime::GetCurrentTime();
		CTimeSpan num = now - StartTime;
		runtime.Format("����������: %dСʱ%d����%d��", num.GetHours(), num.GetMinutes(), num.GetSeconds());
		::PostMessage((HWND)pParam, WM_UPDATE_RUN_TIME, (WPARAM)runtime.GetBuffer(), 0);
	}

	return 0;;
}