//
// DirectSound class
//
#include "DebugOut.h"
#include "DirectSound.h"
#include "COM.h"

CDirectSound	DirectSound;

#define	COMUSE	TRUE

//
// WaveFileのロードとメモリへの保持
//
CWaveData::CWaveData()
{
	m_pBuffer = NULL;
	m_pwfex = NULL;
	m_dwSize = 0;
	m_pData = NULL;
}

CWaveData::~CWaveData()
{
	Free();
}

BOOL	CWaveData::Load( LPCSTR szFileName )
{
	Free();

	FILE*	fp = NULL;
	if( !(fp = ::fopen( szFileName, "rb" )) ) {
		return	FALSE;
	}

	LONG	size;
	::fseek( fp, 0, SEEK_END );
	size = ::ftell( fp );
	::fseek( fp, 0, SEEK_SET );

	if( size < 0 ) {
		return	FALSE;
	}

	if( !(m_pBuffer = ::malloc( size )) ) {
		return	FALSE;
	}

	if( ::fread( m_pBuffer, size, 1, fp ) != 1 ) {
		Free();
		return	FALSE;
	}
	FCLOSE( fp );

	LPWAVEFORMATEX	pWaveHeader;
	BYTE	*pbWaveData;
	DWORD	cbWaveSize;
	DWORD	*pdw, *pdwEnd;
	DWORD	dwRiff, dwLength, dwType;

	// WAVEデータの解析
	pWaveHeader = NULL;
	pbWaveData  = NULL;
	cbWaveSize  = 0;

	pdw = (DWORD *)m_pBuffer;
	dwRiff   = *pdw++;
	dwLength = *pdw++;
	dwType   = *pdw++;
	if( dwRiff != mmioFOURCC( 'R','I','F','F' ) ) {
		Free();
		return	FALSE;
	}
	if( dwType != mmioFOURCC( 'W','A','V','E' ) ) {
		Free();
		return	FALSE;
	}
	pdwEnd = (DWORD*)((BYTE*)pdw+dwLength-sizeof(DWORD));

	while( pdw < pdwEnd ) {
		dwType   = *pdw++;
		dwLength = *pdw++;

		switch( dwType ) {
			case	mmioFOURCC( 'f','m','t',' ' ):
				if( pWaveHeader == NULL ) {
					if( dwLength < sizeof(WAVEFORMAT) ) {
						Free();
						return	FALSE;
					}
					pWaveHeader = (WAVEFORMATEX *)pdw;
				}
				break;
			case	mmioFOURCC( 'd','a','t','a' ):
				if( (pbWaveData == NULL)||(!cbWaveSize) ) {
					pbWaveData = (BYTE *)pdw;
					cbWaveSize = dwLength;
				}
		}
		if( pWaveHeader && (pbWaveData != NULL) && cbWaveSize)
			break;
		pdw = (DWORD *)((BYTE*)pdw + ((dwLength+1)&~1));
	}
	if( pdwEnd <= pdw ) {
		Free();
		return	FALSE;
	}
	if( pWaveHeader->wFormatTag != WAVE_FORMAT_PCM ) {
		Free();
		return	FALSE;
	}

	m_pwfex = pWaveHeader;
	m_dwSize = dwLength;
	m_pData = pbWaveData;

	return	TRUE;
}

void	CWaveData::Free()
{
	FREE( m_pBuffer );
}

DWORD	CWaveData::GetSize()
{
	if( !m_pBuffer )
		return	0;
	return	m_dwSize;
}

WAVEFORMATEX*	CWaveData::GetFormat()
{
	if( !m_pBuffer )
		return	0;
	return	m_pwfex;
}

LPVOID	CWaveData::GetData()
{
	if( !m_pBuffer )
		return	0;
	return	m_pData;
}

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
CDirectSound::SAMPLERATE CDirectSound::m_SampleRateTable[] = {
	11025, 8,
	22050, 8,
	44100, 8,
	48000, 8,
	11025, 16,
	22050, 16,
	44100, 16,
	48000, 16,
	0, 0
};

INT	CDirectSound::m_BufferSizeTable[] = {
	2, 3, 4, 5, 6, 7, 8, 9, 10, 0
};

