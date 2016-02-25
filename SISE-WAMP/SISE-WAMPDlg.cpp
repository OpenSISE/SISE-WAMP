
// SISE-WAMPDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SISE-WAMP.h"
#include "SISE-WAMPDlg.h"
#include "afxdialogex.h"
#include "CaptionThread.h"
#include "MyIni.h"
#include "tlhelp32.h"
#include "EXIT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern char ApacheVersion[32] = "Apache24";
extern char MysqlVersion[32] = "MySQL5.1";
char* EnvPath;
CMyIni* Ini = new CMyIni();
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	afx_msg void OpenAbout();
	afx_msg void ChatAuthor();
// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDB_BUTTON_CHAT, &CAboutDlg::ChatAuthor)
END_MESSAGE_MAP()

void CAboutDlg::OpenAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CAboutDlg::ChatAuthor()
{
	AfxMessageBox("Bison很神秘，没留下什么联系方式！");
}
// CSISEWAMPDlg 对话框



CSISEWAMPDlg::CSISEWAMPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSISEWAMPDlg::IDD, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_LOGO);
}

void CSISEWAMPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STATUS, m_MsgList);

}

BEGIN_MESSAGE_MAP(CSISEWAMPDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_ABOUT_BS, &CAboutDlg::OpenAbout)
	ON_BN_CLICKED(IDC_BUTTON_START, &CSISEWAMPDlg::OnBnClickedButton_START)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CSISEWAMPDlg::OnBnClickedButton_Stop)
	ON_BN_CLICKED(ID_APACHE_SET,&CSISEWAMPDlg::SetApache)
	ON_BN_CLICKED(ID_PHP_SET, &CSISEWAMPDlg::SetPHP)
	ON_BN_CLICKED(ID_MYSQL_SET, &CSISEWAMPDlg::SetMysql)
	ON_BN_CLICKED(IDC_BUTTON_LOCAL, &CSISEWAMPDlg::OnBnClickedButtonLOCAL)
	ON_BN_CLICKED(IDC_BUTTON_CODE, &CSISEWAMPDlg::OnBnClickedButtonCode)
	ON_BN_CLICKED(IDC_SISE_WAMP, &CSISEWAMPDlg::OpenSetting)
	ON_BN_CLICKED(ID_USAGE,&CSISEWAMPDlg::OpenUsage)
	ON_MESSAGE(WM_UPDATE_RUN_TIME, UpdateRunTime)
	ON_MESSAGE(UM_ICONNOTIFY, (LRESULT(__thiscall CWnd::*)(WPARAM, LPARAM))OnIconNotify)
	ON_COMMAND(ID_NOTIFY_SHOW, &CSISEWAMPDlg::OnNotifyShow)
	ON_COMMAND(ID_NOTIFY_EXIT, &CSISEWAMPDlg::OnNotifyExit)
	ON_MESSAGE(WM_DELETE_NOTIFY,DeleteNotify)
	ON_MESSAGE(WM_HIDE_WINDOWS,HideWindows)
END_MESSAGE_MAP()


// CSISEWAMPDlg 消息处理程序

