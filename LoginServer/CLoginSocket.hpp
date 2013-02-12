#pragma once
#undef WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include "../Common/CDataPool.hpp"
#include "CLoginClientList.hpp"
#include "CLoginServerList.hpp"
#include "CLoginAccountList.hpp"
#include "../RoseCommon/CRoseSocket.hpp"
#include "../RoseCommon/CPacket.hpp"
#include "../RoseCommon/CSqlThread.hpp"

enum eLSSTATE {
	eLSSTATE_DEFAULT,
	eLSSTATE_LOGGEDIN,
	eLSSTATE_TRANSFERING,
};

class CLoginSocket
	: public CRoseSocket
{
public:
	virtual void Init( );
	void HandleDisconnect( );
	void HandlePacket( CPacket* pak );
	void PakCryptRequest( CPacket* pak );
	void PakLoginRequest( CPacket* pak );
	void PakChannelRequest( CPacket* pak );
	void PakServerRequest( CPacket* pak );
	void SendLoginReply( char val );

	int m_Lsid;
	eLSSTATE m_State;
	char m_AccName[ 33 ];
	char m_Pass[ 33 ];
	unsigned int m_Right;
	unsigned int m_PayFlag;
};

extern CDataPool< CLoginSocket > g_LoginSocketDataPool;