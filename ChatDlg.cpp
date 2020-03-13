//
// チャットダイアログクラス
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <mbstring.h>
#include <shlwapi.h>

#include <string>
using namespace std;

#include "typedef.h"
#include "macro.h"

#include "VirtuaNESres.h"
#include "DebugOut.h"
#include "App.h"
#include "Pathlib.h"
#include "Config.h"

#include "NetPlay.h"

#include "Wnd.h"
#include "ChatDlg.h"

// メッセージ
DLG_MESSAGE_BEGIN(CChatDlg)
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )
DLG_ON_MESSAGE( WM_DESTROY,	OnDestroy )
DLG_ON_MESSAGE( WM_CLOSE,	OnClose )
DLG_ON_MESSAGE( WM_ACTIVATE,	OnActivate )
DLG_ON_MESSAGE( WM_SETCURSOR,	OnSetCursor )
DLG_ON_MESSAGE( WM_SIZE,	OnSize )
DLG_ON_MESSAGE( WM_CTLCOLORSTATIC, OnControlColorStatic )
DLG_ON_MESSAGE( WM_COPYDATA,	OnCopyData )

// コマンド
DLG_COMMAND_BEGIN()
DLG_ON_COMMAND_NOTIFY( IDC_NCT_MESSAGE, EN_SETFOCUS, OnMessageFocus )
DLG_ON_COMMAND( IDOK, OnOK )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_ON_COMMAND( IDC_NCT_SEND, OnSend )
DLG_COMMAND_END()
// Notify メッセージ
DLG_NOTIFY_BEGIN()
DLG_NOTIFY_END()
DLG_MESSAGE_END()

BOOL	CChatDlg::Create( HWND hWndParent )
{
	// 親はデスクトップにする
	m_hWnd = ::CreateDialogParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_NETPLAY_CHAT),
				NULL, g_DlgProc, (LPARAM)this );
	if( !m_hWnd )
		return	FALSE;

	// モードレスダイアログリストに加える
	CWndList::Add( this );

	return	TRUE;
}

void	CChatDlg::Destroy()
{
	if( m_hWnd ) {
		// モードレスダイアログリストから削除
		CWndList::Del( this );

		::GetWindowRect( m_hWnd, &Config.netplay.rcChatPos );

		::DestroyWindow( m_hWnd );
		m_hWnd = NULL;
	}
}

void	CChatDlg::SetEditText()
{
	CHAR	szText[256+1];

	::GetWindowText( ::GetDlgItem(m_hWnd, IDC_NCT_EDIT), szText, sizeof(szText)-1 );
	::SetWindowText( ::GetDlgItem(m_hWnd, IDC_NCT_EDIT), "" );

	if( ::strlen(szText) ) {
		// clear message window
		if( ::StrCmpI( szText, "/clear" ) == 0 ) {
			::SendDlgItemMessage( m_hWnd, IDC_NCT_MESSAGE, EM_SETSEL, 0, -1 );
			::SendDlgItemMessage( m_hWnd, IDC_NCT_MESSAGE, EM_REPLACESEL, FALSE, (WPARAM)"" );
			return;
		}

		string	str;

		if( NetPlay.IsConnect() ) {
			str = "(";
			str = str + Config.netplay.szNick;
			str = str + ") ";
		}

		str = str + szText;
		str = str + "\r\n";

		// 相手に送信
		if( NetPlay.IsConnect() ) {
			NetPlay.ChatSend( (LPSTR)str.c_str() );
		}

		// 自分自身のメッセージを表示
		INT n = GetWindowTextLength( GetDlgItem( m_hWnd, IDC_NCT_MESSAGE ) );
		::SendDlgItemMessage( m_hWnd, IDC_NCT_MESSAGE, EM_SETSEL, (WPARAM)n, (LPARAM)n );
		::SendDlgItemMessage( m_hWnd, IDC_NCT_MESSAGE, EM_REPLACESEL, (WPARAM)TRUE, (LPARAM)str.c_str() );
	}
}

