//
// ÉâÉìÉ`ÉÉÅ[É_ÉCÉAÉçÉOÉNÉâÉX
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <mbstring.h>
#include <shlwapi.h>

#include <list>
#include <vector>
#include <string>
using namespace std;

#include "typedef.h"
#include "macro.h"

#include "VirtuaNESres.h"
#include "DebugOut.h"
#include "App.h"
#include "Pathlib.h"
#include "Crclib.h"
#include "Config.h"
#include "Archive.h"

#include "Wnd.h"
#include "LauncherDlg.h"

#include "rom.h"
#include "romdb.h"

#include "EmuThread.h"

// ÉÅÉbÉZÅ[ÉW
DLG_MESSAGE_BEGIN(CLauncherDlg)
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )
DLG_ON_MESSAGE( WM_DESTROY,	OnDestroy )
DLG_ON_MESSAGE( WM_CLOSE,	OnClose )
DLG_ON_MESSAGE( WM_ACTIVATE,	OnActivate )
DLG_ON_MESSAGE( WM_SETCURSOR,	OnSetCursor )
DLG_ON_MESSAGE( WM_SIZE,	OnSize )
DLG_ON_MESSAGE( WM_TIMER,	OnTimer )
DLG_ON_MESSAGE( WM_INITMENUPOPUP, OnInitMenuPopup )
// ÉRÉ}ÉìÉh
DLG_COMMAND_BEGIN()
DLG_ON_COMMAND( IDOK, OnOK )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_ON_COMMAND( ID_LCH_REFRESH, OnRefresh )
DLG_ON_COMMAND( ID_LCH_DISPEDIT, OnDispEdit )
DLG_ON_COMMAND( ID_LCH_HEADEREDIT, OnHeaderEdit )
DLG_ON_COMMAND( ID_LCH_FOLDER, OnFolder )
DLG_ON_COMMAND_RANGE( ID_LCH_LIST0, ID_LCH_LIST9, OnListSelect )
DLG_COMMAND_END()
// Notify ÉÅÉbÉZÅ[ÉW
DLG_NOTIFY_BEGIN()
DLG_ON_NOTIFY( IDC_LCH_LIST, LVN_KEYDOWN, OnKeyDownListView )
DLG_ON_NOTIFY( IDC_LCH_LIST, NM_RETURN, OnReturnListView )
DLG_ON_NOTIFY( IDC_LCH_LIST, NM_DBLCLK, OnDoubleClickListView )
DLG_ON_NOTIFY( IDC_LCH_LIST, LVN_COLUMNCLICK, OnColumnClickListView )
DLG_ON_NOTIFY( IDC_LCH_LIST, LVN_ITEMCHANGED, OnItemChangedListView )
DLG_NOTIFY_END()
DLG_MESSAGE_END()

// ÉXÉ^ÉeÉBÉbÉNÉÅÉìÉo
BOOL	CLauncherDlg::m_bSortDir = FALSE;

INT		CLauncherDlg::m_FileListNum = 0;
vector<FILELIST> CLauncherDlg::m_FileList;
CHAR		CLauncherDlg::m_LaunchPath[_MAX_PATH];

#define	LAUNCHHEADER_MAX	15

INT	CLauncherDlg::m_HeaderID[] = {
	IDS_LCH_FILENAME,
	IDS_LCH_PATH,
	IDS_LCH_MAPPER,
	IDS_LCH_PRG,
	IDS_LCH_CHR,
	IDS_LCH_ALLCRC,
	IDS_LCH_PRGCRC,
	IDS_LCH_INFO,
	IDS_LCH_DATABASE,
	IDS_LCH_TITLE,
	IDS_LCH_COUNTRY,
	IDS_LCH_MANUFACTURER,
	IDS_LCH_SALEDATE,
	IDS_LCH_PRICE,
	IDS_LCH_GENRE,
};

BOOL	CLauncherDlg::Create( HWND hWndParent )
{
	m_hImageList = NULL;
	m_hImageListHdr = NULL;

	// êeÇÕÉfÉXÉNÉgÉbÉvÇ…Ç∑ÇÈ
	m_hWnd = ::CreateDialogParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_LAUNCHER),
				NULL, g_DlgProc, (LPARAM)this );
	if( !m_hWnd )
		return	FALSE;

	// ÉÇÅ[ÉhÉåÉXÉ_ÉCÉAÉçÉOÉäÉXÉgÇ…â¡Ç¶ÇÈ
	CWndList::Add( this );

	return	TRUE;
}

void	CLauncherDlg::Destroy()
{
	if( m_hWnd ) {
		// ÉÇÅ[ÉhÉåÉXÉ_ÉCÉAÉçÉOÉäÉXÉgÇ©ÇÁçÌèú
		CWndList::Del( this );

		HWND	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_LCH_LIST );
		INT	i;
		INT	Order[16];
		INT	nCount = Header_GetItemCount( ListView_GetHeader( hWndCtrl ) );

		ListView_GetColumnOrderArray( hWndCtrl, nCount, Order );

		INT	OrderTemp[16];
		for( i = 0; i < LAUNCHHEADER_MAX; i++ ) {
			OrderTemp[i] = Config.launcher.nHeaderOrder[i];
		}
		for( i = 0; i < nCount; i++ ) {
			Config.launcher.nHeaderOrder[i] = OrderTemp[Order[i]];
			Config.launcher.nHeaderWidth[Config.launcher.nHeaderOrder[i]] = ListView_GetColumnWidth( hWndCtrl, Order[i] );
		}
		OnUpdateStop();

		::GetWindowRect( m_hWnd, &Config.launcher.rcWindowPos );

		if( !m_FileListNum ) {
			Config.launcher.szLastSelect[0] = '\0';
		} else {
			FILELIST& fl = m_FileList[m_SelectPos];
			string	Path = CPathlib::MakePath( fl.path.c_str(), fl.fname.c_str() );
			::strcpy( Config.launcher.szLastSelect, Path.c_str() );
		}

		if( !m_bFileLoaded ) {
			SaveFileList();
		}

		::DestroyWindow( m_hWnd );
		m_hWnd = NULL;
	}
}

INT CALLBACK CLauncherDlg::ListViewCompare( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
//DEBUGOUT( "L1:%08X L2:%08X PA:%d\n", lParam1, lParam2, lParamSort );

	FILELIST& fl1 = m_FileList[lParam1];
	FILELIST& fl2 = m_FileList[lParam2];

	CHAR	szTemp[_MAX_PATH];
	string	s1, s2;
	INT	n1, n2;

	INT	ret = 0;
	switch( lParamSort ) {
		case	COLUMN_FILENAME:
#if	1
			::strcpy( szTemp, fl1.fname.c_str() );
			s1 = (CHAR*)::_mbsupr( (UCHAR*)szTemp );
			::strcpy( szTemp, fl2.fname.c_str() );
			s2 = (CHAR*)::_mbsupr( (UCHAR*)szTemp );
			if( !Config.launcher.bSortDir ) {
				ret = s1.compare( s2.c_str() );
			} else {
				ret = s2.compare( s1.c_str() );
			}
#else
			{
			INT	cmpresult = ::CompareString( LOCALE_USER_DEFAULT, NORM_IGNORECASE|NORM_IGNOREKANATYPE|NORM_IGNOREWIDTH,
								fl1.fname.c_str(), fl1.fname.size(), fl2.fname.c_str(), fl1.fname.size() );
				if( cmpresult == CSTR_LESS_THAN )
					ret = -1;
				if( cmpresult == CSTR_EQUAL )
					ret = 0;
				if( cmpresult == CSTR_GREATER_THAN )
					ret = 1;
			}
#endif
			if( ret == 0 ) {
				return	ListViewCompare( lParam1, lParam2, COLUMN_PATH );
			}
			return	ret;
		case	COLUMN_PATH:
			::strcpy( szTemp, fl1.path.c_str() );
			s1 = (CHAR*)::_mbsupr( (UCHAR*)szTemp );
			::strcpy( szTemp, fl2.path.c_str() );
			s2 = (CHAR*)::_mbsupr( (UCHAR*)szTemp );
			if( !Config.launcher.bSortDir ) {
				ret = s1.compare( s2.c_str() );
			} else {
				ret = s2.compare( s1.c_str() );
			}
			return	ret;
		case	COLUMN_MAPPER:
			n1 = fl1.mapper & 0x0FFF;
			n2 = fl2.mapper & 0x0FFF;
			goto	_Compare_Num;
		case	COLUMN_PRG:
			n1 = fl1.prg_size;
			n2 = fl2.prg_size;
			goto	_Compare_Num;
		case	COLUMN_CHR:
			n1 = fl1.chr_size;
			n2 = fl2.chr_size;
			goto	_Compare_Num;
		case	COLUMN_ALLCRC:
			n1 = fl1.crcall;
			n2 = fl2.crcall;
			goto	_Compare_Num;
		case	COLUMN_PRGCRC:
			n1 = fl1.crc;
			n2 = fl2.crc;
			goto	_Compare_Num;
		case	COLUMN_INFO:
			s1 = fl1.info;
			s2 = fl2.info;
			goto	_Compare_Str;
		case	COLUMN_DB:
			s1 = fl1.db;
			s2 = fl2.db;
			goto	_Compare_Str;
		case	COLUMN_TITLE:
			s1 = fl1.title;
			s2 = fl2.title;
			goto	_Compare_Str;
		case	COLUMN_COUNTRY:
			s1 = fl1.country;
			s2 = fl2.country;
			goto	_Compare_Str;
		case	COLUMN_MANUFACTURER:
			s1 = fl1.manufacturer;
			s2 = fl2.manufacturer;
			goto	_Compare_Str;
		case	COLUMN_SALEDATE:
			s1 = fl1.saledate;
			s2 = fl2.saledate;
			goto	_Compare_Str;
		case	COLUMN_PRICE:
			s1 = fl1.price;
			s2 = fl2.price;
			goto	_Compare_Str;
		case	COLUMN_GENRE:
			s1 = fl1.genre;
			s2 = fl2.genre;
			goto	_Compare_Str;
_Compare_Num:
			if( n1 == n2 ) {
				return	ListViewCompare( lParam1, lParam2, COLUMN_FILENAME );
			}
			if( !Config.launcher.bSortDir ) {
				if( n1 < n2 ) ret = -1;
				else	      ret = 1;
			} else {
				if( n1 < n2 ) ret = 1;
				else	      ret = -1;
			}
			return	ret;

_Compare_Str:
#if	1
			::strcpy( szTemp, s1.c_str() );
			s1 = (CHAR*)::_mbsupr( (UCHAR*)szTemp );
			::strcpy( szTemp, s2.c_str() );
			s2 = (CHAR*)::_mbsupr( (UCHAR*)szTemp );
			if( !Config.launcher.bSortDir ) {
				ret = s1.compare( s2.c_str() );
			} else {
				ret = s2.compare( s1.c_str() );
			}
#else
			{
			INT	cmpresult = ::CompareString( LOCALE_USER_DEFAULT, NORM_IGNORECASE|NORM_IGNOREKANATYPE|NORM_IGNOREWIDTH,
								s1.c_str(), s1.size(), s2.c_str(), s2.size() );
				if( cmpresult == CSTR_LESS_THAN )
					ret = -1;
				if( cmpresult == CSTR_EQUAL )
					ret = 0;
				if( cmpresult == CSTR_GREATER_THAN )
					ret = 1;
			}
#endif
			if( ret == 0 ) {
				return	ListViewCompare( lParam1, lParam2, COLUMN_FILENAME );
			}
			return	ret;
		default:
			break;
	}
	return	0;
}

