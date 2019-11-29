#include <winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <thread>
#include <ctime>
#include <stdlib.h>
#include "2d_graphics.h"
#include "convert.h"

#pragma warning(disable:4996) 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
using namespace std;

const int BUFFERLENGTH = 40;
const int SERVERPORT = 1707;
const string SERVERIP = "78.24.219.108";
char buffer[BUFFERLENGTH];
SOCKET connectSocket;
SOCKADDR_IN otherAddr;

int otherSize;
string GuiTime = "0";
string MyOldTime = "0";
int ping = 0;
int lastping = 0;
int prevping[10];
bool gotmail = 0;
int gotmailtime;
int packet_num = 0;
string GuiPacketNum = "0";
string MyOldPacketNum = "0";
int LastShow = 0;
dualnet_int Framecount;
coord ForMeCalc_c;
dir ForMeCalc_d;
int ConnectToGuiFase = 0;

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

void writeconsole()
{
	cout << "CONNECTING";
	while (1)
	{
		switch (ConnectToGuiFase)
		{
		case 0:
			cout << ".";
			break;
		case 1:
				cout << "\nCONNECTED \n\n";
				Sleep(1000);
				ConnectToGuiFase = 2;
				break;
		case 2:
			//cout << ping << endl;
			break;
		}
		Sleep(100);
	}
}

void showt(string output)
{
	cout << output << " : " << clock() - LastShow << endl;
	LastShow = clock();
}

int calcping()
{
	int tempping = 0;
	for (int i = 9; i > 0; i--)
	{
		prevping[i] = prevping[i - 1];
		tempping += prevping[i];
	}
	prevping[0] = lastping;
	tempping += prevping[0];
	return (tempping / 10);
}

void TaskRec() 
{
	int PingCalcTime = 0;
	while (true) 
	{
		SOCKADDR_IN remoteAddr;
		int	remoteAddrLen = sizeof(remoteAddr);

		int iResult = recvfrom(connectSocket, buffer, BUFFERLENGTH, 0, (sockaddr*)&remoteAddr, &remoteAddrLen);

		if (iResult > 0) 
		{
			if (ConnectToGuiFase == 0)ConnectToGuiFase = 1;
			//cout << NormalizedIPString(remoteAddr) << " -> " << string(buffer, buffer + iResult) << endl;
			string recived = ch_tostr(buffer, 40);
			if (buffer[0] == '#')
			{	
				ForMeCalc_c.x = stoi(split(recived, "/", 2));
				ForMeCalc_c.y = stoi(split(recived, "/", 3));
				MyOldTime = split(recived, "/", 4);
				ForMeCalc_d.angle = float(stoi(split(recived, "/", 5))) / 100;
				ForMeCalc_d.value = stoi(split(recived, "/", 6));
				GuiPacketNum = split(recived, "/", 7);
				MyOldPacketNum = split(recived, "/", 8);

				try
				{
					lastping = clock() - stoi(MyOldTime);
				}
				catch (...)
				{
					//cout << "APE OF SHIT" << endl;
				}
				ping = calcping();
				
				if (ForMeCalc_d.value == 0 && me.speed.value != 0)
				{
					//cout << "stop " << abs(me.c.x - ForMeCalc_c.x) << " " << abs(me.c.y - ForMeCalc_c.y) << endl;
					me.c.x = ForMeCalc_c.x;
					me.c.y = ForMeCalc_c.y;
				}
				else if (me.speed.angle != ForMeCalc_d.angle)
				{
					//cout << abs(me.c.x - ForMeCalc_c.x) << " " << abs(me.c.y - ForMeCalc_c.y) << endl;
					cout << "dif  " << sqrt(pow(me.c.x - ForMeCalc_c.x, 2) + pow(me.c.y - ForMeCalc_c.y, 2)) << endl;
					cout << "calc " << ping * ForMeCalc_d.value / (2 * 16.6667) << endl;
					me.c.x = ForMeCalc_c.x;
					me.c.y = ForMeCalc_c.y;
				}
				me.speed.angle = ForMeCalc_d.angle;
				me.speed.value = ForMeCalc_d.value;
			}
			else
			{
				gui.getwasd(buffer);
				GuiTime = split(recived, "/", 2);
				gotmail = true;
				Framecount.me = 0;
			}
		}
		else 
		{
			//cout << "Error" << endl;
		}
	}
}

void TaskSendData()
{
	while (1)
	{
		/*if (gotmail || clock() - gotmailtime > 1000)
		{
			/*if(gotmail)cout << "send" << endl;
			else cout << "send	time" << endl;*/

			packet_num++;
			string msg = "#";
			msg += "/" + to_string(int(gui.c.x)) + "/" + to_string(int(gui.c.y)) + "/" + GuiTime + "/" + to_string(int(gui.speed.angle*100)) + "/" 
				+ to_string(int(gui.speed.value)) + "/" + to_string(packet_num) + "/"  + GuiPacketNum + "/" + to_string(Framecount.me) + "/";
			sendto(connectSocket, msg.c_str(), 40, 0, (sockaddr*)&otherAddr, otherSize);

			//cout << "send: ";
			//showt(to_string(packet_num));

			gotmail = false;
			gotmailtime = clock();
			Sleep(20);
		//}
	}
}

void TaskSendInput()
{
	while (1)
	{
		string msg = ch_tostr(keys, 4);
		msg += "/" + to_string(clock()) + "/";
		sendto(connectSocket, msg.c_str(), 20, 0, (sockaddr*)&otherAddr, otherSize);
		Sleep(20);
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
	//cout << "Identificationnumber: ";  cin >> request;

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

	u_long iMode = 1;
	ioctlsocket(connectSocket, FIONBIO, &iMode);

	gotmailtime = clock();

	thread t1(TaskRec);
	thread t2(TaskSendData);
	thread t3(TaskSendInput);
	thread t4(writeconsole);

	WindowSetup(1030, 150, 500, 500);
	GraphicsWindow();

	getchar();

	closesocket(connectSocket);
	WSACleanup();
	return 0;
}