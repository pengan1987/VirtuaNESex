//
// DATACHバーコードバトラーダイアログクラス
//
#ifndef	__CDATACHBARCODEDLG_INCLUDED__
#define	__CDATACHBARCODEDLG_INCLUDED__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <list>
#include <vector>
#include <string>
using namespace std;

#include "Wnd.h"

class	CDatachBarcodeDlg : public CWnd
{
public:
	// Override from CWnd
	BOOL	Create( HWND hWndParent );
	void	Destroy();

protected:
	// Message map
	DLG_MESSAGE_MAP()
	DLGMSG		OnInitDialog( DLGMSGPARAM );
	DLGMSG		OnActivate( DLGMSGPARAM );
	DLGMSG		OnDestroy( DLGMSGPARAM );
	DLGMSG		OnClose( DLGMSGPARAM );
	DLGCMD		OnCancel( DLGCMDPARAM );
	DLGCMD		OnCodeinput( DLGCMDPARAM );
	DLGCMD		OnCodeTransfer( DLGCMDPARAM );
	DLGCMD		OnCodeCreate( DLGCMDPARAM );
	//
private:
};

#endif	// !__CDATACHBARCODEDLG_INCLUDED__

