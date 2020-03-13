//
// アーカイブファイル操作
//
// Original:NesterJ arc.cpp arc.h by Mikami Kana
// Original:NNNesterJ ulunzip.cpp
//
// Zlib use!
// Reprogrammed by Norix
//
#define	WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlwapi.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mbstring.h>
#include <time.h>

#include "typedef.h"
#include "macro.h"
#include "DebugOut.h"

#include "App.h"
#include "Pathlib.h"

#include "VirtuaNESres.h"

#define UNZ_BUFSIZE (65536)

#include "unzip.h"

#pragma	pack(1)
#define	FNAME_MAX32	512

typedef struct {
	DWORD 	dwOriginalSize;
 	DWORD 	dwCompressedSize;
	DWORD	dwCRC;
	UINT	uFlag;
	UINT	uOSType;
	WORD	wRatio;
	WORD	wDate;
	WORD 	wTime;
	char	szFileName[FNAME_MAX32 + 1];
	char	dummy1[3];
	char	szAttribute[8];
	char	szMode[8];
} INDIVIDUALINFO, *LPINDIVIDUALINFO;
#pragma pack()

// Un??? use function
typedef	int(WINAPI *EXECUTECOMMAND)(HWND,LPCSTR,LPSTR,const DWORD);
typedef	BOOL(WINAPI *CHECKARCHIVE)(LPCSTR,const int);
typedef	int(WINAPI *EXTRACTMEM)(HWND,LPCSTR,LPBYTE,const DWORD,time_t,LPWORD,LPDWORD);
typedef	HGLOBAL(WINAPI *OPENARCHIVE)(HWND,LPCSTR,const DWORD);
typedef	int(WINAPI *CLOSEARCHIVE)(HGLOBAL);
typedef	int(WINAPI *FINDFIRST)(HGLOBAL,LPCSTR,INDIVIDUALINFO*);

static	LPCSTR	pszArchiver[] = {
	"UNLHA32",
	"UNZIP32",
	"UNRAR32",
	"CAB32",
	NULL
};

static	LPCSTR	pszFuncPrefix[] = {
	"Unlha",
	"UnZip",
	"Unrar",
	"Cab",
};

static	LPCSTR	pszCommand[] = {
	NULL,
	NULL,
	"-e -u \"%s\" \"%s\" \"%s\"",
	"-x -j \"%s\" \"%s\" \"%s\"",
};

static	LPCSTR	pszExtension[] = {
	"*.nes",
	"*.fds",
	"*.nsf",
	NULL
};

static	BOOL	bFileMatching[] = {
	FALSE,
	TRUE,
	FALSE,
	FALSE,
};

#define	FREEDLL(h)	if( h ) { FreeLibrary(h);h=NULL; }

#define M_ERROR_MESSAGE_OFF		0x00800000L

