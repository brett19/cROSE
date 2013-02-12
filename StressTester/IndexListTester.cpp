#include <iostream>
#include "../Common/CIndexList.hpp"

struct Test
{
	Test( int a1, int a2 ) : m1(a1), m2(a2) { };
	int m1;
	int m2;
};

int main( int argc, char* argv[] )
{
	CIndexList<Test*> test;
	Test* t1 = new Test( 1, 2 );
	test.Append( 2, t1 );
	Test* t2 = new Test( 3, 4 );
	test.Append( 4, t2 );
	Test* t3 = new Test( 5, 6 );
	test.Append( 9, t3 );
	for( CIndexList<Test*>::Iter i = test.First( ); i.isValid( ); i++ )
		printf( "[%d] {%d,%d}\n", (*i).id, (*i).data->m1, (*i).data->m2 );

	getchar( );
	return 0;
};