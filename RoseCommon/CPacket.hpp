#pragma once
#include "../Common/CDataPool.hpp"

struct CPacket
{
	union
	{
		struct
		{
			unsigned short m_Size;
			unsigned short m_Command;
			unsigned short m_Checksum;
			unsigned char m_Data[ 0x400 - 6 ];
		};
		unsigned char m_Buffer[ 0x400 ];
	};
};
extern CDataPool< CPacket > g_PacketPool;

//* -- PACKET FUNCTIONS!
// Initialization functions
inline void Packet_Copy( CPacket* dest, CPacket* src ) { memcpy( (void*)dest, (void*)src, src->m_Size ); };
inline void Packet_Start( CPacket* pak, int cmd ) { pak->m_Size = 6; pak->m_Command = cmd; };
inline void Packet_Start( CPacket* pak, int size, int cmd ) { pak->m_Size = size; pak->m_Command = cmd; };
inline void Packet_SetSize( CPacket* pak, int size ) { pak->m_Size = size; };

// Set functions
inline void Packet_SetCommand( CPacket* pak, int cmd ) { pak->m_Command = cmd; };
inline void Packet_SetByte( CPacket* pak, int pos, BYTE val ) { *((BYTE*)&pak->m_Data[pos]) = val; };
inline void Packet_SetWord( CPacket* pak, int pos, WORD val ) { *((WORD*)&pak->m_Data[pos]) = val; };
inline void Packet_SetDword( CPacket* pak, int pos, DWORD val ) { *((DWORD*)&pak->m_Data[pos]) = val; };

// Stream add functions
inline void Packet_AddByte( CPacket* pak, BYTE val ) { *((BYTE*)&pak->m_Buffer[pak->m_Size]) = val; pak->m_Size += sizeof(BYTE); };
inline void Packet_AddWord( CPacket* pak, WORD val ) { *((WORD*)&pak->m_Buffer[pak->m_Size]) = val; pak->m_Size += sizeof(WORD); };
inline void Packet_AddDword( CPacket* pak, DWORD val ) { *((DWORD*)&pak->m_Buffer[pak->m_Size]) = val; pak->m_Size += sizeof(DWORD); };
inline void Packet_AddFloat( CPacket* pak, float val ) { *((float*)&pak->m_Buffer[pak->m_Size]) = val; pak->m_Size += sizeof(float); };
inline void Packet_AddString( CPacket* pak, char* val ) { if( val != 0 ) { while( *val ) { Packet_AddByte( pak, *val++ ); } } Packet_AddByte( pak, 0 ); };
inline void Packet_AddBytes( CPacket* pak, unsigned char* val, int size ) { memcpy( &pak->m_Buffer[pak->m_Size], val, size ); pak->m_Size += size; };

// Get functions
inline BYTE Packet_GetByte( CPacket* pak, int pos ) { return *((BYTE*)&pak->m_Data[pos]); };
inline WORD Packet_GetWord( CPacket* pak, int pos ) { return *((WORD*)&pak->m_Data[pos]); };
inline DWORD Packet_GetDword( CPacket* pak, int pos ) { return *((DWORD*)&pak->m_Data[pos]); };
inline char* Packet_GetStringPtr( CPacket* pak, int pos ) { return (char*)&pak->m_Data[pos]; };
inline void Packet_GetString( CPacket* pak, int pos, char* outbuf ) { strcpy( outbuf, (char*)&pak->m_Data[pos] ); };
inline void Packet_GetBytes( CPacket* pak, int pos, int size, char* outbuf ) { memcpy( outbuf, &pak->m_Data[pos], size ); };

// Stream get functions ( Get & update position )
inline BYTE Packet_SGetByte( CPacket* pak, int& pos ) { BYTE a = *((BYTE*)&pak->m_Data[pos]); pos += sizeof(BYTE); return a; };
inline WORD Packet_SGetWord( CPacket* pak, int& pos ) { WORD a = *((WORD*)&pak->m_Data[pos]); pos += sizeof(WORD); return a; };
inline DWORD Packet_SGetDword( CPacket* pak, int& pos ) { DWORD a = *((DWORD*)&pak->m_Data[pos]); pos += sizeof(DWORD); return a; };
inline char* Packet_SGetStringPtr( CPacket* pak, int& pos ) { char* a = (char*)&pak->m_Data[pos]; pos += strlen(a)+1; return a; };
inline void Packet_SGetString( CPacket* pak, int& pos, char* outbuf ) { strcpy( outbuf, (char*)&pak->m_Data[pos] ); pos += strlen(outbuf)+1; };
inline void Packet_GSetBytes( CPacket* pak, int& pos, int size, char* outbuf ) { memcpy( outbuf, &pak->m_Data[pos], size ); pos += size; };