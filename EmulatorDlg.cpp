//
// エミュレータダイアログクラス
//
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
using namespace std;

#include "VirtuaNESres.h"
#include "DebugOut.h"
#include "App.h"
#include "Pathlib.h"

#include "Wnd.h"
#include "EmulatorDlg.h"

DLG_MESSAGE_BEGIN(CEmulatorDlg)
// メッセージ
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )
DLG_ON_MESSAGE( WM_HSCROLL,	OnHScroll )
// コマンド
DLG_COMMAND_BEGIN()
DLG_ON_COMMAND( IDOK, OnOK )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_ON_COMMAND( IDDEFAULT, OnDefault )
DLG_COMMAND_END()
DLG_MESSAGE_END()

INT	CEmulatorDlg::DoModal( HWND hWndParent )
{
	return	::DialogBoxParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_CFG_EMULATOR),
				hWndParent, g_DlgProc, (LPARAM)this );
}

void	CEmulatorDlg::OnInitialMember()
{
	BTNCHECK( IDC_EMU_ILLEGALOP,      Config.emulator.bIllegalOp );
	BTNCHECK( IDC_EMU_FOURPLAYER,     Config.emulator.bFourPlayer );
	BTNCHECK( IDC_EMU_AUTOFRAMESKIP,  Config.emulator.bAutoFrameSkip );
	BTNCHECK( IDC_EMU_THROTTLE,       Config.emulator.bThrottle );
	BTNCHECK( IDC_EMU_BACKGROUND,     Config.emulator.bBackground );

	BTNCHECK( IDC_EMU_CRCCHECK,       Config.emulator.bCrcCheck );
	BTNCHECK( IDC_EMU_DISKTHROTTLE,   Config.emulator.bDiskThrottle );
	BTNCHECK( IDC_EMU_LOADFULLSCREEN, Config.emulator.bLoadFullscreen );

	BTNCHECK( IDC_EMU_DOUBLEEXECUTE,  Config.general.bDoubleExecute );
	BTNCHECK( IDC_EMU_STARTUPLAUNCHER,Config.general.bStartupLauncher );

	BTNCHECK( IDC_EMU_PNGSNAPSHOT,    Config.emulator.bPNGsnapshot );

	// スライダ
	::SendDlgItemMessage( m_hWnd, IDC_EMU_THROTTLE_SLIDER, TBM_SETRANGE, TRUE, MAKELONG(1,60) );
	::SendDlgItemMessage( m_hWnd, IDC_EMU_THROTTLE_SLIDER, TBM_SETPOS, TRUE, (WPARAM)Config.emulator.nThrottleFPS/10 );
	::SendDlgItemMessage( m_hWnd, IDC_EMU_THROTTLE_SLIDER, TBM_SETTICFREQ, 2, 0 );
	::SendDlgItemMessage( m_hWnd, IDC_EMU_THROTTLE_SLIDER, TBM_SETLINESIZE, 0, 1 );
	::SendDlgItemMessage( m_hWnd, IDC_EMU_THROTTLE_SLIDER, TBM_SETPAGESIZE, 0, 1 );

	// FPS値
	CHAR	str[64];
	::wsprintf( str, "%d", Config.emulator.nThrottleFPS );
	::SetDlgItemText( m_hWnd, IDC_EMU_FPS, str );

	// プライオリティコンボボックス
	::SendDlgItemMessage( m_hWnd, IDC_EMU_PRIORITY_COMBO, CB_RESETCONTENT, 0, 0 );
	INT	i, j;
	for( i = IDS_EMU_PRIORITY_IDLE, j = 0; i <= IDS_EMU_PRIORITY_REALTIME; i++, j++ ) {
		CApp::LoadString( i, str, sizeof(str) );
		::SendDlgItemMessage( m_hWnd, IDC_EMU_PRIORITY_COMBO, CB_INSERTSTRING, (WPARAM)j, (LPARAM)str );
	}

	::SendDlgItemMessage( m_hWnd, IDC_EMU_PRIORITY_COMBO, CB_SETCURSEL, (WPARAM)Config.emulator.nPriority, 0 );
}

DLGMSG	CEmulatorDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CEmulatorDlg::OnInitDialog\n" );

	m_ConfigSave = Config.emulator;
	OnInitialMember();

	return	TRUE;
}

DLGMSG	CEmulatorDlg::OnHScroll( DLGMSGPARAM )
{
//	DEBUGOUT( "CEmulatorDlg::OnHScroll\n" );

	INT pos = ::SendDlgItemMessage( m_hWnd, IDC_EMU_THROTTLE_SLIDER, TBM_GETPOS, 0, 0 );
	CHAR	str[16];
	::wsprintf( str, "%d", pos*10 );
	::SetDlgItemText( m_hWnd, IDC_EMU_FPS, str );

	return	FALSE;
}

DLGCMD	CEmulatorDlg::OnOK( DLGCMDPARAM )
{
//	DEBUGOUT( "CEmulatorDlg::OnOK\n" );

	Config.emulator.bIllegalOp      = IsBTNCHECK( IDC_EMU_ILLEGALOP );
	Config.emulator.bFourPlayer     = IsBTNCHECK( IDC_EMU_FOURPLAYER );
	Config.emulator.bAutoFrameSkip  = IsBTNCHECK( IDC_EMU_AUTOFRAMESKIP );
	Config.emulator.bThrottle       = IsBTNCHECK( IDC_EMU_THROTTLE );
	Config.emulator.bBackground     = IsBTNCHECK( IDC_EMU_BACKGROUND );

	Config.emulator.bCrcCheck       = IsBTNCHECK( IDC_EMU_CRCCHECK );
	Config.emulator.bDiskThrottle   = IsBTNCHECK( IDC_EMU_DISKTHROTTLE );
	Config.emulator.bLoadFullscreen = IsBTNCHECK( IDC_EMU_LOADFULLSCREEN );

	Config.general.bDoubleExecute   = IsBTNCHECK( IDC_EMU_DOUBLEEXECUTE );
	Config.general.bStartupLauncher = IsBTNCHECK( IDC_EMU_STARTUPLAUNCHER );

	Config.emulator.bPNGsnapshot    = IsBTNCHECK( IDC_EMU_PNGSNAPSHOT );

	Config.emulator.nThrottleFPS = 10 * ::SendDlgItemMessage( m_hWnd, IDC_EMU_THROTTLE_SLIDER, TBM_GETPOS, 0, 0 );
	Config.emulator.nPriority = ::SendDlgItemMessage( m_hWnd, IDC_EMU_PRIORITY_COMBO, CB_GETCURSEL, 0, 0 );

	::EndDialog( m_hWnd, IDOK );
}

DLGCMD	CEmulatorDlg::OnCancel( DLGCMDPARAM )
{
//	DEBUGOUT( "CEmulatorDlg::OnCancel\n" );
	Config.emulator = m_ConfigSave;
	::EndDialog( m_hWnd, IDCANCEL );
}

DLGCMD	CEmulatorDlg::OnDefault( DLGCMDPARAM )
{
//	DEBUGOUT( "CEmulatorDlg::OnDefault\n" );
	Config.emulator.Default();
	OnInitialMember();
}

