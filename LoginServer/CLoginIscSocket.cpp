#include "CLoginIscSocket.hpp"

void CLoginIscSocket::Init( )
{
	m_Active = false;

	CIocpSocket::Init( );
};

void CLoginIscSocket::Destroy( )
{
	m_GsListLock.Enter( );
	for( CList<CGsEntry*>::Iter i = m_GsList.First( ); i.isValid( ); i++ )
		delete *i;
	m_GsList.Clear( );
	m_GsListLock.Leave( );
};

void CLoginIscSocket::HandlePacket( CPacket* pak )
{
	switch( pak->m_Command )
	{
		case 0x401: PakServerInfo( pak ); break;
		case 0x411: PakUserConfirm( pak ); break;
		case 0x413: PakUserDel( pak ); break;
		case 0x416: PakServerStatus( pak ); break;
		case 0x417: PakChannelList( pak ); break;
		case 0x418: PakUserAdd( pak ); break;
	}
};

void CLoginIscSocket::PakServerInfo( CPacket* pak )
{
	m_Port = Packet_GetWord( pak, 0 );
	m_Seed = Packet_GetDword( pak, 2 );
	Packet_GetString( pak, 11, m_Name );
	//Packet_GetString( pak, curpos, m_SvrIp ); <--- This is broken, WS sends nothing

	printf( "New world connected :: '%s', x.x.x.x:%d, %08x\n", m_Name, m_Port, m_Name );
};

void CLoginIscSocket::SendUserConfirmReply( char retcode, int lsid, int csid, unsigned int logintime, unsigned int right, unsigned int payflag, unsigned char chno, char* accname )
{
	CPacket* pakout = AllocPacket( );
	Packet_Start( pakout, 0x20, 0x411 );
	Packet_SetByte( pakout, 0, retcode );
	Packet_SetDword( pakout, 1, lsid );
	Packet_SetDword( pakout, 5, csid );
	Packet_SetDword( pakout, 13, logintime );
	Packet_SetDword( pakout, 17, right );
	Packet_SetDword( pakout, 21, payflag );
	Packet_SetByte( pakout, 25, chno );
	Packet_AddString( pakout, accname );
	SendPacket( pakout );
};

void CLoginIscSocket::PakUserConfirm( CPacket* pak )
{
	int lsid = Packet_GetDword( pak, 0 );
	int csid = Packet_GetDword( pak, 4 );
	char pass[ 33 ]; Packet_GetBytes( pak, 8, 32, pass ); pass[32] = 0;

	CLoginAccount* acc = g_AccountList.FindAccount( lsid );
	if( acc == 0 ) {
		SendUserConfirmReply( 2, lsid, csid, 0, 0, 0, 0, 0 );
		return;
	}
	if( _strcmpi( acc->m_Pass, pass ) != 0 ) {
		SendUserConfirmReply( 3, lsid, csid, 0, 0, 0, 0, 0 );
		return;
	}
	if( acc->m_State != eACCSTATE_TRANSFERING )
	{
		SendUserConfirmReply( 4, lsid, csid, 0, 0, 0, 0, 0 );
		return;
	}
	if( acc->m_TransferTime < GetTickCount( ) - 30000 )
	{
		SendUserConfirmReply( 4, lsid, csid, 0, 0, 0, 0, 0 );
		return;
	}

	acc->m_State = eACCSTATE_CONFIRMED;
	SendUserConfirmReply( 0, lsid, csid, acc->m_LoginTime, acc->m_Right, acc->m_PayFlag, acc->m_ChannelNo, acc->m_AccName );
};

void CLoginIscSocket::PakUserDel( CPacket* pak )
{
	char accname[33]; Packet_GetString( pak, 0, accname );

	//* JUST FOR DEBUGGING
	int acccnt = g_AccountList.GetRealCount( );
	//* ---

	CLoginAccount* acc = g_AccountList.FindAccountAndRemove( accname );
	if( acc != 0 ) g_LoginAccountDataPool.Release( acc );

	//* JUST FOR DEBUGGING
	int acccnt2 = g_AccountList.GetRealCount( );

	printf( "Account Removed :: '%s' (T:%d/%d)\n", accname, acccnt, acccnt2 );
};

void CLoginIscSocket::PakServerStatus( CPacket* pak )
{
	m_Active = Packet_GetByte( pak, 0 ) == 1;
	if( m_Active )
		printf( "World is now activated :: '%s'\n", m_Name );  
	else
		printf( "World is now deactivated :: '%s'\n", m_Name );
};

void CLoginIscSocket::PakChannelList( CPacket* pak )
{
	m_GsListLock.Enter( );

	for( CList<CGsEntry*>::Iter i = m_GsList.First( ); i.isValid( ); i++ )
		delete *i;
	m_GsList.Clear( );

	int curpos = 0;
	char gscnt = Packet_SGetByte( pak, curpos );
	for( int i = 0; i < gscnt; i++ )
	{
		CGsEntry* newgs = new CGsEntry( );
		newgs->m_ChNo = Packet_SGetByte( pak, curpos );
		newgs->m_LowAge = Packet_SGetByte( pak, curpos );
		newgs->m_HighAge = Packet_SGetByte( pak, curpos );
		newgs->m_Status = Packet_SGetWord( pak, curpos );
		Packet_SGetString( pak, curpos, newgs->m_Name );
		
		m_GsList.Push_Back( newgs );
		printf( "GS Updated :: '%s', %d:'%s'\n", m_Name, newgs->m_ChNo, newgs->m_Name );
	}

	m_GsListLock.Leave( );
};

void CLoginIscSocket::PakUserAdd( CPacket* pak )
{
	int curpos = 0;
	unsigned short usercnt = Packet_SGetWord( pak, curpos );
	for( int i = 0; i < usercnt; i++ )
	{
		int lsid = Packet_SGetDword( pak, curpos );
		CLoginAccount* acc = g_LoginAccountDataPool.Acquire( );
		acc->m_State = eACCSTATE_RESETADDED;
		acc->m_LoginTime = Packet_SGetDword( pak, curpos );
		acc->m_ServerNo = m_Index;
		Packet_SGetString( pak, curpos, acc->m_AccName );
		g_AccountList.AddAccount( lsid, acc );
	}
	
	int svrcnt = 0, acccnt = 0;
	g_AccountList.m_AccountsLock.Enter( );
	for( CIndexList<CLoginAccount*>::Iter i = g_AccountList.m_Accounts.First( ); i.isValid( ); i++ )
	{
		acccnt++;
		if( (*i).data->m_ServerNo == m_Index )
			svrcnt++;
	}
	g_AccountList.m_AccountsLock.Leave( );

	printf( "GS New Users :: '%s', %d(ST:%d,T:%d)\n", m_Name, usercnt, svrcnt, acccnt );
};

void CLoginIscSocket::SendUserKick( char* accname )
{
	CPacket* pakout = AllocPacket( );
	Packet_Start( pakout, 0x06, 0x414 );
	Packet_AddString( pakout, accname );
	SendPacket( pakout );
};
