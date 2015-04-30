// coclientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "coclient.h"
#include "coclientDlg.h"
#include <afxcmn.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define UM_SOCK WM_USER + 100
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoclientDlg dialog

CCoclientDlg::CCoclientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCoclientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCoclientDlg)
	sock = 0;
	m_port = 0;
	m_sendata = _T("");
	m_ack = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCoclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCoclientDlg)
	DDX_Control(pDX, IDC_IPADDRESS, m_ipaddr);
	DDX_Text(pDX, IDC_PORT, m_port);
	DDX_Text(pDX, IDC_SENDATA, m_sendata);
	DDX_Text(pDX, IDC_ACK, m_ack);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCoclientDlg, CDialog)
	//{{AFX_MSG_MAP(CCoclientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_BN_CLICKED(IDC_DISC, OnDisc)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoclientDlg message handlers

BOOL CCoclientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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

void CCoclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCoclientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCoclientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

LRESULT CCoclientDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	int Event;
	int len,retval;
	char buf[128];
	switch(message){
	case UM_SOCK:
		Event = LOWORD(lParam);
		switch(Event){
		case FD_CONNECT:
			MessageBox("try to connect to server!","client",MB_OK);
			WSAAsyncSelect(sock,m_hWnd,UM_SOCK,FD_READ|FD_CLOSE);
		break;
		case FD_WRITE:
			MessageBox("connect to server!","client",MB_OK);
			WSAAsyncSelect(sock,m_hWnd,UM_SOCK,FD_READ|FD_CLOSE);
		break;
		case FD_READ:
			len = recv(sock,buf,128,0);
			if(len <= 0){
				retval = WSAGetLastError();
				if(retval != WSAEWOULDBLOCK){
					closesocket(sock);
					sock = 0;
				}
				break;
			}
			buf[len] = 0;
			m_ack = buf;
			UpdateData(FALSE);
		break;
		case FD_CLOSE:
			closesocket(sock);
		break;
		}
	break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CCoclientDlg::OnConnect() 
{
	sockaddr_in server;
	// TODO: Add your control notification handler code here
	if(sock == 0){

		//MessageBox("You should disconnect first","client",MB_OK);
		sock = socket(AF_INET,SOCK_STREAM,0);
	}
	UpdateData(TRUE);

	server.sin_family = AF_INET;
	m_ipaddr.GetAddress(server.sin_addr.S_un.S_un_b.s_b1,server.sin_addr.S_un.S_un_b.s_b2,server.sin_addr.S_un.S_un_b.s_b3,server.sin_addr.S_un.S_un_b.s_b4);
	server.sin_port = htons(m_port);
	
	WSAAsyncSelect(sock,m_hWnd,UM_SOCK,FD_CLOSE|FD_CONNECT);
	connect(sock,(sockaddr*)&server,sizeof(server));
}

void CCoclientDlg::OnDisc() 
{
	// TODO: Add your control notification handler code here
	closesocket(sock);
	sock = 0;
}

void CCoclientDlg::OnSend() 
{
	int len;
	if(sock == 0){
		MessageBox("You haven`t connect to server yet","client",MB_OK);
		return;
	}
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	len = m_sendata.GetLength();
	send(sock,m_sendata,len,0);
}
