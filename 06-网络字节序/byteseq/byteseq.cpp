// byteseq.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "winsock.h"
#include "stdio.h"
#pragma comment(lib,"wsock32.lib")
//ipaddress 202.115.12.2  0xca730c02  3396537346
int main(int argc, char* argv[])
{
	unsigned long x,y,z;
	unsigned long IPaddr_1,IPaddr_2,IPaddr_3;
	unsigned long *p;
	char *IPaddr_s1, *IPaddr_s2, *IPaddr_s3,IPaddr_s[16];
	struct sockaddr_in IPaddr_in;

	x = 0xca730c01;
	p = &x;

	y = htonl(x);
	p = &y;

	z = ntohl(y);
	p = &z;

	IPaddr_1 = inet_addr("202.115.12.1");
	IPaddr_in.sin_addr.S_un.S_addr = IPaddr_1;
	IPaddr_s1 = inet_ntoa(IPaddr_in.sin_addr);

	IPaddr_2 = 0xca730c02;
	IPaddr_in.sin_addr.S_un.S_addr = IPaddr_2;
	IPaddr_s2 = inet_ntoa(IPaddr_in.sin_addr);
	strcpy(IPaddr_s,IPaddr_s2);

	IPaddr_3 = 0xca730c03;
	IPaddr_in.sin_addr.S_un.S_addr = htonl(IPaddr_3);
	IPaddr_s3 = inet_ntoa(IPaddr_in.sin_addr);


	return 0;
}
