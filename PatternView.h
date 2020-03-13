//
// パターンビュークラス
//
#ifndef	__CPATTERNVIEW_INCLUDED__
#define	__CPATTERNVIEW_INCLUDED__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
using namespace std;

#include "Wnd.h"

class	CPatternView : public CWnd
{
public:
	// Constructor/Destructor
	CPatternView();
	~CPatternView();

	// Override from CWnd
	BOOL	Create( HWND hWndParent );
	void	Destroy();

protected:
	// Message map
	WND_MESSAGE_MAP()
	WNDMSG		OnCreate( WNDMSGPARAM );
	WNDMSG		OnClose( WNDMSGPARAM );
	WNDMSG		OnDestroy( WNDMSGPARAM );
	WNDMSG		OnLButtonDown( WNDMSGPARAM );
	WNDMSG		OnTimer( WNDMSGPARAM );
	//

	struct BITMAPHDR {
		BITMAPINFOHEADER bih;
		RGBQUAD		rgb[16];
	} m_BitmapHdr;

	//
	INT	m_SelectPal;
	RGBQUAD	m_Palette[256];
	LPBYTE	m_lpBank[8];
	LPBYTE	m_lpPattern;
private:
};

#endif	// !__CPATTERNVIEW_INCLUDED__

