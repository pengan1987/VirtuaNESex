//
// Recent File クラス
//
#include <TCHAR.h>

#include "DebugOut.h"
#include "PathLib.h"

#include "VirtuaNESres.h"
#include "Recent.h"

CHAR	CRecent::m_RecentName[RECENT_MAX][_MAX_PATH];
CHAR	CRecent::m_RecentPath[RECENT_MAX][_MAX_PATH];
CHAR	CRecent::m_TempPath[_MAX_PATH];

LPCSTR	CRecent::GetName( INT nID )
{
	// CRecent::Add で同じポインタを指して戻ってくる事があるので一旦テンポラリにコピー
	::strcpy( m_TempPath, m_RecentName[nID] );
	return	(LPCSTR)m_TempPath;
}

LPCSTR	CRecent::GetPath( INT nID )
{
	return	(LPCSTR)m_RecentPath[nID];
}

void	CRecent::MakeManuPath( LPSTR lpszPath )
{
	string	FullPath = lpszPath;
	string	FileName = CPathlib::SplitFnameExt( lpszPath );

	// 30文字以下はそのまま
	if( FullPath.size() <= 30 )
		return;

	// ファイル名が30文字以上の場合
	if( ::strlen( FileName.c_str() ) >= 30 ) {
		::strcpy( lpszPath, FileName.c_str() );
		return;
	}

	LPCSTR lpszCur = lpszPath + 2;
	if( lpszPath[0] == '\\' && lpszPath[1] == '\\' ) {
		while (*lpszCur != '\\') {
			lpszCur = _tcsinc(lpszCur);
		}
	}

	if( ::strlen(FullPath.c_str()) - ::strlen(FileName.c_str()) > 3 ) {
		lpszCur = _tcsinc(lpszCur);
		while( *lpszCur != '\\' ) {
			lpszCur = _tcsinc(lpszCur);
		}
	}

	INT	nVolume = lpszCur - lpszPath;
	if( 30 < nVolume+5+::strlen(FileName.c_str()) ) {
		::strcpy( lpszPath, FileName.c_str() );
		return;
	}

	while ( nVolume+4+::strlen(lpszCur) > 30 ) {
		do {
			lpszCur = _tcsinc(lpszCur);
		}
		while( *lpszCur != '\\' );
	}

	lpszPath[nVolume] = '\0';
	::strcat( lpszPath, "\\..." );
	::strcat( lpszPath, lpszCur );
}

void	CRecent::UpdateMenu( HMENU hMenu )
{
	// メニューなし？
	if( !hMenu )
		return;

	// ﾌｧｲﾙ(&F)メニューの取得
	HMENU hSubMenu = ::GetSubMenu( hMenu, 0 );

	// 最近使ったﾌｫﾙﾀﾞ(&P)ポップアップメニューの取得
	HMENU hPathMenu = ::GetSubMenu( hSubMenu, 12 );
	// 最近使ったﾌｧｲﾙ(&F)ポップアップメニューの取得
	HMENU hFileMenu = ::GetSubMenu( hSubMenu, 13 );

	// 項目が無い場合
	if( ::strlen(m_RecentPath[0]) <= 0 ) {
		// ディセーブルにする
		::EnableMenuItem( hPathMenu, ID_MRU_PATH0, MF_BYCOMMAND|MF_GRAYED );
	} else {
		INT	i;
		// メニューアイテムの削除
		for( i = 0; i < RECENT_MAX; i++ ) {
			::DeleteMenu( hPathMenu, ID_MRU_PATH0+i, MF_BYCOMMAND );
		}

		CHAR	szRecent[_MAX_PATH];
		CHAR	szTemp[_MAX_PATH];
		for( i = 0; i < RECENT_MAX; i++ ) {
			if( ::strlen(m_RecentPath[i]) > 0 ) {
				// パスをメニュー用に短くしたりする
				::strcpy( szRecent, m_RecentPath[i] );

				// '&'付きのファイルの'&'を'&&'に変換する
				LPCSTR	pSrc = szRecent;
				LPSTR	pDst = szTemp;
				while( *pSrc != 0 ) {
					if( *pSrc == '&' )
						*pDst++ = '&';
					if( _istlead(*pSrc) )
						*pDst++ = *pSrc++;
					*pDst++ = *pSrc++;
				}
				*pDst = 0;
				::wsprintf( szRecent, "&%d ", (i+1)%10 );
				::strcat( szRecent, szTemp );

				// メニューに追加
				::InsertMenu( hPathMenu, i, MF_BYPOSITION, ID_MRU_PATH0+i, szRecent );
			} else {
				break;
			}
		}
	}

	// 項目が無い場合
	if( ::strlen(m_RecentName[0]) <= 0 ) {
		// ディセーブルにする
		::EnableMenuItem( hFileMenu, ID_MRU_FILE0, MF_BYCOMMAND|MF_GRAYED );
	} else {
		INT	i;
		// メニューアイテムの削除
		for( i = 0; i < RECENT_MAX; i++ ) {
			::DeleteMenu( hFileMenu, ID_MRU_FILE0+i, MF_BYCOMMAND );
		}

		CHAR	szRecent[_MAX_PATH];
		CHAR	szTemp[_MAX_PATH];
		for( i = 0; i < RECENT_MAX; i++ ) {
			if( ::strlen(m_RecentName[i]) > 0 ) {
				// パスをメニュー用に短くしたりする
				::strcpy( szRecent, m_RecentName[i] );
				MakeManuPath( szRecent );

				// '&'付きのファイルの'&'を'&&'に変換する
				LPCSTR	pSrc = szRecent;
				LPSTR	pDst = szTemp;
				while( *pSrc != 0 ) {
					if( *pSrc == '&' )
						*pDst++ = '&';
					if( _istlead(*pSrc) )
						*pDst++ = *pSrc++;
					*pDst++ = *pSrc++;
				}
				*pDst = 0;
				::wsprintf( szRecent, "&%d ", (i+1)%10 );
				::strcat( szRecent, szTemp );

				// メニューに追加
				::InsertMenu( hFileMenu, i, MF_BYPOSITION, ID_MRU_FILE0+i, szRecent );
			} else {
				break;
			}
		}
	}
}

