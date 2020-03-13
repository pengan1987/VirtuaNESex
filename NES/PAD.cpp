//////////////////////////////////////////////////////////////////////////
//                                                                      //
//      NES Pad                                                         //
//                                                           Norix      //
//                                               written     2001/02/22 //
//                                               last modify ----/--/-- //
//////////////////////////////////////////////////////////////////////////
#define	WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "typedef.h"
#include "macro.h"

#include "DebugOut.h"
#include "Config.h"

#include "DirectDraw.h"
#include "DirectInput.h"

#include "nes.h"
#include "mmu.h"
#include "cpu.h"
#include "ppu.h"
#include "pad.h"
#include "rom.h"

PAD::PAD( NES* parent ) : nes( parent )
{
	excontroller_select = 0;
	expad = NULL;
	bStrobe = FALSE;
	bSwapButton = FALSE;
	bSwapPlayer = FALSE;
	bZapperMode = FALSE;
	nVSSwapType = VS_TYPE0;

	padbit[0] = padbit[1] = padbit[2] = padbit[3] = 0;
	micbit = 0;

	padbitsync[0] = padbitsync[1] = padbitsync[2] = padbitsync[3] = 0;
	micbitsync = 0;
}

PAD::~PAD()
{
	DirectDraw.SetZapperMode( FALSE );
	DirectDraw.SetZapperDrawMode( FALSE );

	DELETEPTR( expad );
}

