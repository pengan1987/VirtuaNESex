//
// ショートカットダイアログクラス
//
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

#include <string>
using namespace std;

#include "VirtuaNESres.h"
#include "DebugOut.h"
#include "App.h"
#include "Pathlib.h"
#include "Config.h"

#include "Wnd.h"
#include "ShortcutDlg.h"

#include "DirectInput.h"

DLG_MESSAGE_BEGIN(CShortcutDlg)
// メッセージ
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )
DLG_ON_MESSAGE( WM_TIMER,	OnTimer )
// NOTIFYメッセージ
DLG_NOTIFY_BEGIN()
DLG_ON_NOTIFY( IDC_CUT_LIST, NM_DBLCLK, OnListDblClick )
DLG_ON_NOTIFY( IDC_CUT_LIST, LVN_ITEMCHANGED, OnListItemChanged )
DLG_NOTIFY_END()
// コマンド
DLG_COMMAND_BEGIN()
DLG_ON_COMMAND( IDOK, OnOK )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_ON_COMMAND( IDDEFAULT, OnDefault )
DLG_COMMAND_END()
DLG_MESSAGE_END()

	DLGMSG		OnTimer( DLGMSGPARAM );
	DLGNOTIFY	OnListDblClick( DLGNOTIFYPARAM );
	DLGNOTIFY	OnListItemChanged( DLGNOTIFYPARAM );

INT	CShortcutDlg::DoModal( HWND hWndParent )
{
	m_bCancelMode = FALSE;
	return	::DialogBoxParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_CFG_SHORTCUT),
				hWndParent, g_DlgProc, (LPARAM)this );
}

BOOL	CShortcutDlg::PreTranslateMessage( MSG* pMsg )
{
	if( pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST ) {
//		DEBUGOUT( "CShortcutDlg::PreTranslateMessage WM_KEY????\n" );
		return	m_bCancelMode;
	}
	return	FALSE;
}

void	CShortcutDlg::OnInitialMember()
{
	// リストビューのアイテム等の削除
	CHAR	szStr[256];
	HWND	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_CUT_LIST );
	ListView_DeleteColumn( hWndCtrl, 0 );
	ListView_DeleteColumn( hWndCtrl, 0 );
	ListView_DeleteColumn( hWndCtrl, 0 );
	ListView_DeleteAllItems( hWndCtrl );
	ListView_SetItemCount( hWndCtrl, 100 );

	// ヘッダコントロールの設定
	LVCOLUMN	lvColumn;
	lvColumn.mask = LVCF_FMT|LVCF_TEXT|LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;

	CApp::LoadString( IDS_CUT_TYPE, szStr, sizeof(szStr) );
	lvColumn.iSubItem = 0;
	lvColumn.pszText = szStr;
	ListView_InsertColumn( hWndCtrl, 0, &lvColumn );

	CApp::LoadString( IDS_CUT_KEY, szStr, sizeof(szStr) );
	lvColumn.iSubItem = 1;
	lvColumn.pszText = szStr;
	ListView_InsertColumn( hWndCtrl, 1, &lvColumn );

	CApp::LoadString( IDS_CUT_KEY2, szStr, sizeof(szStr) );
	lvColumn.iSubItem = 2;
	lvColumn.pszText = szStr;
	ListView_InsertColumn( hWndCtrl, 2, &lvColumn );


	// リストビュー内容設定
	LVITEM	lvItem;
	ZeroMemory( &lvItem, sizeof(lvItem) );
	lvItem.mask = LVIF_TEXT;

	for( INT i = 0; CConfig::ShortcutKeyID[i*3+0]; i++ ) {
		lvItem.iItem = i;
		// 種類
		CApp::LoadString( CConfig::ShortcutKeyID[i*3+1], szStr, sizeof(szStr) );
		lvItem.pszText = szStr;
		ListView_InsertItem( hWndCtrl, &lvItem );

		// 設定ｷｰ
		if( m_ControlBuf[i] == 0 ) {
			ListView_SetItemText( hWndCtrl, i, 1, "----" );
		} else {
			string	str = Config.ShortcutToKeyName( m_ControlBuf[i] );
			ListView_SetItemText( hWndCtrl, i, 1, (LPSTR)str.c_str() );
		}
		// 設定ｷｰ2
		if( m_ControlBuf2[i] == 0 ) {
			ListView_SetItemText( hWndCtrl, i, 2, "----" );
		} else {
			string	str = Config.ShortcutToKeyName( m_ControlBuf2[i] );
			ListView_SetItemText( hWndCtrl, i, 2, (LPSTR)str.c_str() );
		}
	}

	// カラムを設定してから横幅変更しないと横スクロールバーが出て邪魔なので...
	RECT	rc;
	::GetClientRect( hWndCtrl, &rc );

	ListView_SetColumnWidth( hWndCtrl, 0, (rc.right-rc.left)*10/20 );
	ListView_SetColumnWidth( hWndCtrl, 1, (rc.right-rc.left)*5/20 );
	ListView_SetColumnWidth( hWndCtrl, 2, (rc.right-rc.left)*5/20 );
	ListView_SetExtendedListViewStyle( hWndCtrl, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );
}

