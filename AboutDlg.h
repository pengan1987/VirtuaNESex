//
// バージョンダイアログクラス
//
#ifndef	__CABOUTDLG_INCLUDED__
#define	__CABOUTDLG_INCLUDED__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
using namespace std;

#include "Wnd.h"
#include "CHyperLink.h"

class	CAboutDlg : public CWnd
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
	DLGCMD		OnWebsite( DLGCMDPARAM );
	//

	CHyperLink	m_Website;
	CHyperLink	m_Email;
private:
};

#endif	// !__CABOUTDLG_INCLUDED__