CDirectSound::CDirectSound()
{
	m_lpDS        = NULL;
	m_lpDSPrimary = NULL;
	m_lpDSStream  = NULL;

#if	1
	m_SampleRate.Rate = 22050;
#else
	m_SampleRate.Rate = 44100;
#endif

//	m_SampleRate.Bits = 8;
	m_SampleRate.Bits = 16;
	m_BufferSize = 1;
//	m_BufferSize = 2;

	m_bStreamPlay  = FALSE;
	m_bStreamPause = FALSE;

	for( INT i = 0; i < ESF_FILE_MAX; i++ ) {
		m_pEsfDSBuffer[ i ] = NULL;
	}

#if	COMUSE
	COM::AddRef();
#endif
}

CDirectSound::~CDirectSound()
{
	ReleaseDSound();

#if	COMUSE
	COM::Release();
#endif
}

// DirectSoundの初期化
BOOL	CDirectSound::InitialDSound( HWND hWnd )
{
DSBUFFERDESC	dsbdesc;

	m_hWnd = hWnd;

	try {
		// DirectSoundオブジェクトの作成
#if	!COMUSE
		if( DirectSoundCreate( NULL, &m_lpDS, NULL ) != DS_OK ) {
			m_lpDS = NULL;
			throw	"CDirectSound:DirectSoundCreate failed.";
		}
#else
		// COM的利用
//		COM::AddRef();
		if( ::CoCreateInstance( CLSID_DirectSound, NULL, CLSCTX_ALL, IID_IDirectSound, (LPVOID*)&m_lpDS) != S_OK ) {
			m_lpDS = NULL;
			throw	"CDirectSound:CoCreateInstance failed.";
		}
		if( m_lpDS->Initialize( NULL ) != DS_OK )
			throw	"CDirectSound:IDirectSound->Initialize failed.";
#endif

		// 優先協調モードの設定
		if( m_lpDS->SetCooperativeLevel( hWnd, DSSCL_PRIORITY ) != DS_OK )
			throw	"CDirectSound:SetCooperativeLevel failed.";

		// スピーカの設定
//		m_lpDS->SetSpeakerConfig( DSSPEAKER_COMBINED( DSSPEAKER_STEREO, DSSPEAKER_GEOMETRY_WIDE ) );

		// プライマリバッファの作成
		ZeroMemory( &dsbdesc, sizeof(DSBUFFERDESC) );
		dsbdesc.dwSize = sizeof(DSBUFFERDESC);
//		dsbdesc.dwFlags       = DSBCAPS_CTRLVOLUME
//				      | DSBCAPS_PRIMARYBUFFER;
		dsbdesc.dwFlags       = DSBCAPS_PRIMARYBUFFER;
		dsbdesc.dwBufferBytes = 0;
		dsbdesc.lpwfxFormat   = NULL;
		if( m_lpDS->CreateSoundBuffer( &dsbdesc, &m_lpDSPrimary, NULL ) != DS_OK )
			throw	"CDirectSound:CreateSoundBuffer failed.";
	} catch( char *str ) {
		ReleaseDSound();
		::MessageBox( hWnd, str, "ERROR", MB_ICONERROR|MB_OK );

		return	FALSE;
	}

	return	TRUE;
}

// DirectSoundの開放
void	CDirectSound::ReleaseDSound()
{
	ReleaseEsfBuffer();
	ReleaseBuffer();
	// DirectSoundオブジェクトの開放
	RELEASE( m_lpDSPrimary );
	if( m_lpDS ) {
		RELEASE( m_lpDS );
#if	COMUSE
//		COM::Release();
#endif
	}

	m_hWnd = NULL;
}

