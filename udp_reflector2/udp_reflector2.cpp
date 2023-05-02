#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<string>
#include<WS2tcpip.h>
#include <WinSock2.h>
#include<Windows.h>
#include	<mutex>

#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll

using namespace std;

//这里是内网机器1的客户端 源码
//内网机器1的作用是
//连接公网服务器 , 如果公网服务器发来包了 就原封不到 转发给内网机器2

#define MAXLEN 1024*10

int main(int argc, char* argv[])
{
	string	forwading_ip;
	string	forwading_port;
	string	service_port;
	USHORT	port1_local = 0;
	USHORT	port2_local = 0;

	if (argc == 4)
	{
		service_port = argv[1];
		forwading_ip = argv[2];
		forwading_port = argv[3];
		port1_local = atoi(service_port.c_str());
		port2_local = atoi(forwading_port.c_str());

		std::cout << "监听本地  " << service_port << std::endl;
		std::cout << "转发到_ip  " << forwading_ip << std::endl;
		std::cout << "转发到_port  " << forwading_port << std::endl;
	}
	else {
		printf("xxx.exe port forwading_ip port\n");
		printf("xxx.exe 1234 192.168.145.130 2389\n");
		printf("监听本地1234 转发至 192.168.145.130 2389\n");
		return 0;
	}
	//	1	初始化
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	cout << "-----------内网机器1-----------" << endl;

	//服务器通信socket----------------------------------------
	auto serviceSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SOCKET_ERROR == serviceSocket) {
		cout << "套接字创建失败!" << endl;
	}
	sockaddr_in socketAddr;
	socketAddr.sin_family = PF_INET;
	socketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	socketAddr.sin_port = htons(port1_local);
	if (bind(serviceSocket, (sockaddr*)&socketAddr, sizeof(sockaddr)) == -1) {
		std::cout << "绑定失败" << std::endl;
	}
	//转发用 socket----------------------------------------
	auto forwading_client = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SOCKET_ERROR == forwading_client) {
		cout << "套接字2创建失败!" << endl;
	}
	sockaddr_in socketAddr2;
	socketAddr2.sin_family = PF_INET;
	socketAddr2.sin_addr.S_un.S_addr = inet_addr(forwading_ip.c_str());
	socketAddr2.sin_port = htons(port2_local);

	int sock_addr_len = sizeof(socketAddr);

	char* recvBuf = (char*)malloc(MAXLEN);
	char* recvBuf2 = (char*)malloc(MAXLEN);
	if (recvBuf == nullptr || recvBuf2 == nullptr)
	{
		std::cout << "内存分配错误" << std::endl;
		exit(1);
	}
	memset(recvBuf, 0, MAXLEN);
	memset(recvBuf2, 0, MAXLEN);

	//开始转发
	while (1) {
		int sock_addr_len = sizeof(socketAddr);
		int len = recvfrom(serviceSocket, recvBuf, MAXLEN, 0, (struct sockaddr*)&socketAddr, &sock_addr_len);
		if (len > 0)
		{
			sendto(forwading_client, recvBuf, len, 0, (struct sockaddr*)&socketAddr2, sizeof(socketAddr2));
		}
		int len2 = recvfrom(forwading_client, recvBuf2, MAXLEN, 0, (struct sockaddr*)&socketAddr2, &sock_addr_len);
		if (len2 > 0)
		{
			sendto(serviceSocket, recvBuf2, len2, 0, (struct sockaddr*)&socketAddr, sizeof(socketAddr));
		}
	}
	closesocket(serviceSocket);
	WSACleanup();
	return 0;
}
