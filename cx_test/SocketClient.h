#pragma once
#include <winsock.h>
#include <string>
class CSocketClient
{
public:
	CSocketClient();
	virtual ~CSocketClient();
	void InitClient();
	SOCKET ConnectToServer();
	void Disconnect(SOCKET);
	int SendMessage(SOCKET socket, std::string message);
	bool PostMessage();
};

