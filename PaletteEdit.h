//
// パレットエディタクラス
//
#ifndef	__CPALETTEEDIT_INCLUDED__
#define	__CPALETTEEDIT_INCLUDED__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
using namespace std;

#include "Wnd.h"

class	CPaletteEdit : public CWnd
{
public:
	// Constructor/Destructor
	CPaletteEdit();
	~CPaletteEdit();

	// Override from CWnd
	BOOL	Create( HWND hWndParent );
	void	Destroy();

protected:
	// Message map
	DLG_MESSAGE_MAP()
	DLGMSG		OnInitDialog( DLGMSGPARAM );
	DLGMSG		OnDestroy( DLGMSGPARAM );
	DLGMSG		OnActivate( DLGMSGPARAM );
	DLGMSG		OnLButtonDown( DLGMSGPARAM );
	DLGMSG		OnRButtonDown( DLGMSGPARAM );
	DLGMSG		OnHScroll( DLGMSGPARAM );
	DLGMSG		OnPaint( DLGMSGPARAM );
	DLGMSG		OnTimer( DLGMSGPARAM );

	DLGCMD		OnOK( DLGCMDPARAM );
	DLGCMD		OnCancel( DLGCMDPARAM );
	DLGCMD		OnDefault( DLGCMDPARAM );

	DLGCMD		OnRGBChange( DLGCMDPARAM );
	DLGCMD		OnUndo( DLGCMDPARAM );
	DLGCMD		OnLoad( DLGCMDPARAM );
	DLGCMD		OnSave( DLGCMDPARAM );
	//

	void	OnChaneSelect();
	void	OnUpdateColor();

	INT	m_PaletteSelect;
	RGBQUAD	m_RGB;
	BYTE	m_BGPAL[16];
	BYTE	m_SPPAL[16];

	enum	{
		CELL_SIZE = 22,
		CELL_H = 16,
		CELL_V = 4,
	};

	RGBQUAD	m_Palette[256];
	RGBQUAD	m_PaletteUndo[256];
	RGBQUAD	m_PaletteDefault[256];

	//
	BOOL	m_bShortCutDisable;
private:
};

#endif	// !__CPALETTEEDIT_INCLUDED__

