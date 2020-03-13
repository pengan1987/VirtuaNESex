//
// パターンビュークラス
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <string>
using namespace std;

#include "typedef.h"
#include "macro.h"

#include "VirtuaNESres.h"
#include "DebugOut.h"
#include "App.h"
#include "Pathlib.h"

#include "Wnd.h"
#include "PatternView.h"
#include "EmuThread.h"

#include "MMU.h"

#include "DirectDraw.h"

WND_MESSAGE_BEGIN(CPatternView)
WND_ON_MESSAGE( WM_CREATE,	OnCreate )
WND_ON_MESSAGE( WM_CLOSE,	OnClose )
WND_ON_MESSAGE( WM_DESTROY,	OnDestroy )
WND_ON_MESSAGE( WM_LBUTTONDOWN,	OnLButtonDown )
WND_ON_MESSAGE( WM_TIMER,	OnTimer )

WND_COMMAND_BEGIN()
WND_COMMAND_END()
WND_MESSAGE_END()

CPatternView::CPatternView()
{
	m_lpPattern = NULL;
	if( !(m_lpPattern = (LPBYTE)::malloc( 128*256 )) ) {
		throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
	}

	for( INT i = 0; i < 8; i++ ) {
		m_lpBank[i] = NULL;
	}
}

CPatternView::~CPatternView()
{
	FREE( m_lpPattern );
}

BOOL	CPatternView::Create( HWND hWndParent )
{
	HWND hWnd = CreateWindowEx(
			WS_EX_TOOLWINDOW,
			VIRTUANES_WNDCLASS,
			"PatternView BG0",
			WS_OVERLAPPEDWINDOW,	// Window拡大縮小可能
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

	for( INT i = 0; i < 8; i++ ) {
		m_lpBank[i] = NULL;
	}

	//
	ZEROMEMORY( &m_BitmapHdr, sizeof(m_BitmapHdr) );

	m_BitmapHdr.bih.biSize        = sizeof(BITMAPINFOHEADER);
	m_BitmapHdr.bih.biWidth       = 128;
	m_BitmapHdr.bih.biHeight      = -256;
	m_BitmapHdr.bih.biPlanes      = 1;
	m_BitmapHdr.bih.biBitCount    = 8;
	m_BitmapHdr.bih.biCompression = BI_RGB;
	m_BitmapHdr.bih.biClrUsed     = 16;

	DirectDraw.GetPaletteData( m_Palette );

	m_SelectPal = 0;

	return	TRUE;
}

void	CPatternView::Destroy()
{
	if( m_hWnd && IsWindow(m_hWnd) ) {
		::DestroyWindow( m_hWnd );
		m_hWnd = NULL;
	}
}

WNDMSG	CPatternView::OnCreate( WNDMSGPARAM )
{
	DEBUGOUT( "CPatternView::OnCreate\n" );

	// 位置修正
	RECT	rw, rc;
	::GetWindowRect( m_hWnd, &rw );
	::GetClientRect( m_hWnd, &rc );
	INT	x = rw.right - rw.left - rc.right + 128; 
	INT	y = rw.bottom - rw.top - rc.bottom + 256;
	::MoveWindow( m_hWnd, Config.general.rcPatternViewPos.left, Config.general.rcPatternViewPos.top, x, y, FALSE );

	::SetTimer( m_hWnd, 1, 50, NULL );

	::ShowWindow( m_hWnd, SW_SHOW );
	return	TRUE;
}

WNDMSG	CPatternView::OnClose( WNDMSGPARAM )
{
	DEBUGOUT( "CPatternView::OnClose\n" );
	::KillTimer( m_hWnd, 1 );
	::DestroyWindow( m_hWnd );
	return	TRUE;
}

WNDMSG	CPatternView::OnDestroy( WNDMSGPARAM )
{
	DEBUGOUT( "CPatternView::OnDestroy\n" );
	// 位置保存
	::GetWindowRect( m_hWnd, &Config.general.rcPatternViewPos );

	m_hWnd = NULL;
	return	TRUE;
}

WNDMSG	CPatternView::OnLButtonDown( WNDMSGPARAM )
{
	DEBUGOUT( "CPatternView::OnLButtonDown\n" );
	m_SelectPal = (m_SelectPal+1)&7;

	CHAR	szTitle[256];
	if( m_SelectPal < 4 ) {
		::wsprintf( szTitle, "PatternView BG%01d", m_SelectPal&3 );
	} else {
		::wsprintf( szTitle, "PatternView SP%01d", m_SelectPal&3 );
	}
	::SetWindowText( m_hWnd, szTitle );

	return	TRUE;
}

WNDMSG	CPatternView::OnTimer( WNDMSGPARAM )
{
	if( !Emu.IsRunning() )
		return	TRUE;

	// パレット
	LPBYTE	pPAL = (m_SelectPal<4)?&BGPAL[m_SelectPal*4]:&SPPAL[(m_SelectPal&3)*4];
	m_BitmapHdr.rgb[0] = m_Palette[pPAL[0]];
	m_BitmapHdr.rgb[1] = m_Palette[pPAL[1]];
	m_BitmapHdr.rgb[2] = m_Palette[pPAL[2]];
	m_BitmapHdr.rgb[3] = m_Palette[pPAL[3]];

	// キャラクタデータ更新
	for( INT i = 0; i < 8; i++ ) {
		// 更新されたバンクのみキャラクタ更新
		if( m_lpBank[i] != PPU_MEM_BANK[i] || PPU_MEM_TYPE[i] == BANKTYPE_CRAM || PPU_MEM_TYPE[i] == BANKTYPE_YCRAM ) {
			m_lpBank[i] = PPU_MEM_BANK[i];

			LPBYTE	lpPtn = PPU_MEM_BANK[i];
			for( INT p = 0; p < 64; p++ ) {
				LPBYTE	lpScn = &m_lpPattern[i*32*128+(p&15)*8+(p/16)*8*128];
				for( INT y = 0; y < 8; y++ ) {
					BYTE	chr_l = lpPtn[y];
					BYTE	chr_h = lpPtn[y+8];
					lpScn[0] = ((chr_h>>6)&2)|((chr_l>>7)&1);
					lpScn[4] = ((chr_h>>2)&2)|((chr_l>>3)&1);
					lpScn[1] = ((chr_h>>5)&2)|((chr_l>>6)&1);
					lpScn[5] = ((chr_h>>1)&2)|((chr_l>>2)&1);
					lpScn[2] = ((chr_h>>4)&2)|((chr_l>>5)&1);
					lpScn[6] = ((chr_h>>0)&2)|((chr_l>>1)&1);
					lpScn[3] = ((chr_h>>3)&2)|((chr_l>>4)&1);
					lpScn[7] = ((chr_h<<1)&2)|((chr_l>>0)&1);
					// Next line
					lpScn+=128;
				}
				// Next pattern
				lpPtn+=16;
			}
		}
	}

	RECT	rc;
	::GetClientRect( hWnd, &rc );
	HDC	hDC = ::GetDC( hWnd );
	::StretchDIBits( hDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 0, 0, 128, 256, m_lpPattern, (BITMAPINFO*)&m_BitmapHdr, DIB_RGB_COLORS, SRCCOPY );
	::ReleaseDC( hWnd, hDC );

	return	TRUE;
}

