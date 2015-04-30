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
	unsigned long data;
	unsigned long * datap;
	int retval;
	char * dotIP;

	datap = &data;
	WSAStartup(0x101,&wsa);
	
	s = socket(AF_INET,SOCK_STREAM,0);

	local.sin_family = AF_INET;
	local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	local.sin_port = htons(SERVER_PORT);

	retval = bind(s,(sockaddr*)&local,sizeof(local));
	if(retval != 0)
		retval = WSAGetLastError();

	retval = listen(s,1);

	while(1){
		len = sizeof(remote);
		ns = accept(s,(sockaddr*)&remote,&len);

		dotIP = inet_ntoa(remote.sin_addr);
		
		printf("remote IP address1 = %s\n",dotIP);
		printf("remote Port = 0x%x\n",ntohs(remote.sin_port));

		retval = recv(ns,(char*)&data,sizeof(data),0);
		printf("received data = 0x%x\n",data);

		retval = recv(ns,(char*)&data,sizeof(data),0);
		data = ntohl(data);
		printf("received data = 0x%x\n",data);

		retval = recv(ns,(char*)&data,sizeof(data),0);
		data = ntohl(data);
		printf("received data = 0x%x\n",data);

		retval = recv(ns,(char*)&data,sizeof(data),0);
		printf("received data = 0x%x\n",data);
		
		closesocket(ns);
	}

	WSACleanup();
	return 0;
}
