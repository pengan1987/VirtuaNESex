//////////////////////////////////////////////////////////////////////////
// Mapper116 CartSaint : —H—VAV‹­—ñ“`                                   //
//////////////////////////////////////////////////////////////////////////

void	Mapper116::Reset()
{
/*
	for( INT i = 0; i < 8; i++ ) {
		reg[i] = 0x00;
	}

	prg0 = prg0L = 0;
	prg1 = prg1L = 1;
	prg2 = PROM_8K_SIZE-2;
	prg3 = PROM_8K_SIZE-1;

	ExPrgSwitch = 0;
	ExChrSwitch = 0;

	SetBank_CPU();

	if( VROM_1K_SIZE ) {
		chr0 = 0;
		chr1 = 1;
		chr2 = 2;
		chr3 = 3;
		chr4 = 4;
		chr5 = 5;
		chr6 = 6;
		chr7 = 7;
		SetBank_PPU();
	} else {
		chr0 = chr2 = chr4 = chr5 = chr6 = chr7 = 0;
		chr1 = chr3 = 1;
	}

	irq_enable = 0;	// Disable
	irq_counter = 0;
	irq_latch = 0;

//	nes->SetFrameIRQmode( FALSE );
*/
	mode = 0;
	vrc2_chr[0] = 0;
	vrc2_chr[1] = 1;
	vrc2_chr[2] = 2;
	vrc2_chr[3] = 3;
	vrc2_chr[4] = 4;
	vrc2_chr[5] = 5;
	vrc2_chr[6] = 6;
	vrc2_chr[7] = 7;
	vrc2_prg[0] = 0;
	vrc2_prg[1] = 1;
	vrc2_mirr = 0;
	mmc3_regs[0] = 0;
	mmc3_regs[1] = 2;
	mmc3_regs[2] = 4;
	mmc3_regs[3] = 5;
	mmc3_regs[4] = 6;
	mmc3_regs[5] = 7;
	mmc3_regs[6] = 0x3C;
	mmc3_regs[7] = 0x3D;
	mmc3_regs[8] = 0xFE;
	mmc3_regs[9] = 0xFF;
	mmc3_ctrl = mmc3_mirr = 0;
	IRQCount = IRQLatch = IRQa = 0;
	mmc1_regs[0] = 0xc;
	mmc1_regs[1] = 0;
	mmc1_regs[2] = 0;
	mmc1_regs[3] = 0;
	mmc1_buffer = 0;
	mmc1_shift = 0;
	SetBank_CPU();
	SetBank_PPU();
	SetBank_MIR();

}

void	Mapper116::WriteLow( WORD addr, BYTE data )
{
	DEBUGOUT( "MPRWR A=%04X D=%02X L=%3d CYC=%d\n", addr&0xFFFF, data&0xFF, nes->GetScanline(), nes->cpu->GetTotalCycles() );
/*
	if( (addr & 0x4100) == 0x4100 ) {
		ExChrSwitch = data;
		SetBank_PPU();
	}
*/
	if ((addr&0x4100)==0x4100) {
		mode = data;
		if (addr&1) {
			mmc1_regs[0] = 0xc;
			mmc1_regs[3] = 0;
			mmc1_buffer = 0;
			mmc1_shift = 0;
		}
//		if ((data&0x3)!=1)
//		{
//			IRQCount = IRQLatch;
//			IRQa = 0;
//			nes->cpu->ClrIRQ( IRQ_MAPPER );
//		}
		SetBank_CPU();
		SetBank_PPU();
		SetBank_MIR();
	}

}

