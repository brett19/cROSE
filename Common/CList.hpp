#pragma once

template< typename T >
class CList
{
protected:
	struct Entry
	{
		T data;
		Entry* prevnode;
		Entry* nextnode;
	};

	Entry* firstnode;
	Entry* lastnode;

public:
	class Iter
	{
	friend CList;
	protected:
		Entry* myentry;
		
	public:
		Iter( Entry* startentry )
		{
			myentry = startentry;
		};

		bool isValid( )
		{
			return myentry != 0;
		};

		void operator++( )
		{
			if( myentry == 0 ) DebugBreak( );
			myentry = myentry->nextnode;
		};
		void operator++( int ) { operator++( ); };
		
		void operator--( )
		{
			if( myentry == 0 ) DebugBreak( );
			myentry = myentry->prevnode;
		};
		void operator--( int ) { operator--( ); };

		inline T operator*( )
		{
			if( myentry == 0 ) DebugBreak( );
			return myentry->data;
		};
	};

	CList( )
	{
		firstnode = 0;
		lastnode = 0;
	};

	~CList( )
	{
		Clear( );
	};

	void Clear( )
	{
		Entry* thisnode = firstnode;
		while( thisnode != 0 )
		{
			Entry* nextnode = thisnode->nextnode;
			delete thisnode;
			thisnode = nextnode;
		}
		firstnode = 0;
		lastnode = 0;
	};

	void Push_Back( T data )
	{
		Entry* newnode = new Entry;
		newnode->data = data;

		newnode->nextnode = 0;
		newnode->prevnode = lastnode;

		if( lastnode != 0 )
			lastnode->nextnode = newnode;
		if( firstnode == 0 )
			firstnode = newnode;
		lastnode = newnode;
	};

	void Push_Front( T data )
	{
		Entry* newnode = new Entry( );
		newnode->data = data;

		newnode->nextnode = firstnode;
		newnode->prevnode = 0;

		if( firstnode != 0 )
			firstnode->prevnode = newnode;
		if( lastnode == 0 )
			lastnode = newnode;
		firstnode = newnode;
	};

	T Pop_Back( )
	{
		Entry* thisnode = lastnode;
		lastnode = lastnode->prevnode;
		if( lastnode != 0 )
			lastnode->nextnode = 0;
		if( lastnode == 0 )
			firstnode = 0;
		
		T data = thisnode->data;
		delete thisnode;
		return data;
	};

	T Pop_Front( )
	{
		Entry* thisnode = firstnode;
		firstnode = firstnode->nextnode;
		if( firstnode != 0 )
			firstnode->prevnode = 0;
		if( firstnode == 0 )
			lastnode = 0;
		
		T data = thisnode->data;
		delete thisnode;
		return data;
	};

	void Pop( Iter& iter )
	{
		Entry* myentry = iter.myentry;
		if( myentry->prevnode == 0 )
			firstnode = myentry->nextnode;
		else
			myentry->prevnode->nextnode = myentry->nextnode;
		if( myentry->nextnode == 0 )
			lastnode = myentry->prevnode;
		else
			myentry->nextnode->prevnode = myentry->prevnode;

		iter++;
		delete myentry;
	};

	bool IsEmpty( )
	{
		return firstnode == 0;
	};

	Iter First( )
	{
		return Iter( firstnode );
	};

	Iter Last( )
	{
		return Iter( lastnode );
	};
};