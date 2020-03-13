//
// メインウインドウクラス
//
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>
#include <shellapi.h>
#include <commdlg.h>
#include <imm.h>

#include <string>
using namespace std;

#include "VirtuaNESres.h"
#include "DebugOut.h"
#include "App.h"
#include "Pathlib.h"
#include "Plugin.h"

#include "Wnd.h"
#include "WndHook.h"
#include "MainFrame.h"
#include "Recent.h"
#include "Config.h"

#include "NetPlay.h"

#include "AboutDlg.h"
#include "EmulatorDlg.h"
#include "GraphicsDlg.h"
#include "SoundDlg.h"
#include "ControllerDlg.h"
#include "ShortcutDlg.h"
#include "FolderDlg.h"
#include "LanguageDlg.h"
#include "MovieDlg.h"
#include "GameOptionDlg.h"
#include "JoyAxisDlg.h"
#include "RomInfoDlg.h"
#include "NetPlayDlg.h"
#include "MovieInfoDlg.h"

#include "DirectDraw.h"
#include "DirectSound.h"
#include "DirectInput.h"

#include "nes.h"
#include "rom.h"
#include "romdb.h"
#include "pad.h"

BOOL	CMainFrame::m_bKeyEnable = FALSE;
volatile BOOL	CMainFrame::m_bKeyThreadExit = FALSE;

DWORD	CMainFrame::m_dwKeyTime = 0;
BYTE	CMainFrame::m_KeyBuf[256+64*8];
BYTE	CMainFrame::m_KeyOld[256+64*8];
BYTE	CMainFrame::m_KeyCnt[256*64*8];

// メッセージマップ
WND_MESSAGE_BEGIN(CMainFrame)
// メッセージ
WND_ON_MESSAGE( WM_CREATE,	OnCreate )
WND_ON_MESSAGE( WM_CLOSE,	OnClose )
WND_ON_MESSAGE( WM_DESTROY,	OnDestroy )
WND_ON_MESSAGE( WM_GETMINMAXINFO, OnGetMinMaxInfo )
WND_ON_MESSAGE( WM_ACTIVATE,	OnActivate )
WND_ON_MESSAGE( WM_ACTIVATEAPP,	OnActivateApp )
WND_ON_MESSAGE( WM_VNS_SHORTCUTENABLE, OnShortCutEnable )
WND_ON_MESSAGE( WM_ENABLE,	OnEnable )
WND_ON_MESSAGE( WM_ENTERMENULOOP, OnEnterMenuLoop )
WND_ON_MESSAGE( WM_EXITMENULOOP, OnExitMenuLoop )
WND_ON_MESSAGE( WM_SETFOCUS,	OnSetFocus )
WND_ON_MESSAGE( WM_KILLFOCUS,	OnKillFocus )
WND_ON_MESSAGE( WM_INITMENU,	OnInitMenu )
WND_ON_MESSAGE( WM_INITMENUPOPUP, OnInitMenuPopup )
WND_ON_MESSAGE( WM_PAINT,	OnPaint )
WND_ON_MESSAGE( WM_DISPLAYCHANGE, OnDisplayChange )
WND_ON_MESSAGE( WM_PALETTECHANGED, OnPaletteChanged )
WND_ON_MESSAGE( WM_QUERYNEWPALETTE, OnQueryNewPalette )
WND_ON_MESSAGE( WM_MENUCHAR,	OnMenuChar )
WND_ON_MESSAGE( WM_KEYDOWN,	OnKeyDown )
WND_ON_MESSAGE( WM_SIZE,	OnSize )
WND_ON_MESSAGE( WM_SYSCOMMAND,	OnSysCommand )
WND_ON_MESSAGE( WM_EXITSIZEMOVE, OnExitSizeMove )

WND_ON_MESSAGE( WM_SETCURSOR,	OnSetCursor )
WND_ON_MESSAGE( WM_TIMER,	OnTimer )

WND_ON_MESSAGE( WM_COPYDATA, OnCopyData )

WND_ON_MESSAGE( WM_VNS_ERRORMSG, OnErrorMessage )

WND_ON_MESSAGE( WM_DROPFILES,	OnDropFiles )
WND_ON_MESSAGE( WM_VNS_COMMANDLINE, OnCommandLine )
WND_ON_MESSAGE( WM_VNS_LAUNCHERCMD, OnLauncherCommand )

WND_ON_MESSAGE( WM_NETPLAY, OnNetPlay )
WND_ON_MESSAGE( WM_NETPLAY_CLOSE, OnNetPlayClose )
WND_ON_MESSAGE( WM_NETPLAY_ERROR, OnNetPlayError )
WND_ON_MESSAGE( WM_VNS_CHATPOPUP, OnNetPlayChatPopup )

// コマンド
WND_COMMAND_BEGIN()
WND_ON_COMMAND( ID_EXIT, OnExit )
WND_ON_COMMAND( ID_HTMLHELP, OnHelp )
WND_ON_COMMAND( ID_ABOUT, OnAbout )
WND_ON_COMMAND( ID_OPEN, OnFileOpen )
WND_ON_COMMAND( ID_CLOSE, OnFileClose )
WND_ON_COMMAND( ID_ROMINFO, OnRomInfo )
WND_ON_COMMAND( ID_WAVERECORD, OnWaveRecord )
WND_ON_COMMAND( ID_LAUNCHER, OnLauncher )
WND_ON_COMMAND_RANGE( ID_MRU_PATH0, ID_MRU_PATH9, OnRecentOpenPath )
WND_ON_COMMAND_RANGE( ID_MRU_FILE0, ID_MRU_FILE9, OnRecentOpen )

WND_ON_COMMAND( ID_NETPLAY_CONNECT, OnNetPlayConnect )
WND_ON_COMMAND( ID_NETPLAY_DISCONNECT, OnNetPlayDisconnect )
WND_ON_COMMAND( ID_NETPLAY_CHAT, OnNetPlayChat )

WND_ON_COMMAND( ID_CFG_EMULATOR, OnEmulatorCfg )
WND_ON_COMMAND( ID_CFG_GRAPHICS, OnGraphicsCfg )
WND_ON_COMMAND( ID_CFG_SOUND, OnSoundCfg )
WND_ON_COMMAND( ID_CFG_CONTROLLER, OnControllerCfg )
WND_ON_COMMAND( ID_CFG_SHORTCUT, OnShortcutCfg )
WND_ON_COMMAND( ID_CFG_FOLDER, OnFolderCfg )
WND_ON_COMMAND( ID_CFG_LANGUAGE, OnLanguageCfg )
WND_ON_COMMAND( ID_CFG_MOVIE, OnMovieCfg )
WND_ON_COMMAND( ID_CFG_GAMEOPTION, OnGameOptionCfg )
WND_ON_COMMAND( ID_CFG_JOYAXIS, OnJoyAxisCfg )
WND_ON_COMMAND( ID_CFG_PALETTE, OnPaletteEditCfg )

WND_ON_COMMAND( ID_SEARCH, OnSearch )
WND_ON_COMMAND( ID_CHEAT, OnCheat )
WND_ON_COMMAND_RANGE( ID_CHEAT_ENABLE, ID_CHEAT_DISABLE, OnCheatCommand )
WND_ON_COMMAND( ID_GENIE, OnGenie )

WND_ON_COMMAND( ID_BARCODEBATTLER, OnDatachBacode )
WND_ON_COMMAND( ID_VSUNISYSTEM_DIPSWITCH, OnDipSwitch )

WND_ON_COMMAND( ID_FULLSCREEN, OnFullScreen )
WND_ON_COMMAND_RANGE( ID_ZOOMx1, ID_ZOOMx4, OnZoom )
WND_ON_COMMAND( ID_HWRESET, OnEmuCommand )
WND_ON_COMMAND( ID_SWRESET, OnEmuCommand )
WND_ON_COMMAND( ID_PAUSE, OnEmuCommand )
WND_ON_COMMAND( ID_THROTTLE, OnEmuCommand )
WND_ON_COMMAND( ID_STATE_UP, OnEmuCommand )
WND_ON_COMMAND( ID_STATE_DOWN, OnEmuCommand )
WND_ON_COMMAND( ID_FPSDISP, OnEmuCommand )
WND_ON_COMMAND( ID_TVASPECT, OnEmuCommand )
WND_ON_COMMAND( ID_TVFRAME, OnEmuCommand )
WND_ON_COMMAND( ID_SCANLINE, OnEmuCommand )
WND_ON_COMMAND( ID_ALLLINE, OnEmuCommand )
WND_ON_COMMAND( ID_ALLSPRITE, OnEmuCommand )
WND_ON_COMMAND( ID_SYNCDRAW, OnEmuCommand )
WND_ON_COMMAND( ID_FITSCREEN, OnEmuCommand )
WND_ON_COMMAND( ID_SNAPSHOT, OnEmuCommand )

WND_ON_COMMAND( ID_LEFTCLIP, OnEmuCommand )
WND_ON_COMMAND( ID_ONEFRAME, OnEmuCommand )

WND_ON_COMMAND( ID_STATE_LOAD, OnStateCommand )
WND_ON_COMMAND( ID_STATE_SAVE, OnStateCommand )

WND_ON_COMMAND( ID_MOVIE_PLAY, OnMovieCommand )
WND_ON_COMMAND( ID_MOVIE_REC, OnMovieCommand )
WND_ON_COMMAND( ID_MOVIE_REC_APPEND, OnMovieCommand )
WND_ON_COMMAND( ID_MOVIE_STOP, OnMovieCommand )
WND_ON_COMMAND( ID_MOVIE_INFO, OnMovieInfo )

WND_ON_COMMAND( ID_TAPE_PLAY, OnTapeCommand )
WND_ON_COMMAND( ID_TAPE_REC, OnTapeCommand )
WND_ON_COMMAND( ID_TAPE_STOP, OnTapeCommand )

WND_ON_COMMAND( ID_FRAMESKIP_AUTO, OnEmuCommand )
WND_ON_COMMAND( ID_FRAMESKIP_UP, OnEmuCommand )
WND_ON_COMMAND( ID_FRAMESKIP_DOWN, OnEmuCommand )

// Viewers
WND_ON_COMMAND( ID_VIEW_PATTERN, OnViewCommand )
WND_ON_COMMAND( ID_VIEW_NAMETABLE, OnViewCommand )
WND_ON_COMMAND( ID_VIEW_PALETTE, OnViewCommand )
WND_ON_COMMAND( ID_VIEW_MEMORY, OnViewCommand )

//
WND_ON_COMMAND_RANGE( ID_FILTER_NONE, ID_FILTER_END, OnEmuCommand )

WND_ON_COMMAND_RANGE( ID_STATE_SLOT0, ID_STATE_SLOT9, OnEmuCommand )
WND_ON_COMMAND_RANGE( ID_DISK_EJECT, ID_DISK_1B, OnEmuCommand )
//WND_ON_COMMAND_RANGE( ID_EXCTR_NONE, ID_EXCTR_END, OnEmuCommand )
WND_ON_COMMAND_RANGE( ID_EXCTR_SUBOR_KEYBOARD, ID_EXCTR_END, OnEmuCommand )
WND_ON_COMMAND_RANGE( ID_MUTE_0, ID_MUTE_F, OnEmuCommand )
WND_ON_COMMAND_RANGE( ID_TURBOFILE_BANK0, ID_TURBOFILE_BANK3, OnEmuCommand )

WND_ON_COMMAND_RANGE( ID_QUICKLOAD_SLOT0, ID_QUICKLOAD_SLOT9, OnStateCommand2 )
WND_ON_COMMAND_RANGE( ID_QUICKSAVE_SLOT0, ID_QUICKSAVE_SLOT9, OnStateCommand2 )

WND_COMMAND_END()
WND_MESSAGE_END()

BOOL	CMainFrame::Create( HWND hWndParent )
{
	// NESオブジェクト
	Nes = NULL;

	// メンバ変数
	m_hMenu = NULL;

	WNDCLASSEX	wcl;
	ZEROMEMORY( &wcl, sizeof(wcl) );
	wcl.cbSize		= sizeof(wcl);
	wcl.lpszClassName	= "VirtuaNESwndclass";
	wcl.lpfnWndProc		= g_WndProc;
//	wcl.style		= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcl.style		= CS_DBLCLKS;
	wcl.cbClsExtra		= wcl.cbWndExtra = 0;
	wcl.hInstance		= CApp::GetInstance();
	wcl.lpszMenuName	= NULL;
	wcl.hIcon		= 
	wcl.hIconSm		= ::LoadIcon( CApp::GetInstance(), MAKEINTRESOURCE(IDI_ICON) );
	wcl.hCursor		= ::LoadCursor( NULL, IDC_ARROW );
	wcl.hbrBackground	= 0;

	if( !RegisterClassEx( &wcl ) ) {
//		DEBUGOUT( "RegisterClassEx faild.\n" );
		return	FALSE;
	}

	HWND hWnd = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			VIRTUANES_WNDCLASS,
			VIRTUANES_CAPTION,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			hWndParent,
			NULL,
			CApp::GetInstance(),
			(LPVOID)this		// This を埋め込む為
		);
	if( !hWnd ) {
//		DEBUGOUT( "CreateWindow faild.\n" );
		return	FALSE;
	}

	// メインウインドウとして登録
	CApp::SetHWnd( hWnd );

	// ステートスロット
	m_nStateSlot = 0;

	return	TRUE;
}

void	CMainFrame::Destroy()
{
	if( m_hWnd && IsWindow(m_hWnd) ) {
		::DestroyWindow( m_hWnd );
		m_hWnd = NULL;
	}
}

BOOL	CMainFrame::PreTranslateMessage( MSG* pMsg )
{
	if( pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST ) {
//		return	TRUE;
	}
	return	FALSE;
}

