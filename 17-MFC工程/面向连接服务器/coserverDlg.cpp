// coserverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "coserver.h"
#include "coserverDlg.h"

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
// CCoserverDlg dialog

CCoserverDlg::CCoserverDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCoserverDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCoserverDlg)
	m_ack = _T("");
	m_recv = _T("");
	m_port = 0;
	main_sock = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCoserverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCoserverDlg)
	DDX_Control(pDX, ID_START, m_start);
	DDX_Text(pDX, IDC_ACK, m_ack);
	DDX_Text(pDX, IDC_RECV, m_recv);
	DDX_Text(pDX, IDC_PORT, m_port);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCoserverDlg, CDialog)
	//{{AFX_MSG_MAP(CCoserverDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_START, OnStart)
	ON_BN_CLICKED(ID_STOP, OnStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoserverDlg message handlers

BOOL CCoserverDlg::OnInitDialog()
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

void CCoserverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCoserverDlg::OnPaint() 
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
HCURSOR CCoserverDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCoserverDlg::OnStart() 
{
	int retval;
	sockaddr_in server;
	WSAData wsa;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	WSAStartup(0x101,&wsa);

	if(main_sock != 0){
		MessageBox("server has been started!","server",MB_OK);
		return;
	}

	main_sock = socket(AF_INET,SOCK_STREAM,0);

	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server.sin_port = htons(m_port);

	retval = bind(main_sock,(sockaddr *)&server,sizeof(server));

	retval = listen(main_sock,5);

	retval = WSAAsyncSelect(main_sock,m_hWnd,UM_SOCK,FD_ACCEPT);
	m_start.EnableWindow(FALSE);
}

void CCoserverDlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	closesocket(main_sock);
	main_sock = 0;
	WSACleanup();
	m_start.EnableWindow(FALSE);
}

LRESULT CCoserverDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	SOCKET s,ns;
	int Event;
	sockaddr_in remote;
	int retval;
	char buf[128];
	int len;
	// TODO: Add your specialized code here and/or call the base class
	switch(message){
	case UM_SOCK:
		s = (SOCKET)LOWORD(wParam);
		Event = LOWORD(lParam);
		switch(Event){
		case FD_ACCEPT:
			len = sizeof(remote);
			ns = accept(s,(sockaddr*)&remote,&len);
			if(ns == SOCKET_ERROR){
				closesocket(s);
				main_sock = 0;
				break;
			}
			MessageBox("accept a connection","server",MB_OK);
			WSAAsyncSelect(ns,m_hWnd,UM_SOCK,FD_READ|FD_CLOSE);
		break;
		case FD_READ:
			UpdateData(TRUE);
			len = recv(s,buf,128,0);
			if(len <= 0){
				retval = WSAGetLastError();
				if( retval != WSAEWOULDBLOCK){
					closesocket(s);
					MessageBox("recv() failed!","server",MB_OK);
					break;
				}
				else
					break;
			}
			buf[len] = 0;
			m_recv = buf;
			UpdateData(FALSE);
			len = m_ack.GetLength();
			send(s,m_ack,len,0);
		break;
		case FD_CLOSE:
			closesocket(s);
		break;
		}
	break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