// DirectSoundバッファの作成
BOOL	CDirectSound::InitialBuffer()
{
DSBUFFERDESC	dsbdesc;
WAVEFORMATEX	pcmwf;

	try {
		if( !m_lpDSPrimary )
			throw "CDirectSound:DirectSound object uninitialized.";

		// プライマリバッファのWaveフォーマットを設定(常にモノラル)
		ZEROMEMORY( &pcmwf, sizeof(WAVEFORMATEX) );
		pcmwf.wFormatTag      = WAVE_FORMAT_PCM;
		pcmwf.nChannels       = 1;
		pcmwf.nSamplesPerSec  = (WORD)m_SampleRate.Rate;
		pcmwf.nBlockAlign     = (WORD)m_SampleRate.Bits/8;
		pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign;
		pcmwf.wBitsPerSample  = (WORD)m_SampleRate.Bits;

		if( m_lpDSPrimary->SetFormat( &pcmwf ) != DS_OK )
			throw	"CDirectSound:SetFormat failed.";

		// ストリームセカンダリバッファ作成
		if( m_BufferSize < 2 )
			m_BufferSize = 2;

		// バッファサイズ等の計算
		m_dwDSBlockNum = m_BufferSize * 10;
		m_dwDSBlockSize = pcmwf.nAvgBytesPerSec * m_BufferSize / 60;
		m_dwDSBlockSize-= m_dwDSBlockSize % pcmwf.nBlockAlign;
		m_dwDSBufferSize = m_dwDSBlockSize * m_dwDSBlockNum;
		m_dwDSLastBlock = 0;

		ZeroMemory( &dsbdesc, sizeof(DSBUFFERDESC) );
		dsbdesc.dwSize        = sizeof(DSBUFFERDESC);
		dsbdesc.dwFlags       = DSBCAPS_LOCSOFTWARE
				      | DSBCAPS_GETCURRENTPOSITION2
				      | DSBCAPS_GLOBALFOCUS;
		dsbdesc.dwBufferBytes = m_dwDSBufferSize;
		dsbdesc.lpwfxFormat   = &pcmwf;

		if( m_lpDS->CreateSoundBuffer( &dsbdesc, &m_lpDSStream, NULL ) != DS_OK )
			throw	"CDirectSound:CreateSoundBuffer failed.";

		LPBYTE	lpPtr;
		DWORD	dwBytes;
		if( m_lpDSStream->Lock( 0, m_dwDSBufferSize, (LPVOID*)&lpPtr, &dwBytes, NULL, NULL, 0 ) != DS_OK ) {
			throw	"CDirectSound:Lock failed.";
		} else {
			FillMemory( lpPtr, dwBytes, (BYTE)(m_SampleRate.Bits==8?128:0) );
			m_lpDSStream->Unlock( lpPtr, dwBytes, NULL, NULL );
		}
	} catch( char *str ) {
		ReleaseBuffer();

		::MessageBox( m_hWnd, str, "ERROR", MB_ICONERROR|MB_OK );

		return	FALSE;
	}

	return	TRUE;
}

// DirectSoundバッファの開放
void	CDirectSound::ReleaseBuffer()
{
	StreamStop();
	RELEASE( m_lpDSStream );
}

// サンプリングレートの設定
BOOL	CDirectSound::SetSamplingRate( DWORD rate, DWORD bits )
{
INT	i;

	i = 0;
	while( m_SampleRateTable[i].Rate != 0 ) {
		if( m_SampleRateTable[i].Rate == rate
		 && m_SampleRateTable[i].Bits == bits ) {
			m_SampleRate.Rate = rate;
			m_SampleRate.Bits = bits;
			return	TRUE;
		}
		i++;
	}
	return	FALSE;
}

// サンプリングレートの取得
void	CDirectSound::GetSamplingRate( DWORD& rate, DWORD& bits )
{
	rate = m_SampleRate.Rate;
	bits = m_SampleRate.Bits;
}

// ストリーミング再生
void	CDirectSound::StreamPlay()
{
	if( !m_lpDS || !m_lpDSStream )
		return;

	if( !m_bStreamPlay ) {
		// Buffer clear
		LPBYTE	lpPtr;
		DWORD	dwBytes;
		if( m_lpDSStream->Lock( 0, m_dwDSBufferSize, (LPVOID*)&lpPtr, &dwBytes, NULL, NULL, 0 ) != DS_OK ) {
			throw	"CDirectSound:Lock failed.";
		} else {
			FillMemory( lpPtr, dwBytes, (BYTE)(m_SampleRate.Bits==8?128:0) );
			m_lpDSStream->Unlock( lpPtr, dwBytes, NULL, NULL );
		}

		m_dwDSLastBlock = 0xFFFFFFFF;
		m_bStreamPlay  = TRUE;
		m_bStreamPause = FALSE;
		m_lpDSStream->SetCurrentPosition( 0 );
		m_lpDSStream->Play( 0, 0, DSBPLAY_LOOPING );
	}
}

