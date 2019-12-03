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

const int BUFFERLENGTH = 1024;
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
int ConnectToGuiFase = 0;
vector<coord> ForMeCalc_c;
vector<dir> ForMeCalc_d;

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
			//cout << "ping: " << ping << endl;
			break;
		}
		Sleep(100);
	}
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
	vector<coord>().swap(ForMeCalc_c);
	ForMeCalc_c.resize(1);
	vector<dir>().swap(ForMeCalc_d);
	ForMeCalc_d.resize(1);

	int PingCalcTime = 0;
	while (true) 
	{
		SOCKADDR_IN remoteAddr;
		int	remoteAddrLen = sizeof(remoteAddr);
		int iResult = recvfrom(connectSocket, buffer, BUFFERLENGTH, 0, (sockaddr*)&remoteAddr, &remoteAddrLen);

		if (iResult > 0) 
		{
			if (ConnectToGuiFase == 0) ConnectToGuiFase = 1;
			//cout << NormalizedIPString(remoteAddr) << " -> " << string(buffer, buffer + iResult) << endl;

			int byte_num_lengh;
			string recv_lengh;
			for (int i = 0; true; i++)
			{
				if (buffer[i] == '$')
				{
					byte_num_lengh = i + 1;
					break;
				}
			}
			for (int i = byte_num_lengh; buffer[i] != '&'; i++)
				recv_lengh[i - byte_num_lengh] = buffer[i];
			string recived = ch_tostr(buffer, stoi(recv_lengh));

			if (buffer[0] == '#')
			{	
				cout << recived << endl;

				ForMeCalc_c[0].x = stoi(split(recived, "/", 2));
				ForMeCalc_c[0].y = stoi(split(recived, "/", 3));
				MyOldTime = split(recived, "/", 4);
				ForMeCalc_d[0].angle = float(stoi(split(recived, "/", 5))) / 100;
				ForMeCalc_d[0].value = float(stoi(split(recived, "/", 6))) / 100;
				GuiPacketNum = split(recived, "/", 7);
				MyOldPacketNum = split(recived, "/", 8);

				me.dobj_num = stoi(split(recived, "/", 9));
				ForMeCalc_c.resize(me.dobj_num + 1);
				ForMeCalc_d.resize(me.dobj_num + 1);
				for (int i = 0; i < me.dobj_num; i++)
				{
					me.dobj[i].type = stoi(split(recived, "/", 10 + i*5));
					ForMeCalc_c[i + 1].x = stoi(split(recived, "/", 11 + i * 5));
					ForMeCalc_c[i + 1].y = stoi(split(recived, "/", 12 + i * 5));
					ForMeCalc_d[i + 1].angle = float(stoi(split(recived, "/", 13 + i * 5))) / 100;
					ForMeCalc_d[i + 1].value = float(stoi(split(recived, "/", 14 + i * 5))) / 100;

					if ((ForMeCalc_d[i + 1].value == 0 && me.dobj[i].speed.value != ForMeCalc_d[i + 1].value) || (me.dobj[i].speed.angle != ForMeCalc_d[i + 1].angle))
					{
						me.dobj[i].c.x = ForMeCalc_c[i + 1].x;
						me.dobj[i].c.y = ForMeCalc_c[i + 1].y;
					}
					me.dobj[i].speed.angle = ForMeCalc_d[i + 1].angle;
					me.dobj[i].speed.value = ForMeCalc_d[i + 1].value;
				}

				try
				{
					lastping = clock() - stoi(MyOldTime);
				}
				catch (...)
				{
					//cout << "APE OF SHIT" << endl;
				}
				ping = calcping();
				
				if ((ForMeCalc_d[0].value == 0 ) || (me.speed.angle != ForMeCalc_d[0].angle))
				{
					me.c.x = ForMeCalc_c[0].x;
					me.c.y = ForMeCalc_c[0].y;
				}
				me.speed.angle = ForMeCalc_d[0].angle;
				me.speed.value = ForMeCalc_d[0].value;
			}
			else
			{
				//cout << recived << endl;

				gui.getkeys(buffer);
				GuiTime = split(recived, "/", 2);
				gotmail = true;
				Framecount.me = 0;
			}
		}
	}
}

void TaskSendData()
{
	while (1)
	{
		packet_num++;
		string msg = "#";
		msg += "/" + to_string(int(gui.c.x)) + "/" + to_string(int(gui.c.y)) + "/" + GuiTime + "/" + to_string(int(gui.speed.angle * 100)) + "/"
			+ to_string(int(gui.speed.value * 100)) + "/" + to_string(packet_num) + "/" + GuiPacketNum + "/" + to_string(gui.dobj_num) + '/';

		for (int i = 0; i < gui.dobj_num; i++)
		{
			msg += to_string(gui.dobj[i].type) + "/"
				+ to_string(int(gui.dobj[i].c.x)) + "/" + to_string(int(gui.dobj[i].c.y)) + "/" 
				+ to_string(int(gui.dobj[i].speed.angle * 100)) + "/" + to_string(int(gui.dobj[i].speed.value * 100)) + "/";
		}

		msg += "$" + to_string(msg.length()) + "&" + "/";
		sendto(connectSocket, msg.c_str(), msg.length(), 0, (sockaddr*)&otherAddr, otherSize);

		gotmail = false;
		gotmailtime = clock();
		Sleep(20);
	}
}

void TaskSendInput()
{
	while (1)
	{
		string msg = ch_tostr(keys, 6);
		msg += "/" + to_string(clock()) + "/";
		msg += "$" + to_string(msg.length()) + "&" + "/";

		sendto(connectSocket, msg.c_str(), msg.length(), 0, (sockaddr*)&otherAddr, otherSize);
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

	initialphysics();

	WindowSetup(1030, 150, 800, 800);
	GraphicsWindow();

	getchar();

	closesocket(connectSocket);
	WSACleanup();
	return 0;
}