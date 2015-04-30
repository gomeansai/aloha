// wincolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wincol.h"
#include "wincolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define UM_SOCK WM_USER+100
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
// CWincolDlg dialog

CWincolDlg::CWincolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWincolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWincolDlg)
	m_DestPort = 0;
	m_LocalPort = 0;
	m_Recv = _T("");
	m_Send = _T("");
	m_SourceAddr = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWincolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWincolDlg)
	DDX_Control(pDX, IDC_DESTIPADDRESS, m_DestIPAddr);
	DDX_Text(pDX, IDC_DESTEDIT, m_DestPort);
	DDX_Text(pDX, IDC_LOCEDIT, m_LocalPort);
	DDX_Text(pDX, IDC_RECVEDIT, m_Recv);
	DDX_Text(pDX, IDC_SENDEDIT, m_Send);
	DDX_Text(pDX, IDC_SRCADDREDIT, m_SourceAddr);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWincolDlg, CDialog)
	//{{AFX_MSG_MAP(CWincolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INITBTN, OnInitbtn)
	ON_BN_CLICKED(IDC_SENDBTN, OnSendbtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWincolDlg message handlers

BOOL CWincolDlg::OnInitDialog()
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

void CWincolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CWincolDlg::OnPaint() 
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
HCURSOR CWincolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

LRESULT CWincolDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	SOCKET s;
	int wmEvent;
	int len,remote_len;
	struct sockaddr_in remote;
	char buf[48];
	char buffer[1024];
	unsigned long retval;

	switch(message){
	case UM_SOCK:
		s = (SOCKET)wParam;
		wmEvent = LOWORD(lParam);
		switch(wmEvent){
		case FD_READ:
			len = sizeof(buffer);
			remote_len = sizeof(remote);
			//len = recv(sock,buffer,len,0);
			len = recvfrom(sock,buffer,len,0,(sockaddr*)&remote,&remote_len);
			if(len == SOCKET_ERROR){
				retval = WSAGetLastError();
				wsprintf(buf,"recvfrom() failed %u\n",retval);
				MessageBox(buf,"Receiving Error",MB_OK);
				break;
			}else if(len == 0)
				break;
			m_SourceAddr.Empty();
			m_SourceAddr += inet_ntoa(remote.sin_addr);
			m_SourceAddr += ":";
			m_SourceAddr += itoa(ntohs(remote.sin_port),buf,10);
			buffer[len] = 0;
			m_Recv.Empty();
			m_Recv = buffer;
			UpdateData(FALSE);
			break;
		case FD_WRITE:
			MessageBox("write message\n",NULL,MB_OK);
			break;
		case FD_CLOSE:
			MessageBox("socket closed\n",NULL,MB_OK);
			closesocket(s);
			WSACleanup();
			break;
		default:
			if(WSAGETSELECTERROR(lParam) != 0){
				MessageBox("select report error\n",NULL,MB_OK);
				closesocket(s);
				WSACleanup();
			}
		}
	break;
	default:
		return CDialog::WindowProc(message, wParam, lParam);
	}
	return 0;
}

void CWincolDlg::OnInitbtn() 
{
	// TODO: Add your control notification handler code here
	WSAData wsa;
	struct sockaddr_in local;
	int len;

	WSAStartup(0x101,&wsa);
	sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock == INVALID_SOCKET){
		MessageBox("socket() failed\n","初始化",MB_OK);
		return;
	}

	UpdateData(TRUE);
	if(m_LocalPort != 0){
		local.sin_addr.S_un.S_addr = inet_addr("192.168.0.9");//htonl(INADDR_ANY);
		local.sin_family = AF_INET;
		local.sin_port = htons(m_LocalPort);
		len = sizeof(local);
		if(bind(sock,(sockaddr*)&local,len) < 0){
			MessageBox("bind() failed\n","初始化",MB_OK);
			return;
		}
	}
	
	WSAAsyncSelect(sock,m_hWnd,UM_SOCK,FD_READ|FD_WRITE|FD_CLOSE);
		
}


void CWincolDlg::OnSendbtn() 
{
	// TODO: Add your control notification handler code here
	struct sockaddr_in remote;
	int remote_len;
	int len;
	unsigned long retval;
	char buf[48];

	UpdateData(TRUE);
	
	m_DestIPAddr.GetAddress(remote.sin_addr.S_un.S_un_b.s_b1,remote.sin_addr.S_un.S_un_b.s_b2,
							remote.sin_addr.S_un.S_un_b.s_b3,remote.sin_addr.S_un.S_un_b.s_b4);
	remote.sin_family = AF_INET;
	remote.sin_port = htons(m_DestPort);
	remote_len = sizeof(remote);
	len = m_Send.GetLength();

	len = sendto(sock,m_Send,len,0,(sockaddr*)&remote,remote_len);
	if(len == SOCKET_ERROR){
		retval = WSAGetLastError();
		memset(buf,0,sizeof(buf));
		wsprintf(buf,"sendto() failed %u\n",retval);
		MessageBox(buf,"Sending Error",MB_OK);
	}

}

void CWincolDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	closesocket(sock);
	WSACleanup();
	CDialog::OnCancel();
}
