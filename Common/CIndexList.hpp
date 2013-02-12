#pragma once
#include "CList.hpp"

template< typename T >
struct CIndexListEntry
{
	int id;
	T data;
};

template< typename T >
class CIndexList
	: public CList<CIndexListEntry<T>>
{
public:
	void Append( int id, T data )
	{
		CIndexListEntry<T> newentry = { id, data };
		Push_Back( newentry );
	};

	T Find( int id )
	{
		for( Iter i = First( ); i.isValid( ); i++ )
		{
			if( (*i).id == id )
				return (*i).data;
		}
		return 0;
	};

	T Remove( int id )
	{
		for( Iter i = First( ); i.isValid( ); i++ )
		{
			if( (*i).id == id ) {
				T val = (*i).data;
				Pop( i );
				return val;
			}
		}
		return 0;
	};
};