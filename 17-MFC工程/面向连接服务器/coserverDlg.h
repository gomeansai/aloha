// coserverDlg.h : header file
//

#if !defined(AFX_COSERVERDLG_H__1D2AF6E2_D7ED_4106_BCE1_30E8F2A1285B__INCLUDED_)
#define AFX_COSERVERDLG_H__1D2AF6E2_D7ED_4106_BCE1_30E8F2A1285B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCoserverDlg dialog

class CCoserverDlg : public CDialog
{
// Construction
public:
	CCoserverDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCoserverDlg)
	enum { IDD = IDD_COSERVER_DIALOG };
	CButton	m_start;
	CString	m_ack;
	CString	m_recv;
	UINT	m_port;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoserverDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
	SOCKET main_sock;
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCoserverDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	afx_msg void OnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COSERVERDLG_H__1D2AF6E2_D7ED_4106_BCE1_30E8F2A1285B__INCLUDED_)
