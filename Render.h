//
// Render
//

// 16bit Render
void	CDirectDraw::Render16bpp( LPBYTE lpSrc, LPBYTE lpDst )
{
	LPBYTE	pPal;
	DWORD	width;
	DWORD	pitch = SCREEN_WIDTH*sizeof(WORD);

	for( INT i = 0; i < SCREEN_HEIGHT; i++ ) {
		if( !(m_LineColormode[i]&0x80) ) {
			pPal = (LPBYTE)m_cnPalette[m_LineColormode[i]&0x07];
		} else {
			pPal = (LPBYTE)m_mnPalette[m_LineColormode[i]&0x07];
		}

		width = SCREEN_WIDTH;

		__asm {
			mov		eax, lpSrc
			mov		esi, pPal
			mov		edi, lpDst
_r16b_loop:
			mov		edx, [eax+0]
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 0], cx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 2], cx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+ 4], cx
			mov		[edi+ 6], dx

			mov		edx, [eax+4]
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 8], cx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+10], cx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+12], cx
			mov		[edi+14], dx

			lea		eax, [eax+ 8]
			lea		edi, [edi+16]

			sub		width, 8
			jg		_r16b_loop
		}

		lpSrc += RENDER_WIDTH;
		lpDst += SCREEN_WIDTH*sizeof(WORD);
	}
}

// 16bit Pre-Render
void	CDirectDraw::Render16bppPrefilter( LPBYTE lpSrc, LPBYTE lpDst )
{
	LPBYTE	pPal;
	DWORD	width;
	DWORD	pitch = SCREEN_WIDTH*sizeof(WORD);

	for( INT i = 0; i < SCREEN_HEIGHT; i++ ) {
		if( !(m_LineColormode[i]&0x80) ) {
			pPal = (LPBYTE)m_cfPalette[m_LineColormode[i]&0x07];
		} else {
			pPal = (LPBYTE)m_mfPalette[m_LineColormode[i]&0x07];
		}

		width = SCREEN_WIDTH;

		__asm {
			mov		eax, lpSrc
			mov		esi, pPal
			mov		edi, lpDst
_r16b_pf_loop:
			mov		edx, [eax+0]
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 0], cx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 2], cx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+ 4], cx
			mov		[edi+ 6], dx

			mov		edx, [eax+4]
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 8], cx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+10], cx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+12], cx
			mov		[edi+14], dx

			lea		eax, [eax+ 8]
			lea		edi, [edi+16]

			sub		width, 8
			jg		_r16b_pf_loop
		}

		lpSrc += RENDER_WIDTH;
		lpDst += SCREEN_WIDTH*sizeof(WORD);
	}
}

// 32bit Render
void	CDirectDraw::Render32bpp( LPBYTE lpSrc, LPBYTE lpDst )
{
	LPBYTE	pPal;
	DWORD	width;
	DWORD	pitch = SCREEN_WIDTH*sizeof(DWORD);

	for( INT i = 0; i < SCREEN_HEIGHT; i++ ) {
		if( !(m_LineColormode[i]&0x80) ) {
			pPal = (LPBYTE)m_cnPalette[m_LineColormode[i]&0x07];
		} else {
			pPal = (LPBYTE)m_mnPalette[m_LineColormode[i]&0x07];
		}

		width = SCREEN_WIDTH;

		__asm {
			mov		eax, lpSrc
			mov		esi, pPal
			mov		edi, lpDst
_r32b_loop:
			mov		edx, [eax+0]
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 0], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 4], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+ 8], ecx
			mov		[edi+12], edx

			mov		edx, [eax+4]
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+16], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+20], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+24], ecx
			mov		[edi+28], edx

			lea		eax, [eax+ 8]
			lea		edi, [edi+32]

			sub		width, 8
			jg		_r32b_loop
		}

		lpSrc += RENDER_WIDTH;
		lpDst += SCREEN_WIDTH*sizeof(DWORD);
	}
}

// Normal/Scanline/Double/DoubleScanlie Render
#include "Render8bpp.h"
#include "Render16bpp.h"
#include "Render24bpp.h"
#include "Render32bpp.h"

typedef	signed char		eI8;
typedef	signed short		eI16;
typedef	signed long		eI32;
typedef	signed __int64		eI64;
typedef	unsigned char		euI8;
typedef	unsigned short		euI16;
typedef	unsigned long		euI32;
typedef	unsigned __int64	euI64;

