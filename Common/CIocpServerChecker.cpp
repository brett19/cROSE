#include "CIocpServerChecker.hpp"
#include "CIocpServer.hpp"

CIocpServerChecker::CIocpServerChecker( CIocpServer* parent )
	: CThread( false ), m_Parent( parent )
{
};

CIocpServerChecker::~CIocpServerChecker( )
{
};

int CIocpServerChecker::Execute( )
{
	bool working = true;
	while( 1 )
	{
		if( !m_Parent->CheckClients( ) && !IsActive( ) ) break;
		Sleep( 1000 );
	}

	return 0;
};