void	PAD::Reset()
{
	pad1bit = pad2bit = 0;
	bStrobe = FALSE;

//	if( !bZapperMode ) {
//		bZapperMode = FALSE;
//	}

	bBarcodeWorld = FALSE;

	ZEROMEMORY( padcnt, sizeof(padcnt) );

	// Select Extension Devices
	DWORD	crc = nes->rom->GetPROM_CRC();

	if( crc == 0xfbfc6a6c		// Adventures of Bayou Billy, The(E)
	 || crc == 0xcb275051		// Adventures of Bayou Billy, The(U)
	 || crc == 0xfb69c131		// Baby Boomer(Unl)(U)
	 || crc == 0xf2641ad0		// Barker Bill's Trick Shooting(U)
	 || crc == 0xbc1dce96		// Chiller (Unl)(U)
	 || crc == 0x90ca616d		// Duck Hunt(JUE)
	 || crc == 0x59e3343f		// Freedom Force(U)
	 || crc == 0x242a270c		// Gotcha!(U)
	 || crc == 0x7b5bd2de		// Gumshoe(UE)
	 || crc == 0x255b129c		// Gun Sight(J)
	 || crc == 0x8963ae6e		// Hogan's Alley(JU)
	 || crc == 0x51d2112f		// Laser Invasion(U)
	 || crc == 0x0a866c94		// Lone Ranger, The(U)
//	 || crc == 0xe4c04eea		// Mad City(J)
	 || crc == 0x9eef47aa		// Mechanized Attack(U)
	 || crc == 0xc2db7551		// Shooting Range(U)
	 || crc == 0x163e86c0		// To The Earth(U)
	 || crc == 0x0d3cf705		// Wild Gunman(J)
	 || crc == 0x389960db ) {	// Wild Gunman(JUE)
		SetExController( EXCONTROLLER_ZAPPER );
	}
	if( crc == 0x35893b67		// Arkanoid(J)
	 || crc == 0x6267fbd1 ) {	// Arkanoid 2(J)
		SetExController( EXCONTROLLER_PADDLE );
	}
	if( crc == 0xff6621ce		// Hyper Olympic(J)
	 || crc == 0xdb9418e8		// Hyper Olympic(Tonosama Ban)(J)
	 || crc == 0xac98cd70 ) {	// Hyper Sports(J)
		SetExController( EXCONTROLLER_HYPERSHOT );
	}
	if( crc == 0xc68363f6 ) {	// Crazy Climber(J)
		SetExController( EXCONTROLLER_CRAZYCLIMBER );
	}
	if( crc == 0x20d22251 ) {	// Top rider(J)
		SetExController( EXCONTROLLER_TOPRIDER );
	}
	if( crc == 0x0cd00488 ) {	// Space Shadow(J)
		SetExController( EXCONTROLLER_SPACESHADOWGUN );
	}

	if( crc == 0x8c8fa83b		// Family Trainer - Athletic World (J)
	 || crc == 0x7e704a14		// Family Trainer - Jogging Race (J)
	 || crc == 0x2330a5d3 ) {	// Family Trainer - Rairai Kyonshiizu (J)
		SetExController( EXCONTROLLER_FAMILYTRAINER_A );
	}
	if( crc == 0xf8da2506		// Family Trainer - Aerobics Studio (J)
	 || crc == 0xca26a0f1		// Family Trainer - Dai Undoukai (J)
	 || crc == 0x28068b8c		// Family Trainer - Fuuun Takeshi Jou 2 (J)
	 || crc == 0x10bb8f9a		// Family Trainer - Manhattan Police (J)
	 || crc == 0xad3df455		// Family Trainer - Meiro Dai Sakusen (J)
	 || crc == 0x8a5b72c0		// Family Trainer - Running Stadium (J)
	 || crc == 0x59794f2d ) {	// Family Trainer - Totsugeki Fuuun Takeshi Jou (J)
		SetExController( EXCONTROLLER_FAMILYTRAINER_B );
	}
	if( crc == 0x9fae4d46		// Ide Yousuke Meijin no Jissen Mahjong (J)
	 || crc == 0x7b44fb2a ) {	// Ide Yousuke Meijin no Jissen Mahjong 2 (J)
		SetExController( EXCONTROLLER_MAHJANG );
	}
	if( crc == 0x786148b6 ) {	// Exciting Boxing (J)
		SetExController( EXCONTROLLER_EXCITINGBOXING );
	}
	if( crc == 0xc3c0811d		// Oeka Kids - Anpanman no Hiragana Daisuki (J)
	 || crc == 0x9d048ea4 ) {	// Oeka Kids - Anpanman to Oekaki Shiyou!! (J)
		SetExController( EXCONTROLLER_OEKAKIDS_TABLET );
	}
#if	0
	if( crc == 0xe792de94		// Best Play - Pro Yakyuu (New) (J)
	 || crc == 0xf79d684a		// Best Play - Pro Yakyuu (Old) (J)
	 || crc == 0xc2ef3422		// Best Play - Pro Yakyuu 2 (J)
	 || crc == 0x974e8840		// Best Play - Pro Yakyuu '90 (J)
	 || crc == 0xb8747abf		// Best Play - Pro Yakyuu Special (J)
	 || crc == 0x9fa1c11f		// Castle Excellent (J)
	 || crc == 0x0b0d4d1b		// Derby Stallion - Zenkoku Ban (J)
	 || crc == 0x728c3d98		// Downtown - Nekketsu Monogatari (J)
	 || crc == 0xd68a6f33		// Dungeon Kid (J)
	 || crc == 0x3a51eb04		// Fleet Commander (J)
	 || crc == 0x7c46998b		// Haja no Fuuin (J)
	 || crc == 0x7e5d2f1a		// Itadaki Street - Watashi no Mise ni Yottette (J)
	 || crc == 0xcee5857b		// Ninjara Hoi! (J)
	 || crc == 0x50ec5e8b		// Wizardry - Legacy of Llylgamyn (J)
	 || crc == 0x343e9146		// Wizardry - Proving Grounds of the Mad Overlord (J)
	 || crc == 0x33d07e45 ) {	// Wizardry - The Knight of Diamonds (J)
		SetExController( EXCONTROLLER_TURBOFILE );
	}
#endif

	if( crc == 0x67898319 ) {	// Barcode World (J)
		bBarcodeWorld = TRUE;
	}

	// VS-Unisystem
	if( nes->rom->IsVSUNISYSTEM() ) {
		if( crc == 0xff5135a3		// VS Hogan's Alley
		 || crc == 0xed588f00		// VS Duck Hunt
		 || crc == 0x17ae56be ) {	// VS Freedom Force
			SetExController( EXCONTROLLER_VSZAPPER );
		} else {
			SetExController( EXCONTROLLER_VSUNISYSTEM );
		}
	}

	if( crc == 0xf9def527		// Family BASIC(Ver2.0)
	 || crc == 0xde34526e		// Family BASIC(Ver2.1a)
	 || crc == 0xf050b611		// Family BASIC(Ver3)
	 || crc == 0x3aaeed3f ) {	// Family BASIC(Ver3)(Alt)
		SetExController( EXCONTROLLER_KEYBOARD );
	}

	if( crc == 0x82F1Fb96		// [Subor] Subor V1.0 (Russian) [Mapper167]
	 || crc == 0xAA044941		// [Subor] Subor Win2000 (Russian)
	 || crc == 0xE4460DF2		// [Subor] Subor V1.1 (C)
	 || crc == 0x0930349E		// [Subor] Subor V3.0 (C) (With Dr.Mario)
	 || crc == 0x6733607A		// [Subor] Subor V3.0 (C) (With Chinese Chess)
	 || crc == 0x589b6b0d		// [Subor] Subor V3.0 tpu
	 || crc == 0x41EF9AC4		// [Subor] Subor V4.0 (C)
	 || crc == 0x41401c6d		// [Subor] Subor V4.0 tpu
	 || crc == 0x85068811		// [Subor] Subor V6.0 (C)
	 || crc == 0x04260DBC		// [Subor] Subor V7.0 (C)
	 || crc == 0xE475D89A		// [Subor] Subor V9.0 (C)
	 || crc == 0x900D9E00		// [Subor] Subor V9.1 (C)
	 || crc == 0x6F84076D		// [Subor] Subor V10.0 (C)
	 || crc == 0x12D61CE8		// [Subor] Subor V11.0 (C)
	 || crc == 0x5F693117		// [Subor] Subor V13.0 (C)
	 || crc == 0xF18BC238		// [Subor] Subor V14.0 (C)
	 || crc == 0x366C20D7		// [Subor] Subor LOGO V1.0 (C)
	 || crc == 0x5E073A1B		// [Subor] Subor English Word Blaster V1.0 (C)
	 || crc == 0x8b265862		// [Subor] Subor English tpu
	 || crc == 0xF9BC83B9		// [Subor] Cang Ku Shi Jia & Dong Nao Jin (C)
	 || crc == 0x669A32F2		// [Subor] Ma Bu Mi Zhen & Qu Wei Cheng Yu Wu (C)
	 || crc == 0x40139EE3		// [Subor] Tu Xing Tui Li & Zhi Li Pin Tu (C)
	 || crc == 0x3C8F92BD		// [Subor] Zheng Fu Tai Yang Xi & Xiao Guan Jia (C)
	 || crc == 0x0A9329B5		// [Subor] Qi Zhi Wu A (C)
	 || crc == 0x15058291		// [Subor] Qi Zhi Wu B (C)
	 || crc == 0x60E33D27		// [Subor] Ren Shi Ma Xue Xi (C)
	 || crc == 0xD3906F32		// [Subor] Xiao Xue Shu Xue Jiao Shou Xi Tong - San Nian Ji (C)
	 || crc == 0x40A4C574		// [Subor] Yu Yin Zhi Xing Sheng Ji Mu Ka (C)
	 || crc == 0xCA501706		// [Subor] Yu Yin Zhi Xing Sheng Ji Mu Ka + You Sheng Ci Ba (C)
	 || crc == 0xFDD9321C		// [Subor] Yu Yin Zhi Xing Sheng Ji Mu Ka + Xiao Xue Yu Wen Da Biao Le Yuan (C)

	 || crc == 0x6A21D8F8		// [BenLi] Ben Li Dian Nao Ka (C)-1
	 || crc == 0x827207FD		// [BenLi] Ben Li Dian Nao Ka (C)-2
	 || crc == 0xDD150A3C		// [SB-918] Xue Sheng Dian Nao (C)
	 || crc == 0x45A9F4E5		// Chao Ba Dian Nao (C)

	 || crc == 0xE90A6AEB		// Jin Ka Wang - Xue Dian Nao (C)
	 || crc == 0x2AF63A19		// RONG FENG - Study Card (C)
	 || crc == 0x957ABE27		// Sheng Ba Ka (C)
	 || crc == 0x2726FC11		// [FengLi] Jin Ka Wang (C)

	 || crc == 0xb3d11a3c		// BBK bios V1.0
	 || crc == 0x9c7d4833		// BBK bios V2.0

	 || crc == 0xd5d6eac4 ) {	// EDU Computer (C)
		SetExController( EXCONTROLLER_Subor_KEYBOARD );
		nes->SetVideoMode( 2 );
	}

	if( crc == 0xAC7E98FB		// [PYRAMID] PEC-586 Computer & Game (C)_NoTapeOut
	 || crc == 0x6EEF8BB7		// [PYRAMID] PEC-586 Computer & Game (C)
	 || crc == 0x05F958C4		// [PYRAMID] PEC-686F Computer & Game (C)
	 || crc == 0xFE31765B		// [PYRAMID] PEC-9588 Computer & Game (C)
	 || crc == 0x42F89EFD ) {	//
		SetExController( EXCONTROLLER_PEC_KEYBOARD );
	}

	if( crc == 0xAC11B570		// [Kingwon] Jin Ka V5.0 (C)
	 || crc == 0x8D51A23B		// [KeWang] Chao Ji Wu Bi Han Ka (C) V1
	 || crc == 0x2CD32DB0		// [KeWang] Dian Nao Xue Xi Ka III (C)
	 || crc == 0xE4284379 ) {	// [PATTER] 30IN1 Han Ka Wang (C)
		SetExController( EXCONTROLLER_Kingwon_KEYBOARD );
		nes->SetVideoMode( 2 );
	}

	if( crc == 0x6310DDE6		// [ZeCheng] Chao Ji Xue Xi Ka (C)
	 || crc == 0x25C76773		// [KeWang] Chao Ji Wu Bi Han Ka (C) V2
	 || crc == 0x428C1C1D		// [FengLi] Zhong Ying Wen Dian Nao Ka (C)
	 || crc == 0x836CDDEF		// [BaTong] Zhong Ying Wen Dian Nao Ka (C)
	 || crc == 0x44479A27		// [BiTe] Dian Nao Xue Xi Ka (C)
	 || crc == 0xB93F00D3 ) {	// [XinKe] Tiao Zhan Han Ka (C)
		SetExController( EXCONTROLLER_ZeCheng_KEYBOARD );
		nes->SetVideoMode( 2 );
	}

	if( crc == 0x6058DB1C		// [Subor] Subor V15.0 (C) [for KeyBoard]
	 || crc == 0xB72B17ED		// Xue Sheng Dian Nao (C) [for KeyBoard]
	 || crc == 0x8B23F1AA		// Duo Mei Ti Shu Biao Xue Xi Ka (C)
	 || crc == 0x85FD927E		// [BenLi] Jing Pin Yi Zu - Mouse Card (C)
	 || crc == 0x488482c0
	 || crc == 0x3BD46A90		// Game Star - Smart Genius (Unl) P1 2048K
	 || crc == 0xAD82BBEA ) {	// Game Star - Smart Genius (Unl)
		SetExController( EXCONTROLLER_CHINA_EDUCATIONAL_MOUSE );
	}

	if( crc == 0x6085fee8	//YX_V9.0-98
	 || crc == 0xdf730735	//9.2F+YXNet.NES
	 || crc == 0x7fb3a9aa
	 || crc == 0xd1f6b0fb
	 || crc == 0x38D982E3 ) {
		SetExController( EXCONTROLLER_YuXing_Mouse );
	}

}