void	CLauncherDlg::ResetListViewHeader()
{
	HWND	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_LCH_LIST );

	INT	i;
	for( i = 0; i < LAUNCHHEADER_MAX; i++ ) {
		ListView_DeleteColumn( hWndCtrl, 0 );
	}

	LV_COLUMN	lvColumn;
	lvColumn.mask = LVCF_FMT|LVCF_TEXT|LVCF_SUBITEM|LVCF_WIDTH;

	INT	no = 0;
	for( i = 0; i < LAUNCHHEADER_MAX; i++ ) {
		if( Config.launcher.bHeaderView[Config.launcher.nHeaderOrder[i]] ) {
//			if( Config.launcher.nHeaderOrder[i] >= 2 && Config.launcher.nHeaderOrder[i] <= 6 ) {
//				lvColumn.fmt = LVCFMT_RIGHT;
//			} else {
//				lvColumn.fmt = LVCFMT_LEFT;
//			}
			lvColumn.fmt = LVCFMT_LEFT;

			CHAR	szStr[64];
			CApp::LoadString( m_HeaderID[Config.launcher.nHeaderOrder[i]], szStr, sizeof(szStr) );
			lvColumn.iSubItem = no;
			lvColumn.pszText = szStr;
			lvColumn.cx = Config.launcher.nHeaderWidth[Config.launcher.nHeaderOrder[i]];
			ListView_InsertColumn( hWndCtrl, no, &lvColumn );
			no++;
		}
	}
}

void	CLauncherDlg::ResetListView()
{
	HWND	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_LCH_LIST );

	ListView_DeleteAllItems( hWndCtrl );

	FILELIST fl;
	LVITEM	lvitem;
	lvitem.mask = LVIF_TEXT|LVIF_PARAM;
	lvitem.iSubItem = 0;
	lvitem.pszText = "";

	::SendMessage( hWndCtrl, WM_SETREDRAW, (WPARAM)FALSE, 0 );

	for( INT index = 0; index < m_FileListNum; index++ ) {
		lvitem.iItem = index;
		lvitem.lParam = (LPARAM)index;
		ListView_InsertItem( hWndCtrl, &lvitem );
		fl = m_FileList[index];

		// ÉäÉXÉgÉrÉÖÅ[Ç÷ÇÃÉtÉ@ÉCÉãèÓïÒÇÃê›íË
		SetListView( index, fl );
	}

	::SendMessage( hWndCtrl, WM_SETREDRAW, (WPARAM)TRUE, 0 );
	::InvalidateRect( hWndCtrl, NULL, TRUE );
}

void	CLauncherDlg::SetListView( INT index, FILELIST& fl )
{
	HWND	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_LCH_LIST );

	CHAR	szStr[256];
	INT	RealOrder[16];
	INT	i;
	for( i = 0; i < LAUNCHHEADER_MAX; i++ ) {
		RealOrder[Config.launcher.nHeaderOrder[i]] = i;
	}

	szStr[0] = '\0';

	// File name
	if( Config.launcher.bHeaderView[0] ) {
		ListView_SetItemText( hWndCtrl, index, RealOrder[0], (LPSTR)fl.fname.c_str() );
	}
	// Path
	if( Config.launcher.bHeaderView[1] ) {
		ListView_SetItemText( hWndCtrl, index, RealOrder[1], (LPSTR)fl.path.c_str() );
	}
	// Mapper
	if( Config.launcher.bHeaderView[2] ) {
		INT	Mapper = fl.mapper & 0x0FFF;
		INT	image = 0;

		if( Mapper == 20 ) {
			::strcpy( szStr, "FDS" );
			if( fl.mapper&0xF000 )
				image = 4;
			else
				image = 3;
		} else if( Mapper == 0x100 ) {
			::strcpy( szStr, "NSF" );
			image = 5;
		} else if( Mapper > 0x100 ) {
			::strcpy( szStr, "Unknown" );
			image = 0;
		} else {
			::wsprintf( szStr, "%d", fl.mapper&0xFF );
			if( fl.mapper&0xF000 )
				image = 2;
			else
				image = 1;
		}
		ListView_SetItemText( hWndCtrl, index, RealOrder[2], szStr );

		// Image(State)
		ListView_SetItemState( hWndCtrl, index, INDEXTOSTATEIMAGEMASK(image+1), LVIS_STATEIMAGEMASK );

	}
	// PRG size
	if( Config.launcher.bHeaderView[3] ) {
		if( (fl.mapper&0x0FFF) == 20 ) {
		// FDS
			::wsprintf( szStr, "%d", fl.prg_size );
		} else if( (fl.mapper&0x0FFF) == 0x100 ) {
		// NSF
			::wsprintf( szStr, "%d", fl.prg_size );
		} else if( (fl.mapper&0x0FFF) < 0x100 ) {
		// NES
			::wsprintf( szStr, "%d", 16*fl.prg_size );
		} else {
		// Bad file
			::strcpy( szStr, "0" );
		}
		ListView_SetItemText( hWndCtrl, index, RealOrder[3], szStr );
	}
	// CHR size
	if( Config.launcher.bHeaderView[4] ) {
		if( (fl.mapper&0x0FFF) == 0x100 ) {
		// NSF
			::wsprintf( szStr, "%d", fl.chr_size );
		} else if( (fl.mapper&0x0FFF) < 0x100 ) {
		// NES
			::wsprintf( szStr, "%d", 8*fl.chr_size );
		}
//		::wsprintf( szStr, "%d", 8*fl.chr_size );
		ListView_SetItemText( hWndCtrl, index, RealOrder[4], szStr );
	}
	// CRC ALL
	if( Config.launcher.bHeaderView[5] ) {
		::wsprintf( szStr, "%08X", fl.crcall );
		ListView_SetItemText( hWndCtrl, index, RealOrder[5], szStr );
	}
	// CRC
	if( Config.launcher.bHeaderView[6] ) {
		::wsprintf( szStr, "%08X", fl.crc );
		ListView_SetItemText( hWndCtrl, index, RealOrder[6], szStr );
	}
	// Info
	if( Config.launcher.bHeaderView[7] ) {
		ListView_SetItemText( hWndCtrl, index, RealOrder[7], (LPSTR)fl.info.c_str() );
	}
	// DB
	if( Config.launcher.bHeaderView[8] ) {
		ListView_SetItemText( hWndCtrl, index, RealOrder[8], (LPSTR)fl.db.c_str() );
	}
	// TITLE
	if( Config.launcher.bHeaderView[9] ) {
		ListView_SetItemText( hWndCtrl, index, RealOrder[9], (LPSTR)fl.title.c_str() );
	}
	// Country
	if( Config.launcher.bHeaderView[10] ) {
		ListView_SetItemText( hWndCtrl, index, RealOrder[10], (LPSTR)fl.country.c_str() );
	}
	// Manufacturer
	if( Config.launcher.bHeaderView[11] ) {
		ListView_SetItemText( hWndCtrl, index, RealOrder[11], (LPSTR)fl.manufacturer.c_str() );
	}
	// Sale date
	if( Config.launcher.bHeaderView[12] ) {
		ListView_SetItemText( hWndCtrl, index, RealOrder[12], (LPSTR)fl.saledate.c_str() );
	}
	// Price
	if( Config.launcher.bHeaderView[13] ) {
		ListView_SetItemText( hWndCtrl, index, RealOrder[13], (LPSTR)fl.price.c_str() );
	}
	// Genre
	if( Config.launcher.bHeaderView[14] ) {
		ListView_SetItemText( hWndCtrl, index, RealOrder[14], (LPSTR)fl.genre.c_str() );
	}
}

