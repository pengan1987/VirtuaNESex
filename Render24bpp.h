//
// 24bit Normal
//
void	CDirectDraw::Render24bpp_Normal( LPBYTE lpRdr, LPBYTE lpDlt, DDSURFACEDESC2& ddsd, BOOL bForceWrite )
{
	LPBYTE	pScn = lpRdr;
	LPBYTE	pDst = (LPBYTE)ddsd.lpSurface;
	LPBYTE	pDlt = (LPBYTE)lpDlt;
	LPBYTE	pPal;
	DWORD	width;
	DWORD	pitch = ddsd.lPitch;

	for( INT i = 0; i < SCREEN_HEIGHT; i++ ) {
		if( !(m_LineColormode[i]&0x80) ) {
			pPal = (LPBYTE)m_cnPalette[m_LineColormode[i]&0x07];
		} else {
			pPal = (LPBYTE)m_mnPalette[m_LineColormode[i]&0x07];
		}

		BOOL	bFWrite = FALSE;
		if( bForceWrite || *pDlt != m_LineColormode[i] ) {
			bFWrite = TRUE;
			*pDlt = m_LineColormode[i];
		}
		pDlt += 4;

		width = SCREEN_WIDTH;

		if( bFWrite ) {
		__asm {
			mov		eax, pScn
			mov		ebx, pDlt
			mov		esi, pPal
			mov		edi, pDst
_r24bn_loop_fw:
			mov		edx, [eax+0]
			mov		[ebx+0], edx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 0], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 3], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+ 6], ecx
			mov		[edi+ 9], dx
			shr		edx, 16
			mov		[edi+11], dl

			mov		edx, [eax+4]
			mov		[ebx+4], edx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+12], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+15], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+18], ecx
			mov		[edi+21], dx
			shr		edx, 16
			mov		[edi+23], dl

			lea		eax, [eax+ 8]
			lea		ebx, [ebx+ 8]
			lea		edi, [edi+24]

			sub		width, 8
			jg		_r24bn_loop_fw
			}

			pScn += RENDER_WIDTH;
			pDlt += SCREEN_WIDTH;
			pDst += pitch;
		} else {
		__asm {
			mov		eax, pScn
			mov		ebx, pDlt
			mov		esi, pPal
			mov		edi, pDst
_r24bn_loop:
			// check previous!!
			mov		edx, [eax+0]
			cmp		edx, [ebx+0]
			je		_r24bn_skip1
			mov		[ebx+0], edx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 0], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 3], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+ 6], ecx
			mov		[edi+ 9], dx
			shr		edx, 16
			mov		[edi+11], dl
_r24bn_skip1:
			// check previous!!
			mov		edx, [eax+4]
			cmp		edx, [ebx+4]
			je		_r24bn_skip2
			mov		[ebx+4], edx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+12], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+15], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+18], ecx
			mov		[edi+21], dx
			shr		edx, 16
			mov		[edi+23], dl
_r24bn_skip2:
			lea		eax, [eax+ 8]
			lea		ebx, [ebx+ 8]
			lea		edi, [edi+24]

			sub		width, 8
			jg		_r24bn_loop
			}

			pScn += RENDER_WIDTH;
			pDlt += SCREEN_WIDTH;
			pDst += pitch;
		}
	}
}

