#pragma once
#include <iostream>
#include "../Common/CWinsock.hpp"
#include "../Common/CCriticalSection.hpp"
#include "../Common/CList.hpp"
#include "../Common/CIocpSocket.hpp"
#include "CPacket.hpp"

class CIscSocket
	: public CIocpSocket
{
public:
	void Start( );
	void StartRecv( );
	void OnRecvHead( int size );
	void OnRecvBody( int size );
	void StartSend( );
	void OnSend( int size );
	void SendPacket( CPacket* pak );
	virtual void HandlePacket( CPacket* pak ) = 0;
	CPacket* AllocPacket( );
	void FreePacket( CPacket* pak );

protected:
	CPacket m_RecvBuf;
	CCriticalSection m_SendQueueLock;
	CList< CPacket* > m_SendQueue;
};