#include "CEvent.hpp"

CEvent::CEvent( )
{
	m_Event = CreateEvent( 0, 1, 0, 0 );
};

CEvent::~CEvent( )
{
	CloseHandle( m_Event );
};

void CEvent::Set( )
{
	SetEvent( m_Event );
};

void CEvent::Reset( )
{
	ResetEvent( m_Event );
};

void CEvent::WaitFor( )
{
	WaitForSingleObject( m_Event, INFINITE );
};