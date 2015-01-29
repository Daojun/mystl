#include "stdafx.h"
#include "SocketServer.h"
#include <iostream>
#include <thread>
#include <string>
#pragma comment(lib, "ws2_32.lib")

const int MAXBUFLEN = 20000;
const u_short PORT = 44965;

CSocketServer::CSocketServer()
{
	m_Close = false;
}

CSocketServer::~CSocketServer()
{
}

void CSocketServer::InitServer()
{
	//--Init win socket
	WSADATA Data;
	int status;
	//socket version 1.1
	status = WSAStartup(MAKEWORD(1, 1), &Data);
	if (status != 0)
		std::cerr << "ERROR: WSAStartup failed code:" << status << std::endl;

	//--Create socket
	/*
	SOCKET WSAAPI socket(
	_In_  int af,
	_In_  int type,
	_In_  int protocol
	);
	*/
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

	//--Write server addr
	SOCKADDR_IN serverSockAddr;
	/*
	short   sin_family;
	u_short sin_port;
	struct  in_addr sin_addr;
	char    sin_zero[8];
	*/
	memset(&serverSockAddr, 0, sizeof(SOCKADDR_IN));
	serverSockAddr.sin_family = AF_INET;
	/*The htons function converts a u_short from host to TCP/IP network byte order (which is big-endian).*/
	serverSockAddr.sin_port = htons(PORT);
	serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//--Bind server addr to server socket
	status = bind(m_serverSocket, (LPSOCKADDR)&serverSockAddr, sizeof(serverSockAddr));
	if (status == SOCKET_ERROR)
		std::cerr << "ERROR: bind socket error" << std::endl;



}

void CSocketServer::StopServer()
{	
	std::unique_lock<std::mutex> lock(m_mutex);
	closesocket(m_serverSocket);
	for (auto x : m_clientList)
	{
		closesocket(x.first);
	}	
	int status = WSACleanup();
	if (status == SOCKET_ERROR)
		std::cerr << "ERROR: WSACleanup unsuccessful" << std::endl;	
}

void CSocketServer::addClientSocket(SOCKET socket, SOCKADDR_IN addr)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_clientList.insert({ socket, addr });
}

void CSocketServer::deleteClientSocket(SOCKET socket)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_clientList.erase(m_clientList.find(socket));
}

void CSocketServer::RecvThread(CSocketServer *pThis, SOCKET socket)
{
	/*WSADATA Data;
	int status;
	//socket version 1.1
	status = WSAStartup(MAKEWORD(1, 1), &Data);
	if (status != 0)
		std::cerr << "ERROR: WSAStartup failed code:" << status << std::endl;*/

	int numrcv;
	char buffer[MAXBUFLEN+1];	
	while (!pThis->getClosed())
	{
		/* recv(已连接的套接字, 接收输入数据缓冲区的指针, 缓冲区大小, 传输控制方式) */
		memset(buffer, 0, MAXBUFLEN);
		std::cout << "Server start waiting for rev message from:" << socket << std::endl;
		numrcv = recv(socket, buffer, MAXBUFLEN, 0);
		
		if ((numrcv == 0) || (numrcv == SOCKET_ERROR))
		{
			//cout << "Connection terminated." << endl;
			pThis->deleteClientSocket(socket);
			int status = closesocket(socket);
			
			if (status == SOCKET_ERROR)
				std::cerr << "ERROR: closesocket unsuccessful" << std::endl;

			return;
			//status = WSACleanup();
			//if (status == SOCKET_ERROR)
				//cerr << "ERROR: WSACleanup unsuccessful" << endl;
			//return(1);
		}
		else
		{
			pThis->onMessageReceived(socket, std::string(buffer));
		}
	} /* while */

	/*status = WSACleanup();
	if (status == SOCKET_ERROR)
		std::cerr << "ERROR: WSACleanup unsuccessful" << std::endl;*/
}

bool CSocketServer::onClientConnected(SOCKET socket, SOCKADDR_IN addr)
{
	std::cout << "socket connected:" << socket << std::endl;
	return true;
}

void CSocketServer::onMessageReceived(SOCKET socket, std::string message)
{
	std::cout << "server <-- "<<socket <<": " <<message<< std::endl;
}

void CSocketServer::AcceptThread(CSocketServer *pThis)
{
	/*WSADATA Data;
	int status;
	//socket version 1.1
	status = WSAStartup(MAKEWORD(1, 1), &Data);
	if (status != 0)
		std::cerr << "ERROR: WSAStartup failed code:" << status << std::endl;*/

	while (!pThis->getClosed())
	{
		/* allow the socket to take connections listen(已建立、尚未连接的套接字号,连接队列的最大长度) */
		int status = listen(pThis->m_serverSocket, 1);
		if (status == SOCKET_ERROR)
			std::cerr << "ERROR: listen unsuccessful" << std::endl;

		//--Accept the client connnection
		SOCKADDR_IN clientSockAddr;
		int addrLen = sizeof(SOCKADDR_IN);
		std::cout << "Server start waiting for accept connnection." << std::endl;
		SOCKET clientSocket = accept(pThis->m_serverSocket, (LPSOCKADDR)&clientSockAddr, &addrLen);
		if (pThis->onClientConnected(clientSocket, clientSockAddr))
		{
			pThis->addClientSocket(clientSocket, clientSockAddr);
			pThis->startRecvThread(clientSocket);
		}
	}

	/*status = WSACleanup();
	if (status == SOCKET_ERROR)
		std::cerr << "ERROR: WSACleanup unsuccessful" << std::endl;*/
}

void CSocketServer::startAcceptThread()
{
	std::thread acceptThread{ CSocketServer::AcceptThread, this };
	acceptThread.detach();
}

void CSocketServer::startRecvThread(SOCKET socket)
{
	std::thread recvThread{ CSocketServer::RecvThread, this, socket};
	recvThread.detach();
}

void CSocketServer::sendMessage(SOCKET socket, std::string message)
{
	int num = send(socket, message.c_str(), message.length() + 1, 0);
	if (num != message.length() + 1)
	{
		std::cerr << "Message send length :"<< num << std::endl;
	}
}