#include <string.h>

//* Official CrcTable
///*
const unsigned char CrcTable[ ] =
  "\x00\x5E\xBC\xE2\x61\x3F\xDD\x83\xC2\x9C\x7E\x20\xA3\xFD\x1F\x41"
  "\x9D\xC3\x21\x7F\xFC\xA2\x40\x1E\x5F\x01\xE3\xBD\x3E\x60\x82\xDC"
  "\x23\x7D\x9F\xC1\x42\x1C\xFE\xA0\xE1\xBF\x5D\x03\x80\xDE\x3C\x62"
  "\xBE\xE0\x02\x5C\xDF\x81\x63\x3D\x7C\x22\xC0\x9E\x1D\x43\xA1\xFF"
  "\x46\x18\xFA\xA4\x27\x79\x9B\xC5\x84\xDA\x38\x66\xE5\xBB\x59\x07"
  "\xDB\x85\x67\x39\xBA\xE4\x06\x58\x19\x47\xA5\xFB\x78\x26\xC4\x9A"
  "\x65\x3B\xD9\x87\x04\x5A\xB8\xE6\xA7\xF9\x1B\x45\xC6\x98\x7A\x24"
  "\xF8\xA6\x44\x1A\x99\xC7\x25\x7B\x3A\x64\x86\xD8\x5B\x05\xE7\xB9"
  "\x8C\xD2\x30\x6E\xED\xB3\x51\x0F\x4E\x10\xF2\xAC\x2F\x71\x93\xCD"
  "\x11\x4F\xAD\xF3\x70\x2E\xCC\x92\xD3\x8D\x6F\x31\xB2\xEC\x0E\x50"
  "\xAF\xF1\x13\x4D\xCE\x90\x72\x2C\x6D\x33\xD1\x8F\x0C\x52\xB0\xEE"
  "\x32\x6C\x8E\xD0\x53\x0D\xEF\xB1\xF0\xAE\x4C\x12\x91\xCF\x2D\x73"
  "\xCA\x94\x76\x28\xAB\xF5\x17\x49\x08\x56\xB4\xEA\x69\x37\xD5\x8B"
  "\x57\x09\xEB\xB5\x36\x68\x8A\xD4\x95\xCB\x29\x77\xF4\xAA\x48\x16"
  "\xE9\xB7\x55\x0B\x88\xD6\x34\x6A\x2B\x75\x97\xC9\x4A\x14\xF6\xA8"
  "\x74\x2A\xC8\x96\x15\x4B\xA9\xF7\xB6\xE8\x0A\x54\xD7\x89\x6B\x35";
//*/
//* AruaRose CrcTable
/*
const unsigned char CrcTable[ ] = {
  0x00, 0xF1, 0x13, 0x4D, 0xCE, 0x90, 0x72, 0x2C, 0x6D, 0x33, 0xD1, 0x8F, 0x0C, 0x52, 0xB0, 0xEE,
  0x57, 0x09, 0xEB, 0xB5, 0x36, 0x68, 0x8A, 0xD4, 0x95, 0xCB, 0x29, 0x77, 0xF4, 0xAA, 0x48, 0x16,
  0x46, 0x18, 0xFA, 0xA4, 0x27, 0x79, 0x9B, 0xC5, 0x84, 0xDA, 0x38, 0x66, 0xE5, 0xBB, 0x59, 0x07,
  0xBE, 0xE0, 0x02, 0x5C, 0xDF, 0x81, 0x63, 0x3D, 0x7C, 0x22, 0xC0, 0x9E, 0x1D, 0x43, 0xA1, 0xFF,
  0xF8, 0xA6, 0x44, 0x1A, 0x99, 0xC7, 0x25, 0x7B, 0x3A, 0x64, 0x86, 0xD8, 0x5B, 0x05, 0xE7, 0xB9,
  0xCA, 0x94, 0x76, 0x28, 0xAB, 0xF5, 0x17, 0x49, 0x08, 0x56, 0xB4, 0xEA, 0x69, 0x37, 0xD5, 0x8B,
  0x74, 0x2A, 0xC8, 0x96, 0x15, 0x4B, 0xA9, 0xF7, 0xB6, 0xE8, 0x0A, 0x54, 0xD7, 0x89, 0x6B, 0x35,
  0x23, 0x7D, 0x9F, 0xC1, 0x42, 0x1C, 0xFE, 0xA0, 0xE1, 0xBF, 0x5D, 0x03, 0x80, 0xDE, 0x3C, 0x62,
  0x65, 0x3B, 0xD9, 0x87, 0x04, 0x5A, 0xB8, 0xE6, 0xA7, 0xF9, 0x1B, 0x45, 0xC6, 0x98, 0x7A, 0x24,
  0x11, 0x4F, 0xAD, 0xF3, 0x70, 0x2E, 0xCC, 0x92, 0xD3, 0x8D, 0x6F, 0x31, 0xB2, 0xEC, 0x0E, 0x50,
  0xAF, 0x5E, 0xBC, 0xE2, 0x61, 0x3F, 0xDD, 0x83, 0xC2, 0x9C, 0x7E, 0x20, 0xA3, 0xFD, 0x1F, 0x41,
  0xDB, 0x85, 0x67, 0x39, 0xBA, 0xE4, 0x06, 0x58, 0x19, 0x47, 0xA5, 0xFB, 0x78, 0x26, 0xC4, 0x9A,
  0xE9, 0xB7, 0x55, 0x0B, 0x88, 0xD6, 0x34, 0x6A, 0x2B, 0x75, 0x97, 0xC9, 0x4A, 0x14, 0xF6, 0xA8,
  0x32, 0x6C, 0x8E, 0xD0, 0x53, 0x0D, 0xEF, 0xB1, 0xF0, 0xAE, 0x4C, 0x12, 0x91, 0xCF, 0x2D, 0x73,
  0x9D, 0xC3, 0x21, 0x7F, 0xFC, 0xA2, 0x40, 0x1E, 0x5F, 0x01, 0xE3, 0xBD, 0x3E, 0x60, 0x82, 0xDC,
  0x8C, 0xD2, 0x30, 0x6E, 0xED, 0xB3, 0x51, 0x0F, 0x4E, 0x10, 0xF2, 0xAC, 0x2F, 0x71, 0x93, 0xCD
};
//*/

