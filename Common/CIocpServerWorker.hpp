#pragma once
#include "CWinsock.hpp"
#include "CThread.hpp"

class CIocpServer;

class CIocpServerWorker
	: public CThread
{
public:
	CIocpServerWorker( CIocpServer* parent );
	~CIocpServerWorker( );
	int Execute( );

protected:
	CIocpServer* m_Parent;
};