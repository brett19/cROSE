#include "CIocp.hpp"

CIocp::CIocp( )
{
	m_Cp = 0;
};

CIocp::~CIocp( )
{
	Close( );
};

bool CIocp::Open( int thrdcnt )
{
	m_Cp = CreateIoCompletionPort( INVALID_HANDLE_VALUE, 0, 0, thrdcnt );
	return m_Cp != 0;
};

void CIocp::Close( )
{
	if( m_Cp != 0 ) {
		CloseHandle( m_Cp );
		m_Cp = 0;
	}
};

bool CIocp::Link( HANDLE handle, void* key )
{
	return CreateIoCompletionPort( handle, m_Cp, (ULONG_PTR)key, 0 ) != 0;
};

bool CIocp::Post( DWORD numberofbytes, void* key, LPOVERLAPPED overlapped )
{
	return PostQueuedCompletionStatus( m_Cp, numberofbytes, (ULONG_PTR)key, overlapped ) != 0;
};

HANDLE& CIocp::GetHandle( )
{
	return m_Cp;
};