// server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "winsock.h"
#include "stdio.h"
#include "time.h"
#include <sys/timeb.h>
#define SERVER_PORT 0x1234
#pragma comment (lib, "wsock32.lib")

#define FRAME_ACK  1
#define FRAME_DATA 2

#define EVENT_RECV0   1
#define EVENT_RECV1   2

#define STATE_START 1
#define STATE_RECV0 2
#define STATE_RECV1 3

//define the probability
#define PROB 1
#define DELAY 5
#define TIMEOUT 5

struct PDU{
	char type;
	char sequence_num;
	char len;
	char data[100];
}sendbuf,recvbuf;

#define HEADSIZE 3
FILE *Fid,*LogFid;
SOCKET s,ns;
int state;

int R1_fsm(int event);
int R0_fsm(int event);

int main(int argc, char* argv[])
{
	sockaddr_in server,remote;
	int retval;
	int len;
	unsigned long arg;
	fd_set read_list;
	int event;
	WSAData wsa;
	timeval timeout;


	WSAStartup(0x101,&wsa);
	s = socket(AF_INET,SOCK_STREAM,0);

	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server.sin_port = htons(0x1234);

	bind(s,(sockaddr *)&server,sizeof(server));

	listen(s,5);

	len = sizeof(remote);
	ns = accept(s,(sockaddr*)&remote,&len);
	if(ns <= 0)
		return 0;

	Fid = fopen("rx.txt","wb");
	if(Fid == NULL){
		printf("can`t open file\n");
		return 0;
	}

	while(1){
		//communicating

	}
end:
	closesocket(ns);
	fclose(Fid);
	WSACleanup();
	return 0;
}

