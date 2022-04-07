#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include <iostream>
#include <Winsock2.h>
#include <Windows.h>
#include <wsipv6ok.h>
#include "mysql_conn.h"
#include <locale.h>
#include <string>
#include <cstdio>

#pragma comment(lib, "WS2_32.lib")

using namespace std;

#define myip "192.168.2.212"
#define myport 3070

#define BACKLOG 10

/*
// ����ip�����أ�ֻ����iPv4
void *get_in_addr(struct sockaddr *sa)
{
	return &(((struct sockaddr_in*)sa)->sin_addr);
}
*/




int main(int argc, char* argv[]) {
	//set utf8
	SetConsoleOutputCP(CP_UTF8);

	//setvbuf(stdout, nullptr, _IOFBF, 1000);
	
	//locale::global(locale("zh_CN.UTF-8"));

	//��ʼ��
	WSADATA wsaData;
	WORD wsaVersion = MAKEWORD(2, 0);
	::WSAStartup(wsaVersion, &wsaData);


	//��ʼ��socket,TCP(stream)
	SOCKET ServerSock = socket(AF_INET, SOCK_STREAM, 0);
	//cocket���
	if (ServerSock == INVALID_SET_FILE_POINTER) {
		cout<<u8"socket error"<<endl;
		return 0;
	}

	//bind()������port
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	//addr.sin_addr.S_un.S_addr = inet_addr(myip);������ip
	addr.sin_addr.S_un.S_addr = INADDR_ANY;//����ip
	addr.sin_port = htons(myport);
	//bind���
	if (bind(ServerSock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		cout<<u8"BIND ERROR!"<<endl;
		return 0;
	}

	//listen
	if (listen(ServerSock, BACKLOG) == SOCKET_ERROR) {
		cout << u8"listen error!"<<endl;
		return 0;
	}

	//����
	//connect(clientSock, (sockaddr*)&addr, sizeof(addr));

	//ѭ����������
	SOCKET child;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);


	while (true) {
		char Data[60241] = { 0 };
		char resData[60241] = { 0 };


		cout << u8"�ȴ�����.."<<endl;
		child = accept(ServerSock, (sockaddr *)&remoteAddr, &nAddrlen);

		//chile socket check
		if (child == INVALID_SET_FILE_POINTER) {
			cout << u8"accept error"<<endl;
			continue;
		}
		//print ip
		else {
			cout << "Client: " << inet_ntoa(remoteAddr.sin_addr) << ":" << remoteAddr.sin_port << " is connected" << endl;
		}

		while (1) {

			char revData[1025] = { 0 };//temp_cache
			memset(revData, 0, 1025);

			//receive data
			int ret = -1;
			ret = recv(child, revData, 1024, 0);
			if(ret !=-1)
				strcat_s(Data, revData);

			//�ж�����Ƿ�Ϊ���з�
			if (ret < 1024 && ret !=-1 && Data[strlen(Data)-1]=='\n') {
				//���ݴ���mysql����������
				//cout << Data << endl;

				mysql(Data);
				strcpy_s(resData, "success");
				//cout << Data << endl;

				//��ʼ��
				memset(Data, 0, 60241);
				memset(resData, 0, 60241);
				strcpy_s(Data, "");

				send(child, resData, int(strlen(resData)), 0);
			}

		}

	}

	//�ͷ�SOCKET
	closesocket(ServerSock);
	closesocket(child);

	//�ͷ�
	WSACleanup();


	return 0;
}