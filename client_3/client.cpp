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
	server_address.sin_port = htons( PORT );

	bool toserver = 1;

	if (toserver)
	{
		server_address.sin_addr.S_un.S_un_b.s_b1 = 78;
		server_address.sin_addr.S_un.S_un_b.s_b2 = 24;
		server_address.sin_addr.S_un.S_un_b.s_b3 = 219;
		server_address.sin_addr.S_un.S_un_b.s_b4 = 108;
	}
	else
	{
		server_address.sin_addr.S_un.S_un_b.s_b1 = 127;
		server_address.sin_addr.S_un.S_un_b.s_b2 = 0;
		server_address.sin_addr.S_un.S_un_b.s_b3 = 0;
		server_address.sin_addr.S_un.S_un_b.s_b4 = 1;
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
		if( sendto( sock, buf, buffer_length, flags, to, to_length ) == SOCKET_ERROR )
		{
			printf( "sendto failed: %d", WSAGetLastError() );
			return;
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