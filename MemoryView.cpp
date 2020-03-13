//
// メモリビュークラス
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <mbstring.h>
#include <string>
using namespace std;

#include "VirtuaNESres.h"
#include "DebugOut.h"
#include "App.h"
#include "Pathlib.h"

#include "Wnd.h"
#include "MemoryView.h"
#include "EmuThread.h"

#include "MMU.h"

#include "DirectDraw.h"

#define	OFFSETH		4
#define	OFFSETV		4

#define	FONTWIDTH	6
#define	FONTHEIGHT	12

LOGFONT	CMemoryView::m_logFont={ FONTHEIGHT, FONTWIDTH, 0, 0, 0, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_TT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH|FF_DONTCARE, NULL };

#ifndef	WM_MOUSEWHEEL
#define	WM_MOUSEWHEEL	0x020A
#endif

WND_MESSAGE_BEGIN(CMemoryView)
WND_ON_MESSAGE( WM_CREATE,	OnCreate )
WND_ON_MESSAGE( WM_CLOSE,	OnClose )
WND_ON_MESSAGE( WM_DESTROY,	OnDestroy )
WND_ON_MESSAGE( WM_ACTIVATE,	OnActivate )
WND_ON_MESSAGE( WM_SIZE,	OnSize )
WND_ON_MESSAGE( WM_LBUTTONDOWN,	OnLButtonDown )
WND_ON_MESSAGE( WM_MOUSEWHEEL,	OnMouseWheel )
WND_ON_MESSAGE( WM_CHAR,	OnChar )
WND_ON_MESSAGE( WM_KEYDOWN,	OnKeyDown )
WND_ON_MESSAGE( WM_VSCROLL,	OnVScroll )
WND_ON_MESSAGE( WM_PAINT,	OnPaint )
WND_ON_MESSAGE( WM_TIMER,	OnTimer )

WND_COMMAND_BEGIN()
WND_COMMAND_END()
WND_MESSAGE_END()

CMemoryView::CMemoryView()
{
	m_hFont = NULL;
	m_StartAddress = 0;

	m_CursorX = m_CursorY = 0;
	m_DispLines = 16;
}

CMemoryView::~CMemoryView()
{
}

BOOL	CMemoryView::Create( HWND hWndParent )
{
	m_logFont.lfCharSet = SHIFTJIS_CHARSET;
	if( !(m_hFont = ::CreateFontIndirect( &m_logFont )) ) {
		m_logFont.lfCharSet = ANSI_CHARSET;
		if( !(m_hFont = ::CreateFontIndirect( &m_logFont )) ) {
			return	FALSE;
		}
	}

	HWND hWnd = ::CreateWindowEx(
			WS_EX_TOOLWINDOW,
			VIRTUANES_WNDCLASS,
			"MemoryView",
			WS_OVERLAPPEDWINDOW|WS_VSCROLL,	// Window拡大縮小可能
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			hWndParent,
			NULL,
			CApp::GetInstance(),
			(LPVOID)this		// This を埋め込む為
		);
	if( !hWnd ) {
		DEBUGOUT( "CreateWindow faild.\n" );
		return	FALSE;
	}

	m_hWnd = hWnd;

	return	TRUE;
}

void	CMemoryView::Destroy()
{
	if( m_hWnd && IsWindow(m_hWnd) ) {
		// 位置保存
		::GetWindowRect( m_hWnd, &Config.general.rcMemoryViewPos );

		::DestroyWindow( m_hWnd );
		m_hWnd = NULL;
	}

	if( m_hFont ) {
		::DeleteObject( m_hFont );
		m_hFont = NULL;
	}
}