BOOL CSISEWAMPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//skinppLoadSkin(_T("AlphaOS.ssk"));
	skinppLoadSkin(_T("AquaOS.ssk"));
	//skinppLoadSkin(_T("Aura.ssk"));
	//skinppLoadSkin(_T("gold.ssk"));
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	CreatStatusBar();
	IsAlreadyRunning();
	

	Sleep(100);
	SetEnvironmentPath();
	PreventChangeCaption();
	Sleep(10);
	CRect rect;
	GetWindowRect(&rect);
	rect.bottom += 10;
	MoveWindow(rect);
	SetEnvironmentPath();
	CreateUsage();
	ShowWelcomeInfo();
	
	nid.cbSize = sizeof(nid);     //大小赋值
	nid.hWnd = m_hWnd;           //父窗口
	nid.uID = IDR_MAINFRAME;     //icon  ID
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;   //托盘所拥有的状态
	nid.uCallbackMessage = UM_ICONNOTIFY;            //回调消息
	nid.hIcon = m_hIcon;                            //icon 变量
	CString str = "SISE-WAMP 1.0";       //气泡提示
	lstrcpyn(nid.szTip, (LPCSTR)str, sizeof(nid.szTip) / sizeof(nid.szTip[0]));
	Shell_NotifyIcon(NIM_ADD, &nid);   //显示托盘

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSISEWAMPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
		
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSISEWAMPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
		
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void CSISEWAMPDlg::OnSize(UINT nType, int cx, int cy)
{
	if (m_wndStatusBar.m_hWnd != NULL){    //当对话框大小改变时 状态条大小也随之改变
		CRect rc;
		rc.top = cy - 25;
		rc.left = 0;
		rc.right = cx;
		rc.bottom = cy;
		m_wndStatusBar.MoveWindow(rc);
		m_wndStatusBar.SetPaneInfo(0, m_wndStatusBar.GetItemID(0), SBPS_POPOUT, cx - 1);
	}

	switch (nType)
	{

	case SIZE_MINIMIZED:
		ShowWindow(SW_HIDE);
		break;
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSISEWAMPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSISEWAMPDlg::SetEnvironmentPath()
{
	int Index_First,Index_Sign;
	char aVersion[32] = { 0 }, mVersion[32] = { 0 };
	runThread = new CWinThread();
	EnvPath = Ini->ReadIni();
	Sleep(10);
	CString StrVersion = Ini->ReadVersionIni();

	Index_Sign = StrVersion.Find('=');
	Index_First = StrVersion.Find('\r\n');
	for (int i = 0, j = Index_Sign+1; j < Index_First; j++) aVersion[i++] = StrVersion.GetAt(j);        //读Apache版本

	Index_Sign = StrVersion.ReverseFind('=');
	Index_First = StrVersion.Find('\r\n', Index_First+2);
	for (int i = 0, j = Index_Sign + 1; j < Index_First; j++) mVersion[i++] = StrVersion.GetAt(j);       //读Mysql版本

	if (aVersion && mVersion)
	{
		memset(ApacheVersion, 0, 32);
		memcpy(ApacheVersion, aVersion, sizeof(aVersion));
		memset(MysqlVersion, 0, 32);
		memcpy(MysqlVersion, mVersion, sizeof(mVersion));
	}
	//delete Ini;
	//delete EnvPath;
	//CString WorkPath;
	//WorkPath += EnvPath;
	//WorkPath += "\\";
	//WorkPath += ApacheVersion;
	//WorkPath += "\\bin\\httpd.exe -k uninstall";      //先尝试卸载其他一键环境绑定的apache
	//Sleep(10);
	//StartEXE(WorkPath);
}

void CSISEWAMPDlg::OnBnClickedButton_START()
{
	OnBnClickedButton_Stop();  //先关闭服务，再启动
	m_MsgList.ResetContent();
	SetEnvironmentPath();
	Sleep(100);
	CString WorkPath;
	WorkPath = EnvPath;
	WorkPath.Replace("\\", "\\\\");
	StartMysql(WorkPath + "\\" + ApacheVersion);
	StartHttpd(WorkPath + "\\" + MysqlVersion);
	int CheckTimes=0;
	BOOL MysqlStarted = FALSE, ApacheStarted = FALSE;

	Sleep(100);

	while (true)
	{
		ApacheStarted=ProcessIsRunning("httpd.exe");
		MysqlStarted=ProcessIsRunning("mysqld-nt.exe");
		if (ApacheStarted && MysqlStarted)
		{
			ShowMsg("Apache启动成功!!!");
			ShowMsg("Mysql启动成功!!!");
			break;
		}	
		else if (CheckTimes>=5)
		{
			if (ApacheStarted){ ShowMsg("Apache启动成功...."); ShowMsg("Mysql配置出现问题，请检查!"); break; }
			if (MysqlStarted){ ShowMsg("Mysql启动成功...."); ShowMsg("Apache配置出现问题，请检查!"); break; }
		}
		CheckTimes++;
		Sleep(1000);
	}
	ShowRunTime();
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
}


void CSISEWAMPDlg::PreventChangeCaption()    //防止被标题党改标题.....
{
	CaptionThread *newCaptionObject = new CaptionThread;
	pThread = new CWinThread();
	pThread->m_bAutoDelete = false;
	pThread = AfxBeginThread(MyThreadProc, newCaptionObject);
	if (pThread ==NULL)
	{
		AfxMessageBox("程序出错了....正在重启....");
		exit(0);
	}

}

void CSISEWAMPDlg::ShowRunTime()
{
	//CWorkThread *newObject = new CWorkThread;
	HWND UI_hwnd = this->GetSafeHwnd();
	runThread->m_bAutoDelete = FALSE;
	runThread = AfxBeginThread(RunTimeThread, UI_hwnd);
	if (runThread == NULL)
	{
		AfxMessageBox("时钟线程启动失败...");
	}

}


LRESULT CSISEWAMPDlg::UpdateRunTime(WPARAM wParam, LPARAM lParam)
{
	m_wndStatusBar.SetPaneText(0, (LPSTR)wParam);
	return 1;
}


void CSISEWAMPDlg::OnBnClickedButton_Stop()
{
	// TODO:  在此添加控件通知处理程序代码
	DWORD exitCode;
	CString info;
	
	if (GetExitCodeThread(runThread->m_hThread, &exitCode)) runThread->SuspendThread();
	
	m_wndStatusBar.SetPaneText(0,"等待运行....");
	m_MsgList.ResetContent();
	CString Command=EnvPath;
	Command.Replace("\\", "\\\\");
	Sleep(5);
	Command += "\\";
	Command += ApacheVersion;
	ExecDosCmd((Command + "\\bin\\httpd.exe -k stop").GetBuffer(Command.GetLength()));
	ExecDosCmd((Command + "\\bin\\httpd.exe -k uninstall").GetBuffer(Command.GetLength()));
	ExecDosCmd("taskkill /f /im mysqld-nt.exe");
	ExecDosCmd("taskkill /f /im httpd.exe");
	
	ShowMsg("Apache已关闭....");
	ShowMsg("Mysql已关闭....");
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);

}

BOOL CSISEWAMPDlg::ProcessIsRunning(CString szAppName)
{
	BOOL pRunning = FALSE;
	HANDLE hProcess;
	PROCESSENTRY32 pe;          //PROCESSENTRY32
	
	hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcess == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	memset(&pe, 0, sizeof(pe));

	pe.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hProcess, &pe))
	{
		do
		{
			if (szAppName.CompareNoCase(pe.szExeFile) == 0)
			{
				
				pRunning = TRUE; 
				break;
			}

		}
		while (Process32Next(hProcess,&pe));
	}
	CloseHandle(hProcess);
	memset(&pe, 0, sizeof(pe));
	/*if (pRunning)
	{
		this->ShowMsg(szAppName + "稳定运行中....");
	}*/
	return pRunning;
}