void	CLauncherDlg::SortListView()
{
	INT	i;
	HWND	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_LCH_LIST );

	if( ListView_GetItemCount( hWndCtrl ) <= 0 )
		return;

	// ÉwÉbÉ_ïî
	HWND	hWndHdr = ListView_GetHeader( hWndCtrl );
	HD_ITEM	hdi;
	hdi.mask = HDI_FORMAT;
	hdi.fmt  = HDF_STRING;
	for( i = 0; i < Header_GetItemCount(hWndHdr); i++ ) {
		Header_SetItem( hWndHdr, i, &hdi );
	}
	hdi.mask = HDI_FORMAT|HDI_IMAGE;
	hdi.fmt = HDF_STRING | HDF_IMAGE | HDF_BITMAP_ON_RIGHT;
	hdi.iImage = Config.launcher.bSortDir?1:0;

	INT	RealOrder[16];
	for( i = 0; i < LAUNCHHEADER_MAX; i++ ) {
		RealOrder[Config.launcher.nHeaderOrder[i]] = i;
	}

	Header_SetItem( hWndHdr, RealOrder[Config.launcher.nSortType], &hdi );

	// É\Å[Égñ{ëÃ
	ListView_SortItems( hWndCtrl, (PFNLVCOMPARE)ListViewCompare, Config.launcher.nSortType );

	if( m_SelectPos >= 0 ) {
		LV_FINDINFO	fi;
		fi.flags  = LVFI_PARAM;
		fi.lParam = (LPARAM)m_SelectPos;
		INT	index = ListView_FindItem( hWndCtrl, -1, &fi );
		if( index >= 0 ) {
			ListView_EnsureVisible( hWndCtrl, index, FALSE );
			ListView_SetItemState( hWndCtrl, index, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED );
		}
	} else {
		LVITEM	lvitem;
		lvitem.mask = LVIF_PARAM;
		lvitem.iItem = 0;
		ListView_GetItem( hWndCtrl, &lvitem );
		m_SelectPos = lvitem.lParam;
		ListView_SetItemState( hWndCtrl, 0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED );
	}
}

void	CLauncherDlg::SetLastSelect()
{
	if( !m_FileListNum )
		return;

	FILELIST fl;
	BOOL	bSel = FALSE;
	string	path;

	for( INT index = 0; index < m_FileListNum; index++ ) {
		fl = m_FileList[index];
		path = CPathlib::MakePath( fl.path.c_str(), fl.fname.c_str() );
		if( ::strcmp( Config.launcher.szLastSelect, path.c_str() ) == 0 ) {
			m_SelectPos = index;
			bSel = TRUE;
			break;
		}
	}

	if( bSel ) {
		HWND	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_LCH_LIST );
		LV_FINDINFO	fi;
		fi.flags  = LVFI_PARAM;
		fi.lParam = (LPARAM)m_SelectPos;
		INT	index = ListView_FindItem( hWndCtrl, -1, &fi );
		if( index >= 0 ) {
			ListView_EnsureVisible( hWndCtrl, index, FALSE );
			ListView_SetItemState( hWndCtrl, index, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED );
		}
	}
}

void	CLauncherDlg::OnUpdateStart()
{
	if( !m_FileListNum )
		return;

	if( m_bUpdate ) {
		m_UpdatePos = 0;
		m_nUpdateIndex = 0;
	} else {
		m_UpdatePos = 0;
		m_nUpdateIndex = 0;
		m_nTimerID = ::SetTimer( m_hWnd, 1, 100, NULL );
		m_bUpdate = TRUE;
	}
}

void	CLauncherDlg::OnUpdateStop()
{
	if( m_bUpdate ) {
		::KillTimer( m_hWnd, m_nTimerID );
		m_bUpdate = FALSE;

		::SendMessage( ::GetDlgItem( m_hWnd, IDC_LCH_STATUS ), SB_SETTEXT, SBT_NOBORDERS, (LPARAM)"" );
	}
}

DLGMSG	CLauncherDlg::OnTimer( DLGMSGPARAM )
{
	if( m_bUpdate && !m_bUpdating ) {
		UpdateListView();
	}
	return	FALSE;
}

void	CLauncherDlg::UpdateListView()
{
	HWND	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_LCH_LIST );
	INT	nListCount = ListView_GetItemCount( hWndCtrl );
	if( nListCount <= 0 )
		return;

	m_bUpdating = TRUE;

//	for( INT i = 0; i < 5; i++ ) {
	for( INT i = 0; i < 20; i++ ) {
		FILELIST& fl = m_FileList[m_UpdatePos];
		CheckFile( fl );

		LV_FINDINFO	fi;
		fi.flags  = LVFI_PARAM;
		fi.lParam = (LPARAM)m_UpdatePos;
		INT	index = ListView_FindItem( hWndCtrl, -1, &fi );
		if( index >= 0 ) {
			SetListView( index, fl );
		}

		m_UpdatePos++;
		m_nUpdateIndex++;

		if( m_nUpdateIndex >= nListCount ) {
			OnUpdateStop();
			break;
		}
	}

	if( m_nUpdateIndex < nListCount ) {
		// ÉXÉeÅ[É^ÉXÉoÅ[Ç÷ÇÃï\é¶
		CHAR	szTemp[256], szText[256];
		CApp::LoadString( IDS_LCH_LISTUPDATE, szTemp, sizeof(szTemp) );
		::wsprintf( szText, szTemp, m_nUpdateIndex, nListCount );
		::SendMessage( ::GetDlgItem( m_hWnd, IDC_LCH_STATUS ), SB_SETTEXT, SBT_NOBORDERS, (LPARAM)szText );
	}

	m_bUpdating = FALSE;
}

#define MKID(a) ((unsigned long) \
		(((a) >> 24) & 0x000000FF) | \
		(((a) >>  8) & 0x0000FF00) | \
		(((a) <<  8) & 0x00FF0000) | \
		(((a) << 24) & 0xFF000000))

