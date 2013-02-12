#pragma once
#include "../Common/CWinsock.hpp"
#include "../Common/CCriticalSection.hpp"
#include "../RoseCommon/CIscSocket.hpp"
#include "../RoseCommon/CPacket.hpp"

class CWorldIscSocket
	: public CIscSocket
{
public:
	virtual void Init( );
	virtual void Destroy( );
	void HandlePacket( CPacket* pak );

};