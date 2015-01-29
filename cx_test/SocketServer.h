#pragma once
#include <string>
#include <map>
#include <mutex>
#include <winsock.h>

class CSocketServer
{
public:
	CSocketServer();
	virtual ~CSocketServer();
	void InitServer();
	void StopServer();
	void startAcceptThread();
	void sendMessage(SOCKET, std::string);

	virtual bool onClientConnected(SOCKET, SOCKADDR_IN);
	virtual void onMessageReceived(SOCKET, std::string);

	static void AcceptThread(CSocketServer *);
	static void RecvThread(CSocketServer *, SOCKET socket);

private:
	void startRecvThread(SOCKET socket);
	void addClientSocket(SOCKET, SOCKADDR_IN);	
	void deleteClientSocket(SOCKET);
	inline bool getClosed(){ return m_Close; };

private:
	SOCKET	m_serverSocket;
	bool	m_Close;
	std::map<SOCKET, SOCKADDR_IN> m_clientList;
	std::mutex m_mutex;
};