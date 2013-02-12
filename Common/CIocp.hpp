#pragma once
#include <windows.h>

class CIocp
{
public:
	CIocp( );
	~CIocp( );
	bool Open( int thrdcnt = 0 );
	void Close( );
	bool Link( HANDLE handle, void* key );
	bool Post( DWORD numberofbytes, void* key, LPOVERLAPPED overlapped );
	HANDLE& GetHandle( );

protected:
	HANDLE m_Cp;
};