void CLauncherDlg::CheckFile( FILELIST& fl )
{
FILE*	fp = NULL;
LPBYTE	temp = NULL;
LPBYTE  pUnif =  NULL;
LONG	FileSize;
LONG	filesize;
string	path;

	path = CPathlib::MakePath( fl.path.c_str(), fl.fname.c_str() );

	if( (fp = ::fopen( path.c_str(), "rb" )) ) {
		NESHEADER	header;

		// ÉtÉ@ÉCÉãÉTÉCÉYéÊìæ
		::fseek( fp, 0, SEEK_END );
		FileSize = ::ftell( fp );
		::fseek( fp, 0, SEEK_SET );
		// ÉtÉ@ÉCÉãÉTÉCÉYÉ`ÉFÉbÉN(NESÉwÉbÉ_+1ÉoÉCÉgà»è„Ç©ÅH)
		if( FileSize < 17 )
			goto	_error_return;

		// ÉeÉìÉ|ÉâÉäÉÅÉÇÉäämï€
		if( !(temp = (LPBYTE)::malloc( FileSize )) )
			goto	_error_return;

		pUnif = (LPBYTE)::malloc( FileSize );
		filesize = FileSize;

		// ÉTÉCÉYï™ì«Ç›çûÇ›
		if( ::fread( temp, FileSize, 1, fp ) != 1 )
			goto	_error_return;

		memcpy(pUnif,temp,FileSize);

		FCLOSE( fp );

		// ÉwÉbÉ_ÉRÉsÅ[
		memcpy( &header, temp, sizeof(NESHEADER) );

		if( header.ID[0] == 'N' && header.ID[1] == 'E'
		 && header.ID[2] == 'S' && header.ID[3] == 0x1A ) {
			// ÉwÉbÉ_ÉRÉsÅ[
//			memcpy( &header, temp, sizeof(NESHEADER) );
		} else if( header.ID[0] == 'F' && header.ID[1] == 'D'
			&& header.ID[2] == 'S' && header.ID[3] == 0x1A ) {
			// ÉwÉbÉ_ÉRÉsÅ[
//			memcpy( &header, temp, sizeof(NESHEADER) );
		} else if( header.ID[0] == 'N' && header.ID[1] == 'E'
			&& header.ID[2] == 'S' && header.ID[3] == 'M') {
			// ÉwÉbÉ_ÉRÉsÅ[
//			memcpy( &header, temp, sizeof(NESHEADER) );
		} else if( header.ID[0] == 'U' && header.ID[1] == 'N'
			&& header.ID[2] == 'I' && header.ID[3] == 'F' ) {
			//
		} else {
			FREE( temp );
			temp = NULL;
			if( !UnCompress( path.c_str(), &temp, (LPDWORD)&FileSize ) )
				goto	_error_return;
			memcpy( &header, temp, sizeof(NESHEADER) );
		}

		if( header.ID[0] == 'N' && header.ID[1] == 'E'
		 && header.ID[2] == 'S' && header.ID[3] == 0x1A ) {
			fl.mapper = ((header.control1&0xF0)>>4)|(header.control2&0xF0);
			fl.prg_size = header.PRG_PAGE_SIZE;
			fl.chr_size = header.CHR_PAGE_SIZE;
			CHAR	szTemp[64];
			::wsprintf( szTemp, "%s%s%s%s %s",
				(header.control1&0x01)?"V":"H",
				(header.control1&0x02)?"S":"_",
				(header.control1&0x04)?"T":"_",
				(header.control1&0x08)?"4":"_",
				(header.control2&0x01)?"(VS)":"" );
			fl.info = szTemp;

			BOOL	bBad = FALSE;
			if( header.control1&0x04 ) {
				if( FileSize < (512+16384*fl.prg_size+8192*fl.chr_size+sizeof(NESHEADER)) ) {
					fl.crcall   = CRC::CrcRev( FileSize-sizeof(NESHEADER), temp+sizeof(NESHEADER) );
					fl.crc      = 0;
					fl.prg_size = 0;
					fl.chr_size = 0;
					fl.info     = "Bad NES header";
					bBad = TRUE;
				} else {
					fl.crcall = CRC::CrcRev( 512+16384*fl.prg_size+8192*fl.chr_size, temp+sizeof(NESHEADER) );
					fl.crc    = CRC::CrcRev( 512+16384*fl.prg_size, temp+sizeof(NESHEADER) );
				}
			} else {
				if( FileSize < (16384*fl.prg_size+8192*fl.chr_size+sizeof(NESHEADER)) ) {
					fl.crcall   = CRC::CrcRev( FileSize-sizeof(NESHEADER), temp+sizeof(NESHEADER) );
					fl.crc      = 0;
					fl.prg_size = 0;
					fl.chr_size = 0;
					fl.info     = "Bad NES header";
					bBad = TRUE;
				} else {
					fl.crcall = CRC::CrcRev( 16384*fl.prg_size+8192*fl.chr_size, temp+sizeof(NESHEADER) );
					fl.crc    = CRC::CrcRev( 16384*fl.prg_size, temp+sizeof(NESHEADER) );
				}
			}

			if( !bBad ) {
				ROMDB	db;
				INT	ret = romdatabase.HeaderCheck( header, fl.crcall, fl.crc, db );
				if( ret == 0 ) {
					fl.db = "OK";
				} else if( ret == 1 ) {
					fl.db = "NG";
					fl.mapper |= 0x1000;
				} else {
					fl.db = "??";
				}
				if( ret >= 0 ) {
					fl.title        = db.title;
					fl.country      = db.country;
					fl.manufacturer = db.manufacturer;
					fl.saledate     = db.saledate;
					fl.price        = db.price;
					fl.genre        = db.genre;
				}
			} else {
				fl.mapper |= 0x1000;
			}
		}else if( header.ID[0] == 'U' && header.ID[1] == 'N'&& header.ID[2] == 'I' && header.ID[3] == 'F' ) {
		
			DWORD Signature, BlockLen;
			DWORD ipos =0x20;//Ã¯π˝UNIFÕ∑
			BYTE id,i;
			BYTE *tPRG[0x10], *tCHR[0x10];
			DWORD sizePRG[0x10],sizeCHR[0x10];
			char info[100];
			char name[100];
			
			
			for (i = 0; i < 0x10; i++)
			{
				tPRG[i] = tCHR[i] = 0;
			}

			//filesize
			while(ipos<filesize)
			{
				id = 0;
				memcpy(&Signature,&pUnif[ipos],4);ipos+=4;
				memcpy(&BlockLen,&pUnif[ipos],4);ipos+=4;
				
				switch(Signature)
				{
					case MKID('MAPR')://board√˚◊÷
						memcpy( info, &pUnif[ipos], BlockLen);
						fl.info = info;
						ipos+=BlockLen;	break;

					case MKID('NAME'):
						memcpy( name, &pUnif[ipos], BlockLen);
						fl.title = name;
						ipos+=BlockLen;	break;
					
					case MKID('PRGF'):	id++;
					case MKID('PRGE'):	id++;
					case MKID('PRGD'):	id++;
					case MKID('PRGC'):	id++;
					case MKID('PRGB'):	id++;
					case MKID('PRGA'):	id++;
					case MKID('PRG9'):	id++;
					case MKID('PRG8'):	id++;
					case MKID('PRG7'):	id++;
					case MKID('PRG6'):	id++;
					case MKID('PRG5'):	id++;
					case MKID('PRG4'):	id++;
					case MKID('PRG3'):	id++;
					case MKID('PRG2'):	id++;
					case MKID('PRG1'):	id++;
					case MKID('PRG0'):
						sizePRG[id] = BlockLen;
						tPRG[id] = (uint8*)malloc(BlockLen);
						memcpy( tPRG[id], &pUnif[ipos], BlockLen);
						ipos+=BlockLen;
						break;

					case MKID('CHRF'):	id++;
					case MKID('CHRE'):	id++;
					case MKID('CHRD'):	id++;
					case MKID('CHRC'):	id++;
					case MKID('CHRB'):	id++;
					case MKID('CHRA'):	id++;
					case MKID('CHR9'):	id++;
					case MKID('CHR8'):	id++;
					case MKID('CHR7'):	id++;
					case MKID('CHR6'):	id++;
					case MKID('CHR5'):	id++;
					case MKID('CHR4'):	id++;
					case MKID('CHR3'):	id++;
					case MKID('CHR2'):	id++;
					case MKID('CHR1'):	id++;
					case MKID('CHR0'):
						sizeCHR[id] = BlockLen;
						tCHR[id] = (uint8*)malloc(BlockLen);
						memcpy( tCHR[id], &pUnif[ipos], BlockLen);
						ipos+=BlockLen;
						break;
						
					default:
						ipos+=BlockLen;	break;
				}
			}

			fl.mapper = 0;
			fl.prg_size = 0;
			fl.chr_size = 0;
			uint32 LenPRG=0,LenCHR=0;
			for (i = 0; i < 16/*0x10*/; i++)
			{
				if (tPRG[i])
				{
					//memcpy(&g_ROM.ROM_banks[LenPRG], tPRG[i], sizePRG[i]);
					LenPRG += sizePRG[i];
					fl.prg_size  += sizePRG[i]>>14;
					free(tPRG[i]);
				}
				if (tCHR[i])
				{
					//memcpy(&g_ROM.VROM_banks[LenCHR], tCHR[i], sizeCHR[i]);
					LenCHR += sizeCHR[i];
					fl.chr_size = (fl.chr_size)+(sizeCHR[i]>>13);
					free(tCHR[i]);
				}
			}
			FREE( pUnif );

		}else if( header.ID[0] == 'F' && header.ID[1] == 'D'
			&& header.ID[2] == 'S' && header.ID[3] == 0x1A ) {
			fl.mapper = 20;
			fl.info = "Disk";
			// Disk number
			fl.prg_size = header.PRG_PAGE_SIZE;

			if( FileSize < (16+65500*(LONG)header.PRG_PAGE_SIZE) ) {
				// ÉfÉBÉXÉNÉTÉCÉYÇ™àŸèÌÇ≈Ç∑
				fl.mapper |= 0x1000;
				fl.info = "Bad FDS size";
			}
		} else if( header.ID[0] == 'N' && header.ID[1] == 'E'
			&& header.ID[2] == 'S' && header.ID[3] == 'M') {
			fl.mapper = 0x100;
			if( FileSize < 0x80 ) {
				fl.info = "Bad NSF size";
			} else {
				// Total songs
				fl.prg_size = (INT)temp[0x06];
				fl.chr_size = (INT)temp[0x7a];	// NTSC/PAL
				if( temp[0x7b] ) {
					static	LPCSTR	szChipName[] = { "VRC VI", "VRC VII", "FDS", "MMC5", 
									 "N106", "FME-07", NULL, NULL };
					BOOL	bFind = FALSE;
					for( INT i = 0; szChipName[i]; i++ ) {
						if( temp[0x7b] & (1<<i) ) {
							if( bFind )
								fl.info = fl.info + "/";
							fl.info = fl.info + szChipName[i];
							bFind = TRUE;
						}
					}
				} else {
					fl.info = "Internal";
				}
			}
		}

		FREE(temp);
	}
	return;

_error_return:
	FREE(temp);
	FCLOSE( fp );
}

void CLauncherDlg::ResetFileList()
{
INT	i;
LPSTR	pszExt[] = {
	"*.nes",
	"*.fds",
	"*.nsf",
	"*.lzh",
	"*.zip",
	"*.cab",
	"*.rar",
	NULL
};

	m_bFileLoaded = FALSE;

	// ÉtÉ@ÉCÉãÉäÉXÉgÇÃÉNÉäÉA
	m_FileList.clear();
	m_FileListNum = 0;

	string	path;
	CHAR**	pExt;
	BOOL	bFind;
	FILELIST fl;
	fl.crc = fl.crcall = 0;
	fl.mapper = 0x200;	// Unknown
	fl.prg_size = fl.chr_size = 0;

	fl.info = fl.db = fl.title = fl.country = fl.manufacturer = fl.saledate = fl.price = fl.genre = "";

	HANDLE	hFind;
	WIN32_FIND_DATA	fdat;

	// ägí£éqï åüçı
	for( i = 0; i < 16; i++ ) {
		if( ::strlen( Config.launcher.szFolder[i] ) && Config.launcher.bFolderUse[i] ) {
			pExt = pszExt;
			while( *pExt ) {
				path = Config.launcher.szFolder[i];
				path += *pExt;
				bFind = TRUE;
				hFind = ::FindFirstFile( path.c_str(), &fdat );
				if( hFind == INVALID_HANDLE_VALUE )
					bFind = FALSE;
				while( bFind ) {
					fl.fname = fdat.cFileName;
					fl.path = Config.launcher.szFolder[i];
					m_FileList.push_back( fl );
					m_FileListNum++;
					bFind = ::FindNextFile( hFind, &fdat );
				}
				pExt++;
			}
		}
	}

	m_SelectPos = NULL;
}

DLGMSG	CLauncherDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CLauncherDlg::OnInitDialog\n" );
	// ÉXÉeÅ[É^ÉXÉoÅ[ÇÃí«â¡
	HWND	hWndCtrl = ::CreateStatusWindow( WS_CHILD|WS_VISIBLE|CCS_BOTTOM|SBARS_SIZEGRIP,
						"", m_hWnd, IDC_LCH_STATUS );
	if( !hWndCtrl ) {
		bResult = FALSE;
		return	TRUE;
	}
	::SendMessage( hWndCtrl, SB_SETTEXT, SBT_NOBORDERS, (LPARAM)"" );

	// ÉäÉXÉgÉrÉÖÅ[ÇÃÉXÉ^ÉCÉãìôÇÃê›íË
	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_LCH_LIST );
	ListView_SetExtendedListViewStyle( hWndCtrl, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP );
	ListView_SetItemCount( hWndCtrl, 2000 );

	// ÉCÉÅÅ[ÉWÉäÉXÉgÇÃçÏê¨
	m_hImageList = ImageList_LoadBitmap(
		CApp::GetInstance(), MAKEINTRESOURCE(IDB_LAUNCHERIMAGELIST),
		16, 6, RGB(255,0,255) );

	// ÉCÉÅÅ[ÉWÉäÉXÉgÇÉäÉXÉgÉrÉÖÅ[Ç…äÑÇËìñÇƒ
	ListView_SetImageList( hWndCtrl, m_hImageList, LVSIL_STATE );

	// É\Å[ÉgópÉCÉÅÅ[ÉWÉäÉXÉg
	m_hImageListHdr = ImageList_Create( 16, 16, ILC_COLORDDB|ILC_MASK, 2, 2 );
	ImageList_AddIcon( m_hImageListHdr, CApp::LoadIcon(IDI_SORT_DOWN) );
	ImageList_AddIcon( m_hImageListHdr, CApp::LoadIcon(IDI_SORT_UP) );

	// ÉCÉÅÅ[ÉWÉäÉXÉgÇÉwÉbÉ_Ç…äÑÇËìñÇƒ
	Header_SetImageList( ListView_GetHeader( hWndCtrl ), m_hImageListHdr );

	// ÉEÉCÉìÉhÉEà íu/ÉTÉCÉYÇÃê›íË
	RECT	rc = Config.launcher.rcWindowPos;
	if( (rc.right-rc.left) && (rc.bottom-rc.top) ) {
		::SetWindowPos( m_hWnd, NULL, rc.left, rc.top, RCWIDTH(rc), RCHEIGHT(rc), SWP_NOZORDER );
	}

	// ÉäÉXÉgÉrÉÖÅ[ÇÃÉwÉbÉ_çÄñ⁄ÇÃê›íË
	ResetListViewHeader();

	m_bUpdating = FALSE;
	m_nTimerID = 0;
	m_nUpdateIndex = 0;

	m_SelectPos = 0;

