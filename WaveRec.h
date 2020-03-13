//
// Wave record class
//
#ifndef	__CWAVEREC_INCLUDED__
#define	__CWAVEREC_INCLUDED__

#define	WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <memory.h>

#include "typedef.h"

#pragma pack(1)
typedef struct {
	CHAR		tagRIFF[4];		// "RIFF"
	DWORD		dwFileSize;
	CHAR		tagTYPE[8];		// "WAVEfmt "
	DWORD		dwChunkOffset;
	WORD		wCodingType;
	WORD		wChannel;
	DWORD		dwSample;
	DWORD		dwBytesPerSec;
	WORD		wBytesPerSample;
	WORD		wBits;
	CHAR		tagDATA[4];		// "data"
	DWORD		dwDataSize;
} WAVEFILE, *LPWAVEFILE;
#pragma pack()

class	CWaveRec
{
public:
	CWaveRec();
	~CWaveRec();

	void	Start( LPSTR szFile, INT nSample, INT nBits, BOOL bStereo );
	void	Stop();

	BOOL	IsWaveRecord()	{ return fp?TRUE:FALSE; }

	void	Out( LPVOID lpBuf, DWORD dwSize );
protected:
private:
	WAVEFILE	wavefile;
	FILE*		fp;
};

#endif	// !__CWAVEREC_INCLUDED__

