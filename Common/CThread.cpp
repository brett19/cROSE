#include "CThread.hpp"

CThread::CThread( bool autoexec )
{
	m_Active = true;
	m_Running = false;
	m_Thread = (HANDLE)_beginthreadex( 0, 0, &_Execute, this, CREATE_SUSPENDED, 0 );
	if( autoexec )
		Resume( );
};

CThread::~CThread( )
{
	Terminate( );
	if( m_Thread != NULL )
		CloseHandle( m_Thread );
};

void CThread::Resume( )
{
	m_Running = true;
	ResumeThread( m_Thread );
};

void CThread::Suspend( )
{
	m_Running = false;
	SuspendThread( m_Thread );
};

void CThread::Terminate( )
{
	m_Active = false;
};

void CThread::WaitFor( )
{
	WaitForSingleObject( m_Thread, INFINITE );
};

bool CThread::IsCompleted( )
{
	return WaitForSingleObject( m_Thread, 0 ) != WAIT_TIMEOUT;
};

bool CThread::IsActive( )
{
	return m_Active;
};

bool CThread::IsRunning( )
{
	return m_Running;
};

unsigned int CThread::_Execute( void* a )
{
	((CThread*)a)->m_ThreadId = GetCurrentThreadId( );
	return ((CThread*)a)->Execute( );
};