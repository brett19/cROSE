#include "CCriticalSection.hpp"

CCriticalSection::CCriticalSection( )
{
	InitializeCriticalSection( &m_Cs );
};

CCriticalSection::~CCriticalSection( )
{
	DeleteCriticalSection( &m_Cs );
};

bool CCriticalSection::TryEnter( )
{
	return TryEnterCriticalSection( &m_Cs ) == 0;
};

void CCriticalSection::Enter( )
{
	EnterCriticalSection( &m_Cs );
};

void CCriticalSection::Leave( )
{
	LeaveCriticalSection( &m_Cs );
};