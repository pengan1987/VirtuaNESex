//////////////////////////////////////////////////////////////////////////
// Mapper142  SMB2j Pirate (KS 202)                                     //
//////////////////////////////////////////////////////////////////////////
void	Mapper142::Reset()
{
	irq_enable = 0;
	irq_counter = 0;

	SetPROM_8K_Bank( 4, 0 );
	SetPROM_8K_Bank( 5, 0 );
	SetPROM_8K_Bank( 6, 0 );
	SetPROM_8K_Bank( 7, 0 );

	if( VROM_1K_SIZE ) {
		SetVROM_8K_Bank( 0 );
	}
}

BYTE	Mapper142::ReadLow( WORD addr )
{
	return	CPU_MEM_BANK[addr>>13][addr&0x1FFF];
}

void	Mapper142::ExWrite( WORD addr, BYTE data )
{
	if( (addr&0xF0FF) == 0x4022 ) {
		switch( data&0x07 ) {
			case	0x00:
			case	0x02:
			case	0x03:
			case	0x04:
				SetPROM_8K_Bank( 6, 4 );
				break;
			case	0x01:
				SetPROM_8K_Bank( 6, 3 );
				break;
			case	0x05:
				SetPROM_8K_Bank( 6, 7 );
				break;
			case	0x06:
				SetPROM_8K_Bank( 6, 5 );
				break;
			case	0x07:
				SetPROM_8K_Bank( 6, 6 );
				break;
		}
	} 
}

void	Mapper142::WriteLow( WORD addr, BYTE data )
{
	ExWrite( addr, data );
}

void	Mapper142::Write( WORD addr, BYTE data )
{
	if( addr == 0x8122 ) {
		if( data&0x03 ) {
			irq_enable = 1;
		} else {
			irq_counter = 0;
			irq_enable = 0;
		}
		nes->cpu->ClrIRQ( IRQ_MAPPER );
	}
	CPU_MEM_BANK[addr>>13][addr&0x1FFF] = data;
}

void	Mapper142::HSync( INT scanline )
{
	if( irq_enable ) {
		irq_counter += 341;
		if( irq_counter >= 12288 ) {
			irq_counter = 0;
//			nes->cpu->IRQ_NotPending();
			nes->cpu->SetIRQ( IRQ_MAPPER );
		}
	}
}

void	Mapper142::SaveState( LPBYTE p )
{
	p[0] = irq_enable;
	*(INT*)&p[1] = irq_counter;
}

void	Mapper142::LoadState( LPBYTE p )
{
	irq_enable  = p[0];
	irq_counter = *(INT*)&p[1];
}
