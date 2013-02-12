#include "CRoseSocket.hpp"

void CRoseSocket::Init( )
{
	CIocpSocket::Init( );
};

void CRoseSocket::Start( )
{
	StartRecv( );
};

void CRoseSocket::Disconnected( )
{
	HandleDisconnect( );

	// Clean up!
	m_SendQueueLock.Enter( );
	while( !m_SendQueue.IsEmpty( ) )
	{
		CPacket* pak = m_SendQueue.Pop_Front( );
		FreePacket( pak );
	}
	m_SendQueueLock.Leave( );
};

void CRoseSocket::HandleDisconnect( )
{
};

void CRoseSocket::StartRecv( )
{
	RecvStart( (unsigned char*)&m_RecvBuf, 6, (IoEventHandler)&CRoseSocket::OnRecvHead );
};

void CRoseSocket::OnRecvHead( int size )
{
	unsigned short paklen = g_Crypt.DRCH( (unsigned char*)&m_RecvBuf );
	if( paklen == 6 ) {
		OnRecvBody( paklen );
	} else {
		RecvStart( m_RecvBuf.m_Data, paklen - 6, (IoEventHandler)&CRoseSocket::OnRecvBody );
	}
};

void CRoseSocket::OnRecvBody( int size )
{
	//* Decrypt Packet
	if( !g_Crypt.DRCB( (unsigned char*)&m_RecvBuf ) )
	{
		CloseSocket( );
		return;
	}

	CPacket* pak = &m_RecvBuf;
	/*
	printf( "[CLI>%d:%03x]", pak->m_Size, pak->m_Command );
	for( int i = 0; i < pak->m_Size-6; i++ )
		printf( " %02x", pak->m_Data[i] );
	printf( "\n" );
	//*/
	HandlePacket( pak );

	StartRecv( );
};

void CRoseSocket::StartSend( )
{
	m_SendQueueLock.Enter( );
	CList<CPacket*>::Iter first = m_SendQueue.First( );
	CPacket* pak = (*first);
	unsigned int paksize = pak->m_Size;
	g_Crypt.ESSP( (unsigned char*)pak );
	SendStart( pak->m_Buffer, paksize, (IoEventHandler)&CRoseSocket::OnSend );
	m_SendQueueLock.Leave( );
};

void CRoseSocket::OnSend( int size )
{
	m_SendQueueLock.Enter( );
	CPacket* pak = m_SendQueue.Pop_Front( );
	FreePacket( pak );
	if( !m_SendQueue.IsEmpty( ) ) StartSend( );
	m_SendQueueLock.Leave( );
};

CPacket* CRoseSocket::AllocPacket( )
{
	return g_PacketPool.Acquire( );
};

void CRoseSocket::FreePacket( CPacket* pak )
{
	g_PacketPool.Release( pak );
};

void CRoseSocket::SendPacket( CPacket* pak )
{
	/*
	printf( "[SVR>%d:%03x]", pak->m_Size, pak->m_Command );
	for( int i = 0; i < pak->m_Size-6; i++ )
		printf( " %02x", pak->m_Data[i] );
	printf( "\n" );
	//*/

	m_SendQueueLock.Enter( );
	bool startio = m_SendQueue.IsEmpty( );
	m_SendQueue.Push_Back( pak );
	if( startio ) StartSend( );
	m_SendQueueLock.Leave( );
};