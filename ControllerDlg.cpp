//
// コントローラダイアログクラス
//
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

#include <string>
using namespace std;

#include "VirtuaNESres.h"
#include "DebugOut.h"
#include "App.h"
#include "Pathlib.h"

#include "Wnd.h"
#include "ControllerDlg.h"

#include "DirectInput.h"

///////////////////////////////////////////////////////////////////////////////

DLG_MESSAGE_BEGIN(CControllerPageDlg)
// メッセージ
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )
// NOTIFYメッセージ
DLG_NOTIFY_BEGIN()
DLG_NOTIFY_END()
// コマンド
DLG_COMMAND_BEGIN()
DLG_ON_COMMAND_NOTIFY_RANGE( IDC_CTR_UP, IDC_CTR_END, STN_CLICKED, OnClicked )
DLG_COMMAND_END()
DLG_MESSAGE_END()

INT	CControllerPageDlg::Create( UINT nID, HWND hWndParent )
{
	m_nPageID = nID;

	m_hWnd = ::CreateDialogParam( CApp::GetPlugin(), MAKEINTRESOURCE(nID),
				hWndParent,  g_DlgProc, (LPARAM)this );
	return	(m_hWnd)?TRUE:FALSE;
}

void	CControllerPageDlg::Destroy()
{
	::DestroyWindow( m_hWnd );
}

DLGCMD	CControllerPageDlg::OnClicked( DLGCMDPARAM )
{
//	DEBUGOUT( "CControllerPageDlg::OnClicked uID=%d\n", uID );

	// 親のタブコントロールをパスしてメッセージを通知:p
	HWND	hWndParent, hWndOwner;
	hWndParent = ::GetParent( m_hWnd );
	hWndOwner = ::GetParent( hWndParent );
	::SendMessage( hWndOwner, WM_COMMAND, MAKELPARAM(uID,STN_CLICKED), (LPARAM)::GetDlgItem(m_hWnd,uID) );
}

DLGMSG	CControllerPageDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CControllerPageDlg::OnInitDialog\n" );

	return	TRUE;
}

///////////////////////////////////////////////////////////////////////////////

UINT	CControllerDlg::ControllerPageID[] = {
	IDD_CTR_PLAYER1,
	IDD_CTR_PLAYER2,
	IDD_CTR_PLAYER3,
	IDD_CTR_PLAYER4,
	IDD_CTR_CRAZYCLIMBER,
	IDD_CTR_FAMLYTRAINER,
	IDD_CTR_EXCITINGBOXING,
	IDD_CTR_MAHJANG,
	IDD_CTR_NSFPLAYER,
	IDD_CTR_VSUNISYSTEM,
	0
};

DLG_MESSAGE_BEGIN(CControllerDlg)
// メッセージ
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )
DLG_ON_MESSAGE( WM_DESTROY,	OnDestroy )
DLG_ON_MESSAGE( WM_TIMER,	OnTimer )
// NOTIFYメッセージ
DLG_NOTIFY_BEGIN()
DLG_ON_NOTIFY( IDC_CTR_TAB, TCN_SELCHANGE, OnSelectChange )
DLG_NOTIFY_END()
// コマンド
DLG_COMMAND_BEGIN()
DLG_ON_COMMAND( IDOK, OnOK )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_ON_COMMAND( IDDEFAULT, OnDefault )
DLG_ON_COMMAND_NOTIFY_RANGE( IDC_CTR_UP, IDC_CTR_END, STN_CLICKED, OnClicked )
DLG_ON_COMMAND_NOTIFY( IDC_CTR_SELECT_COMBO, CBN_SELCHANGE, OnSettingSelectChange )
DLG_COMMAND_END()
DLG_MESSAGE_END()

INT	CControllerDlg::DoModal( HWND hWndParent )
{
	m_bCancelMode = FALSE;
	m_TimerID = 0;

	m_SettingSel = 0;

	return	::DialogBoxParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_CFG_CONTROLLER),
				hWndParent, g_DlgProc, (LPARAM)this );
}

BOOL	CControllerDlg::PreTranslateMessage( MSG* pMsg )
{
	if( pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST ) {
//		DEBUGOUT( "CControllerDlg::PreTranslateMessage WM_KEY????\n" );
		return	m_bCancelMode;
	}
	return	FALSE;
}

void	CControllerDlg::OnInitialMember()
{
	HWND	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_CTR_TAB );
	RECT	rcT, rcP, rcW;
	INT	page;

