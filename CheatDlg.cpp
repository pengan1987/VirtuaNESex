//
// チートダイアログクラス
//
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <commdlg.h>
#include <mbstring.h>

#include <string>
using namespace std;

#include "VirtuaNESres.h"
#include "DebugOut.h"
#include "App.h"
#include "Pathlib.h"
#include "Config.h"

#include "Wnd.h"
#include "CheatDlg.h"

#include "EmuThread.h"

//
// サーチダイアログ
//
DLG_MESSAGE_BEGIN(CSearchDlg)
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )
DLG_ON_MESSAGE( WM_ACTIVATE,	OnActivate )
DLG_ON_MESSAGE( WM_CLOSE,	OnClose )
DLG_ON_MESSAGE( WM_CONTEXTMENU, OnContextMenu )

DLG_COMMAND_BEGIN()
//DLG_ON_COMMAND( IDOK, OnOK )
//DLG_ON_COMMAND( IDCANCEL, OnCancel )

DLG_ON_COMMAND( IDC_SCH_START,  OnStart )
DLG_ON_COMMAND( IDC_SCH_UPDATE, OnUpdate )
DLG_ON_COMMAND( IDC_SCH_UNDO,   OnUndo )

DLG_ON_COMMAND_RANGE( IDC_SCH_RADIX_DEC, IDC_SCH_RADIX_HEX, OnRadixCommand )
DLG_ON_COMMAND_RANGE( IDC_SCH_LENGTH_1BYTE, IDC_SCH_LENGTH_4BYTE, OnLengthCommand )
DLG_ON_COMMAND_RANGE( IDC_SCH_EQUAL, IDC_SCH_GRATEREQUAL, OnSearchCommand )

DLG_ON_COMMAND( IDC_SCH_SEARCH, OnSearchData )
DLG_ON_COMMAND( IDC_SCH_WRITE,  OnWriteData )

DLG_ON_COMMAND( ID_SCH_APPEND, OnCodeAppend )

//DLG_ON_COMMAND( IDC_VER_WEBSITE, OnWebsite )
DLG_COMMAND_END()

// Notify メッセージ
DLG_NOTIFY_BEGIN()
DLG_ON_NOTIFY( IDC_SCH_RESULT_LIST, NM_DBLCLK, OnDoubleClickListView )
DLG_NOTIFY_END()
DLG_MESSAGE_END()

BOOL	CSearchDlg::Create( HWND hWndParent )
{
	m_hMenu = NULL;
	m_bShortCutDisable = FALSE;

	m_hWnd = ::CreateDialogParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_SEARCH),
				hWndParent, g_DlgProc, (LPARAM)this );
	if( !m_hWnd )
		return	FALSE;

	// モードレスダイアログリストに加える
	CWndList::Add( this );

	return	TRUE;
}

void	CSearchDlg::Destroy()
{
	if( m_hWnd ) {
		// 位置保存
		::GetWindowRect( m_hWnd, &Config.general.rcSearchDlgPos );

		// メニュー削除
		if( m_hMenu )
			::DestroyMenu( m_hMenu );
		m_hMenu = NULL;

		// モードレスダイアログリストから削除
		CWndList::Del( this );

		::DestroyWindow( m_hWnd );
		m_hWnd = NULL;
	}
}

void	CSearchDlg::OnListUpdate()
{
	HWND	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_SCH_RESULT_LIST );
	ListView_DeleteAllItems( hWndCtrl );

	INT	i;
	DWORD	data;
	CHAR	szStr[256], szTemp[256];
	INT	index;

	LVITEM	lvitem;
	lvitem.mask = LVIF_TEXT|LVIF_PARAM;
	lvitem.iSubItem = 0;

	index = 0;

	if( m_nRadix == 10 ) {
		::wsprintf( szTemp, "%%lu" );
	} else {
		::wsprintf( szTemp, "%%0%dX", (m_nLength+1)*2 );
	}

	if( IsBTNCHECK( IDC_SCH_AREA_RAM ) ) {
		for( i = 0; i < 0x0800-m_nLength; i++ ) {
			if( m_Result.RAM_F[i] ) {
				// Address
				::wsprintf( szStr, "%04X", i );
				lvitem.pszText = szStr;
				lvitem.iItem = index;
				lvitem.lParam = (LPARAM)i;
				ListView_InsertItem( hWndCtrl, &lvitem );

				// OLD
				data = GetSearchMemoryOld( m_nLength, i );
				::wsprintf( szStr, szTemp, data );
				ListView_SetItemText( hWndCtrl, index, 1, (LPSTR)szStr );

				// NEW
				data = GetSearchMemory( m_nLength, i );
				::wsprintf( szStr, szTemp, data );
				ListView_SetItemText( hWndCtrl, index, 2, (LPSTR)szStr );

				index++;
			}
		}
	}

	if( IsBTNCHECK( IDC_SCH_AREA_SRAM ) ) {
		for( i = 0; i < 0x2000-m_nLength; i++ ) {
			if( m_Result.SRAM_F[i] ) {
				// Address
				::wsprintf( szStr, "%04X", i+0x6000 );
				lvitem.pszText = szStr;
				lvitem.iItem = index;
				lvitem.lParam = (LPARAM)(i+0x6000);
				ListView_InsertItem( hWndCtrl, &lvitem );

				// OLD
				data = GetSearchMemoryOld( m_nLength, i+0x6000 );
				::wsprintf( szStr, szTemp, data );
				ListView_SetItemText( hWndCtrl, index, 1, (LPSTR)szStr );

				// NEW
				data = GetSearchMemory( m_nLength, i+0x6000 );
				::wsprintf( szStr, szTemp, data );
				ListView_SetItemText( hWndCtrl, index, 2, (LPSTR)szStr );

				index++;
			}
		}
	}

}

DWORD	CSearchDlg::GetNesMemory( INT length, DWORD addr )
{
	DWORD	data = 0;
	for( INT i = 0; i <= length; i++ ) {
		data |= (DWORD)Emu.GetNES()->Read( (WORD)addr+i )*(1<<(i*8));
	}

	return	data;
}

DWORD	CSearchDlg::GetSearchMemory( INT length, DWORD addr )
{
	BYTE*	lpRAM;
	if( addr < 0x0800 ) {
		lpRAM = m_Result.RAM_N;
	} else if( addr < 0x8000 ) {
		lpRAM = m_Result.SRAM_N - 0x6000;
	}

	DWORD	data = 0;
	for( INT i = 0; i <= length; i++ ) {
		data |= (DWORD)lpRAM[ (WORD)addr+i ]*(1<<(i*8));
	}

	return	data;
}

DWORD	CSearchDlg::GetSearchMemoryOld( INT length, DWORD addr )
{
	BYTE*	lpRAM;
	if( addr < 0x0800 ) {
		lpRAM = m_Result.RAM_O;
	} else if( addr < 0x8000 ) {
		lpRAM = m_Result.SRAM_O - 0x6000;
	}

	DWORD	data = 0;
	for( INT i = 0; i <= length; i++ ) {
		data |= (DWORD)lpRAM[ (WORD)addr+i ]*(1<<(i*8));
	}

	return	data;
}

