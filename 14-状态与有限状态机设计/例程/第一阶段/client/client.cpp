// client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#include <time.h>
#include <sys/timeb.h>
//#include <stdlib.h>
#define SERVER_PORT 0x1234
#pragma comment (lib, "wsock32.lib")

#define EVENT_TIMEOUT 1
#define EVENT_ACK     2

#define FRAME_ACK  1
#define FRAME_DATA 2

#define STATE_SEND0 1
#define STATE_SEND1 2
#define STATE_END   3
//define the probability
#define PROB 1
#define TIMEOUT 5

struct PDU{
	char type;
	char sequence_num;
	char len;
	char data[100];
}sendbuf,recvbuf;
#define HEADSIZE 3

FILE *Fid,*LogFid;
int state;
SOCKET sock;

int S1_fsm(int event);
int S0_fsm(int event);

int main(int argc, char* argv[])
{
	struct sockaddr_in server;
	WSAData wsa;
	int retval;
	unsigned long arg;
	timeval timeout;
	fd_set read_list;
	int event;


	WSAStartup(0x101,&wsa);
	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock == INVALID_SOCKET){
		printf("Creating socket error\n");
		return 0;
	}
	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);//inet_addr("202.115.12.35");
	server.sin_port = htons(SERVER_PORT);
	
	if(connect(sock,(sockaddr*)&server,sizeof(server)) < 0){
		retval = WSAGetLastError();
		printf("Conneting to server error\n");
		return 0;
	}


	printf("connect to server\n");


	//open file to read
	Fid = fopen("snd.txt","rb");
	if(Fid == NULL){
		printf("can`t open file\n");
		return 0;
	}

	while(1){
		//communicating
	
	}

//close the socket immediately
end:
	closesocket(sock);
	fclose(Fid);
	WSACleanup();
	return 0;
}
