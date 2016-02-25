
// SISE-WAMPDlg.h : ͷ�ļ�
//



#pragma once


// CSISEWAMPDlg �Ի���
class CSISEWAMPDlg : public CDialogEx
{
// ����
public:
	CSISEWAMPDlg(CWnd* pParent = NULL);	// ��׼���캯��
	CWinThread* pThread;
	CWinThread* runThread;
	CListBox m_MsgList;

	
// �Ի�������
	enum { IDD = IDD_SISEWAMP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	NOTIFYICONDATA nid;
	CStatusBar  m_wndStatusBar;              //״̬��
	
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void CreatStatusBar();

	afx_msg void CreateUsage();
	afx_msg void OnBnClickedButton_START();
	afx_msg void PreventChangeCaption();
	afx_msg void ShowMsg(CString);
	afx_msg BOOL ProcessIsRunning(CString);
	afx_msg void OnBnClickedButton_Stop();
	afx_msg void ShowWelcomeInfo();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void SetApache();
	afx_msg void SetPHP();
	afx_msg void SetMysql();
	afx_msg void Help();
	afx_msg void SetEnvironmentPath();
	afx_msg void StartEXE(CString);
	afx_msg void IsAlreadyRunning();
	afx_msg void OpenUrl(CString);
	afx_msg void OnBnClickedButtonLOCAL();
	afx_msg void OnBnClickedButtonCode();
	afx_msg void OpenUsage();
	afx_msg void ShowRunTime();
	afx_msg void OnIconNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT UpdateRunTime(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnNotifyShow();
	afx_msg void OnNotifyExit();
	afx_msg LRESULT HideWindows(WPARAM, LPARAM);
	afx_msg LRESULT DeleteNotify(WPARAM, LPARAM);
	afx_msg void OpenSetting();
};
