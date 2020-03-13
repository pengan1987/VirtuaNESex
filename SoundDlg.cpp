//
// サウンドダイアログクラス
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

#include "Wnd.h"
#include "SoundDlg.h"

UINT	CSoundDlg::m_nVolumeID[] = {
	IDC_SND_MASTER_SLIDER,	0,
	IDC_SND_CH0_SLIDER,	1,
	IDC_SND_CH1_SLIDER,	2,
	IDC_SND_CH2_SLIDER,	3,
	IDC_SND_CH3_SLIDER,	4,
	IDC_SND_CH4_SLIDER,	5,
	IDC_SND_EX0_SLIDER,	6,
	IDC_SND_EX1_SLIDER,	7,
	IDC_SND_EX2_SLIDER,	8,
	IDC_SND_EX3_SLIDER,	9,
	IDC_SND_EX4_SLIDER,	10,
	IDC_SND_EX5_SLIDER,	11,
	0, 0
};

DLG_MESSAGE_BEGIN(CSoundDlg)
// メッセージ
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )
// コマンド
DLG_COMMAND_BEGIN()
DLG_ON_COMMAND( IDOK, OnOK )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_ON_COMMAND( IDDEFAULT, OnDefault )
DLG_COMMAND_END()
DLG_MESSAGE_END()

INT	CSoundDlg::DoModal( HWND hWndParent )
{
	return	::DialogBoxParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_CFG_SOUND),
				hWndParent, g_DlgProc, (LPARAM)this );
}

void	CSoundDlg::OnInitialMember()
{
	INT	i;
	// チェックボックス
	BTNCHECK( IDC_SND_ENABLE,		Config.sound.bEnable );
	BTNCHECK( IDC_SND_VOLEFFECT_ENABLE,	!Config.sound.bDisableVolumeEffect );
	BTNCHECK( IDC_SND_EXTSOUND_ENABLE,	Config.sound.bExtraSoundEnable );

	// スライダー
	for( i = 0; m_nVolumeID[i*2+0]; i++ ) {
		::SendDlgItemMessage( m_hWnd, m_nVolumeID[i*2+0], TBM_SETRANGE, TRUE, MAKELONG(0,200) );
		::SendDlgItemMessage( m_hWnd, m_nVolumeID[i*2+0], TBM_SETPOS, TRUE, (WPARAM)200-Config.sound.nVolume[m_nVolumeID[i*2+1]] );
		::SendDlgItemMessage( m_hWnd, m_nVolumeID[i*2+0], TBM_SETTICFREQ, 20, 0 );
		::SendDlgItemMessage( m_hWnd, m_nVolumeID[i*2+0], TBM_SETLINESIZE, 0, 1 );
		::SendDlgItemMessage( m_hWnd, m_nVolumeID[i*2+0], TBM_SETPAGESIZE, 0, 20 );
	}
	// サンプリングレートコンボボックス
	CHAR	szMode[64];
	::SendDlgItemMessage( m_hWnd, IDC_SND_SAMPLERATE_COMBO, CB_RESETCONTENT, 0, 0 );
	for( i = 0; i < 8; i++ ) {
		::wsprintf( szMode, "%d.%03dKHz %2dbits",
				CConfig::SamplingRateTable[i*2+0]/1000,
				CConfig::SamplingRateTable[i*2+0]%1000,
				CConfig::SamplingRateTable[i*2+1] );
		::SendDlgItemMessage( m_hWnd, IDC_SND_SAMPLERATE_COMBO, CB_INSERTSTRING, (WPARAM)i, (LPARAM)szMode );
	}
	// バッファサイズコンボボックス
	::SendDlgItemMessage( m_hWnd, IDC_SND_BUFFERSIZE_COMBO, CB_RESETCONTENT, 0, 0 );
	for( i = 2; i <= 10; i++ ) {
		::wsprintf( szMode, "%d Frame", i );
		::SendDlgItemMessage( m_hWnd, IDC_SND_BUFFERSIZE_COMBO, CB_INSERTSTRING, (WPARAM)i-2, (LPARAM)szMode );
	}

	// フィルタコンボボックス
	::SendDlgItemMessage( m_hWnd, IDC_SND_FILTERTYPE_COMBO, CB_RESETCONTENT, 0, 0 );
	for( i = IDS_SND_FILTER_NONE; i <= IDS_SND_FILTER_TYPE4; i++ ) {
		CApp::LoadString( i, szMode, sizeof(szMode) );
		::SendDlgItemMessage( m_hWnd, IDC_SND_FILTERTYPE_COMBO, CB_INSERTSTRING, (WPARAM)i-IDS_SND_FILTER_NONE, (LPARAM)szMode );
	}

	// 初期選択
	::SendDlgItemMessage( m_hWnd, IDC_SND_SAMPLERATE_COMBO, CB_SETCURSEL, 0, 0 );
	for( i = 0; i < 8; i++ ) {
		if( Config.sound.nRate == CConfig::SamplingRateTable[i*2+0]
		 && Config.sound.nBits == CConfig::SamplingRateTable[i*2+1] ) {
			::SendDlgItemMessage( m_hWnd, IDC_SND_SAMPLERATE_COMBO, CB_SETCURSEL, (WPARAM)i, 0 );
			break;
		}
	}
	::SendDlgItemMessage( m_hWnd, IDC_SND_BUFFERSIZE_COMBO, CB_SETCURSEL, (WPARAM)Config.sound.nBufferSize-2, 0 );

	::SendDlgItemMessage( m_hWnd, IDC_SND_FILTERTYPE_COMBO, CB_SETCURSEL, (WPARAM)Config.sound.nFilterType, 0 );
}