//	for( INT page = 0; ControllerPageID[page]; page++ ) {
	for( page = 0; ControllerPageID[page]; page++ ) {
		m_Page[page].Create( ControllerPageID[page], hWndCtrl );

		// 名前の取得
		CHAR	szTitle[256];
		::GetWindowText( m_Page[page].m_hWnd, szTitle, sizeof(szTitle) );

		// 名前を取得したらキャプションを消す
		LONG	style;
		style = ::GetWindowLong( m_Page[page].m_hWnd, GWL_STYLE );
		style &= ~(WS_CAPTION|WS_BORDER|WS_OVERLAPPED);
		::SetWindowLong( m_Page[page].m_hWnd, GWL_STYLE, style );

		// サイズ変更
		::GetWindowRect( m_Page[page].m_hWnd, &rcW );
		rcW.right  -= ::GetSystemMetrics( SM_CXSIZEFRAME );
		rcW.bottom -= ::GetSystemMetrics( SM_CYCAPTION )+::GetSystemMetrics( SM_CYSIZEFRAME )*2;
		::SetWindowPos( m_Page[page].m_hWnd, NULL, 0, 0, RCWIDTH(rcW), RCHEIGHT(rcW), SWP_NOMOVE|SWP_NOZORDER );

		// タブに名前を設定する
		TCITEM	tcitem;
		tcitem.mask = TCIF_TEXT;
		tcitem.pszText = szTitle;
		TabCtrl_InsertItem( hWndCtrl, page, &tcitem );

//		// ページ位置の補正
//		::GetClientRect( hWndCtrl, &rcT );
//		TabCtrl_AdjustRect( hWndCtrl, FALSE, &rcT );
//		::GetWindowRect( m_Page[page].m_hWnd, &rcP );

		// 計算
//		rcW.left   = rcT.left + (RCWIDTH(rcT)-RCWIDTH(rcP))/2;
//		rcW.top    = rcT.top  + (RCHEIGHT(rcT)-RCHEIGHT(rcP))/2;
//		rcW.right  = rcW.left + RCWIDTH(rcP);
//		rcW.bottom = rcW.top  + RCHEIGHT(rcP);
//		::SetWindowPos( m_Page[page].m_hWnd, NULL, rcW.left, rcW.top, RCWIDTH(rcW), RCHEIGHT(rcW), SWP_NOZORDER|SWP_NOACTIVATE );

		// 各ページのリストボックスの設定
		if( page < 4 ) {
			HWND	hWndList;
			hWndList = ::GetDlgItem( m_Page[page].m_hWnd, IDC_CTR_A_RAPID_LIST );
			::SendMessage( hWndList, LB_INSERTSTRING, 0, (LPARAM)" 10FPS" );
			::SendMessage( hWndList, LB_INSERTSTRING, 1, (LPARAM)" 15FPS" );
			::SendMessage( hWndList, LB_INSERTSTRING, 2, (LPARAM)" 20FPS" );
			::SendMessage( hWndList, LB_INSERTSTRING, 3, (LPARAM)" 30FPS" );
			hWndList = ::GetDlgItem( m_Page[page].m_hWnd, IDC_CTR_B_RAPID_LIST );
			::SendMessage( hWndList, LB_INSERTSTRING, 0, (LPARAM)" 10FPS" );
			::SendMessage( hWndList, LB_INSERTSTRING, 1, (LPARAM)" 15FPS" );
			::SendMessage( hWndList, LB_INSERTSTRING, 2, (LPARAM)" 20FPS" );
			::SendMessage( hWndList, LB_INSERTSTRING, 3, (LPARAM)" 30FPS" );
		}

		// ボタン名等の設定
		OnPageSetup( page );

		::ShowWindow( m_Page[page].m_hWnd, (page==0)?SW_SHOW:SW_HIDE );
	}

	// ページ位置再調整
	for( page = 0; ControllerPageID[page]; page++ ) {
		// ページ位置の補正
		::GetClientRect( hWndCtrl, &rcT );
		TabCtrl_AdjustRect( hWndCtrl, FALSE, &rcT );
		::GetWindowRect( m_Page[page].m_hWnd, &rcP );

		// 計算
		rcW.left   = rcT.left + (RCWIDTH(rcT)-RCWIDTH(rcP))/2;
		rcW.top    = rcT.top  + (RCHEIGHT(rcT)-RCHEIGHT(rcP))/2;
		rcW.right  = rcW.left + RCWIDTH(rcP);
		rcW.bottom = rcW.top  + RCHEIGHT(rcP);
		::SetWindowPos( m_Page[page].m_hWnd, NULL, rcW.left, rcW.top, RCWIDTH(rcW), RCHEIGHT(rcW), SWP_NOZORDER|SWP_NOACTIVATE );
	}

	TabCtrl_SetCurSel( hWndCtrl, 0 );
	m_PageSel = 0;

	m_PageNum = page;

	// 設定コンボボックス
	::SendDlgItemMessage( m_hWnd, IDC_CTR_SELECT_COMBO, CB_RESETCONTENT, 0, 0 );
	CHAR	szStr[64];
	for( INT i = IDS_CTR_SELECT1; i <= IDS_CTR_SELECT2; i++ ) {
		CApp::LoadString( i, szStr, sizeof(szStr) );
		::SendDlgItemMessage( m_hWnd, IDC_CTR_SELECT_COMBO, CB_INSERTSTRING, (WPARAM)i-IDS_CTR_SELECT1, (LPARAM)szStr );
	}
	::SendDlgItemMessage( m_hWnd, IDC_CTR_SELECT_COMBO, CB_SETCURSEL, 0, 0 );
}

