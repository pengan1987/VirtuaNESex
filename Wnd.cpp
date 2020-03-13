//
// へなちょこウインドウクラス
//
#include "DebugOut.h"
#include "Wnd.h"

// Instance
CWndList	WndList;

list<CWnd*>	CWndList::m_WndPtrList;

// 泥臭い…
CWndList::CWndList()
{
//	m_WndPtrList.clear();
}

CWndList::~CWndList()
{
//	if( !m_WndPtrList.empty() )
//		m_WndPtrList.clear();
}

void	CWndList::Add( CWnd* pWnd )
{
	m_WndPtrList.push_back( pWnd );
}

void	CWndList::Del( CWnd* pWnd )
{
	for( list<CWnd*>::iterator it=m_WndPtrList.begin(); it!=m_WndPtrList.end(); ) {
		if( *it == pWnd ) {
			m_WndPtrList.erase(it);
			break;
		} else {
			++it;
		}
	}
}

BOOL	CWndList::IsDialogMessage( LPMSG msg )
{
	if( m_WndPtrList.empty() )
		return	FALSE;

	list<CWnd*>::iterator it=m_WndPtrList.begin();
	while( it != m_WndPtrList.end() ) {
		if( ::IsDialogMessage( (*it)->m_hWnd, msg ) )
			return	TRUE;
		++it;
	}

	return	FALSE;
}

CWnd::CWnd()
{
	m_hWnd = NULL;
	m_hMenu = NULL;
}

CWnd::~CWnd()
{
}

void	CWnd::SetThis()
{
	// Dispatch出来るようにCWnd*を埋め込む
	if( m_hWnd ) {
		::SetWindowLong( m_hWnd, GWL_USERDATA, (LONG)this );
	}
}

LRESULT	CALLBACK CWnd::g_WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	// ウインドウが開く前にこっそり処理する
	if( msg == WM_CREATE ) {
		LPCREATESTRUCT	lpcs = (LPCREATESTRUCT)lParam;
		CWnd* pWnd = (CWnd*)::GetWindowLong( hWnd, GWL_USERDATA );
		if( !pWnd ) {
			// CWnd* thisを埋め込む
			::SetWindowLong( hWnd, GWL_USERDATA, (LONG)lpcs->lpCreateParams );
			// 自分のウインドウハンドル
			pWnd = (CWnd*)lpcs->lpCreateParams;
			pWnd->m_hWnd = hWnd;
		}
	}
	// CWnd* thisを埋め込んである
	CWnd* pWnd = (CWnd*)::GetWindowLong( hWnd, GWL_USERDATA );

	if( pWnd ) {
		return	pWnd->DispatchWnd( hWnd, msg, wParam, lParam );
	} else {
		return	::DefWindowProc(  hWnd, msg, wParam, lParam );
	}
}

BOOL	CALLBACK CWnd::g_DlgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	// ディスパッチ前にこっそり処理する
	if( msg == WM_INITDIALOG ) {
		// Dispatch出来るようにCWnd*を埋め込む(モーダルは必ずDialogBoxParamで起動する事)
		// CWnd* thisを埋め込んであるが，モーダルでは入っていない
		CWnd* pWnd = (CWnd*)::GetWindowLong( hWnd, GWL_USERDATA );

		if( !pWnd ) {
			::SetWindowLong( hWnd, GWL_USERDATA, (LONG)lParam );
			pWnd = (CWnd*)lParam;
		}
		// 自分のウインドウハンドル
		pWnd->m_hWnd = hWnd;

		// ダイアログを中央に移動する:)
		HWND hWndParent = ::GetParent( hWnd );
		if( hWndParent ) {
			RECT	rcParent, rc;
			::GetWindowRect( hWndParent, &rcParent );
			::GetWindowRect( hWnd, &rc );
			INT x = rcParent.left+(rcParent.right-rcParent.left)/2-(rc.right-rc.left)/2;
			INT y = rcParent.top +(rcParent.bottom-rcParent.top)/2-(rc.bottom-rc.top)/2;
//			DEBUGOUT( "X=%d  Y=%d\n", x, y );
			::SetWindowPos( hWnd, NULL, x, y, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );
		}
	}

	// CWnd* thisを埋め込んである
	CWnd* pWnd = (CWnd*)::GetWindowLong( hWnd, GWL_USERDATA );

	if( pWnd ) {
		return	pWnd->DispatchDlg( hWnd, msg, wParam, lParam );
	} else {
		return	FALSE;
	}
}

