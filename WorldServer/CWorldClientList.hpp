#pragma once
#include "../Common/CIocpServer.hpp"
#include "CWorldSocket.hpp"

class CWorldClientList
	: public CIocpServer
{
public:
	void DisconnectUser( char* accname, int notlsid );

protected:
	CIocpSocket* AllocSocket( );
	void FreeSocket( CIocpSocket* sock );
};

extern CWorldClientList g_ClientList;