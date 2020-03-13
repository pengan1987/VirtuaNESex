/*---------------------------------------------------------------------*
 * The following (piece of) code, (part of) the 2xSaI engine,          *
 * copyright (c) 2001 by Derek Liauw Kie Fa.                           *
 * Non-Commercial use of the engine is allowed and is encouraged,      *
 * provided that appropriate credit be given and that this copyright   *
 * notice will not be removed under any circumstance.                  *
 * You may freely modify this code, but I request                      *
 * that any improvements to the engine be submitted to me, so          *
 * that I can implement these improvements in newer versions of        *
 * the engine.                                                         *
 * If you need more information, have any comments or suggestions,     *
 * you can e-mail me. My e-mail: DerekL666@yahoo.com                   *
 *---------------------------------------------------------------------*/
//
// This code was converted into VirtuaNES by Norix.
//
static	void	nx_SuperEagleLine_16bpp_mmx( euI8* pSrc, euI8* pDlt, euI32 srcPitch, euI32 width, euI8* pDst, euI32 dstPitch, euI32 bForceWrite )
{
	__asm {
		mov		eax, pSrc
		mov		ebx, srcPitch
		mov		edx, pDst
		sub		eax, ebx
nx_SuperEagleLine_16mmx_loop:
		mov		ecx, bForceWrite
		test		ecx, ecx
		jz		nx_SuperEagleLine_16mmx_normal

		mov		esi, pDlt
		movq		mm6, [eax+colorB0]
		movq		[esi+colorB0], mm6

		jmp		nx_SuperEagleLine_16mmx_forcewrite
nx_SuperEagleLine_16mmx_normal:
		// Check delta
		mov		ecx, pDlt

		// load source img
		lea		esi, [eax+ebx]
		movq		mm0, [eax+colorB0]
		movq		mm1, [eax+colorB3]
		movq		mm2, [eax+ebx+color4]
		movq		mm3, [eax+ebx+colorS2]
		movq		mm4, [eax+ebx*2+color1]
		movq		mm5, [eax+ebx*2+colorS1]
		movq		mm6, [esi+ebx*2+colorA0]
		movq		mm7, [esi+ebx*2+colorA3]

		// compare to delta
		lea		esi, [ecx+ebx]
		pcmpeqw		mm0, [ecx+colorB0]
		pcmpeqw		mm1, [ecx+colorB3]
		pcmpeqw		mm2, [ecx+ebx+color4]
		pcmpeqw		mm3, [ecx+ebx+colorS2]
		pcmpeqw		mm4, [ecx+ebx*2+color1]
		pcmpeqw		mm5, [ecx+ebx*2+colorS1]
		pcmpeqw		mm6, [esi+ebx*2+colorA0]
		pcmpeqw		mm7, [esi+ebx*2+colorA3]

		// compose results
		pand		mm0, mm1
		pand		mm2, mm3
		pand		mm4, mm5
		pand		mm6, mm7
		pand		mm0, mm2
		pand		mm4, mm6
		pxor		mm7, mm7
		pand		mm0, mm4
		movq		mm6, [eax+colorB0]
		pcmpeqw		mm7, mm0

		movq		[ecx+colorB0], mm6

		packsswb	mm7, mm7
		movd		ecx, mm7
		test		ecx, ecx
		jz		nx_SuperEagleLine_16mmx_skipprocess
		// End Delta
nx_SuperEagleLine_16mmx_forcewrite:
		//------------------------------
		// Interpolate pixels
		// (c0&c1)+(((c0^c1)&colorMask)>>1)
		//------------------------------
		movq		mm6, cMask

		movq		mm0, [eax+ebx+color5]
		movq		mm1, [eax+ebx+color6]
		movq		mm2, mm0
		movq		mm3, mm1
		movq		mm4, mm0
		movq		mm5, mm1

		pxor		mm3, mm2
		pand		mm0, mm1
		pand		mm3, mm6
		psrlw		mm3, 1
		paddw		mm0, mm3
		movq		I56Pixel, mm0
		movq		mm7, mm0

		//------------------------------
		movq		mm0, mm7
		movq		mm2, mm7
		movq		mm1, mm7
		movq		mm3, mm7
		pxor		mm2, mm4
		pxor		mm3, mm5
		pand		mm0, mm4
		pand		mm2, mm6
		pand		mm1, mm5
		pand		mm3, mm6
		psrlw		mm2, 1
		psrlw		mm3, 1
		paddw		mm0, mm2
		paddw		mm1, mm3
		movq		product1a, mm0
		movq		product1b, mm1

		//------------------------------
		movq		mm0, [eax+ebx*2+color2]
		movq		mm1, [eax+ebx*2+color3]
		movq		mm2, mm0
		movq		mm3, mm1
		movq		mm4, mm0
		movq		mm5, mm1

		pxor		mm3, mm2
		pand		mm0, mm1
		pand		mm3, mm6
		psrlw		mm3, 1
		paddw		mm0, mm3
		movq		I23Pixel, mm0
		movq		mm7, mm0

		//------------------------------
		movq		mm0, mm7
		movq		mm2, mm7
		movq		mm1, mm7
		movq		mm3, mm7
		pxor		mm2, mm4
		pxor		mm3, mm5
		pand		mm0, mm4
		pand		mm2, mm6
		pand		mm1, mm5
		pand		mm3, mm6
		psrlw		mm2, 1
		psrlw		mm3, 1
		paddw		mm0, mm2
		paddw		mm1, mm3
		movq		product2a, mm0
		movq		product2b, mm1

		//------------------------------
		// Decide which "branch" to take
		//------------------------------
		movq		mm4, [eax+ebx+color5]
		movq		mm5, [eax+ebx+color6]
		movq		mm6, [eax+ebx*2+color3]
		movq		mm7, [eax+ebx*2+color2]

		pxor		mm3, mm3
		movq		mm0, mm4
		movq		mm1, mm5

		pcmpeqw		mm0, mm6
		pcmpeqw		mm1, mm7
		pcmpeqw		mm1, mm3
		pand		mm0, mm1
		movq		Mask35, mm0

		lea		esi, [eax+ebx]
		movq		mm0, [eax+ebx*2+colorS1]
		movq		mm1, [eax+ebx+color4]
		movq		mm2, [esi+ebx*2+colorA2]
		movq		mm3, [eax+colorB1]
		pcmpeqw		mm0, mm4
		pcmpeqw		mm1, mm4
		pcmpeqw		mm2, mm4
		pcmpeqw		mm3, mm4
		pand		mm0, mm1
		pand		mm2, mm3
		por		mm0, mm2
		pand		mm0, Mask35
		movq		Mask35b, mm0

		//------------------------------
		pxor		mm3, mm3
		movq		mm0, mm4
		movq		mm1, mm5

		pcmpeqw		mm0, mm6
		pcmpeqw		mm1, mm7
		pcmpeqw		mm0, mm3
		pand		mm0, mm1
		movq		Mask26, mm0

		lea		esi, [eax+ebx]
		movq		mm0, [eax+ebx*2+color1]
		movq		mm1, [eax+ebx+colorS2]
		movq		mm2, [esi+ebx*2+colorA1]
		movq		mm3, [eax+colorB2]
		pcmpeqw		mm0, mm5
		pcmpeqw		mm1, mm5
		pcmpeqw		mm2, mm5
		pcmpeqw		mm3, mm5
		pand		mm0, mm1
		pand		mm2, mm3
		por		mm0, mm2
		pand		mm0, Mask26
		movq		Mask26b, mm0

		//------------------------------
		movq		mm0, mm4
		movq		mm1, mm5
		movq		mm2, mm0

		pcmpeqw		mm2, mm1
		pcmpeqw		mm0, mm6
		pcmpeqw		mm1, mm7
		pand		mm0, mm1
		pand		mm2, mm0
		pxor		mm0, mm2
		movq		mm7, mm0

		//------------------------------
		packsswb	mm7, mm7
		movd		ecx, mm7
		test		ecx, ecx
		jz		nx_SuperEagleLine_16mmx_skipguess

		//------------------------------
		// Map of the pixels: I|E F|J
		//                    G|A B|K
		//                    H|C D|L
		//                    M|N O|P
		//------------------------------
		movq		mm6, mm0
		movq		mm4, [eax+ebx+color5]
		movq		mm5, [eax+ebx+color6]
		pxor		mm7, mm7
		pand		mm6, ONE

		movq		mm0, [eax+colorB1]
		movq		mm1, [eax+ebx+color4]
		movq		mm2, mm0
		movq		mm3, mm1
		pcmpeqw		mm0, mm4
		pcmpeqw		mm1, mm4
		pcmpeqw		mm2, mm5
		pcmpeqw		mm3, mm5
		pand		mm0, mm6
		pand		mm1, mm6
		pand		mm2, mm6
		pand		mm3, mm6
		paddw		mm0, mm1
		paddw		mm2, mm3

		pxor		mm3, mm3
		pcmpgtw		mm0, mm6
		pcmpgtw		mm2, mm6
		pcmpeqw		mm0, mm3
		pcmpeqw		mm2, mm3
		pand		mm0, mm6
		pand		mm2, mm6
		paddw		mm7, mm0
		psubw		mm7, mm2

		movq		mm0, [eax+colorB2]
		movq		mm1, [eax+ebx+colorS2]
		movq		mm2, mm0
		movq		mm3, mm1
		pcmpeqw		mm0, mm4
		pcmpeqw		mm1, mm4
		pcmpeqw		mm2, mm5
		pcmpeqw		mm3, mm5
		pand		mm0, mm6
		pand		mm1, mm6
		pand		mm2, mm6
		pand		mm3, mm6
		paddw		mm0, mm1
		paddw		mm2, mm3

		lea		edi, [eax+ebx]
		pxor		mm3, mm3
		pcmpgtw		mm0, mm6
		pcmpgtw		mm2, mm6
		pcmpeqw		mm0, mm3
		pcmpeqw		mm2, mm3
		pand		mm0, mm6
		pand		mm2, mm6
		paddw		mm7, mm0
		psubw		mm7, mm2

		movq		mm0, [edi+ebx+color1]
		movq		mm1, [edi+ebx*2+colorA1]
		movq		mm2, mm0
		movq		mm3, mm1
		pcmpeqw		mm0, mm4
		pcmpeqw		mm1, mm4
		pcmpeqw		mm2, mm5
		pcmpeqw		mm3, mm5
		pand		mm0, mm6
		pand		mm1, mm6
		pand		mm2, mm6
		pand		mm3, mm6
		paddw		mm0, mm1
		paddw		mm2, mm3

		pxor		mm3, mm3
		pcmpgtw		mm0, mm6
		pcmpgtw		mm2, mm6
		pcmpeqw		mm0, mm3
		pcmpeqw		mm2, mm3
		pand		mm0, mm6
		pand		mm2, mm6
		paddw		mm7, mm0
		psubw		mm7, mm2

		movq		mm0, [edi+ebx+colorS1]
		movq		mm1, [edi+ebx*2+colorA2]
		movq		mm2, mm0
		movq		mm3, mm1
		pcmpeqw		mm0, mm4
		pcmpeqw		mm1, mm4
		pcmpeqw		mm2, mm5
		pcmpeqw		mm3, mm5
		pand		mm0, mm6
		pand		mm1, mm6
		pand		mm2, mm6
		pand		mm3, mm6
		paddw		mm0, mm1
		paddw		mm2, mm3

		pxor		mm3, mm3
		pcmpgtw		mm0, mm6
		pcmpgtw		mm2, mm6
		pcmpeqw		mm0, mm3
		pcmpeqw		mm2, mm3
		pand		mm0, mm6
		pand		mm2, mm6
		paddw		mm7, mm0
		psubw		mm7, mm2

		movq		mm1, mm7
		pxor		mm0, mm0
		pcmpgtw		mm7, mm0
		pcmpgtw		mm0, mm1

		por		mm7, Mask35
		por		mm0, Mask26
		movq		Mask35, mm7
		movq		Mask26, mm0

nx_SuperEagleLine_16mmx_skipguess:
		// Start the ASSEMBLY !!!

		movq		mm4, Mask35
		movq		mm5, Mask26
		movq		mm6, Mask35b
		movq		mm7, Mask26b

		movq		mm0, [eax+ebx+color5]
		movq		mm1, [eax+ebx+color6]
		movq		mm2, [eax+ebx*2+color2]
		movq		mm3, [eax+ebx*2+color3]
		pcmpeqw		mm0, mm2
		pcmpeqw		mm1, mm3
		movq		mm2, mm4
		movq		mm3, mm5
		por		mm0, mm1
		por		mm2, mm3
		pand		mm2, mm0
		pxor		mm0, mm2
		movq		mm3, mm0

		movq		mm2, mm0
		pxor		mm0, mm0
		por		mm2, mm4
		pxor		mm4, mm6
		por		mm2, mm5
		pxor		mm5, mm7
		pcmpeqw		mm2, mm0
		;----------------

		movq		mm0, [eax+ebx+color5]
		movq		mm1, mm3
		por		mm1, mm4
		por		mm1, mm6
		pand		mm0, mm1
		movq		mm1, mm5
		pand		mm1, I56Pixel
		por		mm0, mm1
		movq		mm1, mm7
		pand		mm1, product1b
		por		mm0, mm1
		movq		mm1, mm2
		pand		mm1, product1a
		por		mm0, mm1
		movq		final1a, mm0

		movq		mm0, [eax+ebx+color6]
		movq		mm1, mm3
		por		mm1, mm5
		por		mm1, mm7
		pand		mm0, mm1
		movq		mm1, mm4
		pand		mm1, I56Pixel
		por		mm0, mm1
		movq		mm1, mm6
		pand		mm1, product1a
		por		mm0, mm1
		movq		mm1, mm2
		pand		mm1, product1b
		por		mm0, mm1
		movq		final1b, mm0

		movq		mm0, [eax+ebx*2+color2]
		movq		mm1, mm3
		por		mm1, mm5
		por		mm1, mm7
		pand		mm0, mm1
		movq		mm1, mm4
		pand		mm1, I23Pixel
		por		mm0, mm1
		movq		mm1, mm6
		pand		mm1, product2b
		por		mm0, mm1
		movq		mm1, mm2
		pand		mm1, product2a
		por		mm0, mm1
		movq		final2a, mm0

		movq		mm0, [eax+ebx*2+color3]
		movq		mm1, mm3
		por		mm1, mm4
		por		mm1, mm6
		pand		mm0, mm1
		movq		mm1, mm5
		pand		mm1, I23Pixel
		por		mm0, mm1
		movq		mm1, mm7
		pand		mm1, product2a
		por		mm0, mm1
		movq		mm1, mm2
		pand		mm1, product2b
		por		mm0, mm1
		movq		final2b, mm0

		//------------------------------
		// Write final image
		//------------------------------
		movq		mm0, final1a
		movq		mm2, final1b
		movq		mm1, mm0
		movq		mm4, final2a
		movq		mm6, final2b
		movq		mm5, mm4

		punpcklwd	mm0, mm2		// B1A1B0A0=B3B2B1B0(dst):A3A2A1A0(src)
		punpckhwd	mm1, mm2		// B3A3B2A2=B3B2B1B0(dst):A3A2A1A0(src)
		punpcklwd	mm4, mm6		// B1A1B0A0=B3B2B1B0(dst):A3A2A1A0(src)
		punpckhwd	mm5, mm6		// B3A3B2A2=B3B2B1B0(dst):A3A2A1A0(src)

		mov		edi, dstPitch
		movq		[edx+0], mm0		// 1st line
		movq		[edx+8], mm1
		movq		[edi+edx+0], mm4	// 2nd line
		movq		[edi+edx+8], mm5

nx_SuperEagleLine_16mmx_skipprocess:
		add		pDlt, 8		// 4 pixels
		lea		eax, [eax+ 8]	// 4 pixels
		lea		edx, [edx+16]	// 8 pixels
		sub		width, 4	// 4 pixels
		jg		nx_SuperEagleLine_16mmx_loop

		emms
	}
}