struct CR001
{
	void Init( unsigned int seed )
	{
		m_VcSeed = seed;
		m_BcSeed = seed;
		m_AcSeed = seed;
		m_MySeed = seed;
	};

	void SetType( unsigned char type )
	{
		m_Type = type;
	};

	int R_BC( )
	{
		m_BcSeed = 0x8088405 * m_BcSeed + 1;
		return m_BcSeed / 0x10000;
	};

	int R_AC( )
	{
		m_AcSeed = ( 0x41C64E6D * m_AcSeed + 12345 ) & 0x7FFFFFFF;
		return m_AcSeed;
	};

	int R_MY( )
	{
		m_MySeed = 0x41C64E6D * m_MySeed + 12345;
		return m_MySeed / 0x10000;
	};

	int R_VC( )
	{
		m_VcSeed = ( 0x343FD * m_VcSeed + 0x269EC3 ) & 0x7FFFFFFF;
		return m_VcSeed / 0x10000;
	};

	int Get( )
	{
		if( m_Type == 0 )
			return R_BC( );
		else if( m_Type == 1 )
			return R_AC( );
		else if( m_Type == 2 )
			return R_MY( );
		else
			return R_VC( );
	};

	unsigned char m_Type;
	int m_VcSeed;
	int m_BcSeed;
	int m_AcSeed;
	int m_MySeed;
};

struct HeadCryptedServer
{
	unsigned int AddBufferLen2 : 3;		//S2
	unsigned int AddTableValue1 : 3;	//R1
	unsigned int Command3 : 3;			//T3
	unsigned int EncryptValue1 : 3;		//R7
	unsigned int AddBufferLen3 : 3;		//S3
	unsigned int AddTableValue3 : 3;	//R3
	unsigned int Command2 : 3;			//T2
	unsigned int AddTableValue4 : 2;	//R4
	unsigned int Command1 : 3;			//T1
	unsigned int EncryptAddValue1 : 2;	//R5
	unsigned int AddBufferLen4 : 2;		//S4
	unsigned int EncryptAddValue2 : 2;	//R6
	unsigned char AddBufferLen1 : 3;	//S1
	unsigned char AddTableValue2 : 3;	//R2
	unsigned char Command4 : 2;			//T4
};
struct HeadCryptedClient
{
	unsigned int Command2 : 3;			//T2
	unsigned int AddTableValue2 : 3;	//R2
	unsigned int AddBufferLen1 : 3;		//S1
	unsigned int AddTableValue3 : 3;	//R3
	unsigned int Command1 : 3;			//T1
	unsigned int EncryptValue1 : 3;		//R7
	unsigned int AddBufferLen2 : 3;		//S2
	unsigned int EncryptAddValue2 : 2;	//R6
	unsigned int AddBufferLen3 : 3;		//S3
	unsigned int AddTableValue4 : 2;	//R4
	unsigned int Command4 : 2;			//T4
	unsigned int EncryptAddValue1 : 2;	//R5
	unsigned char Command3 : 3;			//T3
	unsigned char AddTableValue1 : 3;	//R1
	unsigned char AddBufferLen4 : 2;	//S4
};
struct HeadDecrypted
{
	unsigned __int64 AddBufferLen1 : 3;
	unsigned __int64 AddBufferLen2 : 3;
	unsigned __int64 AddBufferLen3 : 3;
	unsigned __int64 AddBufferLen4 : 2;
	unsigned __int64 Command1 : 3;
	unsigned __int64 Command2 : 3;
	unsigned __int64 Command3 : 3;
	unsigned __int64 Command4 : 2;
	unsigned __int64 AddTableValue1 : 3;
	unsigned __int64 AddTableValue2 : 3;
	unsigned __int64 AddTableValue3 : 3;
	unsigned __int64 AddTableValue4 : 2;
	unsigned __int64 EncryptAddValue1 : 2;
	unsigned __int64 EncryptAddValue2 : 2;
	unsigned __int64 EncryptValue1 : 3;
};
struct Head
{
	unsigned __int64 AddBufferLen : 11;
	unsigned __int64 Command	: 11;
	unsigned __int64 AddTableValue : 11;
	unsigned __int64 EncryptAddValue : 4;
	unsigned __int64 EncryptValue : 3;
};

