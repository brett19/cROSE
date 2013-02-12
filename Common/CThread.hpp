#pragma once
#include <windows.h>
#include <process.h>
#include <iostream>

class CThread
{
public:
	CThread( bool autoexec );
	~CThread( );
	void Resume( );
	void Suspend( );
	void Terminate( );
	void WaitFor( );
	bool IsCompleted( );
	bool IsActive( );
	bool IsRunning( );
	virtual int Execute( ) = 0;

protected:
	static unsigned int __stdcall _Execute( void* a );

	bool m_Active;		// Is supposed to keep running
	bool m_Running;		// Is currently resumed
	HANDLE m_Thread;
	DWORD m_ThreadId;
};

//* Expected Execute Function:
/*
int Execute( )
{
	while( IsActive( ) )
	{
		// Do Stuff
	}
	return 0;
};
*/