// ストリーミング停止
void	CDirectSound::StreamStop()
{
	if( !m_lpDS || !m_lpDSStream )
		return;

	if( m_bStreamPlay ) {
		m_bStreamPlay  = FALSE;
		m_bStreamPause = FALSE;
		m_lpDSStream->Stop();

		// 完全停止まで待つ
		DWORD	dwStatus;
		do {
			m_lpDSStream->GetStatus( &dwStatus );
		} while( dwStatus & DSBSTATUS_PLAYING );

		m_lpDSStream->SetCurrentPosition( 0 );
	}
}

// ストリーミングポーズ
void	CDirectSound::StreamPause()
{
//	DEBUGOUT( "CDirectSound::StreamPause\n" );

	if( !m_lpDS || !m_lpDSStream )
		return;

	if( m_bStreamPlay ) {
		if( !m_bStreamPause ) {
			m_bStreamPause = TRUE;
			m_lpDSStream->Stop();
		}
	}
}

// ストリーミングレジューム
void	CDirectSound::StreamResume()
{
//	DEBUGOUT( "CDirectSound::StreamResume\n" );

	if( !m_lpDS || !m_lpDSStream )
		return;

	if( m_bStreamPlay ) {
		if( m_bStreamPause ) {
			m_bStreamPause = FALSE;
			m_lpDSStream->Play( 0, 0, DSBPLAY_LOOPING );
		}
	}
}

// ストリーミング
BOOL	CDirectSound::GetStreamLockPosition( LPDWORD lpdwStart, LPDWORD lpdwSize )
{
static	BOOL	bLockHalf = FALSE;
DWORD	dwPlayPos, dwWritePos;

	if( m_lpDSStream->GetCurrentPosition( &dwPlayPos, &dwWritePos ) == DS_OK ) {
		if( (dwWritePos / m_dwDSBlockSize) != m_dwDSLastBlock ) {
			m_dwDSLastBlock = dwWritePos / m_dwDSBlockSize;
			dwWritePos = (((dwWritePos/m_dwDSBlockSize)+1)%m_dwDSBlockNum) * m_dwDSBlockSize;
			// ロックすべき場所
			*lpdwStart = dwWritePos;
			*lpdwSize = m_dwDSBlockSize;
			return	TRUE;
		}
	}

	return	FALSE;
}

BOOL	CDirectSound::StreamLock( DWORD dwWriteCursor, DWORD dwWriteBytes, LPVOID* lplpvPtr1, LPDWORD lpdwBytes1, LPVOID* lplpvPtr2, LPDWORD lpdwBytes2, DWORD dwFlags )
{
	if( m_lpDSStream->Lock( dwWriteCursor, dwWriteBytes, lplpvPtr1, lpdwBytes1, lplpvPtr2, lpdwBytes2, dwFlags ) == DS_OK )
		return	TRUE;
	return	FALSE;
}

BOOL	CDirectSound::StreamUnlock( LPVOID lpvPtr1, DWORD dwBytes1, LPVOID lpvPtr2, DWORD dwBytes2 )
{
	if( m_lpDSStream->Unlock( lpvPtr1, dwBytes1, lpvPtr2, dwBytes2 ) == DS_OK )
		return	TRUE;
	return	FALSE;
}

BOOL	CDirectSound::LoadEsf( LPCSTR szFileName, INT no )
{
	if( no < 0 || no > ESF_FILE_MAX-1 )
		return	FALSE;

	if( m_EsfWaveFile[no].Load( szFileName ) ) {
		return	CreateESFBuffer( no, m_EsfWaveFile[no].GetFormat(), m_EsfWaveFile[no].GetData(), m_EsfWaveFile[no].GetSize() );
	} else {
DEBUGOUT( "CDirectSound::LoadEsf error. [%s]\n", szFileName );
	}

	return	FALSE;
}

