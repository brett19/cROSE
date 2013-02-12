#include "CLoginSocket.hpp"

CDataPool< CLoginSocket > g_LoginSocketDataPool;

void CLoginSocket::Init( )
{
	CRoseSocket::Init( );

	strcpy( m_AccName, "" );
	strcpy( m_Pass, "" );
	m_Right = 0;
	m_State = eLSSTATE_DEFAULT;
};

void CLoginSocket::HandleDisconnect( )
{
	if( m_State == eLSSTATE_LOGGEDIN )
	{
		int acccnt = g_AccountList.GetRealCount( );

		CLoginAccount* acc = g_AccountList.FindAccountAndRemove( m_AccName );
		if( acc != 0 ) g_LoginAccountDataPool.Release( acc );

		int acccnt2 = g_AccountList.GetRealCount( );
		printf( "Acc Local DC :: '%s' (T:%d/%d)\n", m_AccName, acccnt, acccnt2 );
	}
};

void CLoginSocket::HandlePacket( CPacket* pak )
{
	switch( pak->m_Command )
	{
		case 0x703: PakCryptRequest( pak ); break;
		case 0x501: PakLoginRequest( pak ); break;
		case 0x704: PakChannelRequest( pak ); break;
		case 0x70a: PakServerRequest( pak ); break;
	}
};

void CLoginSocket::PakCryptRequest( CPacket* pak )
{
	if( m_State != eLSSTATE_DEFAULT )
		return;

	CPacket* pakout = AllocPacket( );
	Packet_Start( pakout, 0x23, 0x7ff );
	Packet_SetByte( pakout, 0, 2 );
	Packet_SetDword( pakout, 1, timeGetTime( ) );
	SendPacket( pakout );
};

void CLoginSocket::PakLoginRequest( CPacket* pak )
{
	if( m_State != eLSSTATE_DEFAULT )
		return;

	GET_SQL_CONTROL( );

	Packet_GetBytes( pak, 0, 32, m_Pass ); m_Pass[32] = 0;
	Packet_GetString( pak, 32, m_AccName );

	/*
	SACommand cmd( &g_SqlThread.m_Con, "EXEC seven_ORA..UserAuthenticate :1" );
	try {
		cmd << m_AccName;
		cmd.Execute( );
	} catch( ... ) {
		SendLoginReply( 1 );
		return;
	}
	if( !cmd.FetchNext( ) ) 
	{
		SendLoginReply( 2 );
		return;
	}
	if( cmd["MD5Password"].asString( ).Compare( m_Pass ) != 0 )
	{
		SendLoginReply( 3 );
		return;
	}
	if( cmd["MailIsConfirm"].asULong( ) < 1 )
	{
		SendLoginReply( 9 );
		return;
	}
	if( !cmd["BlockStart"].isNull( ) )
	{
		SendLoginReply( 5 );
		return;
	}
	if( cmd["Right"].asULong( ) < 1 )
	{
		SendLoginReply( 7 );
		return;
	}
	CLoginAccount* oldacc = g_AccountList.FindAccount( m_AccName );
	if( oldacc != 0 )
	{
		// Kick the bastard!
		if( oldacc->m_State == eACCSTATE_TRANSFERING )
		{
			oldacc = g_AccountList.FindAccountAndRemove( m_AccName );
			
			int acccnt = g_AccountList.GetRealCount( );
			printf( "Dbl Acc Login :: '%s','%s' (T:%d)\n", m_AccName, oldacc->m_AccName, acccnt );
					
			if( oldacc != 0 ) g_LoginAccountDataPool.Release( oldacc );
		}
		else 
		{
			if( oldacc->m_State == eACCSTATE_DEFAULT ) {
				g_ClientList.DisconnectUser( m_AccName, m_Lsid );
			} else {
				CLoginIscSocket* sock = (CLoginIscSocket*)g_ServerList.m_Clients[ oldacc->m_ServerNo ];
				if( sock != 0 ) sock->SendUserKick( m_AccName );
			}

			SendLoginReply( 4 );
			return;
		}
	}

	m_Right = cmd["Right"].asULong( );
	m_PayFlag = 0x6F;
	m_State = eLSSTATE_LOGGEDIN;

	int acccnt = g_AccountList.GetRealCount( );

	CLoginAccount* acc = g_LoginAccountDataPool.Acquire( );
	acc->m_State = eACCSTATE_DEFAULT;
	acc->m_Right = m_Right;
	acc->m_PayFlag = m_PayFlag;
	acc->m_LoginTime = timeGetTime( );
	strcpy( acc->m_AccName, m_AccName );
	strcpy( acc->m_Pass, m_Pass );
	m_Lsid = g_AccountList.GetNewLsid( );
	g_AccountList.AddAccount( m_Lsid, acc );

	int acccnt2 = g_AccountList.GetRealCount( );
	printf( "Account Added :: '%s' (T:%d/%d)\n", m_AccName, acccnt, acccnt2 );
	printf( "Account List Total :: %d\n", g_AccountList.GetLoggedCount( ) );
	*/

	SendLoginReply( 0 );
};

