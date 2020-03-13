//
// バージョンダイアログクラス
//
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <string>
using namespace std;

#include "VirtuaNESres.h"
#include "DebugOut.h"
#include "App.h"
#include "Pathlib.h"
#include "Config.h"

#include "Wnd.h"
#include "NetPlayDlg.h"

#include "EmuThread.h"
#include "NetPlay.h"

DLG_MESSAGE_BEGIN(CNetPlayDlg)
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )
DLG_ON_MESSAGE( WM_NETPLAY_HOSTBYNAME, OnNetwordHostByName )
DLG_ON_MESSAGE( WM_NETPLAY_ACCEPT, OnNetworkAccept )
DLG_ON_MESSAGE( WM_NETPLAY_CONNECT, OnNetworkConnect )
DLG_ON_MESSAGE( WM_NETPLAY_CLOSE, OnNetworkClose )
DLG_ON_MESSAGE( WM_NETPLAY_ERROR, OnNetworkError )

DLG_COMMAND_BEGIN()
DLG_ON_COMMAND( IDC_NET_CONNECT, OnConnect )
DLG_ON_COMMAND( IDC_NET_SERVER, OnServer )
DLG_ON_COMMAND( IDC_NET_CLIENT, OnClient )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_COMMAND_END()
DLG_MESSAGE_END()

INT	CNetPlayDlg::DoModal( HWND hWndParent )
{
	m_hWndParent = hWndParent;

	return	::DialogBoxParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_NETPLAY),
				hWndParent, g_DlgProc, (LPARAM)this );
}

void	CNetPlayDlg::SetStatusMessage( UINT uID )
{
	CHAR	szTemp[256];
	CApp::LoadString( uID, szTemp, sizeof(szTemp) );
	::SetDlgItemText( m_hWnd, IDC_NET_STATUS, szTemp );
}

void	CNetPlayDlg::SetControlEnable( BOOL bEnable )
{
	if( bEnable ) {
		::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_PORT_COMBO ), IsBTNCHECK(IDC_NET_SERVER)?TRUE:FALSE );
		::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_HOST_COMBO ), IsBTNCHECK(IDC_NET_SERVER)?FALSE:TRUE );
		::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_LATENCY_COMBO ), IsBTNCHECK(IDC_NET_SERVER)?TRUE:FALSE );
		::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_SERVER ), TRUE );
		::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_CLIENT ), TRUE );
		::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_NICKNAME ), TRUE );
	} else {
		::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_PORT_COMBO ), FALSE );
		::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_HOST_COMBO ), FALSE );
		::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_LATENCY_COMBO ), FALSE );
		::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_SERVER ), FALSE );
		::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_CLIENT ), FALSE );
		::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_NICKNAME ), FALSE );
	}
}

void	CNetPlayDlg::AddRecentPort( LPCSTR lpszPort )
{
	// リストにあるかどうか
	BOOL	bFound = FALSE;
	INT	i, j;
	for( i = 0; i < 16; i++ ) {
		if( ::strlen(Config.netplay.szRecentPort[i]) <= 0 )
			break;
	}
	for( j = 0; j < i; j++ ) {
		if( ::strcmp( Config.netplay.szRecentPort[j], lpszPort ) == 0 ) {
			bFound = TRUE;
			break;
		}
	}
	if( j == 16 )
		j--;
	for( ; j > 0; j-- ) {
		::lstrcpy( Config.netplay.szRecentPort[j], Config.netplay.szRecentPort[j-1] );
	}
	::strcpy( Config.netplay.szRecentPort[0], lpszPort );
	if( !bFound ) {
		Config.netplay.nRecentPort++;
	}
}


void	CNetPlayDlg::AddRecentHost( LPCSTR lpszHost )
{
	// リストにあるかどうか
	BOOL	bFound = FALSE;
	INT	i, j;
	for( i = 0; i < 16; i++ ) {
		if( ::strlen(Config.netplay.szRecentHost[i]) <= 0 )
			break;
	}
	for( j = 0; j < i; j++ ) {
		if( ::strcmp( Config.netplay.szRecentHost[j], lpszHost ) == 0 ) {
			bFound = TRUE;
			break;
		}
	}
	if( j == 16 )
		j--;
	for( ; j > 0; j-- ) {
		::lstrcpy( Config.netplay.szRecentHost[j], Config.netplay.szRecentHost[j-1] );
	}
	::strcpy( Config.netplay.szRecentHost[0], lpszHost );
	if( !bFound ) {
		Config.netplay.nRecentHost++;
	}
}