BOOL	CSearchDlg::CompareData( INT type, DWORD dataA, DWORD dataB )
{
	switch( type ) {
		case	0:	// EQUAL
			if( dataA == dataB )
				return	TRUE;
			break;
		case	1:	// NOTEQUAL
			if( dataA != dataB )
				return	TRUE;
			break;

		case	2:	// LESS
			if( dataA < dataB )
				return	TRUE;
			break;
		case	3:	// GRATER
			if( dataA > dataB )
				return	TRUE;
			break;

		case	4:	// LESSEQUAL
			if( dataA <= dataB )
				return	TRUE;
			break;
		case	5:	// GRATEREQUAL
			if( dataA >= dataB )
				return	TRUE;
			break;
	}

	return	FALSE;
}

BOOL	CSearchDlg::CompareRange( INT length, DWORD dataA, DWORD dataB, DWORD range )
{
	DWORD	dmin, dmax;

	switch( length ) {
		case	0:	// 1byte
			dataA &= 0x000000FF;
			dataB &= 0x000000FF;
			range &= 0x000000FF;

			dmin = dataB-range;
			dmax = dataB+range;

			if( dataB < dmin )
				dmin = 0;
			if( dataB > dmax )
				dmax = 0xFF;
			break;

		case	1:	// 2byte
			dataA &= 0x0000FFFF;
			dataB &= 0x0000FFFF;
			range &= 0x0000FFFF;

			dmin = dataB-range;
			dmax = dataB+range;

			if( dataB < dmin )
				dmin = 0;
			if( dataB > dmax )
				dmax = 0xFFFF;
			break;

		case	2:	// 3byte
			dataA &= 0x00FFFFFF;
			dataB &= 0x00FFFFFF;
			range &= 0x00FFFFFF;

			dmin = dataB-range;
			dmax = dataB+range;

			if( dataB < dmin )
				dmin = 0;
			if( dataB > dmax )
				dmax = 0xFFFFFF;
			break;

		case	3:	// 4byte
//			dataA &= 0xFFFFFFFF;
//			dataB &= 0xFFFFFFFF;
//			range &= 0xFFFFFFFF;

			dmin = dataB-range;
			dmax = dataB+range;

			if( dataB < dmin )
				dmin = 0;
			if( dataB > dmax )
				dmax = 0xFFFFFFFF;
			break;
	}

	if( dataA >= dmin && dataA <= dmax )
		return	TRUE;

	return	FALSE;
}

DLGMSG	CSearchDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CSearchDlg::OnInitDialog\n" );

	m_nRadix = 10;
	m_nLength = 0;

	HWND	hWndCtrl;
	INT	i, j;
	CHAR	szStr[256];

	// 位置修正
	if( Config.general.rcSearchDlgPos.right-Config.general.rcSearchDlgPos.left != 0
	 && Config.general.rcSearchDlgPos.bottom-Config.general.rcSearchDlgPos.top != 0 ) {
		::SetWindowPos( m_hWnd, HWND_NOTOPMOST, Config.general.rcSearchDlgPos.left, Config.general.rcSearchDlgPos.top,
				0, 0, SWP_NOSIZE | SWP_NOZORDER );
	}

	// サーチ結果初期化
	::memset( &m_Result, 0, sizeof(m_Result) );
	::memset( &m_ResultOld, 0, sizeof(m_ResultOld) );

	// 基数ラジオボタン
	BTNCHECK( IDC_SCH_RADIX_DEC, TRUE );
//	BTNCHECK( IDC_SCH_RADIX_HEX, FALSE );

	// データ長ラジオボタン
	BTNCHECK( IDC_SCH_LENGTH_1BYTE, TRUE );
//	BTNCHECK( IDC_SCH_LENGTH_2BYTE, FALSE );
//	BTNCHECK( IDC_SCH_LENGTH_3BYTE, FALSE );
//	BTNCHECK( IDC_SCH_LENGTH_4BYTE, FALSE );

	// 検索範囲チェックボックス
	BTNCHECK( IDC_SCH_AREA_RAM, TRUE );
	BTNCHECK( IDC_SCH_AREA_SRAM, FALSE );
	BTNCHECK( IDC_SCH_AREA_EXRAM, FALSE );

	// リストビュー
	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_SCH_RESULT_LIST );
	ListView_SetExtendedListViewStyle( hWndCtrl, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );

	ListView_DeleteAllItems( hWndCtrl );
	ListView_SetItemCount( hWndCtrl, 16384 );

	// ヘッダコントロールの設定
	LVCOLUMN	lvColumn;
	lvColumn.mask = LVCF_FMT|LVCF_TEXT|LVCF_SUBITEM|LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_RIGHT;
	lvColumn.pszText = szStr;

	RECT	rc;
	::GetClientRect( hWndCtrl, &rc );
	INT	nWidth = RCWIDTH(rc) - ::GetSystemMetrics( SM_CXVSCROLL );

	CApp::LoadString( IDS_CHT_ADDRESS, szStr, sizeof(szStr) );
	lvColumn.iSubItem = 0;
	lvColumn.cx = nWidth*2/8;
	ListView_InsertColumn( hWndCtrl, 0, &lvColumn );

	CApp::LoadString( IDS_CHT_DATA_OLD, szStr, sizeof(szStr) );
	lvColumn.iSubItem = 1;
	lvColumn.cx = nWidth*3/8;
	ListView_InsertColumn( hWndCtrl, 1, &lvColumn );

	CApp::LoadString( IDS_CHT_DATA_NOW, szStr, sizeof(szStr) );
	lvColumn.iSubItem = 2;
	lvColumn.cx = nWidth*3/8;
	ListView_InsertColumn( hWndCtrl, 2, &lvColumn );

	// データ等設定
	::SetDlgItemText( m_hWnd, IDC_SCH_DATA_EDIT, "0" );

	::SetDlgItemText( m_hWnd, IDC_SCH_WADDR_EDIT, "0000" );
	::SetDlgItemText( m_hWnd, IDC_SCH_WDATA_EDIT, "0" );

	// Undoボタンをディセーブル
	::EnableWindow( ::GetDlgItem( m_hWnd, IDC_SCH_UNDO ), FALSE );

	// 初期データ設定
	for( i = 0; i < 0x0800; i++ ) {
		m_Result.RAM_N[i] = m_Result.RAM_O[i] = Emu.GetNES()->Read( (WORD)i );
		m_Result.RAM_F[i] = 1;
	}

	j = 0;
	BYTE	da = Emu.GetNES()->Read( 0x6000 );
	for( i = 0; i < 0x2000; i++ ) {
		m_Result.SRAM_N[i] = m_Result.SRAM_O[i] = Emu.GetNES()->Read( (WORD)i+0x6000 );
		m_Result.SRAM_F[i] = 1;

		if( m_Result.SRAM_N[i] != da )
			j = 1;
	}

	// SRAMが使用されていたら自動でONに
	if( j ) {
		BTNCHECK( IDC_SCH_AREA_SRAM, TRUE );
	}

	// メニュー
	m_hMenu = CApp::LoadMenu( IDR_SCH_MENU );
	m_hSubMenu = ::GetSubMenu( m_hMenu, 0 );

	OnListUpdate();

	return	TRUE;
}