DLGMSG	CShortcutDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CShortcutDlg::OnInitDialog\n" );

	// 設定の保存
	m_ConfigSave = Config.shortcut;

	// コピー
	for( INT i = 0; CConfig::ShortcutKeyID[i*3+0]; i++ ) {
		m_ControlBuf [i] = Config.shortcut.nShortCut[CConfig::ShortcutKeyID[i*3+2]];
		m_ControlBuf2[i] = Config.shortcut.nShortCut[CConfig::ShortcutKeyID[i*3+2]+128];
	}
	// ウインドウタイトルの保存
	::GetWindowText( m_hWnd, m_szWindowTitle, sizeof(m_szWindowTitle) );
	// 設定中メッセージの保存
	CApp::LoadString( IDS_CUT_TITLE, m_szMessage, sizeof(m_szMessage) );

	// 初期化
	OnInitialMember();

	m_bCancelMode = FALSE;
	m_SelectPos = -1;
	m_TimerID = 0;
	m_SelectID = 0;

	return	TRUE;
}

DLGMSG	CShortcutDlg::OnTimer( DLGMSGPARAM )
{
	if( wParam == (WPARAM)m_TimerID ) {
		m_TimerCount++;

		DirectInput.Poll();

		if( !(m_TimerCount&0x04) ) {
			string	str = m_szWindowTitle;
			str = str + "    ";
			str = str + m_szMessage;
			::SetWindowText( m_hWnd, str.c_str() );
		} else {
			::SetWindowText( m_hWnd, m_szWindowTitle );
		}

		if( m_TimerCount > 60 ) {
		// タイムアウト
			if( m_TimerID ) {
				::KillTimer( m_hWnd, m_TimerID );
				m_TimerID  = 0;
				m_SelectID = 0;
			}
			m_bCancelMode = FALSE;
			::SetWindowText( m_hWnd, m_szWindowTitle );
		} else {
		// Button push?
			HWND	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_CUT_LIST );
			INT	i;
			// Set cancel
			if( DirectInput.m_Sw[DIK_ESCAPE] && !(DirectInput.m_Sw[DIK_LSHIFT] || DirectInput.m_Sw[DIK_RSHIFT]) ) {
				if( m_SelectSubID == 2 ) {
					ListView_SetItemText( hWndCtrl, m_SelectID, 2, "----" );
					m_ControlBuf2[m_SelectID] = 0;
				} else {
					ListView_SetItemText( hWndCtrl, m_SelectID, 1, "----" );
					m_ControlBuf[m_SelectID] = 0;
				}
//				::SetWindowText( hWndCtrl, "----" );
				if( m_TimerID ) {
					::KillTimer( m_hWnd, m_TimerID );
					m_TimerID  = 0;
					m_SelectID = 0;
				}
				m_bCancelMode = FALSE;
				::SetWindowText( m_hWnd, m_szWindowTitle );
			} else {
				string	str;
				WORD	keyex = 0;
				if( !DirectInput.m_Sw[DIK_ESCAPE] ) {
					if( DirectInput.m_Sw[DIK_LMENU] || DirectInput.m_Sw[DIK_RMENU] ) {
						keyex |= CCfgShortCut::K_ALT;
						str = str + "Alt+";
					}
					if( DirectInput.m_Sw[DIK_LCONTROL] || DirectInput.m_Sw[DIK_RCONTROL] ) {
						keyex |= CCfgShortCut::K_CTRL;
						str = str + "Ctrl+";
					}
					if( DirectInput.m_Sw[DIK_LSHIFT] || DirectInput.m_Sw[DIK_RSHIFT] ) {
						keyex |= CCfgShortCut::K_SHIFT;
						str = str + "Shift+";
					}
				}

				for( i = 0; i < 256+64*8; i++ ) {
					if( i == DIK_LMENU || i == DIK_RMENU
					 || i == DIK_LCONTROL || i == DIK_RCONTROL
					 || i == DIK_LSHIFT || i == DIK_RSHIFT )
						continue;

					if( DirectInput.m_Sw[i]&0x80 ) {
						if( DirectInput.SearchKeyName(i) ) {
							str = str + DirectInput.SearchKeyName(i);
							if( m_SelectSubID == 2 ) {
								m_ControlBuf2[m_SelectID] = keyex+i;
								ListView_SetItemText( hWndCtrl, m_SelectID, 2, (LPSTR)str.c_str() );
							} else {
								m_ControlBuf [m_SelectID] = keyex+i;
								ListView_SetItemText( hWndCtrl, m_SelectID, 1, (LPSTR)str.c_str() );
							}

							if( m_TimerID ) {
								::KillTimer( m_hWnd, m_TimerID );
								m_TimerID  = 0;
								m_SelectID = 0;
							}
							m_bCancelMode = FALSE;
							::SetWindowText( m_hWnd, m_szWindowTitle );
							break;
						}
					}
				}
			}
		}
	}
	return	FALSE;
}