DLGMSG	CNetPlayDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CNetPlayDlg::OnInitDialog\n" );

	INT	i;
	// ポート
	::SendDlgItemMessage( m_hWnd, IDC_NET_PORT_COMBO, CB_RESETCONTENT, 0, 0 ); 
	for( i = 0; i < Config.netplay.nRecentPort; i++ ) {
		::SendDlgItemMessage( m_hWnd, IDC_NET_PORT_COMBO, CB_INSERTSTRING, -1, (LPARAM)Config.netplay.szRecentPort[i] );
	}
	::SendDlgItemMessage( m_hWnd, IDC_NET_PORT_COMBO, CB_SETCURSEL, 0, 0 ); 

	// レイテンシ
	::SendDlgItemMessage( m_hWnd, IDC_NET_LATENCY_COMBO, CB_RESETCONTENT, 0, 0 ); 
	for( i = 0; i < 9; i++ ) {
		CHAR	szStr[64];
		::wsprintf( szStr,  "%d Frame buffer", i+1 );
		::SendDlgItemMessage( m_hWnd, IDC_NET_LATENCY_COMBO, CB_INSERTSTRING, -1, (LPARAM)szStr );
	}
	::SendDlgItemMessage( m_hWnd, IDC_NET_LATENCY_COMBO, CB_SETCURSEL, 0, 0 ); 

	// ホストリスト
	::SendDlgItemMessage( m_hWnd, IDC_NET_HOST_COMBO, CB_RESETCONTENT, 0, 0 ); 
	for( i = 0; i < Config.netplay.nRecentHost; i++ ) {
		::SendDlgItemMessage( m_hWnd, IDC_NET_HOST_COMBO, CB_INSERTSTRING, -1, (LPARAM)Config.netplay.szRecentHost[i] );
	}
	::SendDlgItemMessage( m_hWnd, IDC_NET_HOST_COMBO, CB_SETCURSEL, 0, 0 ); 

	// ニックネーム
	::SetDlgItemText( m_hWnd, IDC_NET_NICKNAME, Config.netplay.szNick );

	::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_HOST_COMBO ), FALSE );

	::CheckRadioButton( m_hWnd, IDC_NET_SERVER, IDC_NET_CLIENT, IDC_NET_SERVER );
	::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_LATENCY_COMBO ), IsBTNCHECK(IDC_NET_SERVER)?TRUE:FALSE );

	SetStatusMessage( IDS_NET_NOCONNECT );

	return	TRUE;
}

DLGMSG	CNetPlayDlg::OnNetwordHostByName( DLGMSGPARAM )
{
DEBUGOUT( "CNetPlayDlg::OnNetwordHostByName [%08X][%08X]\n", wParam, lParam );

	HRESULT	hResult = NetPlay.ASyncWndProc( hWnd, wParam, lParam );

	if( hResult ) {
		struct	in_addr	addr;
		addr.s_addr = hResult;
		LPSTR	pHost = inet_ntoa( addr );

DEBUGOUT( "Host:%s\n", pHost );

		// エラーとかその他もろもろをこのウインドウで受け取る為
		NetPlay.SetMsgWnd( m_hWnd );

		if( NetPlay.Connect( FALSE, pHost, m_uPort ) ) {
			SetStatusMessage( IDS_NET_CONNECTING );
			::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_CONNECT ), FALSE );
			SetControlEnable( FALSE );
		} else {
DEBUGOUT( "CNetPlayDlg::OnNetwordHostByName Connect error.\n" );
			SetStatusMessage( IDS_ERROR_NETWORKERROR );
			::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_CONNECT ), TRUE );
			SetControlEnable( TRUE );
		}
	} else {
DEBUGOUT( "CNetPlayDlg::OnNetwordHostByName error.\n" );
		SetStatusMessage( IDS_ERROR_NETWORKERROR );
		::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_CONNECT ), TRUE );

		SetControlEnable( TRUE );
	}

	return	TRUE;
}