DLGMSG	CSearchDlg::OnActivate( DLGMSGPARAM )
{
//	DEBUGOUT( "CSearchDlg::OnActivate\n" );

	if( LOWORD(wParam) == WA_INACTIVE ) {
		DEBUGOUT( "CSearchDlg::OnActivate:Inactive\n" );
		if( !m_bShortCutDisable )
			::PostMessage( CApp::GetHWnd(), WM_VNS_SHORTCUTENABLE, (WPARAM)TRUE, 0 );

		if( Emu.IsRunning() )
			Emu.Resume();
	} else {
		DEBUGOUT( "CSearchDlg::OnActivate:Active\n" );
		::PostMessage( CApp::GetHWnd(), WM_VNS_SHORTCUTENABLE, (WPARAM)FALSE, 0 );

		if( Emu.IsRunning() )
			Emu.Pause();
	}

	return	FALSE;
}

DLGMSG	CSearchDlg::OnClose( DLGMSGPARAM )
{
//	DEBUGOUT( "CSearchDlg::OnClose\n" );
	::ShowWindow( m_hWnd, SW_HIDE ); // 非表示にするだけ
	return	TRUE;
}

DLGMSG	CSearchDlg::OnContextMenu( DLGMSGPARAM )
{
//	DEBUGOUT( "CSearchDlg::OnContextMenu\n" );

	HWND	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_SCH_RESULT_LIST );

	POINT	pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);

	LVHITTESTINFO	lvhti;

	lvhti.pt = pt;
	::ScreenToClient( hWndCtrl, &lvhti.pt );

	INT	nItem = ListView_HitTest( hWndCtrl, &lvhti );

//DEBUGOUT( "OnContextMenu nItem=%d\n", nItem );
	if( nItem >= 0 ) {
		CHAR	szStr[256];
		ListView_GetItemText( hWndCtrl, nItem, 0, szStr, sizeof(szStr) );
		m_Address = (WORD)::strtoul( szStr, NULL, 16 );

		::TrackPopupMenu( m_hSubMenu, TPM_LEFTALIGN|TPM_TOPALIGN, pt.x, pt.y, 0, m_hWnd, NULL );
	}

	return	TRUE;
}

DLGCMD	CSearchDlg::OnOK( DLGCMDPARAM )
{
//	DEBUGOUT( "CSearchDlg::OnOK\n" );
//	::EndDialog( m_hWnd, IDOK );
}

DLGCMD	CSearchDlg::OnCancel( DLGCMDPARAM )
{
//	DEBUGOUT( "CSearchDlg::OnCancel\n" );
	::ShowWindow( m_hWnd, SW_HIDE ); // 非表示にするだけ
}

DLGCMD	CSearchDlg::OnRadixCommand( DLGCMDPARAM )
{
	if( uID == IDC_SCH_RADIX_DEC )
		m_nRadix = 10;
	if( uID == IDC_SCH_RADIX_HEX )
		m_nRadix = 16;

	OnListUpdate();
}

DLGCMD	CSearchDlg::OnLengthCommand( DLGCMDPARAM )
{
	m_nLength = (INT)uID - IDC_SCH_LENGTH_1BYTE;

	OnListUpdate();
}

DLGCMD	CSearchDlg::OnStart( DLGCMDPARAM )
{
//	DEBUGOUT( "CSearchDlg::OnStart\n" );

	INT	i;

	// Undoバッファにセーブ
	m_ResultOld = m_Result;

	// 今回データの初期化
	::memset( &m_Result, 0, sizeof(m_Result) );

	// データ検索候補の設定
	if( IsBTNCHECK( IDC_SCH_AREA_RAM ) ) {
		for( i = 0; i < 0x0800; i++ ) {
			m_Result.RAM_N[i] = m_Result.RAM_O[i] = Emu.GetNES()->Read( (WORD)i );
			m_Result.RAM_F[i] = 1;
		}
	}

	if( IsBTNCHECK( IDC_SCH_AREA_SRAM ) ) {
		for( i = 0; i < 0x2000; i++ ) {
			m_Result.SRAM_N[i] = m_Result.SRAM_O[i] = Emu.GetNES()->Read( (WORD)i+0x6000 );
			m_Result.SRAM_F[i] = 1;
		}
	}

	OnListUpdate();

	// Undoボタンをイネーブル
	::EnableWindow( ::GetDlgItem( m_hWnd, IDC_SCH_UNDO ), TRUE );
}

DLGCMD	CSearchDlg::OnUpdate( DLGCMDPARAM )
{
//	DEBUGOUT( "CSearchDlg::OnUpdate\n" );

	INT	i;

	// Undoバッファにセーブ
	m_ResultOld = m_Result;

	// データの更新
	if( IsBTNCHECK( IDC_SCH_AREA_RAM ) ) {
		for( i = 0; i < 0x0800; i++ ) {
			m_Result.RAM_O[i] = m_Result.RAM_N[i];
			m_Result.RAM_N[i] = Emu.GetNES()->Read( (WORD)i );
		}
	}

	if( IsBTNCHECK( IDC_SCH_AREA_SRAM ) ) {
		for( i = 0; i < 0x2000; i++ ) {
			m_Result.SRAM_N[i] = m_Result.SRAM_O[i];
			m_Result.SRAM_N[i] = Emu.GetNES()->Read( (WORD)i+0x6000 );
		}
	}

	OnListUpdate();

	// Undoボタンをイネーブル
	::EnableWindow( ::GetDlgItem( m_hWnd, IDC_SCH_UNDO ), TRUE );
}

DLGCMD	CSearchDlg::OnUndo( DLGCMDPARAM )
{
//	DEBUGOUT( "CSearchDlg::OnUndo\n" );

//	// Undo
	m_Result = m_ResultOld;

	OnListUpdate();

	// Undoボタンをディセーブル
	::EnableWindow( ::GetDlgItem( m_hWnd, IDC_SCH_UNDO ), FALSE );
}

DLGCMD	CSearchDlg::OnSearchCommand( DLGCMDPARAM )
{
//	DEBUGOUT( "CSearchDlg::OnSearchCommand\n" );

	INT	i;
	INT	type = (INT)uID - IDC_SCH_EQUAL;

	// Undoバッファにセーブ
	m_ResultOld = m_Result;

	// テンポラリ
	struct	RESULT	ResultTemp;
	::memset( &ResultTemp, 0, sizeof(ResultTemp) );

	// 候補の中から条件で検索する
	if( IsBTNCHECK( IDC_SCH_AREA_RAM ) ) {
		for( i = 0; i < 0x0800-m_nLength; i++ ) {
			if( m_Result.RAM_F[i] ) {
				if( CompareData( type, GetNesMemory( m_nLength, i ), GetSearchMemory( m_nLength, i ) ) ) {
					ResultTemp.RAM_F[i] = 1;
				}
			}
		}
		// RAMの状態をコピー
		for( i = 0; i < 0x0800; i++ ) {
			ResultTemp.RAM_O[i] = m_Result.RAM_N[i];
			ResultTemp.RAM_N[i] = Emu.GetNES()->Read( (WORD)i );
		}
	}
	if( IsBTNCHECK( IDC_SCH_AREA_SRAM ) ) {
		for( i = 0; i < 0x2000-m_nLength; i++ ) {
			if( m_Result.SRAM_F[i] ) {
				if( CompareData( type, GetNesMemory( m_nLength, i+0x6000 ), GetSearchMemory( m_nLength, i+0x6000 ) ) ) {
					ResultTemp.SRAM_F[i] = 1;
				}
			}
		}
		// RAMの状態をコピー
		for( i = 0; i < 0x2000; i++ ) {
			ResultTemp.SRAM_O[i] = m_Result.SRAM_N[i];
			ResultTemp.SRAM_N[i] = Emu.GetNES()->Read( (WORD)i+0x6000 );
		}
	}

	// テンポラリデータをコピー
	m_Result = ResultTemp;

	OnListUpdate();

	// Undoボタンをイネーブル
	::EnableWindow( ::GetDlgItem( m_hWnd, IDC_SCH_UNDO ), TRUE );
}

