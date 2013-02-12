#pragma once
#include <windows.h>

class CEvent
{
public:
	CEvent( );
	~CEvent( );
	void Set( );
	void Reset( );
	void WaitFor( );

protected:
	HANDLE m_Event;
};