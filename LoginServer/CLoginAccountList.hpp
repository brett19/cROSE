#pragma once
#include <string.h>
#include "../Common/CDataPool.hpp"
#include "../Common/CCriticalSection.hpp"
#include "../Common/CIndexList.hpp"

enum eACCSTATE
{
	eACCSTATE_DEFAULT,
	eACCSTATE_TRANSFERING,
	eACCSTATE_CONFIRMED,
	eACCSTATE_RESETADDED
};

struct CLoginAccount
{
	eACCSTATE m_State;
	char m_AccName[ 33 ];
	char m_Pass[ 33 ];
	unsigned int m_Right;
	unsigned int m_PayFlag;
	unsigned int m_ServerNo;
	unsigned char m_ChannelNo;
	unsigned int m_LoginTime;
	unsigned int m_TransferTime;
};
extern CDataPool<CLoginAccount> g_LoginAccountDataPool;

class CLoginAccountList
{
public:
	~CLoginAccountList( )
	{
		m_AccountsLock.Enter( );
		for( CIndexList<CLoginAccount*>::Iter i = m_Accounts.First( ); i.isValid( ); i++ )
			g_LoginAccountDataPool.Release( (*i).data );
		m_Accounts.Clear( );
		m_AccountsLock.Leave( );
	};

	void AddAccount( int id, CLoginAccount* acc )
	{
		m_AccountsLock.Enter( );
		m_Accounts.Append( id, acc );
		m_AccountsLock.Leave( );
	};

	CLoginAccount* FindAccount( int id )
	{
		m_AccountsLock.Enter( );
		CLoginAccount* acc = m_Accounts.Find( id );
		m_AccountsLock.Leave( );
		return acc;
	};

	CLoginAccount* FindAccount( char* accname )
	{
		CLoginAccount* acc = 0;
		m_AccountsLock.Enter( );
		for( CIndexList<CLoginAccount*>::Iter i = m_Accounts.First( ); i.isValid( ); i++ )
		{
			if( _strcmpi( (*i).data->m_AccName, accname ) == 0 )
			{
				acc = (*i).data;
				break;
			}
		}
		m_AccountsLock.Leave( );
		return acc;
	};

	CLoginAccount* FindAccountAndRemove( int id )
	{
		m_AccountsLock.Enter( );
		CLoginAccount* acc = m_Accounts.Remove( id );
		m_AccountsLock.Leave( );
		return acc;
	};

	CLoginAccount* FindAccountAndRemove( char* accname )
	{
		CLoginAccount* acc = 0;
		m_AccountsLock.Enter( );
		for( CIndexList<CLoginAccount*>::Iter i = m_Accounts.First( ); i.isValid( ); i++ )
		{
			if( _strcmpi( (*i).data->m_AccName, accname ) == 0 )
			{
				acc = (*i).data;
				m_Accounts.Pop( i );
				break;
			}
		}
		m_AccountsLock.Leave( );
		return acc;
	};

	unsigned int GetNewLsid( )
	{
		unsigned int newid = timeGetTime( );
		if( newid == m_LastLsid )
			newid++;
		m_LastLsid = newid;
		return newid;
	};

	unsigned int GetCount( )
	{
		int acccnt = 0;
		m_AccountsLock.Enter( );
		for( CIndexList<CLoginAccount*>::Iter i = m_Accounts.First( ); i.isValid( ); i++ )
		{
			if( (*i).data->m_TransferTime > GetTickCount( ) - 30000 )
				acccnt++;
		}
		m_AccountsLock.Leave( );
		return acccnt;
	};

	unsigned int GetRealCount( )
	{
		int acccnt = 0;
		m_AccountsLock.Enter( );
		for( CIndexList<CLoginAccount*>::Iter i = m_Accounts.First( ); i.isValid( ); i++ )
			acccnt++;
		m_AccountsLock.Leave( );
		return acccnt;
	};

	unsigned int GetLoggedCount( )
	{
		int acccnt = 0;
		m_AccountsLock.Enter( );
		for( CIndexList<CLoginAccount*>::Iter i = m_Accounts.First( ); i.isValid( ); i++ )
		{
			if( (*i).data->m_State == eACCSTATE_CONFIRMED || (*i).data->m_State == eACCSTATE_RESETADDED )
				acccnt++;
		}
		m_AccountsLock.Leave( );
		return acccnt;
	};

	DWORD m_LastLsid;
	CCriticalSection m_AccountsLock;
	CIndexList< CLoginAccount* > m_Accounts;
};

extern CLoginAccountList g_AccountList;