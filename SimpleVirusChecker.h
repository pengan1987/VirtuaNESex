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
#ifndef	__EASYVIRUSCHECKER__
#define	__EASYVIRUSCHECKER__

#include	<windows.h>
#include	<shlwapi.h>

#pragma comment( lib, "shlwapi.lib" )

#if	defined(__cplusplus)
extern	"C"
{
#endif

/*
 ret==0	Don't exist in registry.
 ret>0	The number of hit of the registry which a virus uses.
*/
int	SimpleVirusChecker(void);

#if	defined(__cplusplus)
}
#endif
#endif	/* __EASYVIRUSCHECKER__ */


