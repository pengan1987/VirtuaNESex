//////////////////////////////////////////////////////////////////////////
// Mapper074  Nintendo MMC3                                             //
//////////////////////////////////////////////////////////////////////////
void	Mapper074::Reset()
{
	if(nes->rom->GetPROM_CRC() == 0x227f8f9f){
		memcpy( YWRAM, PROM, 512*1024);
		DEBUGOUT("!!!=%10d\n", sizeof(YWRAM) );
		for( INT i = 0; i < (512*1024); i++ )
		{
			switch(YWRAM[i]&0x0f){
				case 0x01:YWRAM[i]=(YWRAM[i]&0xF0)|0x08; break;
				case 0x02:YWRAM[i]=(YWRAM[i]&0xF0)|0x04; break;
				case 0x03:YWRAM[i]=(YWRAM[i]&0xF0)|0x0c; break;
				case 0x04:YWRAM[i]=(YWRAM[i]&0xF0)|0x02; break;
				case 0x05:YWRAM[i]=(YWRAM[i]&0xF0)|0x0a; break;
				case 0x07:YWRAM[i]=(YWRAM[i]&0xF0)|0x0e; break;
				case 0x08:YWRAM[i]=(YWRAM[i]&0xF0)|0x01; break;
				case 0x0A:YWRAM[i]=(YWRAM[i]&0xF0)|0x05; break;
				case 0x0B:YWRAM[i]=(YWRAM[i]&0xF0)|0x0d; break;
				case 0x0C:YWRAM[i]=(YWRAM[i]&0xF0)|0x03; break;
				case 0x0D:YWRAM[i]=(YWRAM[i]&0xF0)|0x0b; break;
				case 0x0E:YWRAM[i]=(YWRAM[i]&0xF0)|0x07; break;
			}
		}
		memcpy( &PROM[0], &YWRAM[0], 512*1024);
	}
	
	nes->ppu->SetVromWrite(1);
	for( INT j = 0; j < 8; j++ ) reg[j] = 0x00;
	prg0 = 0;
	prg1 = 1;
	prg2 = PROM_8K_SIZE-2;
	prg3 = PROM_8K_SIZE-1;
	SetBank_CPU();
	chr01 = 0;
	chr1  = 1;
	chr23 = 2;
	chr3  = 3;
	chr4  = 4;
	chr5  = 5;
	chr6  = 6;
	chr7  = 7;
	SetBank_PPU();
	irq_enable = 0;
	irq_counter = 0;
	irq_latch = 0;
	irq_request = 0;
	we_sram  = 0;

	reg5000 = 0;
	reg5001 = 0;
	reg5002 = 0;
	reg5003 = 0;
	reg5010 = 0;
	reg5011 = 0;
	reg5012 = 0;
	reg5013 = 0;
	reg5FF3 = 0;
	reg6000 = 0;
	reg6010 = 0;
	reg6013 = 0;
	WF_sn_5000 = 1;
	WF_sn_5010 = 1;
	WF_sn_5013 = 1;
	RF_sn_6000 = 1;
	RF_sn_6010 = 1;
	RF_sn_6013 = 1;

//	nes->SetSAVERAM_SIZE( 32*1024 );

	for( j = 0; j < 0x2000; j++ ){
		WX_WRAM00[j] = 0;
		WX_WRAM01[j] = WRAM[j];
		WX_WRAM02[j] = 0;
		WX_WRAM03[j] = 0;
	}

	sp_rom = 0;
	DWORD	crc = nes->rom->GetPROM_CRC();

	if(crc == 0xeb08bfe9){	//[ES-1125] Gui Mei Zhan Ji (C)
//		sp_rom = 2;
//		for(j=0;j<0x2000;j++) WX_WRAM01[j]=WRAM[j];
	}

	if(crc == 0x65F30078	//[ES-1121] Guang Zhi Qi Shi (C)
	|| crc == 0xB76205B5){	//[ES-1136] Shang Gu Shen Dian (C)
//		sp_rom = 2;
		for(j=0;j<0x2000;j++) WX_WRAM01[j]=WRAM[0x1000+j];
	}

	if(crc == 0x84966C88){	//[KT-1005] Feng Shen Bang (C)
		sp_rom = 3;
	}
	if(crc == 0x830BCF70){	//[KT-1015] Chu Liu Xiang Xin Zhuan (C)
		sp_rom = 4;
		bank = 0;
		prg2 = (PROM_8K_SIZE>>1)-2;
		prg3 = (PROM_8K_SIZE>>1)-1;
		SetBank_CPU();
	}

}