WNDMSG	CMainFrame::OnCreate( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnCreate\n" );

	DirectDraw.SetSystemMemory( Config.graphics.bSystemMemory );
	DirectDraw.SetUseHEL      ( Config.graphics.bUseHEL );

	if( !DirectDraw.InitialDDraw( m_hWnd ) ) {
		hResult = -1L;
		return	TRUE;
	}
	if( !DirectSound.InitialDSound( m_hWnd ) ) {
		hResult = -1L;
		return	TRUE;
	}
	if( !DirectInput.InitialDInput( m_hWnd, CApp::GetInstance() ) ) {
		hResult = -1L;
		return	TRUE;
	}
	// DirectDrawｵﾌﾟｼｮﾝの設定
	DirectDraw.SetFlipMode     ( Config.graphics.bSyncDraw );
	DirectDraw.SetAspectMode   ( Config.graphics.bAspect );
	DirectDraw.SetAllLineMode  ( Config.graphics.bAllLine );
	DirectDraw.SetMaxZoom      ( Config.graphics.bFitZoom );
	DirectDraw.SetTVFrameMode  ( Config.graphics.bTVFrame );
	DirectDraw.SetScanlineMode ( Config.graphics.bScanline );
	DirectDraw.SetScanlineColor( Config.graphics.nScanlineColor );

	DirectDraw.SetWindowVSyncMode( Config.graphics.bWindowVSync );

	DirectDraw.SetDoubleSize( Config.graphics.bDoubleSize );
	DirectDraw.SetGraphicsFilter( Config.graphics.nGraphicsFilter );

	// DirectDrawサーフェスの構築
	if( !DirectDraw.InitialSurface( FALSE ) ) {
		hResult = -1L;
		return	TRUE;
	}

	// パレットファイルのロード
	if( Config.graphics.bPaletteFile ) {
		string	pathstr = CPathlib::CreatePath( CApp::GetModulePath(), Config.graphics.szPaletteFile );
		DirectDraw.SetPaletteFile( pathstr.c_str() );
	}

	// DirectSoundｵﾌﾟｼｮﾝの設定
	DirectSound.SetSamplingRate( Config.sound.nRate, Config.sound.nBits );
	DirectSound.SetBufferSize( Config.sound.nBufferSize );

	// ExtraSoundFileのロード
	for( INT i = 0; i < ESF_FILE_MAX; i++ ) {
		string	pathstr = CPathlib::CreatePath( CApp::GetModulePath(), Config.extsound.szExtSoundFile[i] );
		DirectSound.LoadEsf( pathstr.c_str(), i );
	}

	// DirectInputｵﾌﾟｼｮﾝの設定
	DirectInput.SetJoyAxisMode( Config.general.JoyAxisSetting );

	// DirectSoundバッファの構築
	if( !DirectSound.InitialBuffer() ) {
		hResult = -1L;
		return	TRUE;
	}

	// パレットテーブルの計算
	DirectDraw.CalcPaletteTable();

	// ネットプレイ
	NetPlay.Initialize( m_hWnd );

	// メニューのロード
	m_hMenu = CApp::LoadMenu( IDR_MENU );
	CApp::SetMenu( m_hMenu );
	::SetMenu( m_hWnd, m_hMenu );
	m_bMenu = TRUE;		// メニュー表示中フラグ

	// メニューにアクセラレータキー名の追加
	OnRebuildMenu();

	// ウインドウ位置の設定
	RECT	rc = Config.general.rcWindowPos;
	if( !((rc.right-rc.left) <= 0 || (rc.bottom-rc.top) <= 0) ) {
		if( (m_bZoomed = Config.general.bWindowZoom) ) {
			CApp::SetCmdShow( SW_SHOWMAXIMIZED );
		}
		// 保存されていたウインドウサイズにする
		m_WindowRect = rc;
		::SetWindowPos( m_hWnd, HWND_NOTOPMOST, rc.left, rc.top,
				rc.right-rc.left, rc.bottom-rc.top, 0 );
	} else {
		m_bZoomed = FALSE;
		// デフォルトサイズを調整する
		OnSetWindowSize();
	}

	// ウインドウスタイルの保存
	m_StyleBackup   = ::GetWindowLong( m_hWnd, GWL_STYLE );
	m_ExStyleBackup = ::GetWindowLong( m_hWnd, GWL_EXSTYLE );

	// IME使用禁止:D
	::ImmAssociateContext( m_hWnd, NULL );

	// Drag&Dropの許可
	::DragAcceptFiles( m_hWnd, TRUE );

	// フラグ類
	m_bActivate = TRUE;
	m_bActivateApp = TRUE;
	m_bForcus = TRUE;
	m_bCursor = TRUE;
	m_bEnable = TRUE;
	m_bKeyEnable = TRUE;
	m_bKeyChecking = FALSE;
	m_LastMovedTime = 0;

	m_uTimerID = ::SetTimer( m_hWnd, 0x0001, 1000, NULL );
//	m_uKeyTimerID = ::SetTimer( m_hWnd, 0x0100, 30, NULL );	// 30ms

	m_dwKeyTime = 0;

	// Key Thread
	::memset( m_KeyBuf, 0x00, sizeof(m_KeyBuf) );
	::memset( m_KeyOld, 0x00, sizeof(m_KeyBuf) );
	::memset( m_KeyCnt, 0x00, sizeof(m_KeyCnt) );

	::_beginthread( KeyThreadProc, 0, NULL );

	ZEROMEMORY( m_KeyBuf, sizeof(m_KeyBuf) );
	ZEROMEMORY( m_KeyCnt, sizeof(m_KeyCnt) );

#if	0
	// For Command Lines
	if( ::strlen( CApp::GetCmdLine() ) > 0 ) {
		LPSTR	pCmd = CApp::GetCmdLine();
		if( pCmd[0] == '"' ) {	// Shell execute!!
			ZEROMEMORY( m_szCommandLine, sizeof(m_szCommandLine) );
			::memcpy( m_szCommandLine, pCmd+1, ::strlen(pCmd)-2 );
			m_szCommandLine[::strlen(m_szCommandLine)] = '\0';
		} else {
			::strcpy( m_szCommandLine, pCmd );
		}
		::PostMessage( m_hWnd, WM_VNS_COMMANDLINE, 0, 0L );
	}
#endif
	return	FALSE;
}

WNDMSG	CMainFrame::OnClose( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnClose\n" );

	// ネットプレイ
	NetPlay.Release();

	// エミュレーション終了
	Emu.Stop();
	DELETEPTR( Nes );

	// キースレッド終了
	m_bKeyThreadExit = TRUE;

	// タイマーの終了
	::KillTimer( m_hWnd, m_uTimerID );
//	::KillTimer( m_hWnd, m_uKeyTimerID );
	m_uTimerID = 0;
	m_uKeyTimerID = 0;

	// ウインドウ破棄
	::DestroyWindow( m_hWnd );

	// サーチダイアログ終了
	m_SearchDlg.Destroy();

	// ランチャー終了
	m_LauncherDlg.Destroy();

	// チャット終了
	m_ChatDlg.Destroy();

	// パターンビュア終了
	m_PatternView.Destroy();
	m_NameTableView.Destroy();
	m_PaletteView.Destroy();
	m_MemoryView.Destroy();

	// バーコード入力ダイアログ終了
	m_DatachBarcodeDlg.Destroy();

	// パレット編集の終了
	m_PaletteEdit.Destroy();

	// ウインドウ位置の保存
	Config.general.bWindowZoom = m_bZoomed;
	Config.general.rcWindowPos = m_WindowRect;

	::DragAcceptFiles( m_hWnd, FALSE );

	return	FALSE;
}

WNDMSG	CMainFrame::OnDestroy( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnDestroy\n" );

	::PostQuitMessage( 0 );
	return	FALSE;
}

WNDMSG	CMainFrame::OnGetMinMaxInfo( WNDMSGPARAM )
{
	MINMAXINFO* lpMMI = (MINMAXINFO*)lParam;

	// 最小サイズだけ
	lpMMI->ptMinTrackSize.x = 128;
	lpMMI->ptMinTrackSize.y = 128;
//	lpMMI->ptMaxTrackSize.x = 65535;
//	lpMMI->ptMaxTrackSize.y = 65535;

	return	TRUE;
}

WNDMSG	CMainFrame::OnActivate( WNDMSGPARAM )
{
//DEBUGOUT( "WA_ACTIVATE: Minimized:%s  Prev:%08X  This:%08X\n", HIWORD(wParam)?"TRUE":"FALSE", lParam, m_hWnd );

#if	1
	if( HIWORD(wParam) || (LOWORD(wParam) == WA_INACTIVE && !lParam) ) {
		if( m_bActivate ) {
//		DEBUGOUT( "Inactivate.\n" );
			m_bActivate = FALSE;
			// バックグラウンド動作ONかネットプレイ中はポーズしない
			if( !(Config.emulator.bBackground || NetPlay.IsConnect()) ) {
				Emu.Pause();
			}
		}
		// DirectInput無効化
		DirectInput.Unacquire();
	} else if( !lParam ) {
//		DEBUGOUT( "Activate.\n" );
		m_bActivate = TRUE;
		// バックグラウンド動作ONかネットプレイ中はレジュームしない
		if( !(Config.emulator.bBackground || NetPlay.IsConnect()) ) {
			Emu.Resume();
		}
		// DirectInput有効
		DirectInput.Acquire();
	}
#else
	if( LOWORD(wParam) == WA_INACTIVE ) {
//		DEBUGOUT( "WM_ACTIVATE: WA_INACTIVE\n" );
////		DEBUGOUT( "CMainFrame::OnActivate:Inactive\n" );
		m_bActivate = FALSE;
////		m_bActivate = TRUE;
	} else if( LOWORD(wParam) == WA_ACTIVE ) {
//		DEBUGOUT( "WM_ACTIVATE: WA_ACTIVE\n" );
////		DEBUGOUT( "CMainFrame::OnActivate:Active\n" );
		m_bActivate = TRUE;
	} else if( LOWORD(wParam) == WA_CLICKACTIVE ) {
//		DEBUGOUT( "WM_ACTIVATE: WA_CLICKACTIVE\n" );
////		DEBUGOUT( "CMainFrame::OnActivate:ClickActive\n" );
		m_bActivate = TRUE;
	}
#endif
	return	FALSE;
}

WNDMSG	CMainFrame::OnActivateApp( WNDMSGPARAM )
{
//DEBUGOUT( "wParam:%08X  lParam:%08X\n", wParam, lParam );

#if	0
	if( (BOOL)wParam ) {
//		DEBUGOUT( "CMainFrame::OnActivateApp:Active\n" );
		m_bActivateApp = TRUE;
		// バックグラウンド動作ONかネットプレイ中はレジュームしない
		if( !(Config.emulator.bBackground || NetPlay.IsConnect()) ) {
			Emu.Resume();
		}
		DirectInput.Acquire();
	} else {
//		DEBUGOUT( "CMainFrame::OnActivateApp:Inactive\n" );
		if( m_bActivateApp ) {
			m_bActivateApp = FALSE;
			// バックグラウンド動作ONかネットプレイ中はポーズしない
			if( !(Config.emulator.bBackground || NetPlay.IsConnect()) ) {
				Emu.Pause();
			}
		}
		DirectInput.Unacquire();
	}
#else
	if( (BOOL)wParam ) {
//		DEBUGOUT( "CMainFrame::OnActivateApp:Active\n" );
		m_bActivateApp = TRUE;
		if( !m_bActivate ) {
			m_bActivate = TRUE;
			// バックグラウンド動作ONかネットプレイ中はレジュームしない
			if( !(Config.emulator.bBackground || NetPlay.IsConnect()) ) {
				Emu.Resume();
			}
		}
		DirectInput.Acquire();
	} else {
//		DEBUGOUT( "CMainFrame::OnActivateApp:Inactive\n" );
		m_bActivateApp = FALSE;
		if( m_bActivate ) {
			m_bActivate = FALSE;
			// バックグラウンド動作ONかネットプレイ中はポーズしない
			if( !(Config.emulator.bBackground || NetPlay.IsConnect()) ) {
				Emu.Pause();
			}
		}
		DirectInput.Unacquire();
	}
#endif
	return	FALSE;
}

WNDMSG	CMainFrame::OnShortCutEnable( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnShortCutmode Req=%s\n", (BOOL)wParam?"TRUE":"FALSE" );

	if( (BOOL)wParam ) {
		if( m_bEnable ) {
			m_bKeyEnable = TRUE;
//DEBUGOUT( "CMainFrame::OnShortCutmode TRUE\n" );
		}
	} else {
		m_bKeyEnable = FALSE;
//DEBUGOUT( "CMainFrame::OnShortCutmode FALSE\n" );
	}
//	m_bKeyEnable = (BOOL)wParam;

#if	0
	if( (BOOL)wParam ) {
		if( --m_nKeyDisableCount < 0 ) {
			m_bKeyEnable = TRUE;
		}
	} else {
		m_nKeyDisableCount++;
		m_bKeyEnable = FALSE;
	}
#endif
	return	FALSE;
}

WNDMSG	CMainFrame::OnEnable( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnEnable = %s\n", (BOOL)wParam?"TRUE":"FALSE" );

	if( (BOOL)wParam )
		Emu.Resume();
	else
		Emu.Pause();

	m_bEnable    = (BOOL)wParam;
	m_bKeyEnable = (BOOL)wParam;

	return	FALSE;
}

WNDMSG	CMainFrame::OnEnterMenuLoop( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnEnterMenuLoop\n" );
	m_bKeyEnable = FALSE;
	Emu.Pause();
	return	FALSE;
}

WNDMSG	CMainFrame::OnExitMenuLoop( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnExitMenuLoop\n" );
	m_bKeyEnable = TRUE;
	Emu.Resume();
	return	FALSE;
}

WNDMSG	CMainFrame::OnSetFocus( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnSetFocus\n" );
	m_bForcus = TRUE;
	if( Config.general.bScreenMode ) {
		if( Emu.IsRunning() ) {
			if( !m_bMenu ) {
				Emu.EventParam( CEmuThread::EV_FULLSCREEN_GDI, FALSE );
			}
		}
	}
	return	FALSE;
}

WNDMSG	CMainFrame::OnKillFocus( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnKillFocus\n" );
	m_bForcus = FALSE;
	return	FALSE;
}

WNDMSG	CMainFrame::OnInitMenu( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnInitMenu\n" );
	CRecent::UpdateMenu( CApp::GetMenu() );
	DrawMenuBar( m_hWnd );
	return	FALSE;
}

WNDMSG	CMainFrame::OnInitMenuPopup( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnInitMenuPopup\n" );
	// System menu
	if( (BOOL)HIWORD(lParam) )
		return	FALSE;

//	DEBUGOUT( "SubMenu=%08X uPos=%d bSys=%d\n", wParam, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam) );

	HMENU	hMenu = (HMENU)wParam;

	INT	MenuCount = ::GetMenuItemCount( hMenu );
	for( INT i = 0; i < MenuCount; i++ ) {
		OnUpdateMenu( hMenu, ::GetMenuItemID( hMenu, i ) );
	}

	return	FALSE;
}

WNDMSG	CMainFrame::OnPaint( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnPaint\n" );
	HDC	hDC;
	PAINTSTRUCT	ps;
	hDC = ::BeginPaint( m_hWnd, &ps );
	if( !Emu.IsRunning() ) {
		RECT	rc;
		::GetClientRect( m_hWnd, &rc );
		::SetBkColor( hDC, 0x00000000 );
		::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL );
	} else if( Emu.IsPausing() ) {
		DirectDraw.OnScreenDraw();
	}
	::EndPaint( m_hWnd, &ps );

	return	TRUE;
}

WNDMSG	CMainFrame::OnDisplayChange( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnDisplayChange\n" );

	// フルスクリーンならば自分でチェンジ中もしくはALT+TAB切り替え中
	if( !Config.general.bScreenMode ) {
		// 危険なのでスレッドをポーズ
		Emu.Pause();
		DirectDraw.OnChangeDisplayMode();

		// 再描画
		::InvalidateRect( m_hWnd, NULL, TRUE );

		// スレッドをレジューム
		Emu.Resume();
	}

	return	FALSE;
}

WNDMSG	CMainFrame::OnPaletteChanged( WNDMSGPARAM )
{
	if( (HWND)wParam == m_hWnd )
		return	FALSE;

	DirectDraw.RealizePalette();
	return	FALSE;
}

WNDMSG	CMainFrame::OnQueryNewPalette( WNDMSGPARAM )
{
	DirectDraw.RealizePalette();
	return	TRUE;
}

WNDMSG	CMainFrame::OnMenuChar( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnMenuChar\n" );
	// キンキンうるさいから閉じる…と嘘をつく
	hResult = MAKELONG(0,MNC_CLOSE);
	return	TRUE;
}

WNDMSG	CMainFrame::OnKeyDown( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnKeyDown\n" );

	if( Config.general.bScreenMode ) {
		if( wParam == VK_ESCAPE && (lParam & (1<<30)) == 0 ) {
			OnChangeMenu( !m_bMenu );
			return	TRUE;
		}
	}

	return	FALSE;
}

WNDMSG	CMainFrame::OnSize( WNDMSGPARAM )
{
	// 最大化表示等
	// WM_SYSCOMMANDでも良いのだが，キャプションをダブルクリックしたときに来ないので...
	switch( wParam ) {
		case	SIZE_MAXIMIZED:
			m_bZoomed = TRUE;
			{
			WINDOWPLACEMENT wpl;
			::GetWindowPlacement( m_hWnd, &wpl );
			m_WindowRect = wpl.rcNormalPosition;	// 元の位置をコピー
			}
			break;
		case	SIZE_RESTORED:
			m_bZoomed = FALSE;
			break;
		default:
			break;
	}

	return	FALSE;
}

