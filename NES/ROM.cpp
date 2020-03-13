//////////////////////////////////////////////////////////////////////////
//                                                                      //
//      NES ROM Cartridge class                                         //
//                                                           Norix      //
//                                               written     2001/02/20 //
//                                               last modify ----/--/-- //
//////////////////////////////////////////////////////////////////////////
#define	WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <mbstring.h>

#include "typedef.h"
#include "macro.h"

#include "VirtuaNESres.h"

#include "DebugOut.h"
#include "App.h"
#include "Plugin.h"
#include "Pathlib.h"
#include "Crclib.h"

#include "Archive.h"

#include "rom.h"
#include "romdb.h"
#include "mmu.h"
#include "mapper.h"

const char* img_fname;	//for bbk

BOOL g_bSan2;
unsigned char pFont[256*1024];

INT	 g_UnfTVMode = -1;
#define MKID(a) ((unsigned long) \
	(((a) >> 24) & 0x000000FF) | \
	(((a) >>  8) & 0x0000FF00) | \
	(((a) <<  8) & 0x00FF0000) | \
	(((a) << 24) & 0xFF000000))

//
// 僐儞僗僩儔僋僞
//
ROM::ROM( const char* fname )
{
FILE	*fp = NULL;
LPBYTE	temp = NULL;
LPBYTE	bios = NULL;
LONG	FileSize;

	g_bSan2 = FALSE;

	ZEROMEMORY( &header, sizeof(header) );
	ZEROMEMORY( path, sizeof(path) );
	ZEROMEMORY( name, sizeof(name) );

	bPAL = FALSE;
	bNSF = FALSE;
	NSF_PAGE_SIZE = 0;

	board = 0;
	bUnif = FALSE;
	g_UnfTVMode = -1;

	lpPRG = lpCHR = lpTrainer = lpDiskBios = lpDisk = NULL;

	crc = crcall = 0;
	mapper = 0;
	diskno = 0;

	try {

//for bbk-------------------------------------------------------------------------
		if( !(fp = ::fopen( fname, "rb" )) ) {
			LPCSTR	szErrStr = CApp::GetErrorString( IDS_ERROR_OPEN );
			::wsprintf( szErrorString, szErrStr, fname );
			throw	szErrorString;
		}
		::fseek( fp, 0, SEEK_END );
		FileSize = ::ftell( fp );
		::fseek( fp, 0, SEEK_SET );
		if( !(temp = (LPBYTE)::malloc( FileSize )) ) {
			throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
		}
		if( ::fread( temp, FileSize, 1, fp ) != 1 ) {
			throw	CApp::GetErrorString( IDS_ERROR_READ );
		}
		FCLOSE( fp );
		::memcpy( &header, temp, sizeof(NESHEADER) );

		if( header.ID[0] == 0xEB && header.ID[1] == 0x3C
		 && header.ID[2] == 0x90 && header.ID[3] == 0x53 ) {
			img_fname = fname;
//			DEBUGOUT( "img_fname: %s\n", img_fname );
			fname = "bbk_bios10.rom";
		} else {
			FREE( temp );
		}
//--------------------------------------------------------------------------------

		if( !(fp = ::fopen( fname, "rb" )) ) {
			// xxx 僼傽僀儖傪奐偗傑偣傫
			LPCSTR	szErrStr = CApp::GetErrorString( IDS_ERROR_OPEN );
			::wsprintf( szErrorString, szErrStr, fname );
			throw	szErrorString;
		}
		// 僼傽僀儖僒僀僘庢摼
		::fseek( fp, 0, SEEK_END );
		FileSize = ::ftell( fp );
		::fseek( fp, 0, SEEK_SET );
		// 僼傽僀儖僒僀僘僠僃僢僋(NES僿僢僟+1僶僀僩埲忋偐丠)
		if( FileSize < 17 ) {
			// 僼傽僀儖僒僀僘偑彫偝偡偓傑偡
			throw	CApp::GetErrorString( IDS_ERROR_SMALLFILE );
		}

		// 僥儞億儔儕儊儌儕妋曐
		if( !(temp = (LPBYTE)::malloc( FileSize )) ) {
			// 儊儌儕傪妋曐弌棃傑偣傫
			throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
		}

		// 僒僀僘暘撉傒崬傒
		if( ::fread( temp, FileSize, 1, fp ) != 1 ) {
			// 僼傽僀儖偺撉傒崬傒偵幐攕偟傑偟偨
			throw	CApp::GetErrorString( IDS_ERROR_READ );
		}

		FCLOSE( fp );

		// 僿僢僟僐僺乕
		::memcpy( &header, temp, sizeof(NESHEADER) );

		header.CHR_PAGE_SIZE = header.CHR_PAGE_SIZE_old;
		header.PRG_PAGE_SIZE = header.PRG_PAGE_SIZE_old;

		if( header.ID[0] == 'N' && header.ID[1] == 'E'
		 && header.ID[2] == 'S' && header.ID[3] == 0x1A ) {
			// 僿僢僟僐僺乕
			memcpy( &header, temp, sizeof(NESHEADER) );
			header.CHR_PAGE_SIZE = header.CHR_PAGE_SIZE_old;
			header.PRG_PAGE_SIZE = header.PRG_PAGE_SIZE_old;
		} else if( header.ID[0] == 'F' && header.ID[1] == 'D'
			&& header.ID[2] == 'S' && header.ID[3] == 0x1A ) {
			// 僿僢僟僐僺乕
			memcpy( &header, temp, sizeof(NESHEADER) );
			header.CHR_PAGE_SIZE = header.CHR_PAGE_SIZE_old;
			header.PRG_PAGE_SIZE = header.PRG_PAGE_SIZE_old;
		} else if( header.ID[0] == 'N' && header.ID[1] == 'E'
			&& header.ID[2] == 'S' && header.ID[3] == 'M') {
			// 僿僢僟僐僺乕
			memcpy( &header, temp, sizeof(NESHEADER) );
			header.CHR_PAGE_SIZE = header.CHR_PAGE_SIZE_old;
			header.PRG_PAGE_SIZE = header.PRG_PAGE_SIZE_old;
		} else if( header.ID[0] == 'U' && header.ID[1] == 'N'
			&& header.ID[2] == 'I' && header.ID[3] == 'F') {
			//处理Unif ROM	
		} else {
			FREE( temp );

			if( !UnCompress( fname, &temp, (LPDWORD)&FileSize ) ) {
				// 枹懳墳宍幃偱偡
				throw	CApp::GetErrorString( IDS_ERROR_UNSUPPORTFORMAT );
			}
			// 僿僢僟僐僺乕
			::memcpy( &header, temp, sizeof(NESHEADER) );
			header.CHR_PAGE_SIZE = header.CHR_PAGE_SIZE_old;
			header.PRG_PAGE_SIZE = header.PRG_PAGE_SIZE_old;
		}

		DWORD	PRGoffset, CHRoffset;
		LONG	PRGsize, CHRsize;
		BYTE *pUnif = temp;
		DWORD filesize = FileSize;

		if( header.ID[0] == 'N' && header.ID[1] == 'E'
		 && header.ID[2] == 'S' && header.ID[3] == 0x1A ) {
		// 晛捠偺NES僼傽僀儖
			PRGsize = (LONG)header.PRG_PAGE_SIZE*0x4000;
			CHRsize = (LONG)header.CHR_PAGE_SIZE*0x2000;
			PRGoffset = sizeof(NESHEADER);
			CHRoffset = PRGoffset + PRGsize;

			//for Game Star - Smart Genius (Unl)
			if(header.PRG_PAGE_SIZE==0xff)
				PRGsize = (LONG)(header.PRG_PAGE_SIZE+1)*0x4000;

			if( IsTRAINER() ) {
				PRGoffset += 512;
				CHRoffset += 512;
			}

			if( PRGsize <= 0 || (PRGsize+CHRsize) > FileSize ) {
				// NES僿僢僟偑堎忢偱偡
				throw	CApp::GetErrorString( IDS_ERROR_INVALIDNESHEADER );
			}

			// PRG BANK
			if( !(lpPRG = (LPBYTE)malloc( PRGsize )) ) {
				// 儊儌儕傪妋曐弌棃傑偣傫
				throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
			}

			::memcpy( lpPRG, temp+PRGoffset, PRGsize );

			// CHR BANK
			if( CHRsize > 0 ) {
				if( !(lpCHR = (LPBYTE)malloc( CHRsize )) ) {
					// 儊儌儕傪妋曐弌棃傑偣傫
					throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
				}

				if( FileSize >= CHRoffset+CHRsize ) {
					memcpy( lpCHR, temp+CHRoffset, CHRsize );
				} else {
					// CHR僶儞僋彮側偄乧
					CHRsize -= (CHRoffset+CHRsize - FileSize);
					memcpy( lpCHR, temp+CHRoffset, CHRsize );
				}
			} else {
				lpCHR = NULL;
			}

			// Trainer
			if( IsTRAINER() ) {
				if( !(lpTrainer = (LPBYTE)malloc( 512 )) ) {
					// 儊儌儕傪妋曐弌棃傑偣傫
					throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
				}

				memcpy( lpTrainer, temp+sizeof(NESHEADER), 512 );
			} else {
				lpTrainer = NULL;
			}

		} else if( header.ID[0] == 'U' && header.ID[1] == 'N'
			&& header.ID[2] == 'I' && header.ID[3] == 'F' ) {

			DWORD Signature, BlockLen;
			DWORD ipos =0x20;//跳过UNIF头
			BYTE id,i;
			BYTE *tPRG[0x10], *tCHR[0x10];
			DWORD sizePRG[0x10],sizeCHR[0x10];
			//char info[100];
			//char name[100];
			PRGsize = 0x00;
			CHRsize = 0x00;
			
			header.ID[0] = 'N';
			header.ID[1] = 'E';
			header.ID[2] = 'S';
			header.ID[3] = 0x1A;
			
			board = 0;
			bUnif = TRUE;


		//	header.PRG_PAGE_SIZE = (BYTE)diskno*4;
		//	header.CHR_PAGE_SIZE = 0;
		//	header.control1 = 0x40;
		//	header.control2 = 0x10;
			header.control1 = 0;
			header.control2 = 0;
			
			for (i = 0; i < 0x10; i++)
			{
				tPRG[i] = tCHR[i] = 0;
			}

			//filesize
			while(ipos<filesize)
			{
				id = 0;
				memcpy(&Signature,&pUnif[ipos],4);ipos+=4;
				memcpy(&BlockLen,&pUnif[ipos],4);ipos+=4;
				
				switch(Signature)
				{
					case MKID('MAPR')://board名字
						memcpy( pboardname, &pUnif[ipos], BlockLen);
						pboardname[BlockLen]=0;
						//memcpy( info, &pUnif[ipos], BlockLen);
						//fl.info = info;
						ipos+=BlockLen;	break;

					case MKID('NAME'):
						//memcpy( pboardname, &pUnif[ipos], BlockLen);
						//fl.title = name;
						ipos+=BlockLen;	break;

					case MKID('TVCI')://电视制式
						g_UnfTVMode = pUnif[ipos];
						ipos+=BlockLen;	break;

					case MKID('BATR')://使用电池记忆
						header.control1 |=2;
						ipos+=BlockLen;	break;						

					case MKID('FONT')://字库
//						memcpy( pFont, &pUnif[ipos], BlockLen>65536?65536:BlockLen );
						memcpy( pFont, &pUnif[ipos], BlockLen );
						ipos+=BlockLen;	break;
						
					case MKID('MIRR'):
						if (pUnif[ipos]==0)
							header.control1 &=14;
						else if (pUnif[ipos]==1)
							header.control1 |=1;
						ipos+=BlockLen;
						break;
					
					case MKID('PRGF'):	id++;
					case MKID('PRGE'):	id++;
					case MKID('PRGD'):	id++;
					case MKID('PRGC'):	id++;
					case MKID('PRGB'):	id++;
					case MKID('PRGA'):	id++;
					case MKID('PRG9'):	id++;
					case MKID('PRG8'):	id++;
					case MKID('PRG7'):	id++;
					case MKID('PRG6'):	id++;
					case MKID('PRG5'):	id++;
					case MKID('PRG4'):	id++;
					case MKID('PRG3'):	id++;
					case MKID('PRG2'):	id++;
					case MKID('PRG1'):	id++;
					case MKID('PRG0'):
						sizePRG[id] = BlockLen;
						tPRG[id] = (BYTE*)malloc(BlockLen);
						memcpy( tPRG[id], &pUnif[ipos], BlockLen );
						ipos+=BlockLen;
						PRGsize += BlockLen;
						break;

					case MKID('CHRF'):	id++;
					case MKID('CHRE'):	id++;
					case MKID('CHRD'):	id++;
					case MKID('CHRC'):	id++;
					case MKID('CHRB'):	id++;
					case MKID('CHRA'):	id++;
					case MKID('CHR9'):	id++;
					case MKID('CHR8'):	id++;
					case MKID('CHR7'):	id++;
					case MKID('CHR6'):	id++;
					case MKID('CHR5'):	id++;
					case MKID('CHR4'):	id++;
					case MKID('CHR3'):	id++;
					case MKID('CHR2'):	id++;
					case MKID('CHR1'):	id++;
					case MKID('CHR0'):
						sizeCHR[id] = BlockLen;
						tCHR[id] = (BYTE*)malloc(BlockLen);
						memcpy( tCHR[id], &pUnif[ipos], BlockLen );
						ipos+=BlockLen;
						CHRsize += BlockLen;
						break;
						
					default:
						ipos+=BlockLen;	break;
				}
			}

			//fl.mapper = 0;
			//fl.prg_size = 0;
			//fl.chr_size = 0;

			board = NES_ROM_get_unifBoardID(pboardname);
			
			header.PRG_PAGE_SIZE = PRGsize/(16*1024);
			header.CHR_PAGE_SIZE = CHRsize/(8*1024);

			DWORD LenPRG=0,LenCHR=0;
			if(PRGsize)
				lpPRG = (LPBYTE)malloc( PRGsize );
			if(CHRsize)
				lpCHR = (LPBYTE)malloc( CHRsize );

			for (i = 0; i < 16; i++)
			{
				if (tPRG[i])
				{
					memcpy(&lpPRG[LenPRG], tPRG[i], sizePRG[i]);
					LenPRG += sizePRG[i];
					//fl.prg_size  += sizePRG[i]>>14;
					//PRGsize = PRGsize+LenPRG;
					free(tPRG[i]);
				}
				if (tCHR[i])
				{
					memcpy(&lpCHR[LenCHR], tCHR[i], sizeCHR[i]);
					LenCHR += sizeCHR[i];
					//fl.chr_size = (fl.chr_size)+(sizeCHR[i]>>13);
					//CHRsize =  CHRsize+LenCHR;
					free(tCHR[i]);
				}
			}

		} else if( header.ID[0] == 'F' && header.ID[1] == 'D'
			&& header.ID[2] == 'S' && header.ID[3] == 0x1A ) {
		// FDS(Nintendo Disk System)
			// 僨傿僗僋僒僀僘
			diskno = header.PRG_PAGE_SIZE;

			if( FileSize < (16+65500*diskno) ) {
				// 僨傿僗僋僒僀僘偑堎忢偱偡
				throw	CApp::GetErrorString( IDS_ERROR_ILLEGALDISKSIZE );
			}
			if( diskno > 4 ) {
				// 4柺傛傝懡偄僨傿僗僋偼懳墳偟偰偄傑偣傫
				throw	CApp::GetErrorString( IDS_ERROR_UNSUPPORTDISK );
			}

			ZEROMEMORY( &header, sizeof(NESHEADER) );

			// 僟儈乕僿僢僟傪嶌傞
			header.ID[0] = 'N';
			header.ID[1] = 'E';
			header.ID[2] = 'S';
			header.ID[3] = 0x1A;
			header.PRG_PAGE_SIZE = (BYTE)diskno*4;
			header.CHR_PAGE_SIZE = 0;
			header.control1 = 0x40;
			header.control2 = 0x10;

			PRGsize = sizeof(NESHEADER)+65500*(LONG)diskno;
			// PRG BANK
			if( !(lpPRG = (LPBYTE)malloc( PRGsize )) ) {
				// 儊儌儕傪妋曐弌棃傑偣傫
				throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
			}
			// 僨乕僞偺僶僢僋傾僢僾梡
			if( !(lpDisk = (LPBYTE)malloc( PRGsize )) ) {
				// 儊儌儕傪妋曐弌棃傑偣傫
				throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
			}
			// CHR BANK
			lpCHR = NULL;

			::memcpy( lpPRG, &header, sizeof(NESHEADER) );
			::memcpy( lpPRG+sizeof(NESHEADER), temp+sizeof(NESHEADER), 65500*(LONG)diskno );
			// 僨乕僞偺彂偒姺偊応強摿掕梡
			ZEROMEMORY( lpDisk, PRGsize );
//			memcpy( lpDisk, &header, sizeof(NESHEADER) );
//			memcpy( lpDisk+sizeof(NESHEADER), temp+sizeof(NESHEADER), PRGsize-sizeof(NESHEADER) );

			lpPRG[0] = 'F';
			lpPRG[1] = 'D';
			lpPRG[2] = 'S';
			lpPRG[3] = 0x1A;
			lpPRG[4] = (BYTE)diskno;

			// DISKSYSTEM BIOS偺儘乕僪
			string	Path = CPathlib::MakePathExt( CApp::GetModulePath(), "DISKSYS", "ROM" );

			if( !(fp = fopen( Path.c_str(), "rb" )) ) {
				// DISKSYS.ROM偑偁傝傑偣傫
				throw	CApp::GetErrorString( IDS_ERROR_NODISKBIOS );
			}

			::fseek( fp, 0, SEEK_END );
			FileSize = ::ftell( fp );
			::fseek( fp, 0, SEEK_SET );
			if( FileSize < 17 ) {
				// 僼傽僀儖僒僀僘偑彫偝偡偓傑偡
				throw	CApp::GetErrorString( IDS_ERROR_SMALLFILE );
			}
			if( !(bios = (LPBYTE)malloc( FileSize )) ) {
				// 儊儌儕傪妋曐弌棃傑偣傫
				throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
			}
			if( fread( bios, FileSize, 1, fp ) != 1 ) {
				// 僼傽僀儖偺撉傒崬傒偵幐攕偟傑偟偨
				throw	CApp::GetErrorString( IDS_ERROR_READ );
			}
			FCLOSE( fp );

			if( !(lpDiskBios = (LPBYTE)malloc( 8*1024 )) ) {
				// 儊儌儕傪妋曐弌棃傑偣傫
				throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
			}

			if( bios[0] == 'N' && bios[1] == 'E' && bios[2] == 'S' && bios[3] == 0x1A ) {
			// NES宍幃BIOS
				::memcpy( lpDiskBios, bios+0x6010, 8*1024 );
			} else {
			// 惗BIOS
				::memcpy( lpDiskBios, bios, 8*1024 );
			}
			FREE( bios );
		} else if( header.ID[0] == 'N' && header.ID[1] == 'E'
			&& header.ID[2] == 'S' && header.ID[3] == 'M') {
		// NSF
			bNSF = TRUE;
			ZEROMEMORY( &header, sizeof(NESHEADER) );

			// 僿僢僟僐僺乕
			memcpy( &nsfheader, temp, sizeof(NSFHEADER) );

			PRGsize = FileSize-sizeof(NSFHEADER);
//DEBUGOUT( "PRGSIZE:%d\n", PRGsize );
			PRGsize = (PRGsize+0x0FFF)&~0x0FFF;
//DEBUGOUT( "PRGSIZE:%d\n", PRGsize );
			if( !(lpPRG = (LPBYTE)malloc( PRGsize )) ) {
				// 儊儌儕傪妋曐弌棃傑偣傫
				throw	CApp::GetErrorString( IDS_ERROR_OUTOFMEMORY );
				throw	szErrorString;
			}
			ZEROMEMORY( lpPRG, PRGsize );
			memcpy( lpPRG, temp+sizeof(NSFHEADER), FileSize-sizeof(NSFHEADER) );

			NSF_PAGE_SIZE = PRGsize>>12;
//DEBUGOUT( "PAGESIZE:%d\n", NSF_PAGE_SIZE );
		} else {
			// 枹懳墳宍幃偱偡
			throw	CApp::GetErrorString( IDS_ERROR_UNSUPPORTFORMAT );
		}

		// 僷僗/僼傽僀儖柤庢摼
		{
		string	tempstr;
		tempstr = CPathlib::SplitPath( fname );
		::strcpy( path, tempstr.c_str() );
		tempstr = CPathlib::SplitFname( fname );
		::strcpy( name, tempstr.c_str() );
		// 僆儕僕僫儖僼傽僀儖柤(僼儖僷僗)
		::strcpy( fullpath, fname );
		}

		// 儅僢僷愝掕
		if( !bNSF ) {
			mapper = (header.control1>>4)|(header.control2&0xF0);
			crc = crcall = crcvrom = 0;

			if( mapper != 20 ) {
				// PRG crc偺寁嶼(NesToy偺PRG CRC偲摨偠)
				if( IsTRAINER() ) {
					crcall  = CRC::CrcRev( 512+PRGsize+CHRsize, temp+sizeof(NESHEADER) );
					crc     = CRC::CrcRev( 512+PRGsize, temp+sizeof(NESHEADER) );
					if( CHRsize )
						crcvrom = CRC::CrcRev( CHRsize, temp+PRGsize+512+sizeof(NESHEADER) );
				} else {
					crcall  = CRC::CrcRev( PRGsize+CHRsize, temp+sizeof(NESHEADER) );
					crc     = CRC::CrcRev( PRGsize, temp+sizeof(NESHEADER) );
					if( CHRsize )
						crcvrom = CRC::CrcRev( CHRsize, temp+PRGsize+sizeof(NESHEADER) );
				}

				FilenameCheck( name );

				romdatabase.HeaderCorrect( header, crcall, crc );

#include "ROM_Patch.cpp"
				fdsmakerID = fdsgameID = 0;
			} else {
				crc = crcall = crcvrom = 0;

				fdsmakerID = lpPRG[0x1F];
				fdsgameID  = (lpPRG[0x20]<<24)|(lpPRG[0x21]<<16)|(lpPRG[0x22]<<8)|(lpPRG[0x23]<<0);
			}

			if(bUnif){
				mapper = board;
				crc = CRC::CrcRev( PRGsize, lpPRG );
				if( CHRsize )
				crcvrom = CRC::CrcRev( CHRsize, lpCHR );
			}

		} else {
		// NSF
			mapper = 0x0100;	// Private mapper
			crc = crcall = crcvrom = 0;
			fdsmakerID = fdsgameID = 0;
		}

		FREE( temp );
	} catch( CHAR* str ) {
		// 尨場偑傢偐偭偰偄傞僄儔乕張棟
		FCLOSE( fp );
		FREE( temp );
		FREE( bios );

		FREE( lpPRG );
		FREE( lpCHR );
		FREE( lpTrainer );
		FREE( lpDiskBios );
		FREE( lpDisk );

		throw	str;
#ifndef	_DEBUG
	} catch(...) {
		// 堦斒曐岇僄儔乕偲偐弌偟偨偔柍偄偺偱...(^^;
		FCLOSE( fp );
		FREE( temp );
		FREE( bios );

		FREE( lpPRG );
		FREE( lpCHR );
		FREE( lpTrainer );
		FREE( lpDiskBios );
		FREE( lpDisk );

#ifdef	_DATATRACE
		// For dis...
		FREE( PROM_ACCESS );
#endif

		// 晄柧側僄儔乕偑敪惗偟傑偟偨
		throw	CApp::GetErrorString( IDS_ERROR_UNKNOWN );
#endif	// !_DEBUG
	}

}