DLGCMD	CSearchDlg::OnSearchData( DLGCMDPARAM )
{
//	DEBUGOUT( "CSearchDlg::OnSearchData\n" );

	INT	i;
	DWORD	data;
	CHAR	szStr[256];

	// Undoバッファにセーブ
	m_ResultOld = m_Result;

	// データ/レンジの取得
	::GetDlgItemText( m_hWnd, IDC_SCH_DATA_EDIT, szStr, sizeof(szStr) );
	if( m_nRadix == 10 ) {
		::GetDlgItemText( m_hWnd, IDC_SCH_DATA_EDIT, szStr, sizeof(szStr) );
		data = (DWORD)::strtoul( szStr, NULL, 10 );
	} else {
		::GetDlgItemText( m_hWnd, IDC_SCH_DATA_EDIT, szStr, sizeof(szStr) );
		data = (DWORD)::strtoul( szStr, NULL, 16 );
	}

	// テンポラリ
	struct	RESULT	ResultTemp;
	::memset( &ResultTemp, 0, sizeof(ResultTemp) );

	// 候補の中からデータと範囲で検索する
	if( IsBTNCHECK( IDC_SCH_AREA_RAM ) ) {
		for( i = 0; i < 0x0800-m_nLength; i++ ) {
			if( m_Result.RAM_F[i] ) {
				if( CompareData( 0, GetNesMemory( m_nLength, i ), data ) ) {
					ResultTemp.RAM_F[i] = 1;
				}
			}
		}
		// RAMの状態をコピー
		for( i = 0; i < 0x0800; i++ ) {
			ResultTemp.RAM_O[i] = m_Result.RAM_O[i];
			ResultTemp.RAM_N[i] = Emu.GetNES()->Read( (WORD)i );
		}
	}
	if( IsBTNCHECK( IDC_SCH_AREA_SRAM ) ) {
		for( i = 0; i < 0x2000-m_nLength; i++ ) {
			if( m_Result.SRAM_F[i] ) {
				if( CompareData( 0, GetNesMemory( m_nLength, i+0x6000 ), data ) ) {
					ResultTemp.SRAM_F[i] = 1;
				}
			}
		}
		// RAMの状態をコピー
		for( i = 0; i < 0x2000; i++ ) {
			ResultTemp.SRAM_O[i] = m_Result.SRAM_O[i];
			ResultTemp.SRAM_N[i] = Emu.GetNES()->Read( (WORD)i+0x6000 );
		}
	}

	// テンポラリデータをコピー
	m_Result = ResultTemp;

	OnListUpdate();

	// Undoボタンをイネーブル
	::EnableWindow( ::GetDlgItem( m_hWnd, IDC_SCH_UNDO ), TRUE );
}

DLGCMD	CSearchDlg::OnWriteData( DLGCMDPARAM )
{
//	DEBUGOUT( "CSearchDlg::OnWriteData\n" );

	DWORD	addr, data;
	CHAR	szStr[256];

	// アドレス/データの取得
	::GetDlgItemText( m_hWnd, IDC_SCH_WADDR_EDIT, szStr, sizeof(szStr) );
	addr = (DWORD)::strtoul( szStr, NULL, 16 );

	if( m_nRadix == 10 ) {
		::GetDlgItemText( m_hWnd, IDC_SCH_WDATA_EDIT, szStr, sizeof(szStr) );
		data = (DWORD)::strtoul( szStr, NULL, 10 );
	} else {
		::GetDlgItemText( m_hWnd, IDC_SCH_WDATA_EDIT, szStr, sizeof(szStr) );
		data = (DWORD)::strtoul( szStr, NULL, 16 );
	}

	for( INT i = 0; i <= m_nLength; i++ ) {
		Emu.GetNES()->Write( (WORD)(addr+i), data&0xFF );
		data >>= 8;
	}
}

DLGCMD	CSearchDlg::OnCodeAppend( DLGCMDPARAM )
{
//	DEBUGOUT( "CSearchDlg::OnCodeAppend\n" );

	if( Emu.IsRunning() )
		Emu.Pause();

	CCheatCodeEditDlg dlg;
	dlg.m_Code.enable  = 1;
	dlg.m_Code.address = m_Address;
	dlg.m_Code.data    = 0;
	dlg.m_Code.length  = m_nLength;
	dlg.m_Code.type    = CHEAT_TYPE_ALWAYS;

	dlg.m_nRadix = m_nRadix;

	m_bShortCutDisable = TRUE;

	if( dlg.DoModal( m_hWnd ) == IDOK ) {
		Emu.GetNES()->AddCheatCode( dlg.m_Code );
	}

	m_bShortCutDisable = FALSE;

	if( Emu.IsRunning() )
		Emu.Resume();
}

DLGNOTIFY CSearchDlg::OnDoubleClickListView( DLGNOTIFYPARAM )
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//DEBUGOUT( "Column click!! I:%d S:%d\n", pNMListView->iItem, pNMListView->iSubItem );

	CHAR	szStr[256];
	ListView_GetItemText( pNMHDR->hwndFrom, pNMListView->iItem, 0, szStr, sizeof(szStr) );

	::SetDlgItemText( m_hWnd, IDC_SCH_WADDR_EDIT, szStr );
}

//
// チートコード編集ダイアログ
//
DLG_MESSAGE_BEGIN(CCheatCodeEditDlg)
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )

DLG_COMMAND_BEGIN()
DLG_ON_COMMAND( IDOK, OnOK )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_COMMAND_END()
DLG_MESSAGE_END()

INT	CCheatCodeEditDlg::DoModal( HWND hWndParent )
{
	return	::DialogBoxParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_CODEEDIT),
				hWndParent, g_DlgProc, (LPARAM)this );
}

