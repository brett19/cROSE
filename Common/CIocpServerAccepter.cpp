#include "CIocpServerAccepter.hpp"
#include "CIocpServer.hpp"

CIocpServerAccepter::CIocpServerAccepter( CIocpServer* parent, SOCKET listensocket )
	: CThread( false ), m_Parent( parent ), m_Socket( listensocket )
{
};

CIocpServerAccepter::~CIocpServerAccepter( )
{
};

int CIocpServerAccepter::Execute( )
{
	SOCKET newsock;
	sockaddr_in addr;
	int addr_len = sizeof(addr);

	while( IsActive( ) )
	{
		newsock = WSAAccept( m_Socket, (sockaddr*)&addr, &addr_len, 0, 0 );
		if( newsock != INVALID_SOCKET )
		{
			m_Parent->OnNewConnection( newsock, inet_ntoa( addr.sin_addr ) );
		} else {
			if( IsActive( ) )
			{
				printf( "Accept call failed: %d\n", WSAGetLastError( ) );
			}
		}
	}

	return 0;
};