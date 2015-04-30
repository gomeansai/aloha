// coclient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#pragma comment (lib, "wsock32.lib")

#define SERVER_PORT 0x1234

int main(int argc, char* argv[])
{
	SOCKET sock;
	struct sockaddr_in server;
	char buf[128];
	WSAData wsa;
	int retval;

	WSAStartup(0x101,&wsa);
	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock == INVALID_SOCKET){
		printf("Creating socket error\n");
		return 0;
	}

	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);//inet_addr("202.115.12.38");
	server.sin_port = htons(SERVER_PORT);

	if(connect(sock,(sockaddr*)&server,sizeof(server)) < 0){
		retval = WSAGetLastError();
		printf("Conneting to server error\n");
		return 0;
	}
	retval = 1;
	printf("connect to server\n");
	while(1){
		//scan the keboard until input string is "exit"
		printf("Send = ");
		scanf("%s",buf);
		if(strcmp(buf,"exit") == 0)
			break;
		send(sock,buf,strlen(buf),0);
		/*retval = recv(sock,buf,sizeof(buf),0);
		if(retval > 0){
			buf[retval] = 0;
			printf("%s\n",buf);
		}else
			break;*/
	}

	//close the socket 

	closesocket(sock);
	WSACleanup();

	printf("passed\n");
	
	return 0;
}