DLGMSG	CCheatCodeEditDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CCheatCodeEditDlg::OnInitDialog\n" );

	CHAR	szStr[256], szTemp[256];

	// アドレス
	::wsprintf( szStr, "%04X", m_Code.address );
	::SetDlgItemText( m_hWnd, IDC_CED_ADDR, szStr );

	// データ
	if( m_nRadix == 10 ) {
		::wsprintf( szStr, "%d", m_Code.data );
	} else {
		::wsprintf( szTemp, "%%0%dX", (m_Code.length+1)*2 );
		::wsprintf( szStr, szTemp, m_Code.data );
	}
	::SetDlgItemText( m_hWnd, IDC_CED_DATA, szStr );

	// コメント
	::SetDlgItemText( m_hWnd, IDC_CED_COMMENT, m_Code.comment.c_str() );

	// ラジオボタン
	BTNCHECK( (m_nRadix==10)?IDC_CED_RADIX_DEC:IDC_CED_RADIX_HEX, TRUE );
	BTNCHECK( (IDC_CED_LENGTH_1BYTE+m_Code.length), TRUE );

	if( m_Code.type == CHEAT_TYPE_ALWAYS ) {
		BTNCHECK( IDC_CED_TYPE_ALWAYS, TRUE );
	} else
	if( m_Code.type == CHEAT_TYPE_ONCE ) {
		BTNCHECK( IDC_CED_TYPE_ONCE, TRUE );
	} else
	if( m_Code.type == CHEAT_TYPE_GREATER ) {
		BTNCHECK( IDC_CED_TYPE_GREATER, TRUE );
	} else
	if( m_Code.type == CHEAT_TYPE_LESS ) {
		BTNCHECK( IDC_CED_TYPE_LESS, TRUE );
	}

	::PostMessage( CApp::GetHWnd(), WM_VNS_SHORTCUTENABLE, (WPARAM)FALSE, 0 );

	return	TRUE;
}

DLGCMD	CCheatCodeEditDlg::OnOK( DLGCMDPARAM )
{
//	DEBUGOUT( "CCheatCodeEditDlg::OnOK\n" );
	CHAR	szStr[256];
	DWORD	data;

	::GetDlgItemText( m_hWnd, IDC_CED_DATA, szStr, sizeof(szStr) );
	if( IsBTNCHECK( IDC_CED_RADIX_DEC ) ) {
		data = (DWORD)::strtoul( szStr, NULL, 10 );
	} else {
		data = (DWORD)::strtoul( szStr, NULL, 16 );
	}

	if( IsBTNCHECK( IDC_CED_LENGTH_1BYTE ) ) {
		data &= 0x000000FF;
		m_Code.length = CHEAT_LENGTH_1BYTE;
	} else
	if( IsBTNCHECK( IDC_CED_LENGTH_2BYTE ) ) {
		data &= 0x0000FFFF;
		m_Code.length = CHEAT_LENGTH_2BYTE;
	} else
	if( IsBTNCHECK( IDC_CED_LENGTH_3BYTE ) ) {
		data &= 0x00FFFFFF;
		m_Code.length = CHEAT_LENGTH_3BYTE;
	} else
	if( IsBTNCHECK( IDC_CED_LENGTH_4BYTE ) ) {
		m_Code.length = CHEAT_LENGTH_4BYTE;
	}

	m_Code.data    = data;

	if( IsBTNCHECK( IDC_CED_TYPE_ALWAYS ) ) {
		m_Code.type = CHEAT_TYPE_ALWAYS;
	} else
	if( IsBTNCHECK( IDC_CED_TYPE_ONCE ) ) {
		m_Code.type = CHEAT_TYPE_ONCE;
	} else
	if( IsBTNCHECK( IDC_CED_TYPE_GREATER ) ) {
		m_Code.type = CHEAT_TYPE_GREATER;
	} else
	if( IsBTNCHECK( IDC_CED_TYPE_LESS ) ) {
		m_Code.type = CHEAT_TYPE_LESS;
	}

	::GetDlgItemText( m_hWnd, IDC_CED_COMMENT, szStr, sizeof(szStr) );
	m_Code.comment = szStr;

	::EndDialog( m_hWnd, IDOK );
}

DLGCMD	CCheatCodeEditDlg::OnCancel( DLGCMDPARAM )
{
//	DEBUGOUT( "CCheatCodeEditDlg::OnCancel\n" );

	::EndDialog( m_hWnd, IDCANCEL );
}

//
// チートコード入力ダイアログ
//
DLG_MESSAGE_BEGIN(CCheatCodeInputDlg)
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )

DLG_COMMAND_BEGIN()
DLG_ON_COMMAND( IDOK, OnOK )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_COMMAND_END()
DLG_MESSAGE_END()

INT	CCheatCodeInputDlg::DoModal( HWND hWndParent )
{
	return	::DialogBoxParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_CODEINPUT),
				hWndParent, g_DlgProc, (LPARAM)this );
}

DLGMSG	CCheatCodeInputDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CCheatCodeInputDlg::OnInitDialog\n" );

	::SetDlgItemText( m_hWnd, IDC_CHC_CODE, m_Codes.c_str() );
	::SetDlgItemText( m_hWnd, IDC_CHC_COMMENT, m_Comment.c_str() );

	return	TRUE;
}

DLGCMD	CCheatCodeInputDlg::OnOK( DLGCMDPARAM )
{
//	DEBUGOUT( "CCheatCodeInputDlg::OnOK\n" );

	CHAR	szStr[256];

	::GetDlgItemText( m_hWnd, IDC_CHC_CODE, szStr, sizeof(szStr) );
	m_Codes = szStr;

	::GetDlgItemText( m_hWnd, IDC_CHC_COMMENT, szStr, sizeof(szStr) );
	m_Comment = szStr;

	::EndDialog( m_hWnd, IDOK );
}

DLGCMD	CCheatCodeInputDlg::OnCancel( DLGCMDPARAM )
{
//	DEBUGOUT( "CCheatCodeInputDlg::OnCancel\n" );

	::EndDialog( m_hWnd, IDCANCEL );
}

//
// チートコードダイアログ
//

// メッセージ
DLG_MESSAGE_BEGIN(CCheatCodeDlg)
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )
DLG_ON_MESSAGE( WM_DESTROY,	OnDestroy )
DLG_ON_MESSAGE( WM_TIMER,	OnTimer )
// コマンド
DLG_COMMAND_BEGIN()
DLG_ON_COMMAND( IDOK, OnOK )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_ON_COMMAND( IDC_CHT_ENABLE, OnEnable )
DLG_ON_COMMAND( IDC_CHT_DISABLE, OnDisable )
DLG_ON_COMMAND( IDC_CHT_CLEAR, OnClear )
DLG_ON_COMMAND( IDC_CHT_REMOVE, OnRemove )
DLG_ON_COMMAND( IDC_CHT_INPUT, OnInput )
DLG_ON_COMMAND( IDC_CHT_EDIT, OnEdit )
DLG_ON_COMMAND( IDC_CHT_LOAD, OnLoad )
DLG_ON_COMMAND( IDC_CHT_SAVE, OnSave )
DLG_COMMAND_END()
// Notify メッセージ
DLG_NOTIFY_BEGIN()
DLG_ON_NOTIFY( IDC_CHT_CODE_LIST, LVN_KEYDOWN, OnKeyDownListView )
DLG_ON_NOTIFY( IDC_CHT_CODE_LIST, NM_CLICK, OnClickListView )
DLG_ON_NOTIFY( IDC_CHT_CODE_LIST, NM_DBLCLK, OnDblClkListView )
DLG_NOTIFY_END()

DLG_MESSAGE_END()

INT	CCheatCodeDlg::DoModal( HWND hWndParent )
{
	m_hImageList = NULL;

	return	::DialogBoxParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_CHEAT),
				hWndParent, g_DlgProc, (LPARAM)this );
}