BYTE	Mapper074::ReadLow( WORD addr )
{
	DEBUGOUT("ReadLow : Address=%04X\n", addr&0xFFFF );

	if( addr >= 0x5000 && addr <= 0x5FFF ) {
		return	XRAM[addr-0x4000];
	}else{
		if( addr>=0x6000 ) {

//			if( sp_rom == 2 ){

				switch( we_sram&0xE3 ) {
					case 0xe0:
						return WX_WRAM00[addr&0x1FFF];
						break;
					case 0xe1:
						return WX_WRAM01[addr&0x1FFF];
						break;
					case 0xe2:
						return WX_WRAM02[addr&0x1FFF];
						break;
					case 0xe3:
						return WX_WRAM03[addr&0x1FFF];
						break;
					default:
						return CPU_MEM_BANK[addr>>13][addr&0x1FFF];
						break;
				}

//			}

			if((addr==0x6000)&&(RF_sn_6000)){
				RF_sn_6000 = 0;
				return reg6000;
			}
			if((addr==0x6010)&&(RF_sn_6010)){
				RF_sn_6010 = 0;
				return reg6010;
			}
			if((addr==0x6013)&&(RF_sn_6013)){
				RF_sn_6013 = 0;
				return reg6013;
			}

			return CPU_MEM_BANK[addr>>13][addr&0x1FFF];
		}	
	}
	return	Mapper::ReadLow( addr );
}

void	Mapper074::WriteLow( WORD addr, BYTE data )
{
	DEBUGOUT("WriteLow : Address=%04X Data=%02X\n", addr&0xFFFF, data&0xFF );

	if ((addr&0x4100)==0x4100) {
//		if(data&2){
//			SetCRAM_8K_Bank(0);
//		}else
//			SetVROM_1K_Bank(addr>>10,data);
//		data &= 0x03;
//		if		( data == 0 )	SetCRAM_8K_Bank( 2 );
//		else if ( data == 1 )	SetCRAM_8K_Bank( 3 );
//		else if ( data == 2 )	SetCRAM_8K_Bank( 0 );
//		else					SetCRAM_8K_Bank( 1 );
	}

	if( addr >= 0x5000 && addr <= 0x5FFF ) {
		XRAM[addr-0x4000] = data;

		switch( addr ) {
			case	0x5000:
				reg5000 = data;
				if (WF_sn_5000){
					reg6000 = reg5000;
					WF_sn_5000 = 0;
				}
				if ( sp_rom == 4 ) {	//[KT-1015] Chu Liu Xiang Xin Zhuan (C)
					bank = ((reg5000<<4)|(reg5003&0xf))>>2;
					SetPROM_32K_Bank( bank );
				}
				break;
			case	0x5001:
				reg5001 = data;
				break;
			case	0x5002:
				reg5002 = data;
				break;
			case	0x5003:
				reg5003 = data;
				break;
			case	0x5010:
				reg5010 = data;
				if (WF_sn_5010){
					reg6010 = reg5010;
					WF_sn_5010 = 0;
				}
				break;
			case	0x5011:
				reg5011 = data;
				break;
			case	0x5012:
				reg5012 = data;
				break;
			case	0x5013:
				reg5013 = data;
				if (WF_sn_5013){
					reg6013 = reg5013;
					WF_sn_5013 = 0;
				}
				break;
			case	0x5FF3:
				reg5FF3 = data;
				if( reg5FF3 == 2 ) {
					SetPROM_32K_Bank( 0, 0, 0, 0 );
				}
				break;

			default:
				//
				break;
		}

	} else {
		if( addr >= 0x6000 ) {

//			if( sp_rom == 2 ){

				switch( we_sram&0xE3 ) {
					case 0xe0:	//CPU_MEM_BANK
						WX_WRAM00[addr&0x1FFF] = data;
						CPU_MEM_BANK[addr>>13][addr&0x1FFF] = data;
						break;
					case 0xe1:	//SRAM
						WX_WRAM01[addr&0x1FFF] = data;
						break;
					case 0xe2:
						WX_WRAM02[addr&0x1FFF] = data;
						break;
					case 0xe3:
						WX_WRAM03[addr&0x1FFF] = data;
						break;
					default:
						CPU_MEM_BANK[addr>>13][addr&0x1FFF] = data;
						break;
				}

//				return;

			}

//			CPU_MEM_BANK[addr>>13][addr&0x1FFF] = data;
//		}
	}
}

