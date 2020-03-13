//
// メモリビュークラス
//
#ifndef	__CMEMORYVIEW_INCLUDED__
#define	__CMEMORYVIEW_INCLUDED__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
using namespace std;

#include "Wnd.h"

class	CMemoryView : public CWnd
{
public:
	// Constructor/Destructor
	CMemoryView();
	~CMemoryView();

	// Override from CWnd
	BOOL	Create( HWND hWndParent );
	void	Destroy();

protected:
	// Message map
	WND_MESSAGE_MAP()
	WNDMSG		OnCreate( WNDMSGPARAM );
	WNDMSG		OnClose( WNDMSGPARAM );
	WNDMSG		OnDestroy( WNDMSGPARAM );
	WNDMSG		OnActivate( WNDMSGPARAM );
	WNDMSG		OnSize( WNDMSGPARAM );
	WNDMSG		OnLButtonDown( WNDMSGPARAM );
	WNDMSG		OnMouseWheel( WNDMSGPARAM );
	WNDMSG		OnChar( WNDMSGPARAM );
	WNDMSG		OnKeyDown( WNDMSGPARAM );
	WNDMSG		OnVScroll( WNDMSGPARAM );
	WNDMSG		OnPaint( WNDMSGPARAM );
	WNDMSG		OnTimer( WNDMSGPARAM );
	//

	void	OnDraw( HDC hDC );

	static	LOGFONT	m_logFont;
	HFONT	m_hFont;

	INT	m_StartAddress;
	INT	m_DispLines;

	INT	m_CursorX, m_CursorY;
private:
};

#endif	// !__CMEMORYVIEW_INCLUDED__

