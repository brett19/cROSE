#pragma once
#include <winsock2.h>
#include "CDataPool.hpp"

class CIocpSocket;
typedef void (CIocpSocket::*IoEventHandler)( int size );

enum IOMODE
{
	IOMODE_RECV,
	IOMODE_SEND
};

struct CIocpIoData
{
	OVERLAPPED m_Overlapped;
	IOMODE mode;
	IoEventHandler handler;
	unsigned char* buffer;
	int buffersize;
	int bufferlen;
	CIocpSocket* m_Owner;
};

extern CDataPool< CIocpIoData > g_IoDataPool;