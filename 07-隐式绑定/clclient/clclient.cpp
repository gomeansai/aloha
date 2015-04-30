// clclient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "winsock.h"
#include "stdio.h"
#pragma comment(lib,"wsock32.lib")
#define SERVER_PORT 0x1234
#define CLIENT_PORT 0x4321
int main(int argc, char* argv[])
{
	SOCKET s;
	sockaddr_in local,remote;
	int len;
	int retval;
	char recvbuf[128];
	char sendbuf[128];
	WSAData wsa;

	WSAStartup(0x101,&wsa);
	s = socket(AF_INET,SOCK_DGRAM,0);

	//local.sin_family = AF_INET;
	//local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	//local.sin_port = htons(CLIENT_PORT);

	//bind(s,(sockaddr*)&local,sizeof(local));

	len = sizeof(remote);

	remote.sin_family = AF_INET;
	remote.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);
	remote.sin_port = htons(SERVER_PORT);


	while(strcmp(sendbuf,"exit")){
		printf("Please enter the string to send('exit' to end):");
		scanf("%s",sendbuf);

		retval = sendto(s,sendbuf,strlen(sendbuf),0,(sockaddr*)&remote,len);
		if(retval == -1){
			retval = WSAGetLastError();
			break;
		}

		retval = recvfrom(s,recvbuf,sizeof(recvbuf),0,(sockaddr *)&remote,&len);
		if(retval == -1){
			retval = WSAGetLastError();
			break;
		}
		recvbuf[retval] = 0;
		printf("from server: %s\n",recvbuf);
	}
	
	closesocket(s);

	WSACleanup();
	return 0;
}
