// winserver.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "winsock.h"
#pragma comment (lib,"wsock32.lib")
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
struct socket_list{
	SOCKET MainSock;
	int num;
	SOCKET sock_array[64];
};

void init_list(socket_list *list)
{
	int i;
	list->MainSock = 0;
	list->num = 0;
	for(i = 0;i < 64;i ++){
		list->sock_array[i] = 0;
	}
}
void insert_list(SOCKET s,socket_list *list)
{
	int i;
	for(i = 0;i < 64; i++){
		if(list->sock_array[i] == 0){
			list->sock_array[i] = s;
			list->num += 1;
			break;
		}
	}
}
void delete_list(SOCKET s,socket_list *list)
{
	int i;
	for(i = 0;i < 64; i++){
		if(list->sock_array[i] == s){
			list->sock_array[i] = 0;
			list->num -= 1;
			break;
		}
	}
}
void make_fdlist(socket_list *list,fd_set *fd_list)
{
	int i;
	FD_SET(list->MainSock,fd_list);
	for(i = 0;i < 64;i++){
		if(list->sock_array[i] > 0){
			FD_SET(list->sock_array[i],fd_list);
		}
	}
}

struct socket_list sock_list;
fd_set readfds,writefds,exceptfds;
int row = 0;
char buf[1024];
void InitServer(HWND hWnd);
void StartServer(HWND hWnd);

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
	LoadString(hInstance, IDC_WINSERVER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_WINSERVER);

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
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_WINSERVER);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_WINSERVER;
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
				   DestroyWindow(hWnd);
				   break;
				case IDM_START:
					StartServer(hWnd);
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
		case WM_CREATE:
			InitServer(hWnd);
			return DefWindowProc(hWnd, message, wParam, lParam);
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
void StartServer(HWND hWnd)
{
	sockaddr_in remote_addr;
	int len;
	HDC hdc;
	int retval;
	int i;
	SOCKET sock;
	timeval timeout;

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	while(1){
		make_fdlist(&sock_list,&readfds);
		make_fdlist(&sock_list,&writefds);
		make_fdlist(&sock_list,&exceptfds);
		retval = select(0,&readfds,&writefds,&exceptfds,&timeout);
		if(retval == SOCKET_ERROR){
			retval = WSAGetLastError();
			break;
		}
		if(FD_ISSET(sock_list.MainSock,&readfds)){
			len = sizeof(remote_addr);
			sock = accept(sock_list.MainSock,(sockaddr*)&remote_addr,&len);
			if(sock == SOCKET_ERROR){
				MessageBox(hWnd,"accept() failed","server",MB_OK);
				continue;
			}
			MessageBox(hWnd, "accept a connection","server",MB_OK);
			insert_list(sock,&sock_list);
		}
		for(i = 0;i < 64;i++){
			if(sock_list.sock_array[i] == 0)
				continue;
			sock = sock_list.sock_array[i];
			if(FD_ISSET(sock,&readfds)){
				retval = recv(sock,buf,128,0);
				if(retval == 0){
					closesocket(sock);
					MessageBox(hWnd, "close a socket","server",MB_OK);
					delete_list(sock,&sock_list);
					continue;
				}else if(retval == -1){
					retval = WSAGetLastError();
					if(retval == WSAEWOULDBLOCK)
						continue;
					closesocket(sock);
					MessageBox(hWnd,"close a socket of error","server",MB_OK);
					delete_list(sock,&sock_list);
					continue;
				}
				//printf buf
				hdc = GetDC(hWnd);
				TextOut(hdc,0,row,buf,retval);
				row += 16;
				ReleaseDC(hWnd,hdc);
				//send ack
				send(sock,"ACK",3,0);

			}
			//if(FD_ISSET(sock,&writefds)){
			//}
			//if(FD_ISSET(sock,&exceptfds)){
			//}
		}
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_ZERO(&exceptfds);
	}
}
void InitServer(HWND hWnd)
{
	sockaddr_in server;
	int retval;
	SOCKET sock;
	WSAData wsa;
	unsigned long arg;

	WSAStartup(0x101,&wsa);
	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock == SOCKET_ERROR){
		MessageBox(hWnd,"socket() failed","server",MB_OK);
		return;
	}

	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server.sin_port = htons(0x1234);
	retval = bind(sock,(sockaddr*)&server,sizeof(server));
	
	retval = listen(sock,5);

	/*set the socket to nonbolck */
	arg = 1;
	ioctlsocket(sock,FIONBIO,&arg);
	init_list(&sock_list);

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&exceptfds);
	sock_list.MainSock = sock;

	return;
}