void	Mapper116::Write( WORD addr, BYTE data )
{
	DEBUGOUT( "MPRWR A=%04X D=%02X L=%3d CYC=%d\n", addr&0xFFFF, data&0xFF, nes->GetScanline(), nes->cpu->GetTotalCycles() );
/*
	switch( addr & 0xE001 ) {
		case	0x8000:
			reg[0] = data;
			SetBank_CPU();
			SetBank_PPU();
			break;
		case	0x8001:
			reg[1] = data;
			switch( reg[0] & 0x07 ) {
				case	0x00:
					chr0 = data & 0xFE;
					chr1 = chr0+1;
					SetBank_PPU();
					break;
				case	0x01:
					chr2 = data & 0xFE;
					chr3 = chr2+1;
					SetBank_PPU();
					break;
				case	0x02:
					chr4 = data;
					SetBank_PPU();
					break;
				case	0x03:
					chr5 = data;
					SetBank_PPU();
					break;
				case	0x04:
					chr6 = data;
					SetBank_PPU();
					break;
				case	0x05:
					chr7 = data;
					SetBank_PPU();
					break;
				case	0x06:
					prg0 =  data;
					SetBank_CPU();
					break;
				case	0x07:
					prg1 =  data;
					SetBank_CPU();
					break;
			}
			break;
		case	0xA000:
			reg[2] = data;
			if( !nes->rom->Is4SCREEN() ) {
				if( data & 0x01 ) SetVRAM_Mirror( VRAM_HMIRROR );
				else		  SetVRAM_Mirror( VRAM_VMIRROR );
			}
			break;
		case	0xA001:
			reg[3] = data;
			break;
		case	0xC000:
			reg[4] = data;
			irq_counter = data;
//			irq_enable = 0xFF;
			break;
		case	0xC001:
			reg[5] = data;
			irq_latch = data;
			break;
		case	0xE000:
			reg[6] = data;
			irq_counter = irq_latch;
			irq_enable = 0;
			nes->cpu->ClrIRQ( IRQ_MAPPER );
			break;
		case	0xE001:
			reg[7] = data;
			irq_enable = 0xFF;
			break;
	}
*/
	switch (mode & 3) {
	case 0: {
		if ((addr >= 0xB000) && (addr <= 0xE003)) {
			int32 ind = ((((addr & 2) | (addr >> 10)) >> 1) + 2) & 7;
			int32 sar = ((addr & 1) << 2);
			vrc2_chr[ind] = (vrc2_chr[ind] & (0xF0 >> sar)) | ((data & 0x0F) << sar);
			SetBank_PPU();
		} else
			switch (addr & 0xF000) {
			case 0x8000: vrc2_prg[0] = data; SetBank_CPU(); break;
			case 0xA000: vrc2_prg[1] = data; SetBank_CPU(); break;
			case 0x9000: vrc2_mirr = data; SetBank_MIR(); break;
			}
		break;
	}
	case 1: {
		switch (addr & 0xE001) {
		case 0x8000:
//			uint8 old_ctrl = mmc3_ctrl;
//			mmc3_ctrl = data;
//			if ((old_ctrl & 0x40) != (mmc3_ctrl & 0x40))
//				SetBank_CPU();
//			if ((old_ctrl & 0x80) != (mmc3_ctrl & 0x80))
//				SetBank_PPU();

			addr = mmc3_ctrl ^ data;
			mmc3_ctrl = data;
			if (addr & 0x40)
				SetBank_CPU();
			if (addr & (0x80|0x07))
				SetBank_PPU();
			break;
		case 0x8001:
//			mmc3_regs[mmc3_ctrl & 7] = data;
//			if ((mmc3_ctrl & 7) < 6)
//				SetBank_PPU();
//			else
//				SetBank_CPU();

			addr = mmc3_ctrl & 0x7;
			if (addr<2) data>>= 1;
			if (mmc3_regs[addr] != data)
			{
				mmc3_regs[addr] = data;
				if (addr<6)
					SetBank_PPU();
				else
					SetBank_CPU();
			}
			break;
		case 0xA000:
			mmc3_mirr = data;
			SetBank_MIR();
			break;
		case 0xC000:
			IRQCount = data;
			break;
		case 0xC001:
			IRQLatch = data;
			break;
		case 0xE000:
			IRQCount = IRQLatch;
			IRQa = 0;
			nes->cpu->ClrIRQ( IRQ_MAPPER );
			break;
		case 0xE001:
			IRQa = 0xFF;
			break;
		}
		break;
	}
	case 2:
	case 3: {
		if (data & 0x80) {
			mmc1_regs[0] |= 0xc;
			mmc1_buffer = mmc1_shift = 0;
			SetBank_CPU();
		} else {
			uint8 n = (addr >> 13) - 4;
			mmc1_buffer |= (data & 1) << (mmc1_shift++);
			if (mmc1_shift == 5) {
				mmc1_regs[n] = mmc1_buffer;
				mmc1_buffer = mmc1_shift = 0;
				switch (n) {
				case 0: SetBank_MIR();
				case 2: SetBank_PPU();
				case 3:
				case 1: SetBank_CPU();
				}
			}
		}
		break;
	}
	}

}

void	Mapper116::HSync( INT scanline )
{
/*
	if( (scanline >= 0 && scanline <= 239) ) {
		if ((mode & 3) == 1) {
			int32 count = IRQCount;
			if (!count || IRQReload) {
				IRQCount = IRQLatch;
				IRQReload = 0;
			} else
				IRQCount--;
			if (!IRQCount) {
				if (IRQa)
					nes->cpu->SetIRQ( IRQ_MAPPER );
			}
		}
	}
*/
	if( (scanline >= 0 && scanline <= 239) ) {
		if( IRQCount <= 0 ) {
			if( IRQa ) {
					nes->cpu->SetIRQ( IRQ_MAPPER );
				return;
			}
		}

		if(nes->ppu->IsDispON() ) {
			IRQCount--;
		}
	}

}

