// wincolDlg.h : header file
//

#if !defined(AFX_WINCOLDLG_H__0E99FEC2_3341_4AA6_89E5_3E90F9CC664B__INCLUDED_)
#define AFX_WINCOLDLG_H__0E99FEC2_3341_4AA6_89E5_3E90F9CC664B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWincolDlg dialog

class CWincolDlg : public CDialog
{
// Construction
public:
	CWincolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CWincolDlg)
	enum { IDD = IDD_WINCOL_DIALOG };
	CIPAddressCtrl	m_DestIPAddr;
	UINT	m_DestPort;
	UINT	m_LocalPort;
	CString	m_Recv;
	CString	m_Send;
	CString	m_SourceAddr;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWincolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
	SOCKET sock;
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CWincolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnInitbtn();
	afx_msg void OnSendbtn();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINCOLDLG_H__0E99FEC2_3341_4AA6_89E5_3E90F9CC664B__INCLUDED_)
