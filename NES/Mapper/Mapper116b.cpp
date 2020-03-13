//////////////////////////////////////////////////////////////////////////
// Mapper116 CartSaint : —H—VAV‹­—ñ“`                                   //
//////////////////////////////////////////////////////////////////////////

void	Mapper116::Reset()
{
	int i;
	mode = 0;

	vrc2_prg[0] = 0x0;
	vrc2_prg[1] = 0x1;
	vrc2_nmt = 0;

	
	for (i=0; i < 8; ++i)
		vrc2_chr[i] = i;

	mmc3_ctrl = 0;
	mmc3_nmt = 0;

	mmc3_banks[0] = 0x0;
	mmc3_banks[1] = 0x1;
	mmc3_banks[2] = 0x4;
	mmc3_banks[3] = 0x5;
	mmc3_banks[4] = 0x6;
	mmc3_banks[5] = 0x7;

	mmc3_banks[6] = 0x3C;
	mmc3_banks[7] = 0x3D;
	mmc3_banks[8] = 0xFE;
	mmc3_banks[9] = 0xFF;

	mmc1_buffer = 0;
	mmc1_shifter = 0;

	mmc1_regs[0] = 0x4U|0x8U;
	mmc1_regs[1] = 0;
	mmc1_regs[2] = 0;
	mmc1_regs[3] = 0;

	irq_enable = 0;	// Disable
	irq_counter = 0;
	irq_latch = 0;
	
	NES_mapper116_MMC3_set_CPU_banks();
	NES_mapper116_MMC3_set_Nmt();
	NES_mapper116_MMC3_set_PPU_banks();
}


void	Mapper116::NES_mapper116_MMC3_set_CPU_banks()
{
	switch (mode & 0x3)
	{
		case 0x0:
			SetPROM_32K_Bank(vrc2_prg[0], 	vrc2_prg[1], 0x1E, 0x1F);
			break;

		case 0x1:
		{
			const uint32 i = mmc3_ctrl >> 5 & 0x2U;
			SetPROM_32K_Bank(mmc3_banks[6+i],mmc3_banks[6+1],mmc3_banks[6+(i^2)],mmc3_banks[6+3] );		
			break;
		}

		case 0x2:
		{
			const uint32 bank = mmc1_regs[3] & 0xFU;
			if (mmc1_regs[0] & 0x8U)
			{
				SetPROM_16K_Bank(4,(mmc1_regs[0] & 0x4U) ? bank : 0x0);
				SetPROM_16K_Bank(6,(mmc1_regs[0] & 0x4U) ? 0xF : bank );				
			}
			else{
				SetPROM_32K_Bank( bank >> 1 );
			}
			break;
		}
	}
}


void	Mapper116::NES_mapper116_MMC3_set_PPU_banks()
{	
	const uint32 base = (mode & 0x4) << 6;

	switch (mode & 0x3)
	{
		case 0x0:
			SetVROM_8K_Bank(
				base|vrc2_chr[0],
				base|vrc2_chr[1],
				base|vrc2_chr[2],
				base|vrc2_chr[3],
				base|vrc2_chr[4],
				base|vrc2_chr[5],
				base|vrc2_chr[6],
				base|vrc2_chr[7] );			
			break;

		case 0x1:
		{
			const uint32 swap = (mmc3_ctrl & 0x80U) << 5;
			SetVROM_2K_Bank( (0x0000 ^ swap)/0x400+0,base >> 1 | mmc3_banks[0]);
			SetVROM_2K_Bank( (0x0000 ^ swap)/0x400+2,base >> 1 | mmc3_banks[1]);
			SetVROM_1K_Bank((0x1000 ^ swap)/0x400+0,base|mmc3_banks[2]);
			SetVROM_1K_Bank((0x1000 ^ swap)/0x400+1,base|mmc3_banks[3]);
			SetVROM_1K_Bank((0x1000 ^ swap)/0x400+2,base|mmc3_banks[4]);
			SetVROM_1K_Bank((0x1000 ^ swap)/0x400+3,base|mmc3_banks[5]);
			break;
		}

		case 0x2:
			SetVROM_4K_Bank(0,(mmc1_regs[0] & 0x10U) ? mmc1_regs[1] : mmc1_regs[1] & 0x1EU );
			SetVROM_4K_Bank(4,(mmc1_regs[0] & 0x10U) ? mmc1_regs[2] : mmc1_regs[1] | 0x01U );
			break;
	}	
}


