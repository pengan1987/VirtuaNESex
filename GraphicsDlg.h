//
// グラフィックスダイアログクラス
//
#ifndef	__CGRAPHICSDLG_INCLUDED__
#define	__CGRAPHICSDLG_INCLUDED__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
using namespace std;

#include "Wnd.h"
#include "Config.h"

class	CGraphicsDlg : public CWnd
{
public:
	// Override from CWnd
	INT	DoModal( HWND hWndParent );
protected:
	void	OnInitialMember();
	void	OnUpdateComboBox();

	// Message map
	DLG_MESSAGE_MAP()
	DLGMSG		OnInitDialog( DLGMSGPARAM );
	DLGMSG		OnHScroll( DLGMSGPARAM );
	DLGCMD		OnOK( DLGCMDPARAM );
	DLGCMD		OnCancel( DLGCMDPARAM );
	DLGCMD		OnDefault( DLGCMDPARAM );
	DLGCMD		OnPaletteBrowse( DLGCMDPARAM );
	DLGCMD		OnPaletteUseClick( DLGCMDPARAM );
	DLGCMD		OnNoSquareListClick( DLGCMDPARAM );
	//

	CCfgGraphics	m_ConfigSave;

private:
};

#endif	// !__CGRAPHICSDLG_INCLUDED__

