#pragma once
#include <winsock2.h>
#include "CIndexArray.hpp"
#include "CList.hpp"
#include "CSelectSocket.hpp"
#include "CSelectServerAccepter.hpp"

class CSelectServer
{
public:
	CSelectServer( );
	~CSelectServer( );
	void Start( int port );
	void OnNewConnection( SOCKET sock, char* addr );
	virtual CSelectSocket* AllocSocket( ) = 0;
	virtual void FreeSocket( CSelectSocket* sock ) = 0;

protected:
	CIndexArray< CSelectSocket > m_Clients;
	CList< CSelectServerAccepter* > m_Accepters;
};