//
// 24bit Scanline
//
void	CDirectDraw::Render24bpp_Scanline( LPBYTE lpRdr, LPBYTE lpDlt, DDSURFACEDESC2& ddsd, BOOL bForceWrite )
{
	LPBYTE	pScn = lpRdr;
	LPBYTE	pDst = (LPBYTE)ddsd.lpSurface;
	LPBYTE	pDlt = (LPBYTE)lpDlt;
	LPBYTE	pPal, pPalScan;
	DWORD	width = SCREEN_WIDTH;
	DWORD	pitch = ddsd.lPitch;

	for( INT i = 0; i < SCREEN_HEIGHT; i++ ) {
		if( !(m_LineColormode[i]&0x80) ) {
			pPal     = (LPBYTE)m_cnPalette[m_LineColormode[i]&0x07];
			pPalScan = (LPBYTE)m_csPalette[m_LineColormode[i]&0x07];
		} else {
			pPal     = (LPBYTE)m_mnPalette[m_LineColormode[i]&0x07];
			pPalScan = (LPBYTE)m_msPalette[m_LineColormode[i]&0x07];
		}

		BOOL	bFWrite = FALSE;
		if( bForceWrite || *pDlt != m_LineColormode[i] ) {
			bFWrite = TRUE;
			*pDlt = m_LineColormode[i];
		}
		pDlt += 4;

		width = SCREEN_WIDTH;

		if( bFWrite ) {
		__asm {
			mov		eax, pScn
			mov		ebx, pDlt
			mov		esi, pPal
			mov		edi, pDst
_r24bs_loop_fw:
			// check previous!!
			mov		edx, [eax+0]
			mov		[ebx+0], edx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 0], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 3], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+ 6], ecx
			mov		[edi+ 9], dx
			shr		edx, 16
			mov		[edi+11], dl

			mov		edx, [eax+4]
			mov		[ebx+4], edx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+12], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+15], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+18], ecx
			mov		[edi+21], dx
			shr		edx, 16
			mov		[edi+23], dl

			lea		eax, [eax+ 8]
			lea		ebx, [ebx+ 8]
			lea		edi, [edi+24]

			sub		width, 8
			jg		_r24bs_loop_fw
			}

			width = SCREEN_WIDTH;
			pDst += pitch;

		__asm {
			mov		eax, pScn
			mov		ebx, pDlt
			mov		esi, pPalScan
			mov		edi, pDst
_r24bs_loop2_fw:
			mov		edx, [eax+0]
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 0], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 3], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+ 6], ecx
			mov		[edi+ 9], dx
			shr		edx, 16
			mov		[edi+11], dl

			mov		edx, [eax+4]
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+12], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+15], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+18], ecx
			mov		[edi+21], dx
			shr		edx, 16
			mov		[edi+23], dl

			lea		eax, [eax+ 8]
			lea		ebx, [ebx+ 8]
			lea		edi, [edi+24]

			sub		width, 8
			jg		_r24bs_loop2_fw
			}

			pScn += RENDER_WIDTH;
			pDlt += SCREEN_WIDTH;
			pDst += pitch;
		} else {
		__asm {
			mov		eax, pScn
			mov		ebx, pDlt
			mov		esi, pPal
			mov		edi, pDst
_r24bs_loop:
			// check previous!!
			mov		edx, [eax+0]
			cmp		edx, [ebx+0]
			je		_r24bs_skip1
//			mov		[ebx+0], edx		// 1‰ñ–Ú‚Í‘‚«ž‚Ü‚È‚¢
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 0], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 3], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+ 6], ecx
			mov		[edi+ 9], dx
			shr		edx, 16
			mov		[edi+11], dl
_r24bs_skip1:
			// check previous!!
			mov		edx, [eax+4]
			cmp		edx, [ebx+4]
			je		_r24bs_skip2
//			mov		[ebx+4], edx		// 1‰ñ–Ú‚Í‘‚«ž‚Ü‚È‚¢
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+12], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+15], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+18], ecx
			mov		[edi+21], dx
			shr		edx, 16
			mov		[edi+23], dl
_r24bs_skip2:
			lea		eax, [eax+ 8]
			lea		ebx, [ebx+ 8]
			lea		edi, [edi+24]

			sub		width, 8
			jg		_r24bs_loop
			}

			width = SCREEN_WIDTH;
			pDst += pitch;

		__asm {
			mov		eax, pScn
			mov		ebx, pDlt
			mov		esi, pPalScan
			mov		edi, pDst
_r24bs_loop2:
			// check previous!!
			mov		edx, [eax+0]
			cmp		edx, [ebx+0]
			je		_r24bs_skip3
			mov		[ebx+0], edx		// 2‰ñ–Ú‚Í‘‚«ž‚Þ
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 0], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 3], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+ 6], ecx
			mov		[edi+ 9], dx
			shr		edx, 16
			mov		[edi+11], dl
_r24bs_skip3:
			// check previous!!
			mov		edx, [eax+4]
			cmp		edx, [ebx+4]
			je		_r24bs_skip4
			mov		[ebx+4], edx		// 2‰ñ–Ú‚Í‘‚«ž‚Þ
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+12], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+15], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+18], ecx
			mov		[edi+21], dx
			shr		edx, 16
			mov		[edi+23], dl
_r24bs_skip4:
			lea		eax, [eax+ 8]
			lea		ebx, [ebx+ 8]
			lea		edi, [edi+24]

			sub		width, 8
			jg		_r24bs_loop2
			}

			pScn += RENDER_WIDTH;
			pDlt += SCREEN_WIDTH;
			pDst += pitch;
		}
	}
}

