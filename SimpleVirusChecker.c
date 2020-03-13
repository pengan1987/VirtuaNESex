/*

 SimpleVirusChecker
 version 1.1
 programmed by Norix

 Copyright Note
 source code: SimpleVirusChecker.c,SimpleVirusChecker.h

 Please use this program freely.
 An author isn't concerned on a loss at having used this program at all.
 An author prays for virus software disappearing from a PC in the world.

 --Attention--
 Because an author doesn't have a PC infected with a virus program, can't
 guarantee it whether you were able to completely check it.

 Histroy:
 v1.1	add WORM_FIZZER
	add WORM_PALYH
	add WORM_LOVGATE
	add WORM_OPASERV
 v1.11	add WORM_SOBIG.F
*/
#include	"SimpleVirusChecker.h"
#include	<tchar.h>

#define	KEYTYPE_1	(1<<0)	/* Windows startup registry */
#define	KEYTYPE_2	(1<<1)	/* Windows Servece registry(NT only) */

typedef	struct	{
	TCHAR*	keyname;
	int	type;		/* type */
	int	length;		/* use strncmp length (0 is use strcmp) */
} VIRUSKEYTBL;

static	VIRUSKEYTBL	viruskeytbl[] = {
/*	KEY				KEYTYPE			LENGTH */
	_T("Wink"),			KEYTYPE_1|KEYTYPE_2,	4,	/* Klez */
	_T("WindowsMGM"),		KEYTYPE_1|KEYTYPE_2,	0,	/* Sobig */
	_T("Avril Lavigne - Muse"),	KEYTYPE_1,		0,	/* Lirva */
	_T("SystemInit"),		KEYTYPE_1,		0,	/* Fizzer */
	_T("System Tray"),		KEYTYPE_1,		0,	/* Palyh */
	_T("WinGate initialize"),	KEYTYPE_1,		0,	/* Lovgate */
	_T("ScrSvr"),			KEYTYPE_1,		0,	/* Opaserv */
	_T("SSK Service"),		KEYTYPE_1,		0,	/* Sobig.E */
	_T("TrayX"),			KEYTYPE_1,		0,	/* Sobig.F */
	NULL, 0, 0	/* term */
};

/* WindowsNT? */
static	int	IsNT( void )
{
	OSVERSIONINFO	osvi;
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	if( !GetVersionEx(&osvi) )
		return	0;

	if( osvi.dwPlatformId == VER_PLATFORM_WIN32_NT )
		return	1;

	if( osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
		return	0;

	/* unknown */
	return	-1;
}

/* A check of a type to be infected with *.exe activation (representative example:SIRCAM) */
static	int	ExeRegistryCheck( void )
{
	HKEY	hKey;
	LONG	lResult;
	DWORD	dwCount;

	if( RegOpenKeyEx( HKEY_CLASSES_ROOT, _T("exefile\\shell\\open\\command"),
			0, KEY_READ, &hKey ) != ERROR_SUCCESS ) {
		return	-1;
	}

	lResult = RegQueryValueEx( hKey, _T(""), NULL, NULL, NULL, &dwCount );
	RegCloseKey( hKey );
	if( lResult != ERROR_SUCCESS ) {
		return	-1;
	}
	/*
	  Check it whether a parameter is different from a default parameter.
	  May think that this is originally infected when it is only '"%1" %*'
	  and has length more than it.
	*/
	if( dwCount > 8 ) {
		return	1;
	}
	/* OK */
	return	0;
}

/* Character string comparison */
static	int	KlezTypeCheckSub( const char* name, int mask )
{
	VIRUSKEYTBL*	tbl = viruskeytbl;

	while( tbl->keyname ) {
		if( tbl->type & mask ) {
			if( !tbl->length ) {
				if( StrCmpI( name, tbl->keyname ) == 0 )
					return	1;
			} else {
				if( StrCmpNI( name, tbl->keyname, tbl->length ) == 0 )
					return	1;
			}
		}
		tbl++;
	}
	return	0;
}

/* A check of the registry which a virus uses */
static	int	KlezTypeCheck( void )
{
	HKEY	hKey;
	LONG	lResult;
	TCHAR	szKeyName[1024];
	DWORD	dwIndex, dwCount;
	FILETIME ft;
	int	viruscount = 0;

	/* Search a thing registered with Windows Startup */
	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),
			    0, KEY_READ, &hKey ) != ERROR_SUCCESS ) {
		return	-1;
	}
	dwIndex = 0;
	while( 1 ) {
		dwCount = sizeof(szKeyName)/sizeof(TCHAR);
		lResult = RegEnumValue( hKey, dwIndex, szKeyName, &dwCount, 0, NULL, NULL, NULL );
		if( lResult != ERROR_SUCCESS ) {
			break;
		}
		viruscount += KlezTypeCheckSub( szKeyName, KEYTYPE_1 );
		dwIndex++;
	}
	RegCloseKey( hKey );

	if( !IsNT() ) {
		return	viruscount;
	}

	/* Search a thing registered with WindowsNT Service */
	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T("System\\CurrentControlSet\\Services"),
			    0, KEY_READ, &hKey ) != ERROR_SUCCESS ) {
		return	-1;
	}
	dwIndex = 0;
	while( 1 ) {
		dwCount = sizeof(szKeyName)/sizeof(TCHAR);
		lResult = RegEnumKeyEx( hKey, dwIndex, szKeyName, &dwCount, 0, NULL, NULL, &ft );
		if( lResult != ERROR_SUCCESS ) {
			break;
		}
		viruscount += KlezTypeCheckSub( szKeyName, KEYTYPE_2 );
		dwIndex++;
	}
	RegCloseKey( hKey );

	return	viruscount;
}

int	SimpleVirusChecker(void)
{
	int	viruscount = 0;

	if( IsNT() < 0 )
		return	-1;

	if( ExeRegistryCheck() > 0 )
		viruscount++;

	viruscount += KlezTypeCheck();

	return	viruscount;
}

