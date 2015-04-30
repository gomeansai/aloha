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
#define PROB 20
#define TIMEOUT 6

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
int End_fsm(int event);
void ProgramLog(char *Event,char *Data);
int error_simulator(int prob);

int main(int argc, char* argv[])
{
	struct sockaddr_in server;
	WSAData wsa;
	int retval;
	unsigned long arg;
	timeval timeout;
	fd_set read_list;
	int event;
	clock_t now_time;

	//open log file
	LogFid = fopen("clogfile.txt","a+t");

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
	ProgramLog("Connect to server",NULL);

	//set the socket to be unblocked
	arg = 1;
	ioctlsocket(sock,FIONBIO,&arg);
	now_time = clock();
	srand((unsigned int)now_time);

	//open file to read
	Fid = fopen("snd.txt","rb");
	if(Fid == NULL){
		printf("can`t open file\n");
		return 0;
	}
	sendbuf.len = fread(sendbuf.data,sizeof(char),sizeof(sendbuf.data),Fid);
	sendbuf.type = FRAME_DATA;
	sendbuf.sequence_num = 0;
	if(sendbuf.len < sizeof(sendbuf.data)){
		state = STATE_END;
	}
	else{
		state = STATE_SEND0;
	}

	retval = error_simulator(PROB);
	if(retval == FALSE){
		send(sock,(char*)&sendbuf,sendbuf.len+HEADSIZE,0);
		ProgramLog("Send Frame 0",NULL);
	}else{
		ProgramLog("Send Frame 0","But Lost");
	}

	while(1){
		FD_ZERO(&read_list);
		FD_SET(sock,&read_list);
		timeout.tv_sec = TIMEOUT;
		timeout.tv_usec = 0;
		retval = select(0,&read_list,NULL,NULL,&timeout);
		if(retval == SOCKET_ERROR)
			break;
		if(retval == 0){
			event = EVENT_TIMEOUT;
			ProgramLog("Timeout",NULL);
		}else{
			if(!FD_ISSET(sock,&read_list)){
				continue;
			}
			retval = recv(sock,(char*)&recvbuf,sizeof(recvbuf),0);
			if(retval <= 0){
				retval = WSAGetLastError();
				if(retval == WSAEWOULDBLOCK)
					continue;
				ProgramLog("Connection was closed",NULL);
				goto end;
			}
			if(recvbuf.type != FRAME_ACK){
				continue;
			}
			event = EVENT_ACK;
			ProgramLog("Recv ACK",NULL);
		}
		switch(state){
		case STATE_SEND0:
			state = S0_fsm(event);
		break;
		case STATE_SEND1:
			state = S1_fsm(event);
		break;
		case STATE_END:
			state = End_fsm(event);
		break;
		default :
			goto end;
		}
	}

//close the socket immediately
end:
	closesocket(sock);
	fclose(Fid);
	WSACleanup();
	return 0;
}
int S0_fsm(int event)
{
	int next_state;

	next_state = STATE_SEND0;
	switch(event){
	case EVENT_ACK:
		sendbuf.len = fread(sendbuf.data,sizeof(char),sizeof(sendbuf.data),Fid);
		sendbuf.sequence_num = 1;
		sendbuf.type = FRAME_DATA;

		if(error_simulator(PROB) == FALSE){
			send(sock,(char*)&sendbuf,sendbuf.len+HEADSIZE,0);
			ProgramLog("Send frame 1",NULL);
		}else
			ProgramLog("Send frame 1","But lost");

		if(sendbuf.len < sizeof(sendbuf.data)){
			next_state = STATE_END;
		}
		else{
			next_state = STATE_SEND1;
		}
	break;
	case EVENT_TIMEOUT:
		//resend the buf
		if(error_simulator(PROB) == FALSE){
			send(sock,(char*)&sendbuf,sendbuf.len+HEADSIZE,0);
			ProgramLog("Resend frame 0",NULL);
		}else
			ProgramLog("Resend frame 0","But lost");
	break;
	}
	return next_state;
}
int S1_fsm(int event)
{
	int next_state;
	
	next_state = STATE_SEND1;
	switch(event){
	case EVENT_ACK:
		sendbuf.len = fread(sendbuf.data,sizeof(char),sizeof(sendbuf.data),Fid);
		sendbuf.sequence_num = 0;
		sendbuf.type = FRAME_DATA;

		if(error_simulator(PROB) == FALSE){
			send(sock,(char*)&sendbuf,sendbuf.len+HEADSIZE,0);
			ProgramLog("Send frame 0",NULL);
		}else
			ProgramLog("Send frame 0","But lost");

		if(sendbuf.len < sizeof(sendbuf.data)){
			next_state = STATE_END;
		}
		else{
			next_state = STATE_SEND0;
		}
	break;
	case EVENT_TIMEOUT:
		if(error_simulator(PROB) == FALSE){
			send(sock,(char*)&sendbuf,sendbuf.len+HEADSIZE,0);
			ProgramLog("Resend frame 1",NULL);
		}else
			ProgramLog("Resend frame 1","But lost");

	break;
	}
	return next_state;
}
int End_fsm(int event)
{
	int next_state;

	next_state = STATE_END;
	switch(event){
	case EVENT_ACK:
		next_state = 0;
		closesocket(sock);
		ProgramLog("Disconnect",NULL);
	break;
	case EVENT_TIMEOUT:
		if(error_simulator(PROB) == FALSE){
			send(sock,(char*)&sendbuf,sendbuf.len+HEADSIZE,0);
			ProgramLog("Resend frame",NULL);
		}else
			ProgramLog("Resend frame","But lost");
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
void ProgramLog(char *Event,char *Data)
{
	struct _timeb timebuffer;
	char *timeline;

	_ftime( &timebuffer );
	timeline = ctime( & ( timebuffer.time ) );
	fprintf( LogFid, "Time %.19s.%hu : ", timeline, timebuffer.millitm);
	printf("Time %.19s.%hu : ", timeline, timebuffer.millitm);
	fprintf(LogFid,"%s",Event);
	printf("%s",Event);

	if(Data != NULL){
		fprintf(LogFid,"      %s\n",Data);
		printf("      %s\n",Data);
	}
	else{
		fprintf(LogFid,"\n");
		printf("\n");
	}
	return;
}