//
// 24bit Double
//
void	CDirectDraw::Render24bpp_Double( LPBYTE lpRdr, LPBYTE lpDlt, DDSURFACEDESC2& ddsd, BOOL bForceWrite )
{
	LPBYTE	pScn = lpRdr;
	LPBYTE	pDst = (LPBYTE)ddsd.lpSurface;
	LPBYTE	pDlt = (LPBYTE)lpDlt;
	LPBYTE	pPal;
	DWORD	width;
	DWORD	pitch = ddsd.lPitch;

	for( INT i = 0; i < SCREEN_HEIGHT; i++ ) {
		if( !(m_LineColormode[i]&0x80) ) {
			pPal = (LPBYTE)m_cnPalette[m_LineColormode[i]&0x07];
		} else {
			pPal = (LPBYTE)m_mnPalette[m_LineColormode[i]&0x07];
		}

		BOOL	bFWrite = FALSE;
		if( bForceWrite || *pDlt != m_LineColormode[i] ) {
			bFWrite = TRUE;
			*pDlt = m_LineColormode[i];
		}
		pDlt += 4;

		width = SCREEN_WIDTH;

		if( bFWrite ) {
		__asm {
			mov		eax, pScn
			mov		ebx, pDlt
			mov		esi, pPal
			mov		edi, pDst
_r24bn_d_loop_fw:
			// check previous!!
			mov		edx, [eax+0]
			mov		[ebx+0], edx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 0], ecx
			mov		[edi+ 3], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 6], ecx
			mov		[edi+ 9], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+12], ecx
			mov		[edi+15], ecx
			mov		[edi+18], edx
			mov		[edi+21], dx
			shr		edx, 16
			mov		[edi+23], dl

			// check previous!!
			mov		edx, [eax+4]
			mov		[ebx+4], edx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+24], ecx
			mov		[edi+27], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+30], ecx
			mov		[edi+33], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+36], ecx
			mov		[edi+39], ecx
			mov		[edi+42], edx
			mov		[edi+45], edx
			shr		edx, 16
			mov		[edi+47], dl

			lea		eax, [eax+ 8]
			lea		ebx, [ebx+ 8]
			lea		edi, [edi+48]

			sub		width, 8
			jg		_r24bn_d_loop_fw
			}

			width = SCREEN_WIDTH;
			pDst += pitch;

		__asm {
			mov		eax, pScn
			mov		ebx, pDlt
			mov		esi, pPal
			mov		edi, pDst
_r24bn_d_loop2_fw:
			mov		edx, [eax+0]
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 0], ecx
			mov		[edi+ 3], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 6], ecx
			mov		[edi+ 9], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+12], ecx
			mov		[edi+15], ecx
			mov		[edi+18], edx
			mov		[edi+21], dx
			shr		edx, 16
			mov		[edi+23], dl

			mov		edx, [eax+4]
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+24], ecx
			mov		[edi+27], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+30], ecx
			mov		[edi+33], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+36], ecx
			mov		[edi+39], ecx
			mov		[edi+42], edx
			mov		[edi+45], dx
			shr		edx, 16
			mov		[edi+47], dl

			lea		eax, [eax+ 8]
			lea		ebx, [ebx+ 8]
			lea		edi, [edi+48]

			sub		width, 8
			jg		_r24bn_d_loop2_fw
			}

			pScn += RENDER_WIDTH;
			pDlt += SCREEN_WIDTH;
			pDst += pitch;
		} else {
		__asm {
			mov		eax, pScn
			mov		ebx, pDlt
			mov		esi, pPal
			mov		edi, pDst
_r24bn_d_loop:
			// check previous!!
			mov		edx, [eax+0]
			cmp		edx, [ebx+0]
			je		_r24bn_d_skip1
//			mov		[ebx+0], edx		// 1‰ñ–Ú‚Í‘‚«ž‚Ü‚È‚¢
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 0], ecx
			mov		[edi+ 3], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 6], ecx
			mov		[edi+ 9], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+12], ecx
			mov		[edi+15], ecx
			mov		[edi+18], edx
			mov		[edi+21], dx
			shr		edx, 16
			mov		[edi+23], dl
_r24bn_d_skip1:
			// check previous!!
			mov		edx, [eax+4]
			cmp		edx, [ebx+4]
			je		_r24bn_d_skip2
//			mov		[ebx+4], edx		// 1‰ñ–Ú‚Í‘‚«ž‚Ü‚È‚¢
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+24], ecx
			mov		[edi+27], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+30], ecx
			mov		[edi+33], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+36], ecx
			mov		[edi+39], ecx
			mov		[edi+42], edx
			mov		[edi+45], edx
			shr		edx, 16
			mov		[edi+47], dl
