//
// ランゲージダイアログクラス
//
#ifndef	__CLANGUAGEDLG_INCLUDED__
#define	__CLANGUAGEDLG_INCLUDED__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
using namespace std;

#include "Wnd.h"

class	CLanguageDlg : public CWnd
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
	DLGCMD		OnDefault( DLGCMDPARAM );
	//

	INT	m_nPluginID;
private:
};

#endif	// !__CLANGUAGEDLG_INCLUDED__

