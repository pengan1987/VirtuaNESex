//
// ランゲージダイアログクラス
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

#include "Plugin.h"

#include "Wnd.h"
#include "LanguageDlg.h"

DLG_MESSAGE_BEGIN(CLanguageDlg)
// メッセージ
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )
// コマンド
DLG_COMMAND_BEGIN()
DLG_ON_COMMAND( IDOK, OnOK )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_ON_COMMAND( IDDEFAULT, OnDefault )
DLG_COMMAND_END()
DLG_MESSAGE_END()

INT	CLanguageDlg::DoModal( HWND hWndParent )
{
	return	::DialogBoxParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_CFG_LANGUAGE),
				hWndParent, g_DlgProc, (LPARAM)this );
}

DLGMSG	CLanguageDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CLanguageDlg::OnInitDialog\n" );

	m_nPluginID = CPlugin::GetPluginID();

	for( INT i = 0; i < CPlugin::GetPluginNum(); i++ ) {
		::SendDlgItemMessage( m_hWnd, IDC_LNG_LIST, LB_INSERTSTRING, (WPARAM)i, (LPARAM)CPlugin::GetPluginLanguage(i) );
	}
	// デフォルト選択
	::SendDlgItemMessage( m_hWnd, IDC_LNG_LIST, LB_SETCURSEL, (WPARAM)m_nPluginID, 0 );

	return	TRUE;
}

DLGCMD	CLanguageDlg::OnOK( DLGCMDPARAM )
{
//	DEBUGOUT( "CLanguageDlg::OnOK\n" );
	INT nID = ::SendDlgItemMessage( m_hWnd, IDC_LNG_LIST, LB_GETCURSEL, 0, 0 );
	if( m_nPluginID != nID ) {
		CPlugin::SetPluginID( nID );
		::EndDialog( m_hWnd, IDOK );
	} else {
		::EndDialog( m_hWnd, IDCANCEL );
	}
}

DLGCMD	CLanguageDlg::OnCancel( DLGCMDPARAM )
{
//	DEBUGOUT( "CLanguageDlg::OnCancel\n" );
	::EndDialog( m_hWnd, IDCANCEL );
}

DLGCMD	CLanguageDlg::OnDefault( DLGCMDPARAM )
{
//	DEBUGOUT( "CLanguageDlg::OnDefault\n" );

	// デフォルト選択
	SendDlgItemMessage( m_hWnd, IDC_LNG_LIST, LB_SETCURSEL, (WPARAM)CPlugin::GetPluginID(), 0 );
}