DLGMSG	CNetPlayDlg::OnNetworkAccept( DLGMSGPARAM )
{
BYTE	databuf;
DWORD	dwStartTime, dwEndTime;
BYTE	nServerLatency, nClientLatency;
BYTE	nServerStep, nClientStep;
INT	nLatency;
BOOL	bRet = FALSE;
DWORD	dwCRC;
INT	i;

	// ニックネームを保存
	CHAR	szNick[_MAX_PATH+1];
	::GetWindowText( ::GetDlgItem( m_hWnd, IDC_NET_NICKNAME ), szNick, _MAX_PATH );
	::lstrcpy( Config.netplay.szNick, szNick );

	// 最近使ったポートの更新
	CHAR	szPort[_MAX_PATH+1];
	::GetWindowText( ::GetDlgItem( m_hWnd, IDC_NET_PORT_COMBO ), szPort, _MAX_PATH );
	AddRecentPort( szPort );

	// 通信レイテンシの計測
	CHAR	str[256];
	::wsprintf( str, "VirtuaNES version %01d.%01d%01d",
		    (VIRTUANES_VERSION&0xF00)>>8,
		    (VIRTUANES_VERSION&0x0F0)>>4,
		    (VIRTUANES_VERSION&0x00F) );

	dwStartTime = ::timeGetTime();
	for( i = 0; i < ::strlen(str); i++ ) {
		if( NetPlay.Send( (BYTE)str[i] ) ) {
			goto	_accept_error;
		}
		if( NetPlay.RecvTime( databuf, 10*1000 ) < 0 ) {
			goto	_accept_error;
		} else {
			if( databuf != (BYTE)str[i] ) {
				SetStatusMessage( IDS_ERROR_NETWORKERROR_VERSION );
				goto	_accept_error2;
			}
		}
	}
	dwEndTime = ::timeGetTime()-dwStartTime;

	DEBUGOUT( "Server: Transfer Bytes: %d byte / Time: %d ms\n", ::strlen(str), dwEndTime );

	// イメージのCRCを確認
	NES*	pNes;
	if( !(pNes = Emu.GetNES()) ) {
		goto	_accept_error;
	}
	if( pNes->rom->GetMapperNo() != 20 ) {
		dwCRC = pNes->rom->GetPROM_CRC();
	} else {
		dwCRC = pNes->rom->GetGameID();
	}
	for( i = 0; i < 32; i+=8 ) {
		if( NetPlay.Send( (BYTE)(dwCRC>>i) ) ) {
			goto	_accept_error;
		}
		if( NetPlay.RecvTime( databuf, 10*1000 ) < 0 ) {
			goto	_accept_error;
		} else if( databuf != (BYTE)(dwCRC>>i) ) {
			SetStatusMessage( IDS_ERROR_NETWORKERROR_CRC );
			goto	_accept_error2;
		}
	}

	// レイテンシ設定
	nServerLatency = m_nNetLatency;
	if( nServerLatency < 0 ) {
		nServerLatency = 0;
	}
	if( nServerLatency > 8 ) {
		nServerLatency = 8;
	}
DEBUGOUT( "Latancy:%d\n", nServerLatency );
	nServerStep    = 0;
	nClientStep    = 0;

	// Send Latency
	if( NetPlay.Send( nServerLatency ) ) {
		goto	_accept_error;
	}
	// Recv Latency dummy
	if( NetPlay.RecvTime( nClientLatency, 10*1000 ) < 0 ) {
		goto	_accept_error;
	}
	// Send Step
	if( NetPlay.Send( nServerStep ) ) {
		goto	_accept_error;
	}
	// Recv Step dummy
	if( NetPlay.RecvTime( nClientStep, 10*1000 ) < 0 ) {
		goto	_accept_error;
	}

	nLatency = nServerLatency;
	DEBUGOUT( "Server: Network Latency:%d frames\n", nLatency );

	NetPlay.SetLatency( nLatency );
	NetPlay.SetMsgWnd( m_hWndParent );

	::EndDialog( m_hWnd, IDOK );
	return	TRUE;
_accept_error:
	SetStatusMessage( IDS_ERROR_NETWORKERROR );
_accept_error2:
	NetPlay.Disconnect();
	::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_CONNECT ), TRUE );

	SetControlEnable( TRUE );
	return	TRUE;
}