/*
void	Mapper116::HSync( INT scanline )
{
	if( (scanline >= 0 && scanline <= 239) ) {
		if( irq_counter <= 0 ) {
			if( irq_enable ) {
//				nes->cpu->IRQ_NotPending();
				nes->cpu->SetIRQ( IRQ_MAPPER );

#if	0
DEBUGOUT( "IRQ L=%3d\n", scanline );
{
LPBYTE	lpScn = nes->ppu->GetScreenPtr();

	lpScn = lpScn+(256+16)*scanline;

	for( INT i = 0; i < 64; i++ ) {
		lpScn[i] = 22;
	}
}
#endif
				return;
			}
		}

		if( nes->ppu->IsDispON() ) {
			irq_counter--;
		}
	}

#if	0
	if( (scanline >= 0 && scanline <= 239) ) {
		if( nes->ppu->IsDispON() ) {
			if( irq_enable ) {
				if( !(irq_counter--) ) {
//					irq_counter = irq_latch;
					nes->cpu->IRQ_NotPending();
				}
			}
		}
	}
#endif
}

void	Mapper116::SetBank_CPU()
{
	if( reg[0] & 0x40 ) {
		SetPROM_32K_Bank( PROM_8K_SIZE-2, prg1, prg0, PROM_8K_SIZE-1 );
	} else {
		SetPROM_32K_Bank( prg0, prg1, PROM_8K_SIZE-2, PROM_8K_SIZE-1 );
	}
}

void	Mapper116::SetBank_PPU()
{
	if( VROM_1K_SIZE ) {
		if( ExChrSwitch & 0x04 ) {
			INT	chrbank = 256;
			SetVROM_8K_Bank( chrbank+chr4, chrbank+chr5,
					 chrbank+chr6, chrbank+chr7,
					 chr0, chr1,
					 chr2, chr3 );
		} else {
			INT	chrbank = 0;
			SetVROM_8K_Bank( chrbank+chr4, chrbank+chr5,
					 chrbank+chr6, chrbank+chr7,
					 chr0, chr1,
					 chr2, chr3 );
		}

#if	0
		if( reg[0] & 0x80 ) {
//			SetVROM_8K_Bank( chrbank+chr4, chrbank+chr5,
//					 chrbank+chr6, chrbank+chr7,
//					 chrbank+chr0, chrbank+chr1,
//					 chrbank+chr2, chrbank+chr3 );
			SetVROM_8K_Bank( chrbank+chr4, chrbank+chr5,
					 chrbank+chr6, chrbank+chr7,
					 chr0, chr1,
					 chr2, chr3 );
		} else {
			SetVROM_8K_Bank( chr0, chr1,
					 chr2, chr3,
					 chrbank+chr4, chrbank+chr5,
					 chrbank+chr6, chrbank+chr7 );
		}
#endif
	}
}
*/

