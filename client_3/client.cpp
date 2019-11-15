#include "odin.h"

#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <windows.h>

using namespace std;

void main()
{
	WORD winsock_version = 0x202;
	WSADATA winsock_data;
	if( WSAStartup( winsock_version, &winsock_data ) )
	{
		printf( "WSAStartup failed: %d", WSAGetLastError() );
		return;
	}

	int address_family = AF_INET;
	int type = SOCK_DGRAM;
	int protocol = IPPROTO_UDP;
	SOCKET sock = socket( address_family, type, protocol );

	if( sock == INVALID_SOCKET )
	{
		printf( "socket failed: %d", WSAGetLastError() );
		return;

	}

	SOCKADDR_IN server_address;
	server_address.sin_family = AF_INET;

	bool toserver = 0;

	if (toserver)
	{
		server_address.sin_addr.S_un.S_un_b.s_b1 = 78;
		server_address.sin_addr.S_un.S_un_b.s_b2 = 24;
		server_address.sin_addr.S_un.S_un_b.s_b3 = 219;
		server_address.sin_addr.S_un.S_un_b.s_b4 = 108;
		server_address.sin_port = htons(1707);
	}
	else
	{
		server_address.sin_addr.S_un.S_un_b.s_b1 = 188;
		server_address.sin_addr.S_un.S_un_b.s_b2 = 170;
		server_address.sin_addr.S_un.S_un_b.s_b3 = 83;
		server_address.sin_addr.S_un.S_un_b.s_b4 = 199;
		server_address.sin_port = htons(34001);
	}

	//int8 buffer[SOCKET_BUFFER_SIZE];
	char buf[20];
	

	printf("%d.%d.%d.%d:%d\n", server_address.sin_addr.S_un.S_un_b.s_b1, server_address.sin_addr.S_un.S_un_b.s_b2, server_address.sin_addr.S_un.S_un_b.s_b3, server_address.sin_addr.S_un.S_un_b.s_b4, ntohs(server_address.sin_port));
	while(1)
	{
		cin >> buf;
		int buffer_length = 19;
		int flags = 0;
		SOCKADDR* to = (SOCKADDR*)&server_address;
		int to_length = sizeof( server_address );
		for (int i = 0; i < 10; i++)
		{
			sendto(sock, buf, buffer_length, flags, to, to_length);
		}

		//recive
		flags = 0;
		SOCKADDR_IN from;
		int from_size = sizeof(from);
		int bytes_received = recvfrom(sock, buf, 20, flags, (SOCKADDR*)&from, &from_size);

		if (bytes_received == SOCKET_ERROR)
		{
			printf("recvfrom returned SOCKET_ERROR, WSAGetLastError() %d", WSAGetLastError());
			break;
		}
		printf("%s\n", buf);
	}
}