DLGMSG	CNetPlayDlg::OnNetworkConnect( DLGMSGPARAM )
{
BYTE	databuf;
DWORD	dwStartTime, dwEndTime;
BYTE	nServerLatency, nClientLatency;
BYTE	nServerStep, nClientStep;
INT	nLatency;
DWORD	dwCRC;
INT	i;

	// ニックネームを保存
	CHAR	szNick[_MAX_PATH+1];
	::GetWindowText( ::GetDlgItem( m_hWnd, IDC_NET_NICKNAME ), szNick, _MAX_PATH );
	::lstrcpy( Config.netplay.szNick, szNick );
DEBUGOUT( "cnn Nick:%s\n", szNick );

	// 最近使ったポートの更新
	CHAR	szHost[_MAX_PATH+1];
	::GetWindowText( ::GetDlgItem( m_hWnd, IDC_NET_HOST_COMBO ), szHost, _MAX_PATH );
DEBUGOUT( "cnn Host:%s\n", szHost );
	AddRecentHost( szHost );

	// 通信レイテンシの計測
	CHAR	str[256];
	::wsprintf( str, "VirtuaNES version %01d.%01d%01d",
		    (VIRTUANES_VERSION&0xF00)>>8,
		    (VIRTUANES_VERSION&0x0F0)>>4,
		    (VIRTUANES_VERSION&0x00F) );

	dwStartTime = ::timeGetTime();
	for( i = 0; i < ::strlen(str); i++ ) {
		if( NetPlay.Send( (BYTE)str[i] ) ) {
			goto	_connect_error;
		}
		if( NetPlay.RecvTime( databuf, 10*1000 ) < 0 ) {
			goto	_connect_error;
		} else {
			if( databuf != str[i] ) {
				SetStatusMessage( IDS_ERROR_NETWORKERROR_VERSION );
				goto	_connect_error2;
			}
		}
	}
	dwEndTime = ::timeGetTime()-dwStartTime;

	DEBUGOUT( "Client: Transfer Bytes: %d byte / Time: %d ms\n", ::strlen(str), dwEndTime );

	// イメージのCRCを確認
	NES*	pNes;
	if( !(pNes = Emu.GetNES()) ) {
		goto	_connect_error;
	}
	if( pNes->rom->GetMapperNo() != 20 ) {
		dwCRC = pNes->rom->GetPROM_CRC();
	} else {
		dwCRC = pNes->rom->GetGameID();
	}
	for( i = 0; i < 32; i+=8 ) {
		if( NetPlay.Send( (BYTE)(dwCRC>>i) ) ) {
			goto	_connect_error;
		}
		if( NetPlay.RecvTime( databuf, 10*1000 ) < 0 ) {
			goto	_connect_error;
		} else if( databuf != (BYTE)(dwCRC>>i) ) {
			SetStatusMessage( IDS_ERROR_NETWORKERROR_CRC );
			goto	_connect_error2;
		}
	}

	nClientLatency = 0;
	nServerStep    = 0;
	nClientStep    = 0;

	// Send Latency dummy
	if( NetPlay.Send( nClientLatency ) ) {
		goto	_connect_error;
	}
	// Recv Latency
	if( NetPlay.RecvTime( nServerLatency, 10*1000 ) < 0 ) {
		goto	_connect_error;
	}
	// Send Step
	if( NetPlay.Send( nClientStep ) ) {
		goto	_connect_error;
	}
	// Recv Step dummy
	if( NetPlay.RecvTime( nServerStep, 10*1000 ) < 0 ) {
		goto	_connect_error;
	}

	nLatency = nServerLatency;
	DEBUGOUT( "Client: Network Latency:%d frames\n", nLatency );

	NetPlay.SetLatency( nLatency );
	NetPlay.SetMsgWnd( m_hWndParent );

	::EndDialog( m_hWnd, IDOK );
	return	TRUE;
_connect_error:
	SetStatusMessage( IDS_ERROR_NETWORKERROR );
_connect_error2:
	NetPlay.Disconnect();
	::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_CONNECT ), TRUE );

	SetControlEnable( TRUE );
	return	TRUE;
}

DLGMSG	CNetPlayDlg::OnNetworkClose( DLGMSGPARAM )
{
	NetPlay.SetMsgWnd( NULL );
	SetStatusMessage( IDS_ERROR_NETWORKDISCONNECT );
	::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_CONNECT ), TRUE );
	SetControlEnable( TRUE );
	return	TRUE;
}

DLGMSG	CNetPlayDlg::OnNetworkError( DLGMSGPARAM )
{
	NetPlay.SetMsgWnd( NULL );
	SetStatusMessage( IDS_ERROR_NETWORKERROR );
	::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_CONNECT ), TRUE );
	SetControlEnable( TRUE );
	return	TRUE;
}

DLGCMD	CNetPlayDlg::OnServer( DLGCMDPARAM )
{
	DEBUGOUT( "CNetPlayDlg::OnServer\n" );
	SetControlEnable( TRUE );
}

DLGCMD	CNetPlayDlg::OnClient( DLGCMDPARAM )
{
	DEBUGOUT( "CNetPlayDlg::OnClient\n" );
	SetControlEnable( TRUE );
}