_r24bn_d_skip2:
			lea		eax, [eax+ 8]
			lea		ebx, [ebx+ 8]
			lea		edi, [edi+48]

			sub		width, 8
			jg		_r24bn_d_loop
			}

			width = SCREEN_WIDTH;
			pDst += pitch;

		__asm {
			mov		eax, pScn
			mov		ebx, pDlt
			mov		esi, pPal
			mov		edi, pDst
_r24bn_d_loop2:
			// check previous!!
			mov		edx, [eax+0]
			cmp		edx, [ebx+0]
			je		_r24bn_d_skip3
			mov		[ebx+0], edx		// 2‰ñ–Ú‚Í‘‚«ž‚Þ
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 0], ecx
			mov		[edi+ 3], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 6], ecx
			mov		[edi+ 9], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+12], ecx
			mov		[edi+15], ecx
			mov		[edi+18], edx
			mov		[edi+21], dx
			shr		edx, 16
			mov		[edi+23], dl
_r24bn_d_skip3:
			// check previous!!
			mov		edx, [eax+4]
			cmp		edx, [ebx+4]
			je		_r24bn_d_skip4
			mov		[ebx+4], edx		// 2‰ñ–Ú‚Í‘‚«ž‚Þ
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+24], ecx
			mov		[edi+27], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+30], ecx
			mov		[edi+33], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+36], ecx
			mov		[edi+39], ecx
			mov		[edi+42], edx
			mov		[edi+45], dx
			shr		edx, 16
			mov		[edi+47], dl
_r24bn_d_skip4:
			lea		eax, [eax+ 8]
			lea		ebx, [ebx+ 8]
			lea		edi, [edi+48]

			sub		width, 8
			jg		_r24bn_d_loop2
			}

			pScn += RENDER_WIDTH;
			pDlt += SCREEN_WIDTH;
			pDst += pitch;
		}
	}
}

//
// 24bit Double Scanline
//
void	CDirectDraw::Render24bpp_DoubleScanline( LPBYTE lpRdr, LPBYTE lpDlt, DDSURFACEDESC2& ddsd, BOOL bForceWrite )
{
	LPBYTE	pScn = lpRdr;
	LPBYTE	pDst = (LPBYTE)ddsd.lpSurface;
	LPBYTE	pDlt = (LPBYTE)lpDlt;
	LPBYTE	pPal, pPalScan;
	DWORD	width;
	DWORD	pitch = ddsd.lPitch;

	for( INT i = 0; i < SCREEN_HEIGHT; i++ ) {
		if( !(m_LineColormode[i]&0x80) ) {
			pPal     = (LPBYTE)m_cnPalette[m_LineColormode[i]&0x07];
			pPalScan = (LPBYTE)m_csPalette[m_LineColormode[i]&0x07];
		} else {
			pPal     = (LPBYTE)m_mnPalette[m_LineColormode[i]&0x07];
			pPalScan = (LPBYTE)m_msPalette[m_LineColormode[i]&0x07];
		}

		BOOL	bFWrite = FALSE;
		if( bForceWrite || *pDlt != m_LineColormode[i] ) {
			bFWrite = TRUE;
			*pDlt = m_LineColormode[i];
		}
		pDlt += 4;

		width = SCREEN_WIDTH;

		if( bFWrite ) {
		__asm {
			mov		eax, pScn
			mov		ebx, pDlt
			mov		esi, pPal
			mov		edi, pDst
_r24bs_d_loop_fw:
			mov		edx, [eax+0]
			mov		[ebx+0], edx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 0], ecx
			mov		[edi+ 3], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 6], ecx
			mov		[edi+ 9], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+12], ecx
			mov		[edi+15], ecx
			mov		[edi+18], edx
			mov		[edi+21], dx
			shr		edx, 16
			mov		[edi+23], dl

			mov		edx, [eax+4]
			mov		[ebx+4], edx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+24], ecx
			mov		[edi+27], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+30], ecx
			mov		[edi+33], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+36], ecx
			mov		[edi+39], ecx
			mov		[edi+42], edx
			mov		[edi+45], edx
			shr		edx, 16
			mov		[edi+47], dl

			lea		eax, [eax+ 8]
			lea		ebx, [ebx+ 8]
			lea		edi, [edi+48]

			sub		width, 8
			jg		_r24bs_d_loop_fw
			}

			width = SCREEN_WIDTH;
			pDst += pitch;

		__asm {
			mov		eax, pScn
			mov		ebx, pDlt
			mov		esi, pPalScan
			mov		edi, pDst
_r24bs_d_loop2_fw:
			mov		edx, [eax+0]
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 0], ecx
			mov		[edi+ 3], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 6], ecx
			mov		[edi+ 9], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+12], ecx
			mov		[edi+15], ecx
			mov		[edi+18], edx
			mov		[edi+21], dx
			shr		edx, 16
			mov		[edi+23], dl

			mov		edx, [eax+4]
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+24], ecx
			mov		[edi+27], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+30], ecx
			mov		[edi+33], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+36], ecx
			mov		[edi+39], ecx
			mov		[edi+42], edx
			mov		[edi+45], dx
			shr		edx, 16
			mov		[edi+47], dl

			lea		eax, [eax+ 8]
			lea		ebx, [ebx+ 8]
			lea		edi, [edi+48]

			sub		width, 8
			jg		_r24bs_d_loop2_fw
			}

			pScn += RENDER_WIDTH;
			pDlt += SCREEN_WIDTH;
			pDst += pitch;
		} else {
		__asm {
			mov		eax, pScn
			mov		ebx, pDlt
			mov		esi, pPal
			mov		edi, pDst
_r24bs_d_loop:
			// check previous!!
			mov		edx, [eax+0]
			cmp		edx, [ebx+0]
			je		_r24bs_d_skip1
//			mov		[ebx+0], edx		// 1‰ñ–Ú‚Í‘‚«ž‚Ü‚È‚¢
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 0], ecx
			mov		[edi+ 3], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 6], ecx
			mov		[edi+ 9], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+12], ecx
			mov		[edi+15], ecx
			mov		[edi+18], edx
			mov		[edi+21], dx
			shr		edx, 16
			mov		[edi+23], dl
