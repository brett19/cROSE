#pragma once
#include <winsock2.h>
#include "CThread.hpp"

class CIocpServer;

class CIocpServerChecker
	: public CThread
{
public:
	CIocpServerChecker( CIocpServer* parent );
	~CIocpServerChecker( );
	int Execute( );

protected:
	CIocpServer* m_Parent;
};