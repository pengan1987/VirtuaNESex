//////////////////////////////////////////////////////////////////////////
// YuXing_Mouse                                                         //
//////////////////////////////////////////////////////////////////////////
void	EXPAD_YuXing_Mouse::Reset()
{
	mouse_data = 0x00;
	mouse_bits = 0;
	mouse_count = 0;
	mouse_button = 0;
	mouse_x = 0;
	mouse_y = 0;
	mouse_x_old = 0;
	mouse_y_old = 0;
	mouse_x_move = 0;
	mouse_y_move = 0;
	temp1 = 3;
	reg[0] = 0;
	reg[1] = 0;
	reg[2] = 0;
	reg[3] = 0;
	reg[4] = 0;
	reg[5] = 0;
}

void	EXPAD_YuXing_Mouse::Strobe()
{
	mouse_data = 0x00;
	mouse_x_move = 0;
	mouse_y_move = 0;

	if ( mouse_x == mouse_x_old ){
		reg[0] = 0;
	}
	if ( mouse_y == mouse_y_old ){
		reg[1] = 0;
	}
	if ( (mouse_x-mouse_x_old)>0 ){	//鼠标右移
		mouse_x_move = mouse_x - mouse_x_old;
		reg[0] = ( mouse_x_move & 0xC0 ) >> 4;
		reg[0] = reg[0] & 0x7;
//		DEBUGOUT( "鼠标右移！！！\n" );
	}
	if ( (mouse_x-mouse_x_old)<0 ){	//鼠标左移
		mouse_x_move = (mouse_x - mouse_x_old)&0xFF;
		reg[0] = ( mouse_x_move & 0xC0 ) >> 4;
		reg[0] = reg[0] | 0xC;
//		DEBUGOUT( "鼠标左移！！！\n" );
	}

	if ( (mouse_y-mouse_y_old)>0 ){	//鼠标下移
		mouse_y_move = mouse_y - mouse_y_old;
		reg[1] = ( mouse_y_move & 0xC0 ) >> 6;
		reg[1] = reg[1] & 0x1;
//		DEBUGOUT( "鼠标下移！！！\n" );
	}
	if ( (mouse_y-mouse_y_old)<0 ){	//鼠标上移
		mouse_y_move = (mouse_y - mouse_y_old)&0xFF;
		reg[1] = ( mouse_y_move & 0xC0 ) >> 6;
		reg[1] = reg[1] | 0x3;
//		DEBUGOUT( "鼠标上移！！！\n" );
	}

}

