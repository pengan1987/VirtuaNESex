//////////////////////////////////////////////////////////////////////////
//                                                                      //
//      NES Memory Management Unit                                      //
//                                                           Norix      //
//                                               written     2001/02/21 //
//                                               last modify ----/--/-- //
//////////////////////////////////////////////////////////////////////////

#ifndef	__MMU_INCLUDED__
#define	__MMU_INCLUDED__

#include "typedef.h"
#include "macro.h"

extern	BYTE	nnn;

// CPU メモリバンク
extern	LPBYTE	CPU_MEM_BANK[8];	// 8K単位
extern	BYTE	CPU_MEM_TYPE[8];
extern	INT	CPU_MEM_PAGE[8];	// ステートセーブ用

// PPU メモリバンク
extern	LPBYTE	PPU_MEM_BANK[12];	// 1K単位
extern	BYTE	PPU_MEM_TYPE[12];
extern	INT		PPU_MEM_PAGE[12];	// ステートセーブ用
extern	BYTE	CRAM_USED[16];		// ステートセーブ用
extern	PBYTE	VROM_WRITED;		// for mapper 74

// NESメモリ
extern	BYTE	RAM [  8*1024];		// NES内臓RAM
extern	BYTE	WRAM[128*1024];		// ワーク/バックアップRAM
extern	BYTE	DRAM[ 40*1024];		// ディスクシステムRAM
extern	BYTE	XRAM[  8*1024];		// ダミーバンク
extern	BYTE	ERAM[ 32*1024];		// 拡張機器用RAM

extern	BYTE	VRAM[  4*1024];		// ネームテーブル/アトリビュートRAM
extern	BYTE	CRAM[ 32*1024];		// キャラクタパターンRAM

extern	BYTE	YWRAM[1024*1024];	// for YuXing 98/F 1024K PRam
extern	BYTE	YSRAM[  32*1024];	// for YuXing 98/F 32K SRam
extern	BYTE	YCRAM[ 128*1024];	// for YuXing 98/F 128K CRam

extern	BYTE	BDRAM[ 512*1024];	// for BBK 512K PRam
extern	BYTE	BSRAM[  32*1024];	// for BBK 32K SRam
extern	BYTE	BCRAM[ 512*1024];	// for BBK 512K CRam

extern	BYTE	JDRAM[ 512*1024];	// for DrPCJr 512K PRam
extern	BYTE	JSRAM[   8*1024];	// for DrPCJr 8K SRam
extern	BYTE	JCRAM[ 512*1024];	// for DrPCJr 512K CRam

extern	BYTE	tempRAM[ 4*1024];

extern	BYTE	WAVRAM[256];

extern	BYTE	SPRAM[0x100];		// スプライトRAM
extern	BYTE	BGPAL[0x10];		// BGパレット
extern	BYTE	SPPAL[0x10];		// SPパレット

// レジスタ
extern	BYTE	CPUREG[0x18];		// Nes $4000-$4017
extern	BYTE	PPUREG[0x04];		// Nes $2000-$2003

// Frame-IRQレジスタ($4017)
extern	BYTE	FrameIRQ;

// PPU内部レジスタ
extern	BYTE	PPU56Toggle;		// $2005-$2006 Toggle
extern	BYTE	PPU7_Temp;		// $2007 read buffer
extern	WORD	loopy_t;		// same as $2005/$2006
extern	WORD	loopy_v;		// same as $2005/$2006
extern	WORD	loopy_x;		// tile x offset

// ROMデータポインタ
extern	LPBYTE	PROM;		// PROM ptr
extern	LPBYTE	VROM;		// VROM ptr

#ifdef	_DATATRACE
// For dis...
extern	LPBYTE	PROM_ACCESS;
#endif

// ROM バンクサイズ
extern	INT	PROM_8K_SIZE, PROM_16K_SIZE, PROM_32K_SIZE;
extern	INT	VROM_1K_SIZE, VROM_2K_SIZE, VROM_4K_SIZE,  VROM_8K_SIZE;


// 関数
extern	void	NesSub_MemoryInitial();