void	CControllerDlg::OnPageSetup( UINT nPage )
{
	HWND	hWnd = m_Page[nPage].m_hWnd;
	HWND	hWndCtrl;
	INT	nKey;
	LPCSTR	lpStr;
	INT	nOffset = m_SettingSel?32:0;

	for( INT nID = IDC_CTR_UP; nID <= IDC_CTR_END; nID++ ) {
		if( (hWndCtrl = ::GetDlgItem( hWnd, nID )) ) {
			if( nPage < 4 ) {
				nKey = Config.controller.nButton[nPage][nID-IDC_CTR_UP+nOffset];
			} else if( ControllerPageID[nPage] == IDD_CTR_CRAZYCLIMBER ) {
				nKey = Config.controller.nExButton[0][nID-IDC_CTR_UP+nOffset];
			} else if( ControllerPageID[nPage] == IDD_CTR_FAMLYTRAINER ) {
				nKey = Config.controller.nExButton[1][nID-IDC_CTR_UP+nOffset];
			} else if( ControllerPageID[nPage] == IDD_CTR_EXCITINGBOXING ) {
				nKey = Config.controller.nExButton[2][nID-IDC_CTR_UP+nOffset];
			} else if( ControllerPageID[nPage] == IDD_CTR_MAHJANG ) {
				nKey = Config.controller.nExButton[3][nID-IDC_CTR_UP+nOffset];
			} else if( ControllerPageID[nPage] == IDD_CTR_NSFPLAYER ) {
				nKey = Config.controller.nNsfButton[nID-IDC_CTR_UP+nOffset];
			} else if( ControllerPageID[nPage] == IDD_CTR_VSUNISYSTEM ) {
				nKey = Config.controller.nVSUnisystem[nID-IDC_CTR_UP+nOffset];
			}
			if( (lpStr = DirectInput.SearchKeyName(nKey)) ) {
				::SetWindowText( hWndCtrl, lpStr );
			} else {
				::SetWindowText( hWndCtrl, "----" );
			}
			::InvalidateRect( hWndCtrl, NULL, TRUE );
		}
	}

	if( nPage < 4 ) {
		::SendDlgItemMessage( hWnd, IDC_CTR_A_RAPID_LIST, LB_SETCURSEL,
					(WPARAM)Config.controller.nRapid[nPage][0], 0 );
		::SendDlgItemMessage( hWnd, IDC_CTR_B_RAPID_LIST, LB_SETCURSEL,
					(WPARAM)Config.controller.nRapid[nPage][1], 0 );
	}
}

DLGMSG	CControllerDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CControllerDlg::OnInitDialog\n" );
	m_ConfigSave = Config.controller;

	OnInitialMember();

	return	TRUE;
}

DLGMSG	CControllerDlg::OnDestroy( DLGMSGPARAM )
{
//	DEBUGOUT( "CControllerDlg::OnDestroy\n" );

	// チャイルドウインドウを破棄
	for( INT i = 0; i < m_PageNum; i++ ) {
		::DestroyWindow( m_Page[i].m_hWnd );
	}
	return	FALSE;
}

DLGNOTIFY CControllerDlg::OnSelectChange( DLGNOTIFYPARAM )
{
//	DEBUGOUT( "CControllerDlg::OnSelectChange\n" );

	HWND	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_CTR_TAB );
	m_PageSel = TabCtrl_GetCurSel( hWndCtrl );

	OnPageSetup( m_PageSel );

	for( INT i = 0; i < m_PageNum; i++ ) {
		if( i == m_PageSel ) {
			::ShowWindow( m_Page[i].m_hWnd, SW_SHOW );
		} else {
			::ShowWindow( m_Page[i].m_hWnd, SW_HIDE );
		}
	}

	if( m_TimerID ) {
		::KillTimer( m_hWnd, m_TimerID );
		m_TimerID   = 0;
		m_ControlID = 0;
//		::InvalidateRect( hWndCtrl, NULL, TRUE );
	}
	m_bCancelMode = FALSE;
}

