//////////////////////////////////////////////////////////////////////////
// China Educational Mouse                                              //
//////////////////////////////////////////////////////////////////////////
void	EXPAD_ChinaEduMouse::Reset()
{
	olddata = 0;
	outbits = 0;
	databits = 0;

	zapper_x = 0;
	zapper_y = 0;
	zapper_button = 0;

	readkey = FALSE;
	bOut = FALSE;
	ScanNo = 0;
}

BYTE	EXPAD_ChinaEduMouse::Read4016()
{
	BYTE	data = 0;

	return	data;
}

BYTE	EXPAD_ChinaEduMouse::Read4017()
{
	BYTE	data;
	if(readkey){
		data = 0xFF;
		switch( ScanNo ) {
			case	1:
				if( bOut ) {
					if( DirectInput.m_Sw[DIK_4       ] ) data &= ~0x02;
					if( DirectInput.m_Sw[DIK_G       ] ) data &= ~0x04;
					if( DirectInput.m_Sw[DIK_F       ] ) data &= ~0x08;
					if( DirectInput.m_Sw[DIK_C       ] ) data &= ~0x10;
				} else {
					if( DirectInput.m_Sw[DIK_F2       ] ) data &= ~0x02;
					if( DirectInput.m_Sw[DIK_E        ] ) data &= ~0x04;
					if( DirectInput.m_Sw[DIK_5        ] ) data &= ~0x08;
					if( DirectInput.m_Sw[DIK_V        ] ) data &= ~0x10;
				}
				break;
			case	2:
				if( bOut ) {
					if( DirectInput.m_Sw[DIK_2        ] ) data &= ~0x02;
					if( DirectInput.m_Sw[DIK_D        ] ) data &= ~0x04;
					if( DirectInput.m_Sw[DIK_S        ] ) data &= ~0x08;
					if( DirectInput.m_Sw[DIK_END      ] ) data &= ~0x10;
				} else {
					if( DirectInput.m_Sw[DIK_F1       ] ) data &= ~0x02;
					if( DirectInput.m_Sw[DIK_W         ] ) data &= ~0x04;
					if( DirectInput.m_Sw[DIK_3         ] ) data &= ~0x08;
					if( DirectInput.m_Sw[DIK_X         ] ) data &= ~0x10;
				}
				break;
			case	3:
				if( bOut ) {
					if( DirectInput.m_Sw[DIK_INSERT    ] ) data &= ~0x02;
					if( DirectInput.m_Sw[DIK_BACK      ] ) data &= ~0x04;
					if( DirectInput.m_Sw[DIK_NEXT      ] ) data &= ~0x08;
					if( DirectInput.m_Sw[DIK_RIGHT     ] ) data &= ~0x10;
				} else {
					if( DirectInput.m_Sw[DIK_F8    ] ) data &= ~0x02;
					if( DirectInput.m_Sw[DIK_PRIOR ] ) data &= ~0x04;
					if( DirectInput.m_Sw[DIK_DELETE] ) data &= ~0x08;
					if( DirectInput.m_Sw[DIK_HOME  ] ) data &= ~0x10;
				}
				break;
			case	4:
				if( bOut ) {
					if( DirectInput.m_Sw[DIK_9     ] ) data &= ~0x02;
					if( DirectInput.m_Sw[DIK_I     ] ) data &= ~0x04;
					if( DirectInput.m_Sw[DIK_L     ] ) data &= ~0x08;
					if( DirectInput.m_Sw[DIK_COMMA ] ) data &= ~0x10;
				} else {
					if( DirectInput.m_Sw[DIK_F5     ] ) data &= ~0x02;
					if( DirectInput.m_Sw[DIK_O      ] ) data &= ~0x04;
					if( DirectInput.m_Sw[DIK_0      ] ) data &= ~0x08;
					if( DirectInput.m_Sw[DIK_PERIOD ] ) data &= ~0x10;
				}
				break;
			case	5:
				if( bOut ) {
					if( DirectInput.m_Sw[DIK_RBRACKET ] ) data &= ~0x02;
					if( DirectInput.m_Sw[DIK_RETURN   ] ) data &= ~0x04;
					if( DirectInput.m_Sw[DIK_UP       ] ) data &= ~0x08;
					if( DirectInput.m_Sw[DIK_LEFT     ] ) data &= ~0x10;
				} else {
					if( DirectInput.m_Sw[DIK_F7        ] ) data &= ~0x02;
					if( DirectInput.m_Sw[DIK_LBRACKET  ] ) data &= ~0x04;
					if( DirectInput.m_Sw[DIK_BACKSLASH ] ) data &= ~0x08;
					if( DirectInput.m_Sw[DIK_DOWN      ] ) data &= ~0x10;
				}
				break;
			case	6:
				if( bOut ) {
					if( DirectInput.m_Sw[DIK_Q       ] ) data &= ~0x02;
					if( DirectInput.m_Sw[DIK_CAPITAL ] ) data &= ~0x04;
					if( DirectInput.m_Sw[DIK_Z       ] ) data &= ~0x08;
					if( DirectInput.m_Sw[DIK_TAB     ] ) data &= ~0x10;
				} else {
					if( DirectInput.m_Sw[DIK_ESCAPE   ] ) data &= ~0x02;
					if( DirectInput.m_Sw[DIK_A        ] ) data &= ~0x04;
					if( DirectInput.m_Sw[DIK_1        ] ) data &= ~0x08;
					if( DirectInput.m_Sw[DIK_LCONTROL ] ) data &= ~0x10;
				}
				break;
			case	7:
				if( bOut ) {
					if( DirectInput.m_Sw[DIK_7 ] ) data &= ~0x02;
					if( DirectInput.m_Sw[DIK_Y ] ) data &= ~0x04;
					if( DirectInput.m_Sw[DIK_K ] ) data &= ~0x08;
					if( DirectInput.m_Sw[DIK_M ] ) data &= ~0x10;
				} else {
					if( DirectInput.m_Sw[DIK_F4 ] ) data &= ~0x02;
					if( DirectInput.m_Sw[DIK_U  ] ) data &= ~0x04;
					if( DirectInput.m_Sw[DIK_8  ] ) data &= ~0x08;
					if( DirectInput.m_Sw[DIK_J  ] ) data &= ~0x10;
				}
				break;
			case	8:
				if( bOut ) {
					if( DirectInput.m_Sw[DIK_MINUS      ] ) data &= ~0x02;
					if( DirectInput.m_Sw[DIK_SEMICOLON  ] ) data &= ~0x04;
					if( DirectInput.m_Sw[DIK_APOSTROPHE ] ) data &= ~0x08;
					if( DirectInput.m_Sw[DIK_SLASH      ] ) data &= ~0x10;
				} else {
					if( DirectInput.m_Sw[DIK_F6     ] ) data &= ~0x02;
					if( DirectInput.m_Sw[DIK_P      ] ) data &= ~0x04;
					if( DirectInput.m_Sw[DIK_EQUALS ] ) data &= ~0x08;
					if( DirectInput.m_Sw[DIK_LSHIFT ] ||
						DirectInput.m_Sw[DIK_RSHIFT ] ) data &= ~0x10;
				}
				break;
			case	9:
				if( bOut ) {
					if( DirectInput.m_Sw[DIK_T     ] ) data &= ~0x02;
					if( DirectInput.m_Sw[DIK_H     ] ) data &= ~0x04;
					if( DirectInput.m_Sw[DIK_N     ] ) data &= ~0x08;
					if( DirectInput.m_Sw[DIK_SPACE ] ) data &= ~0x10;
				} else {
					if( DirectInput.m_Sw[DIK_F3 ] ) data &= ~0x02;
					if( DirectInput.m_Sw[DIK_R  ] ) data &= ~0x04;
					if( DirectInput.m_Sw[DIK_6  ] ) data &= ~0x08;
					if( DirectInput.m_Sw[DIK_B  ] ) data &= ~0x10;
				}
				break;
		}
		return	data;
	}else{
			return	outbits;
	}
}

