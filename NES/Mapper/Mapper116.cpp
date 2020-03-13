//////////////////////////////////////////////////////////////////////////
// Mapper116 CartSaint : —H—VAV‹­—ñ“`                                   //
//////////////////////////////////////////////////////////////////////////

void	Mapper116::Reset()
{
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
	if ((addr&0x4100)==0x4100) {
		mode = data;
		if (addr&1) {
			mmc1_regs[0] = 0xc;
			mmc1_regs[3] = 0;
			mmc1_buffer = 0;
			mmc1_shift = 0;
		}
		SetBank_CPU();
		SetBank_PPU();
		SetBank_MIR();
	}
	if(addr>=0x6000) CPU_MEM_BANK[addr>>13][addr&0x1FFF] = data;
}

void	Mapper116::Write( WORD addr, BYTE data )
{
DEBUGOUT( "MPRWR A=%04X D=%02X L=%3d CYC=%d\n", addr&0xFFFF, data&0xFF, nes->GetScanline(), nes->cpu->GetTotalCycles() );
	switch (mode & 3) {
	case 0:
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
	case 1:
		switch (addr & 0xE001) {
		case 0x8000:
			addr = mmc3_ctrl ^ data;
			mmc3_ctrl = data;
			if (addr & 0x40) SetBank_CPU();
			if (addr & 0x87) SetBank_PPU();
			break;
		case 0x8001:
			addr = mmc3_ctrl & 0x7;
			if (addr<2) data>>= 1;
			if (mmc3_regs[addr] != data){
				mmc3_regs[addr] = data;
				if (addr<6) SetBank_PPU();
				else		SetBank_CPU();
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
	case 2:
	case 3:
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

void	Mapper116::HSync( INT scanline )
{
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
		} else SetPROM_32K_Bank(bank>>1);
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
		SetVROM_2K_Bank(((0x0000^swap)>>10)+0,base>>1 | mmc3_regs[0]);
		SetVROM_2K_Bank(((0x0000^swap)>>10)+2,base>>1 | mmc3_regs[1]);
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
	//
}

void	Mapper116::LoadState( LPBYTE p )
{
	//
}