//	m_nSortType = COLUMN_FILENAME;
//	m_bSortDir  = FALSE;

	// ÉâÉìÉ`ÉÉÅ[ÉäÉXÉgÉiÉìÉoÅ[
	m_nListSelect = Config.launcher.nListSelect;

	// ÉtÉ@ÉCÉãÉäÉXÉgêî
	m_FileListNum = 0;

	//
	m_bFileLoaded = FALSE;
	if( LoadFileList() ) {
		if( !m_FileListNum ) {
			::PostMessage( m_hWnd, WM_COMMAND, ID_LCH_REFRESH, 0 );
		} else {
			ResetListView();
			SortListView();
			SetLastSelect();
			m_bFileLoaded = TRUE;
		}
	} else {
		::PostMessage( m_hWnd, WM_COMMAND, ID_LCH_REFRESH, 0 );
	}

//	// ï\é¶
//	::ShowWindow( m_hWnd, SW_SHOW );

	return	TRUE;
}

DLGMSG	CLauncherDlg::OnDestroy( DLGMSGPARAM )
{
	if( m_hImageList ) {
		ImageList_Destroy( m_hImageList );
		m_hImageList = NULL;
	}
	if( m_hImageListHdr ) {
		ImageList_Destroy( m_hImageListHdr );
		m_hImageListHdr = NULL;
	}

	return	TRUE;
}

DLGMSG	CLauncherDlg::OnClose( DLGMSGPARAM )
{
	::ShowWindow( m_hWnd, SW_HIDE ); // îÒï\é¶Ç…Ç∑ÇÈÇæÇØ
	return	TRUE;
}

DLGMSG	CLauncherDlg::OnActivate( DLGMSGPARAM )
{
	if( LOWORD(wParam) == WA_INACTIVE ) {
//		DEBUGOUT( "CLauncherDlg::OnActivate:Inactive\n" );
		::PostMessage( CApp::GetHWnd(), WM_VNS_SHORTCUTENABLE, (WPARAM)TRUE, 0 );

		if( Config.launcher.bActivePause ) {
			if( Emu.IsRunning() )
				Emu.Resume();
		}
	} else {
//		DEBUGOUT( "CLauncherDlg::OnActivate:Active\n" );
		::PostMessage( CApp::GetHWnd(), WM_VNS_SHORTCUTENABLE, (WPARAM)FALSE, 0 );

		if( Config.launcher.bActivePause ) {
			if( Emu.IsRunning() )
				Emu.Pause();
		}
	}
	return	TRUE;
}

DLGMSG	CLauncherDlg::OnSetCursor( DLGMSGPARAM )
{
//	DEBUGOUT( "CLauncherDlg::OnSetCursor\n" );
	return	FALSE;
}

DLGMSG	CLauncherDlg::OnSize( DLGMSGPARAM )
{
//	DEBUGOUT( "CLauncherDlg::OnSize\n" );
	HWND	hWndCtrl;
	RECT	rcT, rcC, rcW;
	::GetClientRect( m_hWnd, &rcC );
	rcT = rcC;
	if( (hWndCtrl = ::GetDlgItem( m_hWnd, IDC_LCH_STATUS )) ) {
		::GetWindowRect( hWndCtrl, &rcW );
		rcT.top = rcC.bottom - (rcW.bottom-rcW.top);
		::MoveWindow( hWndCtrl, rcT.left, rcT.top, RCWIDTH(rcT), RCHEIGHT(rcT), TRUE );
		if( (hWndCtrl = ::GetDlgItem( m_hWnd, IDC_LCH_LIST )) ) {
			rcT.top = rcC.top;
			rcT.bottom = rcC.bottom - (rcW.bottom-rcW.top);
			::MoveWindow( hWndCtrl, rcT.left, rcT.top, RCWIDTH(rcT), RCHEIGHT(rcT), TRUE );
		}
	}
	return	FALSE;
}

DLGMSG	CLauncherDlg::OnInitMenuPopup( DLGMSGPARAM )
{
//	DEBUGOUT( "CLauncherDlg::OnInitMenuPopup\n" );
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


void	CLauncherDlg::OnUpdateMenu( HMENU hMenu, UINT uID )
{
	BOOL	bCheck = FALSE;
	switch( uID ) {
		case	ID_LCH_LIST0:	case	ID_LCH_LIST1:
		case	ID_LCH_LIST2:	case	ID_LCH_LIST3:
		case	ID_LCH_LIST4:	case	ID_LCH_LIST5:
		case	ID_LCH_LIST6:	case	ID_LCH_LIST7:
		case	ID_LCH_LIST8:	case	ID_LCH_LIST9:
			if( m_nListSelect == (uID-ID_LCH_LIST0) )
				bCheck = TRUE;
			else
				bCheck = FALSE;
			::CheckMenuItem( hMenu, uID, MF_BYCOMMAND|(bCheck?MF_CHECKED:MF_UNCHECKED) );
			break;
	}
}

DLGNOTIFY CLauncherDlg::OnKeyDownListView( DLGNOTIFYPARAM )
{
	NMLVKEYDOWN* pNMLVKeyDown = (NMLVKEYDOWN*)pNMHDR;
	if( pNMLVKeyDown->wVKey == VK_ESCAPE ) {
		::PostMessage( m_hWnd, WM_COMMAND, IDCANCEL, 0 );
	}
}

DLGNOTIFY CLauncherDlg::OnReturnListView( DLGNOTIFYPARAM )
{
	::PostMessage( m_hWnd, WM_COMMAND, IDOK, 0 );
}

DLGNOTIFY CLauncherDlg::OnDoubleClickListView( DLGNOTIFYPARAM )
{
	if( m_FileListNum ) {
//DEBUGOUT( "Item double click!! SEL=%08X\n", m_SelectPos );
		FILELIST& fl = m_FileList[m_SelectPos];
		string	path = CPathlib::MakePath( fl.path.c_str(), fl.fname.c_str() );
		::strcpy( m_LaunchPath, path.c_str() );

		// ÉÅÉCÉìÉEÉCÉìÉhÉEÇ…É|ÉXÉg
		::PostMessage( CApp::GetHWnd(), WM_VNS_LAUNCHERCMD, 0, (LPARAM)m_LaunchPath );
	}
}

DLGNOTIFY CLauncherDlg::OnColumnClickListView( DLGNOTIFYPARAM )
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//DEBUGOUT( "Column click!! I:%d S:%d\n", pNMListView->iItem, pNMListView->iSubItem );
//DEBUGOUT( "Order:%d\n", Config.launcher.nHeaderOrder[pNMListView->iSubItem] );

	if( Config.launcher.nSortType == Config.launcher.nHeaderOrder[pNMListView->iSubItem] ) {
		Config.launcher.bSortDir = !Config.launcher.bSortDir;
	} else {
		Config.launcher.bSortDir = FALSE;
		Config.launcher.nSortType = Config.launcher.nHeaderOrder[pNMListView->iSubItem];
	}
	SortListView();
}

DLGNOTIFY CLauncherDlg::OnItemChangedListView( DLGNOTIFYPARAM )
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

        if( !(pNMListView->uOldState & LVIS_SELECTED) && (pNMListView->uNewState & LVIS_SELECTED) ) {
//DEBUGOUT( "Item Select. SEL=%d\n", pNMListView->iItem );
		m_SelectPos = (INT)pNMListView->lParam;
	}
}

DLGCMD	CLauncherDlg::OnOK( DLGCMDPARAM )
{
//	DEBUGOUT( "CLauncherDlg::OnOK\n" );
	if( m_FileListNum ) {
//DebugOut( "Item double click!! SEL=%08X\n", m_SelectPos );
		FILELIST& fl = m_FileList[m_SelectPos];
		string	path = CPathlib::MakePath( fl.path.c_str(), fl.fname.c_str() );
		::strcpy( m_LaunchPath, path.c_str() );

		// ÉÅÉCÉìÉEÉCÉìÉhÉEÇ…É|ÉXÉg
		::PostMessage( CApp::GetHWnd(), WM_VNS_LAUNCHERCMD, 0, (LPARAM)m_LaunchPath );
	}
}

DLGCMD	CLauncherDlg::OnCancel( DLGCMDPARAM )
{
//	DEBUGOUT( "CLauncherDlg::OnCancel\n" );
	::ShowWindow( m_hWnd, SW_HIDE ); // îÒï\é¶Ç…Ç∑ÇÈÇæÇØ
}

