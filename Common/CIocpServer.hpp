#pragma once
#include <winsock2.h>
#include "CIndexArray.hpp"
#include "CList.hpp"
#include "CCriticalSection.hpp"
#include "CIocp.hpp"
#include "CIocpSocket.hpp"
#include "CIocpServerAccepter.hpp"
#include "CIocpServerWorker.hpp"
#include "CIocpServerChecker.hpp"

class CIocpServer
{
public:
	CIocpServer( );
	~CIocpServer( );
	void Start( int port, unsigned int timeout );
	void Stop( );
	HANDLE& GetIocpHandle( );
	void OnCompletionTrue( DWORD transfersize, ULONG_PTR key, CIocpIoData* iodata );
	void OnCompletionFalse( DWORD transfersize, ULONG_PTR key, CIocpIoData* iodata );
	void OnNewConnection( SOCKET sock, char* addr );
	bool CheckClients( );
	virtual CIocpSocket* AllocSocket( ) = 0;
	virtual void FreeSocket( CIocpSocket* sock ) = 0;
	
	CIocp m_Iocp;
	SOCKET m_ListenSocket;
	CCriticalSection m_ClientsLock;
	CIocpServerChecker* m_Checker;
	CIndexArray< CIocpSocket > m_Clients;
	CList< CIocpServerAccepter* > m_Accepters;
	CList< CIocpServerWorker* > m_Workers;
	unsigned int m_ClientTimeout;
};