//
// パターンビュークラス
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <commdlg.h>
#include <string>
using namespace std;

#include "VirtuaNESres.h"
#include "DebugOut.h"
#include "App.h"
#include "Pathlib.h"

#include "Wnd.h"
#include "PaletteEdit.h"
#include "EmuThread.h"

#include "MMU.h"

#include "DirectDraw.h"

// メッセージ
DLG_MESSAGE_BEGIN(CPaletteEdit)
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )
DLG_ON_MESSAGE( WM_DESTROY,	OnDestroy )
DLG_ON_MESSAGE( WM_ACTIVATE,	OnActivate )
DLG_ON_MESSAGE( WM_LBUTTONDOWN,	OnLButtonDown )
DLG_ON_MESSAGE( WM_RBUTTONDOWN,	OnRButtonDown )
DLG_ON_MESSAGE( WM_HSCROLL,	OnHScroll )
DLG_ON_MESSAGE( WM_PAINT,	OnPaint )
DLG_ON_MESSAGE( WM_TIMER,	OnTimer )

// コマンド
DLG_COMMAND_BEGIN()
DLG_ON_COMMAND( IDOK, OnOK )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_ON_COMMAND( IDDEFAULT, OnDefault )

DLG_ON_COMMAND_NOTIFY_RANGE( IDC_PAL_R_EDIT, IDC_PAL_B_EDIT, EN_CHANGE, OnRGBChange )
DLG_ON_COMMAND( IDC_PAL_UNDO, OnUndo )
DLG_ON_COMMAND( IDC_PAL_LOAD, OnLoad )
DLG_ON_COMMAND( IDC_PAL_SAVE, OnSave )

DLG_COMMAND_END()
// Notify メッセージ
DLG_NOTIFY_BEGIN()
DLG_NOTIFY_END()
DLG_MESSAGE_END()

CPaletteEdit::CPaletteEdit()
{
}

CPaletteEdit::~CPaletteEdit()
{
}

BOOL	CPaletteEdit::Create( HWND hWndParent )
{
	m_bShortCutDisable = FALSE;

	m_hWnd = ::CreateDialogParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_CFG_PALETTE),
				hWndParent, g_DlgProc, (LPARAM)this );
	if( !m_hWnd )
		return	FALSE;

	// モードレスダイアログリストに加える
	CWndList::Add( this );

	DirectDraw.GetPaletteTable( m_Palette );

	m_PaletteSelect = 0;
	ZEROMEMORY( &m_RGB, sizeof(RGBQUAD) );

	return	TRUE;
}

void	CPaletteEdit::Destroy()
{
	if( m_hWnd ) {
		// 位置保存
		::GetWindowRect( m_hWnd, &Config.general.rcPaletteEditPos );

		// モードレスダイアログリストから削除
		CWndList::Del( this );

		::KillTimer( m_hWnd, 1 );

		::DestroyWindow( m_hWnd );
		m_hWnd = NULL;
	}
}

DLGMSG	CPaletteEdit::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CPaletteEdit::OnInitDialog\n" );

	// 位置修正
	if( Config.general.rcPaletteEditPos.right-Config.general.rcPaletteEditPos.left != 0
	 && Config.general.rcPaletteEditPos.bottom-Config.general.rcPaletteEditPos.top != 0 ) {
		::SetWindowPos( m_hWnd, HWND_NOTOPMOST, Config.general.rcPaletteEditPos.left, Config.general.rcPaletteEditPos.top,
				0, 0, SWP_NOSIZE | SWP_NOZORDER );
	}

	// UPDOWN
	::SendDlgItemMessage( m_hWnd, IDC_PAL_R_UPDOWN, UDM_SETRANGE, 0, (LPARAM)MAKELONG(255, 0) ); 
	::SendDlgItemMessage( m_hWnd, IDC_PAL_G_UPDOWN, UDM_SETRANGE, 0, (LPARAM)MAKELONG(255, 0) ); 
	::SendDlgItemMessage( m_hWnd, IDC_PAL_B_UPDOWN, UDM_SETRANGE, 0, (LPARAM)MAKELONG(255, 0) ); 

	// スライダ
	::SendDlgItemMessage( m_hWnd, IDC_PAL_R_SLIDER, TBM_SETRANGE, 0, MAKELONG(0,255) );
	::SendDlgItemMessage( m_hWnd, IDC_PAL_G_SLIDER, TBM_SETRANGE, 0, MAKELONG(0,255) );
	::SendDlgItemMessage( m_hWnd, IDC_PAL_B_SLIDER, TBM_SETRANGE, 0, MAKELONG(0,255) );
	::SendDlgItemMessage( m_hWnd, IDC_PAL_R_SLIDER, TBM_SETPAGESIZE, 0, 10 );
	::SendDlgItemMessage( m_hWnd, IDC_PAL_G_SLIDER, TBM_SETPAGESIZE, 0, 10 );
	::SendDlgItemMessage( m_hWnd, IDC_PAL_B_SLIDER, TBM_SETPAGESIZE, 0, 10 );

	// COPY/EXCHANGE
	BTNCHECK( IDC_PAL_COPY, TRUE );

	m_bShortCutDisable = TRUE;

	DirectDraw.GetPaletteTable( m_Palette );
	::memcpy( m_PaletteUndo, m_Palette, sizeof(m_Palette) );
	::memcpy( m_PaletteDefault, m_Palette, sizeof(m_Palette) );
	::memcpy( m_BGPAL, BGPAL, 16 );
	::memcpy( m_SPPAL, SPPAL, 16 );

	m_PaletteSelect = 0;
	OnChaneSelect();

	::SetTimer( m_hWnd, 1, 50, NULL );

	return	TRUE;
}

