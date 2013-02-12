#pragma once
#undef WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include "../Common/CDataPool.hpp"
#include "CWorldClientList.hpp"
#include "CWorldServerList.hpp"
#include "CWorldAccountList.hpp"
#include "../RoseCommon/CRoseSocket.hpp"
#include "../RoseCommon/CPacket.hpp"
#include "../RoseCommon/CSqlThread.hpp"

class CWorldSocket
	: public CRoseSocket
{
public:
	virtual void Init( );
	void HandleDisconnect( );
	void HandlePacket( CPacket* pak );
};

extern CDataPool< CWorldSocket > g_WorldSocketDataPool;