DLGMSG	CSoundDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CSoundDlg::OnInitDialog\n" );
	m_ConfigSave = Config.sound;
	OnInitialMember();

	return	TRUE;
}

DLGCMD	CSoundDlg::OnOK( DLGCMDPARAM )
{
//	DEBUGOUT( "CSoundDlg::OnOK\n" );

	Config.sound.bEnable              = IsBTNCHECK( IDC_SND_ENABLE );
	Config.sound.bDisableVolumeEffect = !IsBTNCHECK( IDC_SND_VOLEFFECT_ENABLE );
	Config.sound.bExtraSoundEnable    = IsBTNCHECK( IDC_SND_EXTSOUND_ENABLE );

	for( INT i = 0; m_nVolumeID[i*2+0]; i++ ) {
		Config.sound.nVolume[m_nVolumeID[i*2+1]] = 200-::SendDlgItemMessage( m_hWnd, m_nVolumeID[i*2+0], TBM_GETPOS, 0, 0 );
	}

	INT sel = ::SendDlgItemMessage( m_hWnd, IDC_SND_SAMPLERATE_COMBO, CB_GETCURSEL, 0, 0 );
	Config.sound.nRate = CConfig::SamplingRateTable[sel*2+0];
	Config.sound.nBits = CConfig::SamplingRateTable[sel*2+1];
	Config.sound.nBufferSize = 2+::SendDlgItemMessage( m_hWnd, IDC_SND_BUFFERSIZE_COMBO, CB_GETCURSEL, 0, 0 );
	Config.sound.nFilterType = ::SendDlgItemMessage( m_hWnd, IDC_SND_FILTERTYPE_COMBO, CB_GETCURSEL, 0, 0 );

	::EndDialog( m_hWnd, IDOK );
}

DLGCMD	CSoundDlg::OnCancel( DLGCMDPARAM )
{
//	DEBUGOUT( "CSoundDlg::OnCancel\n" );
	Config.sound = m_ConfigSave;
	::EndDialog( m_hWnd, IDCANCEL );
}

DLGCMD	CSoundDlg::OnDefault( DLGCMDPARAM )
{
//	DEBUGOUT( "CSoundDlg::OnDefault\n" );
	Config.sound.Default();
	OnInitialMember();
}

