//
// ROM情報ダイアログクラス
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
using namespace std;

#include "VirtuaNESres.h"
#include "DebugOut.h"
#include "App.h"
#include "Pathlib.h"

#include "Wnd.h"
#include "RomInfoDlg.h"

DLG_MESSAGE_BEGIN(CRomInfoDlg)
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )

DLG_COMMAND_BEGIN()
DLG_ON_COMMAND( IDOK, OnOK )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_COMMAND_END()
DLG_MESSAGE_END()

INT	CRomInfoDlg::DoModal( HWND hWndParent )
{
	return	::DialogBoxParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_ROMINFO),
				hWndParent, g_DlgProc, (LPARAM)this );
}

DLGMSG	CRomInfoDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CRomInfoDlg::OnInitDialog\n" );

	::SetDlgItemText( m_hWnd, IDC_ROM_NAME, m_szName );

	CHAR	szStr[64];
	if( m_nMapper < 256 )		::wsprintf( szStr, "%d", m_nMapper );
	else if( m_nMapper == 256 )	::wsprintf( szStr, "NSF" );
	else if( m_nMapper > 256 )	::wsprintf( szStr, "UNIF" );

	::SetDlgItemText( m_hWnd, IDC_ROM_MAPPER, szStr );

	::wsprintf( szStr, "%dKB", m_nPRG*16 );
	::SetDlgItemText( m_hWnd, IDC_ROM_PRG, szStr );
	::wsprintf( szStr, "%dKB", m_nCHR*8 );
	::SetDlgItemText( m_hWnd, IDC_ROM_CHR, szStr );

	::SetDlgItemText( m_hWnd, IDC_ROM_MIRROR,  m_bMirror?"V":"H" );
	::SetDlgItemText( m_hWnd, IDC_ROM_SRAM,    m_bSram?"Yes":"No" );
	::SetDlgItemText( m_hWnd, IDC_ROM_4SCREEN, m_b4Screen?"Yes":"No" );
	::SetDlgItemText( m_hWnd, IDC_ROM_TRAINER, m_bTrainer?"Yes":"No" );
	::SetDlgItemText( m_hWnd, IDC_ROM_VSUNISYSTEM, m_bVSUnisystem?"Yes":"No" );

	::wsprintf( szStr, "%08X", m_dwCRC );
	::SetDlgItemText( m_hWnd, IDC_ROM_CRC, szStr );
	::wsprintf( szStr, "%08X", m_dwCRCALL );
	::SetDlgItemText( m_hWnd, IDC_ROM_CRCALL, szStr );
	::wsprintf( szStr, "%08X", m_dwCRCCHR );
	::SetDlgItemText( m_hWnd, IDC_ROM_CRCCHR, szStr );

	return	TRUE;
}

DLGCMD	CRomInfoDlg::OnOK( DLGCMDPARAM )
{
//	DEBUGOUT( "CRomInfoDlg::OnOK\n" );

	::EndDialog( m_hWnd, IDOK );
}

DLGCMD	CRomInfoDlg::OnCancel( DLGCMDPARAM )
{
//	DEBUGOUT( "CRomInfoDlg::OnCancel\n" );

	::EndDialog( m_hWnd, IDCANCEL );
}

