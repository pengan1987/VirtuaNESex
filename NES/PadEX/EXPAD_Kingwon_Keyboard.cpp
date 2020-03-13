//////////////////////////////////////////////////////////////////////////
// Kingwon Keyboard                                                     //
//                                                   written by Temryu  //
//                                                     last 2013/10/13  //
//////////////////////////////////////////////////////////////////////////

void	EXPAD_Kingwon_Keyboard::Reset()
{
	ScanNo =0;
	count4017 =0;
}

BYTE	EXPAD_Kingwon_Keyboard::Read4017()
{
	BYTE	data = 0xFF;

	switch( ScanNo ) {
		case	0:
			if(count4017==0) if( DirectInput.m_Sw[DIK_Z      ] ) data &= ~0x02;
			if(count4017==1) if( DirectInput.m_Sw[DIK_A      ] ) data &= ~0x02;
			if(count4017==2) if( DirectInput.m_Sw[DIK_Q      ] ) data &= ~0x02;
			if(count4017==3) if( DirectInput.m_Sw[DIK_1      ] ) data &= ~0x02;
			if(count4017==4) if( DirectInput.m_Sw[DIK_LSHIFT ] ) data &= ~0x02;
			if(count4017==4) if( DirectInput.m_Sw[DIK_RSHIFT ] ) data &= ~0x02;
			if(count4017==5) if( DirectInput.m_Sw[DIK_CAPITAL] ) data &= ~0x02;
			if(count4017==6) if( DirectInput.m_Sw[DIK_TAB    ] ) data &= ~0x02;
			if(count4017==7) if( DirectInput.m_Sw[DIK_GRAVE  ] ) data &= ~0x02;
			count4017++ ;
			break;
		case	1:
			if(count4017==0) if( DirectInput.m_Sw[DIK_C] ) data &= ~0x02;
			if(count4017==1) if( DirectInput.m_Sw[DIK_D] ) data &= ~0x02;
			if(count4017==2) if( DirectInput.m_Sw[DIK_E] ) data &= ~0x02;
			if(count4017==3) if( DirectInput.m_Sw[DIK_3] ) data &= ~0x02;
			if(count4017==4) if( DirectInput.m_Sw[DIK_X] ) data &= ~0x02;
			if(count4017==5) if( DirectInput.m_Sw[DIK_S] ) data &= ~0x02;
			if(count4017==6) if( DirectInput.m_Sw[DIK_W] ) data &= ~0x02;
			if(count4017==7) if( DirectInput.m_Sw[DIK_2] ) data &= ~0x02;
			count4017++ ;
			break;
		case	2:
			if(count4017==0) if( DirectInput.m_Sw[DIK_B] ) data &= ~0x02;
			if(count4017==1) if( DirectInput.m_Sw[DIK_G] ) data &= ~0x02;
			if(count4017==2) if( DirectInput.m_Sw[DIK_T] ) data &= ~0x02;
			if(count4017==3) if( DirectInput.m_Sw[DIK_5] ) data &= ~0x02;
			if(count4017==4) if( DirectInput.m_Sw[DIK_V] ) data &= ~0x02;
			if(count4017==5) if( DirectInput.m_Sw[DIK_F] ) data &= ~0x02;
			if(count4017==6) if( DirectInput.m_Sw[DIK_R] ) data &= ~0x02;
			if(count4017==7) if( DirectInput.m_Sw[DIK_4] ) data &= ~0x02;
			count4017++ ;
			break;
		case	3:
			if(count4017==0) if( DirectInput.m_Sw[DIK_M] ) data &= ~0x02;
			if(count4017==1) if( DirectInput.m_Sw[DIK_J] ) data &= ~0x02;
			if(count4017==2) if( DirectInput.m_Sw[DIK_U] ) data &= ~0x02;
			if(count4017==3) if( DirectInput.m_Sw[DIK_7] ) data &= ~0x02;
			if(count4017==4) if( DirectInput.m_Sw[DIK_N] ) data &= ~0x02;
			if(count4017==5) if( DirectInput.m_Sw[DIK_H] ) data &= ~0x02;
			if(count4017==6) if( DirectInput.m_Sw[DIK_Y] ) data &= ~0x02;
			if(count4017==7) if( DirectInput.m_Sw[DIK_6] ) data &= ~0x02;
			count4017++ ;
			break;
		case	4:
			if(count4017==0) if( DirectInput.m_Sw[DIK_PERIOD] ) data &= ~0x02;
			if(count4017==1) if( DirectInput.m_Sw[DIK_L     ] ) data &= ~0x02;
			if(count4017==2) if( DirectInput.m_Sw[DIK_O     ] ) data &= ~0x02;
			if(count4017==3) if( DirectInput.m_Sw[DIK_9     ] ) data &= ~0x02;
			if(count4017==4) if( DirectInput.m_Sw[DIK_COMMA ] ) data &= ~0x02;
			if(count4017==5) if( DirectInput.m_Sw[DIK_K     ] ) data &= ~0x02;
			if(count4017==6) if( DirectInput.m_Sw[DIK_I     ] ) data &= ~0x02;
			if(count4017==7) if( DirectInput.m_Sw[DIK_8     ] ) data &= ~0x02;
			count4017++ ;
			break;
		case	5:
			if(count4017==0) if( DirectInput.m_Sw[DIK_BACKSLASH ] ) data &= ~0x02;
			if(count4017==1) if( DirectInput.m_Sw[DIK_APOSTROPHE] ) data &= ~0x02;
			if(count4017==2) if( DirectInput.m_Sw[DIK_LBRACKET  ] ) data &= ~0x02;
			if(count4017==3) if( DirectInput.m_Sw[DIK_MINUS     ] ) data &= ~0x02;
			if(count4017==4) if( DirectInput.m_Sw[DIK_SLASH     ] ) data &= ~0x02;
			if(count4017==5) if( DirectInput.m_Sw[DIK_SEMICOLON ] ) data &= ~0x02;
			if(count4017==6) if( DirectInput.m_Sw[DIK_P         ] ) data &= ~0x02;
			if(count4017==7) if( DirectInput.m_Sw[DIK_0         ] ) data &= ~0x02;
			count4017++ ;
			break;
		case	6:
			if(count4017==0) if( DirectInput.m_Sw[DIK_END     ] ) data &= ~0x02;
			if(count4017==1) if( DirectInput.m_Sw[DIK_LEFT    ] ) data &= ~0x02;
			if(count4017==2) if( DirectInput.m_Sw[DIK_HOME    ] ) data &= ~0x02;
//			if(count4017==3) if( FALSE ) data &= ~0x02;	//Caps Lock
			if(count4017==4) if( DirectInput.m_Sw[DIK_BACK    ] ) data &= ~0x02;
			if(count4017==5) if( DirectInput.m_Sw[DIK_RETURN  ] ) data &= ~0x02;
			if(count4017==6) if( DirectInput.m_Sw[DIK_RBRACKET] ) data &= ~0x02;
			if(count4017==7) if( DirectInput.m_Sw[DIK_EQUALS  ] ) data &= ~0x02;
			count4017++ ;
			break;
		case	7:
			if(count4017==0) if( DirectInput.m_Sw[DIK_NEXT  ] ) data &= ~0x02;
			if(count4017==1) if( DirectInput.m_Sw[DIK_RIGHT ] ) data &= ~0x02;
			if(count4017==2) if( DirectInput.m_Sw[DIK_PRIOR ] ) data &= ~0x02;
			if(count4017==3) if( DirectInput.m_Sw[DIK_ESCAPE] ) data &= ~0x02;	//ºÃÏñÊÇ
			if(count4017==4) if( DirectInput.m_Sw[DIK_DOWN  ] ) data &= ~0x02;
//			if(count4017==5) if( FALSE ) data &= ~0x02;	//Num 5
			if(count4017==6) if( DirectInput.m_Sw[DIK_UP    ] ) data &= ~0x02;
//			if(count4017==7) if( FALSE ) data &= ~0x02;	//???
			count4017++ ;
			break;
		case	8:
			if(count4017==0) if( DirectInput.m_Sw[DIK_DELETE     ] ) data &= ~0x02;
			if(count4017==1) if( DirectInput.m_Sw[DIK_ADD        ] ) data &= ~0x02;
			if(count4017==2) if( DirectInput.m_Sw[DIK_MULTIPLY   ] ) data &= ~0x02;
			if(count4017==3) if( DirectInput.m_Sw[DIK_NUMPADENTER] ) data &= ~0x02;
			if(count4017==4) if( DirectInput.m_Sw[DIK_INSERT     ] ) data &= ~0x02;
			if(count4017==5) if( DirectInput.m_Sw[DIK_SPACE      ] ) data &= ~0x02;
			if(count4017==6) if( DirectInput.m_Sw[DIK_LMENU      ] ) data &= ~0x02;
			if(count4017==6) if( DirectInput.m_Sw[DIK_RMENU      ] ) data &= ~0x02;
			if(count4017==7) if( DirectInput.m_Sw[DIK_LCONTROL   ] ) data &= ~0x02;
			if(count4017==7) if( DirectInput.m_Sw[DIK_RCONTROL   ] ) data &= ~0x02;
			count4017++ ;
			break;
		case	9:
			if(count4017==0) if( DirectInput.m_Sw[DIK_F8] ) data &= ~0x02;
			if(count4017==1) if( DirectInput.m_Sw[DIK_F7] ) data &= ~0x02;
			if(count4017==2) if( DirectInput.m_Sw[DIK_F6] ) data &= ~0x02;
			if(count4017==3) if( DirectInput.m_Sw[DIK_F5] ) data &= ~0x02;
			if(count4017==4) if( DirectInput.m_Sw[DIK_F4] ) data &= ~0x02;
			if(count4017==5) if( DirectInput.m_Sw[DIK_F3] ) data &= ~0x02;
			if(count4017==6) if( DirectInput.m_Sw[DIK_F2] ) data &= ~0x02;
			if(count4017==7) if( DirectInput.m_Sw[DIK_F1] ) data &= ~0x02;
			count4017++ ;
			break;
		case	10:
			//???
			break;
		case	11:
			//???
			break;
		case	12:
			//???
			break;
	}

	return	data;
}

void	EXPAD_Kingwon_Keyboard::Write4016( BYTE data )
{
	if( data == 0x04 ) {
		count4017 = 0;
	} else if( data == 0x05 ) {
		if( ++ScanNo > 12 )
			ScanNo = 0;
	} else if( data == 0x07 ) {
		ScanNo = 0;
	}
}

