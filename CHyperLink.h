#ifndef	__CHYPERLINK_INCLUDED__
#define	__CHYPERLINK_INCLUDED__

//
// スタティックコントロールをSubclass化してハイパーリンクをサポート
//
// Usage: CHyperLink	hyperlink;
// hyperlink.Attach( GetDlgItem( hWnd, IDC_URL ) );
//
class	CHyperLink
{
public:
	CHyperLink() : m_hWnd(NULL), m_lpLabel(NULL), 
			m_hCursor(NULL), m_hFont(NULL),
			m_lpHyperLink(NULL), m_lpfnOldWndProc(NULL)
	{
	}
	~CHyperLink()
	{
		Detach();
	}

	// コントロールにアタッチする
	BOOL	Attach( HWND hWnd, LPCSTR lpLabel = NULL, LPCSTR lpHyperLink = NULL )
	{
		if( m_hWnd )
			return	FALSE;	// 既にAttach済み

		m_hWnd = hWnd;

		// 文字列をコピー
		if( lpLabel ) {
			m_lpLabel = new CHAR[::lstrlen(lpLabel)+1];
			::lstrcpy( m_lpLabel, lpLabel );
			::SendMessage( m_hWnd, WM_SETTEXT, 0, (LPARAM)m_lpLabel );
		}
		// 表示とハイパーリンクが同じ場合はハイパーリンクはラベルと同じに
		if( lpLabel && !lpHyperLink ) {
			m_lpHyperLink = new CHAR[::lstrlen(lpLabel)+1];
			::lstrcpy( m_lpHyperLink, lpLabel );
		}
		// ハイパーリンクが別の場合
		if( lpHyperLink ) {
			m_lpHyperLink = new CHAR[::lstrlen(lpHyperLink)+1];
			::lstrcpy( m_lpHyperLink, lpHyperLink );
		}

		// ポイントカーソル(無ければデフォルト)
		m_hCursor = ::LoadCursor( NULL, MAKEINTRESOURCE(32649) );

		// アンダーラインフォントの作成
		HFONT	m_hOldFont = (HFONT)::SendMessage( hWnd, WM_GETFONT, 0, 0 );
		LOGFONT	lFont;
		::GetObject( m_hOldFont, sizeof(LOGFONT), &lFont );
		lFont.lfUnderline = TRUE;
		m_hFont = ::CreateFontIndirect( &lFont );

		// ラベルの表示領域を計算
		CalcLabelRectangle();

		// スタイルの変更(通知をイネーブルにする)
		DWORD	dwStyle = ::GetWindowLong( hWnd, GWL_STYLE );
		dwStyle |= SS_NOTIFY;
		::SetWindowLong( hWnd, GWL_STYLE, (LONG)dwStyle );

		// コントロールのサブクラス化
		m_lpfnOldWndProc = (WNDPROC)::SetWindowLong( hWnd, GWL_WNDPROC, (LONG)HyperLinkProc );

		// Thisを埋め込む
		::SetWindowLong( hWnd, GWL_USERDATA, (LONG)this );

		return	TRUE;
	}

	BOOL	Detach()
	{
		if( m_hWnd ) {
			// サブクラス化を解除
			if( m_lpfnOldWndProc ) {
				::SetWindowLong( m_hWnd, GWL_WNDPROC, (LONG)m_lpfnOldWndProc );
				m_lpfnOldWndProc = NULL;
			}

			// フォントを削除
			if( m_hFont ) {
				::DeleteObject( (HGDIOBJ)m_hFont );
				m_hFont = NULL;
			}

			// 文字列を削除
			if( m_lpLabel ) {
				delete[] m_lpLabel;
				m_lpLabel = NULL;
			}
			if( m_lpHyperLink ) {
				delete[] m_lpHyperLink;
				m_lpHyperLink = NULL;
			}
		}

		m_hWnd = NULL;

		return	TRUE;
	}

	BOOL	SetLabel( LPCSTR lpLabel )
	{
		if( m_lpLabel ) {
			delete[] m_lpLabel;
		}

		m_lpLabel = new CHAR[::lstrlen(lpLabel)+1];
		::lstrcpy( m_lpLabel, lpLabel );

		// コントロールに文字列を設定
		::SendMessage( m_hWnd, WM_SETTEXT, 0, (LPARAM)m_lpLabel );

		// ラベルの表示領域を計算
		CalcLabelRectangle();
	}