DLGCMD	CLauncherDlg::OnListSelect( DLGCMDPARAM )
{
//	DEBUGOUT( "CLauncherDlg::OnListSelect uID:%d\n", uID );
	INT	nSel = uID-ID_LCH_LIST0;

	// à·Ç§ÉäÉXÉgÇ…ÇµÇΩÇ¢ÅH
	if( m_nListSelect != nSel ) {
		// ÇµÇΩÇ¢Ç≈Ç∑
//		DEBUGOUT( "LISTSEL=%d\n", nSel );

		// íºëOÇÃÇ‡ÇÃÇÉZÅ[Éu
		m_bFileLoaded = FALSE;
		SaveFileList();

		Config.launcher.nListSelect = m_nListSelect = nSel;
		LoadFileList();
		if( m_FileListNum ) {
			ResetListView();
			SortListView();
			SetLastSelect();
			m_bFileLoaded = TRUE;
		} else {
			ResetListView();
		}
	}
}

DLGCMD	CLauncherDlg::OnRefresh( DLGCMDPARAM )
{
	OnUpdateStop();
	ResetFileList();
	ResetListView();
	SortListView();
	OnUpdateStart();
}

DLGCMD	CLauncherDlg::OnDispEdit( DLGCMDPARAM )
{
	CLchDispEditDlg	dlg;

	if( dlg.DoModal( m_hWnd ) == IDOK ) {
		ResetListViewHeader();
		ResetListView();
		SortListView();
	}
}

DLGCMD	CLauncherDlg::OnFolder( DLGCMDPARAM )
{
	CLchFolderConfigDlg	dlg;

	if( dlg.DoModal( m_hWnd ) == IDOK ) {
//		::PostMessage( m_hWnd, WM_COMMAND, ID_LCH_REFRESH, 0 );
	}
}

DLGCMD	CLauncherDlg::OnHeaderEdit( DLGCMDPARAM )
{
DEBUGOUT( "CLauncherDlg::OnHeaderEdit SEL=%d\n", m_SelectPos );

	if( m_SelectPos >= 0 && m_FileListNum ) {
		FILE*	fp = NULL;
		NESHEADER header;

		FILELIST& fl = m_FileList[ m_SelectPos ];

		string Path = CPathlib::MakePath( fl.path.c_str(), fl.fname.c_str() );
DEBUGOUT( "Path:%s\n", Path.c_str() );

		// ÉAÅ[ÉJÉCÉuÉtÉ@ÉCÉãÇÕÉGÉfÉBÉbÉgïsâ¬
		char*	pExt = ::PathFindExtension( Path.c_str() );
		if( _stricmp( pExt, ".lzh" ) == 0 || _stricmp( pExt, ".zip" ) == 0 || _stricmp( pExt, ".cab" ) == 0 || _stricmp( pExt, ".rar" ) == 0 ) {
			return;
		}

		if( (fp = ::fopen( Path.c_str(), "rb" )) ) {
			if( ::fread( &header, sizeof(header), 1, fp ) != 1 ) {
				FCLOSE( fp );
				return;
			}
			FCLOSE( fp );
		} else {
			return;
		}

		// NESà»äOÇÕÉGÉfÉBÉbÉgïsâ¬
		if( header.ID[0] != 'N' || header.ID[1] != 'E'
		 || header.ID[2] != 'S' || header.ID[3] != 0x1A) {
			return;
		}

		CLchHeaderEditDlg	dlg;

		dlg.m_nMapperNo    = (INT)((header.control1&0xF0)>>4)|(header.control2&0xF0);
		dlg.m_bMirror      = (header.control1&0x01)?TRUE:FALSE;
		dlg.m_bSram        = (header.control1&0x02)?TRUE:FALSE;
		dlg.m_bTrainer     = (header.control1&0x04)?TRUE:FALSE;
		dlg.m_bFourScreen  = (header.control1&0x08)?TRUE:FALSE;
		dlg.m_bVSUnisystem = (header.control2&0x01)?TRUE:FALSE;

		if( dlg.DoModal( m_hWnd ) == IDOK ) {
			BYTE	control1 = 0;
			BYTE	control2 = 0;
			INT	no = dlg.m_nMapperNo;

			control1 = (no&0x0F)<<4;
			control2 =  no&0xF0;

			if( dlg.m_bMirror      ) control1 |= 0x01;
			if( dlg.m_bSram        ) control1 |= 0x02;
			if( dlg.m_bTrainer     ) control1 |= 0x04;
			if( dlg.m_bFourScreen  ) control1 |= 0x08;
			if( dlg.m_bVSUnisystem ) control2 |= 0x01;

			if( header.control1 != control1 || header.control2 != control2 ) {
				CHAR	szStr[256];
				CApp::LoadString( IDS_LCH_HEADERREWRITE, szStr, sizeof(szStr) );

				if( ::MessageBox( m_hWnd, szStr, "", MB_YESNO ) == IDYES ) {
					header.control1 = control1;
					header.control2 = control2;

					for( INT i = 0; i < 8; i++ )
						header.reserved[i] = 0;

					LPBYTE	temp = NULL;
					LONG	size;

					try {
						if( (fp = ::fopen( Path.c_str(), "r+b" )) ) {
							// ÉtÉ@ÉCÉãÉTÉCÉYéÊìæ
							::fseek( fp, 0, SEEK_END );
							size = ::ftell( fp );
							::fseek( fp, 0, SEEK_SET );

							// ÉeÉìÉ|ÉâÉäÉÅÉÇÉäämï€
							if( !(temp = (LPBYTE)::malloc( size )) )
								// ÉÅÉÇÉäÇämï€èoóàÇ‹ÇπÇÒ
								throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );

							// ÉTÉCÉYï™ì«Ç›çûÇ›
							if( ::fread( temp, size, 1, fp ) != 1 )
								// ÉtÉ@ÉCÉãÇÃì«Ç›çûÇ›Ç…é∏îsÇµÇ‹ÇµÇΩ
								throw	CApp::GetErrorString( IDS_ERROR_READ );

							::memcpy( temp, &header, sizeof(header) );

							::fseek( fp, 0, SEEK_SET );

							if( ::fwrite( temp, size, 1, fp ) != 1 )
								// ÉtÉ@ÉCÉãÇÃèëÇ´çûÇ›Ç…é∏îsÇµÇ‹ÇµÇΩ
								throw	CApp::GetErrorString( IDS_ERROR_WRITE );

							FCLOSE( fp );
							FREE( temp );
						}

						CheckFile( fl );
						m_bFileLoaded = FALSE;	// ÉZÅ[ÉuÇ∑ÇÈà◊Ç…ÉtÉâÉOè¡Ç∑

						HWND hWndCtrl = ::GetDlgItem( m_hWnd, IDC_LCH_LIST );

						LV_FINDINFO	fi;
						fi.flags  = LVFI_PARAM;
						fi.lParam = (LPARAM)m_SelectPos;
						INT	index = ListView_FindItem( hWndCtrl, -1, &fi );
						if( index >= 0 ) {
							SetListView( index, fl );
						}
					} catch( CHAR* str ) {
						::MessageBox( m_hWnd, str, "ERROR", MB_ICONERROR|MB_OK );
#ifndef	_DEBUG
					} catch(...) {
						::MessageBox( m_hWnd, CApp::GetErrorString( IDS_ERROR_UNKNOWN ), "ERROR", MB_ICONERROR|MB_OK );
#endif
					}
				}
			}
		}
	}
}

BOOL CLauncherDlg::LoadFileList()
{
FILE*	fp = NULL;
CHAR	buf[1024+1];
const UCHAR seps[] = ";\n\0";	// ÉZÉpÉåÅ[É^
FILELIST fl;

//	string	Path = CPathlib::MakePathExt( CApp::GetModulePath(), "launcher", "lst" );
	string	Path = CPathlib::MakePath( CApp::GetModulePath(), "launcher" );
	if( m_nListSelect ) {
		Path = Path + (char)('0'+m_nListSelect) + ".lst";
	} else {
		Path = Path + ".lst";
	}

	m_FileList.clear();
	m_FileListNum = 0;

	if( (fp = ::fopen( Path.c_str(), "r" )) ) {
DEBUGOUT( "Load Launcher File:%s\n", Path.c_str() );
		while( ::fgets( buf, 1024, fp ) ) {
			if( buf[0] == ';' ) {
				continue;
			}

			CHAR*	pToken;

			// File Name
			if( !(pToken = (CHAR*)::_mbstok( (UCHAR*)buf, seps )) )
				continue;
			fl.fname = pToken;

			// Path
			if( !(pToken = (CHAR*)::_mbstok( NULL, seps )) )
				continue;
			fl.path = pToken;

			// Mapper
			if( !(pToken = (CHAR*)::_mbstok( NULL, seps )) )
				continue;
			fl.mapper = ::atoi( pToken );

			// PRG SIZE
			if( !(pToken = (CHAR*)::_mbstok( NULL, seps )) )
				continue;
			fl.prg_size = ::atoi( pToken );

			// CHR SIZE
			if( !(pToken = (CHAR*)::_mbstok( NULL, seps )) )
				continue;
			fl.chr_size = ::atoi( pToken );

			// ALL CRC
			if( !(pToken = (CHAR*)::_mbstok( NULL, seps )) )
				continue;
			fl.crcall = ::strtoul( pToken, NULL, 16 );

			// CRC
			if( !(pToken = (CHAR*)::_mbstok( NULL, seps )) )
				continue;
			fl.crc = ::strtoul( pToken, NULL, 16 );

			// Info
			if( (pToken = (CHAR*)::_mbstok( NULL, seps )) ) {
				fl.info = pToken;
			} else {
				fl.info = "";
			}

			// DB
			if( (pToken = (CHAR*)::_mbstok( NULL, seps )) ) {
				fl.db = pToken;
			} else {
				fl.db = "";
			}

			// TITLE
			if( (pToken = (CHAR*)::_mbstok( NULL, seps )) ) {
				fl.title = pToken;
			} else {
				fl.title = "";
			}

			// Country
			if( (pToken = (CHAR*)::_mbstok( NULL, seps )) ) {
				fl.country = pToken;
			} else {
				fl.country = "";
			}

			// Manufacturer
			if( (pToken = (CHAR*)::_mbstok( NULL, seps )) ) {
				fl.manufacturer = pToken;
			} else {
				fl.manufacturer = "";
			}

			// Sale date
			if( (pToken = (CHAR*)::_mbstok( NULL, seps )) ) {
				fl.saledate = pToken;
			} else {
				fl.saledate = "";
			}

			// Price
			if( (pToken = (CHAR*)::_mbstok( NULL, seps )) ) {
				fl.price = pToken;
			} else {
				fl.price = "";
			}

			// Genre
			if( (pToken = (CHAR*)::_mbstok( NULL, seps )) ) {
				fl.genre = pToken;
			} else {
				fl.genre = "";
			}

			m_FileList.push_back( fl );
			m_FileListNum++;
		}

		FCLOSE( fp );
		return	TRUE;
	}

	return	FALSE;
}

