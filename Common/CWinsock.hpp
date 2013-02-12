#pragma once
#include <winsock2.h>

class CWinsock
{
public:
	static bool Start( );
	static void Cleanup( );
};