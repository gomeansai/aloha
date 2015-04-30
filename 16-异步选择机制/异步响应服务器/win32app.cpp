// win32app.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "win32app.h"
#include <winsock.h>

#define MAX_LOADSTRING 100

#pragma comment (lib,"wsock32.lib")
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text
int row = 0;
SOCKET sock;
char buf[1024];
#define SERVER_PORT 0x1234
// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
VOID AlertUser(HWND hWnd,LPSTR lpszWarning);
VOID Start_server(HWND hWnd);
VOID Stop_server(HWND hWnd);
BOOL MyBlockingHook(void);
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32APP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_WIN32APP);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_WIN32APP);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_WIN32APP;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	SOCKET s,newsock;
	char recvbuf[128];
	struct sockaddr remote;
	int len;
	int retval;
	

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   Stop_server(hWnd);
				   DestroyWindow(hWnd);
				   break;
				case IDM_START:
				   Start_server(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);
			DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case UM_SOCK:
			s = (SOCKET)wParam;
			wmEvent = LOWORD(lParam);

			switch(wmEvent){
			case FD_ACCEPT:
				len = sizeof(remote);
				newsock = accept(s,&remote,&len);
				WSAAsyncSelect(newsock,hWnd,UM_SOCK,FD_READ|FD_CLOSE);//|FD_WRITE);
				break;
			case FD_READ:
				retval = recv(s,recvbuf,sizeof(recvbuf),0);
				if(retval <= 0){
					closesocket(s);
					break;
				}
				recvbuf[retval] = 0;
				
				hdc = BeginPaint(hWnd, &ps);
				// TODO: Add any drawing code here...
				hdc = GetDC(hWnd);
				TextOut(hdc,0,row,recvbuf,strlen(recvbuf));
				row += 16;
				ReleaseDC(hWnd,hdc);
				//»ØËÍÐÅÏ¢
				send(s,"ACK",3,0);
				
				break;
			case FD_CLOSE:
				MessageBox(hWnd,"closesocket\n","server",MB_OK);
				closesocket(s);
				break;
			case FD_WRITE:
				MessageBox(hWnd,"write event\n","server",MB_OK);
				break;
			default:
				if(WSAGETSELECTERROR(lParam) != 0){
					MessageBox(hWnd,"select report error\n","server",MB_OK);
					closesocket(s);
					WSACleanup();
				}
			}
		break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
VOID AlertUser(HWND hWnd,LPSTR lpszWarning)
{
	MessageBox(hWnd,lpszWarning,"Server",MB_OK|MB_ICONEXCLAMATION);
}
VOID Start_server(HWND hWnd)
{
	WSAData wsa;
	struct sockaddr_in server;

	if(WSAStartup(0x101,&wsa)){
		MessageBox(hWnd,"WSAStartup() failed\n","server",MB_OK);
		PostQuitMessage(0);
	}
	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock == INVALID_SOCKET){
		WSACleanup();
		MessageBox(hWnd,"socket() failed\n","server",MB_OK);
		PostQuitMessage(0);
	}

	server.sin_addr.S_un.S_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	if(bind(sock,(sockaddr*)&server,sizeof(server)) < 0){
		closesocket(sock);
		WSACleanup();
		MessageBox(hWnd,"bind() failed\n","server",MB_OK);
		return;
	}
	
	if(listen(sock,5) != 0){
		closesocket(sock);
		WSACleanup();
		MessageBox(hWnd,"listen() failed\n","server",MB_OK);
		PostQuitMessage(0);
	}
	if(WSAAsyncSelect(sock,hWnd,UM_SOCK,FD_ACCEPT) == SOCKET_ERROR){
		MessageBox(hWnd,"WSAAsyncSelect failed\n","server",MB_OK);
	}
	return ;
}
VOID Stop_server(HWND hWnd)
{
	closesocket(sock);
	WSACleanup();
	return;
}
