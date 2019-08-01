/*
Tiny CMD - by Michael Haephrati
(c)2019, Michael Haephrati and Secured Globe, Inc.
https://www.securedglobe.net

How to run CMD commands from a c++ based Dialog applicationb
Article first published at CodeProject.
https://www.codeproject.com/Articles/5163375/Tiny-CMD

*/

#include "stdafx.h"
#include "CMDUI.h"
#include "CMDUIDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <vector>
#include <ctime>
#include "sg_cmd.h"
#include "EasySize.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int HistoryIndex = -1;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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
END_MESSAGE_MAP()


// CCMDUIDlg dialog



CCMDUIDlg::CCMDUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCMDUIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCMDUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMMAND, CommandTyped);
	DDX_Control(pDX, IDC_CMD_RESULT, Result);
}

BEGIN_MESSAGE_MAP(CCMDUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCMDUIDlg::OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CCMDUIDlg)
	EASYSIZE(IDC_CMD_RESULT, ES_BORDER, ES_BORDER,
		ES_BORDER, ES_BORDER, 0)
	EASYSIZE(IDC_COMMAND, ES_BORDER, ES_BORDER,
		ES_BORDER, ES_KEEPSIZE, 0)
	EASYSIZE(IDC_CMD_RESULT_TEXT, ES_BORDER, ES_BORDER,
		ES_BORDER, ES_KEEPSIZE, 0)
	EASYSIZE(IDC_COMMAND_TEXT, ES_BORDER, ES_BORDER,
		ES_BORDER, ES_KEEPSIZE, 0)
	EASYSIZE(IDOK, ES_KEEPSIZE, ES_KEEPSIZE,
		ES_BORDER, ES_BORDER, 0)
END_EASYSIZE_MAP

// CCMDUIDlg message handlers

BOOL CCMDUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	INIT_EASYSIZE;
	m_brush = new CBrush(COLOR_BLACK);

	// Creates a 140-point-Courier-font
	m_Font.CreatePointFont(140, FONT_NAME);

	// With a member variable associated to the static control
	CommandTyped.SetFont(&m_Font);

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCMDUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCMDUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCMDUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
#define TIMER_COMMAND_UPDATE	1000
#define TIMER_TIMEOUT			2000
#define UPDATE_MSECS			300
#define TIMEOUT_MSECS			30000
void CCMDUIDlg::OnBnClickedOk()
{
	CString CommandTypedText, ResultText, ResultTextLine;
	CommandTyped.GetWindowTextW(CommandTypedText);
	SetCommand(CommandTypedText);
	Result.SetWindowText(L"");
	SetTimer(TIMER_TIMEOUT, TIMEOUT_MSECS, NULL);
	SetTimer(TIMER_COMMAND_UPDATE, UPDATE_MSECS, NULL);
}

CStringArray Commands;

BOOL CCMDUIDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg)
	{
		if (pMsg->message == WM_KEYDOWN)
		{
			if (pMsg->wParam == VK_DOWN)
			{
				if (HistoryIndex == -1 && Commands.GetCount()>0)
					HistoryIndex = Commands.GetCount() - 1;
				if (HistoryIndex < Commands.GetCount()-1) HistoryIndex++;
				if (Commands.GetCount() > 0)
				{
					CommandTyped.SetWindowTextW(Commands[HistoryIndex]);
					CommandTyped.SetSel(Commands[HistoryIndex].GetLength(),Commands[HistoryIndex].GetLength());
				}
				return TRUE;
			}
			if (pMsg->wParam == VK_UP)
			{
				if (HistoryIndex == -1 && Commands.GetCount() > 0)
					HistoryIndex = Commands.GetCount() - 1;
				else
					if (HistoryIndex > 0) HistoryIndex--;
				if (Commands.GetCount() > 0)
				{
					CommandTyped.SetWindowTextW(Commands[HistoryIndex]);
					CommandTyped.SetSel(Commands[HistoryIndex].GetLength(), Commands[HistoryIndex].GetLength());
				}
				return TRUE;
			}
			if (pMsg->wParam == VK_RETURN)
			{
				OnBnClickedOk();
				return TRUE;
			}
		}
	}
	// Call the base class PreTranslateMessage. In this example,
	// CRhinoDialog is the base class to CMyModalDialog.
	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CCMDUIDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	CString wText;
	int ID = pWnd->GetDlgCtrlID();
	switch (nCtlColor)
	{

	case CTLCOLOR_EDIT:
		if(ID == IDC_CMD_RESULT || ID == IDC_COMMAND)
		{
			pDC->SetTextColor(COLOR_YELLOW);
			pDC->SetBkColor(COLOR_BLACK);
			return (HBRUSH)(m_brush->GetSafeHandle());
		}
		break;
	default:
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

}

void CCMDUIDlg::CycleResultWaitText()
{
	CString curVal;
	Result.GetWindowText(curVal);
	if (curVal.GetLength()>5)
	{
		Result.SetWindowText(L".");
	}
	else
	{
		Result.SetWindowText(curVal+L".");
	}

}
void CCMDUIDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (IsRunning)
	{
		CycleResultWaitText();
	}
	else
	{
		Result.SetWindowText(CommandResult);
		KillTimer(TIMER_COMMAND_UPDATE);
		KillTimer(TIMER_TIMEOUT);
		Commands.Add(Command);
		CommandTyped.SetWindowTextW((CString)L"");
	}

	if (nIDEvent == TIMER_TIMEOUT)
	{
		IsRunning = FALSE;
		Result.SetWindowText(L"Timeout");
		KillTimer(TIMER_COMMAND_UPDATE);
		KillTimer(TIMER_TIMEOUT);
		CommandTyped.SetWindowTextW((CString)L"");
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CCMDUIDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	UPDATE_EASYSIZE;
	// TODO: Add your message handler code here
}