void	CCheatCodeDlg::OnListUpdate()
{
	HWND	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_CHT_CODE_LIST );
	ListView_DeleteAllItems( hWndCtrl );

	INT	i;
	CHAR	szStr[256], szTemp[256];

	LVITEM	lvitem;
	lvitem.mask = LVIF_TEXT;
	lvitem.iSubItem = 0;

	INT	codenum = Emu.GetNES()->GetCheatCodeNum();

	for( i = 0; i < codenum; i++ ) {
		CHEATCODE	code;
		Emu.GetNES()->GetCheatCode( i, code );

		// Code
		::wsprintf( szTemp, "%%04X-%%01d%%01d-%%0%dX", (code.length+1)*2 );
		::wsprintf( szStr, szTemp, code.address, code.type, code.length+1, code.data );
		lvitem.pszText = szStr;
		lvitem.iItem = i;

		ListView_InsertItem( hWndCtrl, &lvitem );

		// Comment
		ListView_SetItemText( hWndCtrl, i, 1, (LPSTR)code.comment.c_str() );

		// State
		ListView_SetItemState( hWndCtrl, i, INDEXTOSTATEIMAGEMASK(code.enable+1), LVIS_STATEIMAGEMASK );
	}
}

DLGMSG	CCheatCodeDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CCheatCodeDlg::OnInitDialog\n" );

	HWND	hWndCtrl;
	CHAR	szStr[256];

	// リストビュー
	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_CHT_CODE_LIST );
//	ListView_SetExtendedListViewStyle( hWndCtrl, LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );
	ListView_SetExtendedListViewStyle( hWndCtrl, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );
	ListView_DeleteAllItems( hWndCtrl );

	// イメージリストの作成
	m_hImageList = ImageList_LoadBitmap(
		CApp::GetInstance(), MAKEINTRESOURCE(IDB_CHEATIMAGELIST),
		16, 4, RGB(255,0,255) );

	// イメージリストをリストビューに割り当て
	ListView_SetImageList( hWndCtrl, m_hImageList, LVSIL_STATE );

	// ヘッダコントロールの設定
	LVCOLUMN	lvColumn;
	lvColumn.mask = LVCF_FMT|LVCF_TEXT|LVCF_SUBITEM|LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.pszText = szStr;

	RECT	rc;
	::GetClientRect( hWndCtrl, &rc );
	INT	nWidth = RCWIDTH(rc) - ::GetSystemMetrics( SM_CXVSCROLL );

	CApp::LoadString( IDS_CHT_CHEATCODE, szStr, sizeof(szStr) );
	lvColumn.iSubItem = 0;
	lvColumn.cx = nWidth*2/5;
	ListView_InsertColumn( hWndCtrl, 0, &lvColumn );

	CApp::LoadString( IDS_CHT_COMMENT, szStr, sizeof(szStr) );
	lvColumn.iSubItem = 1;
	lvColumn.cx = nWidth*3/5;
	ListView_InsertColumn( hWndCtrl, 1, &lvColumn );

	// チートコードをバックアップする(Cancel用)
	INT	codenum = Emu.GetNES()->GetCheatCodeNum();
	if( codenum > 0 ) {
		CHEATCODE	code;

		for( INT i = 0; i < codenum; i++ ) {
			if( Emu.GetNES()->GetCheatCode( i, code ) ) {
				m_CheatCode.push_back( code );
			}
		}
	} else {
		m_CheatCode.clear();
	}

	OnListUpdate();

	m_uTimerID = ::SetTimer( m_hWnd, 0x0001, 1000, NULL );	// 1sec

	return	TRUE;
}

DLGMSG	CCheatCodeDlg::OnDestroy( DLGMSGPARAM )
{
//	DEBUGOUT( "CCheatCodeDlg::OnDestroy\n" );

	::KillTimer( m_hWnd, m_uTimerID );
	m_uTimerID = 0;

	if( m_hImageList ) {
		ImageList_Destroy( m_hImageList );
		m_hImageList = NULL;
	}

	return	TRUE;
}

DLGMSG	CCheatCodeDlg::OnTimer( DLGMSGPARAM )
{
//	DEBUGOUT( "CCheatCodeDlg::OnTimer\n" );

	if( Emu.GetNES()->IsCheatCodeAdd() ) {
		OnListUpdate();
	}

	return	TRUE;
}

DLGNOTIFY CCheatCodeDlg::OnKeyDownListView( DLGNOTIFYPARAM )
{
//	DEBUGOUT( "CCheatCodeDlg::OnKeyDownListView\n" );

	NMLVKEYDOWN* pNMKeyDown = (NMLVKEYDOWN*)pNMHDR;

	if( pNMKeyDown->wVKey != VK_SPACE )
		return;

	INT	codenum = Emu.GetNES()->GetCheatCodeNum();

	for( INT i = 0; i < codenum; i++ ) {
		if( ListView_GetItemState( pNMKeyDown->hdr.hwndFrom, i, LVIS_SELECTED ) ) {
			UINT	state = ListView_GetItemState( pNMKeyDown->hdr.hwndFrom, i, LVIS_STATEIMAGEMASK );
			if( (state+=0x1000) > 0x4000 )
				state = 0x1000;
			ListView_SetItemState( pNMKeyDown->hdr.hwndFrom, i, state, LVIS_STATEIMAGEMASK );

			// コード更新
			CHEATCODE	code;
			if( Emu.GetNES()->GetCheatCode( i, code ) ) {
				code.enable = (state>>12)-1;
				Emu.GetNES()->ReplaceCheatCode( i, code );
			}
			break;
		}
	}
}

DLGNOTIFY CCheatCodeDlg::OnClickListView( DLGNOTIFYPARAM )
{
//	DEBUGOUT( "CCheatCodeDlg::OnClickListView\n" );

	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;

	LVHITTESTINFO	lvht;

	lvht.pt = pNMListView->ptAction;
	ListView_HitTest( pNMListView->hdr.hwndFrom, &lvht );

	// ステートイメージ以外は帰る
	if( (lvht.flags&LVHT_NOWHERE) || (lvht.flags&LVHT_ONITEMLABEL) || lvht.iItem < 0 )
		return;

	// ステートアイコンならば更新
	if( lvht.flags & LVHT_ONITEMSTATEICON ) {
		UINT	state = ListView_GetItemState( pNMListView->hdr.hwndFrom, lvht.iItem, LVIS_STATEIMAGEMASK );
		if( (state+=0x1000) > 0x4000 )
			state = 0x1000;
		ListView_SetItemState( pNMListView->hdr.hwndFrom, lvht.iItem, state, LVIS_STATEIMAGEMASK );

		// コード更新
		CHEATCODE	code;
		if( Emu.GetNES()->GetCheatCode( lvht.iItem, code ) ) {
			code.enable = (state>>12)-1;
			Emu.GetNES()->ReplaceCheatCode( lvht.iItem, code );
		}
	}
}

DLGNOTIFY CCheatCodeDlg::OnDblClkListView( DLGNOTIFYPARAM )
{
//	DEBUGOUT( "CCheatCodeDlg::OnClickListView\n" );

	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;

	LVHITTESTINFO	lvht;

	lvht.pt = pNMListView->ptAction;
	ListView_HitTest( pNMListView->hdr.hwndFrom, &lvht );

	// ラベル以外は帰る
	if( (lvht.flags&LVHT_NOWHERE) || !(lvht.flags&LVHT_ONITEMLABEL) || lvht.iItem < 0 )
		return;

	// エディットしてね…と。
	::PostMessage( m_hWnd, WM_COMMAND, (WPARAM)IDC_CHT_EDIT, (LPARAM)0 );
}

