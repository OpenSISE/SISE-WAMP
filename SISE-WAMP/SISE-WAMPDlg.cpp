
// SISE-WAMPDlg.cpp : ʵ���ļ�
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
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	afx_msg void OpenAbout();
	afx_msg void ChatAuthor();
// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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
	AfxMessageBox("Bison�����أ�û����ʲô��ϵ��ʽ��");
}
// CSISEWAMPDlg �Ի���



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


// CSISEWAMPDlg ��Ϣ�������

BOOL CSISEWAMPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//skinppLoadSkin(_T("AlphaOS.ssk"));
	skinppLoadSkin(_T("AquaOS.ssk"));
	//skinppLoadSkin(_T("Aura.ssk"));
	//skinppLoadSkin(_T("gold.ssk"));
	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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
	
	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
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
	
	nid.cbSize = sizeof(nid);     //��С��ֵ
	nid.hWnd = m_hWnd;           //������
	nid.uID = IDR_MAINFRAME;     //icon  ID
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;   //������ӵ�е�״̬
	nid.uCallbackMessage = UM_ICONNOTIFY;            //�ص���Ϣ
	nid.hIcon = m_hIcon;                            //icon ����
	CString str = "SISE-WAMP 1.0";       //������ʾ
	lstrcpyn(nid.szTip, (LPCSTR)str, sizeof(nid.szTip) / sizeof(nid.szTip[0]));
	Shell_NotifyIcon(NIM_ADD, &nid);   //��ʾ����

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSISEWAMPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
		
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void CSISEWAMPDlg::OnSize(UINT nType, int cx, int cy)
{
	if (m_wndStatusBar.m_hWnd != NULL){    //���Ի����С�ı�ʱ ״̬����СҲ��֮�ı�
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	for (int i = 0, j = Index_Sign+1; j < Index_First; j++) aVersion[i++] = StrVersion.GetAt(j);        //��Apache�汾

	Index_Sign = StrVersion.ReverseFind('=');
	Index_First = StrVersion.Find('\r\n', Index_First+2);
	for (int i = 0, j = Index_Sign + 1; j < Index_First; j++) mVersion[i++] = StrVersion.GetAt(j);       //��Mysql�汾

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
	//WorkPath += "\\bin\\httpd.exe -k uninstall";      //�ȳ���ж������һ�������󶨵�apache
	//Sleep(10);
	//StartEXE(WorkPath);
}

void CSISEWAMPDlg::OnBnClickedButton_START()
{
	OnBnClickedButton_Stop();  //�ȹرշ���������
	m_MsgList.ResetContent();
	SetEnvironmentPath();
	Sleep(100);
	CString WorkPath;
	WorkPath = EnvPath;
	WorkPath.Replace("\\", "\\\\");
	StartMysql(WorkPath + "\\" + MysqlVersion);
	StartHttpd(WorkPath + "\\" + ApacheVersion);
	int CheckTimes=0;
	BOOL MysqlStarted = FALSE, ApacheStarted = FALSE;

	Sleep(100);

	while (true)
	{
		ApacheStarted=ProcessIsRunning("httpd.exe");
		MysqlStarted=ProcessIsRunning("mysqld-nt.exe");
		if (ApacheStarted && MysqlStarted)
		{
			ShowMsg("Apache�����ɹ�!!!");
			ShowMsg("Mysql�����ɹ�!!!");
			break;
		}	
		else if (CheckTimes>=5)
		{
			if (ApacheStarted){ ShowMsg("Apache�����ɹ�...."); ShowMsg("Mysql���ó������⣬����!"); break; }
			if (MysqlStarted){ ShowMsg("Mysql�����ɹ�...."); ShowMsg("Apache���ó������⣬����!"); break; }
		}
		CheckTimes++;
		Sleep(1000);
	}
	ShowRunTime();
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
}


void CSISEWAMPDlg::PreventChangeCaption()    //��ֹ�����⵳�ı���.....
{
	CaptionThread *newCaptionObject = new CaptionThread;
	pThread = new CWinThread();
	pThread->m_bAutoDelete = false;
	pThread = AfxBeginThread(MyThreadProc, newCaptionObject);
	if (pThread ==NULL)
	{
		AfxMessageBox("���������....��������....");
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
		AfxMessageBox("ʱ���߳�����ʧ��...");
	}

}


LRESULT CSISEWAMPDlg::UpdateRunTime(WPARAM wParam, LPARAM lParam)
{
	m_wndStatusBar.SetPaneText(0, (LPSTR)wParam);
	return 1;
}


void CSISEWAMPDlg::OnBnClickedButton_Stop()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	DWORD exitCode;
	CString info;
	
	if (GetExitCodeThread(runThread->m_hThread, &exitCode)) runThread->SuspendThread();
	
	m_wndStatusBar.SetPaneText(0,"�ȴ�����....");
	m_MsgList.ResetContent();
	CString Command=EnvPath;
	Command.Replace("\\", "\\\\");
	Sleep(5);
	Command += "\\";
	Command += ApacheVersion;
	ExecDosCmd((Command + "\\bin\\httpd.exe -k stop").GetBuffer(Command.GetLength()));
	ExecDosCmd((Command + "\\bin\\httpd.exe -k uninstall").GetBuffer(Command.GetLength()));
	ExecDosCmd("net stop mysql");
	
	ShowMsg("Apache�ѹر�....");
	ShowMsg("Mysql�ѹر�....");
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
		this->ShowMsg(szAppName + "�ȶ�������....");
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
	NowTime=mTime.Format("����ʱ��: %Y-%m-%d %H:%M:%S");
	ShowMsg(NowTime);
	
}

