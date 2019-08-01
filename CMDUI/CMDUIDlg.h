/*
Tiny CMD - by Michael Haephrati
(c)2019, Michael Haephrati and Secured Globe, Inc.
https://www.securedglobe.net

How to run CMD commands from a c++ based Dialog applicationb
Article first published at CodeProject.
https://www.codeproject.com/Articles/5163375/Tiny-CMD

*/

#pragma once
#include "afxwin.h"
#include "EasySize.h"

#define FONT_NAME		_T("Courier")
#define COLOR_BLACK		RGB(0, 0, 0)
#define COLOR_YELLOW	RGB(204, 204, 0)

// CCMDUIDlg dialog
class CCMDUIDlg : public CDialogEx
{
	DECLARE_EASYSIZE
// Construction
public:
	CCMDUIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CMDUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	CBrush *m_brush;
	CFont m_Font;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit CommandTyped;
	CEdit Result;
	CEdit m_ProjectFileName;
	CListBox m_ProjectFiles;
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void CCMDUIDlg::CycleResultWaitText();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
