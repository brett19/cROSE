#pragma once
#include <winsock2.h>
#include "CThread.hpp"

class CIocpServer;

class CIocpServerAccepter
	: public CThread
{
public:
	CIocpServerAccepter( CIocpServer* parent, SOCKET listensocket );
	~CIocpServerAccepter( );
	int Execute( );

protected:
	CIocpServer* m_Parent;
	SOCKET m_Socket;
};