DLGMSG	CPaletteEdit::OnDestroy( DLGMSGPARAM )
{
//	DEBUGOUT( "CPaletteEdit::OnDestroy\n" );
	// 位置保存
	::GetWindowRect( m_hWnd, &Config.general.rcPaletteEditPos );

	// モードレスダイアログリストから削除
	CWndList::Del( this );

	m_hWnd = NULL;
	m_bShortCutDisable = FALSE;

	return	TRUE;
}

DLGMSG	CPaletteEdit::OnActivate( DLGMSGPARAM )
{
//	DEBUGOUT( "CPaletteEdit::OnActivate\n" );

	if( LOWORD(wParam) == WA_INACTIVE ) {
//		DEBUGOUT( "CPaletteEdit::OnActivate:Inactive\n" );
////		if( !m_bShortCutDisable )
		::PostMessage( CApp::GetHWnd(), WM_VNS_SHORTCUTENABLE, (WPARAM)TRUE, 0 );
////		m_bShortCutDisable = TRUE;

////		if( Emu.IsRunning() )
////			Emu.Resume();
	} else {
//		DEBUGOUT( "CPaletteEdit::OnActivate:Active\n" );
		::PostMessage( CApp::GetHWnd(), WM_VNS_SHORTCUTENABLE, (WPARAM)FALSE, 0 );
////		m_bShortCutDisable = FALSE;

////		if( Emu.IsRunning() )
////			Emu.Pause();
	}

	return	FALSE;
}

DLGMSG	CPaletteEdit::OnLButtonDown( DLGMSGPARAM )
{
//	DEBUGOUT( "CPaletteEdit::OnLButtonDown\n" );

	INT	xp = LOWORD(lParam);
	INT	yp = HIWORD(lParam);

	RECT	rcC;
	::GetClientRect( m_hWnd, &rcC );

	RECT	rc;
	rc.left   = ((rcC.right-rcC.left) - (CELL_SIZE*CELL_H)) / 2;
	rc.right  = rc.left + CELL_H * CELL_SIZE;
	rc.top    = 8;
	rc.bottom = rc.top + CELL_V * CELL_SIZE;

	if( xp >= rc.left && xp < rc.right && yp >= rc.top && yp < rc.bottom ) {
		m_PaletteSelect = ((xp-rc.left)/CELL_SIZE) + ((yp-rc.top)/CELL_SIZE)*16;

		::memcpy( m_PaletteUndo, m_Palette, sizeof(m_Palette) );

		OnChaneSelect();
	}

	rc.top    = rc.bottom+8;
	rc.bottom = rc.top + 2 * CELL_SIZE;

	if( xp >= rc.left && xp < rc.right && yp >= rc.top && yp < rc.bottom ) {
		int	x = (xp-rc.left)/CELL_SIZE;
		int	y = (yp-rc.top)/CELL_SIZE;

		if( y == 0 ) m_PaletteSelect = BGPAL[x];
		else	     m_PaletteSelect = SPPAL[x];

		::memcpy( m_PaletteUndo, m_Palette, sizeof(m_Palette) );

		OnChaneSelect();
	}

	return	TRUE;
}

