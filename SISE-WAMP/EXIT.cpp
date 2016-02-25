#include "stdafx.h"
#include "EXIT.h"
#include "SISE-WAMP.h"
#include "afxdialogex.h"



CEXIT::CEXIT() : CDialogEx(CEXIT::IDD)
{

}

void CEXIT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEXIT, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SURE, &CEXIT::OnBnClickedSure)
END_MESSAGE_MAP()




void CEXIT::OnBnClickedSure()
{
	// TODO:  在此添加控件通知处理程序代码
	UINT Action[] = { IDC_RADIO_MIN, IDC_RADIO_EXIT };
	CButton *pBtn = NULL;
		pBtn = (CButton*)GetDlgItem(Action[0]);
		if (pBtn->GetCheck()==TRUE)
		{
			HWND hwnd = ::FindWindow("SISE-WAMP", NULL);
			 ::PostMessage(hwnd, WM_HIDE_WINDOWS, NULL, NULL);
			 PostMessage(WM_CLOSE);
			
		}
		pBtn = (CButton*)GetDlgItem(Action[1]);
		if (pBtn->GetCheck() == TRUE)
		{
			if (IDYES == MessageBox("你确定退出SISE-WAMP吗？", "温馨提示", MB_YESNO))
			{
				HWND hwnd = ::FindWindow("SISE-WAMP", NULL);
				::PostMessage(hwnd, WM_DELETE_NOTIFY, NULL, NULL);
			}
		}
	
}
