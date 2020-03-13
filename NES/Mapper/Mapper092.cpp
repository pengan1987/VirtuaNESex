//////////////////////////////////////////////////////////////////////////
// Mapper092  Jaleco/Type1 Higher bank switch                           //
//////////////////////////////////////////////////////////////////////////
void	Mapper092::Reset()
{
	SetPROM_32K_Bank( 0, 1, PROM_8K_SIZE-2, PROM_8K_SIZE-1 );

	if( VROM_8K_SIZE ) {
		SetVROM_8K_Bank( 0 );
	}
}

void	Mapper092::Write( WORD addr, BYTE data )
{
	data = addr & 0xFF;

	if( addr >= 0x9000 ) {
		if( (data & 0xF0) == 0xD0 ) {
			SetPROM_16K_Bank( 6, data & 0x0F );
		} else if( (data & 0xF0) == 0xE0 ) {
			SetVROM_8K_Bank( data & 0x0F );
		}
	} else {
		if( (data & 0xF0) == 0xB0 ) {
			SetPROM_16K_Bank( 6, data & 0x0F );
		} else if( (data & 0xF0) == 0x70 ) {
			SetVROM_8K_Bank( data & 0x0F );
		}
	}
}