DLGMSG	CPaletteEdit::OnRButtonDown( DLGMSGPARAM )
{
//	DEBUGOUT( "CPaletteEdit::OnRButtonDown\n" );

	INT	xp = LOWORD(lParam);
	INT	yp = HIWORD(lParam);

	RECT	rcC;
	::GetClientRect( m_hWnd, &rcC );

	RECT	rc;
	rc.left   = ((rcC.right-rcC.left) - (CELL_SIZE*CELL_H)) / 2;
	rc.right  = rc.left + CELL_H * CELL_SIZE;
	rc.top    = 8;
	rc.bottom = rc.top + CELL_V * CELL_SIZE;

	if( xp >= rc.left && xp < rc.right && yp >= rc.top && yp < rc.bottom ) {
		int nSel = ((xp-rc.left)/CELL_SIZE) + ((yp-rc.top)/CELL_SIZE)*16;

		::memcpy( m_PaletteUndo, m_Palette, sizeof(m_Palette) );

		if( IsBTNCHECK( IDC_PAL_COPY ) ) {
			m_Palette[nSel] = m_Palette[m_PaletteSelect];
		} else {
			RGBQUAD	RGBtemp;
			RGBtemp = m_Palette[m_PaletteSelect];
			m_RGB = m_Palette[m_PaletteSelect] = m_Palette[nSel];
			m_Palette[nSel] = RGBtemp;
		}

		OnChaneSelect();
	}

	rc.top    = rc.bottom+8;
	rc.bottom = rc.top + 2 * CELL_SIZE;

	if( xp >= rc.left && xp < rc.right && yp >= rc.top && yp < rc.bottom ) {
		int	x = (xp-rc.left)/CELL_SIZE;
		int	y = (yp-rc.top)/CELL_SIZE;

		int	nSel;
		if( y == 0 ) nSel = BGPAL[x];
		else	     nSel = SPPAL[x];

		::memcpy( m_PaletteUndo, m_Palette, sizeof(m_Palette) );

		m_Palette[nSel] = m_Palette[m_PaletteSelect];

		OnChaneSelect();
	}

	return	TRUE;
}

DLGMSG	CPaletteEdit::OnHScroll( DLGMSGPARAM )
{
//	DEBUGOUT( "CPaletteEdit::OnHScroll\n" );

//	int	nScrollCode = (int)LOWORD(wParam);	// scroll bar value 
//	int	nPos = (int) HIWORD(wParam);		// scroll box position 
	HWND	hWndBar = (HWND)lParam;		// handle to scroll bar 

	int	nEdit = 0;
	int	nPos = ::SendMessage( hWndBar, TBM_GETPOS, 0, 0 );

	if( hWndBar == ::GetDlgItem( m_hWnd, IDC_PAL_R_SLIDER ) ) {
		nEdit = IDC_PAL_R_EDIT;
	}
	if( hWndBar == ::GetDlgItem( m_hWnd, IDC_PAL_G_SLIDER ) ) {
		nEdit = IDC_PAL_G_EDIT;
	}
	if( hWndBar == ::GetDlgItem( m_hWnd, IDC_PAL_B_SLIDER ) ) {
		nEdit = IDC_PAL_B_EDIT;
	}

	if( nEdit ) {
		CHAR	szStr[16];
		::wsprintf( szStr, "%d", nPos );
		::SetWindowText( ::GetDlgItem( m_hWnd, nEdit ), szStr );
	}

	return	TRUE;
}

