//
// ムービー情報ダイアログクラス
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
#include "MovieInfoDlg.h"

DLG_MESSAGE_BEGIN(CMovieInfoDlg)
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )

DLG_COMMAND_BEGIN()
DLG_ON_COMMAND( IDOK, OnOK )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_COMMAND_END()
DLG_MESSAGE_END()

INT	CMovieInfoDlg::DoModal( HWND hWndParent )
{
	return	::DialogBoxParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_MOVIEINFO),
				hWndParent, g_DlgProc, (LPARAM)this );
}

DLGMSG	CMovieInfoDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CMovieInfoDlg::OnInitDialog\n" );

	CHAR	szTemp[256];

	::wsprintf( szTemp, "%01d.%01d%01d",
		(m_wRecVersion&0xF00)>>8,
		(m_wRecVersion&0x0F0)>>4,
		(m_wRecVersion&0x00F) );
	::SetDlgItemText( m_hWnd, IDC_MIF_RERECORDVERSION, szTemp );

	::wsprintf( szTemp, "%04X", m_wVersion );
	::SetDlgItemText( m_hWnd, IDC_MIF_VERSION, szTemp );

	// フレーム数
	::wsprintf( szTemp, "%d frames", m_dwFrames );
	::SetDlgItemText( m_hWnd, IDC_MIF_FRAMES, szTemp );

	// 時間
	INT	t, h, m, s;
	t = (INT)m_dwFrames;
	h = t / (60*60*60);
	t -= h * (60*60*60);
	m = t / (60*60);
	t -= m * (60*60);
	s = t / 60;

	::wsprintf( szTemp, "%02d:%02d:%02d", h, m, s );
	::SetDlgItemText( m_hWnd, IDC_MIF_TIMES, szTemp );

	// 撮り直し回数
	if( m_dwRerecordTimes == 0xFFFFFFFF ) {
		::SetDlgItemText( m_hWnd, IDC_MIF_RERECORDTIMES, "----" );
	} else {
		::wsprintf( szTemp, "%d times", m_dwRerecordTimes );
		::SetDlgItemText( m_hWnd, IDC_MIF_RERECORDTIMES, szTemp );
	}

	return	TRUE;
}

DLGCMD	CMovieInfoDlg::OnOK( DLGCMDPARAM )
{
//	DEBUGOUT( "CMovieInfoDlg::OnOK\n" );

	::EndDialog( m_hWnd, IDOK );
}

DLGCMD	CMovieInfoDlg::OnCancel( DLGCMDPARAM )
{
//	DEBUGOUT( "CMovieInfoDlg::OnCancel\n" );

	::EndDialog( m_hWnd, IDCANCEL );
}

