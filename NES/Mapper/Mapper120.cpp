//////////////////////////////////////////////////////////////////////////
// Mapper120  Tobidase Daisakusen                                       //
//////////////////////////////////////////////////////////////////////////
void	Mapper120::Reset()
{
	SetPROM_8K_Bank( 3, 8 );
	SetPROM_32K_Bank( 0 );
//	SetPROM_32K_Bank( PROM_8K_SIZE-4, PROM_8K_SIZE-3, PROM_8K_SIZE-2, PROM_8K_SIZE-1 );
}

void	Mapper120::WriteLow( WORD addr, BYTE data )
{
	if( addr == 0x41FF ) {
		SetPROM_8K_Bank( 3, data + 8 );
	} else {
		Mapper::WriteLow( addr, data );
	}
}
