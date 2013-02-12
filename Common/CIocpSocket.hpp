#pragma once
#include "../Common/CList.hpp"
#include "CIocpIoData.hpp"

class CIocpSocket
{
public:
	CIocpSocket( );
	~CIocpSocket( );
	virtual void Init( );
	virtual void Destroy( );
	virtual void Start( );
	virtual void Disconnected( );
	bool IsConnected( );
	void CloseSocket( );
	void RecvStart( unsigned char* buffer, int size, IoEventHandler handler );
	void RecvContinue( CIocpIoData* iodata );
	void RecvComplete( CIocpIoData* iodata );
	void SendStart( unsigned char* buffer, int size, IoEventHandler handler );
	void SendContinue( CIocpIoData* iodata );
	void SendComplete( CIocpIoData* iodata );

	int m_Index;
	CCriticalSection m_Lock;
	SOCKET m_Socket;
	DWORD m_LastRecv;
	char m_Ip[16];
	int m_RefCount;
	int m_DRefCount;
	int m_SRefCount;
	int m_RRefCount;
};