void CLauncherDlg::SaveFileList()
{
FILE*	fp = NULL;

//	string	Path = CPathlib::MakePathExt( CApp::GetModulePath(), "launcher", "lst" );
	string	Path = CPathlib::MakePath( CApp::GetModulePath(), "launcher" );
	if( m_nListSelect ) {
		Path = Path + (char)('0'+m_nListSelect) + ".lst";
	} else {
		Path = Path + ".lst";
	}

	if( !m_FileListNum ) {
DEBUGOUT( "Delete Launcher File:%s\n", Path.c_str() );
		::DeleteFile( Path.c_str() );
		return;
	}

DEBUGOUT( "Save Launcher File:%s\n", Path.c_str() );

	if( (fp = ::fopen( Path.c_str(), "w" )) ) {
		for( INT index = 0; index < m_FileListNum; index++ ) {
			FILELIST& fl = m_FileList[index];
//DEBUGOUT( "%08X:%s\n", index, fl.fname.c_str() );
			::fprintf( fp, "%s;%s;%d;%d;%d;%08X;%08X;%s;%s;%s;%s;%s;%s;%s;%s\n",
				fl.fname.c_str(),
				fl.path.c_str(),
				fl.mapper,
				fl.prg_size,
				fl.chr_size,
				fl.crcall,
				fl.crc,
				fl.info.c_str(),
				fl.db.c_str(),
				fl.title.c_str(),
				fl.country.c_str(),
				fl.manufacturer.c_str(),
				fl.saledate.c_str(),
				fl.price.c_str(),
				fl.genre.c_str() );
		}
		FCLOSE( fp );
	}

	m_FileList.clear();
	m_FileListNum = 0;
}

/////////////////////////////////////////////////////////////////////////////

// ÉÅÉbÉZÅ[ÉW
DLG_MESSAGE_BEGIN(CLchDispEditDlg)
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )
// ÉRÉ}ÉìÉh
DLG_COMMAND_BEGIN()
DLG_ON_COMMAND( IDOK, OnOK )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_ON_COMMAND( IDC_DED_ADD, OnAdd )
DLG_ON_COMMAND( IDC_DED_DEL, OnDel )
DLG_ON_COMMAND( IDC_DED_UP, OnUp )
DLG_ON_COMMAND( IDC_DED_DOWN, OnDown )
DLG_COMMAND_END()
// Notify ÉÅÉbÉZÅ[ÉW
DLG_NOTIFY_BEGIN()
DLG_NOTIFY_END()
DLG_MESSAGE_END()

INT	CLchDispEditDlg::DoModal( HWND hWndParent )
{
	return	::DialogBoxParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_LCH_DISPEDIT),
				hWndParent, g_DlgProc, (LPARAM)this );
}

DLGMSG	CLchDispEditDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CLchDispEditDlg::OnInitDialog\n" );

	INT	i;
	CHAR	szStr[64];
	m_nViewNum = m_nHideNum = 0;
	for( i = 0; i < LAUNCHHEADER_MAX; i++ ) {
		CApp::LoadString( CLauncherDlg::m_HeaderID[Config.launcher.nHeaderOrder[i]], szStr, sizeof(szStr) );
		if( Config.launcher.bHeaderView[Config.launcher.nHeaderOrder[i]] ) {
			::SendDlgItemMessage( m_hWnd, IDC_DED_VIEWLIST, LB_ADDSTRING, 0, (LPARAM)szStr );
			m_nViewOrder[m_nViewNum] = Config.launcher.nHeaderOrder[i];
			m_nViewNum++;
		} else {
			::SendDlgItemMessage( m_hWnd, IDC_DED_HIDELIST, LB_ADDSTRING, 0, (LPARAM)szStr );
			m_nHideOrder[m_nHideNum] = Config.launcher.nHeaderOrder[i];
			m_nHideNum++;
		}
	}
	if( m_nViewNum <= 1 ) {
		::EnableWindow( ::GetDlgItem( m_hWnd, IDC_DED_DEL ), FALSE );
	}
	if( m_nHideNum == 0 ) {
		::EnableWindow( ::GetDlgItem( m_hWnd, IDC_DED_ADD ), FALSE );
	}

	return TRUE;
}

DLGCMD	CLchDispEditDlg::OnAdd( DLGCMDPARAM )
{
//	DEBUGOUT( "CLchDispEditDlg::OnAdd\n" );

	INT nSel = ::SendDlgItemMessage( m_hWnd, IDC_DED_HIDELIST, LB_GETCURSEL, 0, 0 );

	if( nSel != LB_ERR ) {
		CHAR	szStr[64];
		::SendDlgItemMessage( m_hWnd, IDC_DED_HIDELIST, LB_GETTEXT, (WPARAM)nSel, (LPARAM)szStr );

		::SendDlgItemMessage( m_hWnd, IDC_DED_VIEWLIST, LB_ADDSTRING, 0, (LPARAM)szStr );
		::SendDlgItemMessage( m_hWnd, IDC_DED_VIEWLIST, LB_SETCURSEL, (WPARAM)m_nViewNum, 0 );

		m_nViewOrder[m_nViewNum] = m_nHideOrder[nSel];
		m_nViewNum++;

		::SendDlgItemMessage( m_hWnd, IDC_DED_HIDELIST, LB_DELETESTRING, (WPARAM)nSel, 0 );

		for( INT i = nSel; i < m_nHideNum-1; i++ ) {
			m_nHideOrder[i] = m_nHideOrder[i+1];
		}
		m_nHideNum--;
	}

	if( m_nViewNum >= 1 ) {
		::EnableWindow( ::GetDlgItem( m_hWnd, IDC_DED_DEL ), TRUE );
	}
	if( m_nHideNum == 0 ) {
		::EnableWindow( ::GetDlgItem( m_hWnd, IDC_DED_ADD ), FALSE );
	}
}

DLGCMD	CLchDispEditDlg::OnDel( DLGCMDPARAM )
{
//	DEBUGOUT( "CLchDispEditDlg::OnDel\n" );

	INT nSel = ::SendDlgItemMessage( m_hWnd, IDC_DED_VIEWLIST, LB_GETCURSEL, 0, 0 );

	if( nSel != LB_ERR ) {
		CHAR	szStr[64];
		::SendDlgItemMessage( m_hWnd, IDC_DED_VIEWLIST, LB_GETTEXT, (WPARAM)nSel, (LPARAM)szStr );

		::SendDlgItemMessage( m_hWnd, IDC_DED_HIDELIST, LB_ADDSTRING, 0, (LPARAM)szStr );
		::SendDlgItemMessage( m_hWnd, IDC_DED_HIDELIST, LB_SETCURSEL, (WPARAM)m_nHideNum, 0 );

		m_nHideOrder[m_nHideNum] = m_nViewOrder[nSel];
		m_nHideNum++;

		::SendDlgItemMessage( m_hWnd, IDC_DED_VIEWLIST, LB_DELETESTRING, (WPARAM)nSel, 0 );

		for( INT i = nSel; i < m_nViewNum-1; i++ ) {
			m_nViewOrder[i] = m_nViewOrder[i+1];
		}
		m_nViewNum--;
	}

	if( m_nViewNum <= 1 ) {
		::EnableWindow( ::GetDlgItem( m_hWnd, IDC_DED_DEL ), FALSE );
	}
	if( m_nHideNum != 0 ) {
		::EnableWindow( ::GetDlgItem( m_hWnd, IDC_DED_ADD ), TRUE );
	}
}

DLGCMD	CLchDispEditDlg::OnUp( DLGCMDPARAM )
{
//	DEBUGOUT( "CLchDispEditDlg::OnUp\n" );

	INT nSel = ::SendDlgItemMessage( m_hWnd, IDC_DED_VIEWLIST, LB_GETCURSEL, 0, 0 );

	if( nSel != LB_ERR && nSel > 0 ) {
		CHAR	szStr[64];
		::SendDlgItemMessage( m_hWnd, IDC_DED_VIEWLIST, LB_GETTEXT, (WPARAM)nSel, (LPARAM)szStr );
		::SendDlgItemMessage( m_hWnd, IDC_DED_VIEWLIST, LB_DELETESTRING, (WPARAM)nSel, 0 );
		::SendDlgItemMessage( m_hWnd, IDC_DED_VIEWLIST, LB_INSERTSTRING, (WPARAM)nSel-1, (LPARAM)szStr );

		INT	temp = m_nViewOrder[nSel];
		m_nViewOrder[nSel] = m_nViewOrder[nSel-1];
		m_nViewOrder[nSel-1] = temp;

		::SendDlgItemMessage( m_hWnd, IDC_DED_VIEWLIST, LB_SETCURSEL, (WPARAM)nSel-1, 0 );
	}
}

