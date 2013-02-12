#pragma once
#include "../Common/CWinsock.hpp"
#include "../Common/CCriticalSection.hpp"
#include "../RoseCommon/CIscSocket.hpp"
#include "../RoseCommon/CPacket.hpp"
#include "CLoginAccountList.hpp"

struct CGsEntry
{
	unsigned char m_ChNo;
	unsigned char m_LowAge;
	unsigned char m_HighAge;
	short m_Status;
	char m_Name[ 0x20 ];
};

class CLoginIscSocket
	: public CIscSocket
{
public:
	virtual void Init( );
	virtual void Destroy( );
	void HandlePacket( CPacket* pak );
	void PakServerInfo( CPacket* pak );
	void PakUserConfirm( CPacket* pak );
	void PakUserDel( CPacket* pak );
	void PakServerStatus( CPacket* pak );
	void PakChannelList( CPacket* pak );
	void PakUserAdd( CPacket* pak );
	void SendUserConfirmReply( char retcode, int lsid, int csid, unsigned int logintime, unsigned int right, unsigned int payflag, unsigned char chno, char* accname );
	void SendUserKick( char* accname);

	bool m_Active;
	short m_Port;
	int m_Seed;
	char m_Name[ 0x20 ];
	CCriticalSection m_GsListLock;
	CList< CGsEntry* > m_GsList;
};