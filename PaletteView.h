//
// パターンビュークラス
//
#ifndef	__CPALETTEVIEW_INCLUDED__
#define	__CPALETTEVIEW_INCLUDED__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
using namespace std;

#include "Wnd.h"

class	CPaletteView : public CWnd
{
public:
	// Constructor/Destructor
	CPaletteView();
	~CPaletteView();

	// Override from CWnd
	BOOL	Create( HWND hWndParent );
	void	Destroy();

protected:
	// Message map
	WND_MESSAGE_MAP()
	WNDMSG		OnCreate( WNDMSGPARAM );
	WNDMSG		OnClose( WNDMSGPARAM );
	WNDMSG		OnDestroy( WNDMSGPARAM );
	WNDMSG		OnTimer( WNDMSGPARAM );
	//

	struct BITMAPHDR {
		BITMAPINFOHEADER bih;
		RGBQUAD		rgb[32];
	} m_BitmapHdr;

	//
	RGBQUAD	m_Palette[256];
	LPBYTE	m_lpPattern;
private:
};

#endif	// !__CPALETTEVIEW_INCLUDED__

