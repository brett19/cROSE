#include "CLoginClientList.hpp"

CIocpSocket* CLoginClientList::AllocSocket( )
{
	CIocpSocket* sock = (CIocpSocket*)g_LoginSocketDataPool.Acquire( );
	sock->Init( );
	return sock;
};

void CLoginClientList::FreeSocket( CIocpSocket* sock )
{
	sock->Destroy( );
	g_LoginSocketDataPool.Release( (CLoginSocket*)sock );
};

void CLoginClientList::DisconnectUser( char* accname, int notlsid )
{
	m_ClientsLock.Enter( );
	for( CIndexArray<CIocpSocket>::Iter i = m_Clients.First( ); i.isValid( ); i++ )
	{
		CLoginSocket* acc = (CLoginSocket*)*i;
		if( _stricmp( acc->m_AccName, accname ) == 0 && acc->m_Lsid != notlsid )
		{
			acc->CloseSocket( );
			break;
		}
	}
	m_ClientsLock.Leave( );
};

CLoginClientList g_ClientList;