/*
#include <iostream>
#include <windows.h>
#include "../Common/CThread.hpp"
#include "../Common/CWinsock.hpp"

struct packet {
	short size;
	char buf[6];
};

class CStressThread
	: public CThread
{
public:
	CStressThread( int id )
		: CThread( false ), m_Id( id )
	{
	};

	~CStressThread( )
	{
	};

	int Execute( )
	{
		int starttime = GetTickCount( );

		SOCKET serverSock = INVALID_SOCKET;
		struct sockaddr_in ain;
		char buf[8];
		packet* pakin;

		packet pak;
		pak.size = 8;
		memcpy(pak.buf, "monkey", 6);

		serverSock = socket(AF_INET, SOCK_STREAM, 0);
		if (serverSock == INVALID_SOCKET) {
			printf("[%d] Error creating server socket.\n", m_Id);
			return 1;
		}

		ain.sin_family = AF_INET;
		ain.sin_addr.s_addr = inet_addr("127.0.0.1");
		ain.sin_port = htons( 29000 );

		int conval = connect(serverSock, (SOCKADDR*)&ain, sizeof(ain));
		if( conval < 0 ) {
			printf("[%d] Failed to connect: %d\n", m_Id, WSAGetLastError());
			return 1;
		}

		int sendval = send( serverSock, (char*)&pak, 8, 0 );
		if( sendval <= 0 ) {
			printf("[%d] Failed to send data: %d\n", m_Id, WSAGetLastError());
			return 1;
		}

		int recvval = recv(serverSock, buf, 8, 0);
		if( recvval <= 0 ) {
			printf("[%d] Failed to recv data: %d\n", m_Id, WSAGetLastError());
			return 1;
		}
		pakin = (packet*)&buf;

		if (memcmp(&pak, pakin, 8) != 0) {
			printf("[%d] Server failed to echo properly\n", m_Id);
			return 1;
		}

		printf("[%d] Completed in %d ms\n", m_Id, GetTickCount( )-starttime);

		return 0;
	};

protected:
	int m_Id;
};

int main( int argc, char* argv[] )
{
	CWinsock::Start( );

	for( int i = 0; i < 1000; i++ )
	{
		CStressThread* tester = new CStressThread( i );
		tester->Resume( );

		Sleep( 10 );
	};

	Sleep( 400 );

	CWinsock::Cleanup( );

	return 0;
};
*/