DLGCMD	CNetPlayDlg::OnConnect( DLGCMDPARAM )
{
	DEBUGOUT( "CNetPlayDlg::OnConnect\n" );

	CHAR	szPort[_MAX_PATH+1];
	CHAR	szHost[_MAX_PATH+1];
	CHAR	szHostName[_MAX_PATH+1];
	CHAR	szNick[_MAX_PATH+1];

	if( IsBTNCHECK(IDC_NET_SERVER) ) {
	// サーバー
		::GetWindowText( ::GetDlgItem( m_hWnd, IDC_NET_PORT_COMBO ), szPort, _MAX_PATH );
		::GetWindowText( ::GetDlgItem( m_hWnd, IDC_NET_NICKNAME ), szNick, _MAX_PATH );

		INT	porttemp = ::atoi( szPort );
		if( porttemp < 1024 || porttemp > 65535 ) {
			SetStatusMessage( IDS_ERROR_NETWORKERROR );
			return;
		}
		m_uPort = porttemp;

		m_nNetLatency = ::SendDlgItemMessage( m_hWnd, IDC_NET_LATENCY_COMBO, CB_GETCURSEL, 0, 0 );

DEBUGOUT( "Server Port:%d\n", m_uPort );
DEBUGOUT( "Latency    :%d\n", m_nNetLatency );
	} else {
	// クライアント
		::GetWindowText( ::GetDlgItem( m_hWnd, IDC_NET_HOST_COMBO ), szHost, _MAX_PATH );
		::GetWindowText( ::GetDlgItem( m_hWnd, IDC_NET_NICKNAME ), szNick, _MAX_PATH );

DEBUGOUT( "Host and Addr: %s\n", szHost );
DEBUGOUT( "NickName     : %s\n", szNick );

		INT	i;
		BOOL	bFound;
		bFound = FALSE;

		::lstrcpy( szHostName, szHost );
		for( i = 0; i < ::strlen( szHostName ); i++ ) {
			if( szHostName[i] == ':' ) {
				bFound = TRUE;
				break;
			}
		}
		if( !bFound ) {
			SetStatusMessage( IDS_ERROR_NETWORKERROR );
			return;
		}

		szHostName[i] = '\0';
		LPSTR	lpszPort = &szHostName[i+1];

		INT	porttemp = ::atoi( lpszPort );

		if( porttemp < 1024 || porttemp > 65535 ) {
			SetStatusMessage( IDS_ERROR_NETWORKERROR );
			return;
		}

		m_uPort = porttemp;
DEBUGOUT( "Host:%s Port:%d\n", szHostName, m_uPort );

		m_nNetLatency = ::SendDlgItemMessage( m_hWnd, IDC_NET_LATENCY_COMBO, CB_GETCURSEL, 0, 0 ); 
		if( m_nNetLatency == CB_ERR ) {
			SetStatusMessage( IDS_ERROR_UNKNOWN );
			return;
		}
	}

	if( IsBTNCHECK(IDC_NET_SERVER) ) {
		// エラーとかその他もろもろをこのウインドウで受け取る為
		NetPlay.SetMsgWnd( m_hWnd );

		if( NetPlay.Connect( TRUE, szHostName, m_uPort ) ) {
			SetStatusMessage( IDS_NET_ACCEPTING );

			::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_CONNECT ), FALSE );
			SetControlEnable( FALSE );
		} else {
			SetStatusMessage( IDS_ERROR_NETWORKERROR );
			DEBUGOUT( "CNetPlayDlg::OnConnect error.\n" );
		}
	} else {
		SetStatusMessage( IDS_NET_CONNECTING );

		INT	ret = NetPlay.ASyncHostCheck( m_hWnd, szHostName );

		if( ret == 0 ) {
			// エラーとかその他もろもろをこのウインドウで受け取る為
			NetPlay.SetMsgWnd( m_hWnd );

			if( NetPlay.Connect( FALSE, szHostName, m_uPort ) ) {
				SetStatusMessage( IDS_NET_CONNECTING );

				::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_CONNECT ), FALSE );
				SetControlEnable( FALSE );
			} else {
				SetStatusMessage( IDS_ERROR_NETWORKERROR );
				DEBUGOUT( "CNetPlayDlg::OnConnect error.\n" );
			}
		} else if( ret > 0 ) {
			::EnableWindow( ::GetDlgItem( m_hWnd, IDC_NET_CONNECT ), FALSE );
			SetControlEnable( FALSE );
		} else if( ret < 0 ) {
			SetStatusMessage( IDS_ERROR_NETWORKERROR );
			DEBUGOUT( "CNetPlayDlg::OnConnect error.\n" );
		}
	}

//	::EndDialog( m_hWnd, IDOK );
}

DLGCMD	CNetPlayDlg::OnCancel( DLGCMDPARAM )
{
	DEBUGOUT( "CNetPlayDlg::OnCancel\n" );

	NetPlay.Disconnect();

	::EndDialog( m_hWnd, IDCANCEL );
}

