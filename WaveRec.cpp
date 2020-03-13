//
// Wave record class
//
#include "WaveRec.h"

CWaveRec::CWaveRec()
{
	::ZeroMemory( &wavefile, sizeof(wavefile) );
	fp = NULL;

	::CopyMemory( wavefile.tagRIFF, "RIFF",     sizeof(wavefile.tagRIFF) );
	::CopyMemory( wavefile.tagTYPE, "WAVEfmt ", sizeof(wavefile.tagTYPE) );
	::CopyMemory( wavefile.tagDATA, "data",     sizeof(wavefile.tagDATA) );

	wavefile.dwChunkOffset = 0x0010;
	wavefile.wCodingType   = 0x0001;
}

CWaveRec::~CWaveRec()
{
	if( fp )
		Stop();
}

void	CWaveRec::Start( LPSTR szFile, INT nSample, INT nBits, BOOL bStereo )
{
	if( fp )
		Stop();

	if( (fp = ::fopen( szFile, "wb" )) ) {
		wavefile.wChannel        = bStereo?2:1;
		wavefile.dwSample        = nSample;
		wavefile.wBits           = nBits;
		wavefile.wBytesPerSample = (WORD)(nBits>>3)*wavefile.wChannel;
		wavefile.dwBytesPerSec   = nSample*wavefile.wBytesPerSample;
	}
}

void	CWaveRec::Stop()
{
	if( fp ) {
		DWORD	size = (DWORD)::ftell( fp );
		wavefile.dwFileSize = (DWORD)size-8;
		wavefile.dwDataSize = (DWORD)size-sizeof(WAVEFILE);
		::fseek( fp, 0, SEEK_SET );
		::fwrite( &wavefile, sizeof(WAVEFILE), 1, fp );
		::fclose( fp );
		fp = NULL;
	}
}

void	CWaveRec::Out( LPVOID lpBuf, DWORD dwSize )
{
	if( fp ) {
		::fwrite( lpBuf, dwSize, 1, fp );
	}
}

