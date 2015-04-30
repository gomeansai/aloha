// coserver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "winsock.h"
#include "stdio.h"
#pragma comment(lib,"wsock32.lib")
#define SERVER_PORT 0x1234
int main(int argc, char* argv[])
{
	WSAData wsa;
	SOCKET s,ns;
	sockaddr_in local,remote;
	int len;
	char recvbuf[128];
	int retval;

	WSAStartup(0x101,&wsa);
	
	s = socket(AF_INET,SOCK_STREAM,0);
	if(s == SOCKET_ERROR)
		retval = WSAGetLastError();

	local.sin_family = AF_INET;
	local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	local.sin_port = htons(SERVER_PORT);

	retval = bind(s,(sockaddr*)&local,sizeof(local));
	if(retval != 0)
		retval = WSAGetLastError();

	retval = listen(s,1);
	if(retval != 0)
		retval = WSAGetLastError();

	while(1){
		len = sizeof(remote);
		ns = accept(s,(sockaddr*)&remote,&len);
		if(ns == SOCKET_ERROR)
			retval = WSAGetLastError();

		printf("remote IP address = %s\n",inet_ntoa(remote.sin_addr));
		printf("remote port = %u\n",htons(remote.sin_port));

		retval = recv(ns,recvbuf,sizeof(recvbuf),0);
		if(retval == -1)
			retval = WSAGetLastError();
		while(retval > 0){
			recvbuf[retval] = 0;
			printf("recv: %s\n",recvbuf);
		
			send(ns,"ACK",3,0);
			retval = recv(ns,recvbuf,sizeof(recvbuf),0);
		}
		closesocket(ns);
	}

	WSACleanup();
	return 0;
}
