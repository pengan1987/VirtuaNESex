//////////////////////////////////////////////////////////////////////////
// Mapper222                                                            //
//////////////////////////////////////////////////////////////////////////

void	Mapper222::Reset()
{
	SetPROM_32K_Bank( 0, 1, PROM_8K_SIZE-2, PROM_8K_SIZE-1 );
	SetVROM_8K_Bank( 0 );
	SetVRAM_Mirror( VRAM_VMIRROR );

	for(INT i=0;i<8;i++) reg[i] = i;

	irq_enable = 0;
	irq_counter = 0;

	VBankNO = 0;
}

BYTE	Mapper222::ReadLow( WORD addr )
{
	switch( addr & 0xF800 ) {
		case 0x4800: break;
		case 0x5000: return (BYTE)irq_counter&0x00FF;
		case 0x5800: return (BYTE)((irq_counter>>8)&0x7F);
		case 0x6000:
		case 0x6800:
		case 0x7000:
		case 0x7800: return Mapper::ReadLow( addr );
	}
	return (BYTE)(addr>>8);
}

void	Mapper222::WriteLow( WORD addr, BYTE data )
{
	switch( addr & 0xF800 ) {
		case 0x4800:
			//
			break;
		case 0x5000:
			irq_counter = (irq_counter & 0xFF00) | (WORD)data;
			nes->cpu->ClrIRQ( IRQ_MAPPER );
			break;
		case 0x5800:
			//
			break;
		case 0x6000:
		case 0x6800:
		case 0x7000:
		case 0x7800:
			Mapper::WriteLow( addr, data );
			break;
	}
}

void	Mapper222::Write( WORD addr, BYTE data )
{
	switch( addr & 0xF003 ) {
		case 0x8000:
			SetPROM_8K_Bank( 4, data );
			break;
		case 0x9000:
			if(VROM_1K_SIZE>128){
				if( data&0x40 )	SetVRAM_Mirror( VRAM_HMIRROR );
				else			SetVRAM_Mirror( VRAM_VMIRROR );
			}
			break;
		case 0xA000:
			SetPROM_8K_Bank( 5, data );
			break;
		case	0xB000:
			SetVROM_1K_Bank( 0, data );
			VBankNO = data;
			break;
		case	0xB002:
			SetVROM_1K_Bank( 1, data );
			break;
		case	0xC000:
			SetVROM_1K_Bank( 2, data );
			break;
		case	0xC002:
			SetVROM_1K_Bank( 3, data );
			break;
		case	0xD000:
			SetVROM_1K_Bank( 4, data );
			break;
		case	0xD002:
			SetVROM_1K_Bank( 5, data );
			break;
		case	0xE000:
			SetVROM_1K_Bank( 6, data );
			break;
		case	0xE002:
			SetVROM_1K_Bank( 7, data );
			break;
		case	0xF000:
			//
			break;
		case	0xF001:
			if(VBankNO==0x20){	//针对 Dragon Ninja (J) 第三关
				if(data) data = (data+0x12)>>1;
			}else{
				if(data) data = (data+0x10)>>1;
			}
			irq_counter = (irq_counter & 0x00FF) | ((WORD)(data & 0x7F) << 8);
 			irq_enable  = data;
			nes->cpu->ClrIRQ( IRQ_MAPPER );
			break;
		case	0xF002:
			//
			break;
	}
}

void	Mapper222::Clock( INT cycles )
{
	if( irq_enable ) {
		if( (irq_counter+=cycles) >= 0x7FFF ) {
			irq_enable  = 0;
			irq_counter = 0x7FFF;
			nes->cpu->SetIRQ( IRQ_MAPPER );
		}
	}
}

void	Mapper222::SaveState( LPBYTE p )
{
	//
}

void	Mapper222::LoadState( LPBYTE p )
{
	//
}
