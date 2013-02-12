#include "CIscSocket.hpp"

void CIscSocket::Start( )
{
	StartRecv( );
};

void CIscSocket::StartRecv( )
{
	RecvStart( (unsigned char*)&m_RecvBuf, 6, (IoEventHandler)&CIscSocket::OnRecvHead );
};

void CIscSocket::OnRecvHead( int size )
{
	CPacket* pak = &m_RecvBuf;
	if( pak->m_Size == 6 ) {
		OnRecvBody( size );
	} else {
		RecvStart( m_RecvBuf.m_Data, pak->m_Size - 6, (IoEventHandler)&CIscSocket::OnRecvBody );
	}
};

void CIscSocket::OnRecvBody( int size )
{
	CPacket* pak = &m_RecvBuf;

	//* Decrypt Packet
	/*
	printf( "[CLI>%d:%03x]", pak->m_Size, pak->m_Command );
	for( int i = 0; i < pak->m_Size-6; i++ )
		printf( " %02x", pak->m_Data[i] );
	printf( "\n" );
	*/

	HandlePacket( pak );

	StartRecv( );
};

void CIscSocket::StartSend( )
{
	m_SendQueueLock.Enter( );
	CList<CPacket*>::Iter first = m_SendQueue.First( );
	SendStart( (*first)->m_Buffer, (*first)->m_Size, (IoEventHandler)&CIscSocket::OnSend );
	m_SendQueueLock.Leave( );
};

void CIscSocket::OnSend( int size )
{
	m_SendQueueLock.Enter( );
	CPacket* pak = m_SendQueue.Pop_Front( );
	FreePacket( pak );
	if( !m_SendQueue.IsEmpty( ) ) StartSend( );
	m_SendQueueLock.Leave( );
};

CPacket* CIscSocket::AllocPacket( )
{
	return g_PacketPool.Acquire( );
};

void CIscSocket::FreePacket( CPacket* pak )
{
	g_PacketPool.Release( pak );
};

void CIscSocket::SendPacket( CPacket* pak )
{
	//* Encrypt Packet
	/* No encryption in evoROSE
	*/

	/*
	printf( "[SVR>%d:%03x]", pak->m_Size, pak->m_Command );
	for( int i = 0; i < pak->m_Size-6; i++ )
		printf( " %02x", pak->m_Data[i] );
	printf( "\n" );
	*/

	m_SendQueueLock.Enter( );
	bool startio = m_SendQueue.IsEmpty( );
	m_SendQueue.Push_Back( pak );
	if( startio ) StartSend( );
	m_SendQueueLock.Leave( );
};