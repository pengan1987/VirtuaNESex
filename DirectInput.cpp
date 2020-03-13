//
// DirectInput class
//
#include "DebugOut.h"
#include "DirectInput.h"
#include "COM.h"
#include "Config.h"

CDirectInput	DirectInput;

#define	COMUSE	TRUE

//
// Table
//
CDirectInput::DIKEYTBL	CDirectInput::DIKeyTable[] = {
	DIK_ESCAPE,	"ESC",		DIK_1,		"1",
	DIK_2,		"2",		DIK_3,		"3",
	DIK_4,		"4",		DIK_5,		"5",
	DIK_6,		"6",		DIK_7,		"7",
	DIK_8,		"8",		DIK_9,		"9",
	DIK_0,		"0",		DIK_MINUS,	"-",
	DIK_EQUALS,	"=",		DIK_BACK,	"BackSpace",
	DIK_TAB,	"TAB",		DIK_Q,		"Q",
	DIK_W,		"W",		DIK_E,		"E",
	DIK_R,		"R",		DIK_T,		"T",
	DIK_Y,		"Y",		DIK_U,		"U",
	DIK_I,		"I",		DIK_O,		"O",
	DIK_P,		"P",		DIK_LBRACKET,	"[",
	DIK_RBRACKET,	"]",		DIK_RETURN,	"Enter",
	DIK_LCONTROL,	"L Ctrl",	DIK_A,		"A",
	DIK_S,		"S",		DIK_D,		"D",
	DIK_F,		"F",		DIK_G,		"G",
	DIK_H,		"H",		DIK_J,		"J",
	DIK_K,		"K",		DIK_L,		"L",
	DIK_SEMICOLON,	";",		DIK_APOSTROPHE,	"'",
	DIK_GRAVE,	"`",		DIK_LSHIFT,	"L Shift",
	DIK_BACKSLASH,	"\\",		DIK_Z,		"Z",
	DIK_X,		"X",		DIK_C,		"C",
	DIK_V,		"V",		DIK_B,		"B",
	DIK_N,		"N",		DIK_M,		"M",
	DIK_COMMA,	",",		DIK_PERIOD,	".",
	DIK_SLASH,	"/",		DIK_RSHIFT,	"R Shift",
	DIK_MULTIPLY,	"*",		DIK_LMENU,	"L Alt",
	DIK_SPACE,	"Space",
	DIK_F1,		"F1",		DIK_F2,		"F2",
	DIK_F3,		"F3",		DIK_F4,		"F4",
	DIK_F5,		"F5",		DIK_F6,		"F6",
	DIK_F7,		"F7",		DIK_F8,		"F8",
	DIK_F9,		"F9",		DIK_F10,	"F10",

	DIK_NUMPAD7,	"Num 7",	DIK_NUMPAD8,	"Num 8",
	DIK_NUMPAD9,	"Num 9",	DIK_SUBTRACT,	"Num -",
	DIK_NUMPAD4,	"Num 4",	DIK_NUMPAD5,	"Num 5",
	DIK_NUMPAD6,	"Num 6",	DIK_ADD,	"Num +",
	DIK_NUMPAD1,	"Num 1",	DIK_NUMPAD2,	"Num 2",
	DIK_NUMPAD3,	"Num 3",	DIK_NUMPAD0,	"Num 0",
	DIK_DECIMAL,	"Num .",	DIK_F11,	"F11",
	DIK_F12,	"F12",		DIK_F13,	"F13",
	DIK_F14,	"F14",		DIK_F15,	"F15",
	DIK_CONVERT,	"変換",
	DIK_NOCONVERT,	"無変換",	DIK_YEN,	"\\",
	DIK_NUMPADEQUALS,"Num =",	DIK_CIRCUMFLEX,	"^",
	DIK_AT,		"@",		DIK_COLON,	":",
	DIK_UNDERLINE,	"_",
	DIK_STOP,	"Stop",		DIK_NUMPADENTER,"Num Enter",
	DIK_RCONTROL,	"R Ctrl",	DIK_NUMPADCOMMA,"Num ,",
	DIK_DIVIDE,	"Num /",	DIK_SYSRQ,	"SysRq",
	DIK_RMENU,	"R Alt",	DIK_PAUSE,	"Pause",
	DIK_HOME,	"Home",		DIK_UP,		"Up",
	DIK_PRIOR,	"Page Up",	DIK_LEFT,	"Left",
	DIK_RIGHT,	"Right",	DIK_END,	"End",
	DIK_DOWN,	"Down",		DIK_NEXT,	"Page Down",
	DIK_INSERT,	"Insert",	DIK_DELETE,	"Delete",
	DIK_LWIN,	"L Windows",	DIK_LWIN,	"R Windows",
	DIK_APPS,	"AppMenu",

#if	0
// トグル系キーなので使えない
	DIK_CAPITAL,	"Caps Lock",
	DIK_NUMLOCK,	"NumLock",
	DIK_SCROLL,	"ScrollLock",
	DIK_KANA,	"カナ",	
	DIK_KANJI,	"漢字",
#endif
	0x00,		NULL
};

