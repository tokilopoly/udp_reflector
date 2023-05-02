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
#define MAXLEN 1024*10

int main(int argc, char* argv[]) {
	std::string	port1, port2;
	if (argc == 3)
	{
		port1 = argv[1];
		port2 = argv[2];
		std::cout << "recv from-" << port2 << "-package,sendto-" << port1 << std::endl;
	}
	else {
		printf("xxx.exe port port");
		return 0;
	}

	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd)) {
		std::cout << "WSAStartup Error" << std::endl;
		exit(-1);
	}

	//这是作为服务端的端口
	SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr_in sain;
	sain.sin_family = AF_INET;    //使用IPv4
	sain.sin_addr.s_addr = htonl(INADDR_ANY);
	sain.sin_port = htons(atoi(port1.c_str()));
	if (bind(udpSocket, (sockaddr*)&sain, sizeof(sockaddr)) == -1) {
		std::cout << "绑定失败" << std::endl;
	}
	//开始监听本地端口
	SOCKET localSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr_in sain2;
	sain2.sin_family = AF_INET;    //使用IPv4
	sain2.sin_addr.s_addr = htonl(INADDR_ANY);
	sain2.sin_port = htons(atoi(port2.c_str()));
	if (bind(localSocket, (sockaddr*)&sain2, sizeof(sockaddr)) == -1) {
		std::cout << "绑定失败2" << std::endl;
	}

	//sockaddr fromAddr2;
	//sockaddr fromAddr;
	int fromAddrLen = sizeof(sockaddr);

	std::cout << "wait recv..." << std::endl;
	char buf[1024] = { 0 };
	char* recvBuf = (char*)malloc(MAXLEN);
	char* recvBuf2 = (char*)malloc(MAXLEN);
	if (recvBuf == nullptr || recvBuf2 == nullptr)
	{
		std::cout << "内存分配错误" << std::endl;
		exit(1);
	}
	memset(recvBuf, 0, MAXLEN);
	memset(recvBuf2, 0, MAXLEN);

	/*DWORD	tv_out = 30000;
	setsockopt(udpSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv_out, sizeof(tv_out));
	setsockopt(localSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv_out, sizeof(tv_out));*/
	bool is1 = true, is2 = false;
	char buffer[1024] = { 0 };
	while (1) {
		int len2 = recvfrom(udpSocket, buffer, 1024, 0, (struct sockaddr*)&sain, &fromAddrLen);
		if (len2 > 0)
		{
			printf("udpSocket login\n");
			is2 = true;
		}
		if (is1 && is2)
			break;
	}

	while (true) {
		int len = recvfrom(localSocket, recvBuf, MAXLEN, 0, (struct sockaddr*)&sain2, &fromAddrLen);
		if (len > 0)
		{
			sendto(udpSocket, recvBuf, len, 0, (struct sockaddr*)&sain, sizeof(sain));
		}

		int len2 = recvfrom(udpSocket, recvBuf2, MAXLEN, 0, (struct sockaddr*)&sain, &fromAddrLen);
		if (len2 > 0)
		{
			sendto(localSocket, recvBuf2, len2, 0, (struct sockaddr*)&sain2, sizeof(sain2));
		}
	}

	WSACleanup();
	closesocket(udpSocket);
}