void	EXPAD_ChinaEduMouse::Write4016( BYTE data )
{

//	DEBUGOUT( " X=%d Y=%d\n", zapper_x, zapper_y );

/*	if( !(data & 0x01) ) {
		outbits = 0;
		databits = 0;

		if( zapper_button )
			databits |= 0x0001;

		LONG	x, y;

		if( zapper_y >= 48 ) {
			databits |= 0x0002;
		} else if( zapper_button ) {
			databits |= 0x0003;
		}

		if( zapper_x < 0 ) {
			x = 0;
		} else {
			x = ((240*zapper_x)/256)+8;
		}
		if( zapper_y < 0 ) {
			y = 0;
		} else {
			y = ((256*zapper_y)/240)-12;
			if( y < 0 )
				y = 0;
		}

		databits = databits | (x << 10) | (y << 2);
	} else {
		// L->H
		if( ((~olddata)&data) & 0x02 ) {
			databits <<= 1;
		}
		if( !(data & 0x02) ) {
			outbits = 0x04;
		} else
		if( databits & 0x40000 ) {
			outbits = 0x00;
		} else {
			outbits = 0x08;
		}
		olddata = data;
	}
*/
	switch( data ) {
		case 0:
			readkey = FALSE;
			outbits = 0x41;
			break;
		case 1:
			readkey = FALSE;
			outbits = 0x41;
			break;
		case 4:
			readkey = TRUE;
			if( ++ScanNo > 9 )
				ScanNo = 0;
			bOut = !bOut;
			break;
		case 5:
			readkey = FALSE;
			bOut = FALSE;
			ScanNo = 0;

			//if( zapper_button )
				outbits = 0x41;

			break;
		case 6:
			readkey = TRUE;
			bOut = !bOut;
			break;
	}
}

void	EXPAD_ChinaEduMouse::Sync()
{
	nes->GetZapperPos( zapper_x, zapper_y );

	zapper_button = 0;
	if( ::GetAsyncKeyState(VK_LBUTTON)&0x8000 )
		zapper_button = 0xFF;
}

void	EXPAD_ChinaEduMouse::SetSyncData( INT type, LONG data )
{
	if( type == 0 ) {
		zapper_x = data;
	} else if( type == 1 ) {
		zapper_y = data;
	} else if( type == 2 ) {
		zapper_button = data?0xFF:0x00;
	}
}

LONG	EXPAD_ChinaEduMouse::GetSyncData( INT type )
{
LONG	data = 0;

	if( type == 0 ) {
		data = zapper_x;
	} else if( type == 1 ) {
		data = zapper_y;
	} else if( type == 2 ) {
		data = zapper_button?0xFF:0x00;
	}
	return	data;
}