void	Mapper116::SetBank_CPU()
{
	switch (mode & 3) {
	case 0:
		SetPROM_8K_Bank(4, vrc2_prg[0]);
		SetPROM_8K_Bank(5, vrc2_prg[1]);
		SetPROM_8K_Bank(6, 0x1E);
		SetPROM_8K_Bank(7, 0x1F);
		break;
	case 1:
	{
		uint32 swap = (mmc3_ctrl >> 5) & 2;
		SetPROM_8K_Bank(4, mmc3_regs[6 + swap]);
		SetPROM_8K_Bank(5, mmc3_regs[7]);
		SetPROM_8K_Bank(6, mmc3_regs[6 + (swap^2)]);
		SetPROM_8K_Bank(7, mmc3_regs[9]);
		break;
	}
	case 2:
	case 3:
	{
		uint8 bank = mmc1_regs[3] & 0xF;
		if (mmc1_regs[0] & 8) {
			if (mmc1_regs[0] & 4) {
				SetPROM_16K_Bank(4, bank);
				SetPROM_16K_Bank(6, 0x0F);
			} else {
				SetPROM_16K_Bank(4, 0);
				SetPROM_16K_Bank(6, bank);
			}
		} else
			SetPROM_32K_Bank(bank>>1);
		break;
	}
	}
}
void	Mapper116::SetBank_PPU()
{
	uint32 base = (mode & 4) << 6;
	switch (mode & 3) {
	case 0:
		SetVROM_1K_Bank(0, base | vrc2_chr[0]);
		SetVROM_1K_Bank(1, base | vrc2_chr[1]);
		SetVROM_1K_Bank(2, base | vrc2_chr[2]);
		SetVROM_1K_Bank(3, base | vrc2_chr[3]);
		SetVROM_1K_Bank(4, base | vrc2_chr[4]);
		SetVROM_1K_Bank(5, base | vrc2_chr[5]);
		SetVROM_1K_Bank(6, base | vrc2_chr[6]);
		SetVROM_1K_Bank(7, base | vrc2_chr[7]);
		break;
	case 1: {
		uint32 swap = (mmc3_ctrl & 0x80) << 5;
//		SetVROM_1K_Bank(((0x0000^swap)/0x400)+0, base | ((mmc3_regs[0]) & 0xFE));
//		SetVROM_1K_Bank(((0x0400^swap)/0x400)+1, base | (mmc3_regs[0] | 1));
//		SetVROM_1K_Bank(((0x0800^swap)/0x400)+2, base | ((mmc3_regs[1]) & 0xFE));
//		SetVROM_1K_Bank(((0x0c00^swap)/0x400)+3, base | (mmc3_regs[1] | 1));
//		SetVROM_1K_Bank(((0x1000^swap)/0x400)+4, base | mmc3_regs[2]);
//		SetVROM_1K_Bank(((0x1400^swap)/0x400)+5, base | mmc3_regs[3]);
//		SetVROM_1K_Bank(((0x1800^swap)/0x400)+6, base | mmc3_regs[4]);
//		SetVROM_1K_Bank(((0x1c00^swap)/0x400)+7, base | mmc3_regs[5]);
			SetVROM_2K_Bank(((0x0000^swap)>>10)+0,base >> 1 | mmc3_regs[0]);
			SetVROM_2K_Bank(((0x0000^swap)>>10)+2,base >> 1 | mmc3_regs[1]);
			SetVROM_1K_Bank(((0x1000^swap)>>10)+0,base|mmc3_regs[2]);
			SetVROM_1K_Bank(((0x1000^swap)>>10)+1,base|mmc3_regs[3]);
			SetVROM_1K_Bank(((0x1000^swap)>>10)+2,base|mmc3_regs[4]);
			SetVROM_1K_Bank(((0x1000^swap)>>10)+3,base|mmc3_regs[5]);
		break;
	}
	case 2:
	case 3:
		if (mmc1_regs[0] & 0x10) {
			SetVROM_4K_Bank(0, mmc1_regs[1]);
			SetVROM_4K_Bank(4, mmc1_regs[2]);
		} else
			SetVROM_8K_Bank(mmc1_regs[1] >> 1);
		break;
	}
}
void	Mapper116::SetBank_MIR()
{
	switch (mode & 3) {
	case 0: {
		if(vrc2_mirr&0x01)	SetVRAM_Mirror( VRAM_HMIRROR );
		else				SetVRAM_Mirror( VRAM_VMIRROR );
		break;
		break;
	}
	case 1: {
		if(mmc3_mirr&0x01)	SetVRAM_Mirror( VRAM_HMIRROR );
		else				SetVRAM_Mirror( VRAM_VMIRROR );
		break;
	}
	case 2:
	case 3: {
		switch (mmc1_regs[0] & 3) {
		case 0: SetVRAM_Mirror( VRAM_MIRROR4L ); break;
		case 1: SetVRAM_Mirror( VRAM_MIRROR4H ); break;
		case 2: SetVRAM_Mirror( VRAM_VMIRROR ); break;
		case 3: SetVRAM_Mirror( VRAM_HMIRROR ); break;
		}
		break;
	}
	}
}

void	Mapper116::SaveState( LPBYTE p )
{
	for( INT i = 0; i < 8; i++ ) {
		p[i] = reg[i];
	}
	p[ 8] = prg0;
	p[ 9] = prg1;
	p[10] = prg2;
	p[11] = prg3;
	p[12] = chr0;
	p[13] = chr1;
	p[14] = chr2;
	p[15] = chr3;
	p[16] = chr4;
	p[17] = chr5;
	p[18] = chr6;
	p[19] = chr7;
	p[20] = irq_enable;
	p[21] = irq_counter;
	p[22] = irq_latch;
	p[23] = ExPrgSwitch;
	p[24] = prg0L;
	p[25] = prg1L;
	p[26] = ExChrSwitch;
}

void	Mapper116::LoadState( LPBYTE p )
{
	for( INT i = 0; i < 8; i++ ) {
		reg[i] = p[i];
	}
	prg0  = p[ 8];
	prg1  = p[ 9];
	prg2  = p[10];
	prg3  = p[11];
	chr0  = p[12];
	chr1  = p[13];
	chr2  = p[14];
	chr3  = p[15];
	chr4  = p[16];
	chr5  = p[17];
	chr6  = p[18];
	chr7  = p[19];
	irq_enable  = p[20];
	irq_counter = p[21];
	irq_latch   = p[22];
	ExPrgSwitch = p[23];
	prg0L = p[24];
	prg1L = p[25];
	ExChrSwitch = p[26];
}
