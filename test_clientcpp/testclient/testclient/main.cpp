#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include <iostream>
#include <Winsock2.h>
#include <Windows.h>
#include <wsipv6ok.h>

#pragma comment(lib,"ws2_32.lib")
using namespace std;

int main() {
	while(true){
	WSADATA wsaData;
	WORD wsaVersion = MAKEWORD(2, 0);
	WSAStartup(wsaVersion, &wsaData);

	SOCKET ClientSock = socket(AF_INET, SOCK_STREAM, 0);

	//bind()，并绑定port
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	//addr.sin_addr.S_un.S_addr = inet_addr(myip);仅本机ip
	addr.sin_addr.S_un.S_addr = inet_addr("192.168.2.212");
	addr.sin_port = htons(3080);

	connect(ClientSock, (sockaddr*)&addr, sizeof(addr));

	char buf[] = "hi server";
	
	int i = send(ClientSock, buf, strlen(buf), 0);

	while (true) {
		char recvBuf[1024] = "";
		int recvLen = recv(ClientSock, recvBuf, sizeof(recvBuf), 0);
		cout << recvBuf << endl;
		getchar();
	}


	closesocket(ClientSock);
	WSACleanup();

}
	return 0;
}