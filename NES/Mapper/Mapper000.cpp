//////////////////////////////////////////////////////////////////////////
// Mapper000                                                            //
//////////////////////////////////////////////////////////////////////////

#include "nnd.cpp"

void	Mapper000::Reset()
{
	switch( PROM_16K_SIZE ) {
		case	1:	// 16K only
			SetPROM_16K_Bank( 4, 0 );
			SetPROM_16K_Bank( 6, 0 );
			break;
		case	2:	// 32K
			SetPROM_32K_Bank( 0 );
			break;
	}

//------------------------------
//	main();
//------------------------------

	DWORD	crc = nes->rom->GetPROM_CRC();
	if( crc == 0x4e7db5af ) {	// Circus Charlie(J)
		nes->SetRenderMethod( NES::POST_RENDER );
	}
	if( crc == 0x57970078 ) {	// F-1 Race(J)
		nes->SetRenderMethod( NES::POST_RENDER );
	}
	if( crc == 0xaf2bbcbc		// Mach Rider(JU)
	 || crc == 0x3acd4bf1 ) {	// Mach Rider(Alt)(JU)
		nes->SetRenderMethod( NES::POST_RENDER );
	}

}
/*
BYTE	Mapper000::ReadLow( WORD addr )
{
	// $4100-$7FFF Lower Memory read
//	int data;
//	if( addr >= 0x4100 && addr <= 0x5FFF ){
//		data = ppread(addr);
//		main();
//		return data;
//	}
	return	Mapper::ReadLow( addr );
}

void	Mapper000::WriteLow( WORD addr, BYTE data )
{
	// $4100-$7FFF Lower Memory write
//	if( addr >= 0x4100 && addr <= 0x5FFF ){
//		ppwrite(addr, data);
//		main();
//	}
}
*/
void	Mapper000::Write( WORD addr, BYTE data )
{
	// $8000-$FFFF Memory write
//	ppwrite(addr, data);
//	main();

	if(nes->rom->GetPROM_CRC()==0x15612dc8){	// ÃÔ»ê³µ[»ÆÐÅÎ¬]
		SetVROM_4K_Bank( 0, addr&1 );
		SetVROM_4K_Bank( 4, addr&1 );
		if(addr&1)	SetVRAM_Mirror( VRAM_MIRROR4H );
		else		SetVRAM_Mirror( VRAM_MIRROR4L );
	}

	if(nes->rom->GetPROM_CRC()==0x366C20D7){
		SetCRAM_8K_Bank( data );
		if(data&1)	SetVRAM_Mirror( VRAM_MIRROR4H );
		else		SetVRAM_Mirror( VRAM_MIRROR4L );
	}
}