LPSTR	CDirectInput::DIKeyDirTable[] = {
	"X+", "X-", "Y+", "Y-", "Z+", "Z-",
	"RX+", "RX-", "RY+", "RY-", "RZ+", "RZ-",
	"S0+", "S0-", "S1+", "S1-",
};

LPSTR	CDirectInput::DIKeyDirTable2[] = {
	"P0 Up", "P0 Down", "P0 Left", "P0 Right",
	"P1 Up", "P1 Down", "P1 Left", "P1 Right",
	"P2 Up", "P2 Down", "P2 Left", "P2 Right",
	"P3 Up", "P3 Down", "P3 Left", "P3 Right",
};

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CDirectInput::CDirectInput()
{
	m_lpDI          = NULL;
	m_lpKeyboard    = NULL;

	m_nJoystickNum  = 0;

	ZEROMEMORY( m_lpJoystick, sizeof(m_lpJoystick) );
	ZEROMEMORY( m_Sw, sizeof(m_Sw) );

	ZEROMEMORY( m_JoyAxisMode, sizeof(m_JoyAxisMode) );

#if	COMUSE
	COM::AddRef();
#endif
}

CDirectInput::~CDirectInput()
{
	ReleaseDInput();

#if	COMUSE
	COM::AddRef();
#endif
}

//////////////////////////////////////////////////////////////////////
// メンバ関数
//////////////////////////////////////////////////////////////////////
// デバイスオブジェクト列挙コールバック
BOOL CALLBACK CDirectInput::DIEnumDevicesCallback( LPDIDEVICEINSTANCE lpddi, LPVOID pvRef )
{
	CDirectInput* pCDi = (CDirectInput*)pvRef;

//	DEBUGOUT( "dwDevType=%08X  IName:%s  PName:%s\n", lpddi->dwDevType, lpddi->tszInstanceName, lpddi->tszProductName );

	if( pCDi->AddJoystickDevice( lpddi->guidInstance ) )
		return	DIENUM_CONTINUE;

	return	DIENUM_STOP;
}