DLGCMD	CCheatCodeDlg::OnOK( DLGCMDPARAM )
{
//	DEBUGOUT( "CCheatCodeDlg::OnOK\n" );

	// 念のため
	m_CheatCode.clear();
	::EndDialog( m_hWnd, IDOK );
}

DLGCMD	CCheatCodeDlg::OnCancel( DLGCMDPARAM )
{
//	DEBUGOUT( "CCheatCodeDlg::OnCancel\n" );

	// バックアップしていたチートコードを戻す
	Emu.GetNES()->CheatInitial();
	for( INT i = 0; i < m_CheatCode.size(); i++ ) {
		Emu.GetNES()->AddCheatCode( m_CheatCode[i] );
	}

	// 念のため
	m_CheatCode.clear();
	::EndDialog( m_hWnd, IDCANCEL );
}

DLGCMD	CCheatCodeDlg::OnEnable( DLGCMDPARAM )
{
//	DEBUGOUT( "CCheatCodeDlg::OnEnable\n" );

	Emu.GetNES()->SetCheatCodeAllFlag( TRUE, FALSE );
	OnListUpdate();
}

DLGCMD	CCheatCodeDlg::OnDisable( DLGCMDPARAM )
{
//	DEBUGOUT( "CCheatCodeDlg::OnDisable\n" );

	Emu.GetNES()->SetCheatCodeAllFlag( FALSE, FALSE );
	OnListUpdate();
}

DLGCMD	CCheatCodeDlg::OnClear( DLGCMDPARAM )
{
//	DEBUGOUT( "CCheatCodeDlg::OnClear\n" );

	Emu.GetNES()->CheatInitial();
	OnListUpdate();
}

DLGCMD	CCheatCodeDlg::OnRemove( DLGCMDPARAM )
{
//	DEBUGOUT( "CCheatCodeDlg::OnRemove\n" );

	HWND	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_CHT_CODE_LIST );
	INT	nCount = ListView_GetItemCount( hWndCtrl );

	for( INT i = 0; i < nCount; i++ ) {
		if( ListView_GetItemState( hWndCtrl, i, LVIS_SELECTED ) ) {
			ListView_DeleteItem( hWndCtrl, i );
			// 消す
			Emu.GetNES()->DelCheatCode( i );
			break;
		}
	}
}

DLGCMD	CCheatCodeDlg::OnInput( DLGCMDPARAM )
{
	DEBUGOUT( "CCheatCodeDlg::OnInput\n" );

	CCheatCodeInputDlg dlg;

	if( dlg.DoModal( m_hWnd ) == IDOK ) {
		CHEATCODE	code;

		if( ::strlen( dlg.m_Codes.c_str() ) < 10 )
			return;

		const UCHAR seps[] = " -\t\n\0";	// セパレータ
		CHAR	szStr[256];

		::wsprintf( szStr, "%s", dlg.m_Codes.c_str() );

		CHAR*	pToken;

		// 初期状態でイネーブルにする
		code.enable = CHEAT_ENABLE;

		// Initial state
		if( szStr[0] == '#' ) {
			if( (pToken = (CHAR*)_mbstok( (UCHAR*)szStr, seps )) ) {
				if( ::strlen(pToken) == 2 ) {
					if( pToken[1] >= '0' && pToken[1] <= '3' ) {
						code.enable = pToken[1] - '0';
					}
				}
			}
		}

		// Address
		if( szStr[0] != '#' ) {
			if( !(pToken = (CHAR*)_mbstok( (UCHAR*)szStr, seps )) )
				return;
		} else {
			if( !(pToken = (CHAR*)_mbstok( NULL, seps )) )
				return;
		}

		code.address = ::strtoul( pToken, NULL, 16 );
		// Type & Length
		if( !(pToken = (CHAR*)_mbstok( NULL, seps )) )
			return;
		code.type   = pToken[0]-'0';
		code.length = pToken[1]-'1';
		if( code.type < CHEAT_TYPE_ALWAYS )
			code.type = CHEAT_TYPE_ALWAYS;
		if( code.type > CHEAT_TYPE_LESS )
			code.type = CHEAT_TYPE_ALWAYS;

		if( code.length < CHEAT_LENGTH_1BYTE )
			code.length = CHEAT_LENGTH_1BYTE;
		if( code.length > CHEAT_LENGTH_4BYTE )
			code.length = CHEAT_LENGTH_1BYTE;

		// Data
		if( !(pToken = (CHAR*)_mbstok( NULL, seps )) )
			return;
		code.data = ::strtoul( pToken, NULL, 16 );

		// Comment
		code.comment = dlg.m_Comment;

		// 追加
		Emu.GetNES()->AddCheatCode( code );
		// タイマーで検知しないようにフラグ消し
		(void)Emu.GetNES()->IsCheatCodeAdd();

		// リスト更新
		OnListUpdate();
	}
}

DLGCMD	CCheatCodeDlg::OnEdit( DLGCMDPARAM )
{
//	DEBUGOUT( "CCheatCodeDlg::OnEdit\n" );

	HWND	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_CHT_CODE_LIST );
	INT	nCount = ListView_GetItemCount( hWndCtrl );
	INT	i;

	for( i = 0; i < nCount; i++ ) {
		if( ListView_GetItemState( hWndCtrl, i, LVIS_SELECTED ) ) {
			CCheatCodeEditDlg dlg;

			if( Emu.GetNES()->GetCheatCode( i, dlg.m_Code ) ) {
				dlg.m_nRadix = 16;
				if( dlg.DoModal( m_hWnd ) == IDOK ) {
					Emu.GetNES()->ReplaceCheatCode( i, dlg.m_Code );
					OnListUpdate();
				}
			}
			break;
		}
	}
}

