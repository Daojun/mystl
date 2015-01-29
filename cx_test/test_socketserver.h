#ifndef TEST_SOCKET_SERVER_H
#define TEST_SOCKET_SERVER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <regex>
#include "testcase.h"
#include "SocketServer.h"
#include "SocketClient.h"
using namespace std;

class CTestSocketServer : public CTestCase, public CSocketServer
{
public:
	//Test Name
	virtual string getCaseName()
	{
		return __FILE__;
	}
	//Run testcases
	virtual bool excute()
	{
		InitServer();
		startAcceptThread();
		std::cout << "SocketServer_Test case started" << std::endl;
		Sleep(1000);
		CSocketClient client;
		client.InitClient();
		SOCKET socket = client.ConnectToServer();		
		client.SendMessage(socket, std::string{ "Hello, I'm cllient\n" });
		Sleep(1000*500);
		std::cout << "SocketServer_Test case ended" << std::endl;
		StopServer();
		return true;
	}

	virtual bool onClientConnected(SOCKET socket, SOCKADDR_IN addr)
	{
		return CSocketServer::onClientConnected(socket, addr);
	}

	virtual void onMessageReceived(SOCKET socket, std::string message)
	{
		return CSocketServer::onMessageReceived(socket, message);
	}

	CTestSocketServer()
	{

	};

	virtual ~CTestSocketServer()
	{

	};
};

#endif
