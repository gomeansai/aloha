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
#define PROB 20
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
int Start_fsm(int event);
void ProgramLog(char *Event,char *Data);
void delay_simulator(int maxdelay);
int error_simulator(int prob);

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
	closesocket(s);
	
	arg = 1;
	ioctlsocket(ns,FIONBIO,&arg);

	Fid = fopen("rx.txt","wb");
	if(Fid == NULL){
		printf("can`t open file\n");
		return 0;
	}
	state = STATE_START;
	while(1){
		FD_ZERO(&read_list);
		FD_SET(ns,&read_list);
		timeout.tv_sec = TIMEOUT;
		timeout.tv_usec = 0;
		retval = select(0,&read_list,NULL,NULL,&timeout);
		if(retval == SOCKET_ERROR)
			break;
		if(retval == 0){
			continue;
		}else{
			if(!FD_ISSET(ns,&read_list)){
				continue;
			}
			retval = recv(ns,(char*)&recvbuf,sizeof(recvbuf),0);
			if(retval <= 0){
				retval = WSAGetLastError();
				if(retval == WSAEWOULDBLOCK)
					continue;
				goto end;
			}
			if(recvbuf.type == FRAME_DATA){
				if(recvbuf.sequence_num == 0){
					event = EVENT_RECV0;
				}
				else if(recvbuf.sequence_num == 1){
					event = EVENT_RECV1;
				}
			}
			else
				continue;
		}
		switch(state){
		case STATE_START:
			state = Start_fsm(event);
		break;
		case STATE_RECV0:
			state = R0_fsm(event);
		break;
		case STATE_RECV1:
			state = R1_fsm(event);
		break;
		}
	}
end:
	closesocket(ns);
	fclose(Fid);
	WSACleanup();
	return 0;
}
int Start_fsm(int event)
{
	int next_state;

	next_state = STATE_START;
	switch(event){
	case EVENT_RECV0:
		fwrite(recvbuf.data,sizeof(char),recvbuf.len,Fid);
		//send ack back
		sendbuf.type = FRAME_ACK;
		sendbuf.len = 3;
		
			send(ns,(char*)&sendbuf,HEADSIZE,0);

		next_state = STATE_RECV0;
	break;
	case EVENT_RECV1:
		fwrite(recvbuf.data,sizeof(char),recvbuf.len,Fid);
		//send ack back
		sendbuf.type = FRAME_ACK;
		sendbuf.len = 3;

			send(ns,(char*)&sendbuf,HEADSIZE,0);

		next_state = STATE_RECV1;

	break;
	}
	return next_state;
}
int R0_fsm(int event)
{
	int next_state;

	next_state = STATE_RECV0;
	switch(event){
	case EVENT_RECV0:
		//duplicate frame
		//send ack
		sendbuf.type = FRAME_ACK;
		sendbuf.len = 3;
		
			send(ns,(char*)&sendbuf,HEADSIZE,0);
	break;
	case EVENT_RECV1:
		fwrite(recvbuf.data,sizeof(char),recvbuf.len,Fid);
		//send ack back
		sendbuf.type = FRAME_ACK;
		sendbuf.len = 3;

			send(ns,(char*)&sendbuf,HEADSIZE,0);

		next_state = STATE_RECV1;
		
	break;
	}
	return next_state;
}
int R1_fsm(int event)
{
	int next_state;
	
	next_state = STATE_RECV1;
	switch(event){
	case EVENT_RECV0:
		fwrite(recvbuf.data,sizeof(char),recvbuf.len,Fid);
		//send ack back
		sendbuf.type = FRAME_ACK;
		sendbuf.len = 3;
			send(ns,(char*)&sendbuf,HEADSIZE,0);

		next_state = STATE_RECV0;

	break;
	case EVENT_RECV1:
		//duplicate frame
		//send ack
		sendbuf.type = FRAME_ACK;
		sendbuf.len = 3;
			send(ns,(char*)&sendbuf,HEADSIZE,0);

	break;
	}
	return next_state;
}
int error_simulator(int prob)
{
	if(rand()*100/RAND_MAX < prob){
		return TRUE;
	}
	else
		return FALSE;
}
void delay_simulator(int maxdelay)
{
	//fd_set read_list;
	int delay;
	timeval timeout;
	fd_set readlist;
	SOCKET ss;
	char ch[2];

	ss = socket(AF_INET,SOCK_DGRAM,0);
	FD_ZERO(&readlist);
	FD_SET(ss,&readlist);
	delay = rand()*maxdelay/RAND_MAX;
	ch[0] = '0' + delay;
	ch[1] = 0;
	ProgramLog("Delay",ch);
	timeout.tv_sec = delay;
	timeout.tv_usec = 0;
	select(0,&readlist,NULL,NULL,&timeout);
	closesocket(ss);
}
void ProgramLog(char *Event,char *Data)
{
	struct _timeb timebuffer;
	char *timeline;

	_ftime( &timebuffer );
	timeline = ctime( & ( timebuffer.time ) );
	fprintf( LogFid, "Time: %.19s.%hu : ", timeline, timebuffer.millitm);
	printf("Time: %.19s.%hu : ", timeline, timebuffer.millitm);
	fprintf(LogFid,"%s",Event);
	printf("%s",Event);
	if(Data != NULL){
		fprintf(LogFid,"        %s\n",Data);
		printf("        %s\n",Data);
	}
	
	return;
}