// zlibを使用したZIP解凍ルーチン
BOOL	ZlibUnZip( LPCSTR fname, LPBYTE* ppBuf, LPDWORD lpdwSize )
{
	unzFile		unzipFile = NULL;
	unz_global_info	unzipGlobalInfo;
	unz_file_info	unzipFileInfo;
	char		fname_buf[256];

	*ppBuf = NULL;
	*lpdwSize = 0;

	if( !(unzipFile = unzOpen( (const char*)fname )) )
		return	FALSE;

	if( unzGetGlobalInfo( unzipFile, &unzipGlobalInfo ) != UNZ_OK ) {
		unzClose( unzipFile );
		return	FALSE;
	}

	for( uLong i = 0; i < unzipGlobalInfo.number_entry; i++ ) {
		if( unzGetCurrentFileInfo( unzipFile, &unzipFileInfo, fname_buf, sizeof(fname_buf), NULL, 0, NULL, 0 ) != UNZ_OK )
			break;

		char*	pExt = ::PathFindExtension( fname_buf );
		if( _stricmp( pExt, ".nes" ) == 0 || _stricmp( pExt, ".fds" ) == 0 || _stricmp( pExt, ".nsf" ) == 0 ) {
			if( unzipFileInfo.uncompressed_size ) {
				if( unzOpenCurrentFile( unzipFile ) != UNZ_OK )
					break;

				if( unzipFileInfo.uncompressed_size > 0 ) {
					if( !(*ppBuf = (LPBYTE)::malloc( unzipFileInfo.uncompressed_size )) )
						break;

					uInt	size = unzReadCurrentFile( unzipFile, *ppBuf, unzipFileInfo.uncompressed_size );
					if( size != unzipFileInfo.uncompressed_size )
						break;
				}
				*lpdwSize = unzipFileInfo.uncompressed_size;

				if( unzCloseCurrentFile( unzipFile ) != UNZ_OK )
					break;
				unzClose( unzipFile );
				return	TRUE;
			}
		}

		// Next file
		if( (i+1) < unzipGlobalInfo.number_entry ) {
			if( unzGoToNextFile( unzipFile ) != UNZ_OK ) {
				break;
			}
		}
	}

	FREE( *ppBuf );

	if( unzipFile ) {
		unzCloseCurrentFile( unzipFile );
		unzClose( unzipFile );
	}
	return	FALSE;
}