void CSISEWAMPDlg::ShowMsg(CString Message)
{
	m_MsgList.InsertString(0,Message);
}

void CSISEWAMPDlg::ShowWelcomeInfo()
{
	CTime mTime = CTime::GetCurrentTime();
	CString NowTime;
	NowTime=mTime.Format("启动时间: %Y-%m-%d %H:%M:%S");
	ShowMsg(NowTime);
	
}

static UINT indicators[] =
{
	IDR_STATUSBAR_STRING
};

// // 状态条
void CSISEWAMPDlg::CreatStatusBar()
{
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators) / sizeof(UINT)))                    //创建状态条并设置字符资源的ID
	{
		TRACE0("Failed to create status bar\n");
		return;      // fail to create
	}
	CRect rc;
	::GetWindowRect(m_wndStatusBar.m_hWnd, rc);
	m_wndStatusBar.MoveWindow(rc);                              //移动状态条到指定位置

}

void CSISEWAMPDlg::SetApache()
{
	CMyIni* My =new CMyIni;
	CString WorkPath = My->GetFilePath();
	WorkPath.Replace("\\\\", "\\");
	delete My;
	WorkPath += "tools\\vi.exe ";
	WorkPath += EnvPath;
	WorkPath += "\\Apache24\\conf\\httpd.conf";
	this->StartEXE(WorkPath);

}

