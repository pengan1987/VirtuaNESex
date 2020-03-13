//
// ゲームオプションダイアログクラス
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

#include "EmuThread.h"

#include "Wnd.h"
#include "GameOptionDlg.h"

DLG_MESSAGE_BEGIN(CGameOptionDlg)
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )

DLG_COMMAND_BEGIN()
DLG_ON_COMMAND( IDOK, OnOK )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_ON_COMMAND( IDDEFAULT, OnDefault )
DLG_ON_COMMAND( IDC_OPT_NOTSAVE, OnNotSave )
DLG_COMMAND_END()
DLG_MESSAGE_END()

INT	CGameOptionDlg::DoModal( HWND hWndParent )
{
	return	::DialogBoxParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_CFG_GAMEOPTION),
				hWndParent, g_DlgProc, (LPARAM)this );
}

void	CGameOptionDlg::OnInitialMember()
{
	::SendDlgItemMessage( m_hWnd, IDC_OPT_RENDER_COMBO, CB_RESETCONTENT, 0, 0 );
	::SendDlgItemMessage( m_hWnd, IDC_OPT_IRQTYPE_COMBO, CB_RESETCONTENT, 0, 0 );
	::SendDlgItemMessage( m_hWnd, IDC_OPT_VIDEOMODE_COMBO, CB_RESETCONTENT, 0, 0 );

	INT	i, j;
	CHAR	szStr[256];
	for( i = IDS_OPT_RENDER_POST_ALL, j = 0; i <= IDS_OPT_RENDER_TILE; i++, j++ ) {
		CApp::LoadString( i, szStr, sizeof(szStr) );
		::SendDlgItemMessage( m_hWnd, IDC_OPT_RENDER_COMBO, CB_INSERTSTRING, (WPARAM)j, (LPARAM)szStr );
	}
	for( i = IDS_OPT_IRQ_HSYNC, j = 0; i <= IDS_OPT_IRQ_CLOCK; i++, j++ ) {
		CApp::LoadString( i, szStr, sizeof(szStr) );
		::SendDlgItemMessage( m_hWnd, IDC_OPT_IRQTYPE_COMBO, CB_INSERTSTRING, (WPARAM)j, (LPARAM)szStr );
	}
	for( i = IDS_OPT_VIDEOMODE_NTSC, j = 0; i <= IDS_OPT_VIDEOMODE_PALCHINA; i++, j++ ) {
		CApp::LoadString( i, szStr, sizeof(szStr) );
		::SendDlgItemMessage( m_hWnd, IDC_OPT_VIDEOMODE_COMBO, CB_INSERTSTRING, (WPARAM)j, (LPARAM)szStr );
	}

	::SendDlgItemMessage( m_hWnd, IDC_OPT_RENDER_COMBO, CB_SETCURSEL, (WPARAM)GameOption.nRenderMethod, 0 );
	::SendDlgItemMessage( m_hWnd, IDC_OPT_IRQTYPE_COMBO, CB_SETCURSEL, (WPARAM)GameOption.nIRQtype, 0 );
	::SendDlgItemMessage( m_hWnd, IDC_OPT_VIDEOMODE_COMBO, CB_SETCURSEL, (WPARAM)(GameOption.bVideoMode?1:0), 0 );

	BTNCHECK( IDC_OPT_FRAMEIRQ, !GameOption.bFrameIRQ );
}

DLGMSG	CGameOptionDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CGameOptionDlg::OnInitDialog\n" );
	OnInitialMember();

	return	TRUE;
}

DLGCMD	CGameOptionDlg::OnOK( DLGCMDPARAM )
{
//	DEBUGOUT( "CGameOptionDlg::OnOK\n" );

	GameOption.nRenderMethod = ::SendDlgItemMessage( m_hWnd, IDC_OPT_RENDER_COMBO, CB_GETCURSEL, 0, 0 );
	GameOption.nIRQtype      = ::SendDlgItemMessage( m_hWnd, IDC_OPT_IRQTYPE_COMBO, CB_GETCURSEL, 0, 0 );
	GameOption.bVideoMode    = ::SendDlgItemMessage( m_hWnd, IDC_OPT_VIDEOMODE_COMBO, CB_GETCURSEL, 0, 0 );
	GameOption.bFrameIRQ = !IsBTNCHECK( IDC_OPT_FRAMEIRQ );

	// 設定しる
	Emu.GetNES()->SetRenderMethod( (NES::RENDERMETHOD)GameOption.nRenderMethod );
	Emu.GetNES()->SetIrqType     ( GameOption.nIRQtype );
	Emu.GetNES()->SetFrameIRQmode( GameOption.bFrameIRQ );
	Emu.GetNES()->SetVideoMode   ( GameOption.bVideoMode );

	// セーブ
	if( Emu.GetNES()->rom->GetMapperNo() == 20 ) {
		GameOption.Save( Emu.GetNES()->rom->GetRomName(), Emu.GetNES()->rom->GetGameID(), Emu.GetNES()->rom->GetMakerID() );
	} else if( Emu.GetNES()->rom->GetMapperNo() < 256 ) {
		GameOption.Save( Emu.GetNES()->rom->GetRomName(), Emu.GetNES()->rom->GetPROM_CRC() );
	}
	//

	::EndDialog( m_hWnd, IDOK );
}

DLGCMD	CGameOptionDlg::OnCancel( DLGCMDPARAM )
{
//	DEBUGOUT( "CGameOptionDlg::OnCancel\n" );

	::EndDialog( m_hWnd, IDCANCEL );
}

DLGCMD	CGameOptionDlg::OnDefault( DLGCMDPARAM )
{
//	DEBUGOUT( "CGameOptionDlg::OnCancel\n" );

	GameOption.nRenderMethod = GameOption.defRenderMethod;
	GameOption.nIRQtype      = GameOption.defIRQtype;
	GameOption.bFrameIRQ     = GameOption.defFrameIRQ;
	GameOption.bVideoMode    = GameOption.defVideoMode;

	OnInitialMember();
}

DLGCMD	CGameOptionDlg::OnNotSave( DLGCMDPARAM )
{
//	DEBUGOUT( "CGameOptionDlg::OnNotSave\n" );

	GameOption.nRenderMethod = ::SendDlgItemMessage( m_hWnd, IDC_OPT_RENDER_COMBO, CB_GETCURSEL, 0, 0 );
	GameOption.nIRQtype      = ::SendDlgItemMessage( m_hWnd, IDC_OPT_IRQTYPE_COMBO, CB_GETCURSEL, 0, 0 );
	GameOption.bVideoMode    = ::SendDlgItemMessage( m_hWnd, IDC_OPT_VIDEOMODE_COMBO, CB_GETCURSEL, 0, 0 );
	GameOption.bFrameIRQ = !IsBTNCHECK( IDC_OPT_FRAMEIRQ );

	// 設定しる
	Emu.GetNES()->SetRenderMethod( (NES::RENDERMETHOD)GameOption.nRenderMethod );
	Emu.GetNES()->SetIrqType     ( GameOption.nIRQtype );
	Emu.GetNES()->SetFrameIRQmode( GameOption.bFrameIRQ );
	Emu.GetNES()->SetVideoMode   ( GameOption.bVideoMode );

	::EndDialog( m_hWnd, IDOK );
}

