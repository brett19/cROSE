#pragma once
#include <windows.h>

class CCriticalSection
{
public:
	CCriticalSection( );
	~CCriticalSection( );
	void Enter( );
	bool TryEnter( );
	void Leave( );

protected:
	CRITICAL_SECTION m_Cs;
};

template< typename T >
class CCsProtected
	: public CCriticalSection
{
public:
	inline T& operator*( )
	{
		return m_ProtectedObject;
	};

protected:
	T m_ProtectedObject;
};