static	euI64	ONE   = 0x0001000100010001;
static	euI64	cMask;
static	euI64	qMask;
static	euI64	lMask;
static	euI64	ACPixel;
static	euI64	Mask1;
static	euI64	Mask2;
static	euI64	I56Pixel;
static	euI64	I5556Pixel;
static	euI64	I5666Pixel;
static	euI64	I23Pixel;
static	euI64	I2223Pixel;
static	euI64	I2333Pixel;
static	euI64	Mask26;
static	euI64	Mask35;
static	euI64	Mask26b;
static	euI64	Mask35b;
static	euI64	product1a;
static	euI64	product1b;
static	euI64	product2a;
static	euI64	product2b;
static	euI64	final1a;
static	euI64	final1b;
static	euI64	final2a;
static	euI64	final2b;

#define	colorI		-2
#define	colorE		0
#define	colorF		2
#define	colorJ		4
#define	colorG		-2
#define	colorA		0
#define	colorB		2
#define	colorK		4
#define	colorH		-2
#define	colorC		0
#define	colorD		2
#define	colorL		4
#define	colorM		-2
#define	colorN		0
#define	colorO		2
#define	colorP		4

#define	colorB0		-2
#define	colorB1		0
#define	colorB2		2
#define	colorB3		4
#define	color4		-2
#define	color5		0
#define	color6		2
#define	colorS2		4
#define	color1		-2
#define	color2		0
#define	color3		2
#define	colorS1		4
#define	colorA0		-2
#define	colorA1		0
#define	colorA2		2
#define	colorA3		4

// Filtering Render
#include "nx_2xSaI.h"
#include "nx_Super2xSaI.h"
#include "nx_SuperEagle.h"
#include "nx_Scale2x.h"

void	CDirectDraw::nx_2xSaI_16bpp( LPBYTE lpRdr, LPBYTE lpDlt, DDSURFACEDESC2& ddsd, BOOL bForceWrite )
{
	LPBYTE	pScn = new BYTE[SCREEN_WIDTH*(SCREEN_HEIGHT+6)*sizeof(WORD)];

	// Pre-Rendering
	Render16bpp( lpRdr, &pScn[SCREEN_WIDTH*2*sizeof(WORD)] );

	euI8*	srcPtr   = (euI8*)&pScn[SCREEN_WIDTH*2*sizeof(WORD)];
	euI32	srcPitch = SCREEN_WIDTH*sizeof(WORD);
	euI8*	deltaPtr = (euI8*)lpDlt;
	euI8*	dstPtr   = (euI8*)ddsd.lpSurface;
	euI32	dstPitch = (euI32)ddsd.lPitch;
	int	width    = SCREEN_WIDTH;
	int	height   = SCREEN_HEIGHT;

	if( ddsd.ddpfPixelFormat.dwGBitMask == 0x01E0 ) {
	// 555
		cMask = 0x7BDE7BDE7BDE7BDE;
		qMask = 0x739C739C739C739C;
		lMask = 0x0C630C630C630C63;
	} else {
	// 565
		cMask = 0xF7DEF7DEF7DEF7DE;
		qMask = 0xE79CE79CE79CE79C;
		lMask = 0x1863186318631863;
	}

	for( ; height; height-- ) {
		nx_2xSaILine_16bpp_mmx( srcPtr, deltaPtr, srcPitch, width, dstPtr, dstPitch, bForceWrite );
		srcPtr += srcPitch;
		dstPtr += dstPitch * 2;
		deltaPtr += srcPitch;
	}

	delete[] pScn;
}

void	CDirectDraw::nx_2xSaI_32bpp( LPBYTE lpRdr, LPBYTE lpDlt, DDSURFACEDESC2& ddsd, BOOL bForceWrite )
{
	LPBYTE	pScn = new BYTE[SCREEN_WIDTH*(SCREEN_HEIGHT+6)*sizeof(WORD)];

	// Pre-Rendering
	Render16bppPrefilter( lpRdr, &pScn[SCREEN_WIDTH*2*sizeof(WORD)] );

	euI8*	srcPtr   = (euI8*)&pScn[SCREEN_WIDTH*2*sizeof(WORD)];
	euI32	srcPitch = SCREEN_WIDTH*sizeof(WORD);
	euI8*	deltaPtr = (euI8*)lpDlt;
	euI8*	dstPtr   = (euI8*)ddsd.lpSurface;
	euI32	dstPitch = (euI32)ddsd.lPitch;
	int	width    = SCREEN_WIDTH;
	int	height   = SCREEN_HEIGHT;

	// 555
	cMask = 0x7BDE7BDE7BDE7BDE;
	qMask = 0x739C739C739C739C;
	lMask = 0x0C630C630C630C63;

	for( ; height; height-- ) {
		nx_2xSaILine_32bpp_mmx( srcPtr, deltaPtr, srcPitch, width, dstPtr, dstPitch, bForceWrite );
		srcPtr += srcPitch;
		dstPtr += dstPitch * 2;
		deltaPtr += srcPitch;
	}

	delete[] pScn;
}

