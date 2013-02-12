#pragma once
#include <winsock2.h>
#include "CThread.hpp"

class CSelectServer;

class CSelectServerAccepter
	: public CThread
{
public:
	CSelectServerAccepter( CSelectServer* parent );
	~CSelectServerAccepter( );
	int Execute( );
	void Init( int listenport );

protected:
	CSelectServer* m_Parent;
	SOCKET m_Socket;
};