template<typename T, typename U>
inline void FlipHeadMain( T a, U b )
{
	a->AddBufferLen1 = b->AddBufferLen1;
	a->AddBufferLen2 = b->AddBufferLen2;
	a->AddBufferLen3 = b->AddBufferLen3;
	a->AddBufferLen4 = b->AddBufferLen4;
	a->Command1 = b->Command1;
	a->Command2 = b->Command2;
	a->Command3 = b->Command3;
	a->Command4 = b->Command4;
	a->EncryptValue1 = b->EncryptValue1;
	a->EncryptAddValue1 = b->EncryptAddValue1;
	a->EncryptAddValue2 = b->EncryptAddValue2;
};
template<typename T, typename U>
inline void FlipHeadFinal( T a, U b )
{
	a->AddTableValue1 = b->AddTableValue1;
	a->AddTableValue2 = b->AddTableValue2;
	a->AddTableValue3 = b->AddTableValue3;
	a->AddTableValue4 = b->AddTableValue4;
};

struct CPC
{
	CPC( )
	{
		m_Rt = new int*[ 0x10 ];
		for( int i = 0; i < 0x10; i++ )
			m_Rt[ i ] = new int[ 0x800 ];
		m_Ri = new unsigned short[ 0x200 ];
	};

	~CPC( )
	{
		for( int i = 0; i < 0x10; i++ )
			delete[] m_Rt[ i ];
		delete[] m_Rt;
		delete[] m_Ri;
	};

	void ESSP( unsigned char* buffer )
	{
		Head head = { 0 };
		head.AddTableValue = 0x100; // rand() % 0x1FF + 1
		head.EncryptAddValue = 0x04; // rand() % 0xF + 1
		head.EncryptValue = head.AddTableValue + head.EncryptAddValue;
		head.AddBufferLen = *((unsigned short*)&buffer[0]);
		head.Command = *((unsigned short*)&buffer[2]);
		FlipHeadMain( (HeadCryptedServer*)buffer, (HeadDecrypted*)&head );

		unsigned char Checksum = 0;
		for( int i = 0; i < 5; i++ )
		{
			Checksum = CrcTable[ ((unsigned char*)&head)[i] ^ Checksum ];
			buffer[ i ] ^= m_Rt[ i ][ head.AddTableValue ];
		}
		for( int i = 6; i < head.AddBufferLen; i++ )
		{
			Checksum = CrcTable[ buffer[i] ^ Checksum ];
			buffer[ i ] ^= m_Rt[ ( head.EncryptAddValue + i ) & 0xF ][ ( head.AddTableValue + i ) & 0x7FF ];
		}
		buffer[5] = Checksum;

		FlipHeadFinal( (HeadCryptedServer*)buffer, (HeadDecrypted*)&head );
	};