DLGMSG	CPaletteEdit::OnPaint( DLGMSGPARAM )
{
//	DEBUGOUT( "CPaletteEdit::OnPaint\n" );

	PAINTSTRUCT	ps;
	HDC	hDC = ::BeginPaint( hWnd, &ps );

	int	xoffs;
	RECT	rcC;
	::GetClientRect( m_hWnd, &rcC );

	xoffs = ((rcC.right-rcC.left) - (CELL_SIZE*CELL_H)) / 2;

	RECT	rc;
	for( int y = 0; y < CELL_V; y++ ) {
		for( int x = 0; x < CELL_H; x++ ) {
			rc.left   = xoffs + x * CELL_SIZE;
			rc.right  = rc.left + CELL_SIZE;
			rc.top    = 8 + y * CELL_SIZE;
			rc.bottom = rc.top + CELL_SIZE;

			::SetBkColor( hDC, RGB(m_Palette[x+y*16].rgbRed,m_Palette[x+y*16].rgbGreen,m_Palette[x+y*16].rgbBlue) );
			::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL );

			if( m_PaletteSelect == (x+y*16) ) {
				::DrawEdge( hDC, &rc, BDR_SUNKENOUTER, BF_RECT );
			} else {
				::DrawEdge( hDC, &rc, BDR_RAISEDOUTER, BF_RECT );
			}
		}
	}

	// BG/SP PALETTE
	rc.left   = xoffs;
	rc.right  = rc.left + CELL_SIZE;
	for( int i = 0; i < 16; i++ ) {
		rc.top    = 8 + 8 + 4 * CELL_SIZE;
		rc.bottom = rc.top + CELL_SIZE;
		::SetBkColor( hDC, RGB(m_Palette[m_BGPAL[i]].rgbRed,m_Palette[m_BGPAL[i]].rgbGreen,m_Palette[m_BGPAL[i]].rgbBlue) );
		::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL );
		::DrawEdge( hDC, &rc, BDR_RAISEDOUTER, BF_RECT );

		rc.top    += CELL_SIZE;
		rc.bottom += CELL_SIZE;
		::SetBkColor( hDC, RGB(m_Palette[m_SPPAL[i]].rgbRed,m_Palette[m_SPPAL[i]].rgbGreen,m_Palette[m_SPPAL[i]].rgbBlue) );
		::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL );
		::DrawEdge( hDC, &rc, BDR_RAISEDOUTER, BF_RECT );

		rc.left  += CELL_SIZE;
		rc.right += CELL_SIZE;
	}

	::EndPaint( hWnd, &ps );

	return	TRUE;
}

DLGMSG	CPaletteEdit::OnTimer( DLGMSGPARAM )
{
	if( ::memcmp( m_BGPAL, BGPAL, 16 ) != 0 || ::memcmp( m_SPPAL, SPPAL, 16 ) != 0 ) {
		::memcpy( m_BGPAL, BGPAL, 16 );
		::memcpy( m_SPPAL, SPPAL, 16 );

		RECT	rcC;
		::GetClientRect( m_hWnd, &rcC );
		rcC.bottom = 8 + 8 + CELL_V*CELL_SIZE + 2*CELL_SIZE;

		::InvalidateRect( m_hWnd, &rcC, FALSE );
	}

	return	TRUE;
}

DLGCMD	CPaletteEdit::OnOK( DLGCMDPARAM )
{
//	DEBUGOUT( "CPaletteEdit::OnOK\n" );
	::DestroyWindow( m_hWnd );
}

DLGCMD	CPaletteEdit::OnCancel( DLGCMDPARAM )
{
//	DEBUGOUT( "CPaletteEdit::OnCancel\n" );
	::DestroyWindow( m_hWnd );
}

DLGCMD	CPaletteEdit::OnDefault( DLGCMDPARAM )
{
//	DEBUGOUT( "CPaletteEdit::OnDefault\n" );

	::memcpy( m_Palette, m_PaletteDefault, sizeof(m_PaletteDefault) );
	::memcpy( m_PaletteUndo, m_PaletteDefault, sizeof(m_PaletteDefault) );

	OnChaneSelect();
}

DLGCMD	CPaletteEdit::OnRGBChange( DLGCMDPARAM )
{
//	DEBUGOUT( "CPaletteEdit::OnRGBChange\n" );

	CHAR	szColor[16];
	INT	val = 0;

	::GetWindowText( hWnd, szColor, sizeof(szColor)-1 );
	if( ::lstrlen(szColor) > 0 ) {
		val = ::atoi(szColor);
		if( val < 0 ) {
			val = 0;
			::SetWindowText( hWnd, "0" );
		} else if( val > 255 ) {
			val = 255;
			::SetWindowText( hWnd, "255" );
		}
	} else {
		val = 0;
		::SetWindowText( hWnd, "0" );
	}

	int	nSlider = 0;
	if( hWnd == ::GetDlgItem( m_hWnd, IDC_PAL_R_EDIT ) ) {
		m_RGB.rgbRed = (BYTE)val;
		nSlider = IDC_PAL_R_SLIDER;
	}
	if( hWnd == ::GetDlgItem( m_hWnd, IDC_PAL_G_EDIT ) ) {
		m_RGB.rgbGreen = (BYTE)val;
		nSlider = IDC_PAL_G_SLIDER;
	}
	if( hWnd == ::GetDlgItem( m_hWnd, IDC_PAL_B_EDIT ) ) {
		m_RGB.rgbBlue = (BYTE)val;
		nSlider = IDC_PAL_B_SLIDER;
	}

	if( nSlider ) {
		::SendDlgItemMessage( m_hWnd, nSlider, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)val );

		OnUpdateColor();
	}
}