void	Mapper074::Write( WORD addr, BYTE data )
{
	DEBUGOUT("Write : Address=%04X Data=%02X\n", addr&0xFFFF, data&0xFF );

	switch( addr & 0xE001 ) {
		case	0x8000:
			reg[0] = data;
			SetBank_CPU();
			SetBank_PPU();
			break;
		case	0x8001:
			reg[1] = data;
			switch( reg[0] & 0x0f ) {
				case	0x00:
					chr01 = data;
					chr1 = chr01+1;
					SetBank_PPU();
					break;
				case	0x01:
					chr23 = data;
					chr3 = chr23+1;
					SetBank_PPU();
					break;
				case	0x02:
					chr4 = data;
					SetBank_PPU();
					break;
				case	0x03:
					chr5 = data;
					SetBank_PPU();
					break;
				case	0x04:
					chr6 = data;
					SetBank_PPU();
					break;
				case	0x05:
					chr7 = data;
					SetBank_PPU();
					break;
				case	0x06:
					prg0 = data;
					SetBank_CPU();
					break;
				case	0x07:
					prg1 = data;
					SetBank_CPU();
					break;
				case	0x08:
					prg2 = data;
					SetBank_CPU();
					break;
				case	0x09:
					prg3 = data;
					SetBank_CPU();
					break;
				case	0x0a:
					chr1 = data;
					SetBank_PPU();
					break;
				case	0x0b:
					chr3 = data;
					SetBank_PPU();
					break;
			}
			break;
		case	0xA000:
			DEBUGOUT("Write : Address=%04X Data=%02X\n", addr&0xFFFF, data&0xFF );
			reg[2] = data;
			data &= 0x03;
			if		( data == 0 )	SetVRAM_Mirror( VRAM_VMIRROR );
			else if ( data == 1 )	SetVRAM_Mirror( VRAM_HMIRROR );
			else if ( data == 2 )	SetVRAM_Mirror( VRAM_MIRROR4L );
			else					SetVRAM_Mirror( VRAM_MIRROR4H );
			break;
		case	0xA001:
			reg[3] = data;
			we_sram = reg[3];
			break;
		case	0xC000:
			reg[4] = data;
			irq_counter = data;
			irq_request = 0;
			break;
		case	0xC001:
			reg[5] = data;
			irq_latch = data;
			irq_request = 0;
			break;
		case	0xE000:
			reg[6] = data;
			irq_enable = 0;
			irq_request = 0;
			nes->cpu->ClrIRQ( IRQ_MAPPER );
			break;
		case	0xE001:
			reg[7] = data;
			irq_enable = 1;
			irq_request = 0;
			break;
	}	
	
}

void	Mapper074::HSync( INT scanline )
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

void	Mapper074::SetBank_CPU()
{
	if( reg[0] & 0x40 ) {
		SetPROM_32K_Bank( prg2, prg1, prg0, prg3 );
	} else {
		SetPROM_32K_Bank( prg0, prg1, prg2, prg3 );
	}
}

