// coclientDlg.h : header file
//

#if !defined(AFX_COCLIENTDLG_H__4B4621D1_48E7_4CEB_A588_DD46B932174E__INCLUDED_)
#define AFX_COCLIENTDLG_H__4B4621D1_48E7_4CEB_A588_DD46B932174E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCoclientDlg dialog

class CCoclientDlg : public CDialog
{
// Construction
public:
	SOCKET sock;
	CCoclientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCoclientDlg)
	enum { IDD = IDD_COCLIENT_DIALOG };
	CIPAddressCtrl	m_ipaddr;
	UINT	m_port;
	CString	m_sendata;
	CString	m_ack;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoclientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCoclientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConnect();
	afx_msg void OnDisc();
	afx_msg void OnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COCLIENTDLG_H__4B4621D1_48E7_4CEB_A588_DD46B932174E__INCLUDED_)