BOOL	CDirectSound::EsfPlay( INT no )
{
	if( !m_lpDS )
		return	FALSE;

	if( !m_pEsfDSBuffer[no] )
		return	FALSE;

	m_pEsfDSBuffer[no]->SetCurrentPosition( 0 );
	if( m_pEsfDSBuffer[no]->Play( 0, 0, 0 ) == DSERR_BUFFERLOST ) {
		if( m_pEsfDSBuffer[no]->Restore() == DS_OK ) {
			CreateESFBuffer( no, m_EsfWaveFile[no].GetFormat(), m_EsfWaveFile[no].GetData(), m_EsfWaveFile[no].GetSize() );
			m_pEsfDSBuffer[no]->Play( 0, 0, 0 );
		}
	}

	return	TRUE;
}

BOOL	CDirectSound::EsfPlayLoop( INT no )
{
	if( !m_lpDS )
		return	FALSE;

	if( !m_pEsfDSBuffer[no] )
		return	FALSE;

	// 既に再生中か？
	DWORD	dwStatus;
	if( m_pEsfDSBuffer[no]->GetStatus( &dwStatus ) == DS_OK ) {
		if( dwStatus == DSBSTATUS_PLAYING ) {
			return	TRUE;
		}
	}

	m_pEsfDSBuffer[no]->SetCurrentPosition( 0 );
	if( m_pEsfDSBuffer[no]->Play( 0, 0, DSBPLAY_LOOPING ) == DSERR_BUFFERLOST ) {
		if( m_pEsfDSBuffer[no]->Restore() == DS_OK ) {
			CreateESFBuffer( no, m_EsfWaveFile[no].GetFormat(), m_EsfWaveFile[no].GetData(), m_EsfWaveFile[no].GetSize() );
			m_pEsfDSBuffer[no]->Play( 0, 0, DSBPLAY_LOOPING );
		}
	}

	return	TRUE;
}

BOOL	CDirectSound::EsfStop( INT no )
{
	if( !m_lpDS )
		return	FALSE;

	if( !m_pEsfDSBuffer[no] )
		return	FALSE;

	m_pEsfDSBuffer[no]->Stop();
	m_pEsfDSBuffer[no]->SetCurrentPosition( 0 );

	return	TRUE;
}

void	CDirectSound::EsfAllStop()
{
	if( !m_lpDS )
		return;

	for( INT i = 0; i < ESF_FILE_MAX; i++ ) {
		EsfStop( i );
	}
}

BOOL	CDirectSound::CreateESFBuffer( INT no, WAVEFORMATEX* pwfex, LPVOID pData, DWORD dwSize )
{
DSBUFFERDESC	dsbdesc;
LPVOID	lpPtr0, lpPtr1;
DWORD	dwBytes0, dwBytes1;

	// 既に使っていたら開放しておく
	if( m_pEsfDSBuffer[no] ) {
		RELEASE( m_pEsfDSBuffer[no] );
	}

	// DirectSound セカンダリバッファ作成
	ZEROMEMORY( &dsbdesc, sizeof(DSBUFFERDESC) );
	dsbdesc.dwSize        = sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags       = DSBCAPS_LOCSOFTWARE;
	dsbdesc.dwBufferBytes = dwSize;
	dsbdesc.lpwfxFormat   = pwfex;

	if( m_lpDS->CreateSoundBuffer( &dsbdesc, &m_pEsfDSBuffer[no], NULL ) != DS_OK ) {
		m_pEsfDSBuffer[no] = NULL;
		return	FALSE;
	}

	// 作成したセカンダリバッファにウェーブデータをコピー
	m_pEsfDSBuffer[no]->Lock( 0L, dwSize, &lpPtr0, &dwBytes0, &lpPtr1, &dwBytes1, 0 );
	::CopyMemory( lpPtr0, pData, dwBytes0 );
	if( dwBytes1 ) {
		::CopyMemory( lpPtr1, (LPBYTE)pData + dwBytes1, dwBytes1 );
	}
	m_pEsfDSBuffer[no]->Unlock( &lpPtr0, dwBytes0, &lpPtr1, dwBytes1 );

	return	TRUE;
}

void	CDirectSound::ReleaseEsfBuffer()
{
	EsfAllStop();

	for( INT i = 0; i < ESF_FILE_MAX; i++ ) {
		RELEASE( m_pEsfDSBuffer[i] );
		m_EsfWaveFile[i].Free();
	}
}

