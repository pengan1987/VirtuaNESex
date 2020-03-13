//////////////////////////////////////////////////////////////////////////
// Mapper175  15-in-1 (Kaiser)                                          //
//////////////////////////////////////////////////////////////////////////
void	Mapper175::Reset()
{
	SetPROM_16K_Bank( 4, 0 );
	SetPROM_16K_Bank( 6, 0 );
	reg_dat = 0;

	if( VROM_1K_SIZE ) {
		SetVROM_8K_Bank( 0 );
	}
}

BYTE	Mapper175::Read( WORD addr)
{
	if( addr == 0xFFFC ) {
		SetPROM_16K_Bank( 4, reg_dat & 0x0F );
		SetPROM_8K_Bank( 6, (reg_dat & 0x0F)*2 );
	}
	return CPU_MEM_BANK[addr>>13][addr&0x1FFF];
}

void	Mapper175::Write( WORD addr, BYTE data )
{
	switch( addr ) {
		case	0x8000:
			if( data & 0x04 ) {
				SetVRAM_Mirror( VRAM_HMIRROR );
			} else {
				SetVRAM_Mirror( VRAM_VMIRROR );
			}
			break;
		case	0xA000:
			reg_dat = data;
			SetPROM_8K_Bank( 7, (reg_dat & 0x0F)*2+1 );
			SetVROM_8K_Bank( reg_dat & 0x0F );
			break;
	}
}
