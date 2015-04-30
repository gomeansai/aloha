// select.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "winsock.h"
#include "stdio.h"
#pragma comment(lib,"wsock32.lib")

struct socket_list{
	SOCKET MainSock;
	int num;
	SOCKET sock_array[64];
};
void init_list(socket_list *list)
{
	int i;
	list->MainSock = 0;
	list->num = 0;
	for(i = 0;i < 64;i ++){
		list->sock_array[i] = 0;
	}
}
void insert_list(SOCKET s,socket_list *list)
{
	int i;
	for(i = 0;i < 64; i++){
		if(list->sock_array[i] == 0){
			list->sock_array[i] = s;
			list->num += 1;
			break;
		}
	}
}
void delete_list(SOCKET s,socket_list *list)
{
	int i;
	for(i = 0;i < 64; i++){
		if(list->sock_array[i] == s){
			list->sock_array[i] = 0;
			list->num -= 1;
			break;
		}
	}
}
void make_fdlist(socket_list *list,fd_set *fd_list)
{
	int i;
	FD_SET(list->MainSock,fd_list);
	for(i = 0;i < 64;i++){
		if(list->sock_array[i] > 0){
			FD_SET(list->sock_array[i],fd_list);
		}
	}
}

int main(int argc, char* argv[])
{
	SOCKET s;
	sockaddr_in server,remote;
	struct socket_list sock_list;
	WSAData wsa;
	char buf[128];
	fd_set readfds,writefds,exceptfds;
	int retval,len,i;
	timeval timeout;


	WSAStartup(0x101,&wsa);
	s = socket(AF_INET,SOCK_STREAM,0);

	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server.sin_port = htons(0x1234);

	bind(s,(sockaddr *)&server,sizeof(server));

	listen(s,5);
	timeout.tv_sec = 0;
	timeout.tv_usec = 1;

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&exceptfds);

	init_list(&sock_list);
	sock_list.MainSock = s;
	
	while(1){
		make_fdlist(&sock_list,&readfds);
		//make_fdlist(&sock_list,&writefds);
		make_fdlist(&sock_list,&exceptfds);
		
		retval = select(0,&readfds,&writefds,&exceptfds,&timeout);
		if(FD_ISSET(sock_list.MainSock,&readfds)){
			len = sizeof(remote);
			s = accept(sock_list.MainSock,(sockaddr*)&remote,&len);
			insert_list(s,&sock_list);
		}
		for(i = 0; i < 64; i++){
			s = sock_list.sock_array[i];
			if( s == 0){
				continue;
			}
			if(FD_ISSET(s,&readfds)){
				retval = recv(s,buf,sizeof(buf),0);
				if(retval > 0){
					buf[retval] = 0;
					printf("->%s",buf);
					send(s,"ACK by server",13,0);
				}
				else{
					closesocket(s);
					delete_list(s,&sock_list);
				}
			}
			
			//if(FD_ISSET(s,&writefds){
				
			//}
			if(FD_ISSET(s,&exceptfds)){
				closesocket(s);
				delete_list(s,&sock_list);
			}
		}
		FD_ZERO(&readfds);
		//FD_ZERO(&writefds);
		FD_ZERO(&exceptfds);
	}
	WSACleanup();
	return 0;
}
