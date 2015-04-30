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
	sockaddr_in local,server;
	int retval;
	char sendbuf[128];
	char recvbuf[128];
	WSAData wsa;

	WSAStartup(0x101,&wsa);

	s = socket(AF_INET,SOCK_STREAM,0);
	
	//local.sin_family = AF_INET;
	//local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	//local.sin_port = htons(LOCAL_PORT);
	
	//bind(s,(sockaddr*)&local,sizeof(local));

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
	
	while(strcmp(sendbuf,"exit")){
		printf("Please enter the string to send('exit' to end):");
		scanf("%s",sendbuf);
		send(s,sendbuf,strlen(sendbuf),0);
		retval = recv(s,recvbuf,sizeof(recvbuf),0);
		recvbuf[retval] = 0;
		printf("From server = %s\n",recvbuf);
	}

	closesocket(s);


	WSACleanup();
	return 0;
}