DLGCMD	CPaletteEdit::OnUndo( DLGCMDPARAM )
{
//	DEBUGOUT( "CPaletteEdit::OnUndo\n" );

	::memcpy( m_Palette, m_PaletteUndo, sizeof(m_Palette) );
	OnChaneSelect();
}

DLGCMD	CPaletteEdit::OnLoad( DLGCMDPARAM )
{
//	DEBUGOUT( "CPaletteEdit::OnLoad\n" );

	CHAR	szFile[ _MAX_PATH ];
	OPENFILENAME	ofn;
	ZEROMEMORY( &ofn, sizeof(ofn) );

	ofn.lpstrInitialDir = CApp::GetModulePath();
	szFile[0] = '\0';

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
//	ofn.lpstrDefExt = ".pal";

	CHAR	szTitle[256];
	CApp::LoadString( IDS_UI_OPENPALETTE, szTitle, sizeof(szTitle) );
	ofn.lpstrTitle = szTitle;

	ofn.lpstrFilter = "Palette File(*.pal)\0*.pal\0All File(*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_READONLY|OFN_HIDEREADONLY|OFN_EXPLORER|OFN_PATHMUSTEXIST;

	if( ::GetOpenFileName( &ofn ) ) {
		::memcpy( m_PaletteUndo, m_Palette, sizeof(m_Palette) );

		DirectDraw.SetPaletteFile( szFile );
		DirectDraw.GetPaletteTable( m_Palette );
		OnChaneSelect();
	}
}

DLGCMD	CPaletteEdit::OnSave( DLGCMDPARAM )
{
//	DEBUGOUT( "CPaletteEdit::OnSave\n" );

	CHAR	szFile[ _MAX_PATH ];
	OPENFILENAME	ofn;
	ZEROMEMORY( &ofn, sizeof(ofn) );

	ofn.lpstrInitialDir = CApp::GetModulePath();
	szFile[0] = '\0';

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);

	ofn.lpstrDefExt = ".pal";

	CHAR	szTitle[256];
	CApp::LoadString( IDS_UI_SAVEPALETTE, szTitle, sizeof(szTitle) );
	ofn.lpstrTitle = szTitle;

	ofn.lpstrFilter = "Palette File(*.pal)\0*.pal\0All File(*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_READONLY|OFN_HIDEREADONLY|OFN_EXPLORER|OFN_PATHMUSTEXIST;
	ofn.Flags |= OFN_OVERWRITEPROMPT;

	if( ::GetSaveFileName( &ofn ) ) {
		FILE*	fp = NULL;

		if( (fp = ::fopen( szFile, "wb" )) ) {
			for( int i = 0; i < 64; i++ ) {
				::fputc( (int)m_Palette[i].rgbRed, fp );
				::fputc( (int)m_Palette[i].rgbGreen, fp );
				::fputc( (int)m_Palette[i].rgbBlue, fp );
			}
			::fclose( fp );
		}
	}
}

void	CPaletteEdit::OnChaneSelect()
{
//	DEBUGOUT( "CPaletteEdit::OnChaneSelect\n" );

	m_RGB = m_Palette[m_PaletteSelect];

	CHAR	szStr[16];
	::wsprintf( szStr, "%02X", m_PaletteSelect );
	::SetDlgItemText( m_hWnd, IDC_PAL_NO, szStr );

	::wsprintf( szStr, "%d", m_RGB.rgbRed );
	::SetDlgItemText( m_hWnd, IDC_PAL_R_EDIT, szStr );
	::wsprintf( szStr, "%d", m_RGB.rgbGreen );
	::SetDlgItemText( m_hWnd, IDC_PAL_G_EDIT, szStr );
	::wsprintf( szStr, "%d", m_RGB.rgbBlue );
	::SetDlgItemText( m_hWnd, IDC_PAL_B_EDIT, szStr );

	OnUpdateColor();

	::InvalidateRect( m_hWnd, NULL, FALSE );
}

void	CPaletteEdit::OnUpdateColor()
{
//	DEBUGOUT( "CPaletteEdit::OnUpdateColor\n" );

	m_Palette[m_PaletteSelect] = m_RGB;

	DirectDraw.SetPaletteTable( m_Palette );

	if( Emu.IsPausing() ) {
		::InvalidateRect( CApp::GetHWnd(), NULL, FALSE );
	}

	::InvalidateRect( m_hWnd, NULL, FALSE );
}