DLGCMD	CCheatCodeDlg::OnLoad( DLGCMDPARAM )
{
//	DEBUGOUT( "CCheatCodeDlg::OnLoad\n" );

	string	pathstr, tempstr;
	if( Config.path.bCheatPath ) {
		pathstr = CPathlib::CreatePath( CApp::GetModulePath(), Config.path.szCheatPath );
		::CreateDirectory( pathstr.c_str(), NULL );
	} else {
		pathstr = Emu.GetNES()->rom->GetRomPath();
	}
	tempstr = CPathlib::MakePathExt( pathstr.c_str(), Emu.GetNES()->rom->GetRomName(), "vct" );
	DEBUGOUT( "Path: %s\n", tempstr.c_str() );

	OPENFILENAME	ofn;
	CHAR	szFile[_MAX_PATH];

	::strcpy( szFile, tempstr.c_str() );
	ZEROMEMORY( &ofn, sizeof(ofn) );

	ofn.lStructSize     = sizeof(ofn);
	ofn.hwndOwner       = m_hWnd;
	ofn.lpstrFile       = szFile;
	ofn.nMaxFile        = sizeof(szFile);
	ofn.lpstrFilter     = "Cheat Files(*.vct)\0*.vct\0All Files(*.*)\0*.*\0";
	ofn.nFilterIndex    = 1;
	ofn.Flags           = OFN_EXPLORER|OFN_PATHMUSTEXIST;
	ofn.lpstrInitialDir = pathstr.c_str();

	CHAR	szTitle[256], szTemp[256];

	CApp::LoadString( IDS_UI_LOADCHEATCODE, szTitle, sizeof(szTitle) );
	ofn.lpstrTitle = szTitle;
	if( ::GetOpenFileName( &ofn ) ) {
		FILE*	fp = NULL;

		if( (fp = ::fopen( szFile, "r" )) ) {
			// 一旦消す
			Emu.GetNES()->CheatInitial();

			while( ::fgets( szTemp, sizeof(szTemp), fp ) ) {
				if( szTemp[0] == ';' )
					continue;

				CHEATCODE	code;

				if( ::strlen( szTemp ) < 10 )
					continue;

				const UCHAR seps[] = " -\t\n\0";	// セパレータ
				const UCHAR seps2[] = "\t\n\0";		// セパレータ
				const UCHAR seps3[] = "-\t\n\0";	// セパレータ

				CHAR*	pToken;

				code.enable = CHEAT_ENABLE;

				// Initial state
				if( szTemp[0] == '#' ) {
					if( !(pToken = (CHAR*)_mbstok( (UCHAR*)szTemp, seps )) )
						continue;

					if( ::strlen(pToken) == 2 ) {
						if( pToken[1] >= '0' && pToken[1] <= '3' ) {
							code.enable = pToken[1] - '0';
						}
					}
				}

				// Address
				if( szTemp[0] != '#' ) {
					if( !(pToken = (CHAR*)_mbstok( (UCHAR*)szTemp, seps )) )
						continue;
				} else {
					if( !(pToken = (CHAR*)_mbstok( NULL, seps )) )
						continue;
				}

				if( ::strlen( pToken ) == 4 ) {
				// VirtuaNES code
					code.address = ::strtoul( pToken, NULL, 16 );
					// Type & Length
					if( !(pToken = (CHAR*)_mbstok( NULL, seps )) )
						continue;

					if( ::strlen( pToken ) == 2 ) {
					// VirtuaNES code
						code.type   = pToken[0]-'0';
						code.length = pToken[1]-'1';
					} else {
					// NNNesterJ code?
						code.type   = CHEAT_TYPE_ALWAYS;
						code.length = pToken[0]-'1';
					}
					if( code.type < CHEAT_TYPE_ALWAYS )
						code.type = CHEAT_TYPE_ALWAYS;
					if( code.type > CHEAT_TYPE_LESS )
						code.type = CHEAT_TYPE_ALWAYS;

					if( code.length < CHEAT_LENGTH_1BYTE )
						code.length = CHEAT_LENGTH_1BYTE;
					if( code.length > CHEAT_LENGTH_4BYTE )
						code.length = CHEAT_LENGTH_1BYTE;

					// Data
					if( !(pToken = (CHAR*)_mbstok( NULL, seps )) )
						continue;
					code.data = ::strtoul( pToken, NULL, 16 );

					// Comment
					if( (pToken = (CHAR*)_mbstok( NULL, seps2 )) ) {
						code.comment = pToken;
					} else {
						code.comment = "";
					}
				} else if( ::strlen( pToken ) == 5 ) {
				// NNNesterJ code?
					if( pToken[0] == '0' ) {
						code.address = ::strtoul( pToken+1, NULL, 16 );
					} else if( pToken[0] == '1' ) {
						code.address = ::strtoul( pToken+1, NULL, 16 );
						code.address += 0x6000;
					}
					// Length
					if( !(pToken = (CHAR*)_mbstok( NULL, seps )) )
						continue;
					code.type = CHEAT_TYPE_ALWAYS;
					code.length = pToken[1]-'1';
					if( code.length < CHEAT_LENGTH_1BYTE )
						code.length = CHEAT_LENGTH_1BYTE;
					if( code.length > CHEAT_LENGTH_4BYTE )
						code.length = CHEAT_LENGTH_1BYTE;

					// Data
					if( !(pToken = (CHAR*)_mbstok( NULL, seps )) )
						continue;
					code.data = ::strtoul( pToken, NULL, 16 );

					// Comment
					if( (pToken = (CHAR*)_mbstok( NULL, seps3 )) ) {
						code.comment = pToken;
					} else {
						code.comment = "";
					}
				}

				// 追加
				Emu.GetNES()->AddCheatCode( code );
				// タイマーで検知しないようにフラグ消し
				(void)Emu.GetNES()->IsCheatCodeAdd();
			}
			// リスト更新
			OnListUpdate();
		}
		FCLOSE( fp );
	}
}

DLGCMD	CCheatCodeDlg::OnSave( DLGCMDPARAM )
{
//	DEBUGOUT( "CCheatCodeDlg::OnSave\n" );

	INT	codenum = Emu.GetNES()->GetCheatCodeNum();

	if( !codenum )
		return;

	string	pathstr, tempstr;
	if( Config.path.bCheatPath ) {
		pathstr = CPathlib::CreatePath( CApp::GetModulePath(), Config.path.szCheatPath );
		::CreateDirectory( pathstr.c_str(), NULL );
	} else {
		pathstr = Emu.GetNES()->rom->GetRomPath();
	}
	tempstr = CPathlib::MakePathExt( pathstr.c_str(), Emu.GetNES()->rom->GetRomName(), "vct" );
	DEBUGOUT( "Path: %s\n", tempstr.c_str() );

	OPENFILENAME	ofn;
	CHAR	szFile[_MAX_PATH];

	::strcpy( szFile, tempstr.c_str() );
	ZEROMEMORY( &ofn, sizeof(ofn) );

	ofn.lStructSize     = sizeof(ofn);
	ofn.hwndOwner       = m_hWnd;
	ofn.lpstrFile       = szFile;
	ofn.nMaxFile        = sizeof(szFile);
	ofn.lpstrFilter     = "Cheat Files(*.vct)\0*.vct\0All Files(*.*)\0*.*\0";
	ofn.nFilterIndex    = 1;
	ofn.Flags           = OFN_EXPLORER|OFN_PATHMUSTEXIST;
	ofn.lpstrInitialDir = pathstr.c_str();

	CHAR	szTitle[256], szTemp[256], szTemp2[256];

	ofn.Flags |= OFN_OVERWRITEPROMPT;
	CApp::LoadString( IDS_UI_SAVECHEATCODE, szTitle, sizeof(szTitle) );
	ofn.lpstrTitle = szTitle;
	if( ::GetSaveFileName( &ofn ) ) {
		FILE*	fp = NULL;

		if( (fp = ::fopen( szFile, "w" )) ) {
			::fprintf( fp, "; %s\n", Emu.GetNES()->rom->GetRomName() );

			// コードをファイルに保存
			CHEATCODE	code;
			for( INT i = 0; i < codenum; i++ ) {
				Emu.GetNES()->GetCheatCode( i, code );
				// Code
				::wsprintf( szTemp, "#%01d %%04X-%%01d%%01d-%%0%dX", code.enable, (code.length+1)*2 );
				::wsprintf( szTemp2, szTemp, code.address, code.type, code.length+1, code.data );
				::fputs( szTemp2, fp );
				if( ::strlen(szTemp2) >= 16 )
					::fprintf( fp, "\t%s\n", code.comment.c_str() );
				else
					::fprintf( fp, "\t\t%s\n", code.comment.c_str() );
			}
		}
		FCLOSE( fp );
	}
}

