//
// メッセージフィルタリングサポートクラス
//
#include "DebugOut.h"

#include "App.h"
#include "Wnd.h"
#include "WndHook.h"

// Instance
CWndHook	WndHook;

BOOL	CWndHook::m_bMsgFiltering = FALSE;
BOOL	CWndHook::m_bMsgFilter = FALSE;
HHOOK 	CWndHook::m_hOldMsgFilter = NULL;

// メッセージフィルタフックの初期化(ウインドウが初期化されてから呼び出す事)
void	CWndHook::Initialize()
{
	m_hOldMsgFilter = ::SetWindowsHookEx( WH_MSGFILTER, (HOOKPROC)MessageFilterProc, NULL, ::GetCurrentThreadId() );
}

void	CWndHook::Release()
{
	if( m_hOldMsgFilter ) {
		::UnhookWindowsHookEx( m_hOldMsgFilter );
		m_hOldMsgFilter = NULL;
	}
}

// メッセージフィルタ処理(ダイアログは普通では来ないメッセージがあるため)
LRESULT	CALLBACK CWndHook::MessageFilterProc( INT code, WPARAM wParam, LPARAM lParam )
{
	if( code < 0 ) {
		return	::CallNextHookEx( m_hOldMsgFilter, code, wParam, lParam );
	}

	if( m_bMsgFiltering && code == MSGF_DIALOGBOX ) {
		// 既にフィルタリング中か？
		if( m_bMsgFilter )
			return	FALSE;
		m_bMsgFilter = TRUE;
		LPMSG	lpMsg = (LPMSG)lParam;
		if( lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST ) {
			if( WalkPreTranslateTree( lpMsg ) ) {
				m_bMsgFilter = FALSE;
				return	1L;
			}
		}
		m_bMsgFilter = FALSE;
	}

	return	::CallNextHookEx( m_hOldMsgFilter, code, wParam, lParam );
//	return	FALSE;
}

BOOL	CWndHook::WalkPreTranslateTree( MSG* lpMsg )
{
	// メインフレームウインドウ
	HWND	hWndStop = CApp::GetHWnd();

	// PreTranslateMessageを辿る
	for( HWND hWnd = lpMsg->hwnd; hWnd != NULL; hWnd = ::GetParent(hWnd) ) {
		if( hWnd == hWndStop )
			break;
		CWnd*	pWnd = (CWnd*)::GetWindowLong( hWnd, GWL_USERDATA );
		if( pWnd ) {
			if( pWnd->PreTranslateMessage( lpMsg ) )
				return	TRUE;
		}
		if( hWnd == hWndStop )
			break;
	}
	return	FALSE;
}