static	void	nx_SuperEagleLine_32bpp_mmx( euI8* pSrc, euI8* pDlt, euI32 srcPitch, euI32 width, euI8* pDst, euI32 dstPitch, euI32 bForceWrite )
{
	__asm {
		mov		eax, pSrc
		mov		ebx, srcPitch
		mov		edx, pDst
		sub		eax, ebx
nx_SuperEagleLine_32mmx_loop:
		mov		ecx, bForceWrite
		test		ecx, ecx
		jz		nx_SuperEagleLine_32mmx_normal

		mov		esi, pDlt
		movq		mm6, [eax+colorB0]
		movq		[esi+colorB0], mm6

		jmp		nx_SuperEagleLine_32mmx_forcewrite
nx_SuperEagleLine_32mmx_normal:
		// Check delta
		mov		ecx, pDlt

		// load source img
		lea		esi, [eax+ebx]
		movq		mm0, [eax+colorB0]
		movq		mm1, [eax+colorB3]
		movq		mm2, [eax+ebx+color4]
		movq		mm3, [eax+ebx+colorS2]
		movq		mm4, [eax+ebx*2+color1]
		movq		mm5, [eax+ebx*2+colorS1]
		movq		mm6, [esi+ebx*2+colorA0]
		movq		mm7, [esi+ebx*2+colorA3]

		// compare to delta
		lea		esi, [ecx+ebx]
		pcmpeqw		mm0, [ecx+colorB0]
		pcmpeqw		mm1, [ecx+colorB3]
		pcmpeqw		mm2, [ecx+ebx+color4]
		pcmpeqw		mm3, [ecx+ebx+colorS2]
		pcmpeqw		mm4, [ecx+ebx*2+color1]
		pcmpeqw		mm5, [ecx+ebx*2+colorS1]
		pcmpeqw		mm6, [esi+ebx*2+colorA0]
		pcmpeqw		mm7, [esi+ebx*2+colorA3]

		// compose results
		pand		mm0, mm1
		pand		mm2, mm3
		pand		mm4, mm5
		pand		mm6, mm7
		pand		mm0, mm2
		pand		mm4, mm6
		pxor		mm7, mm7
		pand		mm0, mm4
		movq		mm6, [eax+colorB0]
		pcmpeqw		mm7, mm0

		movq		[ecx+colorB0], mm6

		packsswb	mm7, mm7
		movd		ecx, mm7
		test		ecx, ecx
		jz		nx_SuperEagleLine_32mmx_skipprocess
		// End Delta
nx_SuperEagleLine_32mmx_forcewrite:
		//------------------------------
		// Interpolate pixels
		// (c0&c1)+(((c0^c1)&colorMask)>>1)
		//------------------------------
		movq		mm6, cMask

		movq		mm0, [eax+ebx+color5]
		movq		mm1, [eax+ebx+color6]
		movq		mm2, mm0
		movq		mm3, mm1
		movq		mm4, mm0
		movq		mm5, mm1

		pxor		mm3, mm2
		pand		mm0, mm1
		pand		mm3, mm6
		psrlw		mm3, 1
		paddw		mm0, mm3
		movq		I56Pixel, mm0
		movq		mm7, mm0

		//------------------------------
		movq		mm0, mm7
		movq		mm2, mm7
		movq		mm1, mm7
		movq		mm3, mm7
		pxor		mm2, mm4
		pxor		mm3, mm5
		pand		mm0, mm4
		pand		mm2, mm6
		pand		mm1, mm5
		pand		mm3, mm6
		psrlw		mm2, 1
		psrlw		mm3, 1
		paddw		mm0, mm2
		paddw		mm1, mm3
		movq		product1a, mm0
		movq		product1b, mm1

		//------------------------------
		movq		mm0, [eax+ebx*2+color2]
		movq		mm1, [eax+ebx*2+color3]
		movq		mm2, mm0
		movq		mm3, mm1
		movq		mm4, mm0
		movq		mm5, mm1

		pxor		mm3, mm2
		pand		mm0, mm1
		pand		mm3, mm6
		psrlw		mm3, 1
		paddw		mm0, mm3
		movq		I23Pixel, mm0
		movq		mm7, mm0

		//------------------------------
		movq		mm0, mm7
		movq		mm2, mm7
		movq		mm1, mm7
		movq		mm3, mm7
		pxor		mm2, mm4
		pxor		mm3, mm5
		pand		mm0, mm4
		pand		mm2, mm6
		pand		mm1, mm5
		pand		mm3, mm6
		psrlw		mm2, 1
		psrlw		mm3, 1
		paddw		mm0, mm2
		paddw		mm1, mm3
		movq		product2a, mm0
		movq		product2b, mm1

		//------------------------------
		// Decide which "branch" to take
		//------------------------------
		movq		mm4, [eax+ebx+color5]
		movq		mm5, [eax+ebx+color6]
		movq		mm6, [eax+ebx*2+color3]
		movq		mm7, [eax+ebx*2+color2]

		pxor		mm3, mm3
		movq		mm0, mm4
		movq		mm1, mm5

		pcmpeqw		mm0, mm6
		pcmpeqw		mm1, mm7
		pcmpeqw		mm1, mm3
		pand		mm0, mm1
		movq		Mask35, mm0

		lea		esi, [eax+ebx]
		movq		mm0, [eax+ebx*2+colorS1]
		movq		mm1, [eax+ebx+color4]
		movq		mm2, [esi+ebx*2+colorA2]
		movq		mm3, [eax+colorB1]
		pcmpeqw		mm0, mm4
		pcmpeqw		mm1, mm4
		pcmpeqw		mm2, mm4
		pcmpeqw		mm3, mm4
		pand		mm0, mm1
		pand		mm2, mm3
		por		mm0, mm2
		pand		mm0, Mask35
		movq		Mask35b, mm0

		//------------------------------
		pxor		mm3, mm3
		movq		mm0, mm4
		movq		mm1, mm5

		pcmpeqw		mm0, mm6
		pcmpeqw		mm1, mm7
		pcmpeqw		mm0, mm3
		pand		mm0, mm1
		movq		Mask26, mm0

		lea		esi, [eax+ebx]
		movq		mm0, [eax+ebx*2+color1]
		movq		mm1, [eax+ebx+colorS2]
		movq		mm2, [esi+ebx*2+colorA1]
		movq		mm3, [eax+colorB2]
		pcmpeqw		mm0, mm5
		pcmpeqw		mm1, mm5
		pcmpeqw		mm2, mm5
		pcmpeqw		mm3, mm5
		pand		mm0, mm1
		pand		mm2, mm3
		por		mm0, mm2
		pand		mm0, Mask26
		movq		Mask26b, mm0

		//------------------------------
		movq		mm0, mm4
		movq		mm1, mm5
		movq		mm2, mm0

		pcmpeqw		mm2, mm1
		pcmpeqw		mm0, mm6
		pcmpeqw		mm1, mm7
		pand		mm0, mm1
		pand		mm2, mm0
		pxor		mm0, mm2
		movq		mm7, mm0

		//------------------------------
		packsswb	mm7, mm7
		movd		ecx, mm7
		test		ecx, ecx
		jz		nx_SuperEagleLine_32mmx_skipguess

		//------------------------------
		// Map of the pixels: I|E F|J
		//                    G|A B|K
		//                    H|C D|L
		//                    M|N O|P
		//------------------------------
		movq		mm6, mm0
		movq		mm4, [eax+ebx+color5]
		movq		mm5, [eax+ebx+color6]
		pxor		mm7, mm7
		pand		mm6, ONE

		movq		mm0, [eax+colorB1]
		movq		mm1, [eax+ebx+color4]
		movq		mm2, mm0
		movq		mm3, mm1
		pcmpeqw		mm0, mm4
		pcmpeqw		mm1, mm4
		pcmpeqw		mm2, mm5
		pcmpeqw		mm3, mm5
		pand		mm0, mm6
		pand		mm1, mm6
		pand		mm2, mm6
		pand		mm3, mm6
		paddw		mm0, mm1
		paddw		mm2, mm3

		pxor		mm3, mm3
		pcmpgtw		mm0, mm6
		pcmpgtw		mm2, mm6
		pcmpeqw		mm0, mm3
		pcmpeqw		mm2, mm3
		pand		mm0, mm6
		pand		mm2, mm6
		paddw		mm7, mm0
		psubw		mm7, mm2

		movq		mm0, [eax+colorB2]
		movq		mm1, [eax+ebx+colorS2]
		movq		mm2, mm0
		movq		mm3, mm1
		pcmpeqw		mm0, mm4
		pcmpeqw		mm1, mm4
		pcmpeqw		mm2, mm5
		pcmpeqw		mm3, mm5
		pand		mm0, mm6
		pand		mm1, mm6
		pand		mm2, mm6
		pand		mm3, mm6
		paddw		mm0, mm1
		paddw		mm2, mm3

		lea		edi, [eax+ebx]
		pxor		mm3, mm3
		pcmpgtw		mm0, mm6
		pcmpgtw		mm2, mm6
		pcmpeqw		mm0, mm3
		pcmpeqw		mm2, mm3
		pand		mm0, mm6
		pand		mm2, mm6
		paddw		mm7, mm0
		psubw		mm7, mm2

		movq		mm0, [edi+ebx+color1]
		movq		mm1, [edi+ebx*2+colorA1]
		movq		mm2, mm0
		movq		mm3, mm1
		pcmpeqw		mm0, mm4
		pcmpeqw		mm1, mm4
		pcmpeqw		mm2, mm5
		pcmpeqw		mm3, mm5
		pand		mm0, mm6
		pand		mm1, mm6
		pand		mm2, mm6
		pand		mm3, mm6
		paddw		mm0, mm1
		paddw		mm2, mm3

		pxor		mm3, mm3
		pcmpgtw		mm0, mm6
		pcmpgtw		mm2, mm6
		pcmpeqw		mm0, mm3
		pcmpeqw		mm2, mm3
		pand		mm0, mm6
		pand		mm2, mm6
		paddw		mm7, mm0
		psubw		mm7, mm2

		movq		mm0, [edi+ebx+colorS1]
		movq		mm1, [edi+ebx*2+colorA2]
		movq		mm2, mm0
		movq		mm3, mm1
		pcmpeqw		mm0, mm4
		pcmpeqw		mm1, mm4
		pcmpeqw		mm2, mm5
		pcmpeqw		mm3, mm5
		pand		mm0, mm6
		pand		mm1, mm6
		pand		mm2, mm6
		pand		mm3, mm6
		paddw		mm0, mm1
		paddw		mm2, mm3

		pxor		mm3, mm3
		pcmpgtw		mm0, mm6
		pcmpgtw		mm2, mm6
		pcmpeqw		mm0, mm3
		pcmpeqw		mm2, mm3
		pand		mm0, mm6
		pand		mm2, mm6
		paddw		mm7, mm0
		psubw		mm7, mm2

		movq		mm1, mm7
		pxor		mm0, mm0
		pcmpgtw		mm7, mm0
		pcmpgtw		mm0, mm1

		por		mm7, Mask35
		por		mm0, Mask26
		movq		Mask35, mm7
		movq		Mask26, mm0

nx_SuperEagleLine_32mmx_skipguess:
		// Start the ASSEMBLY !!!

		movq		mm4, Mask35
		movq		mm5, Mask26
		movq		mm6, Mask35b
		movq		mm7, Mask26b

		movq		mm0, [eax+ebx+color5]
		movq		mm1, [eax+ebx+color6]
		movq		mm2, [eax+ebx*2+color2]
		movq		mm3, [eax+ebx*2+color3]
		pcmpeqw		mm0, mm2
		pcmpeqw		mm1, mm3
		movq		mm2, mm4
		movq		mm3, mm5
		por		mm0, mm1
		por		mm2, mm3
		pand		mm2, mm0
		pxor		mm0, mm2
		movq		mm3, mm0

		movq		mm2, mm0
		pxor		mm0, mm0
		por		mm2, mm4
		pxor		mm4, mm6
		por		mm2, mm5
		pxor		mm5, mm7
		pcmpeqw		mm2, mm0
		;----------------

		movq		mm0, [eax+ebx+color5]
		movq		mm1, mm3
		por		mm1, mm4
		por		mm1, mm6
		pand		mm0, mm1
		movq		mm1, mm5
		pand		mm1, I56Pixel
		por		mm0, mm1
		movq		mm1, mm7
		pand		mm1, product1b
		por		mm0, mm1
		movq		mm1, mm2
		pand		mm1, product1a
		por		mm0, mm1
		movq		final1a, mm0

		movq		mm0, [eax+ebx+color6]
		movq		mm1, mm3
		por		mm1, mm5
		por		mm1, mm7
		pand		mm0, mm1
		movq		mm1, mm4
		pand		mm1, I56Pixel
		por		mm0, mm1
		movq		mm1, mm6
		pand		mm1, product1a
		por		mm0, mm1
		movq		mm1, mm2
		pand		mm1, product1b
		por		mm0, mm1
		movq		final1b, mm0

		movq		mm0, [eax+ebx*2+color2]
		movq		mm1, mm3
		por		mm1, mm5
		por		mm1, mm7
		pand		mm0, mm1
		movq		mm1, mm4
		pand		mm1, I23Pixel
		por		mm0, mm1
		movq		mm1, mm6
		pand		mm1, product2b
		por		mm0, mm1
		movq		mm1, mm2
		pand		mm1, product2a
		por		mm0, mm1
		movq		final2a, mm0

		movq		mm0, [eax+ebx*2+color3]
		movq		mm1, mm3
		por		mm1, mm4
		por		mm1, mm6
		pand		mm0, mm1
		movq		mm1, mm5
		pand		mm1, I23Pixel
		por		mm0, mm1
		movq		mm1, mm7
		pand		mm1, product2a
		por		mm0, mm1
		movq		mm1, mm2
		pand		mm1, product2b
		por		mm0, mm1
		movq		final2b, mm0

		//------------------------------
		// Write final image
		//------------------------------
		movq		mm0, final1a
		movq		mm2, final1b
		movq		mm1, mm0
		movq		mm4, final2a
		movq		mm6, final2b
		movq		mm5, mm4

		punpcklwd	mm0, mm2		// B1A1B0A0=B3B2B1B0(dst):A3A2A1A0(src)
		punpckhwd	mm1, mm2		// B3A3B2A2=B3B2B1B0(dst):A3A2A1A0(src)
		punpcklwd	mm4, mm6		// B1A1B0A0=B3B2B1B0(dst):A3A2A1A0(src)
		punpckhwd	mm5, mm6		// B3A3B2A2=B3B2B1B0(dst):A3A2A1A0(src)

		// Write image  RGB1555->RGBx888
		// save
		mov		esi, eax
		movd		mm7, ebx
		mov		edi, dstPitch
		mov		ecx, 0x00F8F8F8		// mask

		// 1st line
		movd		eax, mm0
		movd		ebx, mm0
		shl		eax, 3
		shl		ebx, 9
		shl		ah, 3
		and		ebx, ecx
		mov		bx, ax
		psrlq		mm0, 16
		mov		[edx+0], ebx
		movd		eax, mm0
		movd		ebx, mm0
		shl		eax, 3
		shl		ebx, 9
		shl		ah, 3
		and		ebx, ecx
		mov		bx, ax
		psrlq		mm0, 16
		mov		[edx+4], ebx
		movd		eax, mm0
		movd		ebx, mm0
		shl		eax, 3
		shl		ebx, 9
		shl		ah, 3
		and		ebx, ecx
		mov		bx, ax
		psrlq		mm0, 16
		mov		[edx+8], ebx
		movd		eax, mm0
		movd		ebx, mm0
		shl		eax, 3
		shl		ebx, 9
		shl		ah, 3
		and		ebx, ecx
		mov		bx, ax
		mov		[edx+12], ebx

		movd		eax, mm1
		movd		ebx, mm1
		shl		eax, 3
		shl		ebx, 9
		shl		ah, 3
		and		ebx, ecx
		mov		bx, ax
		psrlq		mm1, 16
		mov		[edx+16], ebx
		movd		eax, mm1
		movd		ebx, mm1
		shl		eax, 3
		shl		ebx, 9
		shl		ah, 3
		and		ebx, ecx
		mov		bx, ax
		psrlq		mm1, 16
		mov		[edx+20], ebx
		movd		eax, mm1
		movd		ebx, mm1
		shl		eax, 3
		shl		ebx, 9
		shl		ah, 3
		and		ebx, ecx
		mov		bx, ax
		psrlq		mm1, 16
		mov		[edx+24], ebx
		movd		eax, mm1
		movd		ebx, mm1
		shl		eax, 3
		shl		ebx, 9
		shl		ah, 3
		and		ebx, ecx
		mov		bx, ax
		mov		[edx+28], ebx

		// 2nd line
		movd		eax, mm4
		movd		ebx, mm4
		shl		eax, 3
		shl		ebx, 9
		shl		ah, 3
		and		ebx, ecx
		mov		bx, ax
		psrlq		mm4, 16
		mov		[edx+edi+ 0], ebx
		movd		eax, mm4
		movd		ebx, mm4
		shl		eax, 3
		shl		ebx, 9
		shl		ah, 3
		and		ebx, ecx
		mov		bx, ax
		psrlq		mm4, 16
		mov		[edx+edi+ 4], ebx
		movd		eax, mm4
		movd		ebx, mm4
		shl		eax, 3
		shl		ebx, 9
		shl		ah, 3
		and		ebx, ecx
		mov		bx, ax
		psrlq		mm4, 16
		mov		[edx+edi+ 8], ebx
		movd		eax, mm4
		movd		ebx, mm4
		shl		eax, 3
		shl		ebx, 9
		shl		ah, 3
		and		ebx, ecx
		mov		bx, ax
		mov		[edx+edi+12], ebx

		movd		eax, mm5
		movd		ebx, mm5
		shl		eax, 3
		shl		ebx, 9
		shl		ah, 3
		and		ebx, ecx
		mov		bx, ax
		psrlq		mm5, 16
		mov		[edx+edi+16], ebx
		movd		eax, mm5
		movd		ebx, mm5
		shl		eax, 3
		shl		ebx, 9
		shl		ah, 3
		and		ebx, ecx
		mov		bx, ax
		psrlq		mm5, 16
		mov		[edx+edi+20], ebx
		movd		eax, mm5
		movd		ebx, mm5
		shl		eax, 3
		shl		ebx, 9
		shl		ah, 3
		and		ebx, ecx
		mov		bx, ax
		psrlq		mm5, 16
		mov		[edx+edi+24], ebx
		movd		eax, mm5
		movd		ebx, mm5
		shl		eax, 3
		shl		ebx, 9
		shl		ah, 3
		and		ebx, ecx
		mov		bx, ax
		mov		[edx+edi+28], ebx

		// restore
		mov		eax, esi
		movd		ebx, mm7

nx_SuperEagleLine_32mmx_skipprocess:
		add		pDlt, 8		// 4 pixels
		lea		eax, [eax+ 8]	// 4 pixels
		lea		edx, [edx+32]	// 8 pixels
		sub		width, 4	// 4 pixels
		jg		nx_SuperEagleLine_32mmx_loop

		emms
	}
}