DLGNOTIFY CShortcutDlg::OnListItemChanged( DLGNOTIFYPARAM )
{
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;

	if( !(pNMListView->uOldState & LVIS_SELECTED) && (pNMListView->uNewState & LVIS_SELECTED) ) {
		m_SelectPos = pNMListView->iItem;
	}
}

DLGNOTIFY CShortcutDlg::OnListDblClick( DLGNOTIFYPARAM )
{
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;

//	DEBUGOUT( "CShortcutDlg::OnListDblClick\n" );

	// まだ選択してないよ
	if( m_SelectPos == -1 )
		return;

	LVHITTESTINFO	htInfo;
	ZeroMemory( &htInfo, sizeof(htInfo) );
//	DWORD	pos = ::GetMessagePos();
//	POINT	pt = *((POINT*)&pos);
//	::ScreenToClient( ::GetDlgItem( m_hWnd, IDC_CUT_LIST ), &pt );
//	htInfo.pt.x = pt.x;
//	htInfo.pt.y = pt.y;
	htInfo.pt.x = pNMListView->ptAction.x;
	htInfo.pt.y = pNMListView->ptAction.y;
	ListView_HitTest( ::GetDlgItem( m_hWnd, IDC_CUT_LIST ), &htInfo );

	// セレクトされた場所にヒットしてない？
	if( htInfo.flags & LVHT_NOWHERE ) {
		return;
	}

	if( !m_TimerID && pNMListView->iSubItem ) {
		m_bCancelMode = TRUE;
		m_SelectID    = m_SelectPos;
		m_SelectSubID = pNMListView->iSubItem;
		m_TimerCount  = 0;
		m_TimerID     = ::SetTimer( m_hWnd, 1, 50, NULL );
DEBUGOUT( "ID=%d/SUBID=%d\n", pNMListView->iItem, pNMListView->iSubItem );
	}
}

DLGCMD	CShortcutDlg::OnOK( DLGCMDPARAM )
{
//	DEBUGOUT( "CShortcutDlg::OnOK\n" );

	if( m_TimerID ) {
		::KillTimer( m_hWnd, m_TimerID );
		m_TimerID  = 0;
		m_SelectID = 0;
		m_bCancelMode = FALSE;
	}

	// 設定をコピー
	for( INT i = 0; CConfig::ShortcutKeyID[i*3+0]; i++ ) {
		Config.shortcut.nShortCut[CConfig::ShortcutKeyID[i*3+2]    ] = m_ControlBuf [i];
		Config.shortcut.nShortCut[CConfig::ShortcutKeyID[i*3+2]+128] = m_ControlBuf2[i];
	}

	::EndDialog( m_hWnd, IDOK );
}

DLGCMD	CShortcutDlg::OnCancel( DLGCMDPARAM )
{
//	DEBUGOUT( "CShortcutDlg::OnCancel\n" );

	if( m_TimerID ) {
		::KillTimer( m_hWnd, m_TimerID );
		m_TimerID  = 0;
		m_SelectID = 0;
		m_bCancelMode = FALSE;
	}

	// 設定を戻す
	Config.shortcut = m_ConfigSave;

	::EndDialog( m_hWnd, IDCANCEL );
}

DLGCMD	CShortcutDlg::OnDefault( DLGCMDPARAM )
{
//	DEBUGOUT( "CShortcutDlg::OnDefault\n" );

	if( m_TimerID ) {
		::KillTimer( m_hWnd, m_TimerID );
		m_TimerID  = 0;
		m_SelectID = 0;
		m_bCancelMode = FALSE;
	}

	Config.shortcut.Default();
	// コピー
	for( INT i = 0; CConfig::ShortcutKeyID[i*3+0]; i++ ) {
		m_ControlBuf [i] = Config.shortcut.nShortCut[CConfig::ShortcutKeyID[i*3+2]    ];
		m_ControlBuf2[i] = Config.shortcut.nShortCut[CConfig::ShortcutKeyID[i*3+2]+128];
	}
	OnInitialMember();
}

