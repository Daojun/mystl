#pragma comment(lib, "ws2_32.lib")

#include "stdafx.h"
#include <iostream>
#include "SocketClient.h"

#define NO_FLAGS_SET 0
#define PORT (u_short)44965
#define DEST_IP_ADDR "10.80.45.150"


CSocketClient::CSocketClient()
{
	
}

CSocketClient::~CSocketClient()
{
	WSACleanup();
}

void CSocketClient::InitClient()
{
	WSADATA Data;
	WSAStartup(MAKEWORD(1, 1), &Data);
}

SOCKET CSocketClient::ConnectToServer()
{
	SOCKADDR_IN destSockAddr;
	SOCKET destSocket;

	unsigned long destIP = inet_addr(DEST_IP_ADDR);

	memcpy(&destSockAddr.sin_addr, &destIP, sizeof(destIP));
	destSockAddr.sin_port = htons(PORT);
	destSockAddr.sin_family = AF_INET;

	destSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (destSocket == INVALID_SOCKET)
	{
		return 0;
	}

	int status = connect(destSocket, (LPSOCKADDR)&destSockAddr, sizeof(destSockAddr));
	if (status == SOCKET_ERROR)
	{
		closesocket(destSocket);
		return 0;
	}
	return destSocket;
}

void CSocketClient::Disconnect(SOCKET socket)
{
	if (socket)
	{
		closesocket(socket);
		socket = 0;
	}
}

int CSocketClient::SendMessage(SOCKET socket, std::string message)
{
	int n = send(socket, message.c_str(), message.length() + 1, NO_FLAGS_SET);
	if (n == 0)
	{
		std::cout << "Send message fault:"  << std::endl;
		std::cout << message << std::endl;
	}
	else
	{
		std::cout << "client --> " << socket << ": " << message << std::endl;
	}
	return n;
}

bool CSocketClient::PostMessage()
{
	return true;
}