WNDMSG	CMemoryView::OnCreate( WNDMSGPARAM )
{
//	DEBUGOUT( "CMemoryView::OnCreate\n" );

	// ウインドウサイズの設定
	if( RCWIDTH(Config.general.rcMemoryViewPos) > 0 && RCHEIGHT(Config.general.rcMemoryViewPos) > 0 ) {
		::MoveWindow( m_hWnd, Config.general.rcMemoryViewPos.left, Config.general.rcMemoryViewPos.top,
			 RCWIDTH(Config.general.rcMemoryViewPos), RCHEIGHT(Config.general.rcMemoryViewPos), FALSE );
	} else {
		RECT	rw, rc;
		::GetWindowRect( m_hWnd, &rw );
		::GetClientRect( m_hWnd, &rc );
		INT	x = rw.right - rw.left - rc.right + OFFSETH*2+FONTWIDTH*71; 
		INT	y = rw.bottom - rw.top - rc.bottom + OFFSETV*2+FONTHEIGHT*18;
		::MoveWindow( m_hWnd, 0, 0, x, y, FALSE );
	}

	RECT	rc;
	::GetClientRect( m_hWnd, &rc );

	// スクロールレンジの設定
	m_DispLines = (RCHEIGHT(rc)-(OFFSETV*2+FONTHEIGHT*2))/FONTHEIGHT;
	if( m_DispLines < 0 )
		m_DispLines = 0;

	DEBUGOUT( "Display Lines:%d\n", m_DispLines );
	DEBUGOUT( "Scroll Max   :%d\n", (0xFFF-m_DispLines)<0?0:(0xFFF-m_DispLines) );

	SCROLLINFO	sif;
	::ZeroMemory( &sif, sizeof(sif) );
	sif.cbSize = sizeof(sif);
	sif.fMask  = SIF_ALL;
	sif.nMin   = 0;
	sif.nMax   = (0x1000-m_DispLines);
	sif.nPos   = 0;
	sif.nPage  = 1;
	sif.nTrackPos = 1;
	::SetScrollInfo( m_hWnd, SB_VERT, &sif, TRUE );
	m_StartAddress = 0;

	m_CursorX = m_CursorY = 0;

	// 表示
	::ShowWindow( m_hWnd, SW_SHOW );

	// タイマー起動
	::SetTimer( m_hWnd, 1, 50, NULL );
	return	TRUE;
}

WNDMSG	CMemoryView::OnClose( WNDMSGPARAM )
{
//	DEBUGOUT( "CMemoryView::OnClose\n" );
	::KillTimer( m_hWnd, 1 );
	::DestroyWindow( m_hWnd );
	return	TRUE;
}

WNDMSG	CMemoryView::OnDestroy( WNDMSGPARAM )
{
//	DEBUGOUT( "CMemoryView::OnDestroy\n" );
	// 位置保存
	::GetWindowRect( m_hWnd, &Config.general.rcMemoryViewPos );

	m_hWnd = NULL;
	return	TRUE;
}

WNDMSG	CMemoryView::OnActivate( WNDMSGPARAM )
{
	if( LOWORD(wParam) == WA_INACTIVE ) {
//		DEBUGOUT( "CMemoryView::OnActivate:Inactive\n" );
		::PostMessage( CApp::GetHWnd(), WM_VNS_SHORTCUTENABLE, (WPARAM)TRUE, 0 );
	} else {
//		DEBUGOUT( "CMemoryView::OnActivate:Active\n" );
		::PostMessage( CApp::GetHWnd(), WM_VNS_SHORTCUTENABLE, (WPARAM)FALSE, 0 );
	}
	return	TRUE;
}

WNDMSG	CMemoryView::OnSize( WNDMSGPARAM )
{
//	DEBUGOUT( "CMemoryView::OnSize\n" );

	m_DispLines = (HIWORD(lParam)-(OFFSETV*2+FONTHEIGHT*2))/FONTHEIGHT;
	if( m_DispLines < 0 )
		m_DispLines = 0;

//	DEBUGOUT( "Display Lines:%d\n", m_DispLines );

	SCROLLINFO	sif;
	::ZeroMemory( &sif, sizeof(sif) );
	sif.cbSize = sizeof(sif);
	sif.fMask  = SIF_POS;
	::GetScrollInfo( m_hWnd, SB_VERT, &sif );

	sif.fMask  = SIF_POS|SIF_RANGE;
	sif.nMin   = 0;
	sif.nMax   = (0x1000-m_DispLines)<0?0:(0x1000-m_DispLines);
	if( sif.nMin > sif.nPos )
		sif.nPos = sif.nMin;
	if( sif.nMax < sif.nPos )
		sif.nPos = sif.nMax;
	::SetScrollInfo( m_hWnd, SB_VERT, &sif, TRUE );

	m_StartAddress = sif.nPos * 0x10;

	if( m_CursorY > m_DispLines-1 )
		m_CursorY = m_DispLines-1;
	if( m_CursorY < 0 )
		m_CursorY = 0;

	return	TRUE;
}

