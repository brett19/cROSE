#pragma once
//#include <sqlapi.h>
#include "../Common/CWinsock.hpp"
#include "../Common/CThread.hpp"
#include "../Common/CList.hpp"
#include "../Common/CCriticalSection.hpp"
#include "../Common/CEvent.hpp"
#include "../RoseCommon/CRoseSocket.hpp"

struct CSqlRequest
{
	CRoseSocket* caller;
	CPacket* pak;
};

class CSqlThread
	: public CThread
{
public:
	CSqlThread( );
	~CSqlThread( );
	void Start( char* host, char* user, char* pass );
	void Stop( );
	void PostSqlRequest( CRoseSocket* me, CPacket* pak );
	bool HasControl( );
	int Execute( );

	//SAConnection m_Con;

protected:
	CEvent m_RequestPending;
	CCriticalSection m_RequestListLock;
	CList< CSqlRequest > m_RequestList;
};

extern CSqlThread g_SqlThread;
#define GET_SQL_CONTROL() if( !g_SqlThread.HasControl( ) ) { m_Lock.Enter( ); m_DRefCount++; m_RefCount++; m_Lock.Leave( ); CPacket* sqlpak = AllocPacket( ); Packet_Copy( sqlpak, pak ); g_SqlThread.PostSqlRequest( this, sqlpak ); return; }