// ジョイスティックデバイスオブジェクトの作成
BOOL	CDirectInput::AddJoystickDevice( GUID deviceguid )
{
	LPDIRECTINPUTDEVICE7	lpDIDev;

	if( m_lpDI->CreateDeviceEx( deviceguid, IID_IDirectInputDevice7,
		(LPVOID*)&lpDIDev, NULL ) != DI_OK ) {
		return	FALSE;
	}

	if( lpDIDev->SetDataFormat( &c_dfDIJoystick ) != DI_OK ) {
		DEBUGOUT( "CDirectInput:SetDataFormat failed.\n" );
		RELEASE( lpDIDev );
		return	FALSE;
	}

	INT	nID = m_nJoystickNum;

	if( !Config.general.bNoJoystickID ) {
		// DX7では隠し要素のジョイスティックIDの取得(DX8からはマニュアルに記載されている)
		DIPROPDWORD	diprp_dw;
		ZEROMEMORY( &diprp_dw, sizeof(diprp_dw) );
		diprp_dw.diph.dwSize       = sizeof(DIPROPDWORD);
		diprp_dw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprp_dw.diph.dwHow        = DIPH_DEVICE;
		diprp_dw.diph.dwObj        = 0;

		if( lpDIDev->GetProperty( DIPROP_JOYSTICKID, &diprp_dw.diph ) != DI_OK ) {
			DEBUGOUT( "CDirectInput:GetProperty failed.\n" );
			RELEASE( lpDIDev );
			return	FALSE;
		}
DEBUGOUT( "ID:%d\n", diprp_dw.dwData );

		nID = diprp_dw.dwData;
	}

	if( nID < DIJOYSTICK_MAX ) {
		m_lpJoystick[ nID ] = lpDIDev;

		// 各軸のレンジを設定
		DIPROPRANGE	diprg; 
		diprg.diph.dwSize       = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow        = DIPH_BYOFFSET;
		diprg.diph.dwObj        = DIJOFS_X;
		diprg.lMin              = -10000;
		diprg.lMax              = +10000;
		lpDIDev->SetProperty( DIPROP_RANGE, &diprg.diph );
		diprg.diph.dwObj        = DIJOFS_Y;
		lpDIDev->SetProperty( DIPROP_RANGE, &diprg.diph );
		diprg.diph.dwObj        = DIJOFS_Z;
		lpDIDev->SetProperty( DIPROP_RANGE, &diprg.diph );
		diprg.diph.dwObj        = DIJOFS_RX;
		lpDIDev->SetProperty( DIPROP_RANGE, &diprg.diph );
		diprg.diph.dwObj        = DIJOFS_RY;
		lpDIDev->SetProperty( DIPROP_RANGE, &diprg.diph );
		diprg.diph.dwObj        = DIJOFS_RZ;
		lpDIDev->SetProperty( DIPROP_RANGE, &diprg.diph );

		diprg.diph.dwObj        = DIJOFS_SLIDER(0);
		lpDIDev->SetProperty( DIPROP_RANGE, &diprg.diph );
		diprg.diph.dwObj        = DIJOFS_SLIDER(1);
		lpDIDev->SetProperty( DIPROP_RANGE, &diprg.diph );

		// 名称の取得
		DIDEVICEINSTANCE didins;
		ZEROMEMORY( &didins, sizeof(didins) );
		didins.dwSize = sizeof( didins );
		lpDIDev->GetDeviceInfo( &didins );

		m_JoyName[ nID ] = didins.tszInstanceName;
DEBUGOUT( "Instance Name:%s\n", didins.tszInstanceName );
//DEBUGOUT( "Product  Name:%s\n", didins.tszProductName );
	} else {
		m_lpJoystick[ nID ] = NULL;
		RELEASE( lpDIDev );
	}

	m_nJoystickNum++;

	return	TRUE;
}

// DirectInputオブジェクト／デバイスオブジェクトの構築
BOOL	CDirectInput::InitialDInput(HWND hWnd, HINSTANCE hInst)
{
	try {
		// CDirectInputオブジェクトの作成
#if	!COMUSE
		if( DirectInputCreateEx( hInst, DIRECTINPUT_VERSION, IID_IDirectInput7, (LPVOID*)&m_lpDI, NULL ) != DI_OK ) {
			m_lpDI = NULL;
			throw "CDirectInput:DirectInputCreateEx failed.";
		}
#else
		// COM的利用
//		COM::AddRef();
		if( FAILED(CoCreateInstance( CLSID_DirectInput, NULL, CLSCTX_INPROC_SERVER, IID_IDirectInput7, (VOID**)&m_lpDI )) ) {
			m_lpDI = NULL;
			throw	"CDirectInput:CoCreateInstance failed.";
		}
		if( m_lpDI->Initialize( hInst, DIRECTINPUT_VERSION ) != DI_OK )
			throw	"CDirectInput:IDirectInput7->Initialize failed.";
#endif

		if( m_lpDI->CreateDevice( GUID_SysKeyboard, &m_lpKeyboard, NULL ) != DI_OK )
			throw	"CDirectInput:CreateDevice failed.";

		if( m_lpKeyboard ) {
			if( m_lpKeyboard->SetDataFormat( &c_dfDIKeyboard ) != DI_OK )
				throw	"CDirectInput:SetDataFormat failed.";
			if( m_lpKeyboard->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE|DISCL_BACKGROUND) != DI_OK )
				throw	"CDirectInput:SetCooperativeLevel failed.";
			if( m_lpKeyboard->Acquire() != DI_OK ) {
//				DEBUGOUT( "CDirectInput:Acquire failed.\n" );
			}
		}

		m_nJoystickNum = 0;
		if( m_lpDI->EnumDevices( DIDEVTYPE_JOYSTICK, (LPDIENUMDEVICESCALLBACK)DIEnumDevicesCallback,
					(LPVOID)this, DIEDFL_ATTACHEDONLY ) != DI_OK ) {
			DEBUGOUT( "CDirectInput:EnumDevices failed.\n" );
		}

		if( !m_nJoystickNum ) {
			DEBUGOUT( "CDirectInput:No Joystick device available.\n" );
		} else {
			for( INT i = 0; i < DIJOYSTICK_MAX; i++ ) {
				if( m_lpJoystick[i] ) {
					if( m_lpJoystick[i]->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE|DISCL_BACKGROUND) != DI_OK ) {
						DEBUGOUT( "CDirectInput:SetCooperativeLevel failed.\n" );
						throw 	"CDirectInput:SetCooperativeLevel failed.";
					}
				}
			}

			DEBUGOUT( "CDirectInput:Can use %d Joystick(s)\n", m_nJoystickNum );
		}
	} catch( char *str ) {
		ReleaseDInput();

		MessageBox( hWnd, str, "ERROR", MB_ICONERROR|MB_OK );

		return	FALSE;
	}

	return	TRUE;
}