//
// 僨僗僩儔僋僞
//
ROM::~ROM()
{
	FREE( lpPRG );
	FREE( lpCHR );
	FREE( lpTrainer );
	FREE( lpDiskBios );
	FREE( lpDisk );
}

//
// ROM僼傽僀儖僠僃僢僋
//
INT	ROM::IsRomFile( const char* fname )
{
FILE*	fp = NULL;
NESHEADER	header;

	if( !(fp = fopen( fname, "rb" )) )
		return	IDS_ERROR_OPEN;

	// 僒僀僘暘撉傒崬傒
	if( fread( &header, sizeof(header), 1, fp ) != 1 ) {
		FCLOSE( fp );
		return	IDS_ERROR_READ;
	}
	FCLOSE( fp );

	if( header.ID[0] == 'N' && header.ID[1] == 'E'
	 && header.ID[2] == 'S' && header.ID[3] == 0x1A ) {
		for( INT i = 0; i < 4; i++ ) {
			if( header.reserved[i] )
				return	IDS_ERROR_ILLEGALHEADER;
		}
		return	0;
	} else if( header.ID[0] == 'F' && header.ID[1] == 'D'
		&& header.ID[2] == 'S' && header.ID[3] == 0x1A ) {
		return	0;
	} else if( header.ID[0] == 'U' && header.ID[1] == 'N'
		&& header.ID[2] == 'I' && header.ID[3] == 'F' ) {
		return	0;
	} else if( header.ID[0] == 'N' && header.ID[1] == 'E'
		&& header.ID[2] == 'S' && header.ID[3] == 'M' ) {
		return	0;
	} else if( header.ID[0] == 0xEB && header.ID[1] == 0x3C	//BBK
		&& header.ID[2] == 0x90 && header.ID[3] == 0x53 ) {
		return	0;
	} else {
		LPBYTE	temp = NULL;
		LONG	size;
		if( !UnCompress( fname, &temp, (LPDWORD)&size ) )
			return	IDS_ERROR_UNSUPPORTFORMAT;

		memcpy( &header, temp, sizeof(NESHEADER) );
		header.CHR_PAGE_SIZE = header.CHR_PAGE_SIZE_old;
		header.PRG_PAGE_SIZE = header.PRG_PAGE_SIZE_old;
		FREE( temp );
		if( header.ID[0] == 'N' && header.ID[1] == 'E'
		 && header.ID[2] == 'S' && header.ID[3] == 0x1A ) {
			for( INT i = 0; i < 4; i++ ) {
				if( header.reserved[i] )
					return	IDS_ERROR_ILLEGALHEADER;
			}
			return	0;
		} else if( header.ID[0] == 'U' && header.ID[1] == 'N'
			&& header.ID[2] == 'I' && header.ID[3] == 'F' ) {
			return	0;
		} else if( header.ID[0] == 'F' && header.ID[1] == 'D'
			&& header.ID[2] == 'S' && header.ID[3] == 0x1A ) {
			return	0;
		} else if( header.ID[0] == 'N' && header.ID[1] == 'E'
			&& header.ID[2] == 'S' && header.ID[3] == 'M') {
			return	0;
		} else if( header.ID[0] == 0xEB && header.ID[1] == 0x3C	//BBK
			&& header.ID[2] == 0x90 && header.ID[3] == 0x53 ) {
			return	0;
		}
	}

	return	IDS_ERROR_UNSUPPORTFORMAT;
}

//
// ROM僼傽僀儖柤偺僠僃僢僋(PAL傪帺摦敾暿)
//
void	ROM::FilenameCheck( const char* fname )
{
	unsigned char*	p = (unsigned char*)fname;

	while( *p != (unsigned char)'\0' ) {
		if( *p == (unsigned char)'(' ) {
			if( _mbsnbicmp( p, (unsigned char*)"(E)", 3 ) == 0 ) {
				bPAL = TRUE;
				return;
			}
		}

		p = _mbsinc(p);
	}
}

