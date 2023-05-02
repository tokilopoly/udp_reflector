#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<string>
#include<WS2tcpip.h>
#include <WinSock2.h>
#include<Windows.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
#include	<thread>
using namespace std;

//这里是内网机器1的客户端 源码
//内网机器1的作用是
//连接公网服务器 , 如果公网服务器发来包了 就原封不到 转发给内网机器2

#define MAXLEN 1024*10
int main(int argc, char* argv[])
{
	std::string	forwading_ip_port;
	std::string	service_ip_port;

	string	forwading_ip;
	USHORT	forwading_port;

	string	service_ip;
	USHORT	service_port;

	if (argc == 3)
	{
		service_ip_port = argv[1];
		forwading_ip_port = argv[2];

		forwading_ip = forwading_ip_port.substr(forwading_ip_port.find("-") + 1, forwading_ip_port.find(":") - 1);
		forwading_port = atoi(forwading_ip_port.substr(forwading_ip_port.find(":") + 1, forwading_ip_port.length() - forwading_ip_port.find(":")).c_str());

		service_ip = service_ip_port.substr(service_ip_port.find("-") + 1, service_ip_port.find(":") - 1);
		service_port = atoi(service_ip_port.substr(service_ip_port.find(":") + 1, service_ip_port.length() - service_ip_port.find(":")).c_str());

		std::cout << "service_ip  " << service_ip << std::endl;
		std::cout << "service_port  " << service_port << std::endl;
		std::cout << "转发到_ip  " << forwading_ip << std::endl;
		std::cout << "转发到_port  " << forwading_port << std::endl;
	}
	else {
		printf("xxx.exe -server_ip:port -forwading_ip:port");
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
	socketAddr.sin_addr.S_un.S_addr = inet_addr(service_ip.c_str());
	socketAddr.sin_port = htons(service_port);

	//转发用 socket----------------------------------------
	auto forwading_client = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SOCKET_ERROR == forwading_client) {
		cout << "套接字2创建失败!" << endl;
	}
	sockaddr_in socketAddr2;
	socketAddr2.sin_family = PF_INET;
	socketAddr2.sin_addr.S_un.S_addr = inet_addr(forwading_ip.c_str());
	socketAddr2.sin_port = htons(forwading_port);

	char* recvBuf = (char*)malloc(MAXLEN);
	char* recvBuf2 = (char*)malloc(MAXLEN);
	if (recvBuf == nullptr || recvBuf2 == nullptr)
	{
		std::cout << "内存分配错误" << std::endl;
		exit(1);
	}
	memset(recvBuf, 0, MAXLEN);
	memset(recvBuf2, 0, MAXLEN);

	int sock_addr_len = sizeof(socketAddr);

	/*DWORD	tv_out = 60;
	setsockopt(forwading_client, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv_out, sizeof(tv_out));
	setsockopt(serviceSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv_out, sizeof(tv_out));*/

	char request[] = "hello"; //先通知服务器 内网机器1 上线了
	sendto(serviceSocket, request, 6, 0, (struct sockaddr*)&socketAddr, sizeof(socketAddr));
	////开始转发
	while (1) {
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
	std::cout << "end" << endl;
	closesocket(serviceSocket);
	closesocket(forwading_client);
	WSACleanup();
	return 0;
}

