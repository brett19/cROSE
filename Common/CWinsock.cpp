#include "CWinsock.hpp"

bool CWinsock::Start( )
{
	WSADATA wsa;
	return WSAStartup( 0x0202, &wsa ) == 0;
};

void CWinsock::Cleanup( )
{
	WSACleanup( );
};