WNDMSG	CMainFrame::OnSysCommand( WNDMSGPARAM )
{
	// エミュレーション中のみ
	if( Emu.IsRunning() ) {
		// スクリーンセーバーの起動の抑止
		if( wParam == SC_SCREENSAVE ) {
//			DEBUGOUT( "CMainFrame::OnSysCommand  SC_SCREENSAVE\n" );
			hResult = 1L;
			return	TRUE;
		}
		// ディスプレイ電源オフの抑止
		if( wParam == SC_MONITORPOWER ) {
//			DEBUGOUT( "CMainFrame::OnSysCommand  SC_MONITORPOWER\n" );
			hResult = 1L;
			return	TRUE;
		}
	}

	return	FALSE;
}

WNDMSG	CMainFrame::OnExitSizeMove( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnExitSizeMove\n" );

	if( !Config.general.bScreenMode ) {
		::GetWindowRect( m_hWnd, &m_WindowRect );
	}

	return	FALSE;
}

WNDMSG	CMainFrame::OnSetCursor( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnSetCursor\n" );
	if( LOWORD(lParam) == HTCLIENT ) {
		if( Emu.IsRunning() ) {
//			if( !Emu.IsPausing() && !Emu.IsEmuPausing() ) {
			if( !Emu.IsPausing() ) {
				if( DirectDraw.GetZapperMode() ) {
					m_bCursor = TRUE;
					::SetCursor( NULL );
					hResult = 1L;
					return	TRUE;
				} else {
					m_bCursor = FALSE;
					m_LastMovedTime = ::timeGetTime();
				}
			} else {
				m_bCursor = TRUE;
			}
		} else {
			m_bCursor = TRUE;
		}
	} else {
		m_bCursor = TRUE;
	}

	return	FALSE;
}

WNDMSG	CMainFrame::OnTimer( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnTimer\n" );
//	DEBUGOUT( "CMainFrame::OnTimer bCursor=%s bForcus=%s\n", m_bCursor?"TRUE":"FALSE", m_bForcus?"TRUE":"FALSE" );

	if( wParam == 0x0001 ) {
	// マウスカーソル用タイマー
		RECT	rc;
		POINT	pt;
		::GetWindowRect( m_hWnd, &rc );
		::GetCursorPos( &pt );

		if( !m_bCursor && m_bForcus && pt.x >= rc.left && pt.x <= rc.right && pt.y >= rc.top && pt.y <= rc.bottom ) {
			if( Emu.IsRunning() && !Emu.IsPausing() ) {
				if( (::timeGetTime()-m_LastMovedTime) > 1500 ) {
					::SetCursor( NULL );
				}
			}
		}
	} else if( wParam == 0x0100 ) {
	// キーチェック用タイマー
		// アクティブウインドウでない時やダイアログ等が出ている時はキャンセル
////		if( m_bActivate && m_bKeyEnable )
////		if( m_bKeyEnable )
//			OnKeyControl();
	}

	return	FALSE;
}

WNDMSG	CMainFrame::OnCopyData( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnCopyData\n" );

	COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)lParam;

	OnEmulationStart( (LPSTR)pcds->lpData, FALSE );

	return	TRUE;
}

WNDMSG	CMainFrame::OnErrorMessage( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnErrorMessage\n" );

	// チャット終了
	m_ChatDlg.Destroy();

	// サーチダイアログ終了
	m_SearchDlg.Destroy();

	// パターンビュア終了
	m_PatternView.Destroy();
	m_NameTableView.Destroy();
	m_PaletteView.Destroy();
	m_MemoryView.Destroy();

	// バーコード入力ダイアログ終了
	m_DatachBarcodeDlg.Destroy();

	// NetPlay切断
	NetPlay.Disconnect();

	// エミュレーション終了
	Emu.Stop();
	DELETEPTR( Nes );

	// キャプションを変える
	::SetWindowText( m_hWnd, VIRTUANES_CAPTION );

	if( Config.general.bScreenMode ) {
		OnFullScreenGDI( TRUE );
	}

	::MessageBox( m_hWnd, (LPCTSTR)lParam, "ERROR", MB_ICONERROR|MB_OK );
	::InvalidateRect( m_hWnd, NULL, TRUE );

	if( Config.general.bScreenMode ) {
		OnChangeMenu( TRUE );
	}

	return	TRUE;
}

WNDCMD	CMainFrame::OnExit( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnExit\n" );

	::PostMessage( m_hWnd, WM_CLOSE, 0, 0 );
}

WNDCMD	CMainFrame::OnHelp( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnHelp\n" );

	string	sHelp = CPathlib::MakePath( CApp::GetModulePath(), "virtuanes.chm" );

	// 面倒なんでShellExecuteで代用
	::ShellExecute( HWND_DESKTOP, "open", sHelp.c_str(), NULL, NULL, SW_SHOWNORMAL );
//	::HtmlHelp( m_hWnd, "virtuanes.chm", HH_DISPLAY_TOPIC, NULL ); 
}

WNDCMD	CMainFrame::OnAbout( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnAbout\n" );
	CAboutDlg dlg;
	dlg.DoModal( m_hWnd );
}

WNDCMD	CMainFrame::OnFileOpen( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnFileOpen\n" );

	if( Emu.IsRunning() ) {
		Emu.Pause();
	}

	if( Config.general.bScreenMode ) {
		if( Emu.IsRunning() ) {
			OnFullScreenGDI( TRUE );
		}
	}

	OPENFILENAME	ofn;
	CHAR	szFile[_MAX_PATH];

	ZEROMEMORY( szFile, sizeof(szFile) );
	ZEROMEMORY( &ofn, sizeof(ofn) );

	string	pathstr;
	if( Config.path.bRomPath ) {
		pathstr = CPathlib::CreatePath( CApp::GetModulePath(), Config.path.szRomPath );
	} else {
		pathstr = CApp::GetModulePath();
	}

	CHAR	szTitle[256];
	CApp::LoadString( IDS_UI_OPENROM, szTitle, sizeof(szTitle) );
	ofn.lpstrTitle = szTitle;

	ofn.lStructSize     = sizeof(ofn);
	ofn.hwndOwner       = m_hWnd;
	ofn.lpstrFile       = szFile;
	ofn.nMaxFile        = sizeof(szFile);
	ofn.lpstrFilter     = "All Support Types\0*.nes;*.fds;*.nsf;*.lzh;*.zip;*.rar;*.cab\0"
			      "NES ROM (*.nes)\0*.nes\0Disk Image (*.fds)\0*.fds\0"
			      "NES Music File (*.nsf)\0*.nsf\0Archive File\0*.lzh;*.zip;*.rar;*.cab\0";
	ofn.nFilterIndex    = 1;
	ofn.Flags           = OFN_READONLY|OFN_HIDEREADONLY|OFN_EXPLORER|OFN_PATHMUSTEXIST;
	ofn.lpstrInitialDir = pathstr.c_str();

	if( ::GetOpenFileName( &ofn ) ) {
		OnEmulationStart( szFile, FALSE );
		if( Config.general.bScreenMode && Emu.IsRunning() ) {
			OnChangeMenu( FALSE );
		}
	} else {
		if( Emu.IsRunning() ) {
			OnFullScreenGDI( FALSE );
		}
	}
	while( Emu.IsPausing() ) {
		Emu.Resume();
	}
}

WNDCMD	CMainFrame::OnFileClose( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnFileClose\n" );

	// チャット終了
	m_ChatDlg.Destroy();

	// サーチダイアログ終了
	m_SearchDlg.Destroy();

	// パターンビュア終了
	m_PatternView.Destroy();
	m_NameTableView.Destroy();
	m_PaletteView.Destroy();
	m_MemoryView.Destroy();

	// バーコード入力ダイアログ終了
	m_DatachBarcodeDlg.Destroy();

	if( Emu.IsRunning() ) {
		Emu.Stop();
		DELETEPTR( Nes );

		// キャプションを変える
		::SetWindowText( m_hWnd, VIRTUANES_CAPTION );

		// 再描画
		::InvalidateRect( m_hWnd, NULL, TRUE );
	}

	if( Config.general.bScreenMode ) {
		OnChangeMenu( TRUE );
	}
}

WNDMSG	CMainFrame::OnCommandLine( WNDMSGPARAM )
{
	OnEmulationStart( (LPSTR)lParam, FALSE );

	return	TRUE;
}

