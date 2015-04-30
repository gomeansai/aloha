// clserver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "winsock.h"
#include "stdio.h"
#pragma comment(lib,"wsock32.lib")

#define SERVER_PORT 0x1234
int main(int argc, char* argv[])
{
	SOCKET s;
	sockaddr_in local,remote;
	int len;
	int retval;
	char recvbuf[100];
	WSAData wsa;

	WSAStartup(0x101,&wsa);

	s = socket(AF_INET,SOCK_DGRAM,0);

	local.sin_family  = AF_INET;
	local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	local.sin_port = htons(SERVER_PORT);

	bind(s,(sockaddr*)&local,sizeof(local));

	len = sizeof(remote);

	while(1){
	
		retval = recvfrom(s,recvbuf,sizeof(recvbuf),0,(sockaddr *)&remote,&len);
		if(retval == -1){
			retval = WSAGetLastError();
			continue;
		}
		recvbuf[retval] = 0;
		printf("remote IP address = %s\n",inet_ntoa(remote.sin_addr));
		printf("remote port = %u\n",htons(remote.sin_port));
		printf("recv: %s\n",recvbuf);

		retval = sendto(s,"ACK",3,0,(sockaddr*)&remote,len);
		if(retval == -1){
			retval = WSAGetLastError();
			continue;
		}

	}
	closesocket(s);

	WSACleanup();
	return 0;
}