WNDMSG	CMemoryView::OnLButtonDown( WNDMSGPARAM )
{
//	DEBUGOUT( "CMemoryView::OnLButtonDown\n" );

	INT	x = LOWORD(lParam);
	INT	y = HIWORD(lParam);

	x -= OFFSETH+FONTWIDTH*6;
	y -= OFFSETV+FONTHEIGHT*2;

	if( x >= 0 && x < FONTWIDTH*(3*16)
	 && y >= 0 && y < FONTHEIGHT*m_DispLines ) {
		if( (x%(FONTWIDTH*3)) < FONTWIDTH*2 ) {
			m_CursorX = (x / (FONTWIDTH*3))*2;
			m_CursorY = y / FONTHEIGHT;
		}
	}

	return	TRUE;
}

WNDMSG	CMemoryView::OnMouseWheel( WNDMSGPARAM )
{
//	DEBUGOUT( "CMemoryView::OnMouseWheel\n" );
	short	zDelta = (short)HIWORD(wParam);
	if( zDelta > 0 ) {
		while( zDelta > 0 ) {
			if( LOWORD(wParam) == MK_CONTROL )
				::SendMessage( hWnd, WM_VSCROLL, MAKELONG(SB_PAGEUP, 0), 0L );
			else
				::SendMessage( hWnd, WM_VSCROLL, MAKELONG(SB_LINEUP, 0), 0L );

			zDelta -= 30;
		}
		return	TRUE;
	} else {
		while( zDelta < 0 ) {
			if( LOWORD(wParam) == MK_CONTROL )
				::SendMessage( hWnd, WM_VSCROLL, MAKELONG(SB_PAGEDOWN, 0), 0L );
			else
				::SendMessage( hWnd, WM_VSCROLL, MAKELONG(SB_LINEDOWN, 0), 0L );
			zDelta += 30;
		}
	}
	return	TRUE;
}

WNDMSG	CMemoryView::OnChar( WNDMSGPARAM )
{
//	DEBUGOUT( "CMemoryView::OnChar\n" );

	WORD	wScrollNotify = 0xFFFF;

	CHAR	Keys = ::toupper((CHAR)wParam);

	switch( Keys ) {
		case	'0':
		case	'1':
		case	'2':
		case	'3':
		case	'4':
		case	'5':
		case	'6':
		case	'7':
		case	'8':
		case	'9':
			if( m_DispLines ) {
				INT	addr = m_StartAddress+(m_CursorX/2)+m_CursorY*16;
				BYTE	data = CPU_MEM_BANK[addr>>13][addr&0x1FFF];
				if( !(m_CursorX&1) ) {
					data &= 0x0F;
					data |= (Keys-'0')<<4;
				} else {
					data &= 0xF0;
					data |= (Keys-'0');
				}
				CPU_MEM_BANK[addr>>13][addr&0x1FFF] = data;

				if( ++m_CursorX > 32-1 ) {
					if( ++m_CursorY > m_DispLines-1 ) {
						wScrollNotify = SB_LINEDOWN;
						m_CursorY = m_DispLines-1;
					}
					if( (m_CursorX/2)+m_CursorY*16+m_StartAddress > 0xFFFF ) {
						m_CursorX = 32-1;
					} else {
						m_CursorX = 0;
					}
				}
			}
			break;

		case	'A':
		case	'B':
		case	'C':
		case	'D':
		case	'E':
		case	'F':
			if( m_DispLines ) {
				INT	addr = m_StartAddress+(m_CursorX/2)+m_CursorY*16;
				BYTE	data = CPU_MEM_BANK[addr>>13][addr&0x1FFF];
				if( !(m_CursorX&1) ) {
					data &= 0x0F;
					data |= (Keys-'A'+10)<<4;
				} else {
					data &= 0xF0;
					data |= (Keys-'A'+10);
				}
				CPU_MEM_BANK[addr>>13][addr&0x1FFF] = data;
				if( ++m_CursorX > 32-1 ) {
					if( ++m_CursorY > m_DispLines-1 ) {
						wScrollNotify = SB_LINEDOWN;
						m_CursorY = m_DispLines-1;
					}
					if( (m_CursorX/2)+m_CursorY*16+m_StartAddress > 0xFFFF ) {
						m_CursorX = 32-1;
					} else {
						m_CursorX = 0;
					}
				}
			}
			break;
		default:
			break;
	}
	if( wScrollNotify != 0xFFFF ) 
		::SendMessage( hWnd, WM_VSCROLL, MAKELONG(wScrollNotify, 0), 0L ); 

	return	TRUE;
}