void	Mapper116::NES_mapper116_MMC3_set_Nmt()
{
	uint8 nmtCtrl = 0XFF;
	switch (mode & 0x3)
	{
		case 0x0:

			nmtCtrl = (vrc2_nmt & 0x1U) ? VRAM_HMIRROR   : VRAM_VMIRROR   ;
			break;

		case 0x1:

			nmtCtrl = (mmc3_nmt & 0x1U) ? VRAM_HMIRROR   : VRAM_VMIRROR   ;
			break;

		case 0x2:

			switch (mmc1_regs[0] & 0x3U)
			{
				case 0x0: SetVRAM_Mirror(VRAM_MIRROR4L);       break;
				case 0x1: SetVRAM_Mirror(VRAM_MIRROR4H);        break;
				case 0x2: nmtCtrl =VRAM_VMIRROR;   break;
				default:  nmtCtrl = VRAM_HMIRROR ; break;
			}
			break;

		default: return;
	}
	if(nmtCtrl!=0xff)
	SetVRAM_Mirror(nmtCtrl);
}


void	Mapper116::WriteLow( WORD addr, BYTE data )
{
		
	if( (addr & 0x4100) == 0x4100 ) 
	{
		if (mode != data)
		{
			mode = data;

			if ((data & 0x3) != 1)
			{
				//nes6502_ClrIRQ( IRQ_MAPPER );//irq.unit.Disable( cpu );
				irq_counter = irq_latch;
				irq_enable = 0;
				nes->cpu->ClrIRQ( IRQ_MAPPER );
			}

			NES_mapper116_MMC3_set_CPU_banks();
			NES_mapper116_MMC3_set_Nmt();
			NES_mapper116_MMC3_set_PPU_banks();
		}
	}
}




void Mapper116::Mapper116_Poke_Vrc2_8000(uint32 address,uint32 data)
{
	//NST_ASSERT( (mode & 0x3) == 0 );

	data &= 0x1F;
	address = address >> 13 & 0x1;

	if (vrc2_prg[address] != data)
	{
		vrc2_prg[address] = data;
		NES_mapper116_MMC3_set_CPU_banks();
	}
}

void Mapper116::Mapper116_Poke_Vrc2_9000(uint32 address,uint32 data)
{
	//NST_ASSERT( (mode & 0x3) == 0 );

	data &= 0x1;

	if (vrc2_nmt != data)
	{
		vrc2_nmt = data;
		NES_mapper116_MMC3_set_Nmt();
	}
}

void Mapper116::Mapper116_Poke_Vrc2_B000(uint32 address,uint32 data)
{
	//NST_ASSERT( (mode & 0x3) == 0 );

	data = (data & 0xF) << (address << 1 & 0x4);
	address = ((address - 0xB000) >> 11 & 0x6) | (address & 0x1);

	if (vrc2_chr[address] != data)
	{
		vrc2_chr[address] = data;
		;//ppu.Update();
		NES_mapper116_MMC3_set_PPU_banks();
	}
}

void Mapper116::Mapper116_Poke_Mmc3_8000(uint32 address,uint32 data)
{
	//NST_ASSERT( (mode & 0x3) == 1 );

	if (address & 0x1)
	{
		address = mmc3_ctrl & 0x7U;

		if (address < 2)
			data >>= 1;

		if (mmc3_banks[address] != data)
		{
			mmc3_banks[address] = data;

			if (address < 6)
			{
				;//ppu.Update();
				NES_mapper116_MMC3_set_PPU_banks();
			}
			else
			{
				NES_mapper116_MMC3_set_CPU_banks();
			}
		}
	}
	else
	{
		address = mmc3_ctrl ^ data;
		mmc3_ctrl = data;

		if (address & 0x40)
			NES_mapper116_MMC3_set_CPU_banks();

		if (address & (0x80U|0x07U))
		{
			//ppu.Update();
			NES_mapper116_MMC3_set_PPU_banks();
		}
	}
}

void Mapper116::Mapper116_Poke_Mmc3_A000(uint32 address,uint32 data)
{
	//NST_ASSERT( (mode & 0x3) == 1 );

	if (!(address & 0x1))
	{
		if (mmc3_nmt != data)
		{
			mmc3_nmt = data;
			NES_mapper116_MMC3_set_Nmt();
		}
	}
}

void Mapper116::Mapper116_Poke_Mmc3_C000(uint32 address,uint32 data)
{
	//NST_ASSERT( (mode & 0x3) == 1 );

	;//irq.Update();

	if (address & 0x1)
		irq_latch = data;//irq.unit.Reload();
	else
		irq_counter = data;//irq.unit.SetLatch( data );
}

void Mapper116::Mapper116_Poke_Mmc3_E000(uint32 address,uint32 data)
{
	//NST_ASSERT( (mode & 0x3) == 1 );

	;//irq.Update();

	if (address & 0x1)
		irq_enable = 0xFF;//Enable
	else
	{
		irq_counter = irq_latch;
		irq_enable = 0;
		nes->cpu->ClrIRQ( IRQ_MAPPER );
	}
}