DLGNOTIFY CControllerDlg::OnSettingSelectChange( DLGCMDPARAM )
{
//	DEBUGOUT( "CControllerDlg::OnSettingSelectChange\n" );

	if( m_TimerID ) {
		::KillTimer( m_hWnd, m_TimerID );
		m_TimerID   = 0;
		m_ControlID = 0;
//		::InvalidateRect( ::GetDlgItem( m_hWnd, IDC_CTR_TAB ), NULL, TRUE );
	}
	m_bCancelMode = FALSE;

	m_SettingSel = ::SendDlgItemMessage( m_hWnd, IDC_CTR_SELECT_COMBO, CB_GETCURSEL, 0, 0 );

	OnPageSetup( m_PageSel );
}

DLGCMD	CControllerDlg::OnClicked( DLGCMDPARAM )
{
//	DEBUGOUT( "CControllerDlg::OnClicked uID=%d\n", uID );
	if( !m_TimerID ) {
		m_ControlID = uID;
		m_TimerCount = 0;
		m_TimerID = ::SetTimer( m_hWnd, 1, 50, NULL );
		m_bCancelMode = TRUE;
	}
}

DLGMSG	CControllerDlg::OnTimer( DLGMSGPARAM )
{
	HWND	hWndCtrl = ::GetDlgItem( m_Page[m_PageSel].m_hWnd, m_ControlID );
	INT	nID = m_ControlID - IDC_CTR_UP;
	INT	nOffset = m_SettingSel?32:0;

	if( wParam == (WPARAM)m_TimerID ) {
		if( m_TimerCount == 0 )
			m_TimerStart = ::timeGetTime();
		m_TimerCount++;

		DirectInput.Poll();

		if( m_TimerCount > 60 ) {
		// TimeOut
			if( m_TimerID ) {
				::KillTimer( m_hWnd, m_TimerID );
				m_TimerID   = 0;
				m_ControlID = 0;
				::InvalidateRect( hWndCtrl, NULL, TRUE );
			}
			m_bCancelMode = FALSE;
		} else {
		// Button push?
			BOOL	bSet = FALSE;
			INT	i;
			LPCSTR	str;
			// Set cancel
			if( DirectInput.m_Sw[DIK_ESCAPE] ) {
				if( m_PageSel < 4 ) {
					Config.controller.nButton[m_PageSel][nID+nOffset] = 0;
				} else if( ControllerPageID[m_PageSel] == IDD_CTR_CRAZYCLIMBER ) {
					Config.controller.nExButton[0][nID+nOffset] = 0;
				} else if( ControllerPageID[m_PageSel] == IDD_CTR_FAMLYTRAINER ) {
						Config.controller.nExButton[1][nID+nOffset] = 0;
				} else if( ControllerPageID[m_PageSel] == IDD_CTR_EXCITINGBOXING ) {
						Config.controller.nExButton[2][nID+nOffset] = 0;
				} else if( ControllerPageID[m_PageSel] == IDD_CTR_MAHJANG ) {
						Config.controller.nExButton[3][nID+nOffset] = 0;
				} else if( ControllerPageID[m_PageSel] == IDD_CTR_NSFPLAYER ) {
					Config.controller.nNsfButton[nID+nOffset] = 0;
				} else if( ControllerPageID[m_PageSel] == IDD_CTR_VSUNISYSTEM ) {
					Config.controller.nVSUnisystem[nID+nOffset] = 0;
				}
				::SetWindowText( hWndCtrl, "----" );
				bSet = TRUE;
			} else {
				for( i = 0; i < 512; i++ ) {
					if( DirectInput.m_Sw[i] ) {
						if( (str=DirectInput.SearchKeyName( i )) ) {
							if( m_PageSel < 4 ) {
								Config.controller.nButton[m_PageSel][nID+nOffset] = i;
							} else if( ControllerPageID[m_PageSel] == IDD_CTR_CRAZYCLIMBER ) {
								Config.controller.nExButton[0][nID+nOffset] = i;
							} else if( ControllerPageID[m_PageSel] == IDD_CTR_FAMLYTRAINER ) {
									Config.controller.nExButton[1][nID+nOffset] = i;
							} else if( ControllerPageID[m_PageSel] == IDD_CTR_EXCITINGBOXING ) {
									Config.controller.nExButton[2][nID+nOffset] = i;
							} else if( ControllerPageID[m_PageSel] == IDD_CTR_MAHJANG ) {
									Config.controller.nExButton[3][nID+nOffset] = i;
							} else if( ControllerPageID[m_PageSel] == IDD_CTR_NSFPLAYER ) {
								Config.controller.nNsfButton[nID+nOffset] = i;
							} else if( ControllerPageID[m_PageSel] == IDD_CTR_VSUNISYSTEM ) {
								Config.controller.nVSUnisystem[nID+nOffset] = i;
							}
							::SetWindowText( hWndCtrl, str );
							bSet = TRUE;
						}
					}
				}
			}

			if( bSet ) {
			// Button Pressed
				if( m_TimerID ) {
					::KillTimer( m_hWnd, m_TimerID );
					m_TimerID   = 0;
					m_ControlID = 0;
					::InvalidateRect( hWndCtrl, NULL, TRUE );
				}
				m_bCancelMode = FALSE;
			} else {
			// Flashing
				HDC	hDC = ::GetDC( hWndCtrl );
				RECT	rc;
				::GetClientRect( hWndCtrl, &rc );
				DWORD	c = ((::timeGetTime()-m_TimerStart)/3)&0xFF;
				if( c > 0x7F )
					c = 0x80 - (c - 0x7F);
				c *= 2;
				::SetBkColor( hDC, RGB(c,c,c) );
				::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL );
				::ReleaseDC( hWndCtrl, hDC );
			}
		}
	}
	return	FALSE;
}