void	PAD::SetExController( INT type )
{
	excontroller_select = type;

	DELETEPTR( expad );

	bZapperMode = FALSE;
	DirectDraw.SetZapperMode( FALSE );
	DirectDraw.SetZapperDrawMode( FALSE );

	// ExPad Instance create
	switch( type ) {
		case	EXCONTROLLER_ZAPPER:
			expad = new EXPAD_Zapper( nes );
			bZapperMode = TRUE;
			DirectDraw.SetZapperMode( TRUE );
			DirectDraw.SetZapperDrawMode( TRUE );
			break;
		case	EXCONTROLLER_PADDLE:
			expad = new EXPAD_Paddle( nes );
			DirectDraw.SetZapperMode( TRUE );
			break;
		case	EXCONTROLLER_HYPERSHOT:
			expad = new EXPAD_HyperShot( nes );
			break;
		case	EXCONTROLLER_KEYBOARD:
			expad = new EXPAD_Keyboard( nes );
			break;
		case	EXCONTROLLER_Subor_KEYBOARD:
			expad = new EXPAD_Subor_Keyboard( nes );
			break;
		case	EXCONTROLLER_PEC_KEYBOARD:
			expad = new EXPAD_PEC_Keyboard( nes );
			break;
		case	EXCONTROLLER_Kingwon_KEYBOARD:
			expad = new EXPAD_Kingwon_Keyboard( nes );
			break;
		case	EXCONTROLLER_ZeCheng_KEYBOARD:
			expad = new EXPAD_ZeCheng_Keyboard( nes );
			break;
		case	EXCONTROLLER_CRAZYCLIMBER:
			expad = new EXPAD_CrazyClimber( nes );
			break;
		case	EXCONTROLLER_TOPRIDER:
			expad = new EXPAD_Toprider( nes );
			break;
		case	EXCONTROLLER_SPACESHADOWGUN:
			expad = new EXPAD_SpaceShadowGun( nes );
			bZapperMode = TRUE;
			DirectDraw.SetZapperMode( TRUE );
			DirectDraw.SetZapperDrawMode( TRUE );
			break;

		case	EXCONTROLLER_FAMILYTRAINER_A:
		case	EXCONTROLLER_FAMILYTRAINER_B:
			expad = new EXPAD_FamlyTrainer( nes );
			break;
		case	EXCONTROLLER_EXCITINGBOXING:
			expad = new EXPAD_ExcitingBoxing( nes );
			break;
		case	EXCONTROLLER_MAHJANG:
			expad = new EXPAD_Mahjang( nes );
			break;
		case	EXCONTROLLER_OEKAKIDS_TABLET:
			expad = new EXPAD_OekakidsTablet( nes );
			DirectDraw.SetZapperMode( TRUE );
			DirectDraw.SetZapperDrawMode( FALSE );
			break;
		case	EXCONTROLLER_TURBOFILE:
			expad = new EXPAD_TurboFile( nes );
			break;
		case	EXCONTROLLER_CHINA_EDUCATIONAL_MOUSE:
			expad = new EXPAD_ChinaEduMouse( nes );
			DirectDraw.SetZapperMode( TRUE );
			DirectDraw.SetZapperDrawMode( TRUE );
			break;

		case	EXCONTROLLER_YuXing_Mouse:
			expad = new EXPAD_YuXing_Mouse( nes );
			DirectDraw.SetZapperMode( TRUE );
			DirectDraw.SetZapperDrawMode( TRUE );
			break;

		case	EXCONTROLLER_VSUNISYSTEM:
			expad = new EXPAD_VSUnisystem( nes );
			break;
		case	EXCONTROLLER_VSZAPPER:
			expad = new EXPAD_VSZapper( nes );
			bZapperMode = TRUE;
			DirectDraw.SetZapperMode( TRUE );
			DirectDraw.SetZapperDrawMode( TRUE );
			break;

		case	EXCONTROLLER_YuXingMouse:
			expad = new EXPAD_YuXingMouse( nes );
			break;

		default:
			break;
	}

	if( expad ) {
		expad->Reset();
	}
}

