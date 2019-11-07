#include "odin.h"

#include <stdio.h>
#include <winsock2.h>

#include <iostream>
#include <windows.h>

void main()
{
	//coment
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
		server_address.sin_addr.S_un.S_un_b.s_b1 = 10;
		server_address.sin_addr.S_un.S_un_b.s_b2 = 0;
		server_address.sin_addr.S_un.S_un_b.s_b3 = 1;
		server_address.sin_addr.S_un.S_un_b.s_b4 = 2;
	}

	int8 buffer[SOCKET_BUFFER_SIZE];
	int32 player_x;
	int32 player_y;
	printf("%d.%d.%d.%d:%d", server_address.sin_addr.S_un.S_un_b.s_b1, server_address.sin_addr.S_un.S_un_b.s_b2, server_address.sin_addr.S_un.S_un_b.s_b3, server_address.sin_addr.S_un.S_un_b.s_b4, ntohs(server_address.sin_port));
	//printf( "type w, a, s, or d to move, q to quit\n" );
	bool32 is_running = 1;
	int count = 0;
	while( is_running )
	{
		count++;
		Sleep(500);
		// get input
		//scanf_s( "\n%c", &buffer[0], 1 );
		buffer[0] = 13;
		// send to server
		int buffer_length = 1;
		int flags = 0;
		SOCKADDR* to = (SOCKADDR*)&server_address;
		int to_length = sizeof( server_address );
		if( sendto( sock, buffer, buffer_length, flags, to, to_length ) == SOCKET_ERROR )
		{
			printf( "sendto failed: %d", WSAGetLastError() );
			return;

		}

		// wait for reply
		/*
		flags = 0;
		SOCKADDR_IN from;
		int from_size = sizeof( from );
		int bytes_received = recvfrom( sock, buffer, SOCKET_BUFFER_SIZE, flags, (SOCKADDR*)&from, &from_size );
		
		if( bytes_received == SOCKET_ERROR )
		{
			printf( "recvfrom returned SOCKET_ERROR, WSAGetLastError() %d", WSAGetLastError() );
			break;
		}

		// grab data from packet
		int32 read_index = 0;

		memcpy( &player_x, &buffer[read_index], sizeof( player_x ) );
		read_index += sizeof( player_x );

		memcpy( &player_y, &buffer[read_index], sizeof( player_y ) );
		read_index += sizeof( player_y );

		memcpy( &is_running, &buffer[read_index], sizeof( is_running ) );

		printf( "x:%d, y:%d, is_running:%d\n", player_x, player_y, is_running );*/
	}
}