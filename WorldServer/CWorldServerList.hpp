#pragma once
#include "../Common/CIocpServer.hpp"
#include "CWorldIscSocket.hpp"

class CWorldServerList
	: public CIocpServer
{
public:

protected:
	CIocpSocket* AllocSocket( );
	void FreeSocket( CIocpSocket* sock );
};

extern CWorldServerList g_ServerList;