extern	void	SetPROM_Bank( BYTE page, LPBYTE ptr, BYTE type );
extern	void	SetPROM_8K_Bank ( BYTE page, INT bank );
extern	void	SetPROM_16K_Bank( BYTE page, INT bank );
extern	void	SetPROM_32K_Bank( INT bank );
extern	void	SetPROM_32K_Bank( INT bank0, INT bank1, INT bank2, INT bank3 );

extern	void	SetVROM_Bank( BYTE page, LPBYTE ptr, BYTE type );
extern	void	SetVROM_1K_Bank( BYTE page, INT bank );
extern	void	SetVROM_2K_Bank( BYTE page, INT bank );
extern	void	SetVROM_4K_Bank( BYTE page, INT bank );
extern	void	SetVROM_8K_Bank( INT bank );
extern	void	SetVROM_8K_Bank( INT bank0, INT bank1, INT bank2, INT bank3,
				 INT bank4, INT bank5, INT bank6, INT bank7 );

extern	void	SetCRAM_1K_Bank( BYTE page, INT bank );
extern	void	SetCRAM_2K_Bank( BYTE page, INT bank );
extern	void	SetCRAM_4K_Bank( BYTE page, INT bank );
extern	void	SetCRAM_8K_Bank( INT bank );

extern	void	SetVRAM_1K_Bank( BYTE page, INT bank );
extern	void	SetVRAM_Bank( INT bank0, INT bank1, INT bank2, INT bank3 );
extern	void	SetVRAM_Mirror( INT type );
extern	void	SetVRAM_Mirror( INT bank0, INT bank1, INT bank2, INT bank3 );

// for YuXing 98/F 1024K PRam
extern	void	SetYWRAM_8K_Bank ( BYTE page, INT bank );
extern	void	SetYWRAM_16K_Bank( BYTE page, INT bank );
extern	void	SetYWRAM_32K_Bank( INT bank );
extern	void	SetYWRAM_32K_Bank( INT bank0, INT bank1, INT bank2, INT bank3 );
// for YuXing 98/F 128K CRam
extern	void	SetYCRAM_1K_Bank( BYTE page, INT bank );
extern	void	SetYCRAM_2K_Bank( BYTE page, INT bank );
extern	void	SetYCRAM_4K_Bank( BYTE page, INT bank );
extern	void	SetYCRAM_8K_Bank( INT bank );

extern	void	SetBDRAM_8K_Bank( BYTE page, INT bank );
extern	void	SetBDRAM_16K_Bank( BYTE page, INT bank );
extern	void	SetBDRAM_32K_Bank( INT bank );

extern	void	SetPROM_4K_Bank( WORD addr, INT bank );

extern	void	SetJCRAM_1K_Bank( BYTE page, INT bank );
extern	void	SetJCRAM_2K_Bank( BYTE page, INT bank );
extern	void	SetJCRAM_4K_Bank( BYTE page, INT bank );
extern	void	SetJCRAM_8K_Bank( INT bank );
extern	void	SetJCRAM_8K_Bank( INT bank0, INT bank1, INT bank2, INT bank3,
				 INT bank4, INT bank5, INT bank6, INT bank7 );
extern	void	SetJDRAM_8K_Bank ( BYTE page, INT bank );
extern	void	SetJDRAM_32K_Bank( INT bank );

extern	void	SetOBCRAM_1K_Bank( BYTE page, INT bank );


// メモリタイプ
// For PROM (CPU)
#define	BANKTYPE_ROM	0x00
#define	BANKTYPE_RAM	0xFF
#define	BANKTYPE_DRAM	0x01
#define	BANKTYPE_MAPPER	0x80
// For VROM/VRAM/CRAM (PPU)
#define	BANKTYPE_VROM	0x00
#define	BANKTYPE_CRAM	0x01
#define	BANKTYPE_YCRAM	0x02
#define	BANKTYPE_JCRAM	0x03
#define	BANKTYPE_VRAM	0x80

// ミラータイプ
#define	VRAM_HMIRROR	0x00	// Horizontal
#define	VRAM_VMIRROR	0x01	// Virtical
#define	VRAM_MIRROR4	0x02	// All screen
#define	VRAM_MIRROR4L	0x03	// PA10 L固定 $2000-$23FFのミラー
#define	VRAM_MIRROR4H	0x04	// PA10 H固定 $2400-$27FFのミラー
#define	VRAM_MIRROR3H	0x05

#endif	// !__MMU_INCLUDED__