void CLoginSocket::PakChannelRequest( CPacket* pak )
{
	if( m_State != eLSSTATE_LOGGEDIN )
		return;

	int svrid = Packet_GetDword( pak, 0 ) - 1;

	g_ServerList.m_ClientsLock.Enter( );
	CLoginIscSocket* c = (CLoginIscSocket*)g_ServerList.m_Clients[ svrid ];
	g_ServerList.m_ClientsLock.Leave( );
	if( c == 0 )
	{
		return;
	}

	CPacket* pakout = AllocPacket( );
	Packet_Start( pakout, 0xB, 0x704 );
	Packet_SetDword( pakout, 0, svrid + 1 );
	Packet_SetByte( pakout, 4, 0 );
	
	c->m_GsListLock.Enter( );
	for( CList<CGsEntry*>::Iter i = c->m_GsList.First( ); i.isValid( ); i++ )
	{
		Packet_AddByte( pakout, (*i)->m_ChNo );
		Packet_AddByte( pakout, (*i)->m_LowAge );
		Packet_AddByte( pakout, (*i)->m_HighAge );
		Packet_AddWord( pakout, (*i)->m_Status );
		Packet_AddString( pakout, (*i)->m_Name );

		Packet_SetByte( pakout, 4, Packet_GetByte( pakout, 4 ) + 1 );
	}
	c->m_GsListLock.Leave( );

	SendPacket( pakout );
};

void CLoginSocket::PakServerRequest( CPacket* pak )
{
	if( m_State != eLSSTATE_LOGGEDIN )
		return;

	int svrid = Packet_GetDword( pak, 0 ) - 1;
	char chid = Packet_GetByte( pak, 4 );

	g_ServerList.m_ClientsLock.Enter( );
	CGsEntry* thisgs = 0;
	CLoginIscSocket* c = (CLoginIscSocket*)g_ServerList.m_Clients[ svrid ];
	g_ServerList.m_ClientsLock.Leave( );
	if( c == 0 )
	{
		return;
	}

	CLoginAccount* acc = g_AccountList.FindAccount( m_Lsid );
	if( acc == 0 )
	{
		return;
	}

	acc->m_TransferTime = GetTickCount( );
	acc->m_ServerNo = svrid;
	acc->m_ChannelNo = chid;
	acc->m_State = eACCSTATE_TRANSFERING;

	m_State = eLSSTATE_TRANSFERING;

	CPacket* pakout = AllocPacket( );
	Packet_Start( pakout, 0xF, 0x70a );
	Packet_SetByte( pakout, 0, 0 );
	Packet_SetDword( pakout, 1, m_Lsid );
	Packet_SetDword( pakout, 5, c->m_Seed );
	Packet_AddString( pakout, c->m_Ip );
	Packet_AddWord( pakout, c->m_Port );
	SendPacket( pakout );

	CloseSocket( );
};

void CLoginSocket::SendLoginReply( char val )
{
	CPacket* pakout = AllocPacket( );
	Packet_Start( pakout, 0xB, 0x708 );
	Packet_SetByte( pakout, 0, val );
	Packet_SetWord( pakout, 1, m_Right );
	Packet_SetWord( pakout, 3, 0 );
	if( val == 0 )
	{
		g_ServerList.m_ClientsLock.Enter( );
		CIndexArray<CIocpSocket>& sc = g_ServerList.m_Clients;
		for( CIndexArray<CIocpSocket>::Iter i = sc.First( ); i.isValid( ); i++ )
		{
			CLoginIscSocket* c = (CLoginIscSocket*)(*i);
			Packet_AddString( pakout, c->m_Name );
			Packet_AddDword( pakout, c->m_Index + 1 );
		}
		g_ServerList.m_ClientsLock.Leave( );
	}
	SendPacket( pakout );
};