DWORD	PAD::GetSyncData()
{
DWORD	ret;

	ret = (DWORD)padbit[0]|((DWORD)padbit[1]<<8)|((DWORD)padbit[2]<<16)|((DWORD)padbit[3]<<24);
	ret |= (DWORD)micbit<<8;

	return	ret;
}

void	PAD::SetSyncData( DWORD data )
{
	micbit = (BYTE)((data&0x00000400)>>8);
	padbit[0] = (BYTE) data;
	padbit[1] = (BYTE)(data>> 8);
	padbit[2] = (BYTE)(data>>16);
	padbit[3] = (BYTE)(data>>24);
}

DWORD	PAD::GetSyncExData()
{
DWORD	data = 0;

	switch( excontroller_select ) {
		case	EXCONTROLLER_ZAPPER:
		case	EXCONTROLLER_PADDLE:
		case	EXCONTROLLER_SPACESHADOWGUN:
		case	EXCONTROLLER_OEKAKIDS_TABLET:
		case	EXCONTROLLER_CHINA_EDUCATIONAL_MOUSE:
		case	EXCONTROLLER_YuXing_Mouse:
		case	EXCONTROLLER_VSZAPPER:
		case	EXCONTROLLER_YuXingMouse:
			{
			LONG	x, y;
				x = expad->GetSyncData( 0 );
				y = expad->GetSyncData( 1 );
				if( x == -1 || y == -1 ) {
					data = 0x80000000;
				} else {
					data = (x&0xFF)|((y&0xFF)<<8);
				}
			}
			if( excontroller_select != EXCONTROLLER_SPACESHADOWGUN ) {
				if( expad->GetSyncData( 2 ) )
					data |= 0x0010000;
			} else {
				data |= (DWORD)expad->GetSyncData( 2 )<<16;
			}
			break;
		case	EXCONTROLLER_CRAZYCLIMBER:
			data = (DWORD)expad->GetSyncData( 0 );
			break;
		case	EXCONTROLLER_TOPRIDER:
			data = (DWORD)expad->GetSyncData( 0 );
			break;
		case	EXCONTROLLER_FAMILYTRAINER_A:
		case	EXCONTROLLER_FAMILYTRAINER_B:
			data = (DWORD)expad->GetSyncData( 0 );
			break;
		case	EXCONTROLLER_EXCITINGBOXING:
			data = (DWORD)expad->GetSyncData( 0 );
			break;
		case	EXCONTROLLER_MAHJANG:
			data = (DWORD)expad->GetSyncData( 0 );
			break;

		default:
			break;
	}
	return	data;
}