DLGCMD	CLchDispEditDlg::OnDown( DLGCMDPARAM )
{
//	DEBUGOUT( "CLchDispEditDlg::OnDown\n" );

	INT nSel = ::SendDlgItemMessage( m_hWnd, IDC_DED_VIEWLIST, LB_GETCURSEL, 0, 0 );

	if( nSel != LB_ERR && nSel < m_nViewNum-1 ) {
		CHAR	szStr[64];
		::SendDlgItemMessage( m_hWnd, IDC_DED_VIEWLIST, LB_GETTEXT, (WPARAM)nSel, (LPARAM)szStr );
		::SendDlgItemMessage( m_hWnd, IDC_DED_VIEWLIST, LB_DELETESTRING, (WPARAM)nSel, 0 );
		::SendDlgItemMessage( m_hWnd, IDC_DED_VIEWLIST, LB_INSERTSTRING, (WPARAM)nSel+1, (LPARAM)szStr );

		INT	temp = m_nViewOrder[nSel];
		m_nViewOrder[nSel] = m_nViewOrder[nSel+1];
		m_nViewOrder[nSel+1] = temp;

		::SendDlgItemMessage( m_hWnd, IDC_DED_VIEWLIST, LB_SETCURSEL, (WPARAM)nSel+1, 0 );
	}
}

DLGCMD	CLchDispEditDlg::OnOK( DLGCMDPARAM )
{
	for( INT i = 0; i < LAUNCHHEADER_MAX; i++ ) {
		if( i < m_nViewNum ) {
			Config.launcher.nHeaderOrder[i] = m_nViewOrder[i];
			Config.launcher.bHeaderView[m_nViewOrder[i]] = TRUE;
		} else {
			Config.launcher.nHeaderOrder[i] = m_nHideOrder[i-m_nViewNum];
			Config.launcher.bHeaderView[m_nHideOrder[i-m_nViewNum]] = FALSE;
		}
//DEBUGOUT( "%2d:%2d %2d\n", i, Config.launcher.nHeaderOrder[i], Config.launcher.bHeaderView[i] );
	}

	::EndDialog( m_hWnd, IDOK );
}

DLGCMD	CLchDispEditDlg::OnCancel( DLGCMDPARAM )
{
	::EndDialog( m_hWnd, IDCANCEL );
}

///////////////////////////////////////////////////////////////////////////////

#ifndef ListView_SetCheckState
   #define ListView_SetCheckState(hwndLV, i, fCheck) \
      ListView_SetItemState(hwndLV, i, \
      INDEXTOSTATEIMAGEMASK((fCheck)+1), LVIS_STATEIMAGEMASK)
#endif

// ÉÅÉbÉZÅ[ÉW
DLG_MESSAGE_BEGIN(CLchFolderConfigDlg)
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )
// ÉRÉ}ÉìÉh
DLG_COMMAND_BEGIN()
DLG_ON_COMMAND( IDOK, OnOK )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_ON_COMMAND( IDC_LFL_ADD, OnAdd )
DLG_ON_COMMAND( IDC_LFL_DEL, OnDel )
DLG_COMMAND_END()
// Notify ÉÅÉbÉZÅ[ÉW
DLG_NOTIFY_BEGIN()
DLG_NOTIFY_END()
DLG_MESSAGE_END()

INT	CLchFolderConfigDlg::DoModal( HWND hWndParent )
{
	return	::DialogBoxParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_LCH_FOLDER),
				hWndParent, g_DlgProc, (LPARAM)this );
}

DLGMSG	CLchFolderConfigDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CLchFolderConfigDlg::OnInitDialog\n" );

	HWND	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_LFL_LIST );
	ListView_SetExtendedListViewStyle( hWndCtrl, LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );

	// ÉåÉ|Å[ÉgÉÇÅ[ÉhÇ≈ÇÕÉwÉbÉ_Å[Ç™ñ≥Ç≠ÇƒÇ‡ê›íËÇµÇ»Ç¢Ç∆ï\é¶Ç≥ÇÍÇ»Ç¢
	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvcol.fmt = LVCFMT_LEFT;
	lvcol.cx = 1024;
	lvcol.iSubItem = 0;
	lvcol.pszText = "";
	ListView_InsertColumn( hWndCtrl, 0, &lvcol );

	LV_ITEM	lvitem;
	lvitem.mask = LVIF_TEXT;
	lvitem.iSubItem = 0;
	for( INT i = 0; i < 16; i++ ) {
		lvitem.iItem = i;
		if( ::strlen( Config.launcher.szFolder[i] ) ) {
			lvitem.pszText = Config.launcher.szFolder[i];
			ListView_InsertItem( hWndCtrl, &lvitem );
			ListView_SetCheckState( hWndCtrl, i, Config.launcher.bFolderUse[i] );
		} else {
			break;
		}
	}

	return	TRUE;
}

DLGCMD	CLchFolderConfigDlg::OnOK( DLGCMDPARAM )
{
//	DEBUGOUT( "CLchFolderConfigDlg::OnOK\n" );

	HWND	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_LFL_LIST );
	INT	nCount = ListView_GetItemCount( hWndCtrl );

	INT	i;
	for( i = 0; i < nCount; i++ ) {
		ListView_GetItemText( hWndCtrl, i, 0, Config.launcher.szFolder[i], sizeof(Config.launcher.szFolder[i]) );
		Config.launcher.bFolderUse[i] = ListView_GetCheckState( hWndCtrl, i );
	}
	for( ; i < 16; i++ ) {
		Config.launcher.bFolderUse[i] = FALSE;
		Config.launcher.szFolder[i][0] = '\0';
	}

	::EndDialog( m_hWnd, IDOK );
}

DLGCMD	CLchFolderConfigDlg::OnCancel( DLGCMDPARAM )
{
//	DEBUGOUT( "CLchFolderConfigDlg::OnCancel\n" );

	::EndDialog( m_hWnd, IDCANCEL );
}

DLGCMD	CLchFolderConfigDlg::OnAdd( DLGCMDPARAM )
{
	HWND	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_LFL_LIST );

	CHAR	szFolder[_MAX_PATH];
	szFolder[0] = '\0';
	CHAR	szTitle[256];
	CApp::LoadString( IDS_UI_BROWSE, szTitle, sizeof(szTitle) );

	if( CPathlib::SelectFolder( m_hWnd, szTitle, szFolder ) ) {
		LV_ITEM	lvitem;
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = ListView_GetItemCount( hWndCtrl );
		lvitem.iSubItem = 0;
		lvitem.pszText = szFolder;

		ListView_InsertItem( hWndCtrl, &lvitem );
		ListView_SetCheckState( hWndCtrl, lvitem.iItem, TRUE );

		if( lvitem.iItem+1 >= 16 ) {
			::EnableWindow( ::GetDlgItem( m_hWnd, IDC_LFL_ADD ), FALSE );
		}
	}
}

DLGCMD	CLchFolderConfigDlg::OnDel( DLGCMDPARAM )
{
	HWND	hWndCtrl = ::GetDlgItem( m_hWnd, IDC_LFL_LIST );
	INT	nCount = ListView_GetItemCount( hWndCtrl );

	for( INT i = 0; i < nCount; i++ ) {
		if( ListView_GetItemState( hWndCtrl, i, LVIS_SELECTED ) ) {
			ListView_DeleteItem( hWndCtrl, i );
			::EnableWindow( ::GetDlgItem( m_hWnd, IDC_LFL_ADD ), TRUE );
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

// ÉÅÉbÉZÅ[ÉW
DLG_MESSAGE_BEGIN(CLchHeaderEditDlg)
DLG_ON_MESSAGE( WM_INITDIALOG,	OnInitDialog )
// ÉRÉ}ÉìÉh
DLG_COMMAND_BEGIN()
DLG_ON_COMMAND( IDOK, OnOK )
DLG_ON_COMMAND( IDCANCEL, OnCancel )
DLG_COMMAND_END()
// Notify ÉÅÉbÉZÅ[ÉW
DLG_NOTIFY_BEGIN()
DLG_NOTIFY_END()
DLG_MESSAGE_END()

INT	CLchHeaderEditDlg::DoModal( HWND hWndParent )
{
	return	::DialogBoxParam( CApp::GetPlugin(), MAKEINTRESOURCE(IDD_LCH_HEADEREDIT),
				hWndParent, g_DlgProc, (LPARAM)this );
}

DLGMSG	CLchHeaderEditDlg::OnInitDialog( DLGMSGPARAM )
{
//	DEBUGOUT( "CLchHeaderEditDlg::OnInitDialog\n" );

	::SetDlgItemInt( m_hWnd, IDC_HED_EDIT, (UINT)m_nMapperNo, FALSE );

	BTNCHECK( IDC_HED_VMIRROR,    m_bMirror );
	BTNCHECK( IDC_HED_SRAM,        m_bSram );
	BTNCHECK( IDC_HED_4SCREEN,     m_bFourScreen );
	BTNCHECK( IDC_HED_TRAINER,     m_bTrainer );
	BTNCHECK( IDC_HED_VSUNISYSTEM, m_bVSUnisystem );

	return	TRUE;
}

DLGCMD	CLchHeaderEditDlg::OnOK( DLGCMDPARAM )
{
//	DEBUGOUT( "CLchHeaderEditDlg::OnOK\n" );

	m_nMapperNo = (INT)::GetDlgItemInt( m_hWnd, IDC_HED_EDIT, NULL, FALSE );
	if( m_nMapperNo < 0 || m_nMapperNo > 255 ) {
		CHAR	szStr[256];
		CApp::LoadString( IDS_ERROR_ILLEGALMAPPERNO, szStr, sizeof(szStr) );
		::MessageBox( m_hWnd, szStr, "ERROR", MB_ICONERROR|MB_OK );
		return;
	}

	m_bMirror      = IsBTNCHECK( IDC_HED_VMIRROR );
	m_bSram        = IsBTNCHECK( IDC_HED_SRAM );
	m_bFourScreen  = IsBTNCHECK( IDC_HED_4SCREEN );
	m_bTrainer     = IsBTNCHECK( IDC_HED_TRAINER );
	m_bVSUnisystem = IsBTNCHECK( IDC_HED_VSUNISYSTEM );

	::EndDialog( m_hWnd, IDOK );
}

DLGCMD	CLchHeaderEditDlg::OnCancel( DLGCMDPARAM )
{
//	DEBUGOUT( "CLchHeaderEditDlg::OnCancel\n" );

	::EndDialog( m_hWnd, IDCANCEL );
}

