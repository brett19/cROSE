#pragma once
#include <iostream>
#include "CList.hpp"
#include "CCriticalSection.hpp"

const int DP_DEFAULT_AMOUNT = 100;
const int DP_INC_AMOUNT = 20;

template< typename T >
class CDataPool
{
public:
	CDataPool( )
		: m_DataCount( 0 ), m_UsedCount( 0 )
	{
		Alloc( DP_DEFAULT_AMOUNT );
	};

	CDataPool( int startamount )
		: m_DataCount( 0 ), m_UsedCount( 0 )
	{
		Alloc( startamount );
	};

	~CDataPool( )
	{
		Clear( );
	};

	void Clear( )
	{
		m_Cs.Enter( );

		int clearcount = 0;
		for( CList<T*>::Iter i = m_Data.First( ); i.isValid( ); i++ ) {
			delete *i;
			clearcount++;
		}
		m_Data.Clear( );

		if( m_DataCount > clearcount )
		{
			// Some data wasent returned
			DebugBreak( );
		}
		else if( m_DataCount < clearcount )
		{
			// Too many objects, wtf? dual-commit?
			DebugBreak( );
		}
		m_DataCount = 0;

		m_Cs.Leave( );
	};

	bool Alloc( int amount )
	{
		m_Cs.Enter( );

		for( int i = m_DataCount; i < amount; i++ )
		{
			T* newentry = new T( );
			m_Data.Push_Back( newentry );
		}
		m_DataCount = amount;

		m_Cs.Leave( );

		return true;
	};

	T* Acquire( )
	{
		m_Cs.Enter( );

		CList<T*>::Iter i = m_Data.First( );
		if( !i.isValid( ) )
			Alloc( m_DataCount + DP_INC_AMOUNT );
		T* retval = m_Data.Pop_Front( );
		m_Cs.Leave( );
		m_UsedCount++;

		return retval;
	};

	void Release( T* value )
	{
		m_Cs.Enter( );
		m_Data.Push_Back( value );
		m_Cs.Leave( );
		m_UsedCount--;
	};

protected:
	CList<T*> m_Data;
	int m_UsedCount;
	int m_DataCount;
	CCriticalSection m_Cs;
};