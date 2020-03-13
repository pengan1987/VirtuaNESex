//
// メッセージフィルタリングサポートクラス
//
#ifndef	__CWNDHOOK_INCLUDED__
#define	__CWNDHOOK_INCLUDED__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// prototypes
class	CWnd;

class	CWndHook
{
public:
	static	void	Initialize();
	static	void	Release();

	static	void	SetFiltering( BOOL bMode ) { m_bMsgFiltering = bMode; }

	static	BOOL	WalkPreTranslateTree( MSG* lpMsg );

protected:
	static	LRESULT	CALLBACK MessageFilterProc( INT code, WPARAM wParam, LPARAM lParam );

	// メッセージをフィルタリングするかどうかのフラグ
	static	BOOL	m_bMsgFiltering;

	// メッセージがフィルタリング中かどうかのフラグ
	static	BOOL	m_bMsgFilter;
	// 元のフックプロシージャ
	static	HHOOK 	m_hOldMsgFilter;
private:
};

#endif	// !__CWNDHOOK_INCLUDED__

