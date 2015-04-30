// coclient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "winsock.h"
#include "stdio.h"
#pragma comment(lib,"wsock32.lib")
#define SERVER_PORT 0x1234
#define LOCAL_PORT 0x4321
int main(int argc, char* argv[])
{
	SOCKET s;
	sockaddr_in server;
	int retval;
	char* sendbuf = "I am a client";
	WSAData wsa;

	WSAStartup(0x101,&wsa);

	s = socket(AF_INET,SOCK_STREAM,0);
	
	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);
	server.sin_port = htons(SERVER_PORT);

	retval = connect(s,(sockaddr*)&server,sizeof(server));
	if(retval != 0){
			retval = WSAGetLastError();
			closesocket(s);
			return 0;
	}


	printf("Connect to server!!!\n");
	
	retval = send(s,sendbuf,strlen(sendbuf),0);
	if(retval == SOCKET_ERROR){
		retval = WSAGetLastError();
	}

	closesocket(s);


	WSACleanup();
	return 0;
}