WNDMSG	CMemoryView::OnKeyDown( WNDMSGPARAM )
{
//	DEBUGOUT( "CMemoryView::OnKeyDown\n" );

	WORD	wScrollNotify = 0xFFFF;

	switch( wParam ) {
		case	VK_UP:
			m_CursorX &= ~1;
			if( --m_CursorY < 0 ) {
				wScrollNotify = SB_LINEUP;
				m_CursorY = 0;
			}
			break;
		case	VK_DOWN:
			m_CursorX &= ~1;
			if( ++m_CursorY > m_DispLines-1 ) {
				wScrollNotify = SB_LINEDOWN;
				m_CursorY = m_DispLines-1;
			}
			break;
		case	VK_LEFT:
			if( m_CursorX & 1 ) {
				m_CursorX &= ~1;
			} else
			if( (m_CursorX-=2) < 0 ) {
				if( --m_CursorY < 0 ) {
					wScrollNotify = SB_LINEUP;
					m_CursorX = 0;
					m_CursorY = 0;
				} else {
					m_CursorX = 32-1;
				}
			}
			break;
		case	VK_RIGHT:
			m_CursorX &= ~1;
			if( (m_CursorX+=2) > 32-1 ) {
				if( ++m_CursorY > m_DispLines-1 ) {
					wScrollNotify = SB_LINEDOWN;
					m_CursorY = m_DispLines-1;
				}
				if( (m_CursorX/2)+m_CursorY*16+m_StartAddress > 0xFFFF ) {
					m_CursorX = 32-1;
				} else {
					m_CursorX = 0;
				}
			}
			break;

		case	VK_PRIOR:
			m_CursorX &= ~1;
			if( (m_CursorY-16) < 0 ) {
				wScrollNotify = SB_PAGEUP;
				if( m_StartAddress <= 0 ) {
					m_CursorY = 0;
				}
			} else {
				m_CursorY-= 16;
			}
			break;
		case	VK_NEXT:
			m_CursorX &= ~1;
			if( (m_CursorY+16) > m_DispLines-1 ) {
				wScrollNotify = SB_PAGEDOWN;
				if( m_StartAddress+m_DispLines*16 >= 0xFFFF ) {
					m_CursorY = m_DispLines-1;
				}
			} else {
				m_CursorY += 16;
			}
			break;
		case	VK_HOME:
			wScrollNotify = SB_TOP;
			m_CursorY = 0;
			break;
		case	VK_END:
			wScrollNotify = SB_BOTTOM;
			m_CursorY = m_DispLines-1;
			break;
		default:
			break;
	}
	if( wScrollNotify != 0xFFFF ) 
		::SendMessage( hWnd, WM_VSCROLL, MAKELONG(wScrollNotify, 0), 0L ); 

	return	TRUE;
}

WNDMSG	CMemoryView::OnVScroll( WNDMSGPARAM )
{
//	DEBUGOUT( "CMemoryView::OnVScroll\n" );

	SCROLLINFO	sif;
	::ZeroMemory( &sif, sizeof(sif) );
	sif.cbSize = sizeof(sif);
	sif.fMask  = SIF_POS|SIF_RANGE;
	::GetScrollInfo( m_hWnd, SB_VERT, &sif );

	switch( LOWORD(wParam) ) {
		case	SB_TOP:
			sif.nPos = 0;
			break;
		case	SB_BOTTOM:
			sif.nPos = sif.nMax;
			break;
		case	SB_PAGEUP:
			sif.nPos -= 16;
			break;
		case	SB_PAGEDOWN:
			sif.nPos += 16;
			break;
		case	SB_LINEUP:
			sif.nPos--;
			break;
		case	SB_LINEDOWN:
			sif.nPos++;
			break;
		case	SB_THUMBTRACK:
			sif.nPos = HIWORD(wParam);
			break;
		default:
			break;
	}
	if( sif.nMin > sif.nPos )
		sif.nPos = sif.nMin;
	if( sif.nMax < sif.nPos )
		sif.nPos = sif.nMax;
	sif.fMask  = SIF_POS;
	::SetScrollInfo( m_hWnd, SB_VERT, &sif, TRUE );

	m_StartAddress = sif.nPos * 0x10;

	return	TRUE;
}