WNDMSG	CMainFrame::OnLauncherCommand( WNDMSGPARAM )
{
	if( Config.general.bScreenMode ) {
		if( m_LauncherDlg.m_hWnd ) {
			::SetWindowPos( m_LauncherDlg.m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
		}
	}
	::SetForegroundWindow( m_hWnd );

	OnEmulationStart( (LPSTR)lParam, FALSE );

	return	TRUE;
}

WNDMSG	CMainFrame::OnNetPlay( WNDMSGPARAM )
{
	DEBUGOUT( "CMainFrame::OnNetPlay\n" );
	hResult = NetPlay.WndProc( hWnd, wParam, lParam );
	return	TRUE;
}

WNDMSG	CMainFrame::OnNetPlayClose( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnNetPlayClose\n" );
	NetPlay.SetMsgWnd( NULL );
	NetPlay.Disconnect();

	LPCSTR	szErrStr = CApp::GetErrorString( IDS_ERROR_NETWORKDISCONNECT );
	::PostMessage( m_hWnd, WM_VNS_ERRORMSG, 0, (LPARAM)szErrStr );
	return	TRUE;
}

WNDMSG	CMainFrame::OnNetPlayError( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnNetPlayError\n" );
	NetPlay.SetMsgWnd( NULL );
	NetPlay.Disconnect();

	LPCSTR	szErrStr = CApp::GetErrorString( IDS_ERROR_NETWORKERROR );
	::PostMessage( m_hWnd, WM_VNS_ERRORMSG, 0, (LPARAM)szErrStr );
	return	TRUE;
}

WNDMSG	CMainFrame::OnNetPlayChatPopup( WNDMSGPARAM )
{
//	DEBUGOUT( "CMainFrame::OnNetPlayChatPopup\n" );
	DEBUGOUT( "CMainFrame::OnNetPlayChatPopup\n" );
	if( !NetPlay.IsConnect() )
		return	TRUE;

	if( Config.general.bScreenMode ) {
		if( Emu.IsRunning() ) {
			Emu.EventParam( CEmuThread::EV_FULLSCREEN_GDI, TRUE );
		} else {
			DirectDraw.SetFullScreenGDI( TRUE );
		}
		// ダイアログが画面外ならば中央に移動する:)
		RECT	rcParent, rc;
		::GetWindowRect( m_hWnd, &rcParent );
		::GetWindowRect( m_ChatDlg.m_hWnd, &rc );

		if( (rc.right < rcParent.left) || (rc.left > rcParent.right)
		 || (rc.bottom < rcParent.top) || (rc.top > rcParent.bottom) ) {
			INT x = rcParent.left+(rcParent.right-rcParent.left)/2-(rc.right-rc.left)/2;
			INT y = rcParent.top +(rcParent.bottom-rcParent.top)/2-(rc.bottom-rc.top)/2;
			::SetWindowPos( m_ChatDlg.m_hWnd, NULL, x, y, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );
		} else {
			::SetWindowPos( m_ChatDlg.m_hWnd, NULL, 0, 0, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );
		}
		::SetWindowPos( m_ChatDlg.m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE );
	} else {
		::SetWindowPos( m_ChatDlg.m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE );
	}

	return	TRUE;
}

WNDCMD	CMainFrame::OnRecentOpen( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnRecentOpen ID=%d\n", uID-ID_MRU_FILE0 );
//	DEBUGOUT( "Fname: \"%s\"\n", CRecent::GetName( (INT)uID-ID_MRU_FILE0 ) );

	OnEmulationStart( CRecent::GetName( (INT)uID-ID_MRU_FILE0 ), FALSE );
}

WNDCMD	CMainFrame::OnRecentOpenPath( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnRecentOpenPath ID=%d\n", uID-ID_MRU_FILE0 );
//	DEBUGOUT( "Fname: \"%s\"\n", CRecent::GetPath( (INT)uID-ID_MRU_FILE0 ) );

	if( Emu.IsRunning() ) {
		Emu.Pause();
	}

	if( Config.general.bScreenMode ) {
		if( Emu.IsRunning() ) {
			OnFullScreenGDI( TRUE );
		}
	}

	OPENFILENAME	ofn;
	CHAR	szFile[_MAX_PATH];

	ZEROMEMORY( szFile, sizeof(szFile) );
	ZEROMEMORY( &ofn, sizeof(ofn) );

	CHAR	szTitle[256];
	CApp::LoadString( IDS_UI_OPENROM, szTitle, sizeof(szTitle) );
	ofn.lpstrTitle = szTitle;

	ofn.lStructSize     = sizeof(ofn);
	ofn.hwndOwner       = m_hWnd;
	ofn.lpstrFile       = szFile;
	ofn.nMaxFile        = sizeof(szFile);
	ofn.lpstrFilter     = "All Support Types\0*.nes;*.fds;*.nsf;*.lzh;*.zip;*.rar;*.cab\0"
			      "NES ROM (*.nes)\0*.nes\0Disk Image (*.fds)\0*.fds\0"
			      "NES Music File (*.nsf)\0*.nsf\0Archive File\0*.lzh;*.zip;*.rar;*.cab\0";
	ofn.nFilterIndex    = 1;
	ofn.Flags           = OFN_READONLY|OFN_HIDEREADONLY|OFN_EXPLORER|OFN_PATHMUSTEXIST;
	ofn.lpstrInitialDir = CRecent::GetPath( (INT)uID-ID_MRU_PATH0 );


	if( ::GetOpenFileName( &ofn ) ) {
		OnEmulationStart( szFile, FALSE );
	} else {
		if( Emu.IsRunning() ) {
			OnFullScreenGDI( FALSE );
		}
	}
	while( Emu.IsPausing() ) {
		Emu.Resume();
	}
}

WNDMSG	CMainFrame::OnDropFiles( WNDMSGPARAM )
{
	DEBUGOUT( "CMainFrame::OnDropFiles\n" );
	::SetForegroundWindow( m_hWnd );

	CHAR szFile[_MAX_PATH];
	::DragQueryFile( (HDROP)wParam, 0, szFile, _MAX_PATH );
	::DragFinish( (HDROP)wParam );

	INT	ret;
	if( (ret = ROM::IsRomFile( szFile )) >= 0 ) {
DEBUGOUT( "ROMファイル CHK=%d\n", ret );
		if( ret == IDS_ERROR_ILLEGALHEADER ) {
			if( ::MessageBox( m_hWnd, CApp::GetErrorString(ret), "VirtuaNES", MB_ICONWARNING|MB_YESNO ) != IDYES )
				return	TRUE;

			OnEmulationStart( szFile, TRUE );
			return	TRUE;
		} else if( ret == 0 ) {
			OnEmulationStart( szFile, TRUE );
			return	TRUE;
		}
	}

	if( Emu.IsRunning() && !NetPlay.IsConnect() ) {
		if( (ret = NES::IsStateFile( szFile, Nes->rom )) >= 0 ) {
DEBUGOUT( "ステートファイル CHK=%d\n", ret );
			if( ret == IDS_ERROR_ILLEGALSTATECRC ) {
				if( Config.emulator.bCrcCheck ) {
					if( ::MessageBox( m_hWnd, CApp::GetErrorString(ret), "VirtuaNES", MB_ICONWARNING|MB_YESNO ) != IDYES )
						return	TRUE;
				}
			}
			Emu.EventParam2( CEmuThread::EV_STATE_LOAD, (INT)szFile, -1 );
		} else
		if( (ret = NES::IsMovieFile( szFile, Nes->rom )) >= 0 ) {
DEBUGOUT( "ムービーファイル CHK=%d\n", ret );
			if( ret == IDS_ERROR_ILLEGALMOVIEOLD ) {
				::MessageBox( m_hWnd, CApp::GetErrorString(ret), "VirtuaNES", MB_ICONHAND|MB_OK );
				return	TRUE;
			} else
			if( ret == IDS_ERROR_ILLEGALMOVIEVER ) {
				if( Config.emulator.bCrcCheck ) {
					if( ::MessageBox( m_hWnd, CApp::GetErrorString(ret), "VirtuaNES", MB_ICONWARNING|MB_YESNO ) != IDYES )
						return	TRUE;
				}
			} else
			if( ret == IDS_ERROR_ILLEGALMOVIECRC ) {
				if( Config.emulator.bCrcCheck ) {
					if( ::MessageBox( m_hWnd, CApp::GetErrorString(ret), "VirtuaNES", MB_ICONWARNING|MB_YESNO ) != IDYES )
						return	TRUE;
				}
			}
			Emu.EventParam( CEmuThread::EV_MOVIE_PLAY, (INT)szFile );
		}
	}

	return	TRUE;
}

void	CMainFrame::OnEmulationStart( LPCSTR szFile, BOOL bChecked )
{
	// サーチダイアログ終了
	m_SearchDlg.Destroy();

	// バーコード入力ダイアログ終了
	m_DatachBarcodeDlg.Destroy();

	// パターンビュア終了
//	m_PatternView.Destroy();
//	m_NameTableView.Destroy();
//	m_PaletteView.Destroy();
//	m_MemoryView.Destroy();

	// エミュレーション終了
//	Emu.Stop();
//	DELETEPTR( Nes );

	try {
		if( !bChecked ) {
			INT	ret;
			if( (ret = ROM::IsRomFile( szFile )) != 0 ) {
				// 致命的なエラー
				if( ret == IDS_ERROR_OPEN ) {
					// xxx ファイルを開けません
					::wsprintf( szErrorString, CApp::GetErrorString(ret), szFile );
					throw	szErrorString;
				}
				if( ret == IDS_ERROR_READ ) {
					throw	CApp::GetErrorString(ret);
				}
				if( ret == IDS_ERROR_UNSUPPORTFORMAT ) {
					throw	CApp::GetErrorString(ret);
				}

				// YES/NOチェック
				if( ret == IDS_ERROR_ILLEGALHEADER ) {
					if( ::MessageBox( m_hWnd, CApp::GetErrorString(ret), "VirtuaNES", MB_ICONWARNING|MB_YESNO ) != IDYES )
						return;
				}
//			} else {
//				throw	CApp::GetErrorString( IDS_ERROR_UNSUPPORTFORMAT );
			}
		}

		// エミュレーション終了
		Emu.Stop();
		DELETEPTR( Nes );

		if( (Nes = new NES(szFile)) ) {
			CRecent::Add( szFile );

			if( Config.general.bScreenMode ) {
				DirectDraw.SetFullScreenGDI( FALSE );
				OnChangeMenu( FALSE );
			} else {
				if( Config.emulator.bLoadFullscreen ) {
					::PostMessage( m_hWnd, WM_COMMAND, (WPARAM)ID_FULLSCREEN, (LPARAM)0 );
				}
			}

			// キャプションを変える
			{
			string	str = VIRTUANES_CAPTION;
				str = str + " - " + Nes->rom->GetRomName();
				::SetWindowText( m_hWnd, str.c_str() );
			}

			// エミュレーションスレッドスタート
			Emu.Start( m_hWnd, Nes );
		} else {
			// xxx ファイルを開けません
			LPCSTR	szErrStr = CApp::GetErrorString( IDS_ERROR_OPEN );
			sprintf( szErrorString, szErrStr, szFile );
			throw	szErrorString;
		}
	} catch( CHAR* str ) {
		::strcpy( szErrorString, str );
		PostMessage( m_hWnd, WM_VNS_ERRORMSG, 0, (LPARAM)szErrorString );
#ifndef	_DEBUG
	} catch(...) {
		// 不明なエラーが発生しました
		::strcpy( szErrorString, CApp::GetErrorString( IDS_ERROR_UNKNOWN ) );
		PostMessage( m_hWnd, WM_VNS_ERRORMSG, 0, (LPARAM)szErrorString );
#endif
	}
}

WNDCMD	CMainFrame::OnRomInfo( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnRomInfo\n" );

	if( !Emu.IsRunning() || !Nes )
		return;

	CRomInfoDlg dlg;

	// メンバの設定
	if(Nes->rom->IsUnifMapper())
		::strcpy( dlg.m_szName, Nes->rom->GetBoardName() );
	else
		::strcpy( dlg.m_szName, Nes->rom->GetRomName() );

	dlg.m_nMapper = Nes->rom->GetMapperNo();
	dlg.m_nPRG    = Nes->rom->GetPROM_SIZE();

	//for Game Star - Smart Genius (Unl)
	if(Nes->rom->GetPROM_SIZE()==0xff)
			dlg.m_nPRG    = Nes->rom->GetPROM_SIZE()+1;

	dlg.m_nCHR    = Nes->rom->GetVROM_SIZE();
	dlg.m_bMirror  = Nes->rom->IsVMIRROR();
	dlg.m_bSram    = Nes->rom->IsSAVERAM();
	dlg.m_b4Screen = Nes->rom->Is4SCREEN();
	dlg.m_bTrainer = Nes->rom->IsTRAINER();
	dlg.m_bVSUnisystem = Nes->rom->IsVSUNISYSTEM();

	if( Nes->rom->GetMapperNo() < 256 && Nes->rom->GetMapperNo() != 20 ) {
		dlg.m_dwCRC    = Nes->rom->GetPROM_CRC();
		dlg.m_dwCRCALL = Nes->rom->GetROM_CRC();
		dlg.m_dwCRCCHR = Nes->rom->GetVROM_CRC();
	}
	if(Nes->rom->IsUnifMapper()){
		dlg.m_dwCRC    = Nes->rom->GetPROM_CRC();
		dlg.m_dwCRCCHR = Nes->rom->GetVROM_CRC();
	}

	if( !m_bMenu )
		OnFullScreenGDI( TRUE );
	dlg.DoModal( m_hWnd );
	if( !m_bMenu )
		OnFullScreenGDI( FALSE );
}

WNDCMD	CMainFrame::OnWaveRecord( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnWaveRecord\n" );

	if( !Emu.IsRunning() || !Nes )
		return;

	if( Emu.IsWaveRecord() ) {
	// 録音中だったら止める
		Emu.Event( CEmuThread::EV_WAVEREC_STOP );
	} else {
	// 初めてなら
		string	pathstr, tempstr;
		if( Config.path.bWavePath ) {
			pathstr = CPathlib::CreatePath( CApp::GetModulePath(), Config.path.szWavePath );
			::CreateDirectory( pathstr.c_str(), NULL );
		} else {
			pathstr = Nes->rom->GetRomPath();
		}
		tempstr = CPathlib::MakePathExt( pathstr.c_str(), Nes->rom->GetRomName(), "wav" );
		DEBUGOUT( "Path: %s\n", tempstr.c_str() );

		OPENFILENAME	ofn;
		CHAR	szFile[_MAX_PATH];

		::strcpy( szFile, tempstr.c_str() );
		ZEROMEMORY( &ofn, sizeof(ofn) );

		CHAR	szTitle[256];
		CApp::LoadString( IDS_UI_WAVERECORD, szTitle, sizeof(szTitle) );
		ofn.lpstrTitle      = szTitle;
		ofn.lStructSize     = sizeof(ofn);
		ofn.hwndOwner       = m_hWnd;
		ofn.lpstrFile       = szFile;
		ofn.lpstrDefExt     = "wav";
		ofn.nMaxFile        = sizeof(szFile);
		ofn.lpstrFilter     = "Wave Files(*.wav)\0*.wav\0All Files(*.*)\0*.*\0";
		ofn.nFilterIndex    = 1;
		ofn.Flags           = OFN_EXPLORER|OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT;
		ofn.lpstrInitialDir = pathstr.c_str();

		if( !m_bMenu )
			Emu.EventParam( CEmuThread::EV_FULLSCREEN_GDI, TRUE );

		if( ::GetSaveFileName( &ofn ) ) {
			Emu.EventParam( CEmuThread::EV_WAVEREC_START, (INT)szFile );
		}

		if( !m_bMenu )
			Emu.EventParam( CEmuThread::EV_FULLSCREEN_GDI, FALSE );
	}
}

WNDCMD	CMainFrame::OnLauncher( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnLauncher\n" );

	if( !m_LauncherDlg.m_hWnd ) {
		m_LauncherDlg.Create( NULL );
	}

	if( Config.general.bScreenMode ) {
		if( Emu.IsRunning() ) {
			Emu.EventParam( CEmuThread::EV_FULLSCREEN_GDI, TRUE );
		} else {
			DirectDraw.SetFullScreenGDI( TRUE );
		}
		// ダイアログを中央に移動する:)
		RECT	rcParent, rc;
		::GetWindowRect( m_hWnd, &rcParent );
		::GetWindowRect( m_LauncherDlg.m_hWnd, &rc );
		INT x = rcParent.left+(rcParent.right-rcParent.left)/2-(rc.right-rc.left)/2;
		INT y = rcParent.top +(rcParent.bottom-rcParent.top)/2-(rc.bottom-rc.top)/2;
		::SetWindowPos( m_LauncherDlg.m_hWnd, NULL, x, y, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );

		::SetWindowPos( m_LauncherDlg.m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
	} else {
		::SetWindowPos( m_LauncherDlg.m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
	}

	::ShowWindow( m_LauncherDlg.m_hWnd, SW_SHOW );
}

WNDCMD	CMainFrame::OnNetPlayConnect( WNDCMDPARAM )
{
	if( !Emu.IsRunning() || !Nes )
		return;
	if( Nes->IsMoviePlay() || Nes->IsMovieRec() )
		return;
	if( NetPlay.IsConnect() )
		return;

	Emu.Pause();

	if( !m_bMenu )
		OnFullScreenGDI( TRUE );

	CNetPlayDlg	dlg;
	if( dlg.DoModal( m_hWnd ) == IDOK ) {
		if( !m_ChatDlg.m_hWnd ) {
			m_ChatDlg.Create( NULL );
		}
		::ShowWindow( m_ChatDlg.m_hWnd, SW_SHOW );

		Emu.Event( CEmuThread::EV_NETPLAY_START );

		// test
		if( !Config.emulator.bBackground ) {
			Emu.Resume();
		}
	}

	if( !m_bMenu )
		OnFullScreenGDI( FALSE );

	Emu.Resume();
}

WNDCMD	CMainFrame::OnNetPlayDisconnect( WNDCMDPARAM )
{
	if( !NetPlay.IsConnect() )
		return;

	Emu.Pause();
	NetPlay.Disconnect();
	Emu.Resume();
}

WNDCMD	CMainFrame::OnNetPlayChat( WNDCMDPARAM )
{
	DEBUGOUT( "CMainFrame::OnNetPlayChat\n" );
	if( !NetPlay.IsConnect() )
		return;

	if( Config.general.bScreenMode ) {
		if( Emu.IsRunning() ) {
			Emu.EventParam( CEmuThread::EV_FULLSCREEN_GDI, TRUE );
		} else {
			DirectDraw.SetFullScreenGDI( TRUE );
		}
		// ダイアログが画面外ならば中央に移動する:)
		RECT	rcParent, rc;
		::GetWindowRect( m_hWnd, &rcParent );
		::GetWindowRect( m_ChatDlg.m_hWnd, &rc );

		if( (rc.right < rcParent.left) || (rc.left > rcParent.right)
		 || (rc.bottom < rcParent.top) || (rc.top > rcParent.bottom) ) {
			INT x = rcParent.left+(rcParent.right-rcParent.left)/2-(rc.right-rc.left)/2;
			INT y = rcParent.top +(rcParent.bottom-rcParent.top)/2-(rc.bottom-rc.top)/2;
			::SetWindowPos( m_ChatDlg.m_hWnd, NULL, x, y, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );
		} else {
			::SetWindowPos( m_ChatDlg.m_hWnd, NULL, 0, 0, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );
		}
		::SetWindowPos( m_ChatDlg.m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE );
	} else {
		::SetWindowPos( m_ChatDlg.m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW );
	}
//	::ShowWindow( m_ChatDlg.m_hWnd, SW_SHOW );
}

WNDCMD	CMainFrame::OnSearch( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnSearch\n" );

	if( !Emu.IsRunning() || !Nes )
		return;

	if( !m_SearchDlg.m_hWnd ) {
		m_SearchDlg.Create( NULL );
	}

	if( Config.general.bScreenMode ) {
		if( Emu.IsRunning() ) {
			Emu.EventParam( CEmuThread::EV_FULLSCREEN_GDI, TRUE );
		} else {
			DirectDraw.SetFullScreenGDI( TRUE );
		}
		// ダイアログを中央に移動する:)
		RECT	rcParent, rc;
		::GetWindowRect( m_hWnd, &rcParent );
		::GetWindowRect( m_SearchDlg.m_hWnd, &rc );
		INT x = rcParent.left+(rcParent.right-rcParent.left)/2-(rc.right-rc.left)/2;
		INT y = rcParent.top +(rcParent.bottom-rcParent.top)/2-(rc.bottom-rc.top)/2;
		::SetWindowPos( m_SearchDlg.m_hWnd, NULL, x, y, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );

		::SetWindowPos( m_SearchDlg.m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
	} else {
		::SetWindowPos( m_SearchDlg.m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
	}

	::ShowWindow( m_SearchDlg.m_hWnd, SW_SHOW );
}

WNDCMD	CMainFrame::OnCheat( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnCheat\n" );

	if( !Emu.IsRunning() || !Nes )
		return;

//	if( m_SearchDlg.m_hWnd ) {
//		if( ::IsWindowVisible( m_SearchDlg.m_hWnd ) )
//			return;
//	}

	if( !m_bMenu )
		OnFullScreenGDI( TRUE );

	CCheatCodeDlg	dlg;

	dlg.DoModal( m_hWnd );

	if( !m_bMenu )
		OnFullScreenGDI( FALSE );
}

WNDCMD	CMainFrame::OnGenie( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnGenie\n" );

	if( !Emu.IsRunning() || !Nes )
		return;

	Emu.Pause();

	string	pathstr, tempstr;
	if( Config.path.bCheatPath ) {
		pathstr = CPathlib::CreatePath( CApp::GetModulePath(), Config.path.szCheatPath );
		::CreateDirectory( pathstr.c_str(), NULL );
	} else {
		pathstr = Nes->rom->GetRomPath();
	}
	tempstr = CPathlib::MakePathExt( pathstr.c_str(), Nes->rom->GetRomName(), "gen" );
	DEBUGOUT( "Path: %s\n", tempstr.c_str() );

	OPENFILENAME	ofn;
	CHAR	szFile[_MAX_PATH];

	::strcpy( szFile, tempstr.c_str() );
	ZEROMEMORY( &ofn, sizeof(ofn) );

	ofn.lStructSize     = sizeof(ofn);
	ofn.hwndOwner       = m_hWnd;
	ofn.lpstrFile       = szFile;
	ofn.nMaxFile        = sizeof(szFile);
	ofn.lpstrFilter     = "GameGeine Files(*.gen)\0*.gen\0All Files(*.*)\0*.*\0";
	ofn.nFilterIndex    = 1;
	ofn.Flags           = OFN_EXPLORER|OFN_PATHMUSTEXIST;
	ofn.lpstrInitialDir = pathstr.c_str();

	CHAR	szTitle[256];
	CApp::LoadString( IDS_UI_LOADGENIECODE, szTitle, sizeof(szTitle) );
	ofn.lpstrTitle = szTitle;
	if( ::GetOpenFileName( &ofn ) ) {
		Nes->GenieLoad( szFile );
	}

	Emu.Resume();
}

WNDCMD	CMainFrame::OnCheatCommand( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnCheatCommand\n" );

	if( !Emu.IsRunning() || !Nes )
		return;

	if( uID == ID_CHEAT_ENABLE ) {
		Nes->SetCheatCodeAllFlag( TRUE, TRUE );
	} else
	if( uID == ID_CHEAT_DISABLE ) {
		Nes->SetCheatCodeAllFlag( FALSE, TRUE );
	}
}

WNDCMD	CMainFrame::OnDatachBacode( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnDatachBacode\n" );

	if( !Emu.IsRunning() || !Nes )
		return;

	if( !m_DatachBarcodeDlg.m_hWnd ) {
		m_DatachBarcodeDlg.Create( NULL );
	}

	if( Config.general.bScreenMode ) {
		if( Emu.IsRunning() ) {
			Emu.EventParam( CEmuThread::EV_FULLSCREEN_GDI, TRUE );
		} else {
			DirectDraw.SetFullScreenGDI( TRUE );
		}
		// ダイアログを中央に移動する:)
		RECT	rcParent, rc;
		::GetWindowRect( m_hWnd, &rcParent );
		::GetWindowRect( m_DatachBarcodeDlg.m_hWnd, &rc );
		INT x = rcParent.left+(rcParent.right-rcParent.left)/2-(rc.right-rc.left)/2;
		INT y = rcParent.top +(rcParent.bottom-rcParent.top)/2-(rc.bottom-rc.top)/2;
		::SetWindowPos( m_DatachBarcodeDlg.m_hWnd, NULL, x, y, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );

		::SetWindowPos( m_DatachBarcodeDlg.m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
	} else {
		::SetWindowPos( m_DatachBarcodeDlg.m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
	}

	::ShowWindow( m_DatachBarcodeDlg.m_hWnd, SW_SHOW );
}


WNDCMD	CMainFrame::OnDipSwitch( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnDipSwitch\n" );
	CDipSwitchDlg dlg;
	if( dlg.DoModal( m_hWnd ) == IDOK ) {
	}
}

WNDCMD	CMainFrame::OnEmulatorCfg( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnEmulatorCfg\n" );
	CEmulatorDlg dlg;
	if( dlg.DoModal( m_hWnd ) == IDOK ) {
		Emu.SetPriority( Config.emulator.nPriority );
	}
}

WNDCMD	CMainFrame::OnGraphicsCfg( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnGraphicsCfg\n" );
	CGraphicsDlg dlg;
	if( dlg.DoModal( m_hWnd ) == IDOK ) {
		Emu.Pause();

		// サーフェスをリビルドする必要があるかのチェック
		BOOL	bRebuildDirectDraw = FALSE;
		BOOL	bRebuildSurface = FALSE;
		BOOL	bMenuOFF = FALSE;

		if( DirectDraw.GetUseHEL() != Config.graphics.bUseHEL )
			bRebuildDirectDraw = TRUE;
		if( DirectDraw.GetSystemMemory() != Config.graphics.bSystemMemory )
			bRebuildSurface = TRUE;

		// DirectDrawｵﾌﾟｼｮﾝの設定
		DirectDraw.SetFlipMode     ( Config.graphics.bSyncDraw );
		DirectDraw.SetAspectMode   ( Config.graphics.bAspect );
		DirectDraw.SetAllLineMode  ( Config.graphics.bAllLine );
		DirectDraw.SetMaxZoom      ( Config.graphics.bFitZoom );
		DirectDraw.SetTVFrameMode  ( Config.graphics.bTVFrame );
		DirectDraw.SetScanlineMode ( Config.graphics.bScanline );
		DirectDraw.SetScanlineColor( Config.graphics.nScanlineColor );

		DirectDraw.SetUseHEL      ( Config.graphics.bUseHEL );
		DirectDraw.SetDoubleSize  ( Config.graphics.bDoubleSize );
		DirectDraw.SetSystemMemory( Config.graphics.bSystemMemory );

		DirectDraw.SetWindowVSyncMode( Config.graphics.bWindowVSync );

		if( Config.general.bScreenMode ) {
			// スクリーンモードの変更があった場合
			if( !DirectDraw.IsNowDisplayMode( Config.graphics.dwDisplayWidth,
							  Config.graphics.dwDisplayHeight,
							  Config.graphics.dwDisplayDepth,
							  Config.graphics.dwDisplayRate ) ) {
				if( !bRebuildDirectDraw )
					DirectDraw.BeginDisplayChange();

				// スクリーンモードの設定
				DirectDraw.SetDisplayMode( Config.graphics.dwDisplayWidth,
							   Config.graphics.dwDisplayHeight,
							   Config.graphics.dwDisplayDepth,
							   Config.graphics.dwDisplayRate );

				DirectDraw.SetScreenMode( Config.general.bScreenMode );
				if( !bRebuildDirectDraw )
					DirectDraw.EndDisplayChange();

				::SetWindowPos( m_hWnd, HWND_NOTOPMOST, 0, 0, ::GetSystemMetrics(SM_CXSCREEN),
						::GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW );

				// パレット
				::PostMessage( m_hWnd, WM_QUERYNEWPALETTE, 0, 0 );

				// サーフェスは再構築されたので二度は行わない
				bRebuildSurface = FALSE;

				bMenuOFF = TRUE;
			}
		} else {
			OnSetWindowSize();
		}
		if( bRebuildDirectDraw ) {
			DirectDraw.ReleaseDDraw();
			DirectDraw.InitialDDraw( m_hWnd );
			DirectDraw.InitialSurface( Config.general.bScreenMode );
			bMenuOFF = TRUE;
		} else
		if( bRebuildSurface ) {
			DirectDraw.ReleaseSurface();
			DirectDraw.InitialSurface( Config.general.bScreenMode );
			bMenuOFF = TRUE;
		}

		if( Config.general.bScreenMode && bMenuOFF ) {
			if( Emu.IsRunning() ) {
				OnChangeMenu( FALSE );
			}
		}

		Emu.Resume();
	}
}

WNDCMD	CMainFrame::OnSoundCfg( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnSoundCfg\n" );
	CSoundDlg dlg;

	DWORD	rate, bits;
	DirectSound.GetSamplingRate( rate, bits );
	BOOL	bSoundOn = DirectSound.IsStreamPlay();

	if( dlg.DoModal( m_hWnd ) == IDOK ) {
		if( Config.sound.nRate != (INT)rate
		 || Config.sound.nBits != (INT)bits
		 || Config.sound.nBufferSize != DirectSound.GetBufferSize() ) {
			Emu.Pause();
			if( bSoundOn ) {
				DirectSound.StreamStop();
			}

			DirectSound.ReleaseBuffer();

			DirectSound.SetSamplingRate( Config.sound.nRate, Config.sound.nBits );
			DirectSound.SetBufferSize( Config.sound.nBufferSize );

			if( DirectSound.InitialBuffer() ) {
				if( Nes ) {
					Nes->SoundSetup();
				}
				if( bSoundOn ) {
					DirectSound.StreamPlay();
					DirectSound.StreamPause();
				}
			}
			Emu.Resume();
		}
	}
}

WNDCMD	CMainFrame::OnControllerCfg( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnControllerCfg\n" );
	CControllerDlg dlg;

	CWndHook::SetFiltering( TRUE );
	dlg.DoModal( m_hWnd );
	CWndHook::SetFiltering( FALSE );
}

WNDCMD	CMainFrame::OnShortcutCfg( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnShortcutCfg\n" );
	CShortcutDlg dlg;

	CWndHook::SetFiltering( TRUE );

	if( dlg.DoModal( m_hWnd ) == IDOK ) {
		::SetMenu( m_hWnd, NULL );
		::DestroyMenu( m_hMenu );
		// メニューの再ロード
		m_hMenu = CApp::LoadMenu( IDR_MENU );
		CApp::SetMenu( m_hMenu );
		::SetMenu( m_hWnd, m_hMenu );
		OnRebuildMenu();
	}

	CWndHook::SetFiltering( FALSE );
}

WNDCMD	CMainFrame::OnFolderCfg( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnFolderCfg\n" );
	CFolderDlg dlg;
	dlg.DoModal( m_hWnd );
}

WNDCMD	CMainFrame::OnLanguageCfg( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnLanguageCfg\n" );
	CLanguageDlg dlg;

	if( dlg.DoModal( m_hWnd ) == IDOK ) {
		// ランチャー起動中ならば閉じる
		BOOL	bLauncher = FALSE;
		BOOL	bLauncherVisible = FALSE;
		if( m_LauncherDlg.m_hWnd && ::IsWindow(m_LauncherDlg.m_hWnd) ) {
			bLauncherVisible = ::IsWindowVisible( m_LauncherDlg.m_hWnd );
			bLauncher = TRUE;
			m_LauncherDlg.Destroy();
		}

		// チャットダイアログは閉じる
		BOOL	bChat = FALSE;
		BOOL	bChatVisible = FALSE;
		if( m_ChatDlg.m_hWnd && ::IsWindow(m_ChatDlg.m_hWnd) ) {
			bChatVisible = ::IsWindowVisible( m_ChatDlg.m_hWnd );
			bChat = TRUE;
			m_ChatDlg.Destroy();
		}

		// サーチダイアログは閉じる
		if( m_SearchDlg.m_hWnd && ::IsWindow(m_SearchDlg.m_hWnd) ) {
			m_SearchDlg.Destroy();
		}

		// バーコード入力ダイアログは閉じる
		if( m_DatachBarcodeDlg.m_hWnd && ::IsWindow(m_DatachBarcodeDlg.m_hWnd) ) {
			m_DatachBarcodeDlg.Destroy();
		}

		// メニューの破棄
		::SetMenu( m_hWnd, NULL );
		::DestroyMenu( m_hMenu );

		// 元のプラグインの開放
		::FreeLibrary( CApp::GetPlugin() );
		// 新しいプラグインのロード
		HINSTANCE hPlugin;
		if( !(hPlugin = ::LoadLibrary( CPlugin::GetPluginPath() )) ) {
			::MessageBox( m_hWnd, "Language plug-in load failed.", "VirtuaNES", MB_ICONERROR|MB_OK );
			::PostMessage( m_hWnd, WM_CLOSE, 0, 0 );
			return;
		}
		CApp::SetPlugin( hPlugin );
		// メニューの再ロード
		m_hMenu = CApp::LoadMenu( IDR_MENU );
		CApp::SetMenu( m_hMenu );
		::SetMenu( m_hWnd, m_hMenu );

		OnRebuildMenu();

		// ランチャー再起動
		if( bLauncher ) {
			m_LauncherDlg.Create( NULL );
			if( bLauncherVisible ) {
				::ShowWindow( m_LauncherDlg.m_hWnd, SW_SHOW );
			}
		}

		// チャット再起動
		if( bChat ) {
			m_ChatDlg.Create( NULL );
			if( bChatVisible ) {
				::ShowWindow( m_ChatDlg.m_hWnd, SW_SHOW );
			}
		}

		::SetForegroundWindow( m_hWnd );
	}
}

WNDCMD	CMainFrame::OnMovieCfg( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnMovieCfg\n" );
	CMovieDlg dlg;
	dlg.DoModal( m_hWnd );
}

WNDCMD	CMainFrame::OnGameOptionCfg( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnGameOptionCfg\n" );
	CGameOptionDlg dlg;
	dlg.DoModal( m_hWnd );
}

WNDCMD	CMainFrame::OnJoyAxisCfg( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnJoyAxisCfg\n" );
	CJoyAxisDlg dlg;
	dlg.DoModal( m_hWnd );

	DirectInput.SetJoyAxisMode( Config.general.JoyAxisSetting );
}

WNDCMD	CMainFrame::OnPaletteEditCfg( WNDCMDPARAM )
{
	DEBUGOUT( "CMainFrame::OnPaletteEditCfg\n" );

	if( !m_PaletteEdit.m_hWnd ) {
		m_PaletteEdit.Create( NULL );
	}

	if( Config.general.bScreenMode ) {
		if( Emu.IsRunning() ) {
			Emu.EventParam( CEmuThread::EV_FULLSCREEN_GDI, TRUE );
		} else {
			DirectDraw.SetFullScreenGDI( TRUE );
		}
		// ダイアログを中央に移動する:)
		RECT	rcParent, rc;
		::GetWindowRect( m_hWnd, &rcParent );
		::GetWindowRect( m_PaletteEdit.m_hWnd, &rc );
		INT x = rcParent.left+(rcParent.right-rcParent.left)/2-(rc.right-rc.left)/2;
		INT y = rcParent.top +(rcParent.bottom-rcParent.top)/2-(rc.bottom-rc.top)/2;
		::SetWindowPos( m_PaletteEdit.m_hWnd, NULL, x, y, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );

		::SetWindowPos( m_PaletteEdit.m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
	} else {
		::SetWindowPos( m_PaletteEdit.m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
	}

	::ShowWindow( m_PaletteEdit.m_hWnd, SW_SHOW );
}

WNDCMD	CMainFrame::OnFullScreen( WNDCMDPARAM )
{
	DEBUGOUT( "CMainFrame::OnFullScreen\n" );

	m_bKeyEnable = FALSE;

	// 危険なので...
	Emu.Pause();

	Config.general.bScreenMode = !Config.general.bScreenMode;
//	OnChangeMenu( FALSE );

	if( !Config.general.bScreenMode ) {
	// FullScreen to Window mode
		DirectDraw.BeginDisplayChange();
		// 位置の復帰
		::SetWindowLong( m_hWnd, GWL_STYLE, m_StyleBackup );
		::SetWindowLong( m_hWnd, GWL_EXSTYLE, m_ExStyleBackup );
		::SetWindowPlacement( m_hWnd, &m_WindowPlacement );
	} else {
	// Window to FullScreen mode
		// 位置の保存
//		m_bZoomed = ::IsZoomed( m_hWnd );
		m_WindowPlacement.length = sizeof(m_WindowPlacement);
		::GetWindowPlacement( m_hWnd, &m_WindowPlacement );

		m_StyleBackup   = ::GetWindowLong( m_hWnd, GWL_STYLE );
		m_ExStyleBackup = ::GetWindowLong( m_hWnd, GWL_EXSTYLE );
		::SetWindowLong( m_hWnd, GWL_STYLE, WS_POPUP );
		::SetWindowLong( m_hWnd, GWL_EXSTYLE, 0 );

		// スクリーンモードの設定
		DirectDraw.SetDisplayMode( Config.graphics.dwDisplayWidth,
					   Config.graphics.dwDisplayHeight,
					   Config.graphics.dwDisplayDepth,
					   Config.graphics.dwDisplayRate );

		DirectDraw.BeginDisplayChange();
	}

//	OnChangeMenu( FALSE );

	DirectDraw.SetScreenMode( Config.general.bScreenMode );
	DirectDraw.EndDisplayChange();

	if( !Config.general.bScreenMode ) {
		if( m_LauncherDlg.m_hWnd && ::IsWindow(m_LauncherDlg.m_hWnd) ) {
			::SetWindowPos( m_LauncherDlg.m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE );
		}

		OnChangeMenu( TRUE );
		::SetForegroundWindow( m_hWnd );

		::RedrawWindow( m_hWnd, NULL, NULL, RDW_FRAME|RDW_INVALIDATE );
	} else {
		if( Emu.IsRunning() ) {
			OnChangeMenu( FALSE );
		} else {
			OnChangeMenu( TRUE );
		}
		::SetWindowPos( m_hWnd, HWND_NOTOPMOST, 0, 0, ::GetSystemMetrics(SM_CXSCREEN),
				::GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW );
	}

	// 危険なので...
	Emu.Resume();

	m_bKeyEnable = TRUE;
}

WNDCMD	CMainFrame::OnZoom( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnZoom %d\n", uID-ID_ZOOMx1 );

	// フルスクリーン時は無視
	if( Config.general.bScreenMode )
		return;

	Config.general.nScreenZoom = (INT)(uID-ID_ZOOMx1);
	OnSetWindowSize();

	CHAR	szStr[64];
	::wsprintf( szStr, "Screen Zoom *%d", uID-ID_ZOOMx1+1 );
	Emu.EventParam( CEmuThread::EV_MESSAGE_OUT, (LONG)szStr );
}

WNDCMD	CMainFrame::OnViewCommand( WNDCMDPARAM )
{
	if( !Emu.IsRunning() || !Nes )
		return;

	switch( uID ) {
		case	ID_VIEW_PATTERN:
			if( !m_PatternView.m_hWnd ) {
				m_PatternView.Create( HWND_DESKTOP );
			}
			::SetWindowPos( m_PatternView.m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
			break;
		case	ID_VIEW_NAMETABLE:
			if( !m_NameTableView.m_hWnd ) {
				m_NameTableView.Create( HWND_DESKTOP );
			}
			::SetWindowPos( m_NameTableView.m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
			break;
		case	ID_VIEW_PALETTE:
			if( !m_PaletteView.m_hWnd ) {
				m_PaletteView.Create( HWND_DESKTOP );
			}
			::SetWindowPos( m_PaletteView.m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
			break;

		case	ID_VIEW_MEMORY:
			if( !m_MemoryView.m_hWnd ) {
				m_MemoryView.Create( HWND_DESKTOP );
			}
			::SetWindowPos( m_MemoryView.m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
			break;
		default:
			break;
	}
}

WNDCMD	CMainFrame::OnEmuCommand( WNDCMDPARAM )
{
//	DEBUGOUT( "CMainFrame::OnEmuCommand %d\n", uID );

	switch( uID ) {
		case	ID_HWRESET:
			Emu.Event( CEmuThread::EV_HWRESET );
			break;
		case	ID_SWRESET:
			Emu.Event( CEmuThread::EV_SWRESET );
			break;
		case	ID_PAUSE:
			Emu.Event( CEmuThread::EV_EMUPAUSE );
			break;

		case	ID_ONEFRAME:
			Emu.Event( CEmuThread::EV_ONEFRAME );
			break;

		case	ID_THROTTLE:
			Emu.Event( CEmuThread::EV_THROTTLE );
			break;
		case	ID_FRAMESKIP_UP:
			Emu.Event( CEmuThread::EV_FRAMESKIP_UP );
			break;
		case	ID_FRAMESKIP_DOWN:
			Emu.Event( CEmuThread::EV_FRAMESKIP_DOWN );
			break;
		case	ID_FRAMESKIP_AUTO:
			Emu.Event( CEmuThread::EV_FRAMESKIP_AUTO );
			break;

		case	ID_STATE_UP:
			{
			CHAR	szStr[64];
			if( ++m_nStateSlot > 10-1 ) {
				m_nStateSlot = 0;
			}
			::wsprintf( szStr, "State Slot #%d", m_nStateSlot );
			Emu.EventParam( CEmuThread::EV_MESSAGE_OUT, (LONG)szStr );
			}
			break;
		case	ID_STATE_DOWN:
			{
			CHAR	szStr[64];
			if( --m_nStateSlot < 0 ) {
				m_nStateSlot = 10-1;
			}
			::wsprintf( szStr, "State Slot #%d", m_nStateSlot );
			Emu.EventParam( CEmuThread::EV_MESSAGE_OUT, (LONG)szStr );
			}
			break;

		case	ID_STATE_SLOT0:	case	ID_STATE_SLOT1:
		case	ID_STATE_SLOT2:	case	ID_STATE_SLOT3:
		case	ID_STATE_SLOT4:	case	ID_STATE_SLOT5:
		case	ID_STATE_SLOT6:	case	ID_STATE_SLOT7:
		case	ID_STATE_SLOT8:	case	ID_STATE_SLOT9:
			m_nStateSlot = (INT)(uID-ID_STATE_SLOT0);
			{
			CHAR	szStr[64];
			::wsprintf( szStr, "State Slot #%d", m_nStateSlot );
			Emu.EventParam( CEmuThread::EV_MESSAGE_OUT, (LONG)szStr );
			}
			break;

		case	ID_DISK_EJECT:
		case	ID_DISK_0A: case	ID_DISK_0B:
		case	ID_DISK_1A: case	ID_DISK_1B:
			Emu.EventParam( CEmuThread::EV_DISK_COMMAND, (LONG)(uID-ID_DISK_EJECT) );
			break;

		case	ID_MUTE_0: case	ID_MUTE_1: case	ID_MUTE_2: case	ID_MUTE_3:
		case	ID_MUTE_4: case	ID_MUTE_5: case	ID_MUTE_6: case	ID_MUTE_7:
		case	ID_MUTE_8: case	ID_MUTE_9: case	ID_MUTE_A: case	ID_MUTE_B:
		case	ID_MUTE_C: case	ID_MUTE_D: case	ID_MUTE_E: case	ID_MUTE_F:
			Emu.EventParam( CEmuThread::EV_SOUND_MUTE, (LONG)(uID-ID_MUTE_0) );
			break;

		case	ID_EXCTR_NONE:
		case	ID_EXCTR_PADDLE:
		case	ID_EXCTR_HYPERSHOT:
		case	ID_EXCTR_ZAPPER:
		case	ID_EXCTR_KEYBOARD:
		case	ID_EXCTR_SUBOR_KEYBOARD:
		case	ID_EXCTR_CRAZYCLIMBER:
		case	ID_EXCTR_SPACESHADOWGUN:
		case	ID_EXCTR_FAMILYTRAINER_A:
		case	ID_EXCTR_FAMILYTRAINER_B:
		case	ID_EXCTR_MAHJANG:
		case	ID_EXCTR_EXCITINGBOXING:
		case	ID_EXCTR_OEKAKIDS_TABLET:
		case	ID_EXCTR_TURBOFILE:
		case	ID_EXCTR_CHINA_EDUCATIONAL_MOUSE:
		case	ID_EXCTR_VSUNISYSTEM:
		case	ID_EXCTR_VSUNISYSTEM_ZAPPER:
			if(uID>=ID_EXCTR_CRAZYCLIMBER){
				uID++;
			}else if(uID==ID_EXCTR_SUBOR_KEYBOARD){
				uID=ID_EXCTR_CRAZYCLIMBER;
			}
			Emu.EventParam( CEmuThread::EV_EXCONTROLLER, uID-ID_EXCTR_NONE );
			break;

		case	ID_TURBOFILE_BANK0:
		case	ID_TURBOFILE_BANK1:
		case	ID_TURBOFILE_BANK2:
		case	ID_TURBOFILE_BANK3:
			Emu.EventParam( CEmuThread::EV_TURBOFILE, uID-ID_TURBOFILE_BANK0 );
			break;

		case	ID_SNAPSHOT:
			Emu.Event( CEmuThread::EV_SNAPSHOT );
			break;

		case	ID_TVASPECT:
			Config.graphics.bAspect = !Config.graphics.bAspect;
			Emu.Pause();
			DirectDraw.SetAspectMode( Config.graphics.bAspect );
			if( !Config.general.bScreenMode )
				OnSetWindowSize();
			Emu.Resume();
			break;
		case	ID_SCANLINE:
			Config.graphics.bScanline = !Config.graphics.bScanline;
			Emu.Pause();
			DirectDraw.SetScanlineMode( Config.graphics.bScanline );
			Emu.Resume();
			break;
		case	ID_ALLLINE:
			Config.graphics.bAllLine = !Config.graphics.bAllLine;
			Emu.Pause();
			DirectDraw.SetAllLineMode( Config.graphics.bAllLine );
			if( !Config.general.bScreenMode )
				OnSetWindowSize();
			Emu.Resume();
			break;
		case	ID_ALLSPRITE:
			Config.graphics.bAllSprite = !Config.graphics.bAllSprite;
			break;
		case	ID_SYNCDRAW:
			Config.graphics.bSyncDraw = !Config.graphics.bSyncDraw;
			Emu.Pause();
			DirectDraw.SetFlipMode( Config.graphics.bSyncDraw );
			Emu.Resume();
			break;
		case	ID_FITSCREEN:
			Config.graphics.bFitZoom = !Config.graphics.bFitZoom;
			Emu.Pause();
			DirectDraw.SetMaxZoom( Config.graphics.bFitZoom );
			Emu.Resume();
			break;

		case	ID_TVFRAME:
			Config.graphics.bTVFrame = !Config.graphics.bTVFrame;
			Emu.Pause();
			DirectDraw.SetTVFrameMode( Config.graphics.bTVFrame );
			Emu.Resume();
			break;
		case	ID_FPSDISP:
			Config.graphics.bFPSDisp = !Config.graphics.bFPSDisp;
			break;

		case	ID_LEFTCLIP:
			Config.graphics.bLeftClip = !Config.graphics.bLeftClip;
			break;

		case	ID_FILTER_NONE:
			Emu.EventParam( CEmuThread::EV_MESSAGE_OUT, (LONG)"Filter: None" );
			goto	_gohell;
		case	ID_FILTER_2XSAI:
			Emu.EventParam( CEmuThread::EV_MESSAGE_OUT, (LONG)"Filter: 2xSaI" );
			goto	_gohell;
		case	ID_FILTER_SUPER2XSAI:
			Emu.EventParam( CEmuThread::EV_MESSAGE_OUT, (LONG)"Filter: Super2xSaI" );
			goto	_gohell;
		case	ID_FILTER_SUPEREAGLE:
			Emu.EventParam( CEmuThread::EV_MESSAGE_OUT, (LONG)"Filter: SuperEagle" );
			goto	_gohell;
		case	ID_FILTER_SCALE2X:
			Emu.EventParam( CEmuThread::EV_MESSAGE_OUT, (LONG)"Filter: Scale2x" );
//			goto	_gohell;
_gohell:
			Config.graphics.nGraphicsFilter = (INT)(uID-ID_FILTER_NONE);
			Emu.Pause();
			DirectDraw.SetGraphicsFilter( Config.graphics.nGraphicsFilter );
			Emu.Resume();
			break;

		default:
			break;
	}
}

WNDCMD	CMainFrame::OnStateCommand( WNDCMDPARAM )
{
	if( !Emu.IsRunning() )
		return;

	CHAR	st[16];
	::wsprintf( st, "st%1X", m_nStateSlot );

	string	pathstr, tempstr;
	if( Config.path.bStatePath ) {
		pathstr = CPathlib::CreatePath( CApp::GetModulePath(), Config.path.szStatePath );
		::CreateDirectory( pathstr.c_str(), NULL );
	DEBUGOUT( "Path: %s\n", pathstr.c_str() );
	} else {
		pathstr = Nes->rom->GetRomPath();
	}
	tempstr = CPathlib::MakePathExt( pathstr.c_str(), Nes->rom->GetRomName(), st );
	DEBUGOUT( "Path: %s\n", tempstr.c_str() );

	if( uID == ID_STATE_LOAD ) {
		INT	ret;
		if( (ret = NES::IsStateFile( tempstr.c_str(), Nes->rom )) >= 0 ) {
			if( ret == IDS_ERROR_ILLEGALSTATECRC ) {
				if( Config.emulator.bCrcCheck ) {
					if( ::MessageBox( m_hWnd, CApp::GetErrorString(ret), "VirtuaNES", MB_ICONWARNING|MB_YESNO ) != IDYES )
						return;
				}
			}
		}
		Emu.EventParam2( CEmuThread::EV_STATE_LOAD, (INT)tempstr.c_str(), m_nStateSlot );
	}
	if( uID == ID_STATE_SAVE ) {
		Emu.EventParam2( CEmuThread::EV_STATE_SAVE, (INT)tempstr.c_str(), m_nStateSlot );
	}
}

WNDCMD	CMainFrame::OnStateCommand2( WNDCMDPARAM )
{
	if( !Emu.IsRunning() )
		return;

	BOOL	bLoad = FALSE;
	INT	slot = 0;
	if( uID >= ID_QUICKLOAD_SLOT0 && uID <= ID_QUICKLOAD_SLOT9 ) {
		bLoad = TRUE;
		slot = uID - ID_QUICKLOAD_SLOT0;
	} else {
		bLoad = FALSE;
		slot = uID - ID_QUICKSAVE_SLOT0;
	}

	CHAR	st[16];
	::wsprintf( st, "st%1X", slot );

	string	pathstr, tempstr;
	if( Config.path.bStatePath ) {
		pathstr = CPathlib::CreatePath( CApp::GetModulePath(), Config.path.szStatePath );
		::CreateDirectory( pathstr.c_str(), NULL );
	DEBUGOUT( "Path: %s\n", pathstr.c_str() );
	} else {
		pathstr = Nes->rom->GetRomPath();
	}
	tempstr = CPathlib::MakePathExt( pathstr.c_str(), Nes->rom->GetRomName(), st );
	DEBUGOUT( "Path: %s\n", tempstr.c_str() );

	if( bLoad ) {
		INT	ret;
		if( (ret = NES::IsStateFile( tempstr.c_str(), Nes->rom )) >= 0 ) {
			if( ret == IDS_ERROR_ILLEGALSTATECRC ) {
				if( Config.emulator.bCrcCheck ) {
					if( ::MessageBox( m_hWnd, CApp::GetErrorString(ret), "VirtuaNES", MB_ICONWARNING|MB_YESNO ) != IDYES )
						return;
				}
			}
		}
		Emu.EventParam2( CEmuThread::EV_STATE_LOAD, (INT)tempstr.c_str(), slot );
	}
	if( !bLoad ) {
		Emu.EventParam2( CEmuThread::EV_STATE_SAVE, (INT)tempstr.c_str(), slot );
	}
}

WNDCMD	CMainFrame::OnMovieCommand( WNDCMDPARAM )
{
	if( !Emu.IsRunning() )
		return;

	if( uID == ID_MOVIE_STOP ) {
		Emu.Event( CEmuThread::EV_MOVIE_STOP );
		return;
	}

	Emu.Pause();

	if( Config.general.bScreenMode && !m_bMenu ) {
		OnFullScreenGDI( TRUE );
	}

	string	pathstr, tempstr;
	if( Config.path.bMoviePath ) {
		pathstr = CPathlib::CreatePath( CApp::GetModulePath(), Config.path.szMoviePath );
		::CreateDirectory( pathstr.c_str(), NULL );
	} else {
		pathstr = Nes->rom->GetRomPath();
	}
	tempstr = CPathlib::MakePathExt( pathstr.c_str(), Nes->rom->GetRomName(), "vmv" );
	DEBUGOUT( "Path: %s\n", tempstr.c_str() );

	OPENFILENAME	ofn;
	CHAR	szFile[_MAX_PATH];

	::strcpy( szFile, tempstr.c_str() );
	ZEROMEMORY( &ofn, sizeof(ofn) );

	ofn.lStructSize     = sizeof(ofn);
	ofn.hwndOwner       = m_hWnd;
	ofn.lpstrFile       = szFile;
	ofn.lpstrDefExt     = "vmv";
	ofn.nMaxFile        = sizeof(szFile);
	ofn.lpstrFilter     = "Movie Files(*.vmv)\0*.vmv\0All Files(*.*)\0*.*\0";
	ofn.nFilterIndex    = 1;
	ofn.Flags           = OFN_EXPLORER|OFN_PATHMUSTEXIST;
	ofn.lpstrInitialDir = pathstr.c_str();

	CHAR	szTitle[256];

	if( uID == ID_MOVIE_PLAY ) {
		CApp::LoadString( IDS_UI_PLAYMOVIE, szTitle, sizeof(szTitle) );
		ofn.lpstrTitle = szTitle;
		if( ::GetOpenFileName( &ofn ) ) {
			INT	ret;
			if( (ret = NES::IsMovieFile( szFile, Nes->rom )) >= 0 ) {
				if( ret == IDS_ERROR_ILLEGALMOVIEOLD ) {
					::MessageBox( m_hWnd, CApp::GetErrorString(ret), "VirtuaNES", MB_ICONHAND|MB_OK );
					goto	_Movie_Play_Failed;
				} else
				if( ret == IDS_ERROR_ILLEGALMOVIEVER ) {
					if( Config.emulator.bCrcCheck ) {
						if( ::MessageBox( m_hWnd, CApp::GetErrorString(ret), "VirtuaNES", MB_ICONWARNING|MB_YESNO ) != IDYES )
							goto	_Movie_Play_Failed;
					}
				} else
				if( ret == IDS_ERROR_ILLEGALMOVIECRC ) {
					if( Config.emulator.bCrcCheck ) {
						if( ::MessageBox( m_hWnd, CApp::GetErrorString(ret), "VirtuaNES", MB_ICONWARNING|MB_YESNO ) != IDYES )
							goto	_Movie_Play_Failed;
					}
				}

				Emu.EventParam( CEmuThread::EV_MOVIE_PLAY, (INT)szFile );
			}
_Movie_Play_Failed:;
		}
	}
	if( uID == ID_MOVIE_REC ) {
		ofn.Flags |= OFN_OVERWRITEPROMPT;
		CApp::LoadString( IDS_UI_RECMOVIE, szTitle, sizeof(szTitle) );
		ofn.lpstrTitle = szTitle;
		if( ::GetSaveFileName( &ofn ) ) {
			Emu.EventParam( CEmuThread::EV_MOVIE_REC, (INT)szFile );
		}
	}
	if( uID == ID_MOVIE_REC_APPEND ) {
		CApp::LoadString( IDS_UI_APPENDMOVIE, szTitle, sizeof(szTitle) );
		ofn.lpstrTitle = szTitle;
		if( ::GetOpenFileName( &ofn ) ) {
			INT	ret;
			if( (ret = NES::IsMovieFile( szFile, Nes->rom )) >= 0 ) {
				if( ret == IDS_ERROR_ILLEGALMOVIEOLD ) {
					::MessageBox( m_hWnd, CApp::GetErrorString(IDS_ERROR_ILLEGALMOVIEOLD_A), "VirtuaNES", MB_ICONHAND|MB_OK );
					goto	_Movie_Append_Failed;
				} else
				if( ret == IDS_ERROR_ILLEGALMOVIEVER ) {
					if( Config.emulator.bCrcCheck ) {
						if( ::MessageBox( m_hWnd, CApp::GetErrorString(IDS_ERROR_ILLEGALMOVIEVER_A), "VirtuaNES", MB_ICONWARNING|MB_YESNO ) != IDYES )
							goto	_Movie_Append_Failed;
					}
				} else
				if( ret == IDS_ERROR_ILLEGALMOVIECRC ) {
					if( Config.emulator.bCrcCheck ) {
						if( ::MessageBox( m_hWnd, CApp::GetErrorString(IDS_ERROR_ILLEGALMOVIECRC_A), "VirtuaNES", MB_ICONWARNING|MB_YESNO ) != IDYES )
							goto	_Movie_Append_Failed;
					}
				}

				Emu.EventParam( CEmuThread::EV_MOVIE_RECAPPEND, (INT)szFile );
			} else {
				// 新規作成と同じ
				Emu.EventParam( CEmuThread::EV_MOVIE_REC, (INT)szFile );
			}
		}
_Movie_Append_Failed:;
	}

	if( Config.general.bScreenMode && !m_bMenu ) {
		OnFullScreenGDI( FALSE );
	}

	Emu.Resume();
}

void	CMainFrame::OnMovieInfo( WNDCMDPARAM )
{
	if( !Emu.IsRunning() || !Nes )
		return;

	if( !(Nes->IsMoviePlay() || Nes->IsMovieRec()) )
		return;

	CMovieInfoDlg dlg;

	// メンバの設定
	Nes->GetMovieInfo( dlg.m_wRecVersion, dlg.m_wVersion, dlg.m_dwFrames, dlg.m_dwRerecordTimes );

	if( !m_bMenu )
		OnFullScreenGDI( TRUE );
	dlg.DoModal( m_hWnd );
	if( !m_bMenu )
		OnFullScreenGDI( FALSE );
}

WNDCMD	CMainFrame::OnTapeCommand( WNDCMDPARAM )
{
	if( !Emu.IsRunning() )
		return;

	if( uID == ID_TAPE_STOP ) {
		Emu.Event( CEmuThread::EV_TAPE_STOP );
		return;
	}

	Emu.Pause();

	if( Config.general.bScreenMode && !m_bMenu ) {
		OnFullScreenGDI( TRUE );
	}

	string	pathstr, tempstr;
	if( Config.path.bSavePath ) {
		pathstr = CPathlib::CreatePath( CApp::GetModulePath(), Config.path.szSavePath );
		::CreateDirectory( pathstr.c_str(), NULL );
	} else {
		pathstr = Nes->rom->GetRomPath();
	}
	tempstr = CPathlib::MakePathExt( pathstr.c_str(), Nes->rom->GetRomName(), "vtp" );
	DEBUGOUT( "Path: %s\n", tempstr.c_str() );

	OPENFILENAME	ofn;
	CHAR	szFile[_MAX_PATH];

	::strcpy( szFile, tempstr.c_str() );
	ZEROMEMORY( &ofn, sizeof(ofn) );

	ofn.lStructSize     = sizeof(ofn);
	ofn.hwndOwner       = m_hWnd;
	ofn.lpstrFile       = szFile;
	ofn.lpstrDefExt     = "vtp";
	ofn.nMaxFile        = sizeof(szFile);
	ofn.lpstrFilter     = "Tape Files(*.vtp)\0*.vtp\0All Files(*.*)\0*.*\0";
	ofn.nFilterIndex    = 1;
	ofn.Flags           = OFN_EXPLORER|OFN_PATHMUSTEXIST;
	ofn.lpstrInitialDir = pathstr.c_str();

	CHAR	szTitle[256];

	if( uID == ID_TAPE_PLAY ) {
		CApp::LoadString( IDS_UI_PLAYTAPE, szTitle, sizeof(szTitle) );
		ofn.lpstrTitle = szTitle;
		if( ::GetOpenFileName( &ofn ) ) {
			Emu.EventParam( CEmuThread::EV_TAPE_PLAY, (INT)szFile );
		}
	}
	if( uID == ID_TAPE_REC ) {
		ofn.Flags |= OFN_OVERWRITEPROMPT;
		CApp::LoadString( IDS_UI_RECTAPE, szTitle, sizeof(szTitle) );
		ofn.lpstrTitle = szTitle;
		if( ::GetSaveFileName( &ofn ) ) {
			Emu.EventParam( CEmuThread::EV_TAPE_REC, (INT)szFile );
		}
	}

	if( Config.general.bScreenMode && !m_bMenu ) {
		OnFullScreenGDI( FALSE );
	}

	Emu.Resume();
}

void	CMainFrame::OnFullScreenGDI( BOOL bFlag )
{
	if( Config.general.bScreenMode ) {
		if( Emu.IsRunning() ) {
			Emu.EventParam( CEmuThread::EV_FULLSCREEN_GDI, (INT)bFlag );
		}
	}
}

void	CMainFrame::OnShowCursor( BOOL bFlag )
{
	if( !bFlag ) {
		if( m_bCursor ) {
			while( ::ShowCursor( FALSE ) >= 0 );
			m_bCursor = FALSE;
		}
	} else {
		if( !m_bCursor ) {
			while( ::ShowCursor( TRUE ) < 0 );
			m_bCursor = TRUE;
		}
	}
}

void	CMainFrame::OnChangeMenu( BOOL bFlag )
{
	if( Config.general.bScreenMode ) {
		if( Emu.IsRunning() ) {
			Emu.EventParam( CEmuThread::EV_FULLSCREEN_GDI, (INT)bFlag );
		} else {
			DirectDraw.SetFullScreenGDI( bFlag );
		}
	}

	if( m_bMenu ) {
		if( !bFlag ) {
			::SetMenu( m_hWnd, NULL );
			m_bMenu = FALSE;
		}
	} else {
		if( bFlag ) {
			::SetMenu( m_hWnd, m_hMenu );
			m_bMenu = TRUE;
			::DrawMenuBar( m_hWnd );
		}
	}

//	OnShowCursor( m_bMenu );
}

void	CMainFrame::OnSetWindowSize()
{
	// ウインドウモードの時だけ
	if( Config.general.bScreenMode )
		return;

	LONG	width, height;

	if( !Config.graphics.bAspect )  width  = CDirectDraw::SCREEN_WIDTH;
	else				width  = (LONG)((CDirectDraw::SCREEN_WIDTH)*1.25);
	if( !Config.graphics.bAllLine ) height = CDirectDraw::SCREEN_HEIGHT-16;
	else				height = CDirectDraw::SCREEN_HEIGHT;

	width  *= Config.general.nScreenZoom+1;
	height *= Config.general.nScreenZoom+1;

	RECT	rcW, rcC;
	::GetWindowRect( m_hWnd, &rcW );
	::GetClientRect( m_hWnd, &rcC );

	rcW.right  += width -(rcC.right-rcC.left);
	rcW.bottom += height-(rcC.bottom-rcC.top);
	::SetWindowPos( m_hWnd, HWND_NOTOPMOST, rcW.left, rcW.top,
			rcW.right-rcW.left, rcW.bottom-rcW.top, SWP_NOZORDER );

	// メニューが折り返されると縦が小さくなる事があるので再修正
	::GetClientRect( m_hWnd, &rcC );
	if( (rcC.bottom-rcC.top) != height ) {
		::GetWindowRect( m_hWnd, &rcW );
		::GetClientRect( m_hWnd, &rcC );

		rcW.right  += width -(rcC.right-rcC.left);
		rcW.bottom += height-(rcC.bottom-rcC.top);
		::SetWindowPos( m_hWnd, HWND_NOTOPMOST, rcW.left, rcW.top,
				rcW.right-rcW.left, rcW.bottom-rcW.top, SWP_NOZORDER );
	}


	// 最大表示時にサイズを変更した場合の最大表示の解除及びボタンの再描画
	LONG	style = ::GetWindowLong( m_hWnd, GWL_STYLE );
	style &= ~WS_MAXIMIZE;
	::SetWindowLong( m_hWnd, GWL_STYLE, style );
	::RedrawWindow( m_hWnd, NULL, NULL, RDW_FRAME|RDW_INVALIDATE );

	// 位置保存しなおし
	m_bZoomed = FALSE;
	::GetWindowRect( m_hWnd, &m_WindowRect );
}

void	CMainFrame::OnRebuildMenu()
{
	CHAR	szMenuString[256];
	string	str;
	for( INT i = 0; CConfig::ShortcutKeyID[i*3+0]; i++ ) {
		::GetMenuString( m_hMenu, CConfig::ShortcutKeyID[i*3+0], szMenuString, 256, MF_BYCOMMAND );

		INT	id, code;
		id = CConfig::ShortcutKeyID[i*3+2];
		code = Config.shortcut.nShortCut[id];
		if( code != 0 ) {
			string	temp = szMenuString;
			temp = temp + "\t";
			temp = temp + Config.ShortcutToKeyName( code );

			::ModifyMenu( m_hMenu, CConfig::ShortcutKeyID[i*3+0], MF_BYCOMMAND|MF_STRING,
					CConfig::ShortcutKeyID[i*3+0], temp.c_str() );
		}
	}
}

void	CMainFrame::OnUpdateMenu( HMENU hMenu, UINT uID )
{
	BOOL	bEnable = FALSE;
	BOOL	bCheck = FALSE;
	BOOL	bEmu = Emu.IsRunning();
	BOOL	bScn = Config.general.bScreenMode;

	switch( uID ) {
		case	ID_CLOSE:
		case	ID_ROMINFO:
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|(bEmu?MF_ENABLED:MF_GRAYED) );
			break;

		case	ID_NETPLAY_CONNECT:
			if( bEmu )
				bEnable = !(Nes->IsMoviePlay() || Nes->IsMovieRec() || Nes->rom->IsNSF());
			else
				bEnable = FALSE;
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|((bEnable&&bEmu&&NetPlay.IsNetPlay()&&!NetPlay.IsConnect())?MF_ENABLED:MF_GRAYED) );
			break;
		case	ID_NETPLAY_DISCONNECT:
		case	ID_NETPLAY_CHAT:
			if( bEmu )
				bEnable = !(Nes->IsMoviePlay() || Nes->IsMovieRec() || Nes->rom->IsNSF());
			else
				bEnable = FALSE;
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|((bEnable&&bEmu&&NetPlay.IsNetPlay()&&NetPlay.IsConnect())?MF_ENABLED:MF_GRAYED) );
			break;

		case	ID_WAVERECORD:
			bCheck = bEmu && Emu.IsWaveRecord();
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|(bEmu?MF_ENABLED:MF_GRAYED) );
			::CheckMenuItem( hMenu, uID, MF_BYCOMMAND|(bCheck?MF_CHECKED:MF_UNCHECKED) );
			break;

		case	ID_HWRESET:
		case	ID_SWRESET:
		case	ID_PAUSE:
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|(bEmu?MF_ENABLED:MF_GRAYED) );
			break;
		case	ID_STATE_LOAD:
		case	ID_STATE_SAVE:
			if( Nes ) {
				bEnable = bEmu && !Nes->rom->IsNSF();
			} else {
				bEnable = bEmu;
			}
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|(bEnable?MF_ENABLED:MF_GRAYED) );
			break;

		case	ID_STATE_SLOT0:		case	ID_STATE_SLOT1:
		case	ID_STATE_SLOT2:		case	ID_STATE_SLOT3:
		case	ID_STATE_SLOT4:		case	ID_STATE_SLOT5:
		case	ID_STATE_SLOT6:		case	ID_STATE_SLOT7:
		case	ID_STATE_SLOT8:		case	ID_STATE_SLOT9:
		case	ID_QUICKLOAD_SLOT0:	case	ID_QUICKLOAD_SLOT1:
		case	ID_QUICKLOAD_SLOT2:	case	ID_QUICKLOAD_SLOT3:
		case	ID_QUICKLOAD_SLOT4:	case	ID_QUICKLOAD_SLOT5:
		case	ID_QUICKLOAD_SLOT6:	case	ID_QUICKLOAD_SLOT7:
		case	ID_QUICKLOAD_SLOT8:	case	ID_QUICKLOAD_SLOT9:
		case	ID_QUICKSAVE_SLOT0:	case	ID_QUICKSAVE_SLOT1:
		case	ID_QUICKSAVE_SLOT2:	case	ID_QUICKSAVE_SLOT3:
		case	ID_QUICKSAVE_SLOT4:	case	ID_QUICKSAVE_SLOT5:
		case	ID_QUICKSAVE_SLOT6:	case	ID_QUICKSAVE_SLOT7:
		case	ID_QUICKSAVE_SLOT8:	case	ID_QUICKSAVE_SLOT9:
			// 日時表示(結構面倒ｗ)
			{
			CHAR	szMenuString[256];
			CHAR*	pToken;
			const UCHAR seps[] = " \t\0";	// セパレータ

			// ID番号からインデックスを探す
			for( INT i = 0; CConfig::ShortcutKeyID[i*3+0] != uID; i++ );

			::GetMenuString( m_hMenu, CConfig::ShortcutKeyID[i*3+0], szMenuString, 256, MF_BYCOMMAND );

			if( (pToken = (CHAR*)_mbstok( (UCHAR*)szMenuString, seps )) ) {
				string	str = pToken;

				if( Emu.IsRunning() && Nes ) {
					CHAR	temp[256];
					if( uID >= ID_STATE_SLOT0 && uID <= ID_STATE_SLOT9 )
						::wsprintf( temp, "st%1X", CConfig::ShortcutKeyID[i*3+0]-ID_STATE_SLOT0 );
					else if( uID >= ID_QUICKLOAD_SLOT0 && uID <= ID_QUICKLOAD_SLOT9 )
						::wsprintf( temp, "st%1X", CConfig::ShortcutKeyID[i*3+0]-ID_QUICKLOAD_SLOT0 );
					else
						::wsprintf( temp, "st%1X", CConfig::ShortcutKeyID[i*3+0]-ID_QUICKSAVE_SLOT0 );

					string	pathstr, tempstr;
					if( Config.path.bStatePath ) {
						pathstr = CPathlib::CreatePath( CApp::GetModulePath(), Config.path.szStatePath );
						::CreateDirectory( pathstr.c_str(), NULL );
					} else {
						pathstr = Nes->rom->GetRomPath();
					}
					tempstr = CPathlib::MakePathExt( pathstr.c_str(), Nes->rom->GetRomName(), temp );

					HANDLE	hFile = ::CreateFile( tempstr.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
					if( hFile != INVALID_HANDLE_VALUE ) {
						BY_HANDLE_FILE_INFORMATION fi;
						FILETIME	filetime;
						SYSTEMTIME	systime;
						::GetFileInformationByHandle( hFile, &fi );
						::FileTimeToLocalFileTime( &fi.ftLastWriteTime, &filetime );
						::FileTimeToSystemTime( &filetime, &systime );
						::wsprintf( temp,"%04d/%02d/%02d %02d:%02d:%02d", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond );
						::CloseHandle( hFile );

						str = str + " ";
						str = str + temp;
					} else {
						str = str + " not exist";
					}
				}

				INT	code = Config.shortcut.nShortCut[CConfig::ShortcutKeyID[i*3+2]];
				if( code ) {
					str = str + "\t";
					str = str + Config.ShortcutToKeyName( code );
				}

				::ModifyMenu( m_hMenu, CConfig::ShortcutKeyID[i*3+0], MF_BYCOMMAND|MF_STRING,
						CConfig::ShortcutKeyID[i*3+0], str.c_str() );
			}

			if( !(uID >= ID_STATE_SLOT0 && uID <= ID_STATE_SLOT9) ) {
				::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|(bEmu?MF_ENABLED:MF_GRAYED) );
			}

			if( uID >= ID_STATE_SLOT0 && uID <= ID_STATE_SLOT9 ) {
				::CheckMenuItem( hMenu, uID, MF_BYCOMMAND|(m_nStateSlot==(INT)(uID-ID_STATE_SLOT0)?MF_CHECKED:MF_UNCHECKED) );
			}
			}
			break;

		case	ID_ZOOMx1: case	ID_ZOOMx2:
		case	ID_ZOOMx3: case	ID_ZOOMx4:
			if( bScn ) ::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|MF_GRAYED );
			else	   ::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|MF_ENABLED );
			::CheckMenuItem( hMenu, uID, MF_BYCOMMAND|(Config.general.nScreenZoom==(uID-ID_ZOOMx1)?MF_CHECKED:MF_UNCHECKED) );
			break;

		case	ID_FILTER_NONE:
		case	ID_FILTER_2XSAI:
		case	ID_FILTER_SUPER2XSAI:
		case	ID_FILTER_SUPEREAGLE:
		case	ID_FILTER_SCALE2X:
			::CheckMenuItem( hMenu, uID, MF_BYCOMMAND|(Config.graphics.nGraphicsFilter==(uID-ID_FILTER_NONE)?MF_CHECKED:MF_UNCHECKED) );
			break;

		case	ID_DISK_EJECT:
		case	ID_DISK_0A: case	ID_DISK_0B:
		case	ID_DISK_1A: case	ID_DISK_1B:
			if( Nes ) {
				INT no = Nes->GetDiskNo();
				bEnable = bEmu && !(Nes->rom->GetMapperNo() != 20 || Nes->rom->IsNSF() || (!no || (uID-ID_DISK_EJECT)>no));
			} else {
				bEnable = bEmu;
			}
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|(bEnable?MF_ENABLED:MF_GRAYED) );
			break;

		case	ID_MOVIE_PLAY:
		case	ID_MOVIE_REC:
		case	ID_MOVIE_REC_APPEND:
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|((bEmu&&!NetPlay.IsConnect())?MF_ENABLED:MF_GRAYED) );
			break;

		case	ID_MOVIE_STOP:
		case	ID_MOVIE_INFO:
			if( Nes ) {
				bEnable = Nes->IsMoviePlay() || Nes->IsMovieRec();
			} else {
				bEnable = bEmu;
			}
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|(bEnable?MF_ENABLED:MF_GRAYED) );
			break;

		case	ID_TAPE_PLAY:
		case	ID_TAPE_REC:
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|((bEmu&&!NetPlay.IsConnect())?MF_ENABLED:MF_GRAYED) );
			break;
		case	ID_TAPE_STOP:
			if( Nes ) {
				bEnable = Nes->IsTapePlay() || Nes->IsTapeRec();
			} else {
				bEnable = bEmu;
			}
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|(bEnable?MF_ENABLED:MF_GRAYED) );
			break;

		case	ID_BARCODEBATTLER:
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|((bEmu&&!NetPlay.IsConnect())?MF_ENABLED:MF_GRAYED) );
			break;

		case	ID_VSUNISYSTEM_DIPSWITCH:
			bEnable = bEmu &&
				( (Nes->pad->GetExController() == PAD::EXCONTROLLER_VSUNISYSTEM)
				||(Nes->pad->GetExController() == PAD::EXCONTROLLER_VSZAPPER) );
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|((bEmu&&!NetPlay.IsConnect())?MF_ENABLED:MF_GRAYED) );
			break;

		case	ID_SNAPSHOT:
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|(bEmu?MF_ENABLED:MF_GRAYED) );
			break;

		case	ID_EXCTR_NONE:
		case	ID_EXCTR_PADDLE:
		case	ID_EXCTR_HYPERSHOT:
		case	ID_EXCTR_ZAPPER:
		case	ID_EXCTR_KEYBOARD:
		case	ID_EXCTR_SUBOR_KEYBOARD:
		case	ID_EXCTR_CRAZYCLIMBER:
		case	ID_EXCTR_TOPRIDER:
		case	ID_EXCTR_SPACESHADOWGUN:
		case	ID_EXCTR_FAMILYTRAINER_A:
		case	ID_EXCTR_FAMILYTRAINER_B:
		case	ID_EXCTR_EXCITINGBOXING:
		case	ID_EXCTR_MAHJANG:
		case	ID_EXCTR_OEKAKIDS_TABLET:
		case	ID_EXCTR_TURBOFILE:
		case	ID_EXCTR_CHINA_EDUCATIONAL_MOUSE:
		case	ID_EXCTR_VSUNISYSTEM:
		case	ID_EXCTR_VSUNISYSTEM_ZAPPER:
		{
			UINT tuID = uID;
			if(uID>=ID_EXCTR_CRAZYCLIMBER){
				uID++;
			}else if(uID==ID_EXCTR_SUBOR_KEYBOARD){
				uID=ID_EXCTR_CRAZYCLIMBER;
			}
			if( Nes ) {
				if( Nes->pad->GetExController() == (uID-ID_EXCTR_NONE) )
					bCheck = TRUE;
				else
					bCheck = FALSE;
			} else {
				if( uID == ID_EXCTR_NONE )
					bCheck = TRUE;
				else
					bCheck = FALSE;
			}
			::EnableMenuItem( hMenu, tuID, MF_BYCOMMAND|(bEmu?MF_ENABLED:MF_GRAYED) );
			::CheckMenuItem( hMenu, tuID, MF_BYCOMMAND|(bCheck?MF_CHECKED:MF_UNCHECKED) );
			break;

		}
		case	ID_TURBOFILE_BANK0:
		case	ID_TURBOFILE_BANK1:
		case	ID_TURBOFILE_BANK2:
		case	ID_TURBOFILE_BANK3:
			if( Nes ) {
				bEnable = bEmu && (Nes->pad->GetExController() == PAD::EXCONTROLLER_TURBOFILE);
				if( bEnable ) {
					if( Nes->GetTurboFileBank() == (uID-ID_TURBOFILE_BANK0) )
						bCheck = TRUE;
					else
						bCheck = FALSE;
				}
			}
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|(bEnable?MF_ENABLED:MF_GRAYED) );
			::CheckMenuItem( hMenu, uID, MF_BYCOMMAND|(bCheck?MF_CHECKED:MF_UNCHECKED) );
			break;

		case	ID_FULLSCREEN:
			::CheckMenuItem( hMenu, uID, MF_BYCOMMAND|(bScn?MF_CHECKED:MF_UNCHECKED) );
			break;
		case	ID_FPSDISP:
			::CheckMenuItem( hMenu, uID, MF_BYCOMMAND|(Config.graphics.bFPSDisp?MF_CHECKED:MF_UNCHECKED) );
			break;
		case	ID_TVFRAME:
			::CheckMenuItem( hMenu, uID, MF_BYCOMMAND|(Config.graphics.bTVFrame?MF_CHECKED:MF_UNCHECKED) );
			break;

		case	ID_CFG_GAMEOPTION:
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|(bEmu?MF_ENABLED:MF_GRAYED) );
			break;

		case	ID_SEARCH:
		case	ID_CHEAT:
		case	ID_CHEAT_ENABLE:
		case	ID_CHEAT_DISABLE:
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|(bEmu?MF_ENABLED:MF_GRAYED) );
			break;
