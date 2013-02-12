#include "CIocpServer.hpp"

CIocpServer::CIocpServer( )
{
	m_Iocp.Open( );
};

CIocpServer::~CIocpServer( )
{
	m_Iocp.Close( );
};

void CIocpServer::Start( int port, unsigned int timeout )
{
	m_ClientTimeout = timeout;

	m_ListenSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons( port );
	bind( m_ListenSocket, (sockaddr*)&service, sizeof( service ) );
	listen( m_ListenSocket, 5 );

	for( int i = 0; i < 1; i++ )
	{
		CIocpServerAccepter* newaccepter = new CIocpServerAccepter( this, m_ListenSocket );
		newaccepter->Resume( );
		m_Accepters.Push_Back( newaccepter );
	}

	for( int i = 0; i < 10; i++ )
	{
		CIocpServerWorker* newworker = new CIocpServerWorker( this );
		newworker->Resume( );
		m_Workers.Push_Back( newworker );
	}

	m_Checker = new CIocpServerChecker( this );
	m_Checker->Resume( );
};

void CIocpServer::Stop( )
{
	// Kill Acceptors
	for( CList<CIocpServerAccepter*>::Iter i = m_Accepters.First( ); i.isValid( ); i++ ) {
		(*i)->Terminate( );
	}
	closesocket( m_ListenSocket );
	for( CList<CIocpServerAccepter*>::Iter i = m_Accepters.First( ); i.isValid( ); i++ ) {
		(*i)->WaitFor( );
		delete (*i);
	}

	// Kill Clients
	for( CIndexArray<CIocpSocket>::Iter i = m_Clients.First( ); i.isValid( ); i++ )
	{
		(*i)->CloseSocket( );
	}

	m_Checker->Terminate( );
	m_Checker->WaitFor( );
	delete m_Checker;

	// Kill Workers
	for( CList<CIocpServerWorker*>::Iter i = m_Workers.First( ); i.isValid( ); i++ ) {
		(*i)->Terminate( );
		m_Iocp.Post( 0, 0, 0 );
	}
	for( CList<CIocpServerWorker*>::Iter i = m_Workers.First( ); i.isValid( ); i++ ) {
		(*i)->WaitFor( );
		delete (*i);
	}
	m_Workers.Clear( );
};

HANDLE& CIocpServer::GetIocpHandle( )
{
	return m_Iocp.GetHandle( );
};

bool CIocpServer::CheckClients( )
{
	m_ClientsLock.Enter( );
	int tmpcnt = 0, tmpcnt2 = 0;
	for( CIndexArray<CIocpSocket>::Iter i = m_Clients.First( ); i.isValid( ); i++ )
	{
		CIocpSocket* sock = *i;

		sock->m_Lock.Enter( );

		//* If client should be timed out, print info :)
		if( m_ClientTimeout != 0 && sock->m_LastRecv < GetTickCount( ) - m_ClientTimeout )
			printf( "[OBJ:%d:%s] %d(R:%d,S:%d,D:%d), %d\n", i.id( ), sock->m_Ip, sock->m_RefCount, sock->m_RRefCount, sock->m_SRefCount, sock->m_DRefCount, GetTickCount( ) - sock->m_LastRecv );
		
		//* If client timed out, disconnect
		if( m_ClientTimeout > 0 ) {
			if( sock->m_LastRecv < GetTickCount( ) - m_ClientTimeout )
				sock->CloseSocket( );
		}

		//* If the client has no references, delete it
		if( sock->m_RefCount == 0 ) {
			sock->Disconnected( );
			m_Clients.Remove( i );
			sock->m_Lock.Leave( );
			FreeSocket( sock );

			tmpcnt2++;
		} else {
			sock->m_Lock.Leave( );
		}
		tmpcnt++;
	}
	//printf( "Clients Count :: %d/%d/%d\n", tmpcnt2, tmpcnt, m_Clients.GetUsedCount( ) );
	bool hasobjects = m_Clients.GetUsedCount( ) > 0;
	m_ClientsLock.Leave( );
	return hasobjects;
};

void CIocpServer::OnCompletionTrue( DWORD transfersize, ULONG_PTR key, CIocpIoData* iodata )
{
	CIocpSocket* client = m_Clients[key];
	if( client == 0 ) 
	{
		DebugBreak( );
		return;
	}
	if( transfersize > 0 )
	{
		iodata->bufferlen += transfersize;
		if( iodata->mode == IOMODE_RECV )
		{
			if( iodata->bufferlen == iodata->buffersize )
				client->RecvComplete( iodata );
			else
				client->RecvContinue( iodata );
		} else {
			if( iodata->bufferlen == iodata->buffersize )
				client->SendComplete( iodata );
			else
				client->SendContinue( iodata );
		}
	} else {
		client->CloseSocket( );
		g_IoDataPool.Release( iodata );

		client->m_Lock.Enter( );
		if( iodata->mode == IOMODE_RECV )
			client->m_RRefCount--;
		else
			client->m_SRefCount--;
		client->m_RefCount--;
		client->m_Lock.Leave( );
	}
};

void CIocpServer::OnCompletionFalse( DWORD transfersize, ULONG_PTR key, CIocpIoData* iodata )
{
	iodata->m_Owner->CloseSocket( );
	g_IoDataPool.Release( iodata );

	iodata->m_Owner->m_Lock.Enter( );
	if( iodata->mode == IOMODE_RECV )
		iodata->m_Owner->m_RRefCount--;
	else
		iodata->m_Owner->m_SRefCount--;
	iodata->m_Owner->m_RefCount--;
	iodata->m_Owner->m_Lock.Leave( );
};

void CIocpServer::OnNewConnection( SOCKET sock, char* addr )
{
	CIocpSocket* newclient = AllocSocket( );
	newclient->m_Lock.Enter( );

	newclient->m_Socket = sock;
	strcpy( newclient->m_Ip, addr );

	m_ClientsLock.Enter( );
	int sid = m_Clients.Append( newclient );
	newclient->m_Index = sid;
	m_Iocp.Link( (HANDLE)sock, (void*)sid );
	newclient->Start( );
	m_ClientsLock.Leave( );

	newclient->m_Lock.Leave( );
};