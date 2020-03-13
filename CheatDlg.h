//
// チートダイアログクラス
//
#ifndef	__CCHEATDLG_INCLUDED__
#define	__CCHEATDLG_INCLUDED__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <list>
#include <vector>
#include <string>
using namespace std;

#include "Wnd.h"
#include "cheat.h"

// サーチダイアログ
class	CSearchDlg : public CWnd
{
public:
	// Override from CWnd
	BOOL	Create( HWND hWndParent );
	void	Destroy();

protected:
	void	OnListUpdate();

	// Utils
	DWORD	GetNesMemory( INT length, DWORD addr );
	DWORD	GetSearchMemory( INT length, DWORD addr );
	DWORD	GetSearchMemoryOld( INT length, DWORD addr );

	BOOL	CompareData( INT type, DWORD dataA, DWORD dataB );
	BOOL	CompareRange( INT length, DWORD dataA, DWORD dataB, DWORD range );

	// Message map
	DLG_MESSAGE_MAP()
	DLGMSG		OnInitDialog( DLGMSGPARAM );
	DLGMSG		OnActivate( DLGMSGPARAM );
	DLGMSG		OnClose( DLGMSGPARAM );

	DLGMSG		OnContextMenu( DLGMSGPARAM );

	DLGCMD		OnOK( DLGCMDPARAM );
	DLGCMD		OnCancel( DLGCMDPARAM );

	DLGCMD		OnRadixCommand( DLGCMDPARAM );
	DLGCMD		OnLengthCommand( DLGCMDPARAM );

	DLGCMD		OnStart( DLGCMDPARAM );
	DLGCMD		OnUpdate( DLGCMDPARAM );
	DLGCMD		OnUndo( DLGCMDPARAM );

	DLGCMD		OnSearchCommand( DLGCMDPARAM );
	DLGCMD		OnSearchData( DLGCMDPARAM );
	DLGCMD		OnWriteData( DLGCMDPARAM );

	DLGCMD		OnCodeAppend( DLGCMDPARAM );
	//
	DLGNOTIFY	OnDoubleClickListView( DLGNOTIFYPARAM );

private:
	HMENU	m_hMenu;
	HMENU	m_hSubMenu;

	BOOL	m_bShortCutDisable;

	WORD	m_Address;

	INT	m_nRadix;
	INT	m_nLength;

	struct	RESULT {
		BYTE	RAM_N[0x0800];	// RAM New value
		BYTE	RAM_O[0x0800];	// RAM Old value
		BYTE	RAM_F[0x0800];	// RAM Flag

		BYTE	SRAM_N[0x2000];	// RAM New value
		BYTE	SRAM_O[0x2000];	// RAM Old value
		BYTE	SRAM_F[0x2000];	// RAM Flag
	};

	struct	RESULT	m_Result;	// 今回データ
	struct	RESULT	m_ResultOld;	// １回前
};

// チートコード編集ダイアログ
class	CCheatCodeEditDlg : public CWnd
{
public:
	// Override from CWnd
	INT	DoModal( HWND hWndParent );

	// 
	CHEATCODE	m_Code;
	INT		m_nRadix;

protected:
	// Message map
	DLG_MESSAGE_MAP()
	DLGMSG		OnInitDialog( DLGMSGPARAM );
	DLGCMD		OnOK( DLGCMDPARAM );
	DLGCMD		OnCancel( DLGCMDPARAM );
	//
private:
};

// チートコード入力ダイアログ
class	CCheatCodeInputDlg : public CWnd
{
public:
	// Override from CWnd
	INT	DoModal( HWND hWndParent );

	string	m_Codes;
	string	m_Comment;

protected:
	// Message map
	DLG_MESSAGE_MAP()
	DLGMSG		OnInitDialog( DLGMSGPARAM );
	DLGCMD		OnOK( DLGCMDPARAM );
	DLGCMD		OnCancel( DLGCMDPARAM );
	//
private:
};

// チートコードダイアログ
class	CCheatCodeDlg : public CWnd
{
public:
	// Override from CWnd
	INT	DoModal( HWND hWndParent );

protected:
	void	OnListUpdate();

	// Message map
	DLG_MESSAGE_MAP()
	DLGMSG		OnInitDialog( DLGMSGPARAM );
	DLGMSG		OnDestroy( DLGMSGPARAM );
	DLGMSG		OnTimer( DLGMSGPARAM );

	DLGNOTIFY	OnKeyDownListView( DLGNOTIFYPARAM );
	DLGNOTIFY	OnClickListView( DLGNOTIFYPARAM );
	DLGNOTIFY	OnDblClkListView( DLGNOTIFYPARAM );

	DLGCMD		OnOK( DLGCMDPARAM );
	DLGCMD		OnCancel( DLGCMDPARAM );

	DLGCMD		OnEnable( DLGCMDPARAM );
	DLGCMD		OnDisable( DLGCMDPARAM );
	DLGCMD		OnClear( DLGCMDPARAM );
	DLGCMD		OnRemove( DLGCMDPARAM );

	DLGCMD		OnInput( DLGCMDPARAM );
	DLGCMD		OnEdit( DLGCMDPARAM );

	DLGCMD		OnLoad( DLGCMDPARAM );
	DLGCMD		OnSave( DLGCMDPARAM );

	// Image List
	HIMAGELIST	m_hImageList;

	// Timer
	UINT	m_uTimerID;

	// Temp buffer
	vector<CHEATCODE>	m_CheatCode;

private:
};

#endif	// !__CCHEATDLG_INCLUDED__

