#include "CSelectServerAccepter.hpp"
#include "CSelectServer.hpp"

CSelectServerAccepter::CSelectServerAccepter( CSelectServer* parent )
	: CThread( false ), m_Parent( parent )
{
};

CSelectServerAccepter::~CSelectServerAccepter( )
{
};

void CSelectServerAccepter::Init( int listenport )
{
	m_Socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons( listenport );
	bind( m_Socket, (sockaddr*)&service, sizeof( service ) );
	listen( m_Socket, SOMAXCONN );
};

int CSelectServerAccepter::Execute( )
{
	SOCKET newsock;
	sockaddr_in addr;
	int addr_len = sizeof(addr);

	while( IsActive( ) )
	{
		newsock = WSAAccept( m_Socket, (sockaddr*)&addr, &addr_len, 0, 0 );
		m_Parent->OnNewConnection( newsock, inet_ntoa( addr.sin_addr ) );
	}

	return 0;
};