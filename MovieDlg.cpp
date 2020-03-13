//
// バージョンダイアログクラス
//
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
using namespace std;

#include "VirtuaNESres.h"
#include "DebugOut.h"
#include "App.h"

#include "Wnd.h"
#include "MovieDlg.h"

DLG_MESSAGE_BEGIN(CMovieDlg)
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )

DLG_COMMAND_BEGIN()
DLG_ON_COMMAND( IDOK, OnOK )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_COMMAND_END()
DLG_MESSAGE_END()

INT	CMovieDlg::DoModal( HWND hWndParent )
{
	return	::DialogBoxParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_CFG_MOVIE),
				hWndParent, g_DlgProc, (LPARAM)this );
}

DLGMSG	CMovieDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CAboutDlg::OnInitDialog\n" );

	m_ConfigSave = Config.movie;
	BTNCHECK( IDC_MVI_1P_USE, Config.movie.bUsePlayer[0]?TRUE:FALSE );
	BTNCHECK( IDC_MVI_2P_USE, Config.movie.bUsePlayer[1]?TRUE:FALSE );
	BTNCHECK( IDC_MVI_3P_USE, Config.movie.bUsePlayer[2]?TRUE:FALSE );
	BTNCHECK( IDC_MVI_4P_USE, Config.movie.bUsePlayer[3]?TRUE:FALSE );
	BTNCHECK( IDC_MVI_RESETRECORD, Config.movie.bResetRec );
	BTNCHECK( IDC_MVI_RERECORD_DISABLE, !Config.movie.bRerecord );
	BTNCHECK( IDC_MVI_LOOPPLAY, Config.movie.bLoopPlay );
	BTNCHECK( IDC_MVI_PADDISPLAY, Config.movie.bPadDisplay );

	return	TRUE;
}

DLGCMD	CMovieDlg::OnOK( DLGCMDPARAM )
{
//	DEBUGOUT( "CMovieDlg::OnOK\n" );

	Config.movie.bUsePlayer[0] = IsBTNCHECK(IDC_MVI_1P_USE)?0xFF:0x00;
	Config.movie.bUsePlayer[1] = IsBTNCHECK(IDC_MVI_2P_USE)?0xFF:0x00;
	Config.movie.bUsePlayer[2] = IsBTNCHECK(IDC_MVI_3P_USE)?0xFF:0x00;
	Config.movie.bUsePlayer[3] = IsBTNCHECK(IDC_MVI_4P_USE)?0xFF:0x00;
	Config.movie.bResetRec = IsBTNCHECK( IDC_MVI_RESETRECORD );
	Config.movie.bRerecord = !IsBTNCHECK( IDC_MVI_RERECORD_DISABLE );
	Config.movie.bLoopPlay = IsBTNCHECK( IDC_MVI_LOOPPLAY );
	Config.movie.bPadDisplay = IsBTNCHECK( IDC_MVI_PADDISPLAY );

	// 全部OFFの場合は1Pだけ記録可能にしておく
	if( !Config.movie.bUsePlayer[0] && !Config.movie.bUsePlayer[1]
	 && !Config.movie.bUsePlayer[2] && !Config.movie.bUsePlayer[3] ) {
		Config.movie.bUsePlayer[0] = 0xFF;
	}

	::EndDialog( m_hWnd, IDOK );
}

DLGCMD	CMovieDlg::OnCancel( DLGCMDPARAM )
{
//	DEBUGOUT( "CMovieDlg::OnCancel\n" );
	Config.movie = m_ConfigSave;
	::EndDialog( m_hWnd, IDCANCEL );
}

