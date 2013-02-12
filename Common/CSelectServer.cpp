#include "CSelectServer.hpp"

CSelectServer::CSelectServer( )
{
};

CSelectServer::~CSelectServer( )
{
};

void CSelectServer::Start( int port )
{
	for( int i = 0; i < 1; i++ )
	{
		CSelectServerAccepter* newaccepter = new CSelectServerAccepter( this );
		newaccepter->Init( port );
		newaccepter->Resume( );
		m_Accepters.Push_Back( newaccepter );
	}
};

void CSelectServer::OnNewConnection( SOCKET sock, char* addr )
{
	printf( "New Client Connected (%s)\n", addr );
};