//
// パターンビュークラス
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <string>
using namespace std;

#include "VirtuaNESres.h"
#include "DebugOut.h"
#include "App.h"
#include "Pathlib.h"

#include "Wnd.h"
#include "NameTableView.h"
#include "EmuThread.h"

#include "MMU.h"

#include "DirectDraw.h"

WND_MESSAGE_BEGIN(CNameTableView)
WND_ON_MESSAGE( WM_CREATE,	OnCreate )
WND_ON_MESSAGE( WM_CLOSE,	OnClose )
WND_ON_MESSAGE( WM_DESTROY,	OnDestroy )
WND_ON_MESSAGE( WM_TIMER,	OnTimer )

WND_COMMAND_BEGIN()
WND_COMMAND_END()
WND_MESSAGE_END()

CNameTableView::CNameTableView()
{
	m_lpPattern = NULL;
	if( !(m_lpPattern = (LPBYTE)::malloc( 512*480 )) ) {
		throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
	}
}

CNameTableView::~CNameTableView()
{
	FREE( m_lpPattern );
}

BOOL	CNameTableView::Create( HWND hWndParent )
{
	HWND hWnd = CreateWindowEx(
			WS_EX_TOOLWINDOW,
			VIRTUANES_WNDCLASS,
			"NameTableView",
			WS_OVERLAPPEDWINDOW,	// Window拡大縮小可能
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
//			hWndParent,
			HWND_DESKTOP,
			NULL,
			CApp::GetInstance(),
			(LPVOID)this		// This を埋め込む為
		);
	if( !hWnd ) {
		DEBUGOUT( "CreateWindow faild.\n" );
		return	FALSE;
	}

	m_hWnd = hWnd;

	ZEROMEMORY( &m_BitmapHdr, sizeof(m_BitmapHdr) );

	m_BitmapHdr.bih.biSize        = sizeof(BITMAPINFOHEADER);
	m_BitmapHdr.bih.biWidth       = 512;
	m_BitmapHdr.bih.biHeight      = -480;
	m_BitmapHdr.bih.biPlanes      = 1;
	m_BitmapHdr.bih.biBitCount    = 8;
	m_BitmapHdr.bih.biCompression = BI_RGB;
	m_BitmapHdr.bih.biClrUsed     = 16;

	DirectDraw.GetPaletteData( m_Palette );

//	::memcpy( &m_BitmapHdr.rgb[0], &m_BitmapHdr.rgb[64], 64*sizeof(RGBQUAD) );
//	m_BitmapHdr.rgb[1].rgbBlue  = 0x60;
//	m_BitmapHdr.rgb[1].rgbGreen = 0x60;
//	m_BitmapHdr.rgb[1].rgbRed   = 0x60;

	return	TRUE;
}

void	CNameTableView::Destroy()
{
	if( m_hWnd && IsWindow(m_hWnd) ) {
		::DestroyWindow( m_hWnd );
		m_hWnd = NULL;
	}
}

WNDMSG	CNameTableView::OnCreate( WNDMSGPARAM )
{
	DEBUGOUT( "CNameTableView::OnCreate\n" );

	RECT	rw, rc;
	::GetWindowRect( m_hWnd, &rw );
	::GetClientRect( m_hWnd, &rc );
	INT	x = rw.right - rw.left - rc.right + 512; 
	INT	y = rw.bottom - rw.top - rc.bottom + 480;
	::MoveWindow( m_hWnd, Config.general.rcNameTableViewPos.left, Config.general.rcNameTableViewPos.top, x, y, FALSE );

	::SetTimer( m_hWnd, 1, 50, NULL );

	::ShowWindow( m_hWnd, SW_SHOW );
	return	TRUE;
}

WNDMSG	CNameTableView::OnClose( WNDMSGPARAM )
{
	DEBUGOUT( "CNameTableView::OnClose\n" );
	::KillTimer( m_hWnd, 1 );
	::DestroyWindow( m_hWnd );
	return	TRUE;
}

WNDMSG	CNameTableView::OnDestroy( WNDMSGPARAM )
{
	DEBUGOUT( "CNameTableView::OnDestroy\n" );
	// 位置保存
	::GetWindowRect( m_hWnd, &Config.general.rcNameTableViewPos );

	m_hWnd = NULL;
	return	TRUE;
}

WNDMSG	CNameTableView::OnTimer( WNDMSGPARAM )
{
	if( !Emu.IsRunning() )
		return	TRUE;

	// パレット更新
	for( INT i = 0; i < 16; i++ ) {
		m_BitmapHdr.rgb[i] = m_Palette[BGPAL[i]];
	}

	// キャラクタデータ更新
	for( INT n = 0; n < 4; n++ ) {
		LPBYTE	lpVRAM = PPU_MEM_BANK[8+n];
		LPBYTE	lpScnv = &m_lpPattern[(n>>1)*512*240+(n&1)*256];

		for( INT y = 0; y < 30; y++ ) {
			for( INT x = 0; x < 32; x++ ) {
				INT	tile = lpVRAM[x+y*32]*16+((PPUREG[0]&PPU_BGTBL_BIT)<<8);
				BYTE	attr = ((lpVRAM[0x03C0+(x/4)+(y>>2)*8]>>((x&2)+(y&2)*2))&3)<<2;
				LPBYTE	lpScn = &lpScnv[x*8+y*8*512];
				LPBYTE	lpPtn = &PPU_MEM_BANK[tile>>10][tile&0x03FF];
				for( INT p = 0; p < 8; p++ ) {
					BYTE	chr_l = lpPtn[p];
					BYTE	chr_h = lpPtn[p+8];
					lpScn[0] = ((chr_h>>6)&2)|((chr_l>>7)&1)|attr;
					lpScn[4] = ((chr_h>>2)&2)|((chr_l>>3)&1)|attr;
					lpScn[1] = ((chr_h>>5)&2)|((chr_l>>6)&1)|attr;
					lpScn[5] = ((chr_h>>1)&2)|((chr_l>>2)&1)|attr;
					lpScn[2] = ((chr_h>>4)&2)|((chr_l>>5)&1)|attr;
					lpScn[6] = ((chr_h>>0)&2)|((chr_l>>1)&1)|attr;
					lpScn[3] = ((chr_h>>3)&2)|((chr_l>>4)&1)|attr;
					lpScn[7] = ((chr_h<<1)&2)|((chr_l>>0)&1)|attr;
					// Next line
					lpScn+=512;
				}
			}
		}
	}

	RECT	rc;
	::GetClientRect( hWnd, &rc );
	HDC	hDC = ::GetDC( hWnd );
	::StretchDIBits( hDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 0, 0, 512, 480, m_lpPattern, (BITMAPINFO*)&m_BitmapHdr, DIB_RGB_COLORS, SRCCOPY );
	::ReleaseDC( hWnd, hDC );

	return	TRUE;
}