void	Mapper074::SetBank_PPU()
{
	if( VROM_1K_SIZE ) {
		if( reg[0] & 0x80 ) {
			SetVROM_1K_Bank( 4, chr01);
			SetVROM_1K_Bank( 5, chr1 );
			SetVROM_1K_Bank( 6, chr23);
			SetVROM_1K_Bank( 7, chr3 );
			SetVROM_1K_Bank( 0, chr4 );
			SetVROM_1K_Bank( 1, chr5 );
			SetVROM_1K_Bank( 2, chr6 );
			SetVROM_1K_Bank( 3, chr7 );
		} else {
			SetVROM_1K_Bank( 0, chr01);
			SetVROM_1K_Bank( 1, chr1 );
			SetVROM_1K_Bank( 2, chr23);
			SetVROM_1K_Bank( 3, chr3 );
			SetVROM_1K_Bank( 4, chr4 );
			SetVROM_1K_Bank( 5, chr5 );
			SetVROM_1K_Bank( 6, chr6 );
			SetVROM_1K_Bank( 7, chr7 );
			if ( sp_rom == 3 ) {	//[KT-1005] Feng Shen Bang (C)
				SetVROM_2K_Bank( 0, chr01);
				SetVROM_2K_Bank( 2, chr23);
				SetVROM_2K_Bank( 4, chr4 );
				SetVROM_2K_Bank( 6, chr6 );
			}
		}
	} else {
		if( reg[0] & 0x80 ) {
			SetCRAM_1K_Bank( 4, (chr01+0)&0x07 );
			SetCRAM_1K_Bank( 5, (chr01+1)&0x07 );
			SetCRAM_1K_Bank( 6, (chr23+0)&0x07 );
			SetCRAM_1K_Bank( 7, (chr23+1)&0x07 );
			SetCRAM_1K_Bank( 0, chr4&0x07 );
			SetCRAM_1K_Bank( 1, chr5&0x07 );
			SetCRAM_1K_Bank( 2, chr6&0x07 );
			SetCRAM_1K_Bank( 3, chr7&0x07 );
		} else {
			SetCRAM_1K_Bank( 0, (chr01+0)&0x07 );
			SetCRAM_1K_Bank( 1, (chr01+1)&0x07 );
			SetCRAM_1K_Bank( 2, (chr23+0)&0x07 );
			SetCRAM_1K_Bank( 3, (chr23+1)&0x07 );
			SetCRAM_1K_Bank( 4, chr4&0x07 );
			SetCRAM_1K_Bank( 5, chr5&0x07 );
			SetCRAM_1K_Bank( 6, chr6&0x07 );
			SetCRAM_1K_Bank( 7, chr7&0x07 );
		}
	}
}

void	Mapper074::SaveState( LPBYTE p )
{
	for( INT i = 0; i < 8; i++ ) {
		p[i] = reg[i];
	}
	p[ 8] = prg0;
	p[ 9] = prg1;
	p[10] = chr01;
	p[11] = chr23;
	p[12] = chr4;
	p[13] = chr5;
	p[14] = chr6;
	p[15] = chr7;
	p[16] = irq_enable;
	p[17] = irq_counter;
	p[18] = irq_latch;
	p[19] = irq_request;
	p[20] = prg2;
	p[21] = prg3;
	p[22] = chr1;
	p[23] = chr3;
}

void	Mapper074::LoadState( LPBYTE p )
{
	for( INT i = 0; i < 8; i++ ) {
		reg[i] = p[i];
	}
	prg0  = p[ 8];
	prg1  = p[ 9];
	prg2  = p[20];
	prg3  = p[21];
	chr01 = p[10];
	chr1  = p[22];
	chr23 = p[11];
	chr3  = p[23];
	chr4  = p[12];
	chr5  = p[13];
	chr6  = p[14];
	chr7  = p[15];
	irq_enable  = p[16];
	irq_counter = p[17];
	irq_latch   = p[18];
	irq_request = p[19];
}

void	Mapper074::PPU_Latch( WORD addr )
{
	if(DirectInput.m_Sw[DIK_PAUSE]){
		nes->Dump_YWRAM();
	}
}
