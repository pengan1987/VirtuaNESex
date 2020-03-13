//
// ROM情報ダイアログクラス
//
#ifndef	__CROMINFODLG_INCLUDED__
#define	__CROMINFODLG_INCLUDED__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
using namespace std;

#include "Wnd.h"

class	CRomInfoDlg : public CWnd
{
public:
	CRomInfoDlg() {
		m_szName[0] = '\0';
		m_nMapper = 0;
		m_nPRG = 0;
		m_nCHR = 0;
		m_bMirror = FALSE;
		m_bSram = FALSE;
		m_b4Screen = FALSE;
		m_bTrainer = FALSE;
		m_bVSUnisystem = FALSE;
		m_dwCRC    = 0;
		m_dwCRCALL = 0;
		m_dwCRCCHR = 0;
	}

	// Override from CWnd
	INT	DoModal( HWND hWndParent );

	CHAR	m_szName[_MAX_PATH];
	INT	m_nMapper;
	INT	m_nPRG;
	INT	m_nCHR;
	BOOL	m_bMirror;
	BOOL	m_bSram;
	BOOL	m_b4Screen;
	BOOL	m_bTrainer;
	BOOL	m_bVSUnisystem;
	DWORD	m_dwCRC;
	DWORD	m_dwCRCALL;
	DWORD	m_dwCRCCHR;
protected:
	// Message map
	DLG_MESSAGE_MAP()
	DLGMSG		OnInitDialog( DLGMSGPARAM );
	DLGCMD		OnOK( DLGCMDPARAM );
	DLGCMD		OnCancel( DLGCMDPARAM );
	//
private:
};

#endif	// !__CROMINFODLG_INCLUDED__

