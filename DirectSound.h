//
// DirectSound class
//
#ifndef	__DIRECTSOUND_INCLUDED__
#define	__DIRECTSOUND_INCLUDED__

#define	WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

#define DIRECTSOUND_VERSION	0x0700
#include <dsound.h>

#include "extsoundfile.h"

#include "typedef.h"
#include "macro.h"

class	CWaveData
{
public:
	CWaveData();
	virtual	~CWaveData();

	BOOL		Load( LPCSTR szFileName );
	void		Free();

	DWORD		GetSize();
	WAVEFORMATEX*	GetFormat();
	LPVOID		GetData();
protected:
	LPVOID		m_pBuffer;

	WAVEFORMATEX*	m_pwfex;
	DWORD		m_dwSize;

	LPVOID		m_pData;

private:
};

class	CDirectSound
{
public:
	// パブリックメンバ変数
	typedef	struct	tagSAMPLERATE {
		DWORD	Rate;
		DWORD	Bits;
	} SAMPLERATE, *LPSAMPLERATE;

	SAMPLERATE		m_SampleRate;		// 現在サンプリングレート
	INT			m_BufferSize;		// バッファサイズ(フレーム数)
	static	SAMPLERATE	m_SampleRateTable[];	// サンプリングレートテーブル
	static	INT		m_BufferSizeTable[];	// バッファサイズテーブル

	// パブリックメンバ関数
	CDirectSound();
	virtual ~CDirectSound();

	BOOL	InitialDSound( HWND hWnd );
	void	ReleaseDSound();

	BOOL	InitialBuffer();
	void	ReleaseBuffer();

	BOOL	SetSamplingRate( DWORD rate, DWORD bits );
	void	GetSamplingRate( DWORD& rate, DWORD& bits );

	void	SetBufferSize( INT nSize ) { m_BufferSize = nSize; }
	INT	GetBufferSize()		   { return m_BufferSize; }

	BOOL	IsStreamPlaying() { return (m_bStreamPlay&&!m_bStreamPause); }
	void	StreamPlay();
	void	StreamStop();
	void	StreamPause();
	void	StreamResume();

	BOOL	GetStreamLockPosition( LPDWORD lpdwStart, LPDWORD lpdwSize );
	BOOL	StreamLock( DWORD dwWriteCursor, DWORD dwWriteBytes, LPVOID* lplpvPtr1, LPDWORD lpdwBytes1, LPVOID* lplpvPtr2, LPDWORD lpdwBytes2, DWORD dwFlags );
	BOOL	StreamUnlock( LPVOID lpvPtr1, DWORD dwBytes1, LPVOID lpvPtr2, DWORD dwBytes2 );

	BOOL	IsStreamPlay() { return m_bStreamPlay; }
	BOOL	IsStreamPause() { return m_bStreamPause; }

	// 外部音声ファイル対応
	BOOL	LoadEsf( LPCSTR szFileName, INT no );
	BOOL	EsfPlay( INT no );
	BOOL	EsfPlayLoop( INT no );
	BOOL	EsfStop( INT no );
	void	EsfAllStop();
	BOOL	CreateESFBuffer( INT no, WAVEFORMATEX* pwfex, LPVOID pData, DWORD dwSize );
	void	ReleaseEsfBuffer();

protected:
	// プロテクトメンバ変数
	HWND	m_hWnd;		// Window handle

	LPDIRECTSOUND		m_lpDS;		// DirectSoundオブジェクト
	LPDIRECTSOUNDBUFFER	m_lpDSPrimary;	// プライマリオブジェクト

	LPDIRECTSOUNDBUFFER	m_lpDSStream;		// ストリーミングオブジェクト

	DWORD			m_dwDSBufferSize;
	DWORD			m_dwDSBlockSize;	// 1ブロックのサイズ
	DWORD			m_dwDSBlockNum;		// ブロックの数
	DWORD			m_dwDSLastBlock;	// 最後に書き込んだブロック位置

	volatile BOOL		m_bStreamPlay;	// ストリーム再生中フラグ
	volatile BOOL		m_bStreamPause;	// ストリームポーズフラグ

	// 外部音声ファイル対応
	CWaveData		m_EsfWaveFile[ ESF_FILE_MAX ];
	LPDIRECTSOUNDBUFFER	m_pEsfDSBuffer[ ESF_FILE_MAX ];

	// プロテクトメンバ関数
private:
	// プライベートメンバ変数
	// プライベートメンバ関数
};

extern	CDirectSound	DirectSound;

#endif	// !__DIRECTSOUND_INCLUDED__
