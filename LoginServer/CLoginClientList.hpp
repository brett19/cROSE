#pragma once
#include "../Common/CIocpServer.hpp"
#include "CLoginSocket.hpp"

class CLoginClientList
	: public CIocpServer
{
public:
	void DisconnectUser( char* accname, int notlsid );

protected:
	CIocpSocket* AllocSocket( );
	void FreeSocket( CIocpSocket* sock );
};

extern CLoginClientList g_ClientList;