void	CRecent::Add( LPCSTR lpszPath )
{
	INT	i, j;

	if( ::strlen(m_RecentName[0]) > 0 ) {
		for( i = 0; i < RECENT_MAX; i++ ) {
			if( ::strlen(m_RecentName[i]) <= 0 )
				break;
		}
		for( j = 0; j < i; j++ ) {
			if( ::strcmp( lpszPath, m_RecentName[j] ) == 0 )
				break;
		}
		if( j == RECENT_MAX )
			j--;
		for( ; j > 0; j-- ) {
			::strcpy( &m_RecentName[j][0], &m_RecentName[j-1][0] );
		}
	}
	::strcpy( m_RecentName[0], lpszPath );

	string	temp = CPathlib::SplitPath( lpszPath );
	if( ::strlen(m_RecentPath[0]) > 0 ) {
		for( i = 0; i < RECENT_MAX; i++ ) {
			if( ::strlen(m_RecentPath[i]) <= 0 )
				break;
		}
		for( j = 0; j < i; j++ ) {
			if( ::strcmp( m_RecentPath[j], temp.c_str() ) == 0 )
				break;
		}
		if( j == RECENT_MAX )
			j--;
		for( ; j > 0; j-- ) {
			::strcpy( m_RecentPath[j], m_RecentPath[j-1] );
		}
	}
	::strcpy( m_RecentPath[0], temp.c_str() );
}

void	CRecent::Load()
{
	INT	i;
	CHAR	szTemp[MAX_PATH];
	CHAR	szEntry[32];
	for( i = 0; i < RECENT_MAX; i++ ) {
		::wsprintf( szEntry, "Path%d", i+1 );
		if( CRegistry::GetProfileString( "Recent Path List", szEntry, szTemp, sizeof(szTemp) ) )
			::strcpy( m_RecentPath[i], szTemp );
	}
	for( i = 0; i < RECENT_MAX; i++ ) {
		::wsprintf( szEntry, "File%d", i+1 );
		if( CRegistry::GetProfileString( "Recent File List", szEntry, szTemp, sizeof(szTemp) ) )
			::strcpy( m_RecentName[i], szTemp );
	}
}

void	CRecent::Save()
{
	INT	i;
	CHAR	szEntry[32];
	for( i = 0; i < RECENT_MAX; i++ ) {
		if( ::strlen(m_RecentPath[i]) > 0 ) {
			::wsprintf( szEntry, "Path%d", i+1 );
			CRegistry::WriteProfileString( "Recent Path List", szEntry, m_RecentPath[i] );
		}
	}
	for( i = 0; i < RECENT_MAX; i++ ) {
		if( ::strlen(m_RecentName[i]) > 0 ) {
			::wsprintf( szEntry, "File%d", i+1 );
			CRegistry::WriteProfileString( "Recent File List", szEntry, m_RecentName[i] );
		}
	}
}

