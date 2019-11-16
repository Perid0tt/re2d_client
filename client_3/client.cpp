#include <winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <thread>
#include "2d_graphics.h"
#include "convert.h"

#pragma warning(disable:4996) 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
using namespace std;

const int BUFFERLENGTH = 1024;
const int SERVERPORT = 1707;
const string SERVERIP = "78.24.219.108";
char buffer[BUFFERLENGTH];

SOCKET connectSocket;
SOCKADDR_IN otherAddr;
int otherSize;

extern char keys[];
extern player gui;
extern player me;


string NormalizedIPString(SOCKADDR_IN addr) {
	char host[16];
	ZeroMemory(host, 16);
	inet_ntop(AF_INET, &addr.sin_addr, host, 16);

	USHORT port = ntohs(addr.sin_port);

	int realLen = 0;
	for (int i = 0; i < 16; i++) 
	{
		if (host[i] == '\00') break;
		realLen++;
	}

	string res(host, realLen);
	res += ":" + to_string(port);

	return res;
}

void TaskRec() 
{
	while (true) 
	{
		SOCKADDR_IN remoteAddr;
		int	remoteAddrLen = sizeof(remoteAddr);

		int iResult = recvfrom(connectSocket, buffer, BUFFERLENGTH, 0, (sockaddr*)&remoteAddr, &remoteAddrLen);

		if (iResult > 0) 
		{
			cout << NormalizedIPString(remoteAddr) << " -> " << string(buffer, buffer + iResult) << endl;
			string recived = ch_tostr(buffer, 20);
			gui.getwasd(buffer);
			me.c.x = stoi(split(recived, "/", 2));
			me.c.y = stoi(split(recived, "/", 3));
		}
		else 
		{
			cout << "Error: Peer closed." << endl;
		}
	}
}

void TaskSend()
{
	while (1)
	{
		string msg = ch_tostr(keys, 4);
		msg += "/" +  to_string(gui.c.x) + "/" + to_string(gui.c.y) + "/";
		sendto(connectSocket, msg.c_str(), 20, 0, (sockaddr*)&otherAddr, otherSize);
		Sleep(5);
	}
}


int main(int argc, char* argv[])
{

	SetConsoleTitleA("Client");

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKADDR_IN serverAddr;
	serverAddr.sin_port = htons(SERVERPORT);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(SERVERIP.c_str());


	int serverSize = sizeof(serverAddr);

	connectSocket = socket(AF_INET, SOCK_DGRAM, 0);

	SOCKADDR_IN clientAddr;
	clientAddr.sin_port = 0;
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = INADDR_ANY;

	bind(connectSocket, (LPSOCKADDR)&clientAddr, sizeof(clientAddr));

	int val = 64 * 1024;
	setsockopt(connectSocket, SOL_SOCKET, SO_SNDBUF, (char*)&val, sizeof(val));
	setsockopt(connectSocket, SOL_SOCKET, SO_RCVBUF, (char*)&val, sizeof(val));

	string request = "1";
	cout << "Identificationnumber: ";  cin >> request;

	sendto(connectSocket, request.c_str(), request.length(), 0, (sockaddr*)&serverAddr, serverSize);

	bool notFound = true;

	string endpoint;

	while (notFound) 
	{
		SOCKADDR_IN remoteAddr;
		int	remoteAddrLen = sizeof(remoteAddr);

		int iResult = recvfrom(connectSocket, buffer, BUFFERLENGTH, 0, (sockaddr*)&remoteAddr, &remoteAddrLen);

		if (iResult > 0) 
		{
			endpoint = string(buffer, buffer + iResult);

			cout << "Peer-to-peer Endpoint: " << endpoint << endl;

			notFound = false;
		}
		else 
		{
			cout << WSAGetLastError();
		}
	}
	 
	string host = endpoint.substr(0, endpoint.find(':'));
	int port = stoi(endpoint.substr(endpoint.find(':') + 1));

	otherAddr.sin_port = htons(port);
	otherAddr.sin_family = AF_INET;
	otherAddr.sin_addr.s_addr = inet_addr(host.c_str());

	otherSize = sizeof(otherAddr);

	thread t1(TaskRec);
	thread t2(TaskSend);

	WindowSetup(1030, 150, 500, 500);
	GraphicsWindow();

	getchar();

	closesocket(connectSocket);
	WSACleanup();
	return 0;
}