void	PAD::SetSyncExData( DWORD data )
{
//DEBUGOUT( "PAD::SetSyncExData\n" );
	switch( excontroller_select ) {
		case	EXCONTROLLER_ZAPPER:
		case	EXCONTROLLER_PADDLE:
		case	EXCONTROLLER_SPACESHADOWGUN:
		case	EXCONTROLLER_OEKAKIDS_TABLET:
		case	EXCONTROLLER_CHINA_EDUCATIONAL_MOUSE:
		case	EXCONTROLLER_YuXing_Mouse:
		case	EXCONTROLLER_VSZAPPER:
		case	EXCONTROLLER_YuXingMouse:
			{
			LONG	x, y;
				if( data & 0x80000000 ) {
					x = -1;
					y = -1;
				} else {
					x = data & 0xFF;
					y = (data&0xFF00)>>8;
				}
				expad->SetSyncData( 0, x );
				expad->SetSyncData( 1, y );
				nes->SetZapperPos( x, y );
				DirectDraw.SetZapperPos( x, y );
			}
			if( excontroller_select != EXCONTROLLER_SPACESHADOWGUN ) {
				if( data & 0x0010000 )
					expad->SetSyncData( 2, 1 );
				else
					expad->SetSyncData( 2, 0 );
			} else {
				expad->SetSyncData( 2, (BYTE)(data>>16) );
			}
			break;
		case	EXCONTROLLER_CRAZYCLIMBER:
			expad->SetSyncData( 0, (LONG)data );
			break;
		case	EXCONTROLLER_TOPRIDER:
			expad->SetSyncData( 0, (LONG)data );
			break;
		case	EXCONTROLLER_FAMILYTRAINER_A:
		case	EXCONTROLLER_FAMILYTRAINER_B:
			expad->SetSyncData( 0, (LONG)data );
			break;
		case	EXCONTROLLER_EXCITINGBOXING:
			expad->SetSyncData( 0, (LONG)data );
			break;
		case	EXCONTROLLER_MAHJANG:
			expad->SetSyncData( 0, (LONG)data );
			break;
		default:
			break;
	}
}

