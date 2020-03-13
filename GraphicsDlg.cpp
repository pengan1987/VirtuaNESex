//
// グラフィックスダイアログクラス
//
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>

#include <string>
using namespace std;

#include "VirtuaNESres.h"
#include "DebugOut.h"
#include "App.h"
#include "Pathlib.h"

#include "Wnd.h"
#include "GraphicsDlg.h"

#include "DirectDraw.h"

DLG_MESSAGE_BEGIN(CGraphicsDlg)
// メッセージ
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )
DLG_ON_MESSAGE( WM_HSCROLL,	OnHScroll )
// NOTIFYメッセージ
DLG_NOTIFY_BEGIN()
DLG_NOTIFY_END()
// コマンド
DLG_COMMAND_BEGIN()
DLG_ON_COMMAND( IDOK, OnOK )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_ON_COMMAND( IDDEFAULT, OnDefault )
DLG_ON_COMMAND( IDC_GRA_PALETTE_BROWSE, OnPaletteBrowse )
DLG_ON_COMMAND_NOTIFY( IDC_GRA_PALETTE_USE, BN_CLICKED, OnPaletteUseClick )
DLG_ON_COMMAND_NOTIFY( IDC_GRA_NOSQUARELIST, BN_CLICKED, OnNoSquareListClick )
DLG_COMMAND_END()
DLG_MESSAGE_END()

INT	CGraphicsDlg::DoModal( HWND hWndParent )
{
	return	::DialogBoxParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_CFG_GRAPHICS),
				hWndParent, g_DlgProc, (LPARAM)this );
}

void	CGraphicsDlg::OnInitialMember()
{
	BTNCHECK( IDC_GRA_ASPECT,	  Config.graphics.bAspect );
	BTNCHECK( IDC_GRA_ALLSPRITE,	  Config.graphics.bAllSprite );
	BTNCHECK( IDC_GRA_ALLLINE,	  Config.graphics.bAllLine );
	BTNCHECK( IDC_GRA_FPS,		  Config.graphics.bFPSDisp );
	BTNCHECK( IDC_GRA_TVFRAME,	  Config.graphics.bTVFrame );
	BTNCHECK( IDC_GRA_SCANLINE,	  Config.graphics.bScanline );
	BTNCHECK( IDC_GRA_SYNCDRAW,	  Config.graphics.bSyncDraw );
	BTNCHECK( IDC_GRA_FITZOOM,	  Config.graphics.bFitZoom );
	BTNCHECK( IDC_GRA_LEFTCLIP,	  Config.graphics.bLeftClip );
	BTNCHECK( IDC_GRA_DOUBLESIZE,	  Config.graphics.bDoubleSize );
	BTNCHECK( IDC_GRA_SYSTEMMEMORY,	  Config.graphics.bSystemMemory );
	BTNCHECK( IDC_GRA_USEHEL,	  Config.graphics.bUseHEL );
	BTNCHECK( IDC_GRA_WAITVSYNC,	  Config.graphics.bWindowVSync );
	BTNCHECK( IDC_GRA_DISKACCESSLAMP, Config.graphics.bDiskAccessLamp );
	BTNCHECK( IDC_GRA_NOSQUARELIST,   Config.graphics.bNoSquareList );
	BTNCHECK( IDC_GRA_SYNCNOSLEEP,    !Config.graphics.bSyncNoSleep );

	// スライダ
	::SendDlgItemMessage( m_hWnd, IDC_GRA_SCANLINE_SLIDER, TBM_SETRANGE, TRUE, MAKELONG(0,100) );
	::SendDlgItemMessage( m_hWnd, IDC_GRA_SCANLINE_SLIDER, TBM_SETPOS, TRUE, (WPARAM)Config.graphics.nScanlineColor );
	::SendDlgItemMessage( m_hWnd, IDC_GRA_SCANLINE_SLIDER, TBM_SETLINESIZE, 0, 1 );
	::SendDlgItemMessage( m_hWnd, IDC_GRA_SCANLINE_SLIDER, TBM_SETPAGESIZE, 0, 10 );

	// スキャンラインカラー値
	CHAR	str[64];
	::wsprintf( str, "%d%%", Config.graphics.nScanlineColor );
	::SetDlgItemText( m_hWnd, IDC_GRA_SCANLINE_COLOR, str );

	// パレット
	BTNCHECK( IDC_GRA_PALETTE_USE,	Config.graphics.bPaletteFile );
	::SetDlgItemText( m_hWnd, IDC_GRA_PALETTE_EDIT, Config.graphics.szPaletteFile );

	// 解像度コンボボックス
	OnUpdateComboBox();
}

