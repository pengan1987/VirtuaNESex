//////////////////////////////////////////////////////////////////////////
// Mapper007 AOROM/AMROM                                                //
//////////////////////////////////////////////////////////////////////////
void	Mapper007::Reset()
{
	patch = 0;

	SetPROM_32K_Bank( 0 );

	DWORD	crc = nes->rom->GetPROM_CRC();
	if( crc == 0x3c9fe649 ) {	// WWF Wrestlemania Challenge(U)
		SetVRAM_Mirror( VRAM_VMIRROR );
		patch = 1;
	}
	if( crc == 0x09874777 ) {	// Marble Madness(U)
		nes->SetRenderMethod( NES::TILE_RENDER );
	}
	if( crc == 0xfad97471 ) {	// ริบýณต(unif - [CC-21])
		patch = 2;
	}
}

void	Mapper007::Write( WORD addr, BYTE data )
{
	if( patch == 2 )
	{
		SetVROM_8K_Bank( addr & 0x1 );
		if( addr & 0x2 ) SetVRAM_Mirror( VRAM_MIRROR4H );
		else		 SetVRAM_Mirror( VRAM_MIRROR4L );
	}else{
		SetPROM_32K_Bank( (data & 0x07) );
		if( !patch ) {
			if( data & 0x10 ) SetVRAM_Mirror( VRAM_MIRROR4H );
			else		  SetVRAM_Mirror( VRAM_MIRROR4L );
		}
	}
}
