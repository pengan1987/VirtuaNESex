//////////////////////////////////////////////////////////////////////////
// Mapper166                                                            //
//////////////////////////////////////////////////////////////////////////
void	Mapper166::Reset()
{
	for( INT i = 0; i < 8; i++ ) {
		reg[i] = i;
	}
	reg[8] = 0;

	irq_enable = 0;
	irq_counter = 0;
	irq_latch = 0;
	irq_clock = 0;

	SetPROM_32K_Bank( 0, 1, PROM_8K_SIZE-2, PROM_8K_SIZE-1 );
}

void	Mapper166::Write( WORD addr, BYTE data )
{
	DEBUGOUT( "WriteH A=%04X D=%02X L=%3d CYC=%d\n", addr&0xFFFF, data&0xFF, nes->GetScanline(), nes->cpu->GetTotalCycles() );
	switch( addr ) {
		case	0x8000:
			SetPROM_8K_Bank( 4, data );
			break;
		case	0xA000:
			SetPROM_8K_Bank( 5, data );
			break;

		case	0x9000:
			data &= 0x03;
			if( data == 0 )		 SetVRAM_Mirror( VRAM_VMIRROR );
			else if( data == 1 ) SetVRAM_Mirror( VRAM_HMIRROR );
			else if( data == 2 ) SetVRAM_Mirror( VRAM_MIRROR4L );
			else				 SetVRAM_Mirror( VRAM_MIRROR4H );
			break;

		case 0xB000:
			reg[0] = (reg[0] & 0xF0) | (data & 0x0F);
			SetVROM_1K_Bank( 0, reg[0] );
			break;
		case 0xB001:
			reg[0] = (reg[0] & 0x0F) | ((data & 0x0F)<< 4);
			SetVROM_1K_Bank( 0, reg[0] );
			break;
		case 0xB002:
			reg[1] = (reg[1] & 0xF0) | (data & 0x0F);
			SetVROM_1K_Bank( 1, reg[1] );
			break;
		case 0xB003:
			reg[1] = (reg[1] & 0x0F) | ((data & 0x0F)<< 4);
			SetVROM_1K_Bank( 1, reg[1] );
			break;
		case 0xC000:
			reg[2] = (reg[2] & 0xF0) | (data & 0x0F);
			SetVROM_1K_Bank( 2, reg[2] );
			break;
		case 0xC001:
			reg[2] = (reg[2] & 0x0F) | ((data & 0x0F)<< 4);
			SetVROM_1K_Bank( 2, reg[2] );
			break;
		case 0xC002:
			reg[3] = (reg[3] & 0xF0) | (data & 0x0F);
			SetVROM_1K_Bank( 3, reg[3] );
			break;
		case 0xC003:
			reg[3] = (reg[3] & 0x0F) | ((data & 0x0F)<< 4);
			SetVROM_1K_Bank( 3, reg[3] );
			break;
		case 0xD000:
			reg[4] = (reg[4] & 0xF0) | (data & 0x0F);
			SetVROM_1K_Bank( 4, reg[4] );
			break;
		case 0xD001:
			reg[4] = (reg[4] & 0x0F) | ((data & 0x0F)<< 4);
			SetVROM_1K_Bank( 4, reg[4] );
			break;
		case 0xD002:
			reg[5] = (reg[5] & 0xF0) | (data & 0x0F);
			SetVROM_1K_Bank( 5, reg[5] );
			break;
		case 0xD003:
			reg[5] = (reg[5] & 0x0F) | ((data & 0x0F) << 4);
			SetVROM_1K_Bank( 5, reg[5] );
			break;
		case 0xE000:
			reg[6] = (reg[6] & 0xF0) | (data & 0x0F);
			SetVROM_1K_Bank( 6, reg[6] );
			break;
		case 0xE001:
			reg[6] = (reg[6] & 0x0F) | ((data & 0x0F) << 4);
			SetVROM_1K_Bank( 6, reg[6] );
			break;
		case 0xE002:
			reg[7] = (reg[7] & 0xF0) | (data & 0x0F);
			SetVROM_1K_Bank( 7, reg[7] );
			break;
		case 0xE003:
			reg[7] = (reg[7] & 0x0F) | ((data & 0x0F)<< 4);
			SetVROM_1K_Bank( 7, reg[7] );
			break;

		case 0xF000:
			//
			break;
		case 0xF004:
			//
			break;

		case 0xF008:
			irq_latch = (irq_latch & 0xF0) | (data & 0x0F);

			break;

		case 0xF00C:
			irq_enable = data & 0x03;
			if( irq_enable & 0x02 ) {
				irq_counter = irq_latch;
				irq_clock = 0;
			}
			nes->cpu->ClrIRQ( IRQ_MAPPER );
			break;
	}
}
/*
void	Mapper166::HSync( INT scanline )
{
	if( (scanline >= 0 && scanline <= 239) ) {
		if( nes->ppu->IsDispON() ) {
			if( irq_enable && !irq_request ) {
				if( scanline == 0 ) {
					if( irq_counter ) {
						irq_counter -= 1;
					}
				}
				if(!(irq_counter)){
					irq_request = 0xFF;
					irq_counter = irq_latch;
					nes->cpu->SetIRQ( IRQ_MAPPER );
				}
				irq_counter--;
			}
		}
	}
}
*/
void	Mapper166::Clock( INT cycles )
{
	if( irq_enable & 0x02 ) {
		if( (irq_clock+=cycles) >= 0x72 ) {
			irq_clock -= 0x72;
			if( irq_counter == 0xFF ) {
				irq_counter = irq_latch;
				irq_enable = (irq_enable & 0x01) * 3;
				nes->cpu->SetIRQ( IRQ_MAPPER );
			} else {
				irq_counter++;
			}
		}
	}
}
