
// SISE-WAMP.cpp : 定义应用程序的类行为。
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


// CSISEWAMPApp 构造

CSISEWAMPApp::CSISEWAMPApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSISEWAMPApp 对象

CSISEWAMPApp theApp;


// CSISEWAMPApp 初始化

BOOL CSISEWAMPApp::InitInstance()
{

	
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);


	WNDCLASS wndMyClass;
	::GetClassInfo(AfxGetInstanceHandle(), "#32770", &wndMyClass);
	wndMyClass.lpszClassName = "SISE-WAMP";
	AfxRegisterClass(&wndMyClass); //注册窗口类
	CWinApp::InitInstance();





	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO:  应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CSISEWAMPDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}



bool bThreadRunning = true;
UINT MyThreadProc(LPVOID pParam)
{
	Sleep(3000);

	CaptionThread* pObject = (CaptionThread*)pParam;
	if ((pObject == NULL) || (!pObject->IsKindOf(RUNTIME_CLASS(CaptionThread))))
	{
		AfxMessageBox("参数传入失败。Prepare Exit Thread");
		return 1;
	}

	char CaptionName[64] = { 0 };
	char DefaultCaption[64] = "      SISE->一键WAMP环境     By:Bison               ";
	HWND hwnd = FindWindow("SISE-WAMP", NULL);
	::SetWindowText(hwnd, DefaultCaption);

	while (bThreadRunning)
	{
		memset(CaptionName, 0, 64);
		::GetWindowText(hwnd, CaptionName, 63);
		if (memcmp(CaptionName, DefaultCaption, 63) != 0)
		{
			::MessageBox(hwnd, CaptionName, " 无非是个标题.....", NULL);
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
		runtime.Format("正在运行中: %d小时%d分钟%d秒", num.GetHours(), num.GetMinutes(), num.GetSeconds());
		::PostMessage((HWND)pParam, WM_UPDATE_RUN_TIME, (WPARAM)runtime.GetBuffer(), 0);
	}

	return 0;;
}