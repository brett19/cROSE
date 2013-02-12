#include "CIocpSocket.hpp"

CIocpSocket::CIocpSocket( )
{
};

CIocpSocket::~CIocpSocket( )
{
};

void CIocpSocket::Init( )
{
	m_Socket = INVALID_SOCKET;
	m_RefCount = 0;
	m_RRefCount = 0;
	m_SRefCount = 0;
	m_DRefCount = 0;
	m_LastRecv = GetTickCount( );
};

void CIocpSocket::Destroy( )
{
};

void CIocpSocket::CloseSocket( )
{
	m_Lock.Enter( );
	shutdown( m_Socket, SD_SEND );
	closesocket( m_Socket );
	m_Socket = INVALID_SOCKET;
	m_Lock.Leave( );
};

bool CIocpSocket::IsConnected( )
{
	return m_Socket != INVALID_SOCKET;
};

void CIocpSocket::Disconnected( )
{
};

void CIocpSocket::RecvStart( unsigned char* buffer, int size, IoEventHandler handler )
{
	m_Lock.Enter( );
	m_RefCount++;
	m_RRefCount++;
	m_Lock.Leave( );

	CIocpIoData* iodata = g_IoDataPool.Acquire( );
	memset( iodata, 0, sizeof(CIocpIoData) );
	iodata->buffer = buffer;
	iodata->buffersize = size;
	iodata->bufferlen = 0;
	iodata->handler = handler;
	iodata->mode = IOMODE_RECV;
	iodata->m_Owner = this;
	RecvContinue( iodata );
};

void CIocpSocket::RecvContinue( CIocpIoData* iodata )
{
	WSABUF buf = { iodata->buffersize-iodata->bufferlen, (CHAR*)&iodata->buffer[iodata->bufferlen] };
	DWORD flags = 0;
	DWORD readamount = 0;
	if( WSARecv( m_Socket, &buf, 1, &readamount, &flags, (OVERLAPPED*)iodata, 0 ) != 0 ) {
		int err = WSAGetLastError( );
		if( err != WSA_IO_PENDING ) {
			// ERRORRZZ SIR
			g_IoDataPool.Release( iodata );
			m_Lock.Enter( );
			m_RefCount--;
			m_RRefCount--;
			m_Lock.Leave( );
			CloseSocket( );
			//DebugBreak( );
		}
	}
};

void CIocpSocket::RecvComplete( CIocpIoData* iodata )
{
	(this->*(iodata->handler))( iodata->bufferlen );
	g_IoDataPool.Release( iodata );

	m_Lock.Enter( );
	m_LastRecv = GetTickCount( );
	m_RefCount--;
	m_RRefCount--;
	m_Lock.Leave( );
};

void CIocpSocket::SendStart( unsigned char* buffer, int size, IoEventHandler handler )
{
	m_Lock.Enter( );
	m_RefCount++;
	m_SRefCount++;
	m_Lock.Leave( );

	CIocpIoData* iodata = g_IoDataPool.Acquire( );
	memset( iodata, 0, sizeof(CIocpIoData) );
	iodata->buffer = buffer;
	iodata->buffersize = size;
	iodata->bufferlen = 0;
	iodata->handler = handler;
	iodata->mode = IOMODE_SEND;
	iodata->m_Owner = this;
	SendContinue( iodata );
};

void CIocpSocket::SendContinue( CIocpIoData* iodata )
{
	WSABUF buf = { iodata->buffersize-iodata->bufferlen, (CHAR*)&iodata->buffer[iodata->bufferlen] };
	DWORD flags = 0;
	DWORD sendamount = 0;
	if( WSASend( m_Socket, &buf, 1, &sendamount, flags, (OVERLAPPED*)iodata, 0 ) != 0 ) {
		int err = WSAGetLastError( );
		if( err != WSA_IO_PENDING ) {
			// ERRORRZZ SIR
			g_IoDataPool.Release( iodata );
			m_Lock.Enter( );
			m_RefCount--;
			m_SRefCount--;
			m_Lock.Leave( );
			CloseSocket( );
			//DebugBreak( );
		}
	}
};

void CIocpSocket::SendComplete( CIocpIoData* iodata )
{
	(this->*(iodata->handler))( iodata->bufferlen );
	g_IoDataPool.Release( iodata );

	m_Lock.Enter( );
	m_RefCount--;
	m_SRefCount--;
	m_Lock.Leave( );
};

void CIocpSocket::Start( )
{
};