void	PAD::Sync()
{
	padbit[0] = SyncSub( 0 );
	padbit[1] = SyncSub( 1 );
	padbit[2] = SyncSub( 2 );
	padbit[3] = SyncSub( 3 );

	// Mic
	micbit = 0;
	if( Config.ButtonCheck( 1, 10 ) ) micbit |= 4;

	// For Excontroller
	if( expad ) {
		expad->Sync();
	}

	// For NSF
	NsfSub();
}

void	PAD::VSync()
{
	padbitsync[0] = padbit[0];
	padbitsync[1] = padbit[1];
	padbitsync[2] = padbit[2];
	padbitsync[3] = padbit[3];
	micbitsync = micbit;
}

static	INT	ren30fps[] = {
	1, 0
};
static	INT	ren20fps[] = {
	1, 1, 0
};
static	INT	ren15fps[] = {
	1, 1, 0, 0
};
static	INT	ren10fps[] = {
	1, 1, 1, 0, 0, 0
};

static	INT	renmask[] = {
	6, 4, 3, 2,
};
static	INT*	rentbl[] = {
	ren10fps, ren15fps, ren20fps, ren30fps
};

BYTE	PAD::SyncSub( INT no )
{
WORD	bit = 0;

	// Up
	if( Config.ButtonCheck( no, 0 ) ) bit |= 1<<4;
	// Down
	if( Config.ButtonCheck( no, 1 ) ) bit |= 1<<5;
	// Left
	if( Config.ButtonCheck( no, 2 ) ) bit |= 1<<6;
	// Right
	if( Config.ButtonCheck( no, 3 ) ) bit |= 1<<7;

	// “¯Žž“ü—Í‚ð‹ÖŽ~‚·‚é
	if( (bit&((1<<4)|(1<<5))) == ((1<<4)|(1<<5)) )
		bit &= ~((1<<4)|(1<<5));
	if( (bit&((1<<6)|(1<<7))) == ((1<<6)|(1<<7)) )
		bit &= ~((1<<6)|(1<<7));

	// A
	if( Config.ButtonCheck( no, 4 ) ) bit |= 1<<0;
	// B
	if( Config.ButtonCheck( no, 5 ) ) bit |= 1<<1;

	// A,B Rapid
	if( Config.ButtonCheck( no, 6 ) ) bit |= 1<<8;
	if( Config.ButtonCheck( no, 7 ) ) bit |= 1<<9;

	// Select
	if( Config.ButtonCheck( no, 8 ) ) bit |= 1<<2;
	// Start
	if( Config.ButtonCheck( no, 9 ) ) bit |= 1<<3;

	// A rapid setup
	if( bit&(1<<8) ) {
		INT	spd = Config.controller.nRapid[no][0];
		if( spd >= 3 ) spd = 3;
		INT*	tbl = rentbl[spd];

		if( padcnt[no][0] >= renmask[spd] )
			padcnt[no][0] = 0;

		if( tbl[padcnt[no][0]] )
			bit |= (1<<0);
		else
			bit &= ~(1<<0);

		padcnt[no][0]++;
	} else {
		padcnt[no][0] = 0;
	}
	// B rapid setup
	if( bit&(1<<9) ) {
		INT	spd = Config.controller.nRapid[no][1];
		if( spd >= 3 ) spd = 3;
		INT*	tbl = rentbl[spd];

		if( padcnt[no][1] >= renmask[spd] )
			padcnt[no][1] = 0;

		if( tbl[padcnt[no][1]] )
			bit |= (1<<1);
		else
			bit &= ~(1<<1);

		padcnt[no][1]++;
	} else {
		padcnt[no][1] = 0;
	}

	return	(BYTE)(bit&0xFF);
}

