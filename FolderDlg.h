//
// フォルダダイアログクラス
//
#ifndef	__CFOLDERDLG_INCLUDED__
#define	__CFOLDERDLG_INCLUDED__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
using namespace std;

#include "Wnd.h"
#include "Config.h"

class	CFolderDlg : public CWnd
{
public:
	CFolderDlg();
	~CFolderDlg();

	// Override from CWnd
	INT	DoModal( HWND hWndParent );
protected:
	void	OnInitialMember();
	void	PathCheck( LPSTR szPath );

	// Message map
	DLG_MESSAGE_MAP()
	DLGMSG		OnInitDialog( DLGMSGPARAM );
	DLGCMD		OnOK( DLGCMDPARAM );
	DLGCMD		OnCancel( DLGCMDPARAM );
	DLGCMD		OnDefault( DLGCMDPARAM );
	DLGCMD		OnRomBrowse( DLGCMDPARAM );
	DLGCMD		OnSaveBrowse( DLGCMDPARAM );
	DLGCMD		OnStateBrowse( DLGCMDPARAM );
	DLGCMD		OnSnapshotBrowse( DLGCMDPARAM );
	DLGCMD		OnMovieBrowse( DLGCMDPARAM );
	DLGCMD		OnWaveBrowse( DLGCMDPARAM );
	DLGCMD		OnCheatBrowse( DLGCMDPARAM );
	//

	CCfgPath	m_ConfigSave;
private:
	HFONT		m_hFont;
};

#endif	// !__CFOLDERDLG_INCLUDED__