BYTE	EXPAD_YuXing_Mouse::Read4016()
{
	BYTE data = 0;

	reg[2]++;

//	DEBUGOUT( "Mouse_X= %04x ; Mouse_Y= %04x ; Mouse_data= %03x\n", mouse_x, mouse_y, mouse_data );
//	DEBUGOUT( "Read  :	mouse_x_move = %03x\n", mouse_x_move );
//	DEBUGOUT( "Read  :	mouse_y_move = %03x\n", mouse_y_move );
//	DEBUGOUT( "Read  :	mouse_button = %03x\n", mouse_button );

	if((mouse_x_move==0)&&(mouse_y_move==0)&&(mouse_button==0)){
		reg[5] = 1;
		if(reg[4]==0){
			DEBUGOUT( "[1]检测 0 无输入...\n" );
			temp1 = 3;
			mouse_data = 0x00;
			return	0;
		}
	}

	reg[4] = 1;
	if((reg[3]==1)){
		reg[3] = 0;
		reg[5] = 0;
		DEBUGOUT( "[4]检测 1 有输入...\n" );
		return	1;
	}

	if(temp1==3){
	DEBUGOUT( "[ 0x90]  = %03x\n", RAM[0x90] );
	DEBUGOUT( "[ 0x91]  = %03x\n", RAM[0x91] );
	DEBUGOUT( "[ 0x92]  = %03x\n", RAM[0x92] );
	DEBUGOUT( "[0x200]  = %03x\n", RAM[0x200] );
	DEBUGOUT( "[0x203]  = %03x\n", RAM[0x203] );
		DEBUGOUT( "[2]检测 1 有输入...\n" );
		reg[5] = 0;
		temp1 = 0; 
		return	1;
	}else{
		switch( temp1 ) {
			case	0x00:
//				mouse_data |= 0x40; 
				if(mouse_button&0x01) mouse_data |= 0x20;
				if(mouse_button&0x02) mouse_data |= 0x10;
				mouse_data |= reg[0];
				mouse_data |= reg[1];
				break;
			case	0x01:
				mouse_data |= (mouse_y_move&0x7F);
				break;
			case	0x02:
				mouse_data |= (mouse_x_move&0x7F);
				break;
		}

//		mouse[0] = (mouse_data>>0)&1;
//		mouse[1] = (mouse_data>>1)&1;
//		mouse[2] = (mouse_data>>2)&1;
//		mouse[3] = (mouse_data>>3)&1;
//		mouse[4] = (mouse_data>>4)&1;
//		mouse[5] = (mouse_data>>5)&1;
//		mouse[6] = (mouse_data>>6)&1;
//		mouse[7] = (mouse_data>>7)&1;
//		mouse_data=(mouse[0]<<7)|(mouse[1]<<6)|(mouse[2]<<5)|(mouse[3]<<4)|(mouse[4]<<3)|(mouse[5]<<2)|(mouse[6]<<1)|(mouse[7]<<0);
//		mouse_data=(mouse[0]<<6)|(mouse[1]<<5)|(mouse[2]<<4)|(mouse[3]<<3)|(mouse[4]<<2)|(mouse[5]<<1)|(mouse[6]<<0);

		if(mouse_bits == 7){
			mouse_bits = 0;
			temp1++;
			reg[3] = 1;
			if(temp1==3){
				reg[3]=0;
				reg[4]=0;
			}
			if(reg[5]==1){
				reg[5]=0;
				reg[3]=0;
				reg[4]=0;
				temp1=3;
				DEBUGOUT( "[5]检测 1 有输入...\n" );
				return	1;
			}
			DEBUGOUT( "[3]检测 0 无输入...\n" );
			DEBUGOUT( "temp1  = %03x\n", temp1 );
			DEBUGOUT( "[0x90]  = %03x\n", RAM[0x90] );
			DEBUGOUT( "[0x91]  = %03x\n", RAM[0x91] );
			DEBUGOUT( "[0x92]  = %03x\n", RAM[0x92] );
			DEBUGOUT( "[0x200]  = %03x\n", RAM[0x200] );
			DEBUGOUT( "[0x203]  = %03x\n", RAM[0x203] );
			return	0;
		}else{
			data |= (((mouse_data<<mouse_bits)&0x40)>>6)^1;
			mouse_bits++;
		}

		DEBUGOUT( "mouse_data  = %03x\n", mouse_data );

		return	data;
	}

//	DEBUGOUT( "mouse_bits= %03x ; mouse_data= %03x ; data= %03x\n", mouse_bits, mouse_data, data );
//	DEBUGOUT( "Mouse_X= %04x ; Mouse_Y= %04x ; Mouse_data= %03x\n", mouse_x, mouse_y, mouse_data );
//	DEBUGOUT( "mouse_count = %03x\n", mouse_count );
//	DEBUGOUT( "mouse_bits  = %03x\n", mouse_bits );

}

BYTE	EXPAD_YuXing_Mouse::Read4017()
{
//	DEBUGOUT( "Read4017  : L=%3d CYC=%d\n", nes->GetScanline(), nes->cpu->GetTotalCycles() );

	BYTE data = 0x00;

	//

	return	data;
}

void	EXPAD_YuXing_Mouse::Write4016( BYTE data )
{
//	DEBUGOUT( "Write4016 : L=%3d CYC=%d\n", nes->GetScanline(), nes->cpu->GetTotalCycles() );


}

void	EXPAD_YuXing_Mouse::Sync()
{
	mouse_x_old = mouse_x;
	mouse_y_old = mouse_y;

	nes->GetZapperPos( mouse_x, mouse_y );
	mouse_button = 0;
	if( ::GetAsyncKeyState(VK_LBUTTON)&0x8000 )
		mouse_button |= 0x01;
	if( ::GetAsyncKeyState(VK_RBUTTON)&0x8000 )
		mouse_button |= 0x02;

}
void	EXPAD_YuXing_Mouse::SetSyncData( INT type, LONG data )
{
	if( type == 0 ) {
		mouse_x = data;
	} else if( type == 1 ) {
		mouse_y = data;
	} else if( type == 2 ) {
		mouse_button = data;
	}
}
LONG	EXPAD_YuXing_Mouse::GetSyncData( INT type )
{
	LONG data = 0;
	if( type == 0 ) {
		data = mouse_x;
	} else if( type == 1 ) {
		data = mouse_y;
	} else if( type == 2 ) {
		data = mouse_button;
	}
	return	data;
}
