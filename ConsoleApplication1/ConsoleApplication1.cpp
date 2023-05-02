#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<string>
#include<WS2tcpip.h>
#include <WinSock2.h>
#include<Windows.h>
#include	<thread>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
#define MAXLEN 4096


int main(int argc,char *argv[]) {
	std::string	target_ip;
	std::string	port;
	if (argc==3)
	{
		target_ip=argv[1];
		std::cout << "target_ip-" << target_ip << std::endl;

		port = argv[2];
		std::cout << "port" << port << std::endl;
	}else {
		printf("xxx.exe target_ip port");
		return 0;
	}

	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd)) {
		std::cout << "WSAStartup Error" << std::endl;
		exit(-1);
	}
	//这是向服务器程序发送包
	SOCKET serviceSocket = {};
	serviceSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if (SOCKET_ERROR == serviceSocket) {
		std::cout << "套接字创建失败!" << std::endl;
	}
	else {
		std::cout << "套接字创建成功!" << std::endl;
	}
	//	3	绑定套接字	指定绑定的IP地址和端口号
	sockaddr_in socketAddr;
	socketAddr.sin_family = PF_INET;
	socketAddr.sin_addr.S_un.S_addr = inet_addr(target_ip.c_str());
	socketAddr.sin_port = htons(atoi(port.c_str()));


	DWORD	tv_out = 1;
	setsockopt(serviceSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv_out, sizeof(tv_out));
	
	char recvBuf[MAXLEN] = {};
	int sock_addr_len = sizeof(socketAddr);
	std::string  request = "send speed 1s/item---";
	int len = 0;
	
	do
	{
		//开始转发
		std::thread send{ [&] {
				sendto(serviceSocket, request.c_str(), request.length() + 1, 0, (struct sockaddr*)&socketAddr, sizeof(socketAddr));
			std::cout << "sendto-content:" << request << std::endl;
			Sleep(2000);
			} };
		send.join();
		std::thread recv{ [&] {
			int len = recvfrom(serviceSocket, recvBuf, MAXLEN, 0, (struct sockaddr*)&socketAddr, &sock_addr_len);
		if (len > 0)
		 std::cout << "recv- " << recvBuf << std::endl;
}
		};
		recv.join();
	} while (1);
	
	WSACleanup();
	closesocket(serviceSocket);
}