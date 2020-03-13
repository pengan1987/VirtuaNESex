//
// ランチャーダイアログクラス
//
#ifndef	__CLAUNCHERDLG_INCLUDED__
#define	__CLAUNCHERDLG_INCLUDED__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <list>
#include <vector>
#include <string>
using namespace std;

#include "Wnd.h"

class	FILELIST
{
public:
	string	fname;
	string	path;
	INT	mapper;
	INT	prg_size;
	INT	chr_size;
	DWORD	crcall;
	DWORD	crc;
	string	info;
	string	db;
	string	title;
	string	country;
	string	manufacturer;
	string	saledate;
	string	price;
	string	genre;
};

class	CLauncherDlg : public CWnd
{
public:
	// Override from CWnd
	BOOL	Create( HWND hWndParent );
	void	Destroy();

	// Table
	static	INT	m_HeaderID[];
protected:
	enum	{
		COLUMN_FILENAME = 0,
		COLUMN_PATH,
		COLUMN_MAPPER,
		COLUMN_PRG,
		COLUMN_CHR,
		COLUMN_ALLCRC,
		COLUMN_PRGCRC,
		COLUMN_INFO,
		COLUMN_DB,
		COLUMN_TITLE,
		COLUMN_COUNTRY,
		COLUMN_MANUFACTURER,
		COLUMN_SALEDATE,
		COLUMN_PRICE,
		COLUMN_GENRE,
	};

	static	INT CALLBACK	ListViewCompare( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort );

	void	ResetListViewHeader();
	void	ResetFileList();
	void	ResetListView();
	void	SetListView( INT index, FILELIST& fl );
	void	SortListView();

	void	SetLastSelect();

	void	OnUpdateStart();
	void	OnUpdateStop();
	void	UpdateListView();

	void	CheckFile( FILELIST& fl );

	void	OnUpdateMenu( HMENU hMenu, UINT uID );

	BOOL	LoadFileList();
	void	SaveFileList();

	// Message map
	DLG_MESSAGE_MAP()
	DLGMSG		OnInitDialog( DLGMSGPARAM );
	DLGMSG		OnDestroy( DLGMSGPARAM );
	DLGMSG		OnClose( DLGMSGPARAM );
	DLGMSG		OnActivate( DLGMSGPARAM );
	DLGMSG		OnSetCursor( DLGMSGPARAM );
	DLGMSG		OnSize( DLGMSGPARAM );
	DLGMSG		OnTimer( DLGMSGPARAM );
	DLGMSG		OnInitMenuPopup( DLGMSGPARAM );

	DLGCMD		OnOK( DLGCMDPARAM );
	DLGCMD		OnCancel( DLGCMDPARAM );

	DLGCMD		OnListSelect( DLGCMDPARAM );

	DLGCMD		OnRefresh( DLGCMDPARAM );

	DLGCMD		OnDispEdit( DLGCMDPARAM );
	DLGCMD		OnFolder( DLGCMDPARAM );
	DLGCMD		OnHeaderEdit( DLGCMDPARAM );

	DLGNOTIFY	OnKeyDownListView( DLGNOTIFYPARAM );
	DLGNOTIFY	OnReturnListView( DLGNOTIFYPARAM );
	DLGNOTIFY	OnDoubleClickListView( DLGNOTIFYPARAM );
	DLGNOTIFY	OnColumnClickListView( DLGNOTIFYPARAM );
	DLGNOTIFY	OnItemChangedListView( DLGNOTIFYPARAM );
	//

	// Image List
	HIMAGELIST	m_hImageList;
	HIMAGELIST	m_hImageListHdr;	// sort

	//
	BOOL	m_bFileLoaded;

	// ランチャーリスト番号
	INT	m_nListSelect;

	INT	m_nSortType;
	INT	m_SelectPos;

	INT	m_UpdatePos;
	BOOL	m_bUpdate;
	volatile BOOL	m_bUpdating;
	INT	m_nTimerID;
	INT	m_nUpdateIndex;

	// Sort type
	static	BOOL	m_bSortDir;

	// File list
	static	INT		m_FileListNum;
	static vector<FILELIST>	m_FileList;

	// Path
	static	CHAR		m_LaunchPath[_MAX_PATH];
private:
};

class	CLchDispEditDlg : public CWnd
{
public:
	// Override from CWnd
	INT	DoModal( HWND hWndParent );

protected:
	// Message map
	DLG_MESSAGE_MAP()
	DLGMSG		OnInitDialog( DLGMSGPARAM );
	DLGCMD		OnOK( DLGCMDPARAM );
	DLGCMD		OnCancel( DLGCMDPARAM );
	DLGCMD		OnAdd( DLGCMDPARAM );
	DLGCMD		OnDel( DLGCMDPARAM );
	DLGCMD		OnUp( DLGCMDPARAM );
	DLGCMD		OnDown( DLGCMDPARAM );
	//

	// Temp
	INT	m_nViewOrder[16];
	INT	m_nViewNum;
	INT	m_nHideOrder[16];
	INT	m_nHideNum;
private:
};

class	CLchFolderConfigDlg : public CWnd
{
public:
	// Override from CWnd
	INT	DoModal( HWND hWndParent );

protected:
	// Message map
	DLG_MESSAGE_MAP()
	DLGMSG		OnInitDialog( DLGMSGPARAM );
	DLGCMD		OnOK( DLGCMDPARAM );
	DLGCMD		OnCancel( DLGCMDPARAM );
	DLGCMD		OnAdd( DLGCMDPARAM );
	DLGCMD		OnDel( DLGCMDPARAM );
	//
private:
};

class	CLchHeaderEditDlg : public CWnd
{
public:
	// Override from CWnd
	INT	DoModal( HWND hWndParent );

	INT	m_nMapperNo;
	BOOL	m_bMirror;
	BOOL	m_bSram;
	BOOL	m_bTrainer;
	BOOL	m_bFourScreen;
	BOOL	m_bVSUnisystem;

protected:
	// Message map
	DLG_MESSAGE_MAP()
	DLGMSG		OnInitDialog( DLGMSGPARAM );
	DLGCMD		OnOK( DLGCMDPARAM );
	DLGCMD		OnCancel( DLGCMDPARAM );
	//
private:
};

#endif	// !__CLAUNCHERDLG_INCLUDED__

