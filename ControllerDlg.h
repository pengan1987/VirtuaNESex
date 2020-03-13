//
// コントローラダイアログクラス
//
#ifndef	__CCONTROLLERDLG_INCLUDED__
#define	__CCONTROLLERDLG_INCLUDED__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
using namespace std;

#include "Wnd.h"
#include "Config.h"

class	CControllerPageDlg : public CWnd
{
public:
	BOOL	Create( UINT nID, HWND hWndParent );
	// Override from CWnd
	void	Destroy();

protected:
	// Message map
	DLG_MESSAGE_MAP()
	DLGMSG		OnInitDialog( DLGMSGPARAM );
	DLGCMD		OnClicked( DLGCMDPARAM );
	//

	UINT	m_nPageID;
private:
};

class	CControllerDlg : public CWnd
{
public:
	// Override from CWnd
	INT	DoModal( HWND hWndParent );

	// Override from CWnd
	BOOL	PreTranslateMessage( MSG* pMsg );
protected:
	void	OnInitialMember();
	void	OnPageSetup( UINT nPage );

	// Message map
	DLG_MESSAGE_MAP()
	DLGMSG		OnInitDialog( DLGMSGPARAM );
	DLGMSG		OnDestroy( DLGMSGPARAM );
	DLGMSG		OnTimer( DLGMSGPARAM );
	DLGNOTIFY	OnSelectChange( DLGNOTIFYPARAM );
	DLGNOTIFY	OnSettingSelectChange( DLGCMDPARAM );
	DLGCMD		OnClicked( DLGCMDPARAM );
	DLGCMD		OnOK( DLGCMDPARAM );
	DLGCMD		OnCancel( DLGCMDPARAM );
	DLGCMD		OnDefault( DLGCMDPARAM );
	//

	enum { PAGE_MAX=12 };

	BOOL	m_bCancelMode;

	INT	m_ControlID;
	INT	m_TimerID;
	INT	m_TimerStart;
	INT	m_TimerCount;

	INT	m_SettingSel;

	INT	m_PageSel;
	INT	m_PageNum;
	CControllerPageDlg m_Page[PAGE_MAX];

	CCfgController	m_ConfigSave;

	static	UINT	ControllerPageID[];

private:
};

#endif	// !__CCONTROLLERDLG_INCLUDED__

