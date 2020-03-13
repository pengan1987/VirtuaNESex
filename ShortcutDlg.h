//
// ショートカットダイアログクラス
//
#ifndef	__CSHORTCUTDLG_INCLUDED__
#define	__CSHORTCUTDLG_INCLUDED__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
using namespace std;

#include "Wnd.h"
#include "Config.h"

class	CShortcutDlg : public CWnd
{
public:
	// Override from CWnd
	INT	DoModal( HWND hWndParent );

	// Override from CWnd
	BOOL	PreTranslateMessage( MSG* pMsg );
protected:

	void	OnInitialMember();

	// Message map
	DLG_MESSAGE_MAP()
	DLGMSG		OnInitDialog( DLGMSGPARAM );
	DLGMSG		OnTimer( DLGMSGPARAM );
	DLGNOTIFY	OnListDblClick( DLGNOTIFYPARAM );
	DLGNOTIFY	OnListItemChanged( DLGNOTIFYPARAM );

	DLGCMD		OnOK( DLGCMDPARAM );
	DLGCMD		OnCancel( DLGCMDPARAM );
	DLGCMD		OnDefault( DLGCMDPARAM );
	//

	CHAR	m_szWindowTitle[256];
	CHAR	m_szMessage[256];

	BOOL	m_bCancelMode;
	INT	m_SelectPos;
	INT	m_SelectID;
	INT	m_SelectSubID;
	INT	m_TimerID;
	INT	m_TimerCount;

	INT		m_ControlBuf [128];
	INT		m_ControlBuf2[128];
	CCfgShortCut	m_ConfigSave;

private:
};

#endif	// !__CSHORTCUTDLG_INCLUDED__