DLGMSG	CChatDlg::OnCopyData( DLGMSGPARAM )
{
	COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)lParam;
	CHAR*	lpStr = (CHAR*)pcds->lpData;

	INT n = GetWindowTextLength( GetDlgItem( hWnd, IDC_NCT_MESSAGE ) );
	::SendDlgItemMessage( hWnd, IDC_NCT_MESSAGE, EM_SETSEL, (WPARAM)n, (LPARAM)n );
	::SendDlgItemMessage( hWnd, IDC_NCT_MESSAGE, EM_REPLACESEL, (WPARAM)TRUE, (LPARAM)lpStr );

	if( ::GetFocus() != m_hWnd && ::strlen( lpStr ) > 0 ) {
		::MessageBeep( MB_OK );
//		::PlaySound( "MailBeep", NULL, SND_ALIAS|SND_ASYNC );
	}

	// メッセージが来たらポップアップさせるため
	::SendMessage( CApp::GetHWnd(), WM_VNS_CHATPOPUP, 0, 0 );

	return	TRUE;
}

DLGMSG	CChatDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CChatDlg::OnInitDialog\n" );

	NetPlay.SetChatWnd( m_hWnd );

	// 位置情報を保存
	::GetClientRect( m_hWnd, &m_rcClient );
	::GetWindowRect( ::GetDlgItem(m_hWnd, IDC_NCT_MESSAGE), &m_rcMessage );
	::GetWindowRect( ::GetDlgItem(m_hWnd, IDC_NCT_EDIT), &m_rcEdit );
	::GetWindowRect( ::GetDlgItem(m_hWnd, IDC_NCT_SEND), &m_rcButton );

	// クライアント座標への変換
	::ScreenToClient( m_hWnd, (POINT*)&m_rcMessage.left );
	::ScreenToClient( m_hWnd, (POINT*)&m_rcMessage.right );
	::ScreenToClient( m_hWnd, (POINT*)&m_rcEdit.left );
	::ScreenToClient( m_hWnd, (POINT*)&m_rcEdit.right );
	::ScreenToClient( m_hWnd, (POINT*)&m_rcButton.left );
	::ScreenToClient( m_hWnd, (POINT*)&m_rcButton.right );

	// ウインドウ位置/サイズの設定
//	RECT	rc = Config.launcher.rcWindowPos;
//	if( (rc.right-rc.left) && (rc.bottom-rc.top) ) {
//		::SetWindowPos( m_hWnd, NULL, rc.left, rc.top, RCWIDTH(rc), RCHEIGHT(rc), SWP_NOZORDER );
//	}

	// ウインドウ位置/サイズの設定
	RECT	rc = Config.netplay.rcChatPos;
	if( (rc.right-rc.left) && (rc.bottom-rc.top) ) {
		::SetWindowPos( m_hWnd, NULL, rc.left, rc.top, RCWIDTH(rc), RCHEIGHT(rc), SWP_NOZORDER );
	}

	// 表示
	::ShowWindow( m_hWnd, SW_SHOW );

	return	TRUE;
}

DLGMSG	CChatDlg::OnDestroy( DLGMSGPARAM )
{
	return	TRUE;
}

DLGMSG	CChatDlg::OnClose( DLGMSGPARAM )
{
	::ShowWindow( m_hWnd, SW_HIDE ); // 非表示にするだけ
	return	TRUE;
}

DLGMSG	CChatDlg::OnActivate( DLGMSGPARAM )
{
	if( LOWORD(wParam) == WA_INACTIVE ) {
//		DEBUGOUT( "CChatDlg::OnActivate:Inactive\n" );
		::PostMessage( CApp::GetHWnd(), WM_VNS_SHORTCUTENABLE, (WPARAM)TRUE, 0 );

		Config.InputKeyboardDisable( FALSE );
	} else {
//		DEBUGOUT( "CChatDlg::OnActivate:Active\n" );
		::PostMessage( CApp::GetHWnd(), WM_VNS_SHORTCUTENABLE, (WPARAM)FALSE, 0 );

		Config.InputKeyboardDisable( TRUE );
	}
	return	TRUE;
}

