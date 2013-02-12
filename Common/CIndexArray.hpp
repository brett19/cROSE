#pragma once

const int IA_DEFAULT_AMOUNT = 50;
const int IA_INC_AMOUNT = 15;

template< typename T >
class CIndexArray
{
public:
	class Iter
	{
	friend CIndexArray;
	protected:
		CIndexArray* myarr;
		int myentry;
		
	public:
		Iter( CIndexArray* arr, int startentry )
			: myarr( arr ), myentry( startentry )
		{
		};

		bool isValid( )
		{
			return myentry >= 0 && myentry < myarr->m_Size;
		};

		int id( )
		{
			return myentry;
		};

		void operator++( )
		{
			myentry++;
			while( myentry < myarr->m_Size && myarr->m_Data[myentry] == 0 )
				myentry++;
		};
		void operator++( int ) { operator++( ); };
		
		void operator--( )
		{
			myentry--;
			while( myentry >= 0 && myarr->m_Data[myentry] == 0 )
				myentry--;
		};
		void operator--( int ) { operator--( ); };

		inline T*& operator*( )
		{
			return myarr->m_Data[ myentry ];
		};
	};

	CIndexArray( )
		: m_Data( 0 ), m_Size( 0 ), m_UsedCnt( 0 )
	{
		Resize( IA_DEFAULT_AMOUNT );
	};

	CIndexArray( int startsize )
		: m_Data( 0 ), m_Size( 0 ), m_UsedCnt( 0 )
	{
		Resize( startsize );
	};

	~CIndexArray( )
	{
		Clear( );
	};

	int Append( T* data )
	{
		int newslot = GetEmptySlot( );
		m_Data[ newslot ] = data;
		m_UsedCnt++;
		return newslot;
	};

	void Remove( int pos )
	{
		m_Data[ pos ] = 0;
		m_UsedCnt--;
	};

	void Remove( Iter pos )
	{
		Remove( pos.myentry );
	};

	void Clear( )
	{
		if( m_Data != 0 )
			delete[] m_Data;
		m_Data = 0;
		m_Size = 0;
		m_UsedCnt = 0;
	};

	int GetEmptySlot( )
	{
		for( int i = 0; i < m_Size; i++ )
		{
			if( m_Data[i] == 0 )
				return i;
		}

		int oldsize = m_Size;
		Resize( m_Size + IA_INC_AMOUNT );
		return oldsize;
	};

	int GetUsedCount( )
	{
		return m_UsedCnt;
	};

	void Resize( int size )
	{
		if( m_Data == 0 ) {
			m_Data = new T*[ size ];
		} else {
			T** tmp = m_Data;
			m_Data = new T*[ size ];
			memcpy( m_Data, tmp, sizeof(T*) * m_Size );
			delete[] tmp;
		}
		for( int i = m_Size; i < size; i++ )
			m_Data[i] = 0;
		m_Size = size;
	};

	inline T* operator[]( int index )
	{
		if( index < 0 || index >= m_Size )
			return 0;
		return m_Data[ index ];
	};

	inline Iter First( )
	{
		for( int i = 0; i < m_Size; i++ )
		{
			if( m_Data[i] != 0 )
				return Iter( this, i );
		}
		return Iter( this, -1 );
	};

protected:
	T** m_Data;
	int m_Size;
	int m_UsedCnt;
};