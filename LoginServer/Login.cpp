#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#undef WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>

#include <iostream>
#include "../Common/CWinsock.hpp"
#include "../Common/CEvent.hpp"
#include "../RoseCommon/CSqlThread.hpp"
#include "CLoginServerList.hpp"
#include "CLoginClientList.hpp"

CEvent g_ServerKill;

BOOL CtrlHandler( DWORD fdwCtrlType ) 
{ 
	switch( fdwCtrlType ) 
	{ 
		case CTRL_C_EVENT: 
		case CTRL_CLOSE_EVENT: 
		case CTRL_BREAK_EVENT: 
		case CTRL_LOGOFF_EVENT: 
		case CTRL_SHUTDOWN_EVENT: 
		default: 
			printf( "------\n" );
			printf( " I would appreciate it if you didn't close me like this...\n" );
			printf( "   Press 'q[ENTER]' to shut down gracefully\n" );
			printf( "------\n" );
			return TRUE; 
	} 
};

int main( int argc, char* argv[] )
{
	SetConsoleCtrlHandler( 0, TRUE );
	SetConsoleCtrlHandler( (PHANDLER_ROUTINE)CtrlHandler, TRUE );
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc( 224 );
	CWinsock::Start( );

	char configpath[ 300 ];
	GetCurrentDirectory( 300, configpath );
	strcat( configpath, "\\config.ini" );

	char cliport[65], svrport[65], dbhost[65], dbuser[65], dbpass[65];
	GetPrivateProfileString( "Arua_LS", "ClientListenPort", "29000", cliport, 65, configpath );
	GetPrivateProfileString( "Arua_LS", "ServerListenPort", "29001", svrport, 65, configpath );
	GetPrivateProfileString( "Arua_LS", "DBHost", "", dbhost, 65, configpath );
	GetPrivateProfileString( "Arua_LS", "DBUser", "", dbuser, 65, configpath );
	GetPrivateProfileString( "Arua_LS", "DBPass", "", dbpass, 65, configpath );
	strcat( dbhost, "@" );

	g_Crypt.Default( );
	g_SqlThread.Start( dbhost, dbuser, dbpass );
	g_ServerList.Start( atoi(svrport), 0 );
	g_ClientList.Start( atoi(cliport), 60 * 1000 ); // <-- DISABLED PING CHECKS
	//g_ServerKill.WaitFor( );
	printf( "Server Initialized\n" );

	while( getchar( ) != 'q' );

	g_ClientList.Stop( );
	g_ServerList.Stop( );
	g_SqlThread.Stop( );

	CWinsock::Cleanup( );

	printf( "--- Completed Shutdown\n" );
	while( getchar( ) != 'd' );
	return 1;
};