DLGMSG	CChatDlg::OnSetCursor( DLGMSGPARAM )
{
//	DEBUGOUT( "CChatDlg::OnSetCursor\n" );
	return	FALSE;
}

DLGMSG	CChatDlg::OnSize( DLGMSGPARAM )
{
//	DEBUGOUT( "CChatDlg::OnSize\n" );
	HWND	hWndCtrl;
	RECT	rcC, rcT;
	::GetClientRect( m_hWnd, &rcC );

	// メッセージ枠
	if( (hWndCtrl = ::GetDlgItem( m_hWnd, IDC_NCT_MESSAGE )) ) {
		rcT.left   = rcC.left;
		rcT.right  = rcC.right;
		rcT.top    = rcC.top;
		rcT.bottom = rcC.bottom - (m_rcClient.bottom-m_rcMessage.bottom);
		::MoveWindow( hWndCtrl, rcT.left, rcT.top, RCWIDTH(rcT), RCHEIGHT(rcT), TRUE );
	}
	if( (hWndCtrl = ::GetDlgItem( m_hWnd, IDC_NCT_EDIT )) ) {
		rcT.left   = rcC.left;
		rcT.right  = rcC.right - (m_rcClient.right-m_rcEdit.right);
		rcT.top    = rcC.bottom - (m_rcClient.bottom-m_rcEdit.top);
		rcT.bottom = rcC.bottom - (m_rcClient.bottom-m_rcEdit.bottom);
		::MoveWindow( hWndCtrl, rcT.left, rcT.top, RCWIDTH(rcT), RCHEIGHT(rcT), TRUE );
	}
	if( (hWndCtrl = ::GetDlgItem( m_hWnd, IDC_NCT_SEND )) ) {
		rcT.left   = rcC.right - (m_rcClient.right-m_rcButton.left);
		rcT.right  = rcT.left + RCWIDTH(m_rcButton);
		rcT.top    = rcC.bottom - (m_rcClient.bottom-m_rcButton.top);
		rcT.bottom = rcT.top + RCHEIGHT(m_rcButton);
		::MoveWindow( hWndCtrl, rcT.left, rcT.top, RCWIDTH(rcT), RCHEIGHT(rcT), TRUE );
	}
	return	FALSE;
}

DLGMSG	CChatDlg::OnControlColorStatic( DLGMSGPARAM )
{
	if( (HWND)lParam == ::GetDlgItem( m_hWnd, IDC_NCT_MESSAGE ) ) {
		SetBkColor( (HDC)wParam, (COLORREF)0x00FFFFFF );
		bResult = (LRESULT)GetStockObject( WHITE_BRUSH );
		return	TRUE;
	}
	return	FALSE;
}

DLGCMD	CChatDlg::OnMessageFocus( DLGCMDPARAM )
{
	DEBUGOUT( "CChatDlg::OnMessageFocus\n" );

//	::SetFocus( ::GetDlgItem( m_hWnd, IDC_NCT_EDIT ) );
	::SetFocus( m_hWnd );
}

DLGCMD	CChatDlg::OnOK( DLGCMDPARAM )
{
//	DEBUGOUT( "CChatDlg::OnOK\n" );
	SetEditText();
}

DLGCMD	CChatDlg::OnCancel( DLGCMDPARAM )
{
//	DEBUGOUT( "CChatDlg::OnCancel\n" );
	::ShowWindow( m_hWnd, SW_HIDE ); // 非表示にするだけ
}

DLGCMD	CChatDlg::OnSend( DLGCMDPARAM )
{
//	DEBUGOUT( "CChatDlg::OnSend\n" );
	SetEditText();

	::SetFocus( ::GetDlgItem( m_hWnd, IDC_NCT_EDIT ) );
}