void CSISEWAMPDlg::SetPHP()
{
	CMyIni* My = new CMyIni;
	CString WorkPath = My->GetFilePath();
	WorkPath.Replace("\\\\", "\\");
	delete My;
	WorkPath += "tools\\vi.exe ";
	WorkPath += EnvPath;
	WorkPath += "\\php\\php.ini";
	this->StartEXE(WorkPath);

}

void CSISEWAMPDlg::SetMysql()
{
	CMyIni* My = new CMyIni;
	CString WorkPath = My->GetFilePath();
	delete My;
	WorkPath += "tools\\vi.exe ";
	WorkPath += EnvPath;
	WorkPath += "\\";
	WorkPath += MysqlVersion;
	WorkPath += "\\my.ini";
	this->StartEXE(WorkPath);

}

void CSISEWAMPDlg::StartEXE(CString Command)
{
	STARTUPINFO si;

	PROCESS_INFORMATION pi;

	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));

	si.cb = sizeof(si);
	if(CreateProcess(NULL, Command.GetBuffer(Command.GetLength()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	else

	{

		AfxMessageBox("打开失败!请关闭360等流氓软件或加入信任列表！");
		//AfxMessageBox(Command);
		HANDLE hProcess = GetCurrentProcess();//get current process

		TerminateProcess(hProcess, 0);         //close process
	}
}

void CSISEWAMPDlg::OpenUrl(CString Url)
{

	// TODO:  在此添加控件通知处理程序代码
	ShellExecute(0, "open", Url.GetBuffer(), "", NULL, SW_SHOW);
}



void CSISEWAMPDlg::OnBnClickedButtonLOCAL()
{
	OpenUrl("http://127.0.0.1");
}


void CSISEWAMPDlg::OnBnClickedButtonCode()
{
	// TODO:  在此添加控件通知处理程序代码
	OpenUrl("https://github.com/OpenSISE");
}

void CSISEWAMPDlg::OpenUsage()
{
	CString WorkPath;
	WorkPath += "notepad.exe ";
	WorkPath += EnvPath;
	WorkPath += "\\Readme.txt";
	this->StartEXE(WorkPath);
}

void CSISEWAMPDlg::OpenSetting()
{
	CString WorkPath;
	WorkPath += "notepad.exe ";
	WorkPath += EnvPath;
	WorkPath += "\\SISE_Config.ini";
	this->StartEXE(WorkPath);
}

void CSISEWAMPDlg::CreateUsage()
{
	CString WorkPath;
	WorkPath += EnvPath;
	WorkPath += "\\Readme.txt";
	CFile file;
	if(file.Open(WorkPath, CFile::modeReadWrite)!=0)     //CFile::modeReadWrite //CFile::modeCreate |CFile::modeNoTruncate
	{
		//AfxMessageBox(WorkPath+"    false!");
		file.Close();
	}
	else
	{
		file.Open(WorkPath, CFile::modeCreate | CFile::modeWrite);
		CString Content;
		Content = "使用方法:解压所有文件到D盘根目录，保证路径为D:/SISE-WAMP1.0 \r\n\
然后运行SISE-WAMP.exe，再点一键启动，有安装杀毒软件的会提示安装服务，允许即可，若点错拒绝了的，可一键关闭后再一键启动！\r\n\
一切仅供学习使用，请勿用于非法用途。有BUG可反映给作者:Bison \r\n\
1.当前默认一键环境为Apache2.4+php5.6+Mysql5.1\r\n\
2.当前网站目录为D:/SISE-WAMP1.0/www \r\n\
3.mysql数据库默认root帐号密码为123456 \r\n\
欢迎大家反映BUG！！！\r\n\
代码在  https://github.com/OpenSISE  ";
		file.Write(Content.GetBuffer(), Content.GetLength());
		file.Close();
	}
	

	
}

void CSISEWAMPDlg::IsAlreadyRunning()
{

	HANDLE hMutex = CreateMutex(NULL, TRUE, AfxGetAppName());
	if (GetLastError() == ERROR_ALREADY_EXISTS)

	{
		AfxMessageBox("该程序只能运行一个");
		exit(0);

	}
	if (hMutex)

	{
		::ReleaseMutex(hMutex);

	}
   
	BOOL MysqlIsStarted = FALSE, ApacheIsStarted = FALSE;
	Sleep(1);
	ApacheIsStarted = ProcessIsRunning("httpd.exe");
	MysqlIsStarted = ProcessIsRunning("mysqld-nt.exe");
	if (ApacheIsStarted && MysqlIsStarted)
	{
		this->ShowMsg("Apache稳定运行中....");
		this->ShowMsg("Mysql稳定运行中....");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	}
	else if (ApacheIsStarted && !MysqlIsStarted)
	{
		this->ShowMsg("Apache稳定运行中....");
		this->ShowMsg("Mysql没有启动!");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	}
	else if (!ApacheIsStarted && MysqlIsStarted)
	{
		this->ShowMsg("Apache没有启动!");
		this->ShowMsg("Mysql稳定运行中....");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	}

}

void CSISEWAMPDlg::OnIconNotify(WPARAM wParam, LPARAM lParam)
{
	
	switch ((UINT)lParam)
	{
	case WM_LBUTTONDOWN: // click or dbclick left button on icon
	case WM_LBUTTONDBLCLK: // should show desktop
		if (!IsWindowVisible())
		{
			this->ShowWindow(SW_SHOWNORMAL);
			this->SetActiveWindow();
			this->SetForegroundWindow();
			this->BringWindowToTop();

		}
		else
			ShowWindow(SW_HIDE);
		break;
	case WM_RBUTTONDOWN: // click right button, show menu
		CMenu menu;
		menu.LoadMenu(IDR_MENU_NOTIFY);
		CPoint point;
		GetCursorPos(&point);
		SetForegroundWindow();
		menu.GetSubMenu(0)->TrackPopupMenu(
			TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
			point.x, point.y, this, NULL);
		PostMessage(WM_USER, 0, 0);
		break;
	}
}


void CSISEWAMPDlg::OnNotifyShow()
{
	// TODO:  在此添加命令处理程序代码
	ShowWindow(SW_SHOW);
}


void CSISEWAMPDlg::OnNotifyExit()
{
	// TODO:  在此添加命令处理程序代码
	PostMessage(WM_CLOSE);
}

void CSISEWAMPDlg::OnClose()
{
	///if (ID)
	CEXIT CExitDlg;
	CExitDlg.DoModal();
}

LRESULT CSISEWAMPDlg::HideWindows(WPARAM wParam,LPARAM lParam)
{
	//WM_HIDE_WINDOWS
	ShowWindow(SW_HIDE);
	return 1;
}

LRESULT CSISEWAMPDlg::DeleteNotify(WPARAM wParam, LPARAM lParam)
{
	Shell_NotifyIcon(NIM_DELETE, &nid); //在托盘区删除图标 
	UpdateWindow();
	UpdateData();
	skinppExitSkin();
	exit(0);
	return 1;
}