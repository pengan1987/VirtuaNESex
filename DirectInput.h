//
// DirectInput class
//
#ifndef	__DIRECTINPUT_INCLUDED__
#define	__DIRECTINPUT_INCLUDED__

#define	WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

#define	DIRECTINPUT_VERSION	0x0700
#include <dinput.h>

#include "typedef.h"
#include "macro.h"

#include <string>
using namespace std;

class	CDirectInput
{
public:
	// パブリックメンバ変数
	typedef	struct	tagDIKEYTBL {
		WORD	key;
		LPCSTR	name;
	} DIKEYTBL, *LPDIKEYTBL;

	static	DIKEYTBL	DIKeyTable[];

	INT	m_nJoystickNum;
	enum { DIJOYSTICK_MAX = 16 };
	enum {
		DI_XAXIS  = 0, DI_YAXIS  = 2, DI_ZAXIS  = 4,
		DI_RXAXIS = 6, DI_RYAXIS = 8, DI_RZAXIS = 10,
		DI_SLIDER0 = 12, DI_SLIDER1 = 14,
		DI_MAXAXIS = 16,
		DI_BUTTON = 16, DI_BUTTON_END = 48,
		DI_EXT = 48,
		DI_POV0_UD = 48, DI_POV0_LR = 50,
		DI_POV1_UD = 52, DI_POV1_LR = 54,
		DI_POV2_UD = 56, DI_POV2_LR = 58,
		DI_POV3_UD = 60, DI_POV3_LR = 62,
		DI_EXT_END = 64,
	};

	BYTE	m_Sw[256+64*DIJOYSTICK_MAX];
	LONG	m_JoyAxis[DIJOYSTICK_MAX][8];
	string	m_JoyName[DIJOYSTICK_MAX];

	static	LPSTR	DIKeyDirTable[];
	static	LPSTR	DIKeyDirTable2[];

	// パブリックメンバ関数
	CDirectInput();
	virtual ~CDirectInput();

	BOOL	InitialDInput( HWND hWnd, HINSTANCE hInst );
	void	ReleaseDInput( void );

	void	Acquire();
	void	Unacquire();
	void	Poll();

	BOOL	AddJoystickDevice( GUID deviceguid );

	void	SetJoyAxisMode( LPWORD pMode ) {
		for( INT i = 0; i < DIJOYSTICK_MAX; i++ )
			m_JoyAxisMode[i] = pMode[i];
	};

	LPCSTR	SearchKeyName( INT key );
protected:
	// プロテクトメンバ変数
	LPDIRECTINPUT7		m_lpDI;
	LPDIRECTINPUTDEVICE	m_lpKeyboard;
	LPDIRECTINPUTDEVICE7	m_lpJoystick[DIJOYSTICK_MAX];

	WORD	m_JoyAxisMode[DIJOYSTICK_MAX];

	// プロテクトメンバ関数
	static	BOOL CALLBACK DIEnumDevicesCallback( LPDIDEVICEINSTANCE lpddi, LPVOID pvRef );
};

extern	CDirectInput	DirectInput;

#endif // !__DIRECTINPUT_INCLUDED__