WNDMSG	CMemoryView::OnPaint( WNDMSGPARAM )
{
//	DEBUGOUT( "CMemoryView::OnPaint\n" );

	PAINTSTRUCT	ps;
	HDC	hDC = ::BeginPaint( hWnd, &ps );
	RECT	rc;
	::GetClientRect( m_hWnd, &rc ); 
	::FillRect( hDC, &rc, (HBRUSH)::GetStockObject(WHITE_BRUSH) ); 
	OnDraw( hDC );
	::EndPaint( hWnd, &ps );

	return	TRUE;
}

WNDMSG	CMemoryView::OnTimer( WNDMSGPARAM )
{
	if( !Emu.IsRunning() )
		return	TRUE;

	HDC	hDC = ::GetDC( m_hWnd );
	OnDraw( hDC );
	::ReleaseDC( m_hWnd, hDC );

	return	TRUE;
}

void	CMemoryView::OnDraw( HDC hDC )
{
	RECT	rc;
	::GetClientRect( m_hWnd, &rc ); 

	HFONT	hFontOld = (HFONT)::SelectObject( hDC, m_hFont );

	::SetBkMode( hDC, OPAQUE );
	::TextOut( hDC, OFFSETH, OFFSETV+FONTHEIGHT* 0, "ADDR  +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F  0123456789ABCDEF", 71 );
	::TextOut( hDC, OFFSETH, OFFSETV+FONTHEIGHT* 1, "-----------------------------------------------------------------------", 71 );

	CHAR	szBuf[256];
	INT	address = m_StartAddress;
	for( INT i = 0; i < m_DispLines; i++ ) {
		::wsprintf( szBuf, "%04X  ", address&0xFFFF );

		for( INT d = 0; d < 16; d++ ) {
			CHAR	szTemp[16];
			INT	addr = address+d;
			::wsprintf( szTemp, "%02X ", CPU_MEM_BANK[addr>>13][addr&0x1FFF] );
			::strcat( szBuf, szTemp );
		}
		::strcat( szBuf, " " );
		for( INT a = 0; a < 16; a++ ) {
			CHAR	szTemp[16];
			INT	addr = address+a;

			if( m_logFont.lfCharSet == SHIFTJIS_CHARSET ) {
				::wsprintf( szTemp, "%1c", ::_ismbcprint(CPU_MEM_BANK[addr>>13][addr&0x1FFF])?CPU_MEM_BANK[addr>>13][addr&0x1FFF]:'.' );
			} else {
				::wsprintf( szTemp, "%1c", ::isprint(CPU_MEM_BANK[addr>>13][addr&0x1FFF])?CPU_MEM_BANK[addr>>13][addr&0x1FFF]:'.' );
			}
			::strcat( szBuf, szTemp );
		}

		::TextOut( hDC, OFFSETH, OFFSETV+FONTHEIGHT*(2+i), szBuf, ::strlen(szBuf) );

		address += 16;
		address &= 0xFFFF;
	}

	::TextOut( hDC, OFFSETH, OFFSETV+FONTHEIGHT*(2+i), "                                                                       ", 71 );

	// Cursor
	if( m_DispLines ) {
		RECT	rcInv;
		rcInv.left   = OFFSETH+FONTWIDTH*6+FONTWIDTH*3*(m_CursorX>>1)+FONTWIDTH*(m_CursorX&1)-1;
		rcInv.top    = OFFSETV+FONTHEIGHT*2+FONTHEIGHT*m_CursorY;
		rcInv.right  = rcInv.left+FONTWIDTH;
		rcInv.bottom = rcInv.top+FONTHEIGHT;
		::InvertRect( hDC, &rcInv );

		rcInv.left   = OFFSETH+FONTWIDTH*55+FONTWIDTH*(m_CursorX>>1)-1;
		rcInv.top    = OFFSETV+FONTHEIGHT*2+FONTHEIGHT*m_CursorY;
		rcInv.right  = rcInv.left+FONTWIDTH;
		rcInv.bottom = rcInv.top+FONTHEIGHT;
		::InvertRect( hDC, &rcInv );
	}

	// 元に戻して消す？
	::SelectObject( hDC, hFontOld );
}

