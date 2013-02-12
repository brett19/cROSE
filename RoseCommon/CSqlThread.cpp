#include "CSqlThread.hpp"

CSqlThread::CSqlThread( )
	: CThread( false )
{
};

CSqlThread::~CSqlThread( )
{
};

void CSqlThread::Stop( )
{
	Terminate( );
	m_RequestPending.Set( );
	WaitFor( );
	//m_Con.Disconnect( );
};

void CSqlThread::Start( char* host, char* user, char* pass )
{
	// Connect
	/*
	try {
		m_Con.Connect( host, user, pass, SA_SQLServer_Client );
	} catch( SAException& e ) {
		printf( "Failed to connct to SQL server :: %s\n", (const char*)e.ErrText( ) );
		DebugBreak( );
	}
	*/
	Resume( );
};

void CSqlThread::PostSqlRequest( CRoseSocket* me, CPacket* pak )
{
	CSqlRequest req = { me, pak };
	m_RequestListLock.Enter( );
	m_RequestList.Push_Back( req );
	m_RequestPending.Set( );
	m_RequestListLock.Leave( );
};

bool CSqlThread::HasControl( )
{
	return GetCurrentThreadId( ) == m_ThreadId;
};

int CSqlThread::Execute( )
{
	CSqlRequest req;

	while( IsActive( ) )
	{
		m_RequestPending.WaitFor( );

		m_RequestListLock.Enter( );
		if( m_RequestList.IsEmpty( ) ) {
			m_RequestListLock.Leave( );
			continue;
		}
		req = m_RequestList.Pop_Front( );
		if( m_RequestList.IsEmpty( ) )
			m_RequestPending.Reset( );
		m_RequestListLock.Leave( );

		req.caller->HandlePacket( req.pak );
		req.caller->FreePacket( req.pak );
		req.caller->m_Lock.Enter( );
		req.caller->m_RefCount--;
		req.caller->m_DRefCount--;
		req.caller->m_Lock.Leave( );
	}
	return 0;
};

CSqlThread g_SqlThread;