void	PAD::Strobe()
{
	// For VS-Unisystem
	if( nes->rom->IsVSUNISYSTEM() ) {
		DWORD	pad1 = (DWORD)padbitsync[0] & 0xF3;
		DWORD	pad2 = (DWORD)padbitsync[1] & 0xF3;
		DWORD	st1  = ((DWORD)padbitsync[0] & 0x08)>>3;
		DWORD	st2  = ((DWORD)padbitsync[1] & 0x08)>>3;

		switch( nVSSwapType ) {
			case	VS_TYPE0:
				pad1bit = pad1 | (st1<<2);
				pad2bit = pad2 | (st2<<2);
				break;
			case	VS_TYPE1:
				pad1bit = pad2 | (st1<<2);
				pad2bit = pad1 | (st2<<2);
				break;
			case	VS_TYPE2:
				pad1bit = pad1 | (st1<<2) | (st2<<3);
				pad2bit = pad2;
				break;
			case	VS_TYPE3:
				pad1bit = pad2 | (st1<<2) | (st2<<3);
				pad2bit = pad1;
				break;
			case	VS_TYPE4:
				pad1bit = pad1 | (st1<<2) | 0x08;	// 0x08=Start Protect
				pad2bit = pad2 | (st2<<2) | 0x08;	// 0x08=Start Protect
				break;
			case	VS_TYPE5:
				pad1bit = pad2 | (st1<<2) | 0x08;	// 0x08=Start Protect
				pad2bit = pad1 | (st2<<2) | 0x08;	// 0x08=Start Protect
				break;
			case	VS_TYPE6:
				pad1bit = pad1 | (st1<<2) | (((DWORD)padbitsync[0] & 0x04)<<1);
				pad2bit = pad2 | (st2<<2) | (((DWORD)padbitsync[1] & 0x04)<<1);
				break;
			case	VS_TYPEZ:
				pad1bit = 0;
				pad2bit = 0;
				break;
		}

		// Coin 2‚Æ”í‚éˆ×‚ÉÁ‚·
		micbit  = 0;
	} else {
		if( Config.emulator.bFourPlayer ) {
		// NES type
			pad1bit = (DWORD)padbitsync[0] | ((DWORD)padbitsync[2]<<8) | 0x00080000;
			pad2bit = (DWORD)padbitsync[1] | ((DWORD)padbitsync[3]<<8) | 0x00040000;
		} else {
		// Famicom type
			pad1bit = (DWORD)padbitsync[0];
			pad2bit = (DWORD)padbitsync[1];
		}
	}
	pad3bit = (DWORD)padbitsync[2];
	pad4bit = (DWORD)padbitsync[3];
}