void Mapper116::Mapper116_Poke_Mmc1_8000(uint32 address,uint32 data)
{
	//NST_ASSERT( (mode & 0x3) == 2 );

	if (!(data & 0x80))
	{
		mmc1_buffer |= 
		(data & 0x1) << mmc1_shifter++;

		if (mmc1_shifter != 5)
			return;

		mmc1_shifter = 0;
		data = mmc1_buffer;
		mmc1_buffer = 0;

		address = address >> 13 & 0x3;

		if (mmc1_regs[address] != data)
		{
			mmc1_regs[address] = data;

			NES_mapper116_MMC3_set_CPU_banks();
			NES_mapper116_MMC3_set_Nmt();
			NES_mapper116_MMC3_set_PPU_banks();
		}
	}
	else
	{
		mmc1_buffer = 0;
		mmc1_shifter = 0;

		if ((mmc1_regs[0] & (0x4U|0x8U)) != (0x4U|0x8U))
		{
			mmc1_regs[0] |= (0x4U|0x8U);

			NES_mapper116_MMC3_set_CPU_banks();
			NES_mapper116_MMC3_set_Nmt();
			NES_mapper116_MMC3_set_PPU_banks();
		}
	}
}

void	Mapper116::Write( WORD address, BYTE data )
{
	switch( address & 0xF000 ) {
		case	0x8000:
			switch (mode & 0x3)
			{
				case 0x0: Mapper116_Poke_Vrc2_8000( address, data ); break;
				case 0x1: Mapper116_Poke_Mmc3_8000( address, data ); break;
				case 0x2: Mapper116_Poke_Mmc1_8000( address, data ); break;
			}
			break;	
			
		case	0x9000:
			switch (mode & 0x3)
			{
				case 0x0: Mapper116_Poke_Vrc2_9000( address, data ); break;
				case 0x1: Mapper116_Poke_Mmc3_8000( address, data ); break;
				case 0x2: Mapper116_Poke_Mmc1_8000( address, data ); break;
			}
			break;
			
		case	0xA000:
			switch (mode & 0x3)
			{
				case 0x0: Mapper116_Poke_Vrc2_8000( address, data ); break;
				case 0x1: Mapper116_Poke_Mmc3_A000( address, data ); break;
				case 0x2: Mapper116_Poke_Mmc1_8000( address, data ); break;
			}
			break;
		case	0xB000:
			switch (mode & 0x3)
			{
				case 0x0: Mapper116_Poke_Vrc2_B000( address, data ); break;
				case 0x1: Mapper116_Poke_Mmc3_A000( address, data ); break;
				case 0x2: Mapper116_Poke_Mmc1_8000( address, data ); break;
			}
			break;
		case	0xC000:
			switch (mode & 0x3)
			{
				case 0x0: Mapper116_Poke_Vrc2_B000( address, data ); break;
				case 0x1: Mapper116_Poke_Mmc3_C000( address, data ); break;
				case 0x2: Mapper116_Poke_Mmc1_8000( address, data ); break;
			}
			break;
		case	0xD000:
			switch (mode & 0x3)
			{
				case 0x0: Mapper116_Poke_Vrc2_B000( address, data ); break;
				case 0x1: Mapper116_Poke_Mmc3_C000( address, data ); break;
				case 0x2: Mapper116_Poke_Mmc1_8000( address, data ); break;
			}
			break;
		case	0xE000:
			switch (mode & 0x3)
			{
				case 0x0: Mapper116_Poke_Vrc2_B000( address, data ); break;
				case 0x1: Mapper116_Poke_Mmc3_E000( address, data ); break;
				case 0x2: Mapper116_Poke_Mmc1_8000( address, data ); break;
			}
			break;
		case	0xF000:
			switch (mode & 0x3)
			{
				case 0x0: break;
				case 0x1: Mapper116_Poke_Mmc3_E000( address, data ); break;
				case 0x2: Mapper116_Poke_Mmc1_8000( address, data ); break;
			}
			break;
	}
}


void	Mapper116::HSync( INT scanline )
{	
	if( (scanline >= 0 && scanline <= 239) ) {
		if( irq_counter <= 0 ) {
			if( irq_enable ) {
					nes->cpu->SetIRQ( IRQ_MAPPER );
				return;
			}
		}

		if(nes->ppu->IsDispON() ) {
			irq_counter--;
		}
	}
}

void	Mapper116::SaveState( LPBYTE p )
{
}

void	Mapper116::LoadState( LPBYTE p )
{
}
