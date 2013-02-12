#include "CIocpServerWorker.hpp"
#include "CIocpServer.hpp"

CIocpServerWorker::CIocpServerWorker( CIocpServer* parent )
	: CThread( false ), m_Parent( parent )
{
};

CIocpServerWorker::~CIocpServerWorker( )
{
};

int CIocpServerWorker::Execute( )
{
	bool retval;
	DWORD transfersize;
	ULONG_PTR key;
	OVERLAPPED* overlapped;

	while( IsActive( ) )
	{
		retval = GetQueuedCompletionStatus( m_Parent->GetIocpHandle( ), &transfersize, &key, &overlapped, INFINITE ) != 0;
		// Check for shutdown request
		if( overlapped == 0 )
			break;
		// proceed
		if( retval ) {
			m_Parent->OnCompletionTrue( transfersize, key, (CIocpIoData*)overlapped );
		} else {
			m_Parent->OnCompletionFalse( transfersize, key, (CIocpIoData*)overlapped );
		}
	};

	return 0;
};