static UINT indicators[] =
{
	IDR_STATUSBAR_STRING
};

// // ״̬��
void CSISEWAMPDlg::CreatStatusBar()
{
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators) / sizeof(UINT)))                    //����״̬���������ַ���Դ��ID
	{
		TRACE0("Failed to create status bar\n");
		return;      // fail to create
	}
	CRect rc;
	::GetWindowRect(m_wndStatusBar.m_hWnd, rc);
	m_wndStatusBar.MoveWindow(rc);                              //�ƶ�״̬����ָ��λ��

}

void CSISEWAMPDlg::SetApache()
{
	CMyIni* My =new CMyIni;
	CString WorkPath = My->GetFilePath();
	WorkPath.Replace("\\\\", "\\");
	delete My;
	WorkPath += "tools\\vi.exe ";
	WorkPath += EnvPath;
	WorkPath += "\\";
	WorkPath += ApacheVersion;
	WorkPath += "\\conf\\httpd.conf";
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

		AfxMessageBox("��ʧ��!��ر�360����å�������������б�");
		//AfxMessageBox(Command);
		HANDLE hProcess = GetCurrentProcess();//get current process

		TerminateProcess(hProcess, 0);         //close process
	}
}

void CSISEWAMPDlg::OpenUrl(CString Url)
{

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ShellExecute(0, "open", Url.GetBuffer(), "", NULL, SW_SHOW);
}



void CSISEWAMPDlg::OnBnClickedButtonLOCAL()
{
	OpenUrl("http://127.0.0.1");
}


void CSISEWAMPDlg::OnBnClickedButtonCode()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	OpenUrl("https://github.com/OpenSISE/SISE-WAMP-tools");
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
		Content = "ʹ�÷���:��ѹ�����ļ���D�̸�Ŀ¼����֤·��ΪD:/SISE-WAMP1.1 \r\n\
Ȼ������SISE-WAMP.exe���ٵ�һ���������а�װɱ������Ļ���ʾ��װ���������ɣ������ܾ��˵ģ���һ���رպ���һ��������\r\n\
һ�н���ѧϰʹ�ã��������ڷǷ���;����BUG�ɷ�ӳ������:Bison \r\n\
1.��ǰĬ��һ������ΪApache2.4+php5.6+Mysql5.1\r\n\
2.��ǰ��վĿ¼ΪD:/SISE-WAMP1.0/www \r\n\
3.mysql���ݿ�Ĭ��root�ʺ�����Ϊ123456 \r\n\
��ӭ��ҷ�ӳBUG������\r\n\
������  https://github.com/OpenSISE/SISE-WAMP-tools  ";
		file.Write(Content.GetBuffer(), Content.GetLength());
		file.Close();
	}
	

	
}

void CSISEWAMPDlg::IsAlreadyRunning()
{

	HANDLE hMutex = CreateMutex(NULL, TRUE, AfxGetAppName());
	if (GetLastError() == ERROR_ALREADY_EXISTS)

	{
		AfxMessageBox("�ó���ֻ������һ��");
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
		this->ShowMsg("Apache�ȶ�������....");
		this->ShowMsg("Mysql�ȶ�������....");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	}
	else if (ApacheIsStarted && !MysqlIsStarted)
	{
		this->ShowMsg("Apache�ȶ�������....");
		this->ShowMsg("Mysqlû������!");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	}
	else if (!ApacheIsStarted && MysqlIsStarted)
	{
		this->ShowMsg("Apacheû������!");
		this->ShowMsg("Mysql�ȶ�������....");
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
	// TODO:  �ڴ���������������
	ShowWindow(SW_SHOW);
}


void CSISEWAMPDlg::OnNotifyExit()
{
	// TODO:  �ڴ���������������
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
	Shell_NotifyIcon(NIM_DELETE, &nid); //��������ɾ��ͼ�� 
	UpdateWindow();
	UpdateData();
	skinppExitSkin();
	exit(0);
	return 1;
}