_r24bs_d_skip1:
			// check previous!!
			mov		edx, [eax+4]
			cmp		edx, [ebx+4]
			je		_r24bs_d_skip2
//			mov		[ebx+4], edx		// 1‰ñ–Ú‚Í‘‚«ž‚Ü‚È‚¢
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+24], ecx
			mov		[edi+27], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+30], ecx
			mov		[edi+33], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+36], ecx
			mov		[edi+39], ecx
			mov		[edi+42], edx
			mov		[edi+45], edx
			shr		edx, 16
			mov		[edi+47], dl
_r24bs_d_skip2:
			lea		eax, [eax+ 8]
			lea		ebx, [ebx+ 8]
			lea		edi, [edi+48]

			sub		width, 8
			jg		_r24bs_d_loop
			}

			width = SCREEN_WIDTH;
			pDst += pitch;

		__asm {
			mov		eax, pScn
			mov		ebx, pDlt
			mov		esi, pPalScan
			mov		edi, pDst
_r24bs_d_loop2:
			// check previous!!
			mov		edx, [eax+0]
			cmp		edx, [ebx+0]
			je		_r24bs_d_skip3
			mov		[ebx+0], edx		// 2‰ñ–Ú‚Í‘‚«ž‚Þ
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 0], ecx
			mov		[edi+ 3], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+ 6], ecx
			mov		[edi+ 9], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+12], ecx
			mov		[edi+15], ecx
			mov		[edi+18], edx
			mov		[edi+21], dx
			shr		edx, 16
			mov		[edi+23], dl
_r24bs_d_skip3:
			// check previous!!
			mov		edx, [eax+4]
			cmp		edx, [ebx+4]
			je		_r24bs_d_skip4
			mov		[ebx+4], edx		// 2‰ñ–Ú‚Í‘‚«ž‚Þ
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+24], ecx
			mov		[edi+27], ecx
			movzx		ecx, dl
			mov		ecx, [esi+ecx*4]
			shr		edx, 8
			mov		[edi+30], ecx
			mov		[edi+33], ecx
			movzx		ecx, dl
			shr		edx, 8
			mov		ecx, [esi+ecx*4]
			mov		edx, [esi+edx*4]
			mov		[edi+36], ecx
			mov		[edi+39], ecx
			mov		[edi+42], edx
			mov		[edi+45], dx
			shr		edx, 16
			mov		[edi+47], dl
_r24bs_d_skip4:
			lea		eax, [eax+ 8]
			lea		ebx, [ebx+ 8]
			lea		edi, [edi+48]

			sub		width, 8
			jg		_r24bs_d_loop2
			}

			pScn += RENDER_WIDTH;
			pDlt += SCREEN_WIDTH;
			pDst += pitch;
		}
	}
}