void	CDirectDraw::nx_Super2xSaI_16bpp( LPBYTE lpRdr, LPBYTE lpDlt, DDSURFACEDESC2& ddsd, BOOL bForceWrite )
{
	LPBYTE	pScn = new BYTE[SCREEN_WIDTH*(SCREEN_HEIGHT+6)*sizeof(WORD)];

	// Pre-Rendering
	Render16bpp( lpRdr, &pScn[SCREEN_WIDTH*2*sizeof(WORD)] );

	euI8*	srcPtr   = (euI8*)&pScn[SCREEN_WIDTH*2*sizeof(WORD)];
	euI32	srcPitch = SCREEN_WIDTH*sizeof(WORD);
	euI8*	deltaPtr = (euI8*)lpDlt;
	euI8*	dstPtr   = (euI8*)ddsd.lpSurface;
	euI32	dstPitch = (euI32)ddsd.lPitch;
	int	width    = SCREEN_WIDTH;
	int	height   = SCREEN_HEIGHT;

	if( ddsd.ddpfPixelFormat.dwGBitMask == 0x01E0 ) {
	// 555
		cMask = 0x7BDE7BDE7BDE7BDE;
		qMask = 0x739C739C739C739C;
		lMask = 0x0C630C630C630C63;
	} else {
	// 565
		cMask = 0xF7DEF7DEF7DEF7DE;
		qMask = 0xE79CE79CE79CE79C;
		lMask = 0x1863186318631863;
	}

	for( ; height; height-- ) {
		nx_Super2xSaILine_16bpp_mmx( srcPtr, deltaPtr, srcPitch, width, dstPtr, dstPitch, bForceWrite );
		srcPtr += srcPitch;
		dstPtr += dstPitch * 2;
		deltaPtr += srcPitch;
	}

	delete[] pScn;
}

void	CDirectDraw::nx_Super2xSaI_32bpp( LPBYTE lpRdr, LPBYTE lpDlt, DDSURFACEDESC2& ddsd, BOOL bForceWrite )
{
	LPBYTE	pScn = new BYTE[SCREEN_WIDTH*(SCREEN_HEIGHT+6)*sizeof(WORD)];

	// Pre-Rendering
	Render16bppPrefilter( lpRdr, &pScn[SCREEN_WIDTH*2*sizeof(WORD)] );

	euI8*	srcPtr   = (euI8*)&pScn[SCREEN_WIDTH*2*sizeof(WORD)];
	euI32	srcPitch = SCREEN_WIDTH*sizeof(WORD);
	euI8*	deltaPtr = (euI8*)lpDlt;
	euI8*	dstPtr   = (euI8*)ddsd.lpSurface;
	euI32	dstPitch = (euI32)ddsd.lPitch;
	int	width    = SCREEN_WIDTH;
	int	height   = SCREEN_HEIGHT;

	// 555
	cMask = 0x7BDE7BDE7BDE7BDE;
	qMask = 0x739C739C739C739C;
	lMask = 0x0C630C630C630C63;

	for( ; height; height-- ) {
		nx_Super2xSaILine_32bpp_mmx( srcPtr, deltaPtr, srcPitch, width, dstPtr, dstPitch, bForceWrite );
		srcPtr += srcPitch;
		dstPtr += dstPitch * 2;
		deltaPtr += srcPitch;
	}

	delete[] pScn;
}

void	CDirectDraw::nx_SuperEagle_16bpp( LPBYTE lpRdr, LPBYTE lpDlt, DDSURFACEDESC2& ddsd, BOOL bForceWrite )
{
	LPBYTE	pScn = new BYTE[SCREEN_WIDTH*(SCREEN_HEIGHT+6)*sizeof(WORD)];

	// Pre-Rendering
	Render16bpp( lpRdr, &pScn[SCREEN_WIDTH*2*sizeof(WORD)] );

	euI8*	srcPtr   = (euI8*)&pScn[SCREEN_WIDTH*2*sizeof(WORD)];
	euI32	srcPitch = SCREEN_WIDTH*sizeof(WORD);
	euI8*	deltaPtr = (euI8*)lpDlt;
	euI8*	dstPtr   = (euI8*)ddsd.lpSurface;
	euI32	dstPitch = (euI32)ddsd.lPitch;
	int	width    = SCREEN_WIDTH;
	int	height   = SCREEN_HEIGHT;

	if( ddsd.ddpfPixelFormat.dwGBitMask == 0x01E0 ) {
	// 555
		cMask = 0x7BDE7BDE7BDE7BDE;
		qMask = 0x739C739C739C739C;
		lMask = 0x0C630C630C630C63;
	} else {
	// 565
		cMask = 0xF7DEF7DEF7DEF7DE;
		qMask = 0xE79CE79CE79CE79C;
		lMask = 0x1863186318631863;
	}

	for( ; height; height-- ) {
		nx_SuperEagleLine_16bpp_mmx( srcPtr, deltaPtr, srcPitch, width, dstPtr, dstPitch, bForceWrite );
		srcPtr += srcPitch;
		dstPtr += dstPitch * 2;
		deltaPtr += srcPitch;
	}

	delete[] pScn;
}