void	CDirectInput::ReleaseDInput()
{
	for( INT i = 0; i < DIJOYSTICK_MAX; i++ ) {
		RELEASE( m_lpJoystick[i] );
	}

	if( m_lpKeyboard ) {
//		m_lpKeyboard->Unacquire();
		RELEASE( m_lpKeyboard );
	}

	if( m_lpDI ) {
		RELEASE( m_lpDI );
#if	COMUSE
//		COM::Release();
#endif
	}
}

// 入力フォーカスを取得
void	CDirectInput::Acquire()
{
	if( !m_lpDI )
		return;
	if( m_lpKeyboard )
		m_lpKeyboard->Acquire();
	for( INT i = 0; i < DIJOYSTICK_MAX; i++ ) {
		if( m_lpJoystick[i] ) {
			m_lpJoystick[i]->Acquire();
		}
	}
}

// 入力フォーカスを開放
void	CDirectInput::Unacquire()
{
	if( !m_lpDI )
		return;
	if( m_lpKeyboard )
		m_lpKeyboard->Unacquire();
	for( INT i = 0; i < DIJOYSTICK_MAX; i++ ) {
		if( m_lpJoystick[i] ) {
			m_lpJoystick[i]->Unacquire();
		}
	}
}

// データポーリング
void	CDirectInput::Poll()
{
DIJOYSTATE	js;

	ZEROMEMORY( m_Sw, sizeof(m_Sw) );

	if( !m_lpDI ) {
		return;
	}

	if( m_lpKeyboard ) {
		if( m_lpKeyboard->GetDeviceState( 256, &m_Sw ) == DIERR_INPUTLOST ) {
			m_lpKeyboard->Acquire();
			m_lpKeyboard->GetDeviceState( 256, &m_Sw );
		}
	}

	INT	idx;
	for( INT i = 0; i < DIJOYSTICK_MAX; i++ ) {
		if( !m_lpJoystick[i] )
			continue;

		idx = 256+i*64;

		if( m_lpJoystick[i]->Poll() == DIERR_INPUTLOST ) {
			m_lpJoystick[i]->Acquire();
			m_lpJoystick[i]->Poll();
		}
		if( m_lpJoystick[i]->GetDeviceState( sizeof(DIJOYSTATE), &js ) != DI_OK ) {
			ZEROMEMORY( &js, sizeof(DIJOYSTATE) );
		}

		m_JoyAxis[i][0] = js.lX;
		m_JoyAxis[i][1] = js.lY;
		m_JoyAxis[i][2] = js.lZ;
		m_JoyAxis[i][3] = js.lRx;
		m_JoyAxis[i][4] = js.lRy;
		m_JoyAxis[i][5] = js.lRz;

		if( !(m_JoyAxisMode[i] & (1<<0)) ) {
			if( js.lX >  8000 ) m_Sw[idx + DI_XAXIS+0] = 0x80;
			if( js.lX < -8000 ) m_Sw[idx + DI_XAXIS+1] = 0x80;
		}
		if( !(m_JoyAxisMode[i] & (1<<1)) ) {
			if( js.lY >  8000 ) m_Sw[idx + DI_YAXIS+0] = 0x80;
			if( js.lY < -8000 ) m_Sw[idx + DI_YAXIS+1] = 0x80;
		}
		if( !(m_JoyAxisMode[i] & (1<<2)) ) {
			if( js.lZ >  8000 ) m_Sw[idx + DI_ZAXIS+0] = 0x80;
			if( js.lZ < -8000 ) m_Sw[idx + DI_ZAXIS+1] = 0x80;
		}
		if( !(m_JoyAxisMode[i] & (1<<3)) ) {
			if( js.lRx >  8000 ) m_Sw[idx + DI_RXAXIS+0] = 0x80;
			if( js.lRx < -8000 ) m_Sw[idx + DI_RXAXIS+1] = 0x80;
		}
		if( !(m_JoyAxisMode[i] & (1<<4)) ) {
			if( js.lRy >  8000 ) m_Sw[idx + DI_RYAXIS+0] = 0x80;
			if( js.lRy < -8000 ) m_Sw[idx + DI_RYAXIS+1] = 0x80;
		}
		if( !(m_JoyAxisMode[i] & (1<<5)) ) {
			if( js.lRz >  8000 ) m_Sw[idx + DI_RZAXIS+0] = 0x80;
			if( js.lRz < -8000 ) m_Sw[idx + DI_RZAXIS+1] = 0x80;
		}

#if	0
// 2003/11/3 とりあえず無効化
		if( js.rglSlider[0] >  8000 ) m_Sw[idx + DI_SLIDER0+0] = 0x80;
		if( js.rglSlider[0] < -8000 ) m_Sw[idx + DI_SLIDER0+1] = 0x80;
		if( js.rglSlider[1] >  8000 ) m_Sw[idx + DI_SLIDER1+0] = 0x80;
		if( js.rglSlider[1] < -8000 ) m_Sw[idx + DI_SLIDER1+1] = 0x80;
#endif

		for( INT j = 0; j < 32; j++ ) {
			m_Sw[idx + DI_BUTTON + j] = js.rgbButtons[j];
		}

		// POV
		for( INT pov = 0; pov < 4; pov++ ) {
			DWORD	dwPOV = js.rgdwPOV[pov];
			BOOL	bPOVcenter = (LOWORD(dwPOV) == 0xFFFF);

			BYTE	data = 0;
			if( !bPOVcenter ) {
				static const BYTE dirtbl[] = {
					(1<<0), (1<<0)|(1<<3), (1<<3), (1<<1)|(1<<3),
					(1<<1), (1<<1)|(1<<2), (1<<2), (1<<0)|(1<<2),
				};

				data = dirtbl[ ((dwPOV+(DWORD)(22.5*DI_DEGREES)) % (360*DI_DEGREES))/(45*DI_DEGREES) ];
			}

			// Up/Down
			if( data & (1<<0) ) m_Sw[idx + DI_POV0_UD+i*4+0] = 0x80;
			if( data & (1<<1) ) m_Sw[idx + DI_POV0_UD+i*4+1] = 0x80;
			// Left/Right
			if( data & (1<<2) ) m_Sw[idx + DI_POV0_LR+i*4+0] = 0x80;
			if( data & (1<<3) ) m_Sw[idx + DI_POV0_LR+i*4+1] = 0x80;
		}
	}
}

LPCSTR	CDirectInput::SearchKeyName( INT key )
{
LPDIKEYTBL kt = DIKeyTable;
static	CHAR	KeyStr[256];

	if( key == 0x00 )
		return	NULL;

	if( key < 0x100 ) {
		while( kt->name != NULL ) {
			if( kt->key == key )
				return	kt->name;
			kt++;
		}
	} else {
		INT	no  = (key-256)>>6;
		INT	idx = key & 0x3F;
		if( idx < DI_MAXAXIS ) {
			::wsprintf( KeyStr, "J:%d %s", no, DIKeyDirTable[idx] );
			return	KeyStr;
		} else if( idx >= DI_BUTTON && idx < DI_BUTTON_END ) {
			::wsprintf( KeyStr, "J:%d B:%02d", no, idx-DI_BUTTON );
			return	KeyStr;
		} else if( idx >= DI_EXT && idx < DI_EXT_END ) {
			::wsprintf( KeyStr, "J:%d %s", no, DIKeyDirTable2[idx-DI_EXT] );
			return	KeyStr;
		}
	}

	return	NULL;
}

