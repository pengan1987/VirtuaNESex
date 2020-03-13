//
// メインウインドウクラス
//
#ifndef	__CMAINFRAME_INCLUDED__
#define	__CMAINFRAME_INCLUDED__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
using namespace std;

#include "Wnd.h"
#include "EmuThread.h"
#include "LauncherDlg.h"
#include "CheatDlg.h"
#include "PatternView.h"
#include "NameTableView.h"
#include "PaletteView.h"
#include "MemoryView.h"
#include "DatachBarcodeDlg.h"
#include "ChatDlg.h"
#include "PaletteEdit.h"
#include "DipSwitchDlg.h"

class	CMainFrame : public CWnd
{
public:
	// Override from CWnd
	BOOL	Create( HWND hWndParent );
	void	Destroy();

	// Override from CWnd
	BOOL	PreTranslateMessage( MSG* pMsg );

	static	void	OnKeyControl();
protected:
	void	OnEmulationStart( LPCSTR szFile, BOOL bChecked );

	void	OnFullScreenGDI( BOOL bFlag );
	void	OnShowCursor( BOOL bFlag );
	void	OnChangeMenu( BOOL bFlag );
	void	OnSetWindowSize();
	void	OnRebuildMenu();
	void	OnUpdateMenu( HMENU hMenu, UINT uID );

	static	void _cdecl KeyThreadProc( LPVOID lpParam );

	// Message map
	WND_MESSAGE_MAP()
	WNDMSG		OnCreate( WNDMSGPARAM );
	WNDMSG		OnClose( WNDMSGPARAM );
	WNDMSG		OnDestroy( WNDMSGPARAM );
	WNDMSG		OnActivate( WNDMSGPARAM );
	WNDMSG		OnActivateApp( WNDMSGPARAM );
	WNDMSG		OnShortCutEnable( WNDMSGPARAM );	// Private message
	WNDMSG		OnEnable( WNDMSGPARAM );
	WNDMSG		OnEnterMenuLoop( WNDMSGPARAM );
	WNDMSG		OnExitMenuLoop( WNDMSGPARAM );
	WNDMSG		OnSetFocus( WNDMSGPARAM );
	WNDMSG		OnKillFocus( WNDMSGPARAM );
	WNDMSG		OnInitMenu( WNDMSGPARAM );
	WNDMSG		OnInitMenuPopup( WNDMSGPARAM );
	WNDMSG		OnGetMinMaxInfo( WNDMSGPARAM );
	WNDMSG		OnPaint( WNDMSGPARAM );
	WNDMSG		OnDisplayChange( WNDMSGPARAM );
	WNDMSG		OnPaletteChanged( WNDMSGPARAM );
	WNDMSG		OnQueryNewPalette( WNDMSGPARAM );
	WNDMSG		OnMenuChar( WNDMSGPARAM );
	WNDMSG		OnKeyDown( WNDMSGPARAM );
	WNDMSG		OnSize( WNDMSGPARAM );
	WNDMSG		OnSysCommand( WNDMSGPARAM );
	WNDMSG		OnExitSizeMove( WNDMSGPARAM );

	WNDMSG		OnSetCursor( WNDMSGPARAM );
	WNDMSG		OnTimer( WNDMSGPARAM );

	WNDMSG		OnCopyData( WNDMSGPARAM );

	WNDMSG		OnErrorMessage( WNDMSGPARAM );		// Private message

	WNDMSG		OnDropFiles( WNDMSGPARAM );
	WNDMSG		OnCommandLine( WNDMSGPARAM );		// Private message
	WNDMSG		OnLauncherCommand( WNDMSGPARAM );	// Private message

	WNDMSG		OnNetPlay( WNDMSGPARAM );
	WNDMSG		OnNetPlayClose( WNDMSGPARAM );
	WNDMSG		OnNetPlayError( WNDMSGPARAM );
	WNDMSG		OnNetPlayChatPopup( WNDMSGPARAM );

	WNDCMD		OnExit( WNDCMDPARAM );
	WNDCMD		OnHelp( WNDCMDPARAM );
	WNDCMD		OnAbout( WNDCMDPARAM );
	WNDCMD		OnFileOpen( WNDCMDPARAM );
	WNDCMD		OnFileClose( WNDCMDPARAM );
	WNDCMD		OnRecentOpen( WNDCMDPARAM );
	WNDCMD		OnRecentOpenPath( WNDCMDPARAM );
	WNDCMD		OnRomInfo( WNDCMDPARAM );
	WNDCMD		OnWaveRecord( WNDCMDPARAM );
	WNDCMD		OnLauncher( WNDCMDPARAM );

