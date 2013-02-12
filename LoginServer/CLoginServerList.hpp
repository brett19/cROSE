#pragma once
#include "../Common/CIocpServer.hpp"
#include "CLoginIscSocket.hpp"

class CLoginServerList
	: public CIocpServer
{
public:

protected:
	CIocpSocket* AllocSocket( );
	void FreeSocket( CIocpSocket* sock );
};

extern CLoginServerList g_ServerList;