BOOL	UnCompress( LPCSTR fname, LPBYTE* ppBuf, LPDWORD lpdwSize )
{
HMODULE		hDLL;
INDIVIDUALINFO	idvinfo;

	char*	pExt = ::PathFindExtension( fname );
	if( _stricmp( pExt, ".zip" ) == 0 ) {
		// ZIPならまずzlibライブラリの解凍を使ってみる
		if( ZlibUnZip( fname, ppBuf, lpdwSize ) ) {
//DEBUGOUT( "zlib unzip ok! [%s]\n", fname );
			return	TRUE;
		}
	}

	hDLL = NULL;
	for( INT i = 0; pszArchiver[i]; i++ ) {
		// DLLアンロード
		FREEDLL( hDLL );

		// DLLロード
		if( !(hDLL = LoadLibrary( pszArchiver[i] )) )
			continue;

		CHAR	szTemp[256];
		sprintf( szTemp, "%sCheckArchive", pszFuncPrefix[i] );
		CHECKARCHIVE	CheckArchive;
		if( !(CheckArchive = (CHECKARCHIVE)GetProcAddress( hDLL, szTemp )) )
			continue;
		// 対応するアーカイブかチェックする
		if( !CheckArchive( fname, 1 ) )
			continue;

		// アーカイブ内に対応するファイルがあるかのチェック
		OPENARCHIVE	OpenArchive;
		CLOSEARCHIVE	CloseArchive;
		FINDFIRST	FindFirst;

		sprintf( szTemp, "%sOpenArchive", pszFuncPrefix[i] );
		OpenArchive = (OPENARCHIVE)GetProcAddress( hDLL, szTemp );
		sprintf( szTemp, "%sFindFirst", pszFuncPrefix[i] );
		FindFirst = (FINDFIRST)GetProcAddress( hDLL, szTemp );
		sprintf( szTemp, "%sCloseArchive", pszFuncPrefix[i] );
		CloseArchive = (CLOSEARCHIVE)GetProcAddress( hDLL, szTemp );

		HGLOBAL		hARC;
		BOOL	bFound = FALSE;
		for( INT j = 0; pszExtension[j]; j++ ) {
			if( !(hARC = OpenArchive( NULL, fname, M_ERROR_MESSAGE_OFF ) ) ) {
				CloseArchive( hARC );
				break;
			}
			INT	ret = FindFirst( hARC, pszExtension[j], &idvinfo );
			CloseArchive( hARC );
			if( ret == 0 ) {		// Found!!
				bFound = TRUE;
				break;
			} else if( ret == -1 ) {	// Not found.
			} else {			// 異常終了
				break;
			}
		}
		if( !bFound )
			continue;

		if( !pszCommand[i] ) {
		// メモリ解凍あり(UNLHA32,UNZIP32)
			*lpdwSize = idvinfo.dwOriginalSize;
			*ppBuf = (LPBYTE)malloc( *lpdwSize );

			CHAR	szCmd [256];
			CHAR	szFunc[256];

			if( !bFileMatching[i] ) {
				sprintf( szCmd, "\"%s\" \"%s\"", fname, idvinfo.szFileName );
			} else {
			// UNZIP32 only
				BYTE	szFile[FNAME_MAX32+1];
				LPBYTE	lpF0, lpF1;

				// 正規表現を切るオプションが欲しかった....
				lpF0 = (LPBYTE)idvinfo.szFileName;
				lpF1 = szFile;
				while( *lpF0 ) {
					if( *lpF0 == '[' || *lpF0 == ']' ) {
						*lpF1++ = '\\';
					}
					_mbsncpy( lpF1, lpF0, 1 );
					lpF0 = _mbsinc( lpF0 );
					lpF1 = _mbsinc( lpF1 );
				}
				*lpF1 = '\0';

				sprintf( szCmd, "\"%s\" \"%s\"", fname, szFile );
			}
			sprintf( szFunc, "%sExtractMem", pszFuncPrefix[i] );

			EXTRACTMEM	ExtractMem;
			ExtractMem = (EXTRACTMEM)GetProcAddress( hDLL, szFunc );
			INT ret = ExtractMem( NULL, szCmd, (LPBYTE)(*ppBuf), *lpdwSize, NULL, NULL, NULL );
			FREEDLL( hDLL );
			if( ret == 0 )
				return TRUE;
		} else {
		// メモリ解凍が無い場合
			CHAR	szCmd [256];
			CHAR	szTempPath[_MAX_PATH];
			EXECUTECOMMAND	ExecuteCommand;

			GetTempPath( _MAX_PATH, szTempPath );
//DEBUGOUT( "TempPath:%s\n", szTempPath );

			sprintf( szCmd, pszCommand[i], fname, szTempPath, idvinfo.szFileName );
			ExecuteCommand = (EXECUTECOMMAND)GetProcAddress( hDLL, pszFuncPrefix[i] );
			ExecuteCommand( NULL, szCmd, NULL, 0 );
			FREEDLL( hDLL );

			string	FileName = CPathlib::MakePath( szTempPath, idvinfo.szFileName );

			FILE *fp = NULL;
			if( (fp = fopen( FileName.c_str(), "rb" )) ) {
				// ファイルサイズ取得
				fseek( fp, 0, SEEK_END );
				*lpdwSize = ftell( fp );
				fseek( fp, 0, SEEK_SET );
				if( *lpdwSize < 17 ) {
					// ファイルサイズが小さすぎます
					throw	CApp::GetErrorString( IDS_ERROR_SMALLFILE );
				}

				// テンポラリメモリ確保
				if( !(*ppBuf = (LPBYTE)malloc( *lpdwSize )) ) {
					FCLOSE( fp );
					// メモリを確保出来ません
					throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
				}
				// サイズ分読み込み
				if( fread( *ppBuf, *lpdwSize, 1, fp ) != 1 ) {
					FCLOSE( fp );
					FREE( *ppBuf );
					// ファイルの読み込みに失敗しました
					throw	CApp::GetErrorString( IDS_ERROR_READ );
				}
				FCLOSE( fp );
				DeleteFile( FileName.c_str() );
			} else {
				// xxx ファイルを開けません
				LPCSTR	szErrStr = CApp::GetErrorString( IDS_ERROR_OPEN );
				sprintf( szErrorString, szErrStr, fname );
				throw	szErrorString;
			}
			return	TRUE;
		}
	}
	FREEDLL( hDLL );

	return	FALSE;
}
// Archive