void	CDirectDraw::nx_SuperEagle_32bpp( LPBYTE lpRdr, LPBYTE lpDlt, DDSURFACEDESC2& ddsd, BOOL bForceWrite )
{
	LPBYTE	pScn = new BYTE[SCREEN_WIDTH*(SCREEN_HEIGHT+6)*sizeof(WORD)];

	// Pre-Rendering
	Render16bppPrefilter( lpRdr, &pScn[SCREEN_WIDTH*2*sizeof(WORD)] );

	euI8*	srcPtr   = (euI8*)&pScn[SCREEN_WIDTH*2*sizeof(WORD)];
	euI32	srcPitch = SCREEN_WIDTH*sizeof(WORD);
	euI8*	deltaPtr = (euI8*)lpDlt;
	euI8*	dstPtr   = (euI8*)ddsd.lpSurface;
	euI32	dstPitch = (euI32)ddsd.lPitch;
	int	width    = SCREEN_WIDTH;
	int	height   = SCREEN_HEIGHT;

	// 555
	cMask = 0x7BDE7BDE7BDE7BDE;
	qMask = 0x739C739C739C739C;
	lMask = 0x0C630C630C630C63;

	for( ; height; height-- ) {
		nx_SuperEagleLine_32bpp_mmx( srcPtr, deltaPtr, srcPitch, width, dstPtr, dstPitch, bForceWrite );
		srcPtr += srcPitch;
		dstPtr += dstPitch * 2;
		deltaPtr += srcPitch;
	}

	delete[] pScn;
}

void	CDirectDraw::nx_Scale2x_16bpp( LPBYTE lpRdr, LPBYTE lpDlt, DDSURFACEDESC2& ddsd, BOOL bForceWrite )
{
	// Pre-Rendering
	Render16bpp( lpRdr, lpDlt );

	euI8*	srcPtr   = (euI8*)lpDlt;
	euI32	srcPitch = SCREEN_WIDTH * sizeof(euI16);
	euI8*	dstPtr   = (euI8*)ddsd.lpSurface;
	euI32	dstPitch = ddsd.lPitch;
	int	width    = SCREEN_WIDTH;
	int	height   = SCREEN_HEIGHT;

	euI16	*dst0 = (euI16*)dstPtr;
	euI16	*dst1 = dst0 + (dstPitch/2);
	euI16	*src0 = (euI16*)lpDlt;
	euI16	*src1 = src0 + (srcPitch/2);
	euI16	*src2 = src1 + (srcPitch/2);

	internal_scale2x_16_mmx( dst0, dst1, src0, src0, src1, width );

	int count = height;
	count -= 2;
	while( count ) {
		dst0 += dstPitch;
		dst1 += dstPitch;
		internal_scale2x_16_mmx( dst0, dst1, src0, src1, src2, width );
		src0 = src1;
		src1 = src2;
		src2 += srcPitch/2;
		--count;
	}
	dst0 += dstPitch;
	dst1 += dstPitch;
	internal_scale2x_16_mmx( dst0, dst1, src0, src1, src1, width );
}

void	CDirectDraw::nx_Scale2x_32bpp( LPBYTE lpRdr, LPBYTE lpDlt, DDSURFACEDESC2& ddsd, BOOL bForceWrite )
{
	// Pre-Rendering
	Render32bpp( lpRdr, lpDlt );

	euI8*	srcPtr   = (euI8*)lpDlt;
	euI32	srcPitch = SCREEN_WIDTH * sizeof(euI32);
	euI8*	dstPtr   = (euI8*)ddsd.lpSurface;
	euI32	dstPitch = ddsd.lPitch;
	int	width    = SCREEN_WIDTH;
	int	height   = SCREEN_HEIGHT;

	euI32	*dst0 = (euI32*)dstPtr;
	euI32	*dst1 = dst0 + (dstPitch/4);
	euI32	*src0 = (euI32*)lpDlt;
	euI32	*src1 = src0 + (srcPitch/4);
	euI32	*src2 = src1 + (srcPitch/4);

	internal_scale2x_32_mmx( dst0, dst1, src0, src0, src1, width );

	int count = height;
	count -= 2;
	while( count ) {
		dst0 += dstPitch/2;
		dst1 += dstPitch/2;
		internal_scale2x_32_mmx( dst0, dst1, src0, src1, src2, width );
		src0 = src1;
		src1 = src2;
		src2 += srcPitch/4;
		--count;
	}
	dst0 += dstPitch/2;
	dst1 += dstPitch/2;
	internal_scale2x_32_mmx( dst0, dst1, src0, src1, src1, width );
}