DLGCMD	CControllerDlg::OnOK( DLGCMDPARAM )
{
//	DEBUGOUT( "CControllerDlg::OnOK\n" );

	if( m_TimerID ) {
		::KillTimer( m_hWnd, m_TimerID );
		m_TimerID   = 0;
		m_ControlID = 0;
	}
	m_bCancelMode = FALSE;

	for( INT page = 0; page < 4; page++ ) {
		Config.controller.nRapid[page][0] = ::SendDlgItemMessage( m_Page[page].m_hWnd, IDC_CTR_A_RAPID_LIST, LB_GETCURSEL, 0, 0 );
		Config.controller.nRapid[page][1] = ::SendDlgItemMessage( m_Page[page].m_hWnd, IDC_CTR_B_RAPID_LIST, LB_GETCURSEL, 0, 0 );
	}

	::EndDialog( m_hWnd, IDOK );
}

DLGCMD	CControllerDlg::OnCancel( DLGCMDPARAM )
{
//	DEBUGOUT( "CControllerDlg::OnCancel\n" );
//	if( m_bCancelMode )
//		return;
	if( m_TimerID ) {
		::KillTimer( m_hWnd, m_TimerID );
		m_TimerID   = 0;
		m_ControlID = 0;
	}
	m_bCancelMode = FALSE;

	Config.controller = m_ConfigSave;

	::EndDialog( m_hWnd, IDCANCEL );
}

DLGCMD	CControllerDlg::OnDefault( DLGCMDPARAM )
{
//	DEBUGOUT( "CControllerDlg::OnDefault\n" );
	if( m_TimerID ) {
		::KillTimer( m_hWnd, m_TimerID );
		m_TimerID   = 0;
		m_ControlID = 0;
	}
	m_bCancelMode = FALSE;

	switch( ControllerPageID[m_PageSel] ) {
		case	IDD_CTR_PLAYER1:
			Config.controller.DefaultController1();
			break;
		case	IDD_CTR_PLAYER2:
			Config.controller.DefaultController2();
			break;
		case	IDD_CTR_PLAYER3:
			Config.controller.DefaultController3();
			break;
		case	IDD_CTR_PLAYER4:
			Config.controller.DefaultController4();
			break;
		case	IDD_CTR_CRAZYCLIMBER:
			Config.controller.DefaultExController0();
			break;
		case	IDD_CTR_FAMLYTRAINER:
			Config.controller.DefaultExController1();
			break;
		case	IDD_CTR_EXCITINGBOXING:
			Config.controller.DefaultExController2();
			break;
		case	IDD_CTR_MAHJANG:
			Config.controller.DefaultExController3();
			break;
		case	IDD_CTR_NSFPLAYER:
			Config.controller.DefaultNsfController();
			break;
		case	IDD_CTR_VSUNISYSTEM:
			Config.controller.DefaultVSUnisystem();
			break;
		default:
			break;
	}
	OnPageSetup( m_PageSel );
}