BYTE	PAD::Read( WORD addr )
{
BYTE	data = 0x00;

	if( addr == 0x4016 ) {
		data = (BYTE)pad1bit&1;
		pad1bit>>=1;
		data |= ((BYTE)pad3bit&1)<<1;
		pad3bit>>=1;
		// Mic
		if( !nes->rom->IsVSUNISYSTEM() ) {
			data |= micbitsync;
		}
		if( expad ) {
			data |= expad->Read4016();
		}
	}
	if( addr == 0x4017 ) {
		data = (BYTE)pad2bit&1;
		pad2bit>>=1;
		data |= ((BYTE)pad4bit&1)<<1;
		pad4bit>>=1;

		if( expad ) {
			data |= expad->Read4017();
		}

		if( bBarcodeWorld ) {
			data |= nes->Barcode2();
		}
	}

//	DEBUGOUT( "Read - addr= %04x ; dat= %03x\n", addr, data );

	return	data;
}

void	PAD::Write( WORD addr, BYTE data )
{
	if( addr == 0x4016 ) {
		if( data&0x01 ) {
			bStrobe = TRUE;
		} else if( bStrobe ) {
			bStrobe = FALSE;

			Strobe();
			if( expad ) {
				expad->Strobe();
			}
		}

		if( expad ) {
			expad->Write4016( data );
		}
	}
	if( addr == 0x4017 ) {
		if( expad ) {
			expad->Write4017( data );
		}
	}

//	DEBUGOUT( "Write - addr= %04x ; dat= %03x\n", addr, data );

}

void	PAD::NsfSub()
{
	nsfbit = 0;

	// Play
	if( Config.ButtonCheck( 0, Config.controller.nNsfButton ) ) nsfbit |= 1<<0;
	// Stop
	if( Config.ButtonCheck( 1, Config.controller.nNsfButton ) ) nsfbit |= 1<<1;

	// Number -1
	if( Config.ButtonCheck( 2, Config.controller.nNsfButton ) ) nsfbit |= 1<<2;
	// Number +1
	if( Config.ButtonCheck( 3, Config.controller.nNsfButton ) ) nsfbit |= 1<<3;
	// Number -16
	if( Config.ButtonCheck( 4, Config.controller.nNsfButton ) ) nsfbit |= 1<<4;
	// Number +16
	if( Config.ButtonCheck( 5, Config.controller.nNsfButton ) ) nsfbit |= 1<<5;

	// “¯Žž“ü—Í‚ð‹ÖŽ~‚·‚é
	if( (nsfbit&((1<<2)|(1<<3))) == ((1<<2)|(1<<3)) )
		nsfbit &= ~((1<<2)|(1<<3));
	if( (nsfbit&((1<<4)|(1<<5))) == ((1<<4)|(1<<5)) )
		nsfbit &= ~((1<<4)|(1<<5));
}

#include "EXPAD_Zapper.cpp"
#include "EXPAD_Paddle.cpp"
#include "EXPAD_HyperShot.cpp"
#include "EXPAD_Keyboard.cpp"
#include "EXPAD_Subor_Keyboard.cpp"
#include "EXPAD_PEC_Keyboard.cpp"
#include "EXPAD_Kingwon_Keyboard.cpp"
#include "EXPAD_ZeCheng_Keyboard.cpp"
#include "EXPAD_YuXing_Mouse.cpp"
#include "EXPAD_CrazyClimber.cpp"
#include "EXPAD_Toprider.cpp"
#include "EXPAD_SpaceShadowGun.cpp"

#include "EXPAD_FamlyTrainer.cpp"
#include "EXPAD_ExcitingBoxing.cpp"
#include "EXPAD_Mahjang.cpp"
#include "EXPAD_OekakidsTablet.cpp"
#include "EXPAD_TurboFile.cpp"

#include "EXPAD_VSUnisystem.cpp"
#include "EXPAD_VSZapper.cpp"

#include "EXPAD_ChinaEduMouse.cpp"
#include "EXPAD_YuXingMouse.cpp"