#if	0
		case	ID_CHEAT:
			if( m_SearchDlg.m_hWnd )
				bEnable = bEmu && !::IsWindowVisible( m_SearchDlg.m_hWnd );
			else
				bEnable = bEmu;
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|(bEnable?MF_ENABLED:MF_GRAYED) );
			break;
#endif
		case	ID_GENIE:
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|(bEmu?MF_ENABLED:MF_GRAYED) );
			break;

		case	ID_VIEW_PATTERN:
		case	ID_VIEW_NAMETABLE:
		case	ID_VIEW_PALETTE:
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|(bEmu?MF_ENABLED:MF_GRAYED) );
			break;

		case	ID_VIEW_MEMORY:
		case	ID_VIEW_WATCH:
			::EnableMenuItem( hMenu, uID, MF_BYCOMMAND|(bEmu?MF_ENABLED:MF_GRAYED) );
			break;

		default:
			break;
	}
}

void	CMainFrame::OnKeyControl()
{
BYTE	KeyInp[256+64*8];

	// 20ms未満では受け付けないようにする
	if( (::timeGetTime()-m_dwKeyTime) < 20 )
		return;
	m_dwKeyTime = ::timeGetTime();

	::memset( KeyInp, 0x00, sizeof(KeyInp) );

	if( !m_bKeyEnable ) {
		// トリガを発生させないようにする為
		::memset( m_KeyBuf, 0x80, sizeof(m_KeyBuf) );
//DEBUGOUT( "Key Disable\n" );
		return;
	}

	// トリガ＆リピート生成
	BYTE*	pSw  = (BYTE*)DirectInput.m_Sw;
	BYTE*	pOld = (BYTE*)m_KeyBuf;
	BYTE*	pCnt = (BYTE*)m_KeyCnt;
	BYTE*	pKey = (BYTE*)KeyInp;

	for( INT i = 0; i < 256+64*8; i++ ) {
		pKey[i] = (pSw[i]&0x80) & ~(pOld[i]&0x80);
		if( pKey[i] ) {
			pCnt[i] = 14;
		} else if( pCnt[i] != 0 && (pSw[i]&0x80) ) {
			if( --pCnt[i] == 0 ) {
				pKey[i] = pSw[i]&0x80;
				pCnt[i] = 5;
			} else {
				pKey[i] = 0;
			}
		}
	}
	::memcpy( m_KeyBuf, DirectInput.m_Sw, sizeof(DirectInput.m_Sw) );

//DEBUGOUT( "LMENU:%02X RMENU:%02X ENTER:%02X\n", m_KeyBuf[DIK_LMENU], m_KeyBuf[DIK_RMENU], KeyInp[DIK_RETURN] );

	// イベント発信
	BOOL	bAltOnly = FALSE;
	if( Emu.IsRunning() ) {
		// ファミリーベーシックキーボードの時
		if( Emu.GetExController() == PAD::EXCONTROLLER_KEYBOARD ||
			Emu.GetExController() == PAD::EXCONTROLLER_Subor_KEYBOARD ||
			Emu.GetExController() == PAD::EXCONTROLLER_PEC_KEYBOARD ||
			Emu.GetExController() == PAD::EXCONTROLLER_Kingwon_KEYBOARD ||
			Emu.GetExController() == PAD::EXCONTROLLER_ZeCheng_KEYBOARD ||
			Emu.GetExController() == PAD::EXCONTROLLER_CHINA_EDUCATIONAL_MOUSE ||
			Emu.GetNES()->rom->GetMapperNo() == 169 )	//YuXing
			bAltOnly = TRUE;
		if( Emu.IsPausing() )
			bAltOnly = FALSE;
	}

	WORD	keyex = 0;
	if( m_KeyBuf[DIK_LMENU] || m_KeyBuf[DIK_RMENU] )
		keyex = CCfgShortCut::K_ALT;
	if( m_KeyBuf[DIK_LCONTROL] || m_KeyBuf[DIK_RCONTROL] )
		keyex = CCfgShortCut::K_CTRL;
	if( m_KeyBuf[DIK_LSHIFT] || m_KeyBuf[DIK_RSHIFT] )
		keyex = CCfgShortCut::K_SHIFT;

	WORD*	pShortCutKey = Config.shortcut.nShortCut;
	INT*	pShortCutKeyID = Config.ShortcutKeyID;

	if( bAltOnly ) {
		INT	no;
		WORD	key, key2;
		for( INT i = 0; pShortCutKeyID[i*3+0]; i++ ) {
			no = pShortCutKeyID[i*3+2];
			key = pShortCutKey[no];
			key2 = pShortCutKey[no+128];

			if( (key & 0xF000) == CCfgShortCut::K_ALT && keyex == CCfgShortCut::K_ALT && KeyInp[key&0x0FFF] && (key&0x0FFF) ) {
				::PostMessage( CApp::GetHWnd(), WM_COMMAND, (WPARAM)pShortCutKeyID[i*3+0], (LPARAM)0 );
			}
			if( (key2 & 0xF000) == CCfgShortCut::K_ALT && keyex == CCfgShortCut::K_ALT && KeyInp[key2&0x0FFF] && (key2&0x0FFF) ) {
				::PostMessage( CApp::GetHWnd(), WM_COMMAND, (WPARAM)pShortCutKeyID[i*3+0], (LPARAM)0 );
			}
		}
	} else {
		INT	no;
		WORD	key, key2;
		for( INT i = 0; pShortCutKeyID[i*3+0]; i++ ) {
			no = pShortCutKeyID[i*3+2];
			key = pShortCutKey[no];
			key2 = pShortCutKey[no+128];
			if( (key & 0xF000) == keyex && KeyInp[key&0x0FFF] && (key&0x0FFF) ) {
				::PostMessage( CApp::GetHWnd(), WM_COMMAND, (WPARAM)pShortCutKeyID[i*3+0], (LPARAM)0 );
			}
			if( (key2 & 0xF000) == keyex && KeyInp[key2&0x0FFF] && (key2&0x0FFF) ) {
				::PostMessage( CApp::GetHWnd(), WM_COMMAND, (WPARAM)pShortCutKeyID[i*3+0], (LPARAM)0 );
			}
		}
	}
}

void _cdecl CMainFrame::KeyThreadProc( LPVOID lpParam )
{
	while( !m_bKeyThreadExit ) {
		::Sleep( 20 );

		// エミュレーション中以外こっちでやらない
		if( Emu.IsRunning() ) {
			continue;
		} else {
			DirectInput.Poll();
			OnKeyControl();
		}
	}
	_endthread();
}