	void ESCP( unsigned char* buffer )
	{
		Head head = { 0 };
		head.AddTableValue = 0x100; // rand() % 0x1FF + 1
		head.EncryptAddValue = 0x04; // rand() % 0xF + 1
		head.EncryptValue = head.AddTableValue + head.EncryptAddValue;
		head.AddBufferLen = *((unsigned short*)&buffer[0]);
		head.Command = *((unsigned short*)&buffer[2]);
		FlipHeadMain( (HeadCryptedClient*)buffer, (HeadDecrypted*)&head );

		unsigned char Checksum = 0;
		for( int i = 0; i < 5; i++ )
		{
			Checksum = CrcTable[ ((unsigned char*)&head)[i] ^ Checksum ];
			buffer[ i ] ^= m_Rt[ i ][ head.AddTableValue ];
		}
		for( int i = 6; i < head.AddBufferLen; i++ )
		{
			Checksum = CrcTable[ buffer[i] ^ Checksum ];
			buffer[ i ] ^= m_Rt[ ( head.EncryptAddValue + i ) & 0xF ][ ( head.AddTableValue + i ) & 0x7FF ];
		}
		buffer[5] = Checksum;

		FlipHeadFinal( (HeadCryptedServer*)buffer, (HeadDecrypted*)&head );
	};

	unsigned short DRCH( unsigned char* buffer )
	{
		Head head = { 0 };
		FlipHeadFinal( (HeadDecrypted*)&head, (HeadCryptedClient*)buffer );

		for( int i = 0; i < 5; i++ )
		{
			buffer[ i ] ^= m_Rt[ i ][ head.AddTableValue ];
		}

		FlipHeadMain( (HeadDecrypted*)&head, (HeadCryptedClient*)buffer );
		memcpy( buffer, &head, 5 );
		return head.AddBufferLen;
	};

	bool DRCB( unsigned char* buffer )
	{
		Head head = { 0 };
		memcpy( &head, buffer, 6 );
		unsigned short buflen = (unsigned short)( head.AddBufferLen - head.EncryptValue );

		unsigned char Checksum = 0;
		for( int i = 0; i < 5; i++ )
		{
			Checksum = CrcTable[ ((unsigned char*)&head)[i] ^ Checksum ];
		}
		for( int i = 6; i < buflen; i++ )
		{
			buffer[ i ] ^= m_Rt[ ( head.EncryptAddValue + i ) & 0xF ][ ( head.AddTableValue + i ) & 0x7FF ];
			Checksum = CrcTable[ buffer[i] ^ Checksum ];
		}

		if( Checksum != buffer[5] )
			return false;

		*((unsigned short*)&buffer[0]) = buflen;
		*((unsigned short*)&buffer[2]) = head.Command;

		return true;
	};

	bool Find_DupTABLE( int val, int table, int range )
	{
		for( int i = 0; i < table; i++ )
		{
			for( int j = 0; j < 0x7FF; j++ )
			{
				if( m_Rt[i][j] == val )
					return true;
			}
		}
		for( int i = 0; i < range; i++ )
		{
			if( m_Rt[table][i] == val )
				return true;
		}
		return false;
	};

	bool Find_DupINDEX( int val, int range )
	{
		for( int i = 0; i < range; i++ )
		{
			if( m_Ri[i] == val )
				return true;
		}
		return false;
	};

	void Default( )
	{
		m_Cr.Init( 0x0042D266 );
		for( int i = 0; i < 0x10; i++ )
		{
			m_Cr.SetType( i & 3 );
			for( int j = 0; j < 0x800; j++ )
			{
				do {
					m_Rt[i][j] = m_Cr.Get( );
				} while( Find_DupTABLE( m_Rt[i][j], 0, j ) );
			}
		}

		for( int i = 0; i < 0x200; i++ )
		{
			do {
				m_Ri[i] = m_Cr.R_AC( ) & 0x7FF;
			} while( Find_DupINDEX( m_Ri[i], i ) );
		}
	};

	void Init( int key )
	{
		unsigned char TableFuncList[ 0x11 ];

		m_Cr.Init( key );
		key = m_Cr.R_MY( );
		m_Cr.Init( key );

		for( int i = 0; i < 0x11; i++ )
			TableFuncList[ i ] = (unsigned char)m_Cr.R_BC( );

		for( int i = 0; i < 0x10; i++ )
		{
			m_Cr.SetType( TableFuncList[i] & 3 );
			for( int j = 0; j < 0x800; j++ )
			{
				do {
					m_Rt[i][j] = m_Cr.Get( );
				} while( Find_DupTABLE( m_Rt[i][j], 0, j ) );
			}
		}

		m_Cr.SetType( TableFuncList[0x10] );
		for( int i = 0; i < 0x200; i++ )
		{
			do {
				m_Ri[i] = m_Cr.Get( ) & 0x7FF;
			} while( Find_DupINDEX( m_Ri[i], i ) );
		}
	};

	CR001 m_Cr;
	int** m_Rt;
	unsigned short* m_Ri;
};

extern CPC g_Crypt;