void	CGraphicsDlg::OnUpdateComboBox()
{
	// 解像度コンボボックス
	::SendDlgItemMessage( m_hWnd, IDC_GRA_RESOLUTION_COMBO, CB_RESETCONTENT, 0, 0 );
	INT	i, listno, select = 0;
	CHAR	szMode[64];
	for( i = 0, listno = 0; i < DirectDraw.GetDisplayModeNum(); i++ ) {
		DWORD	dwWidth, dwHeight, dwDepth, dwRate;
		DirectDraw.GetDisplayMode( i, dwWidth, dwHeight, dwDepth, dwRate );

		if( !Config.graphics.bNoSquareList && dwWidth*3 != dwHeight*4 )
			continue;

		if( Config.graphics.dwDisplayWidth  == dwWidth
		 && Config.graphics.dwDisplayHeight == dwHeight
		 && Config.graphics.dwDisplayDepth  == dwDepth
		 && Config.graphics.dwDisplayRate   == dwRate ) {
			select = listno;
		}

		::wsprintf( szMode, "%4dx%4d %dbit %dHz", dwWidth, dwHeight, dwDepth, dwRate );
		::SendDlgItemMessage( m_hWnd, IDC_GRA_RESOLUTION_COMBO, CB_INSERTSTRING, (WPARAM)listno, (LPARAM)szMode );
		::SendDlgItemMessage( m_hWnd, IDC_GRA_RESOLUTION_COMBO, CB_SETITEMDATA,  (WPARAM)listno, (LPARAM)i );

		listno++;
	}
	::SendDlgItemMessage( m_hWnd, IDC_GRA_RESOLUTION_COMBO, CB_SETCURSEL, (WPARAM)select, 0 );
}

DLGMSG	CGraphicsDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CGraphicsDlg::OnInitDialog\n" );
	m_ConfigSave = Config.graphics;

	OnInitialMember();

	return	TRUE;
}

DLGMSG	CGraphicsDlg::OnHScroll( DLGMSGPARAM )
{
//	DEBUGOUT( "CGraphicsDlg::OnHScroll\n" );

	INT	pos;
	CHAR	str[16];
	::wsprintf( str, "%d%%", pos = ::SendDlgItemMessage( m_hWnd, IDC_GRA_SCANLINE_SLIDER, TBM_GETPOS, 0, 0 ) );
	::SetDlgItemText( m_hWnd, IDC_GRA_SCANLINE_COLOR, str );

	// スキャンラインカラーの変更
	DirectDraw.SetScanlineColor( pos );
	// パレットテーブルの計算
	DirectDraw.CalcPaletteTable();

	// 色が変わったのでペイントしてな
	::PostMessage( CApp::GetHWnd(), WM_PAINT, 0, 0 );

	return	FALSE;
}

DLGCMD	CGraphicsDlg::OnOK( DLGCMDPARAM )
{
//	DEBUGOUT( "CGraphicsDlg::OnOK\n" );

	Config.graphics.bAspect         = IsBTNCHECK( IDC_GRA_ASPECT );
	Config.graphics.bAllSprite      = IsBTNCHECK( IDC_GRA_ALLSPRITE );
	Config.graphics.bAllLine        = IsBTNCHECK( IDC_GRA_ALLLINE );
	Config.graphics.bFPSDisp        = IsBTNCHECK( IDC_GRA_FPS );
	Config.graphics.bTVFrame        = IsBTNCHECK( IDC_GRA_TVFRAME );
	Config.graphics.bScanline       = IsBTNCHECK( IDC_GRA_SCANLINE );
	Config.graphics.bSyncDraw       = IsBTNCHECK( IDC_GRA_SYNCDRAW );
	Config.graphics.bFitZoom        = IsBTNCHECK( IDC_GRA_FITZOOM );
	Config.graphics.bLeftClip       = IsBTNCHECK( IDC_GRA_LEFTCLIP );
	Config.graphics.bDoubleSize     = IsBTNCHECK( IDC_GRA_DOUBLESIZE );
	Config.graphics.bSystemMemory   = IsBTNCHECK( IDC_GRA_SYSTEMMEMORY );
	Config.graphics.bUseHEL         = IsBTNCHECK( IDC_GRA_USEHEL );
	Config.graphics.bWindowVSync    = IsBTNCHECK( IDC_GRA_WAITVSYNC );
	Config.graphics.bDiskAccessLamp = IsBTNCHECK( IDC_GRA_DISKACCESSLAMP );
	Config.graphics.bNoSquareList   = IsBTNCHECK( IDC_GRA_NOSQUARELIST );
	Config.graphics.bSyncNoSleep    = !IsBTNCHECK( IDC_GRA_SYNCNOSLEEP );

	Config.graphics.nScanlineColor = ::SendDlgItemMessage( m_hWnd, IDC_GRA_SCANLINE_SLIDER, TBM_GETPOS, 0, 0 );

	INT sel = ::SendDlgItemMessage( m_hWnd, IDC_GRA_RESOLUTION_COMBO, CB_GETCURSEL, 0, 0 );
	INT no  = ::SendDlgItemMessage( m_hWnd, IDC_GRA_RESOLUTION_COMBO, CB_GETITEMDATA, (WPARAM)sel, 0 );

	DirectDraw.GetDisplayMode( no, Config.graphics.dwDisplayWidth,
				       Config.graphics.dwDisplayHeight,
				       Config.graphics.dwDisplayDepth,
				       Config.graphics.dwDisplayRate );

	Config.graphics.bPaletteFile  = IsBTNCHECK( IDC_GRA_PALETTE_USE );
	::GetDlgItemText( m_hWnd, IDC_GRA_PALETTE_EDIT, Config.graphics.szPaletteFile, _MAX_PATH );

	::EndDialog( m_hWnd, IDOK );
}

DLGCMD	CGraphicsDlg::OnCancel( DLGCMDPARAM )
{
//	DEBUGOUT( "CGraphicsDlg::OnCancel\n" );
	Config.graphics = m_ConfigSave;
	::EndDialog( m_hWnd, IDCANCEL );
}

DLGCMD	CGraphicsDlg::OnDefault( DLGCMDPARAM )
{
//	DEBUGOUT( "CGraphicsDlg::OnDefault\n" );
	Config.graphics.Default();
	OnInitialMember();
}

DLGCMD	CGraphicsDlg::OnPaletteBrowse( DLGCMDPARAM )
{
//	DEBUGOUT( "CGraphicsDlg::OnPaletteBrowse\n" );

	CHAR	szTemp[ _MAX_PATH ];
	::GetDlgItemText( m_hWnd, IDC_GRA_PALETTE_EDIT, szTemp, _MAX_PATH );

	CHAR	szPath[ _MAX_PATH ];
	CHAR	szFile[ _MAX_PATH ];
	string	temp;
	temp = CPathlib::SplitPath( szTemp );
	::strcpy( szPath, temp.c_str() );
	temp = CPathlib::SplitFnameExt( szTemp );
	::strcpy( szFile, temp.c_str() );

	OPENFILENAME	ofn;
	ZeroMemory( &ofn, sizeof(ofn) );

	if( strlen( szPath ) > 0 ) {
		// 設定済みならばパス／ファイルネームの設定
		ofn.lpstrInitialDir = szPath;
	} else {
		// 未設定ならば実行ファイルと同じパスの設定
		ofn.lpstrInitialDir = CApp::GetModulePath();
		szFile[0] = '\0';
	}
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);

	CHAR	szTitle[256];
	CApp::LoadString( IDS_UI_OPENPALETTE, szTitle, sizeof(szTitle) );
	ofn.lpstrTitle = szTitle;

	ofn.lpstrFilter = "Palette File(*.PAL)\0*.PAL\0All File(*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_READONLY|OFN_HIDEREADONLY|OFN_EXPLORER|OFN_PATHMUSTEXIST;

	if( ::GetOpenFileName( &ofn ) ) {
		BTNCHECK( IDC_GRA_PALETTE_USE, TRUE );
		::SetDlgItemText( m_hWnd, IDC_GRA_PALETTE_EDIT, szFile );

		DirectDraw.SetPaletteFile( szFile );
	}
}

DLGCMD	CGraphicsDlg::OnPaletteUseClick( DLGCMDPARAM )
{
	if( IsBTNCHECK(IDC_GRA_PALETTE_USE) ) {
		CHAR	szTemp[ _MAX_PATH ];
		::GetDlgItemText( m_hWnd, IDC_GRA_PALETTE_EDIT, szTemp, _MAX_PATH );
		DirectDraw.SetPaletteFile( szTemp );
	} else {
		DirectDraw.SetPaletteTable( (LPBYTE)NULL );
	}

	::PostMessage( CApp::GetHWnd(), WM_PAINT, 0, 0 );
}

DLGCMD	CGraphicsDlg::OnNoSquareListClick( DLGCMDPARAM )
{
	Config.graphics.bNoSquareList = IsBTNCHECK( IDC_GRA_NOSQUARELIST );

	// 解像度コンボボックスの更新
	OnUpdateComboBox();
}

