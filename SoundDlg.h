//
// サウンドダイアログクラス
//
#ifndef	__CSOUNDDLG_INCLUDED__
#define	__CSOUNDDLG_INCLUDED__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
using namespace std;

#include "Wnd.h"
#include "Config.h"

class	CSoundDlg : public CWnd
{
public:
	// Override from CWnd
	INT	DoModal( HWND hWndParent );
protected:
	void	OnInitialMember();

	// Message map
	DLG_MESSAGE_MAP()
	DLGMSG		OnInitDialog( DLGMSGPARAM );
	DLGCMD		OnOK( DLGCMDPARAM );
	DLGCMD		OnCancel( DLGCMDPARAM );
	DLGCMD		OnDefault( DLGCMDPARAM );
	//

	CCfgSound	m_ConfigSave;
private:
	static	UINT	m_nVolumeID[];
};

#endif	// !__CSOUNDDLG_INCLUDED__

