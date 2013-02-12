#include "CLoginServerList.hpp"

CIocpSocket* CLoginServerList::AllocSocket( )
{
	CIocpSocket* sock = (CIocpSocket*)( new CLoginIscSocket( ) );
	sock->Init( );
	return sock;
};

void CLoginServerList::FreeSocket( CIocpSocket* sock )
{
	sock->Destroy( );
	delete (CLoginIscSocket*)sock;
};

CLoginServerList g_ServerList;