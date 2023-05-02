#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <stdio.h>
#include <winsock2.h>
#include    <iostream>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll
#define BUF_SIZE 100
int main(int argc, char* argv[]) {
    
    std::string	port;
    if (argc == 2)
    {
        port = argv[1];
        std::cout << "这个程序 在接收到本地" << port <<"端口之后,每隔5s 发送一个im recx_x_port" << port << std::endl;
    }
    else {
        printf("xxx.exe port");
        return 0;
    }


    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //创建套接字
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

    //绑定套接字
    sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));  //每个字节都用0填充
    servAddr.sin_family = PF_INET;  //使用IPv4地址
    //servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    servAddr.sin_addr.s_addr = htonl(0x0); //自动获取IP地址
    servAddr.sin_port = htons(atoi(port.c_str()));  //端口
    bind(sock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR));

    //接收客户端请求
    SOCKADDR clntAddr;  //客户端地址信息
    int nSize = sizeof(SOCKADDR);
    char buffer[BUF_SIZE];  //缓冲区
    int strLen = recvfrom(sock, buffer, BUF_SIZE, 0, &clntAddr, &nSize);
    std::cout << "开始发送im recx_x_portxxx" << std::endl;
    while (1) {
        sendto(sock, "im recx_x_portxxx", 18, 0, &clntAddr, nSize);
        Sleep(5000);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}