	WNDCMD		OnSearch( WNDCMDPARAM );
	WNDCMD		OnCheat( WNDCMDPARAM );
	WNDCMD		OnGenie( WNDCMDPARAM );
	WNDCMD		OnCheatCommand( WNDCMDPARAM );

	WNDCMD		OnDatachBacode( WNDCMDPARAM );
	WNDCMD		OnDipSwitch( WNDCMDPARAM );

	WNDCMD		OnNetPlayConnect( WNDCMDPARAM );
	WNDCMD		OnNetPlayDisconnect( WNDCMDPARAM );
	WNDCMD		OnNetPlayChat( WNDCMDPARAM );

	WNDCMD		OnEmulatorCfg( WNDCMDPARAM );
	WNDCMD		OnGraphicsCfg( WNDCMDPARAM );
	WNDCMD		OnSoundCfg( WNDCMDPARAM );
	WNDCMD		OnControllerCfg( WNDCMDPARAM );
	WNDCMD		OnShortcutCfg( WNDCMDPARAM );
	WNDCMD		OnFolderCfg( WNDCMDPARAM );
	WNDCMD		OnLanguageCfg( WNDCMDPARAM );
	WNDCMD		OnMovieCfg( WNDCMDPARAM );
	WNDCMD		OnGameOptionCfg( WNDCMDPARAM );
	WNDCMD		OnJoyAxisCfg( WNDCMDPARAM );
	WNDCMD		OnPaletteEditCfg( WNDCMDPARAM );
	WNDCMD		OnFullScreen( WNDCMDPARAM );
	WNDCMD		OnZoom( WNDCMDPARAM );

	WNDCMD		OnViewCommand( WNDCMDPARAM );

	WNDCMD		OnEmuCommand( WNDCMDPARAM );

	WNDCMD		OnStateCommand( WNDCMDPARAM );
	WNDCMD		OnStateCommand2( WNDCMDPARAM );	// For QuickLoad/Save
	WNDCMD		OnMovieCommand( WNDCMDPARAM );
	WNDCMD		OnMovieInfo( WNDCMDPARAM );
	WNDCMD		OnTapeCommand( WNDCMDPARAM );

	// Command line
	CHAR	m_szCommandLine[_MAX_PATH];

	// Window activate flags
	BOOL	m_bActivate;
	BOOL	m_bActivateApp;
	BOOL	m_bForcus;

	BOOL	m_bEnable;
	static	BOOL	m_bKeyEnable;

	// Cursor
	BOOL	m_bCursor;
	UINT	m_uTimerID;
	DWORD	m_LastMovedTime;

	// Menu flags
	BOOL	m_bMenu;

	// Window Position
	BOOL	m_bZoomed;
	RECT	m_WindowRect;
	WINDOWPLACEMENT	m_WindowPlacement;

	// Window Style
	LONG	m_ExStyleBackup;
	LONG	m_StyleBackup;

	// Short Cut
	UINT	m_uKeyTimerID;
	volatile BOOL	m_bKeyChecking;

	static	DWORD	m_dwKeyTime;
	static	BYTE	m_KeyBuf[256+64*8];
	static	BYTE	m_KeyOld[256+64*8];
	static	BYTE	m_KeyCnt[256*64*8];
	static	volatile BOOL	m_bKeyThreadExit;

	// Emulator
	NES*		Nes;
//	CEmuThread	Emu;

	// State slot
	INT	m_nStateSlot;

	// Launcher
	CLauncherDlg	m_LauncherDlg;

	// Cheat
	CSearchDlg	m_SearchDlg;

	// Viewer
	CPatternView	m_PatternView;
	CNameTableView	m_NameTableView;
	CPaletteView	m_PaletteView;

	CMemoryView	m_MemoryView;

	// Palette Editor
	CPaletteEdit	m_PaletteEdit;

	// Cheat
	CDatachBarcodeDlg m_DatachBarcodeDlg;

	// Chat
	CChatDlg	m_ChatDlg;
private:
};

#endif	// !__CMAINFRAME_INCLUDED__