	BOOL	SetHyperLink( LPCSTR lpHyperLink )
	{
		if( m_lpHyperLink ) {
			delete[] m_lpHyperLink;
		}

		m_lpHyperLink = new CHAR[::lstrlen(lpHyperLink)+1];
		::lstrcpy( m_lpHyperLink, lpHyperLink );
	}

protected:
	BOOL	CalcLabelRectangle()
	{
		if( !::IsWindow(m_hWnd) )
			return	FALSE;

		if( !m_lpLabel )
			return	FALSE;

		RECT	rcClient;
		::GetClientRect( m_hWnd, &rcClient );
		m_rcLabel = rcClient;

		HDC	hDC = ::GetDC( m_hWnd );
		HFONT	hOldFont = (HFONT)::SelectObject( hDC, m_hFont );

		// スタイル
		DWORD	dwStyle = ::GetWindowLong( m_hWnd, GWL_STYLE );
		INT	nDrawStyle = DT_LEFT;
		if( dwStyle && SS_CENTER ) {
			nDrawStyle = DT_CENTER;
		} else if( dwStyle && SS_RIGHT ) {
			nDrawStyle = DT_RIGHT;
		}

		// 文字列表示と描画領域の計算
		::DrawText( hDC, m_lpLabel, -1, &m_rcLabel, nDrawStyle | DT_WORDBREAK | DT_CALCRECT );

		::SelectObject( hDC, hOldFont );

		// スタイルによってオフセットを計算
		if( dwStyle & SS_CENTER ) {
			::OffsetRect( &m_rcLabel, (rcClient.right - m_rcLabel.right) / 2, 0 );
		} else if (dwStyle & SS_RIGHT) {
			::OffsetRect( &m_rcLabel, rcClient.right - m_rcLabel.right, 0 );
		}

		return true;
	}

	static	LRESULT CALLBACK HyperLinkProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		CHyperLink*	pHyperLink = reinterpret_cast<CHyperLink*>(::GetWindowLong( hWnd, GWL_USERDATA));

		switch( msg ) {
			case	WM_DESTROY:
				{
				// 2003/10/11 リリース後にfix...
				WNDPROC	pWndProcOld = pHyperLink->m_lpfnOldWndProc;
				pHyperLink->Detach();
				return	CallWindowProc( pWndProcOld, hWnd, msg, wParam, lParam );
				}
				break;
			case	WM_LBUTTONDOWN: {
				POINT	pt = { LOWORD(lParam), HIWORD(lParam) };
				if( pHyperLink->m_lpLabel && pHyperLink->m_lpHyperLink ) {
					if( ::PtInRect( &pHyperLink->m_rcLabel, pt ) ) {
						::ShellExecute( hWnd, NULL, pHyperLink->m_lpHyperLink, NULL, NULL, SW_SHOWNORMAL );
						return	TRUE;
					}
				}
				}
				break;
			case	WM_MOUSEMOVE: {
				POINT	pt = { LOWORD(lParam), HIWORD(lParam) };
				if( pHyperLink->m_lpLabel && pHyperLink->m_lpHyperLink ) {
					if( ::PtInRect( &pHyperLink->m_rcLabel, pt ) ) {
						::SetCursor( pHyperLink->m_hCursor );
						return	TRUE;
					}
				}
				}
				break;
			case	WM_PAINT: {
				PAINTSTRUCT	ps;
				HDC	hDC = ::BeginPaint( hWnd, &ps );
				DWORD	dwStyle = ::GetWindowLong( hWnd, GWL_STYLE );
				INT	nDrawStyle = DT_LEFT;
				if( dwStyle && SS_CENTER ) {
					nDrawStyle = DT_CENTER;
				} else if( dwStyle && SS_RIGHT ) {
					nDrawStyle = DT_RIGHT;
				}

				// アトリビュート
				::SetBkMode( hDC, TRANSPARENT );
				::SetTextColor( hDC, 0x00FF0000 );

				// 文字列表示
				if( pHyperLink->m_lpLabel ) {
					HFONT	hOldFont = (HFONT)::SelectObject( hDC, pHyperLink->m_hFont );
					::DrawText( hDC, pHyperLink->m_lpLabel, -1, &pHyperLink->m_rcLabel, nDrawStyle | DT_WORDBREAK );
					::SelectObject( hDC, hOldFont );
				}

				::EndPaint( hWnd, &ps );
				}
				return	TRUE;
			default:
				break;
		}

		return	CallWindowProc( pHyperLink->m_lpfnOldWndProc, hWnd, msg, wParam, lParam );
	}

	HWND	m_hWnd;
	HFONT	m_hFont;
	HFONT	m_hOldFont;
	HCURSOR	m_hCursor;
	WNDPROC	m_lpfnOldWndProc;

	RECT	m_rcLabel;

	LPSTR	m_lpLabel;
	LPSTR	m_lpHyperLink;
private:
};

#endif	// !__CHYPERLINK_INCLUDED__
