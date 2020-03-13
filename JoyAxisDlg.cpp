//
// ジョイスティック軸設定ダイアログクラス
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

#include "Wnd.h"
#include "JoyAxisDlg.h"

#include "DirectInput.h"

DLG_MESSAGE_BEGIN(CJoyAxisDlg)
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )
DLG_ON_MESSAGE( WM_TIMER,	OnTimer )

DLG_COMMAND_BEGIN()
DLG_ON_COMMAND( IDOK, OnOK )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_ON_COMMAND( IDDEFAULT, OnDefault )
DLG_ON_COMMAND_NOTIFY( IDC_AST_ID_COMBO, CBN_SELCHANGE, OnSelectChange )
DLG_ON_COMMAND_NOTIFY_RANGE( IDC_AST_XAXIS, IDC_AST_RZAXIS, BN_CLICKED, OnCheckChange )

DLG_COMMAND_END()
DLG_MESSAGE_END()

INT	CJoyAxisDlg::DoModal( HWND hWndParent )
{
	return	::DialogBoxParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_CFG_AXISSETTING),
				hWndParent, g_DlgProc, (LPARAM)this );
}

void	CJoyAxisDlg::OnJoySettingSetup( INT nID )
{
	BTNCHECK( IDC_AST_XAXIS,	m_JoyAxisSetting[nID]&(1<<0) );
	BTNCHECK( IDC_AST_YAXIS,	m_JoyAxisSetting[nID]&(1<<1) );
	BTNCHECK( IDC_AST_ZAXIS,	m_JoyAxisSetting[nID]&(1<<2) );
	BTNCHECK( IDC_AST_RXAXIS,	m_JoyAxisSetting[nID]&(1<<3) );
	BTNCHECK( IDC_AST_RYAXIS,	m_JoyAxisSetting[nID]&(1<<4) );
	BTNCHECK( IDC_AST_RZAXIS,	m_JoyAxisSetting[nID]&(1<<5) );
}

DLGMSG	CJoyAxisDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CJoyAxisDlg::OnInitDialog\n" );

	m_JoySel = 0;
	::memcpy( m_JoyAxisSetting, Config.general.JoyAxisSetting, 16*sizeof(WORD) );

	OnJoySettingSetup( m_JoySel );

	for( INT i = IDC_AST_XAXIS_PROGRESS; i <= IDC_AST_RZAXIS_PROGRESS; i++ ) {
		::SendDlgItemMessage( m_hWnd, i, PBM_SETRANGE32, (WPARAM)-10000, (LPARAM)10000 );
		::SendDlgItemMessage( m_hWnd, i, PBM_SETSTEP, (WPARAM)1, 0 ); 
	}

	// IDコンボボックス
	::SendDlgItemMessage( m_hWnd, IDC_AST_ID_COMBO, CB_RESETCONTENT, 0, 0 );
	CHAR	szStr[MAX_PATH+16];
	for( i = 0; i < DirectInput.m_nJoystickNum; i++ ) {
		::wsprintf( szStr, " %d : %s", i, DirectInput.m_JoyName[i].c_str() );
		::SendDlgItemMessage( m_hWnd, IDC_AST_ID_COMBO, CB_INSERTSTRING, (WPARAM)i, (LPARAM)szStr );
	}
	::SendDlgItemMessage( m_hWnd, IDC_AST_ID_COMBO, CB_SETCURSEL, 0, 0 );

	m_TimerID = ::SetTimer( m_hWnd, 1, 50, NULL );

	return	TRUE;
}

DLGMSG	CJoyAxisDlg::OnTimer( DLGMSGPARAM )
{
//	DEBUGOUT( "CJoyAxisDlg::OnTimer\n" );

	DirectInput.Poll();

	INT	i, j;
	for( i = IDC_AST_XAXIS_PROGRESS, j = 0; i <= IDC_AST_RZAXIS_PROGRESS; i++, j++ ) {
		::SendDlgItemMessage( m_hWnd, i, PBM_SETPOS, (WPARAM)DirectInput.m_JoyAxis[ m_JoySel ][ j ], 0 ); 
	}

	return	TRUE;
}

DLGCMD	CJoyAxisDlg::OnCheckChange( DLGCMDPARAM )
{
//	DEBUGOUT( "CJoyAxisDlg::OnCheckChange\n" );

	WORD	bits = 0;

	bits |= IsBTNCHECK( IDC_AST_XAXIS )?(1<<0):0;
	bits |= IsBTNCHECK( IDC_AST_YAXIS )?(1<<1):0;
	bits |= IsBTNCHECK( IDC_AST_ZAXIS )?(1<<2):0;
	bits |= IsBTNCHECK( IDC_AST_RXAXIS )?(1<<3):0;
	bits |= IsBTNCHECK( IDC_AST_RYAXIS )?(1<<4):0;
	bits |= IsBTNCHECK( IDC_AST_RZAXIS )?(1<<5):0;

	m_JoyAxisSetting[ m_JoySel ] = bits;
}

DLGNOTIFY CJoyAxisDlg::OnSelectChange( DLGCMDPARAM )
{
	m_JoySel = ::SendDlgItemMessage( m_hWnd, IDC_AST_ID_COMBO, CB_GETCURSEL, 0, 0 );
	OnJoySettingSetup( m_JoySel );
}

DLGCMD	CJoyAxisDlg::OnOK( DLGCMDPARAM )
{
//	DEBUGOUT( "CJoyAxisDlg::OnOK\n" );

	::memcpy( Config.general.JoyAxisSetting, m_JoyAxisSetting, 16*sizeof(WORD) );
	::KillTimer( m_hWnd, m_TimerID );

	::EndDialog( m_hWnd, IDOK );
}

DLGCMD	CJoyAxisDlg::OnCancel( DLGCMDPARAM )
{
//	DEBUGOUT( "CJoyAxisDlg::OnCancel\n" );

	::KillTimer( m_hWnd, m_TimerID );

	::EndDialog( m_hWnd, IDCANCEL );
}

DLGCMD	CJoyAxisDlg::OnDefault( DLGCMDPARAM )
{
//	DEBUGOUT( "CJoyAxisDlg::OnDefault\n" );

	::memset( m_JoyAxisSetting, 0, 16*sizeof(